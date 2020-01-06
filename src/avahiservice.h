#ifndef AVAHISERVICE_H
#define AVAHISERVICE_H

#include <iostream>
#include <pthread.h>

#include <avahi-client/client.h>
#include <avahi-client/publish.h>
#include <avahi-common/alternative.h>
#include <avahi-common/simple-watch.h>
#include <avahi-common/malloc.h>
#include <avahi-common/error.h>
#include <avahi-common/timeval.h>

#include "copts.h"

class AvahiService
{
public:
    struct AvahiData{
        int port;
        AvahiEntryGroup *group;
        AvahiSimplePoll *poll;
        AvahiStringList *msg;
    };
private:
    AvahiData data;
    pthread_t thread;
public:
    AvahiService(COpts opts, std::string msg);

    ~AvahiService();
    static void *publishAvahiService(void *userdata);
    static void entry_group_callback(AvahiEntryGroup *_group, 
                                     AvahiEntryGroupState _state, 
                                     void *userdata);
	static void create_services(AvahiClient *_client, AvahiData *data);
	static void client_callback(AvahiClient *_client,
                                AvahiClientState _state, 
                                void *userdata);
};

#endif // AVAHISERVICE_H
