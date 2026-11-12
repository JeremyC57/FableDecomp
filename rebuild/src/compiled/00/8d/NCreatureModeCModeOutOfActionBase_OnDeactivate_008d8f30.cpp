// NCreatureMode::CModeOutOfActionBase::OnDeactivate() -- the creature comes back into action: resume its animation if it was in state 1, clear targeting flag 4, wake its physics, kill the out-of-action emitter if it is still alive, then the stand-mode deactivation
// Leaf: OnDeactivate
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D8F30, 161 bytes (`ret`).
// Callees: CThingAICreature::SuspendAI(bool, long) 0x00833A10; CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
// 0x0040F020 (inlined HasTC/GetTC, type 9 = bit 9 of +0x20); the type-9 component's flag setter 0x007EA270 (beside
// CTCTargeted::IsTargetable 0x007EA280; manifest "CObservable::AddExclusiveObserver" is propagated); the physics object's
// (+0x60) virtual slot 44 (+0xb0, wake); CBaseIntelligentPointer::GetPItem 0x00A01B50 (three times); CThing::Kill(bool)
// 0x004C9B80; CModeStandBase::OnDeactivate 0x00861D00 (qualified base call).
// Layout (PDB CModeOutOfActionBase 0x78, byte-packed tail): CModeStandBase 0x68, RecoveryCount +0x68, Recoverable +0x6c,
// PEmitter +0x6d (CIntelligentPointer<CThing> at an odd offset: the class is compiled under pack(1)).
// Shape: HasTC + a returned GetTC whose end/hit choice is one ternary (the component lands straight in ecx); ParentThing is re-read for every use; the flag clear is the component's inline ClearFlag (`if (flags & f) SetFlags(flags ^ f)`, `this` straight in ecx); IS_THING_ALIVE as in the
// landed 0x008C92C0 (GetPItem re-called for the null test, the dead bit and the call).
class CTCBase;

#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

struct CTCPhysicsBase {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043();
    virtual void Wake();   // slot 44 (+0xb0)
};

class CThingAICreature { public:
    void SuspendAI(bool suspend, void* suspender);   // 0x00833A10 (Ego_r.pdb CThingAICreature::SuspendAI)
};
struct CThingAnimatable {                  // empty here: only its member is called (thing base at offset 0)
};

class CTCTargeted { public:
    void SetTargetingFlags(unsigned long a0);   // 0x007EA270 (Ego_r.pdb ?SetTargetingFlags@CTCTargeted@@QAEXK@Z)
};
struct CTCTargetedFlags {                  // the type-9 component: a flags word at +0xc with an out-of-line setter
    unsigned char _pad00[0x0c];
    unsigned long m_flags;                 // +0x0c
    void ClearFlag(unsigned long flag)
    {
        if (m_flags & flag)
            ((CTCTargeted*)this)->SetTargetingFlags(m_flags ^ flag);
    }
};

#pragma pack(push, 1)
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

#pragma pack(pop)

inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.Get() != 0) {
        if (!p.Get()->IsDead())
            return true;
    }
    return false;
}

struct CModeStandBase {                    // retail sizeof 0x68
    virtual void v00();
    CThing*       ParentThing;             // +0x04
    unsigned char _body[0x60];
    virtual void OnDeactivate();           // 0x00861D00 ?OnDeactivate@CModeStandBase@@UAEXXZ (Ego_r/FableWin; vtable 0x01276D8C slot 6)
};

namespace NCreatureMode {

#pragma pack(push, 1)
struct CModeOutOfActionBase : CModeStandBase {
    long                    RecoveryCount; // +0x68
    bool                    Recoverable;   // +0x6c
    CIntelligentPointer<CThing> PEmitter;      // +0x6d
    void OnDeactivate();
};
#pragma pack(pop)

void CModeOutOfActionBase::OnDeactivate()
{
    if (ParentThing->GetType() == 1)
        ((CThingAICreature*)ParentThing)->SuspendAI(false, 0);
    if (ParentThing->HasTC(TCI_TARGETED)) {
        ((CTCTargetedFlags*)&ParentThing->GetTC(TCI_TARGETED))->ClearFlag(4);
    }
    ParentThing->GetPhysicsTC()->Wake();
    if (IS_THING_ALIVE(PEmitter))
        PEmitter.Get()->Kill(true);
    CModeStandBase::OnDeactivate();
}

}