#include <stdio.h>
#include <emu.h>
#include <cart.h>
#include <cpu.h>
#include <ui.h>
#include <pthread.h>
#include <unistd.h>

/* 
  Emu components:

  |Cart|
  |CPU|
  |Address Bus|
  |PPU|
  |Timer|

*/

static emu_context ctx;

emu_context *emu_get_context() {
    return &ctx;
}

void *cpu_run(void *p) {
    cpu_init();

    ctx.running = true;
    ctx.paused = false;
    ctx.ticks = 0;

    while(ctx.running) {
        if (ctx.paused) {
            delay(10);
            continue;
        }

        if (!cpu_step()) {
            printf("CPU Stopped\n");
            return 0;
        }

        ctx.ticks++;
    }

    return 0;
}

int emu_run(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: emu <rom_file>\n");
        return -1;
    }

    if (!cart_load(argv[1])) {
        printf("Failed to load ROM file: %s\n", argv[1]);
        return -2;
    }

    printf("Cart loaded..\n");

    ui_init();

    pthread_t t1;

    if (pthread_create(&t1, NULL, cpu_run, NULL)) {
        fprintf(stderr, "Couldn't create CPU thread. Aborting...\n");
        return -1;
    }

    while (!ctx.die) {
        usleep(1000);
        ui_handle_events();
    }

    return 0;
}

void emu_cycles(int cpu_cycles) {
    // TODO
}
