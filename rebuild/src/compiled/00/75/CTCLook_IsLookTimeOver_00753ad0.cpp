// CTCLook::IsLookTimeOver() -- false while the owner's interface-0xAA component exists and has its "keep looking" byte (+0x7A) clear; otherwise whether the frame counter has reached LookTimer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00753AD0, length 101 bytes.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x004365B0 (inlined GetInterface, type 0xAA = bit 10 of the dword at
// CThing+0x34: `test ch,4`), CTCBase::PeekComponent @ 0x00686D60 (`[0x013B8A1C]+0x1C`) and CMainGameComponent::GetCurrentWorldFrame @ 0x004167C8
// (a `jmp` to 0x0049D870; the landed UpdateLookTimer 0x00751AD0 uses the same pair).
// Layout (retail, local model): ParentThing +0x04, LookTimer +0x3C (PDB offset); the component's flag byte +0x7A (component type
// not identified, named descriptively).
// Shape: out-pointer GetInterface `&&` the flag test returning false; the tail `return frame >= LookTimer;` (`setge cl ; mov al,cl`).
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;       // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
struct CTCLookTimeOwner {                       // the interface-0xAA component
    unsigned char _pad00[0x7a];
    bool KeepLooking;                           // +0x7a
};
class CMainGameComponent { public:
    long GetCurrentWorldFrame() const;             // 0x004167C8 (Ego_r.pdb ?GetCurrentWorldFrame@CMainGameComponent@@QBEJXZ)
};
class CTCBase {
public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04
protected:
    const CMainGameComponent& PeekComponent() const;   // 0x00686D60 (Ego_r.pdb ?PeekComponent@CTCBase@@IBEABVCMainGameComponent@@XZ)
};
class CTCLook : public CTCBase {
public:
    unsigned char _pad08[0x3c - 0x08];
    long LookTimer;                             // +0x3c
    bool IsLookTimeOver();
};

bool CTCLook::IsLookTimeOver()
{
    CTCLookTimeOwner* owner;
    if (ParentThing->GetInterface(&owner, 0xaa) && !owner->KeepLooking)
        return false;
    return PeekComponent().GetCurrentWorldFrame() >= LookTimer;
}