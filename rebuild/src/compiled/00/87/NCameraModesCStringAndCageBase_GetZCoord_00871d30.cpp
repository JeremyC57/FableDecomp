// NCameraModes::CStringAndCageBase::GetZCoord() const -- the observed thing's height plus the current height offset
// Leaf: ?GetZCoord@CStringAndCageBase@NCameraModes@@ABEMXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00871D30, 19 bytes:
//   push esi ; mov esi,ecx ; call 0x0084F1B0 (CBase::GetObservedThing) ; mov ecx,eax ; call 0x004C73D0 (CThing::GetPos) ;
//   fld dword [eax+8] (pos.Z) ; fadd dword [esi+0B0h] (CurrentHeightOffset) ; pop esi ; ret
// Name fix (NF202, 2026-11-20, q12240): was CTCLook::UpdateLookTimer over GetA / GetB stand-ins. Evidence: (1) both PDB name sets hold
//   ?GetZCoord@CStringAndCageBase@NCameraModes@@ABEMXZ (Ego_r 0x0085F880, FableWin 0x029D03C0: a private const member returning float),
//   placed immediately before ?UpdateCageRadius@CStringAndCageBase@NCameraModes@@ in both (retail 0x00871D30 immediately precedes the
//   landed UpdateCageRadius 0x00871D50); (2) the body: `this` is a camera mode (it calls NCameraModes::CBase::GetObservedThing
//   0x0084F1B0 on itself) and +0xB0 is CStringAndCageBase::CurrentHeightOffset (engine/NCameraModes_CStringAndCageBase.h member order;
//   the landed 0x00871D50 / 0x00872050 layouts). Ported onto that header (FABLE_RETAIL_NCAMERAMODES_CSTRINGANDCAGEBASE_GETZCOORD) with the
//   CBase mode of the landed UpdateXYVars 0x00871FD0 plus FABLE_RETAIL_NCAMERAMODES_CBASE_GETOBSERVEDTHING. CThingGameObject comes from the shared
//   engine/CThingGameObject.h (retail chain FABLE_RETAIL_CTHING .. CTHINGGAMEOBJECT); only GetPos is used.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_NCAMERAMODES_CBASE_TYPED_COMPOSITES
#define FABLE_RETAIL_NCAMERAMODES_CBASE_MEMBERS
#define FABLE_RETAIL_NCAMERAMODES_CBASE_STRING_CAGE
#define FABLE_RETAIL_NCAMERAMODES_CSTRINGANDCAGEBASE_GETZCOORD
#define FABLE_RETAIL_NCAMERAMODES_CBASE_GETOBSERVEDTHING
#include "engine/NCameraModes_CStringAndCageBase.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CThingGameObject.h"

float NCameraModes::CStringAndCageBase::GetZCoord() const
{
    return GetObservedThing()->GetPos().Z + CurrentHeightOffset;
}
