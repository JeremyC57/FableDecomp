// CAIStateGroup_RecoverStealableItem::IsRunnable() -- when the creature may recover items, reset the state, find a target point and a stealable item outside its range; runnable when both are alive.
// Leaf: ?IsRunnable@CAIStateGroup_RecoverStealableItem@@UAE_NXZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00965320, length 512 bytes (+ int3 padding). Vtable slot 11.
// Callees: CVectorMap::LowerBound const @ 0x004365B0 (inlined GetInterface: 0xaa = bit 10 of +0x34 on the creature, 0xb2
// = bit 18 of +0x34 on the target point; both miss paths read the never-written key slot), CBaseIntelligentPointer
// ctor/SetPItem/GetPItem/dtor @ 0x00A01B10/0x00A01B90/0x00A01B50/0x00A01C10 (CIntelligentPointer<CThing> vtable 0x01238C6C,
// CIntelligentPointer<const CThing> vtable 0x01237F1C), FindPTargetPoint @ 0x009652C0,
// FindPStealableItemOutsideTargetPointRange(const C3DVector&, float) @ 0x00965210, CThing::GetPos @ 0x004C73D0.
// Layout: engine/CAIStateGroup_RecoverStealableItem.h (PStealableItem +0x14, PTargetPoint +0x1c, PPreviouslyCarriedItem
// +0x24, GotActionPoint +0x2c, ActionPointIndex +0x30, ReachedDestination +0x34, PickUpActionID +0x38, NeedsCleanup +0x3c)
// over the landed local CAIStateGroupBase model (the generated header flattens the vptr).
// Shape: one nested `if` chain with a single `return false` (retail pushes edi before the first test; the early exits
// share the `xor al,al` epilogue); IS_THING_ALIVE is `__forceinline` (VC out-lines the fourth expansion otherwise); the
// target point is taken into a named pointer before the range read (retail calls GetPItem before pushing the range);
// the final `&&` returns through the full register (`mov eax,1` / `xor eax,eax`).
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
#include "engine/C3DVector.h"

class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
struct CIntelligentPointerThing : CBaseIntelligentPointer {        // CIntelligentPointer<CThing> (vtable 0x01238C6C)
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { CThing* item = ((CThing*)other.GetPItem()); SetPItem((CBaseObject*)item); return *this; }
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // CIntelligentPointer<const CThing> (vtable 0x01237F1C)
    virtual void v00();
    CIntelligentPointerConstThing(CThing* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointerConstThing& operator=(const CIntelligentPointerConstThing& other) { CThing* item = ((CThing*)other.GetPItem()); SetPItem((CBaseObject*)item); return *this; }
};
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (((CThing*)p.GetPItem()) != 0) {
        if (!((CThing*)p.GetPItem())->IsDead())
            return true;
    }
    return false;
}
struct CTCStealer {                             // interface 0xaa
    unsigned char _pad[0x69];
    bool CanRecoverItems;                       // +0x69
};
struct CTCTargetPoint {                         // interface 0xb2
    unsigned char _pad[0x0c];
    float Range;                                // +0x0c
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();
    virtual void OnReplace();
    virtual bool IsRunnable();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
};
struct CAIStateGroup_RecoverStealableItem : CAIStateGroupBase {
    CIntelligentPointerThing PStealableItem;    // +0x14
    CIntelligentPointerConstThing PTargetPoint; // +0x1c
    CIntelligentPointerThing PPreviouslyCarriedItem;   // +0x24
    bool GotActionPoint;                        // +0x2c
    long ActionPointIndex;                      // +0x30
    bool ReachedDestination;                    // +0x34
    long PickUpActionID;                        // +0x38
    bool NeedsCleanup;                          // +0x3c
    CThing* FindPTargetPoint();                 // 0x009652C0
    CThing* FindPStealableItemOutsideTargetPointRange(const C3DVector& pos, float range);   // 0x00965210
    virtual bool IsRunnable();
};

bool CAIStateGroup_RecoverStealableItem::IsRunnable()
{
    if (!NeedsCleanup) {
        const CTCStealer* stealer;
        PBrain->PCreature->PeekTC(TCI_AI_SCRATCHPAD, &stealer);
        if (stealer->CanRecoverItems) {
            ActionPointIndex = -1;
            GotActionPoint = false;
            ReachedDestination = false;
            PickUpActionID = -1;
            PStealableItem = CIntelligentPointerThing(0);
            PTargetPoint = CIntelligentPointerConstThing(FindPTargetPoint());
            if (IS_THING_ALIVE(PTargetPoint)) {
                const CTCTargetPoint* target;
                ((CThing*)PTargetPoint.GetPItem())->PeekTC(TCI_STEALABLE_ITEM_LOCATION, &target);
                CThing* point = ((CThing*)PTargetPoint.GetPItem());
                PStealableItem = CIntelligentPointerThing(FindPStealableItemOutsideTargetPointRange(point->GetPos(), target->Range));
            }
            return IS_THING_ALIVE(PStealableItem) && IS_THING_ALIVE(PTargetPoint);
        }
    }
    return false;
}