#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include "wlr-screencopy.h"
#include <wayland-client.h>
#include <errno.h>
#include <time.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "screenshot.h"
#include "frame_handler.h"
#include "globals.c"
#include "frame_handlers.c"

static void registry_global_handler(void *data, struct wl_registry *registry, uint32_t name, const char *interface, uint32_t version) {
    /*
        this function is used to register the swh output screencopy(wlroots) interfaces
    */

    if (strcmp(interface, wl_shm_interface.name) == 0) {

        shm = wl_registry_bind(registry, name, &wl_shm_interface, 1);
        printf("Found the interface %s with name :%d version :%d\n", interface, name, version);
    }
    if (strcmp(interface, wl_output_interface.name) == 0) {

        output = wl_registry_bind(registry, name, &wl_output_interface, 2);
        printf("Found the interface %s with name :%d version :%d\n", interface, name, version);
    }
    if (strcmp(interface, zwlr_screencopy_manager_v1_interface.name) == 0) {

        screencopy = wl_registry_bind(registry, name, &zwlr_screencopy_manager_v1_interface, 1);
        printf("Found the interface %s with name :%d version :%d\n", interface, name, version);
    }

}

static void registry_global_handler_remove(void *data, struct wl_registry *registry, uint32_t name) {/* no-op */}


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


    //register registry event handler
    wl_registry_add_listener(registry, &registry_listener, NULL);
    wl_display_roundtrip(display);
    
    if (!shm || !output || !screencopy) {

        printf("Required interfaces not available\n");
        return -1;
    }

    //register frame event handler
    struct zwlr_screencopy_frame_v1 *frame = zwlr_screencopy_manager_v1_capture_output(screencopy, 1, output);
    zwlr_screencopy_frame_v1_add_listener(frame, &frame_listener, NULL);

    // start the dispatch event loop
    while (1) {wl_display_dispatch(display);}

    return 0;
}
