#include <genesis.h>
#include "Variables.h"
#include "Routines.h"
#include "Init.h"
#include "Animations.h"


/////////////////////////////////////
//              Global  structures
////////////////////////////////////
Sprite1_ Sprites[60];
Sprite1_ SpriteHUD[10];
Sprite1_ NombreScore[5];
Sprite1_ NombreUP;
Sprite1_ NombreLevel;
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
u16 GrenadierON=0;
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
} ;


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

