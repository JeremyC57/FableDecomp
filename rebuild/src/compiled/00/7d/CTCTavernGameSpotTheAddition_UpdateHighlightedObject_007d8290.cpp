// CTCTavernGameSpotTheAddition::UpdateHighlightedObject() -- finds the knick-knack nearest the pointer (within twice the def's knick-knack radius), records the relative distance, deselects when it is beyond the radius, and plays the selection-change sound when the selection changes
// Leaf: ?UpdateHighlightedObject@CTCTavernGameSpotTheAddition@@QAEXXZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007D8290, 291 bytes (plain `ret`).
// Callees: CDefClassBase::GetSubDef<CTavernGameSpotTheAdditionDef>(CDefPointer<const CTavernGameSpotTheAdditionDef>*)
//          0x007D9A80 (on the owner's def class, owner +0x70; this TU, see the stand-in below); CThing::GetPos()
//          const 0x004C73D0; CCharString(const char*, long) 0x0099EBF0 ("TAVERN_GAME_OBJECT_SELECTION_CHANGE" 0x0125603C);
//          CTCTavernGame::PlaySoundW(const CCharString&) 0x005EA950; ~CCharString 0x0099EAE0; the def's slot 1 on release.
// Layout (retail, PDB - 0x18): CTCTavernGameSpotTheAddition Pointer +0x104, DistFromObject +0x10c, KnickKnacks
//          (vector<KnickKnack>, 16-byte elements, Thing at +0x0c) +0x110, Selected +0x144;
//          CTavernGameSpotTheAdditionDef::KnickKnackRadius +0x5c (PDB +0x64, two 12-byte vectors before it).
// Shape: the radius is a named float local (copied through edx into its frame word); the best distance starts at
//        `radius + radius`, the index counter shares the def's zero register; the planar distance is
//        sqrt(dx*dx + dy*dy) with the fsqrt intrinsic (VC emits the Y term first); the sound name is a block-scoped
//        named string; the def was dereferenced, so its release has no null test.
// NF49 (round 7): the def classes come from the shared headers (were local CTavernGameSpotTheAdditionDef / CSpotTheAdditionDefPointer / CDefClassBase models and a same-TU
// stand-in GetSubDef body over made-up FindRef_CDefClassBase_GetSubDef_0 callees): the call at 0x007D9A80 is the member template
// ??$GetSubDef@VCTavernGameSpotTheAdditionDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCTavernGameSpotTheAdditionDef@@@@@Z (Ego_r.pdb / FableWin.pdb), its body visible
// and kept out of line (FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE, the 0x0066BC10 idiom).
// Naming (NF159, 2026-11-20, q4273): ported onto engine/CTCTavernGameSpotTheAddition.h (retail branch) from the local
//   CTCTavernGameSpotTheAddition / KnickKnack / owner stand-ins: ?PlaySoundW@CTCTavernGame@@IAEKABVCCharString@@@Z (protected; was
//   ?PlaySound@CTCTavernGameSpotTheAddition@@QAEKABVCCharString@@@Z); the def comes from ParentThing->PeekBaseDef() (CThing::PBaseDef +0x70), the
//   pointer / knick-knack things are CThingObject (CThing base at +0, as the landed 0x007D8570 reads them). Own leaf unchanged.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CRIGHTHANDEDSET
#include "fable_compiler.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CTavernGameDef.h"
#include "engine/CTavernGameSpotTheAdditionDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CTCTavernGame.h"
#include "engine/CTCTavernGameSpotTheAddition.h"
#include <math.h>
#pragma intrinsic(sqrt)

void CTCTavernGameSpotTheAddition::UpdateHighlightedObject()
{
    CDefPointer<const CTavernGameSpotTheAdditionDef> def;
    ParentThing->PeekBaseDef().GetSubDef(&def);
    float radius = def->KnickKnackRadius;
    float bestDist = radius + radius;
    long selected = -1;
    DistFromObject = -1.0f;
    long index = 0;
    for (KnickKnack* knick = KnickKnacks.begin(); knick != KnickKnacks.end(); ++knick, ++index) {
        const C3DVector& pointerPos = ((CThing*)Pointer)->GetPos();
        const C3DVector& pos = ((CThing*)knick->Thing)->GetPos();
        float dx = pos.X - pointerPos.X;
        float dy = pos.Y - pointerPos.Y;
        float dist = (float)sqrt(dx * dx + dy * dy);
        if (dist < bestDist) {
            bestDist = dist;
            selected = index;
        }
    }
    if (selected != -1)
        DistFromObject = bestDist / radius;
    if (bestDist > radius)
        selected = -1;
    if (selected != Selected) {
        Selected = selected;
        CCharString sound("TAVERN_GAME_OBJECT_SELECTION_CHANGE", -1);
        PlaySoundW(sound);
    }
}
