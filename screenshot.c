#include <stdio.h>
#include <string.h>
#include <wayland-client.h>
#include <errno.h>
#include "wlr-screencopy.h"

struct wl_shm *shm = NULL;
struct wl_output *output = NULL;
struct zwlr_screencopy_manager_v1 *screencopy = NULL;


static void registry_global_handler(void *data, struct wl_registry *registry, uint32_t name, const char *interface, uint32_t version) {
    /*
        this function is used to register the swh output screencopy(wlroots) interfaces
    */

    if (strcmp(interface, wl_shm_interface.name) == 0) {

        shm = wl_registry_bind(registry, name, &wl_shm_interface, 1);
        printf("Found the interface %s with name :%d version :%d\n", interface, name);
    }
    if (strcmp(interface, wl_output_interface.name) == 0) {

        output = wl_registry_bind(registry, name, &wl_output_interface, 2);
        printf("Found the interface %s with name :%d version :%d\n", interface, name);
    }
    if (strcmp(interface, zwlr_screencopy_manager_v1_interface.name) == 0) {

        screencopy = wl_registry_bind(registry, name, &zwlr_screencopy_manager_v1_interface, 1);
        printf("Found the interface %s with name :%d version :%d\n", interface, name);
    }

}

static void registry_global_handler_remove(void *data, struct wl_registry *registry, uint32_t name) {

    /* no-op */
}

int main(int argc, char *argv[]) {

    struct wl_display *display = wl_display_connect(NULL);
    if (&display == NULL) {

        int errval = errno;
        printf("Failed to get display value %d\n", errval); 
        return -1;
    }

    struct wl_registry *registry = wl_display_get_registry(display);
    if (registry == NULL) {

        int errval = errno;
        printf("Failed to get display value %d\n", errval); 
        return -1;
    }

    struct wl_registry_listener registry_listener = {

        .global =  registry_global_handler, 
        .global_remove = registry_global_handler_remove,
    };

    wl_registry_add_listener(registry, &registry_listener, NULL);
    
    while(1) {

        wl_display_dispatch(display);
    }
    return 0;
}
