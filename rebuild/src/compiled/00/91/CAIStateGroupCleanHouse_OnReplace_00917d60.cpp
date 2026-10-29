// CAIStateGroup_CleanHouse::OnReplace() -- CAIStateGroup_Clean::OnReplace, then ask the creature's idle scheduler for a new idle
// Leaf: ?OnReplace@CAIStateGroup_CleanHouse@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00917D60, 75 bytes, thiscall, plain `ret`.
// Identity (proven): CAIStateGroup_CleanHouse vtable 0x01289A9C slot 10 (OnReplace); Ego_r.pdb ?OnReplace@CAIStateGroup_CleanHouse@@UAEXXZ (donor 0x0093B670);
//   RTTI bases CAIStateGroup_CleanHouse : CAIStateGroup_Clean : CAIStateGroupBase.
// Callees: CAIStateGroup_Clean::OnReplace() 0x00917BB0, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020, CTCIdleScheduler::ChooseNewIdle()
//   0x00801A50.
// Shape (family K045, representative 0x00917D60): the qualified base call, then the landed CAIStateGroup_MinionWatchFires::OnReplace
//   0x00936590 body (its model, verbatim: GetInterface on PBrain->PCreature, interface 0xD3 = bit 19 of the dword at +0x38).
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);      // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
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
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    CAIBrain* PBrain;                           // +0x04
};
class CTCIdleScheduler { public:                // interface type 0xD3 component
    void ChooseNewIdle();                       // 0x00801A50
};
class CAIStateGroup_Clean : public CAIStateGroupBase {
public:
    virtual void OnReplace();                   // 0x00917BB0
};
class CAIStateGroup_CleanHouse : public CAIStateGroup_Clean {
public:
    virtual void OnReplace();
};
void CAIStateGroup_CleanHouse::OnReplace()
{
    CAIStateGroup_Clean::OnReplace();
    CTCIdleScheduler* pScheduler;
    if (PBrain->PCreature->GetInterface(&pScheduler, 0xD3))
        pScheduler->ChooseNewIdle();
}