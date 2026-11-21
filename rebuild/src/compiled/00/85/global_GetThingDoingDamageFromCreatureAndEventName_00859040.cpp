// GetThingDoingDamageFromCreatureAndEventName(CThing& creature, const CCharString& eventName, bool useCarrySlot, bool second, bool third) -- the thing whose helper points a strike event uses: the creature's carry-slot item named by the event's first helper point (right/left hand), else its living primary-slot weapon when that has the helper points, else the creature itself when it does, else null
// Leaf: ?GetThingDoingDamageFromCreatureAndEventName@
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +4.
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00859040, 416 bytes, /Gr __fastcall (creature in ecx, eventName in edx, the three
//   bools on the stack), `ret 0Ch`.
// Identity: Ego_r.pdb ?GetThingDoingDamageFromCreatureAndEventName@?A0xad05d24c@@YIPAVCThing@@AAV2@ABVCCharString@@_N22@Z
//   (the combat TU's anonymous namespace, with GetHelperPointNamesFromStrikeEventName 0x00858E00 and
//   IsThingWithHelperPoints 0x00856570 beside it).
// Callees: CMainGameComponent::PeekDefinitionManager 0x00415FBC on CMainGameComponent::Get() 0x013B86A0,
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
// Name fix (NF215, 2026-11-21, q12262): ported onto fable_string.h (FABLE_RETAIL_CCHARSTRING + CCHARSTRINGDATA_FRIEND +
//   SHOVE_STRING_CLEANUP_PASS23 + CCHARSTRING_FORCEINLINE_TEXT_EQ): the local CBasicStringChar / CCharStringData / CCharString models are
//   gone, so the inline CCharString::operator== arm calls the Ego_r.pdb 0x0040C910 / FableWin.pdb 0x01899D90 ??8?$CBasicString@D@@QBE_NPBD@Z
//   (was ??8CBasicStringChar@@QBE_NPBD@Z; declaration only, no copy defined, as at HEAD). The three same-TU bodies are now definitions of
//   the header's members: ReleaseData -> the PDB protected ?UnassignString@CCharString@@IAEXXZ (0x0099E9B0), and the counter
//   g_CCharStringLiveCount -> the header's g_CCharStringInstanceCount_013BD800 (0x013BD800, the 0x0099EAE0 owner's spelling). Each copy
//   equals its owner (bytes and relocation targets: 87 / 12 / 61). UnassignString stays out of line in its two TU-mates as in retail
//   through a guarded __declspec(noinline) on its definition (HEAD: the auto_inline(off) region; inside that region VC7.1 calls the
//   header's ??_GCCharStringData instead of expanding delete data: 43 v 87).
#include "fable_compiler.h"
#include <string.h>
#pragma intrinsic(strcmp)

#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRINGDATA_FRIEND
#define FABLE_RETAIL_SHOVE_STRING_CLEANUP_PASS23
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#include "fable_string.h"

void __cdecl operator delete(void* block);                                       // 0x00BFE9BC

// Fidelity: same-TU strong definition of 0x0099E9B0 (retail TU-mate)
#ifdef _MSC_VER
__declspec(noinline)
#endif
void CCharString::UnassignString()                                              // 0x0099E9B0
{
    CCharStringData* p = PStringData;
    if (p != 0) {
        if (--p->NoRefs <= 0) {
            CCharStringData* data = PStringData;
            if (data != 0)
                delete data;
        }
        PStringData = 0;
    }
}

#pragma auto_inline(off)
// Fidelity: same-TU strong definition of 0x0099EAE0 (retail TU-mate)
CCharString::~CCharString()                                                     // 0x0099EAE0
{
    UnassignString();
    --g_CCharStringInstanceCount_013BD800;
}

// Fidelity: same-TU strong definition of 0x0099EC30 (retail TU-mate)
CCharString::CCharString(const CCharString& other)                              // 0x0099EC30
{
    PStringData = 0;
    ++g_CCharStringInstanceCount_013BD800;
    if (&other != this) {
        if (PStringData != 0)
            UnassignString();
        CCharStringData* data = other.PStringData;
        if (data != 0) {
            PStringData = data;
            ++data->NoRefs;
        }
    }
}
#pragma auto_inline(on)

#include "engine/CVectorMap.h"

// NF132 (q6233): CGameDefinitionManager is the shared engine/CGameDefinitionManager.h class (: CDefinitionManager); 0x009AD410 binds
//   ?GetDefGlobalIndexFromName@CDefinitionManager@@QBEJABVCCharString@@@Z (Ego_r.pdb 0x00B05360 + FableWin.pdb 0x03062820), was this
//   row's local CGameDefinitionManager model's own member.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#include "engine/CGameDefinitionManager.h"
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private:
public:
    const CGameDefinitionManager& PeekDefinitionManager() const;                // 0x00415FBC
};

class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
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
    unsigned char   _pad54[0x3d];
    unsigned char   m_flags91;                                                  // +0x91 bit 0: dead
    FABLE_FORCEINLINE bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template <class T> FABLE_FORCEINLINE T* GetTC(long interfaceType) const
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::value_type* it = map->LowerBound(type);
        return (T*)((void*)((it == map->end() || interfaceType < it->first) ? map->end() : it)->second);
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
    const CGameDefinitionManager& definitions = CMainGameComponent::Get()->PeekDefinitionManager();
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