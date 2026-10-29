// GetThingDoingDamageFromCreatureAndEventName(CThing& creature, const CCharString& eventName, bool useCarrySlot, bool second, bool third) -- the thing whose helper points a strike event uses: the creature's carry-slot item named by the event's first helper point (right/left hand), else its living primary-slot weapon when that has the helper points, else the creature itself when it does, else null
// Leaf: ?GetThingDoingDamageFromCreatureAndEventName@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00859040, 416 bytes, /Gr __fastcall (creature in ecx, eventName in edx, the three
//   bools on the stack), `ret 0Ch`.
// Identity: Ego_r.pdb ?GetThingDoingDamageFromCreatureAndEventName@?A0xad05d24c@@YIPAVCThing@@AAV2@ABVCCharString@@_N22@Z
//   (the combat TU's anonymous namespace, with GetHelperPointNamesFromStrikeEventName 0x00858E00 and
//   IsThingWithHelperPoints 0x00856570 beside it).
// Callees: CMainGameComponent::PeekDefinitionManager 0x00415FBC on g_pMainGameComponent 0x013B86A0,
//   GetHelperPointNamesFromStrikeEventName 0x00858E00, CVectorMap::LowerBound 0x004365B0 (the inline GetTC 0x46:
//   CTCCarrying), CCharString copy ctor 0x0099EC30 / (const char*, long) ctor 0x0099EBF0 / dtor 0x0099EAE0,
//   CBasicString<char>::operator==(const char*) 0x004115A0 (the inline CCharString::operator== arm), CGameDefinitionManager::
//   GetDefGlobalIndexFromName 0x009AD410, CTCCarrying::GetThingInCarrySlot 0x006A11F0 / GetThingInPrimarySlot
//   0x006A1440, IsThingWithHelperPoints 0x00856570 (x2), std::vector<CCharString>::~vector 0x00414EA0.
// Data: "weapon_pos_right_hand" 0x01276518, "weapon_pos_left_hand" 0x01276500, "CARRY_SLOT_RIGHT_HAND" 0x01265440,
//   "CARRY_SLOT_LEFT_HAND" 0x01265428, "" 0x0122D70E.
// Layout: CThing interface bitset +0x20 (interface 0x46 = bit 6 of +0x28), interface map +0x44, flags +0x91 (bit 0 dead).
// Shape: the definition manager is fetched before the helper-point vector is built; the carry slot starts at -1 and
//   each name test builds a named CCharString for GetDefGlobalIndexFromName (VC merges the two literal-only arms into
//   one tail that passes `lea` of the shared slot; a bare temporary passes the constructor's eax instead and lands in
//   the other dead argument slot). The copied helper-point name's block pointer stays in esi across the
//   CBasicString::operator== calls (the null test threaded to the second comparison) only with the CCharString copy
//   ctor, dtor and ReleaseData bodies in the TU (same-TU noinline stand-ins of the landed 0x0099EC30 / 0x0099EAE0 /
//   0x0099E9B0 bodies: without them the local's address escapes and the block is reloaded and re-tested).
#include <string.h>
#pragma intrinsic(strcmp)

#pragma pack(push, 1)
struct CBasicStringChar {
    char*         PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
    bool operator==(const char* text) const;                                    // 0x004115A0
};
class CCharStringData { public:
    CBasicStringChar Data;
    long             NoRefs;
};
#pragma pack(pop)
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);                           // 0x0099EBF0
    CCharString(const CCharString& other);                                      // 0x0099EC30
    ~CCharString();                                                             // 0x0099EAE0
    void ReleaseData();                                                         // 0x0099E9B0
    __forceinline bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};

extern long g_CCharStringLiveCount;                                             // 0x013BD800

void __cdecl operator delete[](void* block);                                     // 0x00BFEB1C
void __cdecl operator delete(void* block);                                       // 0x00BFE9BC

#pragma auto_inline(off)
void CCharString::ReleaseData()                                                 // 0x0099E9B0 (landed body, same-TU stand-in)
{
    CCharStringData* p = PStringData;
    if (p != 0) {
        if (--p->NoRefs <= 0) {
            CCharStringData* data = PStringData;
            if (data != 0) {
                if (data->Data.PData != 0) {
                    delete[] data->Data.PData;
                    data->Data.PData = 0;
                }
                data->Data.StringLength = 0;
                data->Data.DataLength = 0;
                delete data;
            }
        }
        PStringData = 0;
    }
}

CCharString::~CCharString()                                                     // 0x0099EAE0 (landed body, same-TU stand-in)
{
    ReleaseData();
    --g_CCharStringLiveCount;
}

CCharString::CCharString(const CCharString& other)                              // 0x0099EC30 (landed body, same-TU stand-in)
{
    PStringData = 0;
    ++g_CCharStringLiveCount;
    if (&other != this) {
        if (PStringData != 0)
            ReleaseData();
        CCharStringData* data = other.PStringData;
        if (data != 0) {
            PStringData = data;
            ++data->NoRefs;
        }
    }
}
#pragma auto_inline(on)

namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {                  // STLport, 12 bytes
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector();                                                                  // 0x00414EA0
    _Tp& operator[](unsigned int __n) { return *(_M_start + __n); }
};
}

class CGameDefinitionManager {
public:
    long GetDefGlobalIndexFromName(const CCharString& name) const;              // 0x009AD410
};
class CMainGameComponent {
public:
    const CGameDefinitionManager& PeekDefinitionManager() const;                // 0x00415FBC
};
extern CMainGameComponent* g_pMainGameComponent;                                // 0x013B86A0

class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                                                          // CVectorMap<long, CTCBase*>
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* end_of_storage;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;                           // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing;
class CTCCarrying {
public:
    CThing* GetThingInPrimarySlot() const;                                      // 0x006A1440
    CThing* GetThingInCarrySlot(long slot) const;                               // 0x006A11F0
};
class CThing {
public:
    unsigned char   _pad0[0x20];
    unsigned long   m_presentBits[9];                                           // +0x20
    CInterfaceMap   m_interfaces;                                               // +0x44
    unsigned char   _pad50[0x91 - 0x50];
    unsigned char   m_flags91;                                                  // +0x91 bit 0: dead
    __forceinline bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline T* GetTC(long interfaceType) const
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(type);
        return (T*)((void*)((it == map->last || interfaceType < it->first) ? map->last : it)->second);
    }
};

namespace {
void __fastcall GetHelperPointNamesFromStrikeEventName(const CCharString& eventName,
                                                       std::vector<CCharString>& helperPointNames, bool useCarrySlot,
                                                       bool second, bool third);                // 0x00858E00
bool __fastcall IsThingWithHelperPoints(const CThing& thing, std::vector<CCharString>& helperPointNames);   // 0x00856570

CThing* __fastcall GetThingDoingDamageFromCreatureAndEventName(CThing& creature, const CCharString& eventName,
                                                              bool useCarrySlot, bool second, bool third)
{
    const CGameDefinitionManager& definitions = g_pMainGameComponent->PeekDefinitionManager();
    std::vector<CCharString> helperPointNames;
    GetHelperPointNamesFromStrikeEventName(eventName, helperPointNames, useCarrySlot, second, third);
    if (creature.HasTC(0x46)) {
        CTCCarrying* carrying = creature.GetTC<CTCCarrying>(0x46);
        if (useCarrySlot) {
            CCharString helperPointName = helperPointNames[0];
            long slot = -1;
            if (helperPointName == "weapon_pos_right_hand") {
                CCharString slotName("CARRY_SLOT_RIGHT_HAND");
                slot = definitions.GetDefGlobalIndexFromName(slotName);
            } else if (helperPointName == "weapon_pos_left_hand") {
                CCharString slotName("CARRY_SLOT_LEFT_HAND");
                slot = definitions.GetDefGlobalIndexFromName(slotName);
            }
            return carrying->GetThingInCarrySlot(slot);
        }
        CThing* weapon = carrying->GetThingInPrimarySlot();
        if (weapon && !(weapon->m_flags91 & 1) && IsThingWithHelperPoints(*weapon, helperPointNames))
            return weapon;
    }
    if (IsThingWithHelperPoints(creature, helperPointNames))
        return &creature;
    return 0;
}
}

CThing* (__fastcall *g_keepGetThingDoingDamageFromCreatureAndEventName)(CThing&, const CCharString&, bool, bool, bool) =
    &GetThingDoingDamageFromCreatureAndEventName;   // keeps the body emitted