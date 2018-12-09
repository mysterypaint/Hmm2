#pragma once

#include <3ds.h>
#include <string>

typedef struct PHL_RGB {
	uint8_t r;
	uint8_t g;	
	uint8_t b;
} PHL_RGB;

typedef struct PHL_Surface {
	uint8_t* pxdata;
	uint16_t width;
	uint16_t height;
	PHL_RGB colorKey;
} PHL_Surface;

typedef struct Screen {
	gfxScreen_t screen;
	gfx3dSide_t side;
	uint16_t width;
	uint16_t height;
} Screen;

typedef struct PHL_Background {
	int tileX[16][12];
	int tileY[16][12];
} PHL_Background;

void PHL_GraphicsInit();
void PHL_GraphicsExit();

void PHL_StartDrawing();
void PHL_EndDrawing();

void PHL_SetDrawbuffer(PHL_Surface surf);
void PHL_ResetDrawbuffer();

void PHL_SetColorKey(PHL_Surface surf, uint8_t r, uint8_t g, uint8_t b);

void PHL_FreeSurface(PHL_Surface surf);
PHL_RGB PHL_NewRGB(uint8_t r, uint8_t g, uint8_t b);
PHL_Surface PHL_NewSurface(uint16_t w, uint16_t h);

PHL_Surface PHL_LoadTexture(int _img_index);
//PHL_Surface PHL_LoadBMP(int index);

void PHL_DrawRect(int16_t x, int16_t y, uint16_t w, uint16_t h, PHL_RGB col);

void PHL_DrawSurface(int16_t x, int16_t y, PHL_Surface surf);
void PHL_DrawSurfacePart(int16_t x, int16_t y, int16_t cropx, int16_t cropy, int16_t cropw, int16_t croph, PHL_Surface surf);

void PHL_DrawBackground(PHL_Background back, PHL_Background fore);
void PHL_UpdateBackground(PHL_Background back, PHL_Background fore);

void swapScreen(gfxScreen_t screen, gfx3dSide_t side);

void PHL_DrawOtherScreen();
void PHL_ResetAltDrawbuffer();





/*

PHL_Surface db;

PHL_Surface backBuffer;


//PHL_RGB PHL_NewRGB(uint8_t r, uint8_t g, uint8_t b);
//void PHL_SetColorKey(PHL_Surface surf, uint8_t r, uint8_t g, uint8_t b);

//PHL_Surface PHL_NewSurface(uint16_t w, uint16_t h);
//void PHL_FreeSurface(PHL_Surface surf);
PHL_Surface PHL_LoadBMP(char* file);


*/