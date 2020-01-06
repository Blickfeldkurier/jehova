#include "avahiservice.h"

AvahiService::AvahiService(COpts opts, std::string msg)
{
    this->data.port = std::stoi(opts.port, nullptr, 0);
    if(msg.empty() == true){
        this->data.msg = nullptr;
    }else{
        this->data.msg = avahi_string_list_new(msg.c_str(), NULL);
    }
    int rc = pthread_create(&this->thread, NULL, AvahiService::publishAvahiService, &this->data);
    if(rc != 0){
        std::cout << "Could not create pthread: " << std::to_string(rc) << std::endl;
    }
}

AvahiService::~AvahiService(){
    avahi_simple_poll_free(this->data.poll);
}

void AvahiService::entry_group_callback(AvahiEntryGroup *_group, 
                                        AvahiEntryGroupState _state, 
                                        void *userdata)
{
    AvahiService::AvahiData *data = static_cast<AvahiService::AvahiData*>(userdata);
    data->group = _group;
    switch(_state){
        case AVAHI_ENTRY_GROUP_FAILURE:{
					std::cerr << "Avahi Entry Group Failure: " 
                              << avahi_strerror(avahi_client_errno(avahi_entry_group_get_client(_group))) << std::endl;
					avahi_simple_poll_quit(data->poll);
        }break;

		case AVAHI_ENTRY_GROUP_COLLISION :
		case AVAHI_ENTRY_GROUP_ESTABLISHED :
		case AVAHI_ENTRY_GROUP_UNCOMMITED:
		case AVAHI_ENTRY_GROUP_REGISTERING:{}
		break;
    }
}

void AvahiService::create_services(AvahiClient *_client, AvahiService::AvahiData *data){
			if ((!data->group)&&(!(data->group = avahi_entry_group_new(_client, AvahiService::entry_group_callback, data))))
			{
				std::cout << "avahi_entry_group_new() failed: "
                          << avahi_strerror(avahi_client_errno(_client)) << std::endl;
				avahi_simple_poll_quit(data->poll);
			}

			if (avahi_entry_group_is_empty(data->group))
			{
				auto ret = avahi_entry_group_add_service_strlst(data->group,
                                                         AVAHI_IF_UNSPEC, 
                                                         AVAHI_PROTO_UNSPEC, 
                                                         static_cast<AvahiPublishFlags>(0), 
                                                         "Jehova", 
                                                         "_pixelflut._tcp", 
                                                         NULL, NULL, 
                                                         data->port, data->msg);
				if (ret < 0)
				{
					if (ret == AVAHI_ERR_COLLISION){
						create_services(_client, data);
					}
					std::cerr << "Failed to add _ipp._tcp service:" << avahi_strerror(ret) << std::endl;
					avahi_simple_poll_quit(data->poll);
				}
				if ((ret = avahi_entry_group_commit(data->group)) < 0) {
					std::cerr << "Failed to commit entry group: " << avahi_strerror(ret) << std::endl;
					avahi_simple_poll_quit(data->poll);
				}
			}
}

void AvahiService::client_callback(AvahiClient *_client,
                                   AvahiClientState _state,
                                   void *userdata)
{
    AvahiService::AvahiData *data = static_cast<AvahiService::AvahiData*>(userdata);
    switch(_state){
        case AVAHI_CLIENT_S_RUNNING:{
            AvahiService::create_services(_client, data);
        }break;
        case AVAHI_CLIENT_FAILURE:{
            std::cout << "Avahi Client Failure: " 
                      <<  avahi_strerror(avahi_client_errno(_client)) << std::endl;
             avahi_simple_poll_quit(data->poll);
        }break;
        case AVAHI_CLIENT_S_COLLISION:
		case AVAHI_CLIENT_S_REGISTERING:{
		    if (data->group != nullptr){
                avahi_entry_group_reset(data->group);
            }
        }break;
        case AVAHI_CLIENT_CONNECTING:{}break;
    }

}

void *AvahiService::publishAvahiService(void *userdata){
    AvahiService::AvahiData *data = static_cast<AvahiService::AvahiData*>(userdata);
    data->poll = avahi_simple_poll_new();
    if(data->poll == nullptr){
        std::cout << "Could not Avahi Simple Poll Object\n";
        pthread_exit(nullptr);
    }
    int error;
    AvahiClient *client = avahi_client_new(avahi_simple_poll_get(data->poll), 
                                           static_cast<AvahiClientFlags>(0),
                                           AvahiService::client_callback, 
                                           data,  
                                           &error);
    if(client == nullptr){
        std::cout << "Failed to create Avahi Client Object: " 
                  << avahi_strerror(error) << std::endl;
        avahi_simple_poll_free(data->poll);
        pthread_exit(nullptr);
    }
    avahi_simple_poll_loop(data->poll);
	avahi_client_free(client);
    pthread_exit(nullptr);
}