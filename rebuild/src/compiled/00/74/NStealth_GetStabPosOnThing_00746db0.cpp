// NStealth::GetStabPosOnThing(CThing& thing) -- the point a stealth stab aims at: the thing's "STAB_POS" helper point, else midway between its position and its left-eye dummy (the position itself when it has no such dummy)
// Leaf: ?GetStabPosOnThing@NStealth@@YI?AVC3DVector@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00746DB0, 247 bytes, __fastcall (hidden C3DVector result in ecx, the thing in edx), plain `ret`.
// Name: Ego_r.pdb tc_stealth.obj #5 / FableWin.pdb debug\tc_stealth.obj #1 ?GetStabPosOnThing@NStealth@@YI?AVC3DVector@@AAVCThing@@@Z (the
//   catalog's `_global::GetStabPosOnThing` lacked the namespace).
// Callees: CTCGraphicAppearance::GetHelperPoint 0x004C0560, CThing::GetPos 0x004C73D0, CTCGraphicAppearance::GetDummyObject 0x004C08C0;
//   globals STAB_POS 0x013BACDC and EYE_LEFT_DUMMY_OBJECT_NAME 0x013BAC54 (rebuild/oracles/reloc_bindings.tsv), 0.5f 0x0122F59C.
// Source: Ego_d.pdb locals pos; {rhset, target_pos, eye_pos} in the miss block; FableWin.pdb debug body 0x025916F0 gives the statement
//   order (GetTCGraphicAppearance()->GetHelperPoint, target_pos copied from GetPos(), eye_pos / rhset default-constructed, GetDummyObject,
//   `eye_pos = target_pos` on a miss, `pos = (eye_pos + target_pos) * 0.5f`). Ported from the old local model onto the shared headers
//   with these names: RELOCATION_MATCH at the first grade (the old model's x/y/z copy register residue was the local model's).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_CTOR_INLINE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_DUMMYOBJECT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FOCAL
#define FABLE_RETAIL_C3DVECTOR
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CRightHandedSet.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
extern const CCharString STAB_POS;                      // 0x013BACDC "STAB_POS"
extern const CCharString EYE_LEFT_DUMMY_OBJECT_NAME;    // 0x013BAC54 "EYE_SET_L"
namespace NStealth {
C3DVector FABLE_FASTCALL GetStabPosOnThing(CThing& thing)
{
    C3DVector pos;
    if (!thing.GetTCGraphicAppearance()->GetHelperPoint(STAB_POS, 0, 0, pos, 1.0f)) {
        C3DVector target_pos = thing.GetPos();
        C3DVector eye_pos;
        CRightHandedSet rhset;
        if (!thing.GetTCGraphicAppearance()->GetDummyObject(EYE_LEFT_DUMMY_OBJECT_NAME, 0, 0, eye_pos, rhset, 1.0f))
            eye_pos = target_pos;
        pos = (eye_pos + target_pos) * 0.5f;
    }
    return pos;
}
}