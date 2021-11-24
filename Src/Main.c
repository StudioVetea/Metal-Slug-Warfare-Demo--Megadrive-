
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

// Procédure principale

int main(u16 hardreset)
{
	// Init HARD
	if (!hardreset) SYS_hardReset();

	// Init HW
	InitRoutine();

	// Start !
	StartMain();

	return 0;
}

//////////////////////////////////////////
//               Init Main
/////////////////////////////////////////
void StartMain()
{
	// Ecran intro
	InitIntro();

	// Ecran sélection Zone
	InitEcranZone();

	// Init MAP
	InitMAP();

	// Init Scene
	InitScene();

	// Fade In Scene.
    VDP_fadeInAll(palette,15,FALSE);

    // Main Loop
    MainLoop();
    return;
}

//////////////////////////////////////////
//               Boucle Principale
/////////////////////////////////////////
void MainLoop()
{
	while(TRUE)
	{
		// Scene principale
		RandomSeed();
		UpdateScene();

		// Pause
		if (PauseGame)
		{
			TestPause++;
			if (TestPause<10) PAL_fadeOutAll(6,TRUE);
			else PAL_interruptFade();
		}

		// Vblank
		SYS_doVBlankProcess();

		// Update Scrolling BG.
		updateVDPScroll();

		// Game Over ?!
		if (GameOver==2) break;
	}
	// set all palette to black
    VDP_setPaletteColors(0, (u16*) palette_black, 64);
	XGM_stopPlay();
	SPR_end();
	VDP_init();
	MEM_free(bgb);
	MEM_free(bga);

	// Résultat du score
	Result_Screen();
	NumeroZone++;
	if (NumeroZone>1) NumeroZone=0;

	// Init général
	Clear_Variable();
    VDP_setPaletteColors(0, (u16*) palette_black, 64);
	SPR_end();
	VDP_init();
	StartMain();
	return;
}



