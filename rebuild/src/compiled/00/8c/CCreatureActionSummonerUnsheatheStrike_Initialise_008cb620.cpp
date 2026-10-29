// CCreatureAction_SummonerUnsheatheStrike::Initialise() -- the unsheathe-weapons Initialise, then look up the SUMMONER_ATTACK_STYLE def and hand it to the owner's combat component (interface 0x30)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CB620, length 180 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CCreatureAction_UnsheatheWeapons::Initialise @ 0x008B7ED0 (the direct base), CBaseIntelligentPointer::GetPItem
// @ 0x00A01B50, CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0x30, bit 16 of +0x24), CCharString::CCharString
// (const char*, long) @ 0x0099EBF0, CCreatureActionBase::GetPDefinitionManager @ 0x00692910 (manifest label is STL noise),
// CDefinitionManager::GetDef<T>(const CCharString&, CDefPointer<T>*) @ 0x00669770, CCharString::~CCharString @ 0x0099EAE0,
// the combat component's attack-style setter @ 0x007355F0 (takes the def pointer by address; manifest label is noise).
// Shape: the UNCHECKED out-parameter lookup in an inline accessor (the absent path reads the lookup key's frame word, which
// the block-scoped name string reuses); the CDefPointer local is zeroed inside the string construction and released at
// the end through the def's inline ReleaseTemporary on its ref-count base at +4 (null test kept, null store elided).
// Layout: owner CIntelligentPointer +8.
class CTCBase;
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
    unsigned long m_presentBits[9];             // +0x20
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

class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

struct CRefCounted {                            // second base of the def: the count at +4
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
struct CAttackStyleDef : CDefVirtual, CRefCounted {
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
template <class T> class CDefPointer { public:
    T* m_p;
    CDefPointer() : m_p(0) { }
    ~CDefPointer()
    {
        if (m_p) {
            m_p->ReleaseTemporary();
            m_p = 0;
        }
    }
};

class CDefinitionManager { public:
    bool GetDef(const CCharString& name, CDefPointer<CAttackStyleDef>* def);   // 0x00669770
};

class CTCCombat { public:
    void SetAttackStyleDef(const CDefPointer<CAttackStyleDef>* def);         // 0x007355F0
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

class CCreatureAction_UnsheatheWeapons {
public:
    virtual void Initialise();                  // 0x008B7ED0
    unsigned char           _pad04[4];
    CIntelligentPointer<CThing> PCreature;          // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
    CDefinitionManager* GetPDefinitionManager() const;   // 0x00692910
    __forceinline CTCCombat* GetCombat() const
    {
        CTCCombat* combat;
        PCreature.GetPItem()->GetInterface(&combat, 0x30);
        return combat;
    }
};

class CCreatureAction_SummonerUnsheatheStrike : public CCreatureAction_UnsheatheWeapons {
public:
    virtual void Initialise();
};

void CCreatureAction_SummonerUnsheatheStrike::Initialise()
{
    CCreatureAction_UnsheatheWeapons::Initialise();
    CTCCombat* combat = GetCombat();
    CDefPointer<CAttackStyleDef> def;
    {
        CCharString name("SUMMONER_ATTACK_STYLE", -1);
        GetPDefinitionManager()->GetDef(name, &def);
    }
    combat->SetAttackStyleDef(&def);
}