// NCameraModes::CScriptModuleUseLamp::OnInitLeaf() -- when the lamp and the creature are alive: orbit origin at the lamp's targeting-cursor helper point (else its position), axes from the creature-to-lamp direction (flattened, normalised; X the perpendicular), radius four times the distance, height 0.5, and per-frame deltas toward a random side
// Leaf: ?OnInitLeaf@CScriptModuleUseLamp@NCameraModes@@MAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008539C0, 475 bytes, thiscall (vtable slot 8, protected virtual).
// Source: FableWin.pdb debug body 0x0283BEC0 (cm_scripting_controls.obj) statement order with Ego_d.pdb's block locals (theta_end,
//   height_end, mag, rad_end): IS_THING_ALIVE on PLamp then PCreature, GetTCGraphicAppearance()->GetHelperPoint(TARGETING_CURSOR_POSITION_NAME,
//   0, 0, Origin, 1.0f) (0x004C0560; the name 0x013BAC50 is the PDB / Anniversary-PDB global, landed 0x00661B40 spelling), GetPos,
//   operator-, GetAccurateMagnitude, operator/=, CrossWithUp, GFRandom(2, GetOwner().GetPWorldSeed()), GetDuration(). rad_end repeats
//   Rad's product (retail keeps it: `fst [esp+10h]`). Float literals are retail's dwords (4.0f, 0.5f, 2/3).
// Class: new overlay header engine/NCameraModes_CScriptModuleUseLamp.h (Ego_d LF_CLASS 0x3C150).
// Inferred: (owner's 0x006E89B0 / 0x00F361D0 precedents) two no-code PAREN tuples, both in shared helper text (overlay engine/C3DVector.h
//   opt-ins): FABLE_RETAIL_C3DVECTOR_ACCURATE_MAGNITUDE_PAREN_RETURN (`return ((float)sqrt(magnitude));` in GetAccurateMagnitude) and
//   FABLE_RETAIL_C3DVECTOR_DIVASSIGN_RECIPROCAL_PAREN (`float reciprocal = (1.0f / v);` in operator/=). Same instructions; they move VC7.1's
//   81-tuple scheduling-region cut (c2 list-scheduler trace, hand892 sched tools) so retail's order comes out at 0x00853A90..0x00853A98 (AxisY copy: `fstp [esp+20h]` before the `mov edx,[esp+1Ch]` reload)
//   and 0x00853B4B (`test eax,eax` before the store of the unsigned side). Without them DIFFER(475v475: both sites swapped).
//   One tuple alone, CrossWithUp `(-X)`, a parenthesised Rad, or tuples in the subtraction (SUBTRACT_X / XY_GROUP), in CrossWithUp or as
//   three grouped squares do not match both OnInitLeaf rows; GetAccurateMagnitude through GetSquaredMagnitude matches both but contradicts
//   FableWin's body 0x01D99FB0 and breaks landed 0x00875690. A row-side `(AxisY.GetAccurateMagnitude())` is byte-equal to the
//   PAREN_RETURN opt-in. Both opt-ins leave every landed user on the default path matching (16/16 ACCURATE_MAGNITUDE users, ~45 C3DVector
//   users); forced on, PAREN_RETURN breaks 0x00875690 and 0x00761280, so it is a per-TU opt-in, not a general reading. The same pair
//   closes the twin row (0x00853670 OpenChest). No donor shows the parentheses: coordinator to decide.
#include "fable_compiler.h"
#include <math.h>
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
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
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FOCAL
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CIntelligentPointer.h"
#define FABLE_RETAIL_IS_THING_ALIVE
#include "engine/ThingAlive.h"
#include "engine/GFRandom.h"
#define FABLE_RETAIL_NCAMERAMODES_CBASE_MEMBERS
#include "engine/NCameraModes_CBase.h"
#include "engine/NCameraModes_CScriptModuleUseLamp.h"
extern const CCharString TARGETING_CURSOR_POSITION_NAME;           // 0x013BAC50

namespace NCameraModes {
void CScriptModuleUseLamp::OnInitLeaf()
{
    if (IS_THING_ALIVE(PLamp) && IS_THING_ALIVE(PCreature)) {
        if (!PLamp->GetTCGraphicAppearance()->GetHelperPoint(TARGETING_CURSOR_POSITION_NAME, 0, 0, Origin, 1.0f))
            Origin = PLamp->GetPos();
        AxisY = Origin - PCreature->GetPos();
        AxisY.Z = 0.0f;
        float mag = AxisY.GetAccurateMagnitude();
        AxisY /= mag;
        AxisX = AxisY.CrossWithUp();
        Theta = 0.0f;
        Rad = mag * 4.0f;
        Height = 0.5f;
        float theta_end = ((float)GFRandom(2, GetOwner().GetPWorldSeed()) - 0.5f) * (2.0f / 3.0f);
        float rad_end = mag * 4.0f;
        float height_end = 0.5f;
        DeltaTheta = (theta_end - Theta) / GetDuration();
        DeltaRad = (rad_end - Rad) / GetDuration();
        DeltaHeight = (height_end - Height) / GetDuration();
    }
}
}