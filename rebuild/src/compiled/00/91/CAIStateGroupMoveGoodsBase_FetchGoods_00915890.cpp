// CAIStateGroup_MoveGoodsBase::FetchGoods(CIntelligentPointer<CThing>, float, vector<long>, CAIFilterBase*, CThing*) -- target the container (or the next crate of its crate stack), walk up to it and face it, then perform the fetch action with the good for the top crate, free the action point, take the crate off the stack and return true; false when the walk fails, the target dies or the action fails
// Leaf: ?FetchGoods@CAIStateGroup_MoveGoodsBase@@QAE_NUCIntelligentPointerThing@@MUCLongVector@@PAUCAIFilterBase@@PAVCThing@@@Z
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00915890, length 720 bytes (+ int3 padding), `ret 20h`, `and esp,-8` frame.
// Callees: CBaseIntelligentPointer ctor @ 0x00A01B10 / GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10,
// CVectorMap::LowerBound @ 0x0040F020 (GetInterface 0xB5 = CTCCrateStack, bit 21 of +0x34; 0x2D = CTCUserActionPointBase),
// CTCCrateStack::GetNextCrate @ 0x007B6750 / GetNumCrates @ 0x007B6660 / RemoveNextCrate @ 0x007B6790,
// AIAction_MoveToPossiblyBlockedObjectAndFace @ 0x00980770, the fetch action ctor @ 0x00916590 / ~CCreatureActionBase
// @ 0x00693EF0, CCreatureActionBase::GetActionID @ 0x00692620, AIAction_PerformActionAndWait @ 0x0097DB70,
// CTCUserActionPointBase::SetActionPointAsFree @ 0x007FEF40, free @ 0x00BFEA14 (the by-value vector's storage).
// Layout: local model (PBrain +4; brain PCreature +0x18; CurrentActionID +0x18; retail CThing bitset +0x20, map +0x44, dead bit +0x91;
// STLport vector 12 bytes; the fetch action is CCreatureActionBase (0xa8) + a CIntelligentPointer<CThing> at +0xa8).
// Shape: one shared false tail (success path nested); goods.Start read into a local after the walk; the target item hoisted into a local before the action ctor; the crate-stack lookup is the word-local HasTC in the first test and the masked test (null-checked) in the second.
// Name fix (agentNF7 2026-11-10): the local CIntelligentPointer wrapper structs replaced by the shared CIntelligentPointer<T> (engine/CIntelligentPointer.h), so the vtables bind ??_7?$CIntelligentPointer@VCThing@@@@6B@ 0x01238C6C / ??_7?$CIntelligentPointer@$$CBVCThing@@@@6B@ 0x01237F1C.
#include "fable_compiler.h"
extern "C" void __cdecl free(void* block);
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingCreatureBase : public CThing { public: };
class CTCCrateStack { public:                          // interface 0xB5
    CThing* GetNextCrate();                     // 0x007B6750
    long GetNumCrates();                        // 0x007B6660
    CThing* RemoveNextCrate();                  // 0x007B6790
};
struct CTCUserActionPointBase {
public:
    void SetActionPointAsFree(long a0);   // 0x007FEF40 (Ego_r.pdb ?SetActionPointAsFree@CTCUserActionPointBase@@QAEXJ@Z)
public:                 // interface 0x2D
};
class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#include "engine/CIntelligentPointer.h"
// CIntelligentPointerThing = the shared CIntelligentPointer<CThing> (engine/CIntelligentPointer.h).
FABLE_FORCEINLINE bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (((CThing*)p.GetPItem()) != 0) {
        if (!((CThing*)p.GetPItem())->IsDead())
            return true;
    }
    return false;
}
struct CLongVector {                            // STLport vector<long>, 12 bytes
    long* Start;
    long* Finish;
    long* EndOfStorage;
    ~CLongVector() { if (Start) free(Start); }
};
struct CAIFilterBase;
class CCreatureActionBase { public:
    virtual void v00();
    unsigned char _body[0xa4];
    virtual ~CCreatureActionBase();                     // 0x00693EF0
    long GetActionID() const;                   // 0x00692620
};
struct CCreatureAction_FetchGoods : CCreatureActionBase {
    CIntelligentPointer<CThing> PTarget;           // +0xa8
    CCreatureAction_FetchGoods(CThingCreatureBase* creature, CThing* target, long good);   // 0x00916590
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
enum EAIActionReturn { };
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x10];
    long CurrentActionID;                       // +0x18
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    protected: EAIActionReturn AIAction_PerformActionAndWait(const CCreatureActionBase& action, bool wait); public:   // 0x0097DB70
    long AIAction_MoveToPossiblyBlockedObjectAndFace(CIntelligentPointer<CThing> target, long* actionPoint, float distance,
        float faceDistance, CAIFilterBase* filter, CThing* ignore, CThing* other, float speed);   // 0x00980770
};
struct CAIStateGroup_MoveGoodsBase : CAIStateGroupBase {
    bool FetchGoods(CIntelligentPointer<CThing> container, float distance, CLongVector goods, CAIFilterBase* filter, CThing* ignore);
};

bool CAIStateGroup_MoveGoodsBase::FetchGoods(CIntelligentPointer<CThing> container, float distance, CLongVector goods,
    CAIFilterBase* filter, CThing* ignore)
{
    CIntelligentPointer<CThing> target;
    target.SetPItem((CBaseObject*)(((CThing*)container.GetPItem())));
    CTCCrateStack* stack = 0;
    if (((CThing*)container.GetPItem())->GetTC(TCI_CRATE_STACK, &stack))
        target = CIntelligentPointer<CThing>(stack->GetNextCrate());
    long actionPoint;
    if (AIAction_MoveToPossiblyBlockedObjectAndFace(target, &actionPoint, distance, 10.0f, filter, ignore, 0, 1.0f) == 2
            && IS_THING_ALIVE(target)) {
        long* data = goods.Start;
        long index;
        CTCCrateStack* crates;
        if (((CThing*)container.GetPItem())->GetTC(TCI_CRATE_STACK, &crates) && crates)
            index = crates->GetNumCrates() - 1;
        else
            index = 0;
        CThing* item = ((CThing*)target.GetPItem());
        CCreatureAction_FetchGoods action(GetCreature(), item, data[index]);
        CurrentActionID = action.GetActionID();
        if (AIAction_PerformActionAndWait(action, false) == 2) {
            if (IS_THING_ALIVE(target)) {
                CTCUserActionPointBase* point;
                ((CThing*)target.GetPItem())->GetTC(TCI_SINGLE_USER_ACTION_POINTS, &point);
                ((CTCUserActionPointBase*)point)->SetActionPointAsFree(actionPoint);
            }
            if (((CThing*)container.GetPItem()) && stack)
                stack->RemoveNextCrate();
            return true;
        }
    }
    return false;
}