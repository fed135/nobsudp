#include <node_api.h>
#include <assert.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <memory.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <errno.h>
#include <stdlib.h>
#include <iostream>

// Based on https://stackoverflow.com/a/24560310/1005421

int resolvehelper(const char* hostname, int family, const char* service, sockaddr_storage* pAddr)
{
    int result;
    addrinfo* result_list = NULL;
    addrinfo hints = {};
    hints.ai_family = family;
    hints.ai_socktype = SOCK_DGRAM;
    result = getaddrinfo(hostname, service, &hints, &result_list);
    if (result == 0) {
        memcpy(pAddr, result_list->ai_addr, result_list->ai_addrlen);
        freeaddrinfo(result_list);
    }

    return result;
}

napi_value Send(napi_env env, napi_callback_info info) {
    napi_value argv[3];
    size_t argc = 3;
    napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

    if (argc < 3) {
      napi_throw_error(env, "EINVAL", "Expected arguments: (buffer: Buffer, host: string, port: string) mismatch!");
      return NULL;
    }

    size_t size = 0;
    char* buffer;
    napi_get_buffer_info(env, argv[0], (void**) buffer, &size);

    size_t hostLength = 0;
    char host[15];
    napi_get_value_string_utf8(env, argv[1], host, 15, &hostLength);

    size_t portLength = 0;
    char port[5];
    napi_get_value_string_utf8(env, argv[2], port, 5, &portLength);

    //std::cout << "size " << size << std::endl;
    //std::cout << "host " << host << std::endl;
    //std::cout << "port " << port << std::endl;

    // Ideally, set return value here

    int opResult = 0;
    int sock = socket(AF_INET, SOCK_DGRAM, 0);

    sockaddr_storage addrDest = {};
    opResult = resolvehelper(host, AF_INET, port, &addrDest);
    if (opResult != 0) {
       int lasterror = errno;
       std::cout << "error: " << lasterror;
       return NULL;
    }

    opResult = sendto(sock, buffer, size, 0, (sockaddr*)&addrDest, sizeof(addrDest));

    //std::cout << opResult << " bytes sent" << std::endl;
    return NULL;    
}

napi_value init_all (napi_env env, napi_value exports) {
  napi_value send_fn;
  napi_create_function(env, NULL, 0, Send, NULL, &send_fn);
  napi_set_named_property(env, exports, "send", send_fn);
  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, init_all)