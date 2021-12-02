#ifndef _VARIABLES_H
#define _VARIABLES_H

#define MAP_WIDTH    2048
#define MAP_HEIGHT		256
#define  SniperRange  getRandomF32(FIX32(64))+FIX32(64)+getRandomF32(FIX32(64));
#define TempoHelico 1000
#define TempoCivil 950

// Definition Sons & Musiques
#define SFX_GENERIC 64
#define SFX_GENERIC1 65
#define SFX_GENERIC2 66
#define SFX_GENERIC3 67
#define SFX_GENERIC4 68
#define SFX_GENERIC5 69
#define SFX_GENERIC6 70
#define SFX_GENERIC7 71
#define SFX_GENERIC8 72
#define SFX_GENERIC9 73
#define SFX_GENERIC10 74
#define SFX_GENERIC11 75
#define SFX_GENERIC12 76
#define SFX_GENERIC13 77
#define SFX_GENERIC14 78
#define SFX_GENERIC15 79
#define SFX_GENERIC16 80
#define SFX_GENERIC17 81
#define SFX_GENERIC18 82
#define SFX_GENERIC19 83
#define SFX_GENERIC20 84
#define SFX_GENERIC21 85
#define SFX_GENERIC22 86
#define SFX_GENERIC23 87
#define SFX_GENERIC24 88
#define SFX_GENERIC25 89


/*

    ID des différents objets du jeu !

    1 : Type Airunit 1 : Hélicoptère lourd ( Largue des bombes )  Type Airunit 10 : Hélicoptère léger ( Tire des balles )
    2 : Unité terrestre avec bouclier de protection ( tire des balles )
    3 : Fantassin ( Court et cherche le contact au Cac )
    4 : Grenadier ( Court tout en lancant des grenades )
    5 : Parachutistes ( Descente jusqu'au sol tout en planant et tirant sur le joueur )
    6 : Civil à sauver
    7 : Sniper fenêtre Zone 2.
    40 : Bonus
    66 : Bombes
    88 : Grenades
    99 : Joueur principal JOE



*/


// Déclaration des structures de Sprites
typedef struct
{
  Sprite* SpriteA;
  u8 Aggro;
  u8 AggroG;
  u8 RatioLoot;
  u8 GodMode;
  u8 NombreUP;
  u8 TypeLoot;
  u8 Slot1;  // 0 : Magnum, 1 : Uzi, 2 : Desert Eagle, 3 : Cal.12, 4 : Grenades
  u8 Slot2; // 0 : Magnum, 1 : Uzi, 2 : Desert Eagle, 3 : Cal. 12, 4 : Grenades
  u8 Sens;
  u8 SensY;
  u8 SensExplosion;
  u8 Couteau;
  u8 NombreGrenade;
  u8 TempoCouteau;
  u8 Feu;
  u8 CaC;
  u8 Sniper;
  u8 SpriteDYN;
  u8 TypeIA;
  u8 Hauteur;
  u8 MemSprint;
  u8 TirBusy;
  u8 CreateSprite;
  u8 Hit;
  u8 HitB;
  u8 DirectionTir;
  u8 LancerGrenade;
  u8 AnimationON;
  u8 Reach;
  u8 Transition;
  u8 CalculTransition;
  u8 DirectionG;
  u8 TransitionTempo;
  u8 HitTempo;
  u8 ClignoTempo;
  u8 ClignoDuree;
  u8 ClignoTic;
  u8 ClignoON;
  u8 ClignoReach;
  u8 ClignoSprite;
  u8 SensHit;
  u8 NoAnimation;
  u8 TirCouvert;
  u8 Saut;
  u8 TirBunker;
  u8 TempoCountRafaleMax;
  u8 TempoRespawn;
  u8 CouvertBalle;
  u8 ChauffeArmeMax;
  u8 ChauffeArme1;
  u8 ChauffeArme2;
  u8 ChauffeArme;
  u8 DeltaChauffe;
  u8 TempoChauffeArme;
  u8 Die;
  u8 TempoRafale;
  u8 TempoCountRafale;
  u8 SpeedArme;
  u8 Rafale;
  u8 IAFuite;
  u8 DegatArme;
  u8 TypeBouclier;
  u8 CountRafale;
  u8 SearchIA;
  u8 InScene;
  u8 IntIA;
  u8 AirUnit;
  u8 CoupCrosse;
  u8 CriMort;
  u8 HitPoint;
  u8 HitPointMax;
  u8 MortIA;
  u8 Block;
  u8 BlockH;
  u8 BlockB;
  u8 SpeIA;
  u8 BlockD;
  u8 Phase;
  u8 ID;
  u8 Visible;
  u8 IDList;
  u8 BlockG;
  u8 Init;
  u8 StandBy;   //1 : Non visible, 0:Visible
  u8 Direction;    //8 : H, 2: B, 4:G, 6:D  - 84 : HG, 86:HD, 24:BG, 26:BD  44 : Latéral Gauche 66 Latéral Droite
  u8 MemDir;
  u8 TypeUnit;
  u8 Sprint;
  u8 Animation;
  u16 TempoScan;
  u16 TempoSprite;
  u16 TempoInScene;
  u16 TempoSpriteMax;
  u16 TempoRespawnMax;
  u16 TempoCouvert;
  u16  ScoreUnit;
  u16 TempoAggro;
  u16 TempoAggroMax;
  fix32 CoordX;
  fix32 ForceG;
  fix32 ForceGY;
  fix32 GrenadeY;
  fix32 BalleYRef;
  fix32 Acceleration;
  fix32 CoordY;
  fix32 RefY;
  fix32 DeltaY;
  fix32 OffsetX;
  fix32 OffsetY;
  fix32 Vitesse;
  fix32 VitesseBalle;
  fix32 DistanceAggro;
  fix32 VitesseBalleD;
  fix32 VitesseD;
  fix32 VitesseSprintD;
  fix32 VitesseInit;
  fix32 VitesseSprint;
  fix32 PorteeTir;
  fix32 PorteeTirMax;
  fix32 Spotting;
  fix32 Boost;
} Sprite1_;

extern Sprite1_ Sprites[60];
extern Sprite1_ SpriteHUD[10];
extern Sprite1_ NombreScore[5];
extern Sprite1_ NombreBallesShot[2];
extern Sprite1_ NombreGrenades[2];
extern Sprite1_ NombreUP;
extern Sprite1_ NombreLevel;
extern Sprite1_ Marche_Joe;
extern Sprite1_ SpriteCivil;
extern Sprite1_ SpriteBonus;

extern s16 PCamX;
extern s16 PCamY;

extern u16 TestRoutine;
extern u16 BlankPalette[16];
extern u16 PaletteZone2[16];
extern u16 PaletteZone2_1[16];
extern u16 PaletteZone2_2[16];
extern u16 PaletteZone2_3[16];
extern u16 PaletteZone2_4[16];
extern u16 PaletteZone2_5[16];
extern u16 PaletteZone2_6[16];

extern u16 IDCivil;
extern u16 NumeroZone;
extern u16 GoCivil;
extern u16 IDBonus;
extern u16 CivilVisible;
extern u16 NombreCivil;
extern u16 ScanIA;
extern u16 CivilON;
extern u16 NombreGrenade;
extern u16 TempoRegen;

extern u16 RequisZone;
extern u16 TempoPaletteBG;
extern u16 SniperON;
extern u16 ApparitionMedailles;
extern u16 TremblementON;
extern u16 TempoTremblement;
extern u16 IDPlane;
extern u16 IDUnite;
extern u16 IDBouclier;
extern u16 IDBalle;
extern u16 NombreBalleShotgun;
extern u16 TestPause;
extern u16 palette[64];
extern u16 ind;
extern u16 numTile;
extern u16 Difficulte;
extern u16 bgBaseTileIndex[2];
extern u16 MaxObjet;
extern u16 RNDSEED;
extern u16 NombreIA;
extern u16 NombreIAScene;
extern u16 NombreIASceneMax;
extern u16 TempoRespawn;
extern u16 Score;
extern u16 PhaseScene;
extern u16 PhaseAffichageScene;
extern u16 NombreBalle;
extern u16 NombreBouclier;
extern u16 NombreLettre;
extern u16 NombreDigitScore;
extern u16 GameOver;
extern u16 TempoMax;
extern u16 Bombardement;
extern u16 NombrePara;
extern u16 PauseGame;
extern u16 StatutJoy;
extern u16 ChoixUnite;

extern fix32 Civil_CoordX;
extern fix32 Civil_CoordY;

extern const Vect2D_f32 PositionSniper[11];
extern const Vect2D_f32 PositionCivil[10];
extern const Vect2D_f32 PositionPara[16];

// maps (BGA and BGB) position (tile) for alternate method
extern s16 mapMetaTilePosX[2];
extern s16 mapMetaTilePosY[2];
// Camera Position
extern s16 CamPosX;
extern s16 CamPosY;
extern s16 VisCamX;
extern s16 VisCamY;

// maps (BGA and BGB)
extern Map *bgb;
extern TileMap *bga;

extern char Texte[64];

#endif // _MAIN
