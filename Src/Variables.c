#include <genesis.h>
#include "Variables.h"
#include "Routines.h"
#include "Init.h"
#include "Animations.h"


///////////////////////////////////////////////
//    Positions MAP IA Zones
//////////////////////////////////////////////
const Vect2D_f32 PositionSniper[11] = {{FIX32(118), FIX32(84)},{FIX32(220), FIX32(84)}, {FIX32(364), FIX32(16)}, {FIX32(428), FIX32(84)}, {FIX32(700), FIX32(84)}, {FIX32(924), FIX32(60)}, {FIX32(1170), FIX32(84)}, {FIX32(1300), FIX32(16)}, {FIX32(1442), FIX32(16)}, {FIX32(1650), FIX32(84)}, {FIX32(1922), FIX32(84)}};
const Vect2D_f32 PositionCivil[10] = {{FIX32(64), FIX32(164)}, {FIX32(320), FIX32(164)}, {FIX32(576), FIX32(164)}, {FIX32(832), FIX32(164)}, {FIX32(1088), FIX32(164)}, {FIX32(1344), FIX32(164)}, {FIX32(832), FIX32(164)}, {FIX32(1600), FIX32(164)}, {FIX32(1856), FIX32(164)}, {FIX32(1984), FIX32(164)}};
const Vect2D_f32 PositionPara[16] = {{FIX32(64), FIX32(-32)}, {FIX32(192), FIX32(-32)}, {FIX32(320), FIX32(-32)}, {FIX32(448), FIX32(-32)}, {FIX32(576), FIX32(-32)}, {FIX32(704), FIX32(-32)}, {FIX32(832), FIX32(-32)}, {FIX32(960), FIX32(-32)}, {FIX32(1088), FIX32(-32)}, {FIX32(1216), FIX32(-32)}, {FIX32(1344), FIX32(-32)}, {FIX32(1472), FIX32(-32)}, {FIX32(1600), FIX32(-32)}, {FIX32(1728), FIX32(-32)}, {FIX32(1856), FIX32(-32)}, {FIX32(1984), FIX32(-32)}};


/////////////////////////////////////
//              Global  structures
////////////////////////////////////
Sprite1_ Sprites[60];
Sprite1_ SpriteHUD[10];
Sprite1_ NombreScore[5];
Sprite1_ HighScore[16];
Sprite1_ NombreUP;
Sprite1_ NombreLevel;
Sprite1_ SpriteHighScore;
Sprite1_ Marche_Joe;
Sprite1_ NombreBallesShot[2];
Sprite1_ NombreGrenades[2];
Sprite1_ SpriteCivil;
Sprite1_ SpriteBonus;


///////////////////////////////////
//		Variables principales
///////////////////////////////////

s16 PCamX=0;
s16 PCamY=0;

u16 ReadSram=0;
u16 RequisZone3=0;
u16 TestRoutine=0;
u16 RequisZone=0;
u16 TempoPaletteBG=0;
u16 IDCivil=0;
u16 TempoRegen=0;
u16 IDPlane=0;
u16 IDUnite=0;
u16 ApparitionMedailles=0;
u16 IDBouclier=0;
u16 IDBonus=0;
u16 IDBalle=0;
u16 TestPause=0;
u16 palette[64];
u16 NombreGrenade=0;
u16 NombreBalleShotgun=0;
u16 ind;
u16 numTile;
u16 MaxObjet;
u16 RNDSEED=0;
u16 NombreIA=0;
u16 NombreCivil=0;
u16 NombreIAScene=0;
u16 NombreIASceneMax=0;
u16 TempoRespawn=0;
u16 Score=0;
u16 Difficulte=0;
u16 PhaseScene=0;
u16 PhaseAffichageScene=0;
u16 NombreBalle=0;
u16 NombreBouclier=0;
u16 NombreLettre=0;
u16 NombreDigitScore=0;
u16 GameOver=0;
u16 TempoTremblement=0;
u16 TremblementON=0;
u16 TempoMax=0;
u16 SniperON=0;
u16 Bombardement=0;
u16 NombrePara=0;
u16 CivilON=0;
u16 ScanIA=0;
u16 CivilVisible=0;
u16 GoCivil=0;
u16 NumeroZone=0;
u16 PauseGame=0;
u16 StatutJoy=0;
u16 ChoixUnite=0;

fix32 Civil_CoordX=FIX32(0);
fix32 Civil_CoordY=FIX32(0);

u16 BlankPalette[16] =
{
	0x0EEE,0x0EEE,0x0EEE,0x0EEE,0x0EEE,0x0EEE,0x0EEE,0x0EEE,
	0x0EEE,0x0EEE,0x0EEE,0x0EEE,0x0EEE,0x0EEE,0x0EEE,0x0EEE
};

u16 PaletteZone2[16] =
{
     0x0000, 0x0200, 0x0200, 0x0420, 0x0420, 0x0420, 0x0642, 0x0642,
     0x0860, 0x0A84, 0x0AA6, 0x0CC8, 0x0EEA, 0x0EEC, 0x0EEC, 0x0000
};

u16 PaletteZone2_1[16] =
{
     0x0000, 0x0200, 0x0200, 0x0420, 0x0420, 0x0420, 0x0642, 0x0642,
     0x0640, 0x0E62, 0x0884, 0x0BB6, 0x0DD8, 0x0CCB, 0x0CCB, 0x0EEE
};

u16 PaletteZone2_2[16] =
{
     0x0000, 0x0200, 0x0200, 0x0420, 0x0420, 0x0420, 0x0642, 0x0642,
     0x0420, 0x0D40, 0x0662, 0x0AA4, 0x0CC6, 0x0BBA, 0x0BBA, 0x0DDD
};

u16 PaletteZone2_3[16] =
{
     0x0000, 0x0200, 0x0200, 0x0420, 0x0420, 0x0420, 0x0642, 0x0642,
     0x0200, 0x0B20, 0x0440, 0x0882, 0x0BB4, 0x0AA8, 0x0AA8, 0x0CCC
};

u16 PaletteZone2_4[16] =
{
     0x0000, 0x0200, 0x0200, 0x0420, 0x0420, 0x0420, 0x0642, 0x0642,
     0x0200, 0x0B20, 0x0440, 0x0882, 0x0BB4, 0x0AA8, 0x0AA8, 0x0BBB
};

u16 PaletteZone2_5[16] =
{
     0x0000, 0x0200, 0x0200, 0x0420, 0x0420, 0x0420, 0x0642, 0x0642,
     0x0000, 0x0A00, 0x0220, 0x0660, 0x0AA2, 0x0886, 0x0886, 0x0AAA
};

u16 PaletteZone2_6[16] =
{
     0x0000, 0x0200, 0x0200, 0x0420, 0x0420, 0x0420, 0x0642, 0x0642,
     0x0000, 0x0800, 0x0000, 0x0440, 0x0880, 0x0664, 0x0664, 0x0888
};


// BG start tile index
u16 bgBaseTileIndex[2];

// maps (BGA and BGB) position (tile) for alternate method
s16 mapMetaTilePosX[2];
s16 mapMetaTilePosY[2];
s16 CamPosX;
s16 CamPosY;
s16 VisCamX;
s16 VisCamY;


char Texte[64];

// maps (BGA and BGB)
Map *bgb;
TileMap *bga;

