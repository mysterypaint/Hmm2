#include "Graphics.hpp"
#include "../PHL.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Resources.hpp"

PHL_Surface db = {0};
PHL_Surface backBuffer = {0};

PHL_Surface dbAlt = {0};
PHL_Surface backBufferAlt = {0};
const int cWidth = 320;
const int cHeight = 240;

Screen scrnTop = {
    GFX_TOP,
    GFX_LEFT,
    400,
    240
};

Screen scrnBottom = {
    GFX_BOTTOM,
    GFX_LEFT,
    cWidth,
    cHeight
};

Screen* activeScreen = nullptr;       // Where graphics get rendered to
#ifdef _3DS
    Screen* debugScreen = nullptr;    // Bottom screen console, for debugging (or swapping screen locations)
#endif
        
PHL_Rect offset;

void PHL_GraphicsInit() {
    gfxInitDefault();
    
    //Initialize console on top screen. Using NULL as the second argument tells the console library to use the internal console structure as current one
    //gfxSet3D(false);
    //consoleDebugInit(debugDevice_CONSOLE);

    activeScreen = &scrnBottom;
    debugScreen = &scrnTop;
    PHL_ResetDrawbuffer();

    //Create background image
    backBuffer = PHL_NewSurface(cWidth * 2, cHeight * 2);
    dbAlt = PHL_NewSurface(cWidth * 2, cHeight * 2);
    backBufferAlt = PHL_NewSurface(cWidth * 2, cHeight * 2);
}

void PHL_GraphicsExit() {
    gfxExit();
}

void PHL_StartDrawing() {
    PHL_ResetDrawbuffer();
    gfxFlushBuffers();
}

void PHL_EndDrawing() {
    PHL_DrawOtherScreen();
    gfxSwapBuffers();
    gspWaitForVBlank();
}

void PHL_SetDrawbuffer(PHL_Surface surf) {
    db = surf;
    offset.w = db.width;
    offset.h = db.height;
    offset.x = 0;
    offset.y = 0;
}

void PHL_ResetDrawbuffer() {
    db.width = activeScreen->width;
    db.height = activeScreen->height;
    db.pxdata = gfxGetFramebuffer(activeScreen->screen, activeScreen->side, NULL, NULL);

    offset.w = cWidth;
    offset.h = cHeight;
    offset.x = (activeScreen->width - offset.w) / 2;
    offset.y = (activeScreen->height - offset.h) / 2;
}

PHL_RGB PHL_NewRGB(uint8_t r, uint8_t g, uint8_t b) {
    PHL_RGB c = { r, g, b };
    return c;
}

void PHL_SetColorKey(PHL_Surface surf, uint8_t r, uint8_t g, uint8_t b) {
    PHL_RGB key = { r, g, b };
    surf.colorKey = key;
}

PHL_Surface PHL_NewSurface(uint16_t w, uint16_t h) {
    PHL_Surface surf;

    surf.width = w / 2;
    surf.height = h / 2;
    surf.pxdata = (uint8_t *) malloc(surf.width * surf.height * 3 * sizeof(uint8_t));
    surf.colorKey = PHL_NewRGB(0xFF, 0x00, 0xFF);

    return surf;
}

void PHL_FreeSurface(PHL_Surface surf) {
    if (surf.pxdata != NULL) {
        free(surf.pxdata);
        surf.pxdata = NULL;
    }
}

PHL_Surface PHL_LoadTexture(int _img_index) {
    PHL_Surface surf;

    std::string _f_in = "romfs:/graphics/";

    int fileSize = 77880; // The default filesize, in bytes (because nearly all the graphics in the game are this size)

    switch(_img_index) {
        case sprPlayer:
            _f_in += "test_player.bmp";
            fileSize = 12342;
            break;
        case sprTile0:
            _f_in += "tile0.bmp";
            fileSize = 1142;
            break;
        case sprProt1:
            _f_in += "prot1.bmp";
            break;
        case sprTitle:
            _f_in += "title.bmp";
            break;
        case sprMapG00:
            _f_in += "mapg00.bmp";
            break;
        case sprMapG01:
            _f_in += "mapg01.bmp";
            break;
        case sprMapG02:
            _f_in += "mapg02.bmp";
            break;
        case sprMapG03:
            _f_in += "mapg03.bmp";
            break;
        case sprMapG04:
            _f_in += "mapg04.bmp";
            break;
        case sprMapG05:
            _f_in += "mapg05.bmp";
            break;
        case sprMapG06:
            _f_in += "mapg06.bmp";
            break;
        case sprMapG07:
            _f_in += "mapg07.bmp";
            break;
        case sprMapG08:
            _f_in += "mapg08.bmp";
            break;
        case sprMapG09:
            _f_in += "mapg09.bmp";
            break;
        case sprMapG10:
            _f_in += "mapg10.bmp";
            break;
        case sprMapG11:
            _f_in += "mapg11.bmp";
            break;
        case sprMapG12:
            _f_in += "mapg12.bmp";
            break;
        case sprMapG13:
            _f_in += "mapg13.bmp";
            break;
        case sprMapG14:
            _f_in += "mapg14.bmp";
            break;
        case sprMapG15:
            _f_in += "mapg15.bmp";
            break;
        case sprMapG16:
            _f_in += "mapg16.bmp";
            break;
        case sprMapG17:
            _f_in += "mapg17.bmp";
            break;
        case sprMapG18:
            _f_in += "mapg18.bmp";
            break;
        case sprMapG19:
            _f_in += "mapg19.bmp";
            break;
        case sprMapG20:
            _f_in += "mapg20.bmp";
            break;
        case sprMapG21:
            _f_in += "mapg21.bmp";
            break;
        case sprMapG22:
            _f_in += "mapg22.bmp";
            break;
        case sprMapG31:
            _f_in += "mapg31.bmp";
            break;
        case sprMapG32:
            _f_in += "mapg32.bmp";
            break;
        default:
            fileSize = 77880;
    }


    FILE * f;
    if ((f = fopen(_f_in.c_str(), "rb"))) {
        //Save bmp data
        uint8_t* bmpFile = (uint8_t*) malloc(fileSize * sizeof(uint8_t));

        fread(bmpFile, fileSize, 1, f);
        fclose(f);
        //Create surface
        uint16_t w, h;
        memcpy(&w, &bmpFile[18], 2);
        memcpy(&h, &bmpFile[22], 2);

        surf = PHL_NewSurface(w * 2, h * 2);

        //Load Palette
        PHL_RGB palette[20][18];

        int count = 0;
        for (int dx = 0; dx < 20; dx++) {
            for (int dy = 0; dy < 16; dy++) {
                palette[dx][dy].b = bmpFile[54 + count];
                palette[dx][dy].g = bmpFile[54 + count + 1];
                palette[dx][dy].r = bmpFile[54 + count + 2];

                count += 4;
            }
        }

        //Fill pixels
        count = 0;
        for (int dx = w; dx > 0; dx--) {
            for (int dy = 0; dy < h; dy++) {
                int pix = w - dx + w * dy;
                int px = bmpFile[1078 + pix] / 16;
                int py = bmpFile[1078 + pix] % 16;
                
                //Get transparency from first palette color
                if (dx == w &&dy == 0)          
                    surf.colorKey = palette[0][0];
                
                PHL_RGB c = palette[px][py];
                surf.pxdata[count]   = c.b;
                surf.pxdata[count+1] = c.g;
                surf.pxdata[count+2] = c.r;
                count += 3;
            }
        }
        
        //Cleanup
        free(bmpFile);
    }
    return surf;
}

void PHL_DrawRect(int16_t x, int16_t y, uint16_t w, uint16_t h, PHL_RGB col) {
    // Everything is stored in memory at 2x size; Halve it for the 3ds port
    if (x < 0 || y < 0 || x+w > db.width || y+h > db.height)
        return;
    //Shrink values for small 3ds screen
    //x /= 2; 
    //y /= 2;
    
    x += offset.x;
    y += offset.y;
    
    //w /= 2;
    //h /= 2;
    
    s16 x2 = x + w;
    s16 y2 = y + h;
    
    //Keep drawing within screen
    if (x < offset.x) { x = offset.x; }
    if (y < offset.y) { y = offset.y; }
    if (x2 > offset.x + offset.w) { x2 = offset.x + offset.w; }
    if (y2 > offset.y + offset.h) { y2 = offset.y + offset.h; }
    
    w = x2 - x;
    h = y2 - y;
    
    u32 p = ((db.height - h - y) + (x * db.height)) * 3;
    
    for (int i = 0; i < w; i++)
    {
        for (int a = 0; a < h; a++)
        {           
            db.pxdata[p] = col.b;
            db.pxdata[p+1] = col.g;
            db.pxdata[p+2] = col.r;
            
            p += 3;
        }
        p += (db.height - h) * 3;
    }
}

void PHL_DrawSurface(int16_t x, int16_t y, PHL_Surface surf) {
    PHL_DrawSurfacePart(x, y, 0, 0, surf.width * 2, surf.height * 2, surf);
}

void PHL_DrawSurfacePart(int16_t x, int16_t y, int16_t cropx, int16_t cropy, int16_t cropw, int16_t croph, PHL_Surface surf) {
    if (surf.pxdata != NULL) {

        /*
        // Everything is stored in memory at 2x size; Halve it for the 3ds port
        x = (int) x / 2;
        y = (int) y / 2;

        cropx = cropx / 2;
        cropy = cropy / 2;
        cropw /= 2;
        croph /= 2;
        */
        
        if (x > offset.w || y > offset.h || x + cropw < 0 || y + croph < 0) {
            //image is outside of screen, so don't bother drawing
        } else {
            //Crop pixels that are outside of screen
            if (x < 0) {
                cropx += -(x);
                cropw -= -(x);
                x = 0;
            }
            if (y < 0) {
                cropy += -(y);
                croph -= -(y);
                y = 0;
            }

            //3DS exclusive optimization
            /*
            //if (roomDarkness == 1) {
                //if (1) {
                int cornerX = 0;// (herox / 2) - 80;
                int cornerY = 0;// (heroy / 2) + 10 - 80;

                if (x < cornerX) {
                    cropx += cornerX - x;
                    cropw -= cornerX - x;
                    x = cornerX;
                }
                if (y < cornerY) {
                    cropy += cornerY - y;
                    croph -= cornerY - y;
                    y = cornerY;
                }
                if (x + cropw > cornerX + 160) {
                    cropw -= (x + cropw) - (cornerX + 160);
                }
                if (y + croph > cornerY + 160) {
                    croph -= (y + croph) - (cornerY + 160);
                }
            //}*/

            if (x + cropw > offset.w)
                cropw -= (x + cropw) - (offset.w);
            if (y + croph > offset.h)
                croph -= (y + croph) - (offset.h);

            // Adjust the canvas' position based on the new offsets
            x += offset.x;
            y += offset.y;


            // Find the first color and pixel that we're dealing with before we update the rest of the canvas
            uint32_t p = ((offset.h - croph - y) + (x * offset.h)) * 3;
            uint32_t c = ((surf.height - cropy - croph) + surf.height * cropx) * 3;

            // Loop through every single pixel (draw columns from left to right, top to bottom) of the final output canvas and store the correct color at each pixel
            for (int i = 0; i < cropw; i++) {
                for (int a = 0; a < croph; a++) {
                    if (surf.colorKey.r != surf.pxdata[c + 2] ||
                        surf.colorKey.g != surf.pxdata[c + 1] ||
                        surf.colorKey.b != surf.pxdata[c]) {

                        // Only update this pixel's color if necessary
                        db.pxdata[p] = surf.pxdata[c];
                        db.pxdata[p + 1] = surf.pxdata[c + 1];
                        db.pxdata[p + 2] = surf.pxdata[c + 2];
                    }

                    c += 3;
                    p += 3;
                }

                // Skip drawing for all of the columns of pixels that we've cropped out (one pixel = 3 bytes of data {r,g,b})
                p += (offset.h - croph) * 3;
                c += (surf.height - croph) * 3;
            }
        }
    }
}

void PHL_DrawBackground(PHL_Background back, PHL_Background fore) {
    PHL_DrawSurface(0, 0, backBuffer);
}

void PHL_UpdateBackground(PHL_Background back, PHL_Background fore) {

    PHL_SetDrawbuffer(backBuffer);

    /*
        int xx, yy;

        for (yy = 0; yy < 12; yy++) {
            for (xx = 0; xx < 16; xx++) {
                //Draw Background tiles
                PHL_DrawSurfacePart(xx * 40, yy * 40, back.tileX[xx][yy] * 40, back.tileY[xx][yy] * 40, 40, 40, images[imgTiles]);

                //Only draw foreground tile if not a blank tile
                if (fore.tileX[xx][yy] != 0 || fore.tileY[xx][yy] != 0) {
                    PHL_DrawSurfacePart(xx * 40, yy * 40, fore.tileX[xx][yy] * 40, fore.tileY[xx][yy] * 40, 40, 40, images[imgTiles]);
                }
            }
        }
    */

    PHL_ResetDrawbuffer();
}

//3DS exclusive. Changes which screen to draw on
void swapScreen(gfxScreen_t screen, gfx3dSide_t side) {
    //Clear old screen
    PHL_StartDrawing();
    PHL_DrawRect(0, 0, 640, 480, PHL_NewRGB(0, 0, 0));
    PHL_EndDrawing();
    PHL_StartDrawing();
    PHL_DrawRect(0, 0, 640, 480, PHL_NewRGB(0, 0, 0));
    PHL_EndDrawing();

    if (screen == GFX_TOP) {
        activeScreen = &scrnTop;
        debugScreen = &scrnBottom;
    } else {
        activeScreen = &scrnBottom;
        debugScreen = &scrnTop;
    }

    PHL_ResetDrawbuffer();
}

void PHL_DrawOtherScreen() {
    PHL_ResetAltDrawbuffer();
    //printf(":wagu: :nodding: :nodding2: :slownod: :hypernodding: :wagu2: :oj100: :revolving_hearts: ");
}

void PHL_ResetAltDrawbuffer() {
    dbAlt.width = debugScreen->width;
    dbAlt.height = debugScreen->height;
    dbAlt.pxdata = gfxGetFramebuffer(debugScreen->screen, debugScreen->side, NULL, NULL);

    offset.w = cWidth;
    offset.h = cHeight;
    offset.x = (debugScreen->width - offset.w) / 2;
    offset.y = (debugScreen->height - offset.h) / 2;
}