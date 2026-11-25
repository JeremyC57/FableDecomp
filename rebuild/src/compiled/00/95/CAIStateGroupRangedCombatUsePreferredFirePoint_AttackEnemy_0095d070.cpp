// CAIStateGroup_RangedCombatUsePreferredFirePoint::AttackEnemy() -- when the creature may attack and stands within
// Leaf: ?AttackEnemy@CAIStateGroup_RangedCombatUsePreferredFirePoint@@IAE_NXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// 0.05 (squared 0.0025) of its fire point in the XY plane, pump the combat actions; false when they report 1.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0095D070, length 108 bytes (+ int3 padding).
// Callees: the creature's attack-readiness test @ 0x00662410 (bool; manifest "C3DAnimationBlendState::
// AddCombinationBlend" is propagated), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 on PFirePoint at +0x1c,
// CThing::GetPosition @ 0x004C73D0 (fire point, then creature), CAIStateGroupBase::AIAction_PumpCombatActions
// @ 0x0097D980. Global: the squared fire-point tolerance float @ 0x0129531C (0.0025f).
// Shape (retry 10b): three plain early returns (`if (!CanAttackNow()) return false; if (IsAwayFromFirePoint()) return
// false; if (Pump() == 1) return false; return true;`); the creature is read through an inline GetCreature() accessor
// (PBrain->PCreature) at both uses -- the accessor keeps the first `xor al,al` exit after the first test and gives the
// second PBrain load ecx as retail; the 2D delta (creature - fire point) is built on the x87 stack by a by-value
// `operator-` and `X*X + Y*Y` compared with `fcomp [g] ; test ah,41h` (greater-than).
// PFirePoint is the engine header name (the header flattens the base so the class is modelled locally).
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
#include "fable_compiler.h"
class CTCBase;
class CThing;
class CWorld;
class CThingAICreature;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x0040F020 -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class C3DVector;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.Get() != 0) {
        if (!p.Get()->IsDead())
            return true;
    }
    return false;
}

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

// NF51 (round 7): the AI state-group base comes from the shared engine/CAIStateGroupBase.h (was a local CAIStateGroupBase model plus a
// made-up CAIStateGroupBaseC view carrying AIAction_PumpCombatActions): the callee 0x0097D980 is the protected
// ?AIAction_PumpCombatActions@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@XZ (Ego_r.pdb 0x00848740; header _MEMBERS + _PUMPCOMBATACTIONS);
// the row's GetCreature helper moves to the derived class.
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PUMPCOMBATACTIONS
#include "engine/fable_ai_action_enums.h"
#include "engine/CAIStateGroupBase.h"

long __fastcall GFGetCurrentFrame();            // 0x0049D870 (manifest "GFGetPreMainMemoryUsed" is propagated)
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890

class C2DVector { public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector operator-(const C2DVector& o) const { return C2DVector(X - o.X, Y - o.Y); }
    float MagnitudeSquared() const { return X * X + Y * Y; }
};

class C3DVector : public C2DVector { public:    // the X/Y head is the C2DVector base, so the 2D arithmetic below binds the position directly
    float Z;
};
struct CThingPos : CThing {
};

extern const float g_firePointToleranceSquared;    // 0x0129531C (0.0025f)

struct CAIStateGroup_RangedCombatUsePreferredFirePoint : CAIStateGroupBase {
    CThing* GetCreature() const { return PBrain->PCreature; }   // (the row's helper, moved here from its local base model)
    CIntelligentPointer<CThing> PEnemy;             // +0x14 CIntelligentPointer<CThingCreatureBase>
    CIntelligentPointer<CThing> PFirePoint;         // +0x1c CIntelligentPointer<CThing>
    long TimeOutCounter;                        // +0x24
    protected:
    bool AttackEnemy();
    public:
    // the creature is a named local read before the fire-point position; the 2D delta is built by the by-value
    // `operator-` and `X*X + Y*Y` compared with the tolerance
    FABLE_FORCEINLINE bool IsAwayFromFirePoint()
    {
        CThingPos* creature = (CThingPos*)GetCreature();
        const C2DVector& firePos = ((CThingPos*)PFirePoint.Get())->GetPos();
        return (creature->GetPos() - firePos).MagnitudeSquared() > g_firePointToleranceSquared;
    }
};

// Naming (NF438 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration (no layout; rulings r2/r3):
//   0x00662410 ?IsInMeleeAsAttacker@CThingCreatureBase@@QBE_NXZ (was ?CanAttackNow@CThingPos@@QAE_NXZ)
class CThingCreatureBase { public: bool IsInMeleeAsAttacker() const; };
bool CAIStateGroup_RangedCombatUsePreferredFirePoint::AttackEnemy()
{
    if (!reinterpret_cast<const CThingCreatureBase*>(((CThingPos*)GetCreature()))->IsInMeleeAsAttacker())
        return false;
    if (IsAwayFromFirePoint())
        return false;
    if (AIAction_PumpCombatActions() == 1)
        return false;
    return true;
}