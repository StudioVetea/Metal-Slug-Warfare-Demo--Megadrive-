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
//                  Gestion PAD
////////////////////////////////////////
void GestionPAD(Sprite1_ *spr)
{
    // Phase scene en cours ? On quitte !
    if (PhaseScene || GameOver || spr->LancerGrenade) return;

    u16 value=JOY_readJoypad(JOY_1);

    // Pause en jeu ?!
    if (!value) StatutJoy++;
    if (StatutJoy>4) StatutJoy=4;
    if (StatutJoy>2)
    {
        if (value & BUTTON_START && !PauseGame)
        {
            PauseGame=1;
            XGM_pausePlay();
            StatutJoy=0;
            SND_startPlayPCM_XGM(SFX_GENERIC14, 2, SOUND_PCM_CH4);
            return;
        }

        if (value & BUTTON_START && PauseGame)
        {
            PauseGame=0;
            XGM_resumePlay();
            StatutJoy=0;
            TestPause=0;
            VDP_fadeInAll(palette,4,FALSE);
            SND_startPlayPCM_XGM(SFX_GENERIC14, 2, SOUND_PCM_CH4);
            return;
        }
    }

    if (PauseGame) return;



    // Init
    spr->Direction=0;

    // Lancer grenade joueur & Saut
    if (!spr->Saut)
    {
        // Lancer grenade ?!
        if (value & BUTTON_C && !spr->LancerGrenade && NombreGrenade)
        {
            spr->LancerGrenade=1;
            spr->TempoSprite=0;
            u16 i=NombreBalle;
            Sprite1_* spr1 = &Sprites[IDBalle];
            while(i--)
            {
                //spr1 = &Sprites[IDBalle+j];
                if (spr1->StandBy)
                {
                    NombreGrenade--;
                    GestionNombreGrenades();
                    spr1->StandBy=0;
                    spr1->TempoRafale=0;
                    spr1->ID=88;     // ID Grenade
                    spr1->Hit=0;
                    SND_startPlayPCM_XGM(SFX_GENERIC4, 1, SOUND_PCM_CH3);
                    //SPR_setAnim(spr1->SpriteA,8);
                    spr1->Visible=1;
                    spr1->Vitesse=FIX32(1.75);
                    spr1->TypeIA=0;
                    spr1->CoordX=spr->CoordX;
                    spr1->CoordY=spr->CoordY+spr->DeltaY+FIX32(48);
                    spr1->OffsetY=FIX32(-10);
                    spr1->Direction=spr->MemDir;
                    if (spr1->Direction==4) spr1->OffsetX=FIX32(0);
                    else  spr1->OffsetX=FIX32(-16);
                    spr1->Saut=1;
                    spr1->Acceleration=FIX32(4);
                    spr1->RefY=FIX32(140+50);
                    spr1->SensY=1;
                    break;
                }
                spr1++;
            }
            return;
        }

        // Saut ?
        if (value & BUTTON_B)
        {
            //if (spr->MemSprint) spr->Boost=FIX32(2);
            spr->Saut=1;
            spr->Acceleration=FIX32(6);
            spr->RefY=spr->CoordY;
            spr->SensY=1;
        }
    }
    // Déplacement
    if (value & BUTTON_UP) spr->Direction=88;
    if (value & BUTTON_DOWN) spr->Direction=2;

    if (value & BUTTON_RIGHT)
    {
        if (spr->Direction==2) spr->Direction=26;
        else if (spr->Direction==88) spr->Direction=86;
        else spr->Direction=6;
        //if (spr->Feu==1 && spr->MemDir==4)
        //{
           // if (spr->Direction==2) spr->Direction=26;
           // else spr->Direction=0;
       // }
    }
    else if (value & BUTTON_LEFT)
    {
        if (spr->Direction==2) spr->Direction=24;
        else if (spr->Direction==88) spr->Direction=84;
        else spr->Direction=4;
        //if (spr->Feu==1 && spr->MemDir==6)
       // {
           // if (spr->Direction==2) spr->Direction=26;
           // else spr->Direction=0;
       // }
    }

    // Tir ?! On ne tire par en marchant accroupi.
    if (spr->Direction!=26 && spr->Direction!=24 )
    {
        if (value & BUTTON_A && !spr->Feu)
        {
            spr->Feu=1;
            // Test Collision couteau ?
            if (!spr->Saut)
            {
                u16 NbrIA=NombreIA;
                Sprite1_* SprIA=&Sprites[IDUnite];
                while(NbrIA--)
                {
                    if (!SprIA->StandBy && SprIA->Visible && !SprIA->MortIA && SprIA->ID!=5 && SprIA->ID!=7)
                    {
                        if (spr->MemDir==6 || !spr->MemDir || spr->Direction==6)
                        {
                            fix32 DX_IA=abs(spr->CoordX - SprIA->CoordX);
                            if (DX_IA<=FIX32(32) && (spr->CoordX<SprIA->CoordX))
                            {
                                spr->Couteau=1;
                                spr->TempoCouteau=0;
                                SND_startPlayPCM_XGM(SFX_GENERIC4, 1, SOUND_PCM_CH3);
                                break;
                            }
                        }
                        if (spr->MemDir==4 || spr->Direction==4)
                        {
                            fix32 DX_IA=abs(spr->CoordX - SprIA->CoordX);
                            if (DX_IA<=FIX32(40) && (spr->CoordX>=SprIA->CoordX))
                            {
                                spr->Couteau=1;
                                spr->TempoCouteau=0;
                                SND_startPlayPCM_XGM(SFX_GENERIC4, 1, SOUND_PCM_CH3);
                                break;
                            }
                        }
                    }
                    SprIA++;
                }
            }

            // Tir au pistolet ?
            if (!spr->Couteau)
            {
                u16 i=NombreBalle;
                Sprite1_* spr1 = &Sprites[IDBalle];

                // Tir en haut ?!
                //if (value & BUTTON_UP) spr->Direction=88;
                while(i--)
                {
                    if (spr1->StandBy)
                    {
                        if (spr->Slot1)
                        {
                            NombreBalleShotgun--;
                            GestionNombreBallesShotgun();
                        }
                        spr1->StandBy=0;
                        spr->TempoRafale=0;
                        spr1->TempoSprite=0;
                        spr1->ID=45;
                        spr1->Hit=0;
                        spr1->Vitesse=FIX32(8);
                        switch (spr->Slot1)
                        {
                            case 0:
                            SND_startPlayPCM_XGM(SFX_GENERIC1, 1, SOUND_PCM_CH3);
                            spr1->DegatArme=1;
                            break;
                            case 1:
                            SND_startPlayPCM_XGM(SFX_GENERIC10, 1, SOUND_PCM_CH3);
                            spr1->DegatArme=3;
                            break;
                            case 2:
                            SND_startPlayPCM_XGM(SFX_GENERIC24, 1, SOUND_PCM_CH3);
                            spr1->DegatArme=2;
                            break;
                            case 3:
                            SND_startPlayPCM_XGM(SFX_GENERIC10, 1, SOUND_PCM_CH3);
                            spr1->DegatArme=10;
                            spr1->Vitesse=FIX32(3);
                            break;
                            case 10:
                            SND_startPlayPCM_XGM(SFX_GENERIC10, 1, SOUND_PCM_CH3);
                            spr1->Vitesse=FIX32(6);
                            spr1->DegatArme=8;

                            break;
                        }

                        if (!NombreBalleShotgun) spr->Slot1=0;

                        //SPR_setAnim(spr1->SpriteA,0);
                        spr1->Visible=1;
                        spr1->CoordY=spr->CoordY+spr->DeltaY+FIX32(48);
                        spr1->CoordX=spr->CoordX;
                        // Tir en haut !
                        if (spr->Direction>40)
                        {
                            //if (value & BUTTON_RIGHT && spr->Direction==88) spr->DirectionTir=56;
                            //if (value & BUTTON_LEFT && spr->Direction==88) spr->DirectionTir=54;
                            spr1->OffsetY=FIX32(24);
                            spr1->Direction=8;
                            if (!spr->MemDir || spr->MemDir==6) spr1->OffsetX=FIX32(8);
                            else spr1->OffsetX=FIX32(-8);
                            //if (spr->DirectionTir==56) spr1->OffsetX=FIX32(-16);
                            //if (spr->DirectionTir==54) spr1->OffsetX=FIX32(-32);
                            break;
                        }
                        else
                        {
                            spr1->OffsetY=FIX32(-7);
                            spr1->OffsetX=FIX32(26);
                            spr1->Direction=spr->MemDir;
                            if (!spr->MemDir) spr1->Direction=6;
                            if (spr1->Direction==6)
                            {
                                spr1->OffsetX=FIX32(-26);
                                SPR_setAnim(spr->SpriteA,9);
                            }
                            if (spr->Direction==26 || spr->Direction==24 || spr->Direction==2) spr1->OffsetY=FIX32(-17);
                        }
                        break;
                    }
                    spr1++;
                }
            }
            }
        }


    // Mémorisation direction joueur
    if (spr->Direction && spr->Direction!=2 && spr->Direction!=88)
    {
        spr->MemDir=spr->Direction;
        if (spr->MemDir==24 || spr->MemDir==84) spr->MemDir=4;
        if (spr->MemDir==26 || spr->MemDir==86) spr->MemDir=6;
    }
}
