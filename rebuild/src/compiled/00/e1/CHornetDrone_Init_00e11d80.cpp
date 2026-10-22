#include "engine/CHornetDrone.h"  // retyped onto the PDB layout; byte parity re-verified
#include <cstdlib>


void __fastcall Init(CHornetDrone* drone)
{
    drone->EatNow = (rand() % 200) + 1;
}