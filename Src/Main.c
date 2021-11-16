
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

// Proc�dure principale

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

	// Init MAP
	InitMAP();

	// Init Scene
	InitScene();

	// Fade In Scene.
    VDP_fadeInAll(palette,15,TRUE);

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
		UpdateScene();

        // update sprites
        if (!PauseGame) SPR_update();

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

	// R�sultat du score
	Result_Screen();
	NumeroZone++;
	if (NumeroZone>1) NumeroZone=0;

	// Init g�n�ral
	Clear_Variable();
    VDP_setPaletteColors(0, (u16*) palette_black, 64);
	SPR_end();
	VDP_init();
	StartMain();
	return;
}


