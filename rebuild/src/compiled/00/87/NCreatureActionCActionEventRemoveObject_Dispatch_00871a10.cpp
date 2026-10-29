// NCreatureAction::CActionEventRemoveObject::Dispatch(CCreatureActionBase&) -- when the creature carries things: take the object out of the named hand slot and kill it
// Leaf: ?Dispatch@CActionEventRemoveObject
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00871A10, length 112 bytes, `ret 4` (the action argument is unused),
//   vtable 0x0125C1A0 slot 1.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, type 70 = bit 6 of the bitset dword at +0x28 =
//   CTCCarrying), CGameDefinitionManager::Get @ 0x0044C6B0, CDefinitionManager::GetDefGlobalIndexFromName @ 0x009AD410,
//   CTCCarrying::RemoveThingInCarrySlot @ 0x006A16A0 (slot, bool), CThing::Kill(bool) @ 0x004C9B80.
// Name: FableWin.pdb `?Dispatch@CActionEventRemoveObject@NCreatureAction@@UAEXAAVCCreatureActionBase@@@Z`.
// Shape: the model is the landed sibling CActionEventAddObject::Dispatch @ 0x008718B0 (same out-parameter GetInterface
//   with the ternary entry pick; the lookup key lives in the `push ecx` slot); the Hand name is resolved to a
//   carry-slot index, a positive slot removes the carried thing (without dropping it) and a live thing (`!(flags91 & 1)`)
//   is killed immediately.
// Layout: Ego_r.pdb CActionEventBase {vptr, Finished +4, ActivationFrame +8, PThing +0xc, PCameraManager +0x10}, Hand +0x14.
class CTCBase;
class CCreatureActionBase;
class CGameCameraManager;

class CCharString { public:
    void* PStringData;
};

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    void Kill(bool immediately);                // 0x004C9B80
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CTCCarrying { public:                            // interface 70
    CThing* RemoveThingInCarrySlot(long slot, bool drop);   // 0x006A16A0
};

class CDefinitionManager { public:
public:
    long GetDefGlobalIndexFromName(const CCharString&) const;   // 0x009AD410
};
class CGameDefinitionManager : public CDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();          // 0x0044C6B0
};

namespace NCreatureAction {
struct CActionEventRemoveObject {
    virtual ~CActionEventRemoveObject();
    bool Finished;                              // +0x04
    long ActivationFrame;                       // +0x08
    CThing* PThing;                             // +0x0c
    CThing* GetThing() const { return PThing; }
    CGameCameraManager* PCameraManager;         // +0x10
    CCharString Hand;                           // +0x14
    virtual void Dispatch(CCreatureActionBase& action);
};
}

void NCreatureAction::CActionEventRemoveObject::Dispatch(CCreatureActionBase& action)
{
    CTCCarrying* carrying;
    if (GetThing()->GetInterface(&carrying, 70)) {
        long slot = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(Hand);
        if (slot > 0) {
            CThing* thing = carrying->RemoveThingInCarrySlot(slot, false);
            if (thing != 0 && !(thing->m_flags91 & 1))
                thing->Kill(true);
        }
    }
}