// CAIStateGroupBase::AIAction_DoNothing() -- idle action: unless stopping, point the creature's look component at the nearest interesting thing and tick the brain; returns 1 (stopping) or 4 (keep going)
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0097E200, length 112 bytes.
// Callees: CAIStateGroupBase::IsStopping @ 0x008FD010 (twice), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
// @ 0x0040F020 (the inlined GetInterface, interface 0x43 = CTCLook: bitset word 2, bit 3 -> `test byte ptr [eax+28h],8`),
// CTCLook::TrackNearestInterestingThing(bool) @ 0x00753840, the brain's virtual slot 2 (+0x08).
// Shape: the result is an action-status enum: an early `return 1` (`mov eax,1`) and a final
// `IsStopping() ? 1 : 4` lowered branch-free (`neg al ; sbb eax,eax ; and eax,-3 ; add eax,4`).
// The look component comes from the landed GetInterface out-parameter template inside the `if`; it must be
// `__forceinline` here (plain inline emits it out of line next to the two IsStopping calls).
// Model: local (engine/CAIStateGroupBase.h and engine/CAIBrain.h carry the vptr as a plain word, and the brain
// call here is virtual); PDB member names kept.
// Leaf: ?AIAction_DoNothing@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@XZ
// Name fix (agentNF5 2026-11-10): Ego_r.pdb / FableWin.pdb ?AIAction_DoNothing@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@XZ (protected;
//   the action-status enum is EAIActionReturn), was ?AIAction_DoNothing@CAIStateGroupBase@@QAE?AW4EAIActionResult@@XZ.
#include "fable_compiler.h"
class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingAICreature { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound(type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};

class CTCLook { public:
    bool TrackNearestInterestingThing(bool onlyIfNone);   // 0x00753840
};

class CAIBrain { public:
    virtual void v00(); virtual void v01();
    virtual void Update();                      // slot 2 (+0x08)
    unsigned char _pad[0x14];
    CThingAICreature* Creature;                 // +0x18
};

enum EAIActionReturn { AI_ACTION_FINISHED = 1, AI_ACTION_CONTINUE = 4 };

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
protected:
    bool IsStopping() const;                          // 0x008FD010 (Ego_r.pdb protected const)
    EAIActionReturn AIAction_DoNothing();         // protected (Ego_r / FableWin: IAE)
};

EAIActionReturn CAIStateGroupBase::AIAction_DoNothing()
{
    if (IsStopping())
        return AI_ACTION_FINISHED;
    CTCLook* look;
    if (PBrain->Creature->GetInterface(&look, 0x43))
        look->TrackNearestInterestingThing(true);
    PBrain->Update();
    return IsStopping() ? AI_ACTION_FINISHED : AI_ACTION_CONTINUE;
}