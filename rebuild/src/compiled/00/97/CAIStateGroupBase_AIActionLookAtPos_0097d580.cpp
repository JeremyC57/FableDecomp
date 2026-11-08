// CAIStateGroupBase::AIAction_LookAtPos(EAIActionRepeat, C3DVector, long, float) -- turn the creature to look at a position, yielding until it does (or once)
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0097D580, length 366 bytes (`ret 18h`). Ego_r.pdb
//          ?AIAction_LookAtPos@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@VC3DVector@@JM@Z.
// Callees: CAIStateGroupBase::IsStopping @ 0x008FD010, the creature's look-at test @ 0x006650E0 (manifest
//          "CThingCreatureBase::CanSeeTarget" placeholder; (const C3DVector&, float, bool, bool) -> bool),
//          CBaseIntelligentPointer ctor @ 0x00A01B10 / dtor @ 0x00A01C10, CAIBrain::SetBrainOutput @ 0x0088B8F0
//          (manifest "CQuestManager::RemoveAvailableQuestCardType" is propagated), the brain's virtual slot 2 (+8, yield),
//          operator delete @ 0x00BFE9BC.
// Shape: the two look flags are `(flags & N) > 0` bools written in the call; VC hoists them out of the loop with
//          `setg` straight into frame bytes (one in the dead flags slot, the flags live in ebx); the creature is a named
//          local (ecx loads before the argument pushes); each pass reports done (2) or posts a look-at brain output (type bit 2,
//          LookAtPos, LookFlags) through the inlined CBrainOutput ctor/dtor (the landed 0x00835620 body, its
//          Reset(0) release kept) and reports 4 (running); repeat mode 1 loops while running.
// Layout: CBrainOutput per Ego_r.pdb / the landed 0x00835620 (Types +0, LookFlags +4, PAction +0x20, LookAtPos +0x34,
//          LookAtThing +0x40); CNavigationInput's Proximity/Speed are floats, so their zero stores are immediates while
//          MovementType uses the zero register. The base follows engine/CAIStateGroupBase.h (PBrain +4).
#include "fable_compiler.h"
class CThing;
#include "engine/C3DVector.h"

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CCreatureActionBase;
struct SCountBlock {
    long m_references;
    void (__fastcall* m_pfnDestroy)(CCreatureActionBase*);
    CCreatureActionBase* m_pObject;
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
    FABLE_FORCEINLINE ~CCountedPointer_CCreatureActionBase()
    {
        Release();
        m_pObject = 0;
        m_pCount = 0;
    }
};

class CNavigationInput { public:                       // PDB: Destination +0, Proximity +0xc, MovementType +0x10, Speed +0x14
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
    CIntelligentPointer<const CThing>                 LookAtThing;      // +0x40
    FABLE_FORCEINLINE CBrainOutput() : NavigationInput(), PAction(), LookAtThing()
    {
        Types = 0;
        LookFlags = 0;
        PAction.Reset(0);
    }
};

class CThingCreatureBase { public:
    bool IsLookingAtPos(const C3DVector& pos, float tolerance, bool alignBody, bool alignHead);   // 0x006650E0
};

class CAIBrain { public:
    virtual void v00(); virtual void v01();
    virtual void Yield();                       // slot 2 (+0x08)
    unsigned char _pad04[0x18 - 0x04];
    CThingCreatureBase* Creature;               // +0x18
    void SetBrainOutput(const CBrainOutput& output);   // 0x0088B8F0
};

enum EAIActionRepeat { AI_ACTION_ONCE = 0, AI_ACTION_REPEAT = 1 };
enum EAIActionReturn { AI_ACTION_STOPPING = 1, AI_ACTION_DONE = 2, AI_ACTION_RUNNING = 4 };

class CAIStateGroupBase { public:
    virtual void v00();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
protected:
    bool IsStopping() const;                          // 0x008FD010 (Ego_r.pdb protected const)
public:
protected:
    EAIActionReturn AIAction_LookAtPos(EAIActionRepeat repeat, C3DVector pos, long lookFlags, float tolerance);
public:
};

EAIActionReturn CAIStateGroupBase::AIAction_LookAtPos(EAIActionRepeat repeat, C3DVector pos, long lookFlags, float tolerance)
{
    if (IsStopping())
        return AI_ACTION_STOPPING;
    EAIActionReturn result;
    do {
        CThingCreatureBase* creature = PBrain->Creature;
        if (creature->IsLookingAtPos(pos, tolerance, (lookFlags & 2) > 0, (lookFlags & 4) > 0))
            result = AI_ACTION_DONE;
        else {
            CBrainOutput output;
            output.Types |= 2;
            output.LookAtPos = pos;
            output.LookFlags = lookFlags;
            PBrain->SetBrainOutput(output);
            result = AI_ACTION_RUNNING;
        }
        PBrain->Yield();
        if (IsStopping())
            return AI_ACTION_STOPPING;
    } while (repeat == AI_ACTION_REPEAT && result == AI_ACTION_RUNNING);
    return result;
}