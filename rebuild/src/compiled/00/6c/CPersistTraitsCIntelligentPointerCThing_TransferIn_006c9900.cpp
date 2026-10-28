// CPersistTraits<CIntelligentPointer<CThing> >::TransferIn(CPersistContext& context, CStringParser& parser, CIntelligentPointer<CThing>& value) -- read a thing UID from a text save; for a non-zero UID create a THING_DUMMY_FOR_SERIALISATION placeholder carrying the UID and point the value at it
// Leaf: ?TransferIn@?$CPersistTraits@V?$CIntelligentPointer@VCThing@@@@@@QAEXAAVCPersistContext@@AAVCStringParser@@AAV?$CIntelligentPointer@VCThing@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006C9900, 241 bytes, thiscall `ret 0Ch` (the traits `this` is unused).
// Identity (proven): Ego_r.pdb donor 0x006FF800 holds this symbol right after TransferBinaryIn (0x006FF6C0), whose retail twin is
//   the landed 0x006C8780 (same dummy-thing body over a binary stream); the two retail callers 0x006C9773 / 0x006C9B59 are the
//   CPersistContext_TransferableVector / component hooks of the same TU.
// Callees: CStringParser::ReadAsUHUGE() @ 0x009BA5B0 (unsigned __int64 in edx:eax), CCharString(const char*, long) @ 0x0099EBF0,
//   CDefinitionManager::GetDefGlobalIndexFromName(const CCharString&) const @ 0x009AD410, CTCDNone::Create(long, const C3DVector&,
//   bool) @ 0x00835A20 (static __fastcall), ~CCharString @ 0x0099EAE0, CVectorMap<ETCInterfaceType,CTCBase*,..>::LowerBound
//   (non-const) @ 0x0040F020, CBaseIntelligentPointer ctor / SetPItem / GetPItem / dtor @ 0x00A01B10 / 0x00A01B90 / 0x00A01B50 /
//   0x00A01C10 (CIntelligentPointer<CThing> vtable 0x01238C6C). Data: the thing manager @ 0x013B8A1C, "THING_DUMMY_FOR_SERIALISATION"
//   @ 0x0125E9EC.
// Layout: CThingManager World +0x24 (Ego_r +0x30, the PC manager is 0xC smaller), CWorld DefinitionManager +0x10 (Ego_r);
//   retail CThing UsingComponents +0x20 / Components +0x44; CTCThingUIDDummyForSerialisation ThingUID +0x10 (Ego_r), interface
//   0xEB (its GetInterfaceType, vtable 0x0126A724 slot 21).
// Shape: the definition manager is reached through the inline PDB accessors CThingManager::PeekWorld() and
//   CWorld::PeekDefinitionManager() (both const-reference returns; the raw pointer chain loads the global into ecx instead
//   of eax). As the landed TransferBinaryIn twin: the name string and the zero position are locals of a block that ends after
//   Create; the component pointer defaults to 0 (no null test before the UID store); `value = CIntelligentPointer<CThing>(thing)`
//   through CIntelligentPointer's inline assignment (SetPItem(rhs.GetPItem())).
class CBaseObject;
class CBaseObjectPointer;
class CTCBase;
enum ETCInterfaceType { ETCInterfaceType_Force32 = 0x7fffffff };

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CStringParser { public:
    unsigned __int64 ReadAsUHUGE();                     // 0x009BA5B0
};
class CPersistContext;
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};

class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
    CBaseObjectPointer* PData;                          // +4
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C
public:
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc> class vector {           // retail STLport vector (12 bytes)
public:
    typedef _Tp* iterator;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    iterator end() { return this->_M_finish; }
};
template <unsigned int _Nw> struct _Base_bitset {
    typedef unsigned long _WordT;
    _WordT _M_w[_Nw];
    static unsigned int _S_whichword(unsigned int __pos) { return __pos / 32; }
    static _WordT _S_maskbit(unsigned int __pos) { return 1UL << (__pos % 32); }
    _WordT _M_getword(unsigned int __pos) const { return _M_w[_S_whichword(__pos)]; }
};
template <unsigned int _Nb> class bitset : public _Base_bitset<(_Nb + 31) / 32> {
public:
    typedef _Base_bitset<(_Nb + 31) / 32> _Base;
    bool _Unchecked_test(unsigned int __pos) const { return (this->_M_getword(__pos) & _Base::_S_maskbit(__pos)) != 0; }
    bool operator[](unsigned int __pos) const { return _Unchecked_test(__pos); }
};
}   // namespace std

template <class T> class CArray : public std::vector<T, std::allocator<T> > { };
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class TCompare> class CVectorMap {   // retail 0x10 bytes
public:
    typedef std::pair<K, V> value_type;
    CArray<value_type> Data;                            // +0x00
    TCompare           Compare;                         // +0x0C
    bool               Dirty;                           // +0x0D
    value_type* LowerBound(const K& key);               // 0x0040F020
};

class CThing {                                          // partial view, retail offsets
public:
    typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > TCMap;
    unsigned char      Header[0x20];
    std::bitset<274>   UsingComponents;                 // +0x20
    TCMap              Components;                      // +0x44
    bool HasTC(ETCInterfaceType type) const { return UsingComponents[type]; }
    template <class T> T* GetTC(ETCInterfaceType type)
    {
        ETCInterfaceType key = type;
        TCMap* map = &Components;
        TCMap::value_type* it = map->LowerBound(key);
        return (T*)((it == map->Data.end() || type < it->first) ? map->Data.end() : it)->second;
    }
};
class CTCThingUIDDummyForSerialisation {                // Ego_r.pdb, 0x18 bytes (CTCBase 0xC)
public:
    unsigned char    _base[0x10];                       // CTCBase (vptr, ParentThing, flags)
    unsigned __int64 ThingUID;                          // +0x10
};
class CTCDNone { public:
    static CThing* __fastcall Create(long def_index, const C3DVector& position, bool ignore_map = true);   // 0x00835A20
};
class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name) const;   // 0x009AD410
};
class CGameDefinitionManager : public CDefinitionManager { };
class CWorld { public:
    unsigned char           _pad00[0x10];
    CGameDefinitionManager* DefinitionManager;          // +0x10
    const CGameDefinitionManager& PeekDefinitionManager() const { return *DefinitionManager; }
};
class CThingManager { public:
    unsigned char _pad00[0x24];
    CWorld*       World;                                // +0x24
    const CWorld& PeekWorld() const { return *World; }
};
extern CThingManager* g_pThingManager;                  // 0x013B8A1C

template <class T> class CPersistTraits {
public:
    T DefaultVal;
    void TransferIn(CPersistContext& context, CStringParser& parser, T& value);
};

template <>
void CPersistTraits<CIntelligentPointer<CThing> >::TransferIn(CPersistContext& context, CStringParser& parser, CIntelligentPointer<CThing>& value)
{
    unsigned __int64 uid = parser.ReadAsUHUGE();
    if (uid > 0) {
        CThing* thing;
        {
            CCharString name("THING_DUMMY_FOR_SERIALISATION");
            C3DVector position(0.0f, 0.0f, 0.0f);
            thing = CTCDNone::Create(g_pThingManager->PeekWorld().PeekDefinitionManager().GetDefGlobalIndexFromName(name), position, true);
        }
        CTCThingUIDDummyForSerialisation* dummy = 0;
        if (thing->HasTC((ETCInterfaceType)0xEB))
            dummy = thing->GetTC<CTCThingUIDDummyForSerialisation>((ETCInterfaceType)0xEB);
        dummy->ThingUID = uid;
        value = CIntelligentPointer<CThing>(thing);
    }
}