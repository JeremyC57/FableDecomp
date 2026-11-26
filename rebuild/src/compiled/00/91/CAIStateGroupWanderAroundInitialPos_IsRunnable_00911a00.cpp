// CAIStateGroup_WanderAroundInitialPos::IsRunnable() -- runnable once the creature strays more than 10 units from its initial position
// Leaf: ?IsRunnable@CAIStateGroup_WanderAroundInitialPos@@UAE_NXZ
// NF473 (r467b / r469c respell, unit with ctor 0x008FD757 + ??_G 0x008FF704): the self is the PDB spelling
//   ?IsRunnable@CAIStateGroup_WanderAroundInitialPos@@UAE_NXZ (Ego_r.pdb + FableWin.pdb name sets; public virtual bool, slot 11 of the RTTI
//   .?AVCAIStateGroup_WanderAroundInitialPos@@ vftable 0x01289460, its only retail reference 0x0128948C), was the non-virtual QAE_NXZ. The base and the
//   brain come from the shared headers (engine/CAIStateGroupBase.h, engine/CAIBrain.h: Creature +0x18), replacing the local CAIStateGroupBase /
//   CAIBrain / CAIBrainCreature models; no engine header exists for CAIStateGroup_WanderAroundInitialPos, so the class is declared as its ctor row
//   0x008FD757 declares it. Header debt kept (unchanged HEAD text): the local CThingAICreature view (InitialPos at +0xE8 / +0xEC) and
//   CBaseIntelligentPointer / IS_THING_ALIVE (unused by this body); CTCWanderControl stays the landed descriptive callee class (0x007F6150).
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: map pointer made const (the row grades the const LowerBound) re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00911A00, length 137 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x004365B0 (the second instantiation of the interface-map search; inlined
// GetInterface on the brain's creature, type 8 = bit 8 of the presence word at +0x20), the type-8 component's
// bool query @ 0x007F6150 (manifest label "CTCUserActionPointBase::IsActionPointFree" is propagated across a
// byte-identical body; kept descriptive here), CThing::GetPosition @ 0x004C73D0 (landed name).
//
// Shape: `GetInterface(&c, 8) && c->IsEnabled()`, then the creature (re-read into esi) compares its current
// position with the planar initial position stored at +0xe8/+0xec: dx and dy are both left on the x87 stack,
// `d = dx*dx; d += dy*dy;` squares dx first (`fld st(1) ; fmul st(2)` twice; the `dx*dx + dy*dy` form
// squares the top of stack first), `> 100.0f` is the
// `test ah,41h ; jne` false branch. Separate `mov al,1` / `xor al,al` exits. No engine header exists.
class CTCBase;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with LowerBound @ 0x004365B0 -- the landed out-parameter
// GetInterface template (0x007504E0 / 0x00930DC0 shape): the component lands in eax, and a miss leaves the
// caller's out pointer unwritten (the frame slot that also holds the key).
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

#include "engine/C3DVector.h"

struct CBaseIntelligentPointer {                // CIntelligentPointer<T>: vptr + item
    void*   __vftable;                          // +0x00
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}

struct CTCWanderControl {                       // interface type 8 component (descriptive)
    bool IsEnabled() const;                           // 0x007F6150
};

class CThingAICreature : public CThing { public:
    unsigned char _pad2[0xe8 - sizeof(CThing)];
    float InitialPosX;                          // +0xe8 (descriptive: the wander centre)
    float InitialPosY;                          // +0xec
};

#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"

class CAIStateGroup_WanderAroundInitialPos : public CAIStateGroupBase {
public:
    CAIStateGroup_WanderAroundInitialPos();                                 // 0x008FD757
    virtual bool IsRunnable();                                                     // slot 11 0x00911A00
    virtual void Update();                                                         // slot 12 0x00911A90
};
FABLE_STATIC_ASSERT(sizeof(CAIStateGroup_WanderAroundInitialPos) == 0x14);

bool CAIStateGroup_WanderAroundInitialPos::IsRunnable()
{
    const CTCWanderControl* control;
    if (PBrain->Creature->PeekTC(TCI_TARGETING, &control) && control->IsEnabled()) {
        CThingAICreature* creature = PBrain->Creature;
        C3DVector* position = (C3DVector*)&creature->GetPos();
        float dx = creature->InitialPosX - position->X;
        float dy = creature->InitialPosY - position->Y;
        float distanceSq = dx * dx;
        distanceSq += dy * dy;
        if (distanceSq > 100.0f)
            return true;
    }
    return false;
}