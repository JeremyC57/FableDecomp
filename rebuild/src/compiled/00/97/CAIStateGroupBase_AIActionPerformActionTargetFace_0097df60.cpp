// CAIStateGroupBase::AIAction_PerformActionTargetFace(CCreatureActionBase* action, CIntelligentPointer<const CThing> target) -- post the action together with a face-the-target look (flags 0x23) for one brain tick; 1 when stopping, else 2
// Leaf: ?AIAction_PerformActionTargetFace@CAIStateGroupBase@@QAE?AW4EAIActionReturn@@PAVCCreatureActionBase@@UCIntelligentPointerConstThing@@@Z
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0097DF60, length 470 bytes (`ret 0Ch`).
// Callees: IsStopping @ 0x008FD010, CBaseIntelligentPointer GetPItem @ 0x00A01B50 / ctor @ 0x00A01B10 / SetPItem @ 0x00A01B90 /
//          dtor @ 0x00A01C10 (CIntelligentPointer<const CThing> vtable 0x01237F1C), the action's virtual slot 11 (+0x2c, Clone),
//          operator new @ 0x00BFEA1A / delete @ 0x00BFE9BC (the 12-byte count block, destroy routine @ 0x00668610),
//          CAIBrain::SetBrainOutput @ 0x0088B8F0 (manifest "CQuestManager::RemoveAvailableQuestCardType" is propagated),
//          the brain's virtual slot 2 (+8, yield).
// Shape: the CBrainOutput ctor/dtor are inline (the landed 0x0097D580 model); SetAsLookAtThing inline (Types |= 4 after
//          the thing argument, a temporary intelligent pointer assigned into LookAtThing, then the flags) and an inline
//          SetAsPerformAction (Types |= 8, counted-pointer operator=(T*): release, object, null block, then `new`
//          SCountBlock for a non-null clone).
// Layout: the base follows engine/CAIStateGroupBase.h (PBrain +4), modelled locally so the member functions can be declared;
//          CBrainOutput per Ego_r.pdb / the landed 0x00835620 (Types +0, LookFlags +4, PAction +0x20, LookAtThing +0x40).
#include "fable_compiler.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // CIntelligentPointer<const CThing>, vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointerConstThing() : CBaseIntelligentPointer() { }
    CIntelligentPointerConstThing(const CThing* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)((CThing*)item)); }
    CIntelligentPointerConstThing& operator=(const CIntelligentPointerConstThing& other) { SetPItem((CBaseObject*)(((CThing*)other.GetPItem()))); return *this; }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (((CThing*)p.GetPItem()) != 0) {
        if (!((CThing*)p.GetPItem())->IsDead())
            return true;
    }
    return false;
}
class CCreatureActionBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10();
    virtual CCreatureActionBase* Clone() const; // slot 11 (+0x2c)
};
void __fastcall CountedDestroy(void* object);   // 0x00668610
struct SCountBlock {
    long m_references;
    void (__fastcall* m_pfnDestroy)(void*);
    void* m_pObject;
    SCountBlock(void* object) : m_references(1), m_pfnDestroy(CountedDestroy), m_pObject(object) { }
};
struct CCountedPointer_CCreatureActionBase {
    CCreatureActionBase* m_pObject;
    SCountBlock*         m_pCount;
    CCountedPointer_CCreatureActionBase() : m_pObject(0), m_pCount(0) { }
    FABLE_FORCEINLINE void Release()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
    }
    FABLE_FORCEINLINE void Reset(CCreatureActionBase* object)
    {
        Release();
        m_pCount = 0;
        m_pObject = object;
    }
    FABLE_FORCEINLINE CCountedPointer_CCreatureActionBase& operator=(CCreatureActionBase* object)
    {
        Release();
        m_pObject = object;
        m_pCount = 0;
        if (object)
            m_pCount = new SCountBlock(m_pObject);
        return *this;
    }
    FABLE_FORCEINLINE ~CCountedPointer_CCreatureActionBase()
    {
        Release();
        m_pObject = 0;
        m_pCount = 0;
    }
};
class CNavigationInput { public:
    float Destination[3];
    float Proximity;
    long  MovementType;
    float Speed;
    CNavigationInput() : Proximity(0.0f), MovementType(0), Speed(0.0f) { }
};
class CBrainOutput { public:
    unsigned long                       Types;            // +0x00
    long                                LookFlags;        // +0x04
    CNavigationInput                    NavigationInput;  // +0x08
    CCountedPointer_CCreatureActionBase PAction;          // +0x20
    C3DVector                           MoveToPos;        // +0x28
    C3DVector                           LookAtPos;        // +0x34
    CIntelligentPointerConstThing       LookAtThing;      // +0x40
    FABLE_FORCEINLINE void SetAsLookAtThing(const CThing* thing, long lookFlags)
    {
        Types |= 4;
        LookAtThing = CIntelligentPointerConstThing(thing);
        LookFlags = lookFlags;
    }
    FABLE_FORCEINLINE void SetAsPerformAction(const CCreatureActionBase* action)
    {
        Types |= 8;
        PAction = action->Clone();
    }
    FABLE_FORCEINLINE CBrainOutput() : NavigationInput(), PAction(), LookAtThing()
    {
        Types = 0;
        LookFlags = 0;
        PAction.Reset(0);
    }
};
class CAIBrain { public:
    virtual void v00(); virtual void v01();
    virtual void Yield();                       // slot 2 (+0x08)
    void SetBrainOutput(const CBrainOutput& output);   // 0x0088B8F0
};
enum EAIActionReturn { AI_ACTION_STOPPING = 1, AI_ACTION_DONE = 2, AI_ACTION_RUNNING = 4 };
class CAIStateGroupBase { public:
    virtual void v00();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
protected:
    bool IsStopping() const;                          // 0x008FD010 (Ego_r.pdb protected const)
public:
    EAIActionReturn AIAction_PerformActionTargetFace(CCreatureActionBase* action, CIntelligentPointerConstThing target);
};

EAIActionReturn CAIStateGroupBase::AIAction_PerformActionTargetFace(CCreatureActionBase* action, CIntelligentPointerConstThing target)
{
    if (IsStopping())
        return AI_ACTION_STOPPING;
    if (!IS_THING_ALIVE(target))
        return AI_ACTION_DONE;
    {
        CBrainOutput output;
        output.SetAsLookAtThing(((CThing*)target.GetPItem()), 0x23);
        output.SetAsPerformAction(action);
        PBrain->SetBrainOutput(output);
    }
    PBrain->Yield();
    if (IsStopping())
        return AI_ACTION_STOPPING;
    return AI_ACTION_DONE;
}