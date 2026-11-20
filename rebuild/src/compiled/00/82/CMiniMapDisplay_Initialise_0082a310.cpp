// CMiniMapDisplay::Initialise(float) -- drop every map marker (clear the marker list) and clear the mini-map flag byte at 0x013B86C4
// Leaf: ?Initialise@CMiniMapDisplay@@QAEXM@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0082A310, 18 bytes: `add ecx,20h ; call 0x00440730 ; mov byte [0x013B86C4],0 ; ret 4` (the float
//   argument is unused).
// Identity (two sources): Ego_r.pdb 0x00732DE0 + FableWin.pdb 0x027CF940 ?Initialise@CMiniMapDisplay@@QAEXM@Z, after RemoveAllMarkers in both
//   PDBs as in retail (0x0082A300 -> 0x0082A310); the landed caller 0x0064E83F binds this spelling. Markers (STLport list<CMarkerInfo>, retail
//   +0x20) as the landed RemoveAllMarkers 0x0082A300 clears it.
// Callee: ?clear@?$_List_base@VCMarkerInfo@CMiniMapDisplay@@V?$allocator@VCMarkerInfo@CMiniMapDisplay@@@std@@@std@@QAEXXZ 0x00440730 (the
//   landed self row's spelling; 0x0082A300 / 0x00442610 bind it).
// Global: the byte at 0x013B86C4 keeps the established descriptive spelling ?g_miniMapFlag@@3EA (no PDB data symbol found; only this row binds it).
// NF171 (2026-11-20, q5132): re-authored on engine/CMiniMapDisplay.h (NF171 opt-in FABLE_RETAIL_CMINIMAPDISPLAY_INITIALISE); was the free
//   __fastcall stand-in ?CMiniMapDisplay_Initialise@@YIXPAVCMiniMapDisplay@@M@Z over a local CMiniMapDisplay model, binding 0x00440730 as
//   ?Sub_Reset@@YIXPAUSub@@@Z.
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CMINIMAPDISPLAY
#define FABLE_RETAIL_CMINIMAPDISPLAY_INITIALISE
#include "engine/CMiniMapDisplay.h"

extern unsigned char g_miniMapFlag;   // 0x013B86C4

void CMiniMapDisplay::Initialise(float)
{
    Markers.clear();
    g_miniMapFlag = 0;
}
