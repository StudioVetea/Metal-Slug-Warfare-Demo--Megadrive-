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
void GestionPAD()
{
    // Phase scene en cours ? On quitte !
    if (PhaseScene ||GameOver) return;

    u16 value=JOY_readJoypad(JOY_1);

    // Init Joueur
    Sprite1_* spr=Sprites;

    // Lancer de grenade en cours ?! On quitte !
    if (spr->LancerGrenade) return;

	spr = &Sprites[0];
    spr->Direction=0;
    spr->DirectionTir=0;
    //spr->Sprint=0;
    //spr->Boost=FIX32(0);

    // Lancer grenade joueur & Saut
    if (!spr->Saut)
    {
        // Lancer grenade ?!
        if (value & BUTTON_C && !spr->LancerGrenade && NombreGrenade)
        {
            spr->LancerGrenade=1;
            spr->TempoSprite=0;
            u16 i=NombreBalle,j=0;
            Sprite1_* spr1=Sprites;
            spr1 = &Sprites[IDBalle];
            while(i--)
            {
                spr1 = &Sprites[IDBalle+j];
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
                j++;
            }
            return;
        }

        // Saut ?
        if (value & BUTTON_B)
        {
            if (!spr->Saut && !spr->Feu)
            {
                //if (spr->MemSprint) spr->Boost=FIX32(2);
                spr->Saut=1;
                spr->Acceleration=FIX32(6);
                spr->RefY=spr->CoordY;
                spr->SensY=1;
            }
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
                Sprite1_* SprIA=Sprites;
                SprIA=&Sprites[IDUnite];
                while(NbrIA--)
                {
                    if (!SprIA->StandBy && SprIA->Visible && !SprIA->MortIA && SprIA->ID!=5)
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
                u16 i=NombreBalle,j=0;
                Sprite1_* spr1=Sprites;
                spr1 = &Sprites[IDBalle];

                // Tir en haut ?!
                if (value & BUTTON_UP) spr->Direction=88;
                if (value & BUTTON_RIGHT && spr->Direction==88) spr->Direction=56;
                if (value & BUTTON_LEFT && spr->Direction==88) spr->Direction=54;
                while(i--)
                {
                    spr1 = &Sprites[IDBalle+j];
                    if (spr1->StandBy)
                    {
                        if (spr->Slot1)
                        {
                            NombreBalleShotgun--;
                            GestionNombreBallesShotgun();
                            if (!NombreBalleShotgun)
                            {
                                spr->Slot1=0;
                            }
                        }
                        spr1->StandBy=0;
                        spr->TempoRafale=0;
                        spr1->ID=45;
                        spr1->Hit=0;
                        if (!spr->Slot1) SND_startPlayPCM_XGM(SFX_GENERIC1, 1, SOUND_PCM_CH3);
                        else SND_startPlayPCM_XGM(SFX_GENERIC10, 1, SOUND_PCM_CH3);
                        //SPR_setAnim(spr1->SpriteA,0);
                        spr1->Visible=1;
                        spr1->Vitesse=FIX32(8);
                        spr1->CoordY=spr->CoordY+spr->DeltaY+FIX32(48);
                        if (spr->Direction>40)
                        {
                            spr1->OffsetY=FIX32(24);
                            spr1->Direction=8;
                            spr1->OffsetX=FIX32(-24);
                            if (spr->Direction==56) spr1->OffsetX=FIX32(-16);
                            if (spr->Direction==54) spr1->OffsetX=FIX32(-32);
                            spr1->CoordX=spr->CoordX+spr1->OffsetX;
                            break;
                        }
                        else
                        {
                            spr1->OffsetY=FIX32(-7);
                            spr1->OffsetX=FIX32(0);
                            spr1->Direction=spr->MemDir;
                            if (!spr->MemDir) spr1->Direction=6;
                            if (spr1->Direction==6)
                            {
                                if  (spr->Slot1) spr1->OffsetX=FIX32(16);
                                else spr1->OffsetX=FIX32(-16);
                                SPR_setAnim(spr->SpriteA,9);
                            }
                           spr1->CoordX=spr->CoordX+spr1->OffsetX;
                            if (spr->Direction==26 || spr->Direction==24 || spr->Direction==2) spr1->OffsetY=FIX32(-17);
                        }
                        break;
                    }
                    j++;
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
