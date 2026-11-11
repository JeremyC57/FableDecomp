// CPersistTraits<CIntelligentPointer<CThing> >::TransferIn(CPersistContext& context, CStringParser& parser, CIntelligentPointer<CThing>& value) -- read a thing UID from a text save; for a non-zero UID create a THING_DUMMY_FOR_SERIALISATION placeholder carrying the UID and point the value at it
// Leaf: ?TransferIn@?$CPersistTraits@V?$CIntelligentPointer@VCThing@@@@@@QAEXAAVCPersistContext@@AAVCStringParser@@AAV?$CIntelligentPointer@VCThing@@@@@Z
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the local map's CArray member Data is the header's CArray base (Ego_r: CVectorMap derives CArray<pair<K,V> >): Data.end() / begin() / size() read through the base; the local std::bitset operator[] / reference is the STLport model of stlport/fable_containers.h under FABLE_RETAIL_STLP_BITSET_INDEX (retail tests the word inline, e.g. 0x0062D032 test dword ptr [ebx+0x24],0x10000); the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
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
#include "engine/C3DVector.h"

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

#define FABLE_RETAIL_STLP_BITSET_INDEX
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/CVectorMap.h"


class CThingManager;
class CThing {
private:
    static CThingManager* PCurrentThingManager;      // 0x013B8A1C ?PCurrentThingManager@CThing@@0PAVCThingManager@@A (private static)
public:
    static CThingManager* __fastcall GetPCurrentThingManager() { return PCurrentThingManager; }   // Ego_r / FableWin ?GetPCurrentThingManager@CThing@@SIPAVCThingManager@@XZ (inline)
private:
                                          // partial view, retail offsets
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
        return (T*)((it == map->end() || type < it->first) ? map->end() : it)->second;
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
            thing = CTCDNone::Create(CThing::GetPCurrentThingManager()->PeekWorld().PeekDefinitionManager().GetDefGlobalIndexFromName(name), position, true);
        }
        CTCThingUIDDummyForSerialisation* dummy = 0;
        if (thing->HasTC((ETCInterfaceType)0xEB))
            dummy = thing->GetTC<CTCThingUIDDummyForSerialisation>((ETCInterfaceType)0xEB);
        dummy->ThingUID = uid;
        value = CIntelligentPointer<CThing>(thing);
    }
}