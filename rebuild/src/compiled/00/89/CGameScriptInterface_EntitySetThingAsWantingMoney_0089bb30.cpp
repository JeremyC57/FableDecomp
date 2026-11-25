// CGameScriptInterface::EntitySetThingAsWantingMoney(const CScriptThing& thing, long amount) const -- set the amount on the valid thing's money-receiver component (kind 0x80), adding a "CTCMoneyReceiver" component first when it has none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0089BB30, length 142 bytes.
// Callees: the script thing's slots 75 (+0x12c) and 11 (+0x2c), the component-map LowerBound @ 0x0040F020,
//          CCharString::CCharString(const char*, long) @ 0x0099EBF0, CThing::AddTC(const CCharString&, bool, CTCParamsBase*) @ 0x004C9D60,
//          CCharString::~CCharString @ 0x0099EAE0; the string literal "CTCMoneyReceiver" @ 0x0123C1A8.
// Shape: the store of the amount is tail-duplicated into both arms; the name temporary lives in the dead argument slot.
// The key and the name share that slot only when the key's address no longer escapes: a same-TU noinline LowerBound
// stand-in (plain binary search; the retail body is 0x0040F020) plus the name as a named local in the else block (the
// 008967f0 idiom; the stand-in with the temporary argument stays 19 lines).
// Leaf: ?EntitySetThingAsWantingMoney@CGameScriptInterface@@UBEXABVCScriptThing@@J@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
#include "fable_compiler.h"
struct CTCBase_Retail;
struct STCEntry_Retail {                           // one CVectorMap<ETCInterfaceType, CTCBase*> element
    long            m_kind;
    CTCBase_Retail* m_pTC;
};
class CTCBase;
// Naming (NF437, 2026-11-25): the LowerBound split -- the row-local map stand-in CTCMap_Retail (?LowerBound@CTCMap_Retail@@QAEPAUSTCEntry_Retail@@ABK@Z) replaced by the shared
//   CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<..> > typedef of engine/CVectorMap.h that the landed AI rows use (0x00929F10): CThing::Components at +0x44 (0x10 bytes),
//   so the call binds the established ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@...@@QAE spelling at 0x0040F020.
#include "engine/ETCInterfaceType.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CTCMap_Retail;
// Same-TU stand-in for CVectorMap::LowerBound 0x0040F020 (not the retail body).
struct CThing_Retail {
    unsigned char  _pad00[0x10];
    unsigned char  m_type;                         // +0x10 (1 = a creature)
    unsigned char  _pad11[0xf];
    unsigned long  m_tcMask[8];                    // +0x20: which component kinds the thing holds
    unsigned char  _pad40[4];
    CTCMap_Retail  m_tcs;                          // +0x44
    unsigned char  _pad54[0x3D];                   // +0x54 (CVectorMap is 0x10)
    unsigned char  m_dead : 1;                     // +0x91 bit 0
    unsigned char  m_91_1 : 7;
    FABLE_FORCEINLINE unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }   // the masked word
};
// The row-local inline lookups of the stand-in map, now free helpers over the real CVectorMap (same shapes: the key in a stack
// temporary, the eax-join Find and the ternary entry pick).
FABLE_FORCEINLINE CTCBase_Retail* FindTC(CTCMap_Retail& map, long kind)
{
    CTCMap_Retail::value_type* it = map.LowerBound((ETCInterfaceType)kind);
    if (it == map.end() || kind < it->first)
        it = map.end();
    return (CTCBase_Retail*)it->second;
}
FABLE_FORCEINLINE CTCBase_Retail* FindTC(const CTCMap_Retail& map, long kind)
{
    const CTCMap_Retail::value_type* it = map.LowerBound((ETCInterfaceType)kind);
    if (it == map.end() || kind < it->first)
        it = map.end();
    return (CTCBase_Retail*)it->second;
}
class CThingPlayerCreature : public CThing_Retail { };
FABLE_FORCEINLINE CTCBase_Retail* GetTC(CThing_Retail* thing, long kind) { return FindTC(thing->m_tcs, kind); }
FABLE_FORCEINLINE CTCBase_Retail* GetTCConst(const CThing_Retail* thing, long kind) { return FindTC(thing->m_tcs, kind); }
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;              // 0x00487DC0
    const CThingPlayerCreature* PeekPControlledCreature() const;        // 0x00487DD0
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
class CCharString;
struct CDefinitionManager_Retail {
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
class CWorld; class CMainGameComponent; class CDisplayEngine; class CThingSearchTools;
FABLE_FORCEINLINE CTCBase_Retail* GetTCIfPresent(CThing_Retail* thing, long kind)
{
    CTCBase_Retail* tc;                            // deliberately unset when the component is absent (retail reads a dead slot)
    if (thing->HasTC(kind))
        tc = GetTC(thing, kind);
    return tc;
}
FABLE_FORCEINLINE CTCBase_Retail* GetTCIfPresentConst(const CThing_Retail* thing, long kind)
{
    CTCBase_Retail* tc;
    if (thing->HasTC(kind))
        tc = GetTCConst(thing, kind);
    return tc;
}
template <class T> FABLE_FORCEINLINE bool GetTCOut(CThing_Retail* thing, long kind, T** out)   // the out-parameter lookup: the component stays in eax
{
    if (thing->HasTC(kind)) {
        *out = (T*)GetTC(thing, kind);
        return true;
    }
    return false;
}
class CCharString { public:                               // fable_string.h layout: one data-block pointer
    void* m_pData;
    CCharString(const char* text, long length);    // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
FABLE_FORCEINLINE bool IsThingAlive(const CScriptThing& thing)   // the thing exists and is not dead
{
    CThing_Retail* t = ((CThing_Retail*)thing.GetPThing());
    return t && !t->m_dead;
}
class CTCParamsBase;
struct CTCMoneyReceiver_Retail {
    unsigned char _pad[0xc];
    long m_amountWanted;                           // +0x0c
};
class CTCBase;
class CThing { public:
    CTCBase* AddTC(CCharString const&a0, bool a1, CTCParamsBase const*a2);   // 0x004C9D60 (Ego_r.pdb ?AddTC@CThing@@QAEPAVCTCBase@@ABVCCharString@@_NPBVCTCParamsBase@@@Z)
};
struct CThingWithAddTC_Retail : CThing_Retail {
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::EntitySetThingAsWantingMoney(const CScriptThing& thing, long amount) const
{
    if (thing.IsAlive()) {
        CThingWithAddTC_Retail* t = (CThingWithAddTC_Retail*)((CThing_Retail*)thing.GetPThing());
        CTCMoneyReceiver_Retail* receiver;
        if (t->HasTC(0x80))
            receiver = (CTCMoneyReceiver_Retail*)GetTC(t, 0x80);
        else {
            CCharString name("CTCMoneyReceiver", -1);
            receiver = (CTCMoneyReceiver_Retail*)((CTCBase_Retail*)((CThing*)t)->AddTC(name, false, 0));
        }
        receiver->m_amountWanted = amount;
    }
}