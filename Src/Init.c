
// Librairies Projet

#include <genesis.h>
#include "main.h"
#include "gfx.h"
#include "Variables.h"
#include "Routines.h"
#include "dma.h"
#include "Sprites.h"
#include "Sound.h"
#include "Init.h"
#include "Animations.h"
#include "GestionPAD.h"



////////////////////////////////////////
//                  CLEAR Variable
////////////////////////////////////////
void Clear_Variable()
{
    ind=0;
    StatutJoy=0;
    TestRoutine=0;
    PauseGame=0;
    numTile=0;
    RNDSEED=0;
    NombreIA=0;
    NombreIAScene=0;
    NombreIASceneMax=0;
    CivilON=0;
    IDBonus=0;
    TestPause=0;
    ApparitionMedailles=0;
    NombreGrenade=10;
    TempoRespawn=0;
    NombreCivil=0;
    //Difficulte=0;
    Score=0;
    PhaseScene=0;
    PhaseAffichageScene=0;
    NombreBalleShotgun=50;
    NombreBalle=0;
    NombreBouclier=0;
    GoCivil=0;
    SniperON=0;
    NombreLettre=0;
    NombreDigitScore=0;
    CamPosX=0;
    CamPosY=0;
    VisCamX=0;
    VisCamY=0;
    TempoTremblement=0;
    TremblementON=0;
    Bombardement=0;
    Civil_CoordX=FIX32(0);
	Civil_CoordY=FIX32(0);
	ScanIA=0;

	u16 i=60;
	Sprite1_* spr = &Sprites[0];
	while(i--)
    {
        spr->Aggro=0;
        spr->AggroG=0;
        spr->RatioLoot=0;
        spr->GodMode=0;
        spr->NombreUP=0;
        spr->TypeLoot=0;
        spr->Sens=0;
        spr->SensY=0;
        spr->SensExplosion=0;
        spr->Couteau=0;
        spr->NombreGrenade=0;
        spr->TempoCouteau=0;
        spr->Feu=0;
        spr->CaC=0;
        spr->Phase=0;
        spr->Sniper=0;
        spr->InScene=0;
        spr->SpriteDYN=0;
        spr->TypeIA=0;
        spr->Hauteur=0;
        spr->MemSprint=0;
        spr->TirBusy=0;
        spr->LancerGrenade=0;
        spr->CreateSprite=0;
        spr->Hit=0;
        spr->HitB=0;
        spr->DirectionTir=0;
        spr->AnimationON=0;
        spr->Reach=0;
        spr->Transition=0;
        spr->CalculTransition=0;
        spr->DirectionG=0;
        spr->TransitionTempo=0;
        spr-> HitTempo=0;
        spr->ClignoTempo=0;
        spr->ClignoDuree=0;
        spr->ClignoTic=0;
        spr->ClignoON=0;
        spr->ClignoReach=0;
        spr->TempoScan=0;
        spr->ClignoSprite=0;
        spr->SensHit=0;
        spr->NoAnimation=0;
        spr->TirCouvert=0;
        spr->Saut=0;
        spr->TempoCouvert=0;
        spr->TirBunker=0;
        spr->TempoCountRafaleMax=0;
        spr->TempoRespawn=0;
        spr->CouvertBalle=0;
        spr->ChauffeArmeMax=0;
        spr->ChauffeArme1=0;
        spr-> ChauffeArme2=0;
        spr->ChauffeArme=0;
        spr-> DeltaChauffe=0;
        spr->TempoChauffeArme=0;
        spr->Die=0;
        spr->TempoRafale=0;
        spr->TempoCountRafale=0;
        spr-> SpeedArme=0;
        spr->Rafale=0;
        spr->IAFuite=0;
        spr-> DegatArme=0;
        spr->TypeBouclier=0;
        spr-> CountRafale=0;
        spr->SearchIA=0;
        spr-> IntIA=0;
        spr->AirUnit=0;
        spr-> CoupCrosse=0;
        spr->CriMort=0;
        spr->HitPoint=0;
        spr->HitPointMax=0;
        spr->MortIA=0;
        spr->Block=0;
        spr->BlockH=0;
        spr-> BlockB=0;
        spr->SpeIA=0;
        spr->BlockD=0;
        spr->ID=0;
        spr->Visible=0;
        spr->TempoInScene=0;
        spr->IDList=0;
        spr-> BlockG=0;
        spr->Init=0;
        spr->StandBy=0;//1 : Non visible, 0:Visible
        spr-> Direction=0;//8 : H, 2: B, 4:G, 6:D  - 84 : HG, 86:HD, 24:BG, 26:BD  44 : Latéral Gauche 66 Latéral Droite
        spr->MemDir=0;
        spr->TypeUnit=0;
        spr->Sprint=0;
        spr->Animation=0;
        spr-> TempoSprite=0;
        spr-> TempoSpriteMax=0;
        spr->TempoRespawnMax=0;
        spr->ScoreUnit=0;
        spr->TempoAggro=0;
        spr->TempoAggroMax=0;
        spr->CoordX=FIX32(0);
        spr->ForceG=FIX32(0);
        spr->ForceGY=FIX32(0);
        spr->GrenadeY=FIX32(0);
        spr-> BalleYRef=FIX32(0);
        spr->Acceleration=FIX32(0);
        spr->CoordY=FIX32(0);
        spr-> RefY=FIX32(0);
        spr->DeltaY=FIX32(0);
        spr->OffsetX=FIX32(0);
        spr->OffsetY=FIX32(0);
        spr->Vitesse=FIX32(0);
        spr->VitesseBalle=FIX32(0);
        spr-> DistanceAggro=FIX32(0);
        spr->VitesseBalleD=FIX32(0);
        spr->VitesseD=FIX32(0);
        spr->VitesseSprintD=FIX32(0);
        spr->VitesseInit=FIX32(0);
        spr->VitesseSprint=FIX32(0);
        spr->PorteeTir=FIX32(0);
        spr->PorteeTirMax=FIX32(0);
        spr-> Spotting=FIX32(0);
        spr->Boost=FIX32(0);
        spr++;
    }
}


///////////////////////////////
//    Ecran sélection Zone
///////////////////////////////
void InitEcranZone()
{
	u16 Tempo=0;
	// Init
	SYS_doVBlankProcess();
	VDP_init();
	VDP_setPaletteColors(0, (u16*) palette_black, 64);
	memcpy(&palette[48], Palette_Font.data, 16 * 2);
	memcpy(&palette[16], Palette_BGB_1.data, 16 * 2);
	memcpy(&palette[32], Palette_BGB.data, 16 * 2);
	memcpy(&palette[0], Palette_Zone3.data, 16 * 2);

	// Init Scene.
	RequisZone3=1;
	RequisZone=1;
	NumeroZone=0;
	ind = TILE_USERINDEX;
	VDP_loadTileSet(Town_.tileset, ind, DMA);
	TileMap *Zone1 = Town_.tilemap;
	VDP_setTileMapEx(BG_B, Zone1, TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, ind), 20,1, 0, 0, 20, 14, CPU);
	ind +=256;
	VDP_loadTileSet(Jungle_.tileset, ind, DMA);
	TileMap *Zone2 = Jungle_.tilemap;
	VDP_setTileMapEx(BG_B, Zone2, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ind), 0, 1, 0, 0, 20, 14, CPU);


    // Données Textes / FONT
	VDP_setTextPalette(PAL3);
	VDP_loadTileSet(Font1.tileset, TILE_FONTINDEX+1, TRUE);
	VDP_drawText("<JUNGLE ZONE>",4,15);
	VDP_drawText(" TOWN ZONE ",25,15);
	if (!RequisZone) VDP_drawText("! NEED RANK 7 !",23,8);

	// Zone spéciale !
	if (RequisZone3==1)
	{
		ind+=256;
		VDP_loadTileSet(ZoneBoss.tileset, ind, DMA);
		TileMap *Zone3 = ZoneBoss.tilemap;
		VDP_setTileMapEx(BG_B, Zone3, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, ind), 10, 16, 0, 0, 20, 14, CPU);
		VDP_drawText(" FINAL ZONE ",15,26);
	}

	VDP_fadeInAll(palette,24,FALSE);
	XGM_startPlay(ChoixZone_Music);

	// Boucle principale.
	while(TRUE)
	{
		// Zone 2 débloquée ?
		if (!RequisZone)
		{
			Tempo++;
			if (Tempo<5) VDP_drawText("                ",23,8);
			else VDP_drawText("! NEED RANK 7 !",23,8);
			if (Tempo>10) Tempo=0;
		}

		// Init joy
		u16 value=JOY_readJoypad(JOY_1);
		if (value & BUTTON_RIGHT && RequisZone)
		{
			VDP_drawText("<TOWN ZONE>",25,15);
			VDP_drawText(" JUNGLE ZONE ",4,15);
			if (RequisZone3==1) VDP_drawText(" FINAL ZONE ",15,26);
			NumeroZone=1;
		}
		if (value & BUTTON_LEFT)
		{
			VDP_drawText(" TOWN ZONE ",25,15);
			VDP_drawText("<JUNGLE ZONE>",4,15);
			if (RequisZone3==1) VDP_drawText(" FINAL ZONE ",15,26);
			NumeroZone=0;
		}
		if (value & BUTTON_DOWN && RequisZone3==1)
		{
			VDP_drawText(" TOWN ZONE ",25,15);
			VDP_drawText(" JUNGLE ZONE ",4,15);
			VDP_drawText("<FINAL ZONE>",15,26);
			NumeroZone=2;
		}

		if (value & BUTTON_UP && RequisZone3==1)
		{
			VDP_drawText(" TOWN ZONE ",25,15);
			VDP_drawText("<JUNGLE ZONE>",4,15);
			VDP_drawText(" FINAL ZONE ",15,26);
			NumeroZone=0;
		}


		if (value & (BUTTON_A | BUTTON_B | BUTTON_C | BUTTON_START))
		{
			SND_startPlayPCM_XGM(SFX_GENERIC14, 2, SOUND_PCM_CH4);
			break;
		}

		// Vblank
		SYS_doVBlankProcess();
	}

	XGM_stopPlay();
	//SYS_doVBlankProcess();
	VDP_clearPlane(BG_A,TRUE);
    MEM_free(Zone1);
    MEM_free(Zone2);
    if (RequisZone3==1) MEM_free(Zone3);
    VDP_fadeOutAll(16,FALSE);
    //SPR_reset();
    SPR_end();
}

///////////////////////////////
//                  Init Intro
///////////////////////////////
void InitIntro()
{
		// Init
	SYS_doVBlankProcess();
	VDP_init();
	// set all palette to black
    VDP_setPaletteColors(0, (u16*) palette_black, 64);

	if (!GameOver)
	{
		// Logo Sgdk
		memcpy(&palette[0], Palette_LogoSgdk.data, 16 * 2);
		ind = TILE_USERINDEX;
		VDP_loadTileSet(Image_LogoSgdk.tileset, ind, DMA);
		TileMap *bgaIntro1 = Image_LogoSgdk.tilemap;
		VDP_setTileMapEx(BG_B, bgaIntro1, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, ind), 0, 0, 0, 2, 40, 30, CPU);

		VDP_fadeInAll(palette,24,FALSE);
		while(TRUE)
		{

			waitMs(2000);
			break;
			// Vblank
			SYS_doVBlankProcess();
		}
		// Fade In Scene.
		VDP_fadeOutAll(16,FALSE);
		MEM_free(bgaIntro1);
		ind=0;
		SPR_end();
		SYS_doVBlankProcess();
		VDP_init();
	}

	GameOver=0;
	// set all palette to black
    VDP_setPaletteColors(0, (u16*) palette_black, 64);
	ind=0;
	memcpy(&palette[0], Palette_EcranMS.data, 16 * 2);
	memcpy(&palette[16], Palette_Menu.data, 16 * 2);
	memcpy(&palette[48], Palette_Font.data, 16 * 2);

    ind = TILE_USERINDEX;
    VDP_loadTileSet(bga_Ecran_MS.tileset, ind, DMA);
    // Font
	VDP_setTextPalette(PAL3);
	VDP_loadTileSet(Font1.tileset, TILE_FONTINDEX+1, TRUE);

    // init backgrounds
    TileMap *bgaIntro = bga_Ecran_MS.tilemap;
    VDP_setTileMapEx(BG_B, bgaIntro, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, ind), 0, 0, 0, 1, 40, 30, CPU);


    // Chargement du Texte MENU déroulant
    ind +=  bga_Ecran_MS.tileset->numTile;
    bgBaseTileIndex[1] = ind;
    VDP_loadTileSet(&bga_tileset_Menu, ind, DMA);
    // init backgrounds
    bgb = MAP_create(&bga_Map_Menu, BG_A, TILE_ATTR_FULL(PAL1, TRUE, FALSE, FALSE, bgBaseTileIndex[1]));
	CamPosX=64;
	CamPosY=-32;


    // init sprite engine with default parameters
    SPR_init();
	Sprite1_* SprLvl= &NombreLevel;
	SprLvl->SpriteA = SPR_addSprite(&Warfare_Sprite, 0, 0, TILE_ATTR(PAL0, TRUE, FALSE, FALSE));
	SPR_setPosition(SprLvl->SpriteA,100,17);
	SPR_setAlwaysOnTop(SprLvl->SpriteA,TRUE);
	SPR_setAnim(SprLvl->SpriteA,0);
	SPR_setVisibility(SprLvl->SpriteA,VISIBLE);
	u16 Anim=0;
	u8 TempoTexte=0;
	// Fade In Scene.
    VDP_fadeInAll(palette,32,FALSE);
	// start music
	XGM_startPlay(Menu_Music);
	VDP_drawTextBG(BG_B,"NOT FOR SALE !!",12,14);

    while(TRUE)
	{
		u16 value=JOY_readJoypad(JOY_1);

		// Message Important !
		TempoTexte++;
		if (TempoTexte>20) TempoTexte=0;
		if (TempoTexte<10) VDP_drawTextBG(BG_B,"NOT FOR SALE !!",12,14);
		else VDP_drawTextBG(BG_B,"                 ",12,14);

		// Quitter ?!
		if (value & (BUTTON_A | BUTTON_B | BUTTON_C | BUTTON_START)) break;

		// Animation Warfare !
        SprLvl->TempoSprite++;
        if (SprLvl->TempoSprite>3)
		{
			Anim++;
			SprLvl->TempoSprite=0;
			if (Anim>8) Anim=0;
			SPR_setAnim(SprLvl->SpriteA,Anim);
		}
		CamPosX+=4;
		SPR_update();
		// Vblank
		SYS_doVBlankProcess();
		// set new camera position
		if (CamPosX>13100)
		{
			VDP_clearPlane(BG_A,TRUE);
			CamPosX=0;
		}
        MAP_scrollTo(bgb, CamPosX, 0);
		VDP_setHorizontalScroll(BG_A,(-CamPosX));
		VDP_setVerticalScroll(BG_A, CamPosY);
	}
	// Fade In Scene.
	//XGM_pausePlay();
	XGM_stopPlay();
	VDP_clearPlane(BG_A,TRUE);
    VDP_fadeOutAll(16,FALSE);
    MEM_free(bgaIntro);
    MEM_free(bgb);
    //SPR_reset();
    SPR_end();
    ind=0;
}

///////////////////////////////
//                  Init Scene
///////////////////////////////
void InitScene()
{
	// Fade & Palettes
	if (NumeroZone)
	{
		memcpy(&palette[0], Palette_BGA_1.data, 16 * 2);
		memcpy(&palette[16], Palette_BGB_1.data, 16 * 2);
	}
	else
	{
		memcpy(&palette[0], Palette_BGA.data, 16 * 2);
		memcpy(&palette[16], Palette_BGB.data, 16 * 2);
	}

	memcpy(&palette[32],Palette_Joe.data, 16*2);
	memcpy(&palette[48],Palette_Airplane.data,16*2);

    // init sprite engine with default parameters
	Sprite1_* spr = &Sprites[0];
	spr->CoordX=FIX32(48);
	spr->CoordY=FIX32(140-48);
     // BGB
    updateCameraPosition(spr);
	// set new camera position
    setCameraPosition(PCamX, PCamY);
    updateVDPScroll();
	SYS_doVBlankProcess();
    // BGA
    VDP_setTileMapEx(BG_A, bga, TILE_ATTR_FULL(0, FALSE, FALSE, FALSE, bgBaseTileIndex[0]), 0, 0, 0, 0, 64, 32, DMA_QUEUE);

	// camera position (force refresh)
    CamPosX = -1;
    CamPosY = -1;

    // Allocation VRAM SE.
    u16 MemVram=208;
    if (NumeroZone) MemVram=288;
    SPR_initEx(512+MemVram);

    // Medaille Difficulté
	Sprite1_* SprLvl= &NombreLevel;
	//SprLvl->CoordX=FIX32(-6);
	SprLvl->CoordX=FIX32(320);
	SprLvl->CoordY=FIX32(0);
	SprLvl->Vitesse=FIX32(5);
	SprLvl->SpriteA = SPR_addSprite(&Medailles_Sprite, 0, 0, TILE_ATTR(PAL2, TRUE, FALSE, FALSE));
	SPR_setPosition(SprLvl->SpriteA,fix32ToInt(SprLvl->CoordX),fix32ToInt(SprLvl->CoordY));
	SPR_setAlwaysOnTop(SprLvl->SpriteA,TRUE);
	SPR_setAnim(SprLvl->SpriteA,0);
	SPR_setVisibility(SprLvl->SpriteA,HIDDEN);

	// HUD
	Sprite1_* SprHUD= &SpriteHUD[0];
	SprHUD->CoordX=FIX32(16);
	SprHUD->CoordY=FIX32(16);
	SprHUD->SpriteA = SPR_addSprite(&HUDSante_Sprite, 0, 0, TILE_ATTR(PAL2, TRUE, FALSE, FALSE));
	SPR_setAlwaysOnTop(SprHUD->SpriteA,TRUE);
	SPR_setPosition(SprHUD->SpriteA,fix32ToInt(SprHUD->CoordX),fix32ToInt(SprHUD->CoordY));
	SPR_setAnim(SprHUD->SpriteA,0);
	SprHUD++;
	SprHUD->CoordX=FIX32(16);
	SprHUD->CoordY=FIX32(24);
	SprHUD->SpriteA = SPR_addSprite(&HUDCredit_Sprite, 0, 0, TILE_ATTR(PAL2, TRUE, FALSE, FALSE));
	SPR_setPosition(SprHUD->SpriteA,fix32ToInt(SprHUD->CoordX),fix32ToInt(SprHUD->CoordY));
	SPR_setAlwaysOnTop(SprHUD->SpriteA,TRUE);
	SPR_setAnim(SprHUD->SpriteA,0);
	SprHUD++;
	SprHUD->CoordX=FIX32(72);
	SprHUD->CoordY=FIX32(8);
	SprHUD->SpriteA = SPR_addSprite(&HUDArme_Sprite, 0, 0, TILE_ATTR(PAL2, TRUE, FALSE, FALSE));
	SPR_setPosition(SprHUD->SpriteA,fix32ToInt(SprHUD->CoordX),fix32ToInt(SprHUD->CoordY));
	SPR_setAlwaysOnTop(SprHUD->SpriteA,TRUE);
	SPR_setAnim(SprHUD->SpriteA,0);
	SprHUD++;

	// Icone Civile
	Sprite1_* SprCivil=&SpriteCivil;
	SprCivil->SpriteA = SPR_addSprite(&IconeCivil_Sprite, 0, 0, TILE_ATTR(PAL2, TRUE, FALSE, FALSE));
	SPR_setAlwaysOnTop(SprCivil->SpriteA,TRUE);
	SPR_setPosition(SprCivil->SpriteA,300,100);
	SPR_setVisibility(SprCivil->SpriteA,HIDDEN);

	// Init Data
	NombreBalle=6;
	NombreBouclier=2;
	NombreLettre=6;
	NombreIA=5;
	NombreDigitScore=5;
	NombreIAScene=0;
	NombreGrenade=10;
	NombreBalleShotgun=0;
	GoCivil=1;

	// Score
	Sprite1_* SprScore=&NombreScore[0];
    u8 i=NombreDigitScore;
    u8 j=0;
    u16 Pos=0;
    while(i--)
    {
        Pos=64-(j<<3);
        j++;
        SprScore->SpriteA = SPR_addSprite(&Nombre_Sprite, 0, 0, TILE_ATTR(PAL2, TRUE, FALSE, FALSE));
        SPR_setPriorityAttribut(SprScore->SpriteA, TRUE);
        SPR_setAnim(SprScore->SpriteA,0);
        SPR_setPosition(SprScore->SpriteA,Pos,8);
		SPR_setAlwaysOnTop(SprScore->SpriteA,TRUE);
        SprScore++;
    }

    // Nombre UP
	Sprite1_* SprNombreUP=&NombreUP;
	SprNombreUP->SpriteA = SPR_addSprite(&Nombre1_Sprite, 0, 0, TILE_ATTR(PAL2, TRUE, FALSE, FALSE));
	SPR_setPriorityAttribut(SprNombreUP->SpriteA, TRUE);
	SPR_setAnim(SprNombreUP->SpriteA,3);
	SPR_setPosition(SprNombreUP->SpriteA,52,24);
	SPR_setAlwaysOnTop(SprNombreUP->SpriteA,TRUE);

	// Nombre Grenades
	Sprite1_* SprNombreGrenade=&NombreGrenades[0];
    i=2;
    //j=0;
    Pos=0;
    while(i--)
    {
        //Pos=64-(j<<3);
        //j++;
        SprNombreGrenade->SpriteA = SPR_addSprite(&Nombre1_Sprite, 0, 0, TILE_ATTR(PAL2, TRUE, FALSE, FALSE));
        SPR_setPriorityAttribut(SprNombreGrenade->SpriteA, TRUE);
        //SPR_setAnim(SprNombreGrenade->SpriteA,0);
        //SPR_setPosition(SprNombreGrenade->SpriteA,Pos,8);
		SPR_setAlwaysOnTop(SprNombreGrenade->SpriteA,TRUE);
        SprNombreGrenade++;
    }
    GestionNombreGrenades();

    // Nombre Balles Shotgun
	// Nombre Grenades
	Sprite1_* SprNombreBalleShotGun=&NombreBallesShot[0];
    i=2;
    //j=0;
    Pos=0;
    while(i--)
    {
        //Pos=64-(j<<3);
        //j++;
        SprNombreBalleShotGun->SpriteA = SPR_addSprite(&Nombre1_Sprite, 0, 0, TILE_ATTR(PAL2, TRUE, FALSE, FALSE));
        SPR_setPriorityAttribut(SprNombreBalleShotGun->SpriteA, TRUE);
        //SPR_setAnim(SprNombreGrenade->SpriteA,0);
        //SPR_setPosition(SprNombreGrenade->SpriteA,Pos,8);
		SPR_setAlwaysOnTop(SprNombreBalleShotGun->SpriteA,TRUE);
        SprNombreBalleShotGun++;
    }
    GestionNombreBallesShotgun();


	// Joueur principal
	spr->MemDir=6;
	spr->Vitesse=FIX32(1.25);
	spr->AirUnit=0;
	spr->StandBy=0;
	spr->TypeIA=0;
    spr->SpriteDYN=0;
    spr->NombreUP=3;
    spr->Slot1=0;

    spr->HitPointMax=6;
    spr->HitPoint=spr->HitPointMax;
    Difficulte=0;
	// ID Joueur = 99
	spr->ID=99;
	spr->Visible=1;


    // Chargement Sprite Joe
    // Jambes
	Sprite1_* SprMarcheJoe=&Marche_Joe;

	SprMarcheJoe->SpriteA = SPR_addSprite(&MarcheJoe_Sprite, 0, 0, TILE_ATTR(PAL2, TRUE, FALSE, FALSE));
	spr->SpriteA = SPR_addSprite(&Joe_Sprite, 0, 0, TILE_ATTR(PAL2, TRUE, FALSE, FALSE));
	SPR_setPriorityAttribut(spr->SpriteA, TRUE);
	SPR_setVisibility(spr->SpriteA,HIDDEN);
	SPR_setVisibility(SprMarcheJoe->SpriteA,HIDDEN);
	SPR_setPosition(spr->SpriteA,fix32ToInt(spr->CoordX),fix32ToInt(spr->CoordY));
	SPR_setAnim(spr->SpriteA,0);

	// Airplane
	spr++;
	spr->CoordX=FIX32(-64);
	if (NumeroZone) spr->CoordY=FIX32(24);
	else spr->CoordY=FIX32(8);
	spr->MemDir=0;
	spr->AirUnit=10;
	spr->Vitesse=FIX32(0.5);
	spr->Direction=6;
    spr->SpriteDYN=0;
    spr->Phase=0;
	spr->ID=1;
	spr->TypeIA=0;
    spr->HitPointMax=30;
    spr->HitPoint=spr->HitPointMax;
	spr->Visible=1;
	IDPlane=1;

    // Chargement Sprites
	//spr->SpriteA = SPR_addSprite(&Airplane_Sprite, 0, 0, TILE_ATTR(PAL3, TRUE, FALSE, FALSE));
	spr->SpriteA = SPR_addSprite(&AirplaneLight_Sprite, 0, 0, TILE_ATTR(PAL3, TRUE, FALSE, FALSE));
	SPR_setPriorityAttribut(spr->SpriteA, FALSE);
	SPR_setVisibility(spr->SpriteA,VISIBLE);
	SPR_setPosition(spr->SpriteA,fix32ToInt(spr->CoordX),fix32ToInt(spr->CoordY));
    SPR_setAlwaysOnTop(spr->SpriteA,FALSE);
	SPR_setAnim(spr->SpriteA,0);
	spr++;

    // Icone objets
    IDBonus=IDPlane+1;
	spr->SpriteA = SPR_addSprite(&IconeBonus_Sprite, 0, 0, TILE_ATTR(PAL2, TRUE, FALSE, FALSE));
	SPR_setPosition(spr->SpriteA,0,0);
	SPR_setAlwaysOnTop(spr->SpriteA,TRUE);
	SPR_setAnim(spr->SpriteA,0);
	SPR_setVisibility(spr->SpriteA,HIDDEN);
	spr->Visible=0;
	spr->StandBy=1;
	spr->ID=40;
	spr++;

	IDUnite=IDBonus+1;

	// Init sprites Dynamiques.
	i=NombreIA;
	//fix32 Xd;
	//u8 Idi=1;
	while(i--)
    {
    	RandomSeed();
        spr->MemDir=0;
        spr->AirUnit=0;
        spr->StandBy=0;
        spr->SpriteDYN=1;
        spr->SpriteA=NULL;
        spr->TempoRespawnMax=getRandomU16(100)+50;
        spr->TypeIA=1;
        spr->Visible=0;
        spr->TypeBouclier=0;
        spr++;
    }

    // Balles
    i=NombreBalle;
    IDBalle=NombreIA+IDUnite;
	while(i--)
    {
        spr->CoordX=FIX32(0);
        spr->CoordY=FIX32(0);
        spr->MemDir=0;
        spr->Vitesse=FIX32(8);
        spr->SpriteDYN=0;
        spr->AirUnit=0;
        spr->ID=45;
        spr->Visible=0;
        spr->StandBy=1;
        spr->TypeIA=0;
        spr->TypeBouclier=0;
        spr->SpriteA = SPR_addSprite(&Balle_Sprite, 0, 0, TILE_ATTR(PAL2, TRUE, FALSE, FALSE));
        SPR_setPriorityAttribut(spr->SpriteA, TRUE);
        SPR_setVisibility(spr->SpriteA,HIDDEN);
        SPR_setPosition(spr->SpriteA,fix32ToInt(spr->CoordX),fix32ToInt(spr->CoordY));
        SPR_setAnim(spr->SpriteA,0);
        SPR_setAlwaysOnTop(spr->SpriteA,TRUE);
        spr++;
    }

    // Bouclier
    i=NombreBouclier;
    IDBouclier=IDBalle+NombreBalle;
    while(i--)
	{
        spr->CoordX=FIX32(0);
        spr->CoordY=FIX32(0);
        spr->MemDir=0;
        spr->Vitesse=FIX32(8);
        spr->AirUnit=0;
        spr->SpriteDYN=0;
        spr->TypeBouclier=1;
        spr->Visible=0;
        spr->StandBy=1;
        spr->TypeIA=0;
        spr->SpriteA = SPR_addSprite(&Bouclier_Sprite, 0, 0, TILE_ATTR(PAL2, TRUE, FALSE, FALSE));
        SPR_setPriorityAttribut(spr->SpriteA, TRUE);
        SPR_setVisibility(spr->SpriteA,HIDDEN);
        //Sprite1_* SpriteREF=Sprites;
		//SpriteREF = &Sprites[0];
        SPR_setPosition(spr->SpriteA,fix32ToInt(spr->CoordX),fix32ToInt(spr->CoordY));
        SPR_setAnim(spr->SpriteA,0);
        SPR_setAlwaysOnTop(spr->SpriteA,TRUE);
        spr++;
	}

	// Metal Slug !!
	// Init Phase scène : 1 Arrivée du Tank , 2 - Largage de Marco, 3 - Départ du tank, 0 - Démarrage démo.
	PhaseScene=1;
	spr->SpriteA = SPR_addSprite(&MetalSlug_Sprite, 0, 0, TILE_ATTR(PAL3, TRUE, FALSE, FALSE));
	SPR_setAnim(spr->SpriteA,1);
	spr->CoordX=FIX32(-64);
	spr->CoordY=FIX32(140+8);
	spr->Vitesse=FIX32(0.3);
	SPR_setPosition(spr->SpriteA,fix32ToInt(spr->CoordX),fix32ToInt(spr->CoordY));
	SPR_setVisibility(spr->SpriteA,VISIBLE);
	spr->AirUnit=0;
	spr->StandBy=0;
	spr->Direction=6;
	spr->TypeIA=0;
    spr->SpriteDYN=0;
	spr->Visible=1;
	spr->ID=10;
	spr++;

	// Lettre START !
	i=NombreLettre;
	j=0;
	fix32 VX=FIX32(0);
	fix32 VY=FIX32(0);
	PhaseAffichageScene=1;
	while(i--)
	{
        spr->CoordX=FIX32(-32);
        spr->CoordY=FIX32(80);
        spr->MemDir=0;
        spr->Vitesse=FIX32(1)+VX;
        spr->AirUnit=0;
        spr->TypeBouclier=0;
        spr->Direction=0;
        spr->Visible=1;
        spr->TempoSprite=0;
        spr->ID=11;
        spr->StandBy=0;
        spr->TypeIA=0;
        spr->SpriteA = SPR_addSprite(&FontStart_Sprite, 0, 0, TILE_ATTR(PAL2, TRUE, FALSE, FALSE));
        SPR_setPriorityAttribut(spr->SpriteA, TRUE);
        SPR_setVisibility(spr->SpriteA,VISIBLE);
        SPR_setPosition(spr->SpriteA,fix32ToInt(spr->CoordX),fix32ToInt(spr->CoordY));
        SPR_setAnim(spr->SpriteA,j);
        SPR_setAlwaysOnTop(spr->SpriteA,TRUE);
        j++;
        VX+=FIX32(0.5);
        VY+=FIX32(0.1);
        spr->Acceleration=VY;
        spr++;
	}
	NombreIASceneMax=1;
	NombreIAScene=0;
    MaxObjet=NombreLettre+NombreIA+NombreBalle+NombreDigitScore+NombreBouclier;
	GestionNombreBallesShotgun();

	SPR_update();
}


////////////////////////////////////////
//                  Init MAP
////////////////////////////////////////
void InitMAP()
{
	// Init
	SYS_doVBlankProcess();
	VDP_init();

	// Zone 3 ?!
	if (NumeroZone==2)
	{
		Zone3();
		// Une fois connu, on ne reviendra plus dessus.
		RequisZone3=99;
		// Init général
		Clear_Variable();
		GameOver=1;
		VDP_setPaletteColors(0, (u16*) palette_black, 64);
		SPR_end();
		SYS_doVBlankProcess();
		VDP_init();
		StartMain();
		return;
	}

    // set all palette to black
    VDP_setPaletteColors(0, (u16*) palette_black, 64);
	//NumeroZone=1;
	ind = TILE_USERINDEX;
	bgBaseTileIndex[0] = ind;

	switch (NumeroZone)
	{

		case 0:
		// load background tilesets in VRAM
		//VDP_init();
		VDP_loadTileSet(bga_image.tileset, ind, DMA);
		ind +=  bga_image.tileset->numTile;
		bgBaseTileIndex[1] = ind;
		VDP_loadTileSet(&bgb_tileset, ind, DMA);
		ind += bgb_tileset.numTile;

		// init backgrounds
		bgb = MAP_create(&bgb_Map, BG_B, TILE_ATTR_FULL(1, TRUE, FALSE, FALSE, bgBaseTileIndex[1]));
		bga = unpackTileMap(bga_image.tilemap, NULL);
		break;

		case 1:
		// load background tilesets in VRAM
		//VDP_init();
		VDP_loadTileSet(bga_image_1.tileset, ind, DMA);
		ind +=  bga_image_1.tileset->numTile;
		bgBaseTileIndex[1] = ind;
		VDP_loadTileSet(&bgb_tileset_1, ind, DMA);
		ind += bgb_tileset_1.numTile;

		// init backgrounds
		bgb = MAP_create(&bgb_Map_1, BG_B, TILE_ATTR_FULL(1, TRUE, FALSE, FALSE, bgBaseTileIndex[1]));
		bga = unpackTileMap(bga_image_1.tilemap, NULL);
		break;
	}

}


///////////////////////////////
//                  Init Routine
///////////////////////////////
void InitRoutine()
{
	// initialization
    VDP_setScreenWidth320();

    // set all palette to black
    VDP_setPaletteColors(0, (u16*) palette_black, 64);

	// Init Scene
    SND_setPCM_XGM(SFX_GENERIC1, Magnum_SFX, sizeof(Magnum_SFX));
    SND_setPCM_XGM(SFX_GENERIC2, Mort_SFX, sizeof(Mort_SFX));
	SND_setPCM_XGM(SFX_GENERIC3, Mort1_SFX, sizeof(Mort1_SFX));
	SND_setPCM_XGM(SFX_GENERIC4, Couteau_SFX, sizeof(Couteau_SFX));
	SND_setPCM_XGM(SFX_GENERIC5, Crunch_SFX, sizeof(Crunch_SFX));
	SND_setPCM_XGM(SFX_GENERIC6, Aie_SFX, sizeof(Aie_SFX));
	SND_setPCM_XGM(SFX_GENERIC7, Tank_SFX, sizeof(Tank_SFX));
	SND_setPCM_XGM(SFX_GENERIC8, Bombe_SFX, sizeof(Bombe_SFX));
	SND_setPCM_XGM(SFX_GENERIC9, Explosion_SFX, sizeof(Explosion_SFX));
	SND_setPCM_XGM(SFX_GENERIC10, TirBazooka_SFX, sizeof(TirBazooka_SFX));
	SND_setPCM_XGM(SFX_GENERIC11, Medailles_SFX, sizeof(Medailles_SFX));
	SND_setPCM_XGM(SFX_GENERIC12, Impact_SFX, sizeof(Impact_SFX));
	SND_setPCM_XGM(SFX_GENERIC13, Civil_SFX, sizeof(Civil_SFX));
	SND_setPCM_XGM(SFX_GENERIC14, Bonus_SFX, sizeof(Bonus_SFX));
	SND_setPCM_XGM(SFX_GENERIC15, Respawn_SFX, sizeof(Respawn_SFX));
	SND_setPCM_XGM(SFX_GENERIC16, Flamethrower_SFX, sizeof(Flamethrower_SFX));
	SND_setPCM_XGM(SFX_GENERIC17, Grenades_SFX, sizeof(Grenades_SFX));
	SND_setPCM_XGM(SFX_GENERIC18, Health_SFX, sizeof(Health_SFX));
	SND_setPCM_XGM(SFX_GENERIC19, HeavyMachinegun_SFX, sizeof(HeavyMachinegun_SFX));
	SND_setPCM_XGM(SFX_GENERIC20, Machinegun_SFX, sizeof(Machinegun_SFX));
	SND_setPCM_XGM(SFX_GENERIC21, PowerUp_SFX, sizeof(PowerUp_SFX));
	SND_setPCM_XGM(SFX_GENERIC22, RocketLauncher_SFX, sizeof(RocketLauncher_SFX));
	SND_setPCM_XGM(SFX_GENERIC23, Score_SFX, sizeof(Score_SFX));
	SND_setPCM_XGM(SFX_GENERIC24, Uzi_SFX, sizeof(Uzi_SFX));
	SND_setPCM_XGM(SFX_GENERIC25, Baby_SFX, sizeof(Baby_SFX));
	SND_setPCM_XGM(SFX_GENERIC26, Voice_SFX, sizeof(Flamethrower_SFX));


}

