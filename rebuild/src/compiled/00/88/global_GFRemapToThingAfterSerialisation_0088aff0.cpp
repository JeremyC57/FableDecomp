// GFRemapToThingAfterSerialisation(CIntelligentPointer<CThing>& pointer) -- after loading: when the pointed thing is a live placeholder carrying the interface-0xEB component, kill it and repoint at the thing with the UID that component recorded
// Leaf: ?GFRemapToThingAfterSerialisation@@YIXAAV?$CIntelligentPointer@VCThing@@@@@Z
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0088AFF0, length 193 bytes (+ int3 padding), `__fastcall` (pointer in
//   ecx), plain `ret`.
// Identity: Ego_r.pdb `?GFRemapToThingAfterSerialisation@@YIXAAV?$CIntelligentPointer@VCThing@@@@@Z`; callers 0x006C1712,
//   0x006C5688, 0x006C56A6, 0x006C56F6 (the landed CTCHeroOpinionDeedLog::OnInitialActivate 0x006C5680 among them).
// Callees: CBaseIntelligentPointer::GetPItem() const @ 0x00A01B50 (three times), CVectorMap<ETCInterfaceType, CTCBase*>
//   ::LowerBound @ 0x0040F020 (same-TU stand-in below), CThing::Kill(bool) @ 0x004C9B80,
//   CThingManager::GetPThingWithUID(__int64) @ 0x0051ED80 on the global at 0x013B8A1C, the CIntelligentPointer<CThing>
//   temporary: CBaseIntelligentPointer() @ 0x00A01B10, SetPItem @ 0x00A01B90, ~CBaseIntelligentPointer @ 0x00A01C10;
//   vtable 0x01238C6C.
// Layout: CThing interface bitset +0x20 (type 0xEB = bit 11 of the word at +0x3C), interface map +0x44, dead bit +0x91;
//   the interface-0xEB component keeps the remap UID (64-bit) at +0x10 (its class is not named in the PDB sets we have).
// Shape: one GetPItem for the alive test, a second whose thing serves the inlined out-parameter GetInterface (bit test,
//   LowerBound with the key in the frame word, `it == last || type < it->type` fallback), the UID read into edi/ebx
//   before the third GetPItem's Kill(false), then `pointer = CIntelligentPointer<CThing>(found)`: the temporary reuses
//   the key's frame word, which VC7.1 allows only with LowerBound's body visible (GOTCHAS: same-TU auto_inline(off)
//   stand-in).
class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

struct SThingUIDRemapComponent {                        // the interface-0xEB component (descriptive name)
    unsigned char    _pad00[0x10];
    unsigned __int64 RemapUID;                          // +0x10
};

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad54[0x3d];
    unsigned char m_flags91;                            // +0x91 bit 0: dead
    void Kill(bool immediately);                        // 0x004C9B80
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};

class CThingManager {
public:
    CThing* GetPThingWithUID(unsigned __int64 uid);               // 0x0051ED80
};
extern CThingManager* g_pThingManager;                  // 0x013B8A1C

class CBaseObject;
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
    void* PData;                                        // +0x04
};

template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer {   // CIntelligentPointer<CThing>: vtable 0x01238C6C
public:
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        SetPItem(other.GetPItem());
        return *this;
    }
    T* operator->() const { return (T*)GetPItem(); }
};

static const long TC_THING_UID_REMAP = 0xEB;            // interface type of the remap component

inline bool IsThingAlive(const CThing* thing)
{
    return thing && !(thing->m_flags91 & 1);
}

void __fastcall GFRemapToThingAfterSerialisation(CIntelligentPointer<CThing>& pointer)
{
    SThingUIDRemapComponent* remap;
    if (IsThingAlive((CThing*)pointer.GetPItem()) && pointer->GetInterface(&remap, TC_THING_UID_REMAP)) {
        unsigned __int64 uid = remap->RemapUID;
        pointer->Kill(false);
        pointer = CIntelligentPointer<CThing>(g_pThingManager->GetPThingWithUID(uid));
    }
}