#pragma once
#include "3ds/Graphics.hpp"
#include <vector>

using namespace std;

typedef struct {
	int value;
	bool enableIfFalse;
} InitFlag;

typedef struct {
	bool areaEvent;
	int relativeID = 0;
	int event = -1, x = -1, y = -1, op1 = -1, op2 = -1, op3 = -1, op4 = -1, InitFlagCount = 0;
	InitFlag initFlags[4];
} ObjectSpawnData;

typedef struct {
	int cropX, cropY, w, h, originX, originY;
	PHL_Surface* texGroup;
} SpriteTexture;

typedef struct {
	int world;
	int area;
	int x;
	int y;
} RoomDestination;

typedef struct {
	uint16_t tileID;
	float aniIndex = 0;
} Tile;

typedef struct {
	Tile tileData[704]; // Animated tile data is stored in the Area class, defined below
	int roomGroupID;
	RoomDestination mapUp;
	RoomDestination mapRight;
	RoomDestination mapDown;
	RoomDestination mapLeft;
} Room;

typedef struct {
	float waitFrames = 0;
	vector<int> aniTiles;
} TileAnimation;

typedef struct {
	Room room[4][5];
	TileAnimation tileAnimationList[40]; // Keeps track of every single tile animation within an Area (40 max per area, because it's the entire bottom row per area tileset)
} Area;

enum MapFile{MAP00, MAP01, MAP02, MAP03,
			MAP04, MAP05, MAP06, MAP07,
			MAP08, MAP09, MAP10, MAP11,
			MAP12, MAP13, MAP14, MAP15,
			MAP16, MAP17, MAP18, MAP19,
			MAP20, MAP21, MAP22, MAP23,
			MAP24, MAP25, MAP26, MAP27,
			MAP28, MAP29, MAP30, MAP31,
			MAP32, MAP33, MAP34,
			BOSS00, BOSS01, BOSS02, BOSS03,
			BOSS04, BOSS05, BOSS06, BOSS07,
			BOSS08, MAP_MAX};

enum TileTypes {TL_BLANK, TL_SOLID, TL_WATER, TL_HURT, TL_LADDER_LEFT, TL_LADDER_RIGHT, TL_DAIS, TL_DOOR_LEFT, TL_DOOR_RIGHT, TL_HELL_WATERFALL, TL_MAX};

typedef struct {
	uint8_t tile[40][30];
} CollisionMap;

// Define a world of areas, which have rooms in them. We will create <num of maps> areas, and have a parallel array of textures to refer to for each map (so we load the correct graphics for the map data)
typedef struct {
	Area area[MAP_MAX];
	Room blankRoom; // The room to use whenever a room is unloaded or undefined
	int texID[MAP_MAX];
	int collisionID[MAP_MAX];
	CollisionMap collisionMap[22];
} World;

enum Language {ENG, JPN, LNG_MAX};

enum XMLControlCommands {CC_NULL, CC_TALK, CC_FIELD, CC_WORLD, CC_CHIPLINE, CC_HIT, CC_ANIME, CC_OBJECT, CC_START, CC_LEFT, CC_RIGHT, CC_UP, CC_DOWN, CC_MAP, CC_MAX};

enum Direction {DIR_NONE, DIR_UP, DIR_LEFT, DIR_RIGHT, DIR_DOWN, DIR_MAX};

enum TexGroups {T_BOSS, T_EVENT, T_GENEMIES1, T_GENEMIES2, T_AENEMIES, T_ATEX, T_NATEX, T_FONT, T_ITEM, T_PROTAG, T_STDEMO1, T_MAX};

enum GFX {sprTitle, sprProt1, sprPlayer, sprTile0,
	sprMapG00, sprMapG01, sprMapG02, sprMapG03,
	sprMapG04, sprMapG05, sprMapG06, sprMapG07,
	sprMapG08, sprMapG09, sprMapG10, sprMapG11,
	sprMapG12, sprMapG13, sprMapG14, sprMapG15,
	sprMapG16, sprMapG17, sprMapG18, sprMapG19,
	sprMapG20, sprMapG21, sprMapG22,

	sprMapG23, sprMapG24, sprMapG25, sprMapG26, sprMapG27, sprMapG28, sprMapG29, sprMapG30, // These don't actually exist, they're just here to correct the numbering

	sprMapG31, sprMapG32,
	GFX_MAX};

enum SFX { SE00, SE01, SE02, SE03, SE04, SE_MAX};

enum Objects {objPlayer, objLevelData, OBJ_MAX};
