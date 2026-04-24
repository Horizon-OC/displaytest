#include <switch.h>

#define FB_WIDTH  1280
#define FB_HEIGHT 720
#define BLOCK_SIZE 3

int main(int argc, char **argv)
{
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);
    PadState pad;
    padInitializeDefault(&pad);

    Framebuffer fb;
    framebufferCreate(&fb, nwindowGetDefault(), FB_WIDTH, FB_HEIGHT, PIXEL_FORMAT_RGBA_8888, 2);
    framebufferMakeLinear(&fb);
    
    // for some reason there's less cpu load when looping the framebuffer, idk why, but it works!
    while (appletMainLoop())
    {
        padUpdate(&pad);
        u64 kDown = padGetButtonsDown(&pad);
        if (kDown & HidNpadButton_Plus) break;

        u32 stride;
        u32 *framebuf = (u32 *)framebufferBegin(&fb, &stride);

        for (u32 y = 0; y < FB_HEIGHT; y++)
        {
            for (u32 x = 0; x < FB_WIDTH; x++)
            {
                u32 cellX = x / BLOCK_SIZE;
                u32 cellY = y / BLOCK_SIZE;
                if ((cellX + cellY) % 2 == 0)
                    framebuf[y * (stride / sizeof(u32)) + x] = 0xFFFFFFFF;
                else
                    framebuf[y * (stride / sizeof(u32)) + x] = 0xFF000000;
            }
        }

        framebufferEnd(&fb);
    }

    framebufferClose(&fb);
    return 0;
}