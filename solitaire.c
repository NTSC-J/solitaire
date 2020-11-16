#include <gsKit.h>
#include <dmaKit.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <gsToolkit.h>
#include <math.h>

#define max(a, b) ((a) > (b) ? (a) : (b))

float frand()
{
    return (float)rand() / RAND_MAX;
}

int main(int argc, char *argv[])
{
    GSGLOBAL *gsGlobal = gsKit_init_global();
    GSTEXTURE tex, bgTex;

    gsGlobal->Mode = GS_MODE_NTSC;
    gsGlobal->Interlace = GS_INTERLACED;
    gsGlobal->Field = GS_FIELD;
    gsGlobal->Width = 640;
    gsGlobal->Height = 448;

    gsGlobal->PrimAlphaEnable = GS_SETTING_ON;
    gsGlobal->PSM = GS_PSM_CT24;
    gsGlobal->PSMZ = GS_PSMZ_16S;

    // デプステストをしない
    gsGlobal->ZBuffering = GS_SETTING_OFF;
    gsKit_set_test(gsGlobal, GS_ZTEST_OFF);

    // DMAの何か
    dmaKit_init(D_CTRL_RELE_OFF, D_CTRL_MFD_OFF, D_CTRL_STS_UNSPEC, D_CTRL_STD_OFF, D_CTRL_RCYC_8, 1 << DMA_CHANNEL_GIF);
    dmaKit_chan_init(DMA_CHANNEL_GIF);

    gsKit_init_screen(gsGlobal);
    gsKit_mode_switch(gsGlobal, GS_ONESHOT); // glHint的なやつ
    
    u64 TexCol = GS_SETREG_RGBAQ(0x80, 0x80, 0x80, 0x80, 0x00); // 何だかわからない

    gsKit_texture_png(gsGlobal, &bgTex, "host:background.png");
    gsKit_texture_png(gsGlobal, &tex, "host:img.png");
    int w = tex.Width * 0.6;
    int h = tex.Height * 0.6;

    gsKit_set_clamp(gsGlobal, GS_CMODE_CLAMP); // テクスチャをクランプする

    // しましまスプライト
    gsKit_prim_sprite_striped_texture(gsGlobal, &bgTex, 0, 0, 0, 0, gsGlobal->Width, gsGlobal->Height, bgTex.Width, bgTex.Height, 2, TexCol);

    float t = 0.0;
    while(1) {
        //gsKit_clear(gsGlobal, GS_SETREG_RGBAQ(0x3F, 0x3F, 0x3F, 0x00, 0x00));
        float x = tex.Width * frand();
        float dx = (1.8 + 2.0 * frand()) * (frand() < 0.5 ? -1 : 1);
        float a = 1.0;
        t += M_PI * frand();
        while (0 < x + w && x < gsGlobal->Width) {
            float y = gsGlobal->Height - h - a * fabs(sin(t)) * gsGlobal->Height;
            gsKit_prim_sprite_texture(gsGlobal, &tex, x, y, 0, 0, x + w, y + h, tex.Width, tex.Height, 2, TexCol);
            t += 0.05;
            if (0.0 < a)
                a = max(0.0, a - 0.002);
            x += dx;
            gsKit_sync_flip(gsGlobal);
            gsKit_queue_exec(gsGlobal);
        }
    }

    return 0;
}

