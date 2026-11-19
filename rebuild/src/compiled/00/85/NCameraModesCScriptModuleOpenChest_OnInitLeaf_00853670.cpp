// NCameraModes::CScriptModuleOpenChest::OnInitLeaf() -- when the chest and the creature are alive: orbit origin at the chest, axes from the creature-to-chest direction (flattened, normalised; X the perpendicular), radius twice the distance, height 3, and per-frame deltas toward a random side, 1.6x the distance and height 1.5
// Leaf: ?OnInitLeaf@CScriptModuleOpenChest@NCameraModes@@MAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00853670, 445 bytes, thiscall (vtable slot 8, protected virtual).
// Source: FableWin.pdb debug body 0x0283B7D0 (cm_scripting_controls.obj) statement order with Ego_d.pdb's block locals (theta_end,
//   height_end, mag, rad_end): IS_THING_ALIVE<CIntelligentPointer<const CThing> > on PChest then PCreature, GetPos, operator-,
//   GetAccurateMagnitude, operator/=, CrossWithUp, GetOwner().GetPWorldSeed(), GFRandom(2, seed), GetDuration() (the donor's debug-only
//   ASSERT(Origin != ...) emits nothing). Float literals are retail's (0.5f, 2/3, 1.6f, 1.5f dwords); FableWin's are doubles.
// Class: new overlay header engine/NCameraModes_CScriptModuleOpenChest.h (Ego_d LF_CLASS 0x3C140 + Ego_r layout).
// Inferred: (owner's 0x006E89B0 / 0x00F361D0 precedents) two no-code PAREN tuples, both in shared helper text (overlay engine/C3DVector.h
//   opt-ins): FABLE_RETAIL_C3DVECTOR_ACCURATE_MAGNITUDE_PAREN_RETURN (`return ((float)sqrt(magnitude));` in GetAccurateMagnitude) and
//   FABLE_RETAIL_C3DVECTOR_DIVASSIGN_RECIPROCAL_PAREN (`float reciprocal = (1.0f / v);` in operator/=). Same instructions; they move VC7.1's
//   81-tuple scheduling-region cut (c2 list-scheduler trace, hand892 sched tools) so retail's order comes out at 0x008537A4..0x008537AD (Theta store, owner load, Rad store, Height store). Without them DIFFER(445v445: Rad stored
//   before Theta, owner load after Height).
//   One tuple alone, CrossWithUp `(-X)`, a parenthesised Rad, or tuples in the subtraction (SUBTRACT_X / XY_GROUP), in CrossWithUp or as
//   three grouped squares do not match both OnInitLeaf rows; GetAccurateMagnitude through GetSquaredMagnitude matches both but contradicts
//   FableWin's body 0x01D99FB0 and breaks landed 0x00875690. A row-side `(AxisY.GetAccurateMagnitude())` is byte-equal to the
//   PAREN_RETURN opt-in. Both opt-ins leave every landed user on the default path matching (16/16 ACCURATE_MAGNITUDE users, ~45 C3DVector
//   users); forced on, PAREN_RETURN breaks 0x00875690 and 0x00761280, so it is a per-TU opt-in, not a general reading. The same pair
//   closes the twin row (0x008539C0 UseLamp). No donor shows the parentheses: coordinator to decide.
#include "fable_compiler.h"
#include <math.h>
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ACCURATE_MAGNITUDE
#define FABLE_RETAIL_C3DVECTOR_ACCURATE_MAGNITUDE_PAREN_RETURN
#define FABLE_RETAIL_C3DVECTOR_CROSSWITHUP
#define FABLE_RETAIL_C3DVECTOR_DIVASSIGN_RECIPROCAL_PAREN
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
#define FABLE_RETAIL_IS_THING_ALIVE
#include "engine/ThingAlive.h"
#include "engine/GFRandom.h"
#define FABLE_RETAIL_NCAMERAMODES_CBASE_MEMBERS
#include "engine/NCameraModes_CBase.h"
#include "engine/NCameraModes_CScriptModuleOpenChest.h"

namespace NCameraModes {
void CScriptModuleOpenChest::OnInitLeaf()
{
    if (IS_THING_ALIVE(PChest) && IS_THING_ALIVE(PCreature)) {
        Origin = PChest->GetPos();
        AxisY = Origin - PCreature->GetPos();
        AxisY.Z = 0.0f;
        float mag = AxisY.GetAccurateMagnitude();
        AxisY /= mag;
        AxisX = AxisY.CrossWithUp();
        Theta = 0.0f;
        Rad = mag * 2.0f;
        Height = 3.0f;
        float theta_end = ((float)GFRandom(2, GetOwner().GetPWorldSeed()) - 0.5f) * (2.0f / 3.0f);
        float rad_end = mag * 1.6f;
        float height_end = 1.5f;
        DeltaTheta = (theta_end - Theta) / GetDuration();
        DeltaRad = (rad_end - Rad) / GetDuration();
        DeltaHeight = (height_end - Height) / GetDuration();
    }
}
}