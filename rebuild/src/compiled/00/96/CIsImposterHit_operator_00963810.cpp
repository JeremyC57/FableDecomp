// CIsImposterHit::operator()() const -- true when the watched thing is alive, has creature mode 0x25 active in its mode manager (component kind 0x42) (the imposter has been hit)
// Leaf: ??RCIsImposterHit
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00963810, length 108 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three calls on the pointer at +0x14), CVectorMap<ETCInterfaceType,
// CTCBase*>::LowerBound @ 0x0040F020 (the inlined component lookup), CTCCreatureModeManager::IsModeActive @ 0x00811340 (landed;
// manifest label "CPlayer::HasMode" is propagated).
// Model: the out-parameter GetInterface template of the landed CCreatureAction_JackFall::FinishAction 0x008C2490 (the
// component stays in eax: `mov eax,[eax+4] ; push ; mov ecx,eax`); kind 0x42 is bit 2 of the presence word at +0x28.
// Shape: `P.GetPItem() && !P.GetPItem()->Dead` re-calls the getter; the third call's thing serves both the presence test
// and the lookup; the two-exit `mov al,1` / `xor al,al` tail.
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                             // a thing's sorted {type, component} vector map at +0x44
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* end;
    SEntry* LowerBound(const ETCInterfaceType& type);          // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                    // +0x44
    unsigned char _pad50[0x91 - 0x50];
    unsigned char Dead : 1;                        // +0x91 bit 0
    unsigned char _flags91 : 7;
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
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CTCCreatureModeManager { public:                    // component kind 0x42 here
    bool IsModeActive(long mode) const;            // 0x00811340 (landed)
};
struct CIsImposterHit {
    unsigned char           _pad00[0x14];
    CIntelligentPointer<CThing> PImposter;             // +0x14 CIntelligentPointer<CThing>
    bool operator()() const;
};

bool CIsImposterHit::operator()() const
{
    if (PImposter.GetPItem() && !PImposter.GetPItem()->Dead) {
        CTCCreatureModeManager* modes;
        if (PImposter.GetPItem()->GetInterface(&modes, 0x42) && modes->IsModeActive(0x25))
            return true;
    }
    return false;
}