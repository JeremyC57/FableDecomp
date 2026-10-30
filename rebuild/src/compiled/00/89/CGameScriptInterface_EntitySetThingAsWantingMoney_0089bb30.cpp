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
struct CTCBase_Retail;
struct STCEntry_Retail {                           // one CVectorMap<ETCInterfaceType, CTCBase*> element
    long            m_kind;
    CTCBase_Retail* m_pTC;
};
struct CTCMap_Retail {                             // the thing's component map at +0x44: a sorted vector of pairs
    STCEntry_Retail* m_pFirst;
    STCEntry_Retail* m_pLast;
    STCEntry_Retail* m_pEnd;
    STCEntry_Retail* LowerBound(const unsigned long& kind);         // 0x0040F020 (CVectorMap<ETCInterfaceType,...>::LowerBound; the key by address)
    STCEntry_Retail* LowerBound(const unsigned long& kind) const;   // 0x004365B0 (the const instantiation)
    __forceinline CTCBase_Retail* Find(long kind)          // the inlined map `find`: lower bound (the key in a stack temporary), end when the key is absent
    {
        STCEntry_Retail* it = LowerBound((unsigned long)kind);
        if (it == m_pLast || kind < it->m_kind)
            it = m_pLast;
        return it->m_pTC;
    }
    __forceinline CTCBase_Retail* Find(long kind) const
    {
        STCEntry_Retail* it = LowerBound((unsigned long)kind);
        if (it == m_pLast || kind < it->m_kind)
            it = m_pLast;
        return it->m_pTC;
    }
};
// Same-TU stand-in for CVectorMap::LowerBound 0x0040F020 (not the retail body).
#pragma auto_inline(off)
__declspec(noinline) STCEntry_Retail* CTCMap_Retail::LowerBound(const unsigned long& kind)
{
    STCEntry_Retail* it = m_pFirst;
    long count = m_pLast - it;
    while (count > 0) {
        long half = count / 2;
        if ((unsigned long)it[half].m_kind < kind) { it += half + 1; count -= half + 1; }
        else count = half;
    }
    return it;
}
#pragma auto_inline(on)
struct CThing_Retail {
    unsigned char  _pad00[0x10];
    unsigned char  m_type;                         // +0x10 (1 = a creature)
    unsigned char  _pad11[0xf];
    unsigned long  m_tcMask[8];                    // +0x20: which component kinds the thing holds
    unsigned char  _pad40[4];
    CTCMap_Retail  m_tcs;                          // +0x44
    unsigned char  _pad50[0x41];
    unsigned char  m_dead : 1;                     // +0x91 bit 0
    unsigned char  m_91_1 : 7;
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }   // the masked word
};
class CThingPlayerCreature : public CThing_Retail { };
__forceinline CTCBase_Retail* GetTC(CThing_Retail* thing, long kind) { return thing->m_tcs.Find(kind); }
__forceinline CTCBase_Retail* GetTCConst(const CThing_Retail* thing, long kind) { return thing->m_tcs.Find(kind); }
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;              // 0x00487DC0
    const CThingPlayerCreature* PeekPControlledCreature() const;        // 0x00487DD0
};
class CPlayerManager { public:
    CPlayer& GetMainPlayer();               // 0x00449970
};
class CCharString;
struct CDefinitionManager_Retail {
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
class CWorld; class CMainGameComponent; class CDisplayEngine; class CThingSearchTools;
__forceinline CTCBase_Retail* GetTCIfPresent(CThing_Retail* thing, long kind)
{
    CTCBase_Retail* tc;                            // deliberately unset when the component is absent (retail reads a dead slot)
    if (thing->HasTC(kind))
        tc = GetTC(thing, kind);
    return tc;
}
__forceinline CTCBase_Retail* GetTCIfPresentConst(const CThing_Retail* thing, long kind)
{
    CTCBase_Retail* tc;
    if (thing->HasTC(kind))
        tc = GetTCConst(thing, kind);
    return tc;
}
template <class T> __forceinline bool GetTCOut(CThing_Retail* thing, long kind, T** out)   // the out-parameter lookup: the component stays in eax
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
__forceinline bool IsThingAlive(const CScriptThing& thing)   // the thing exists and is not dead
{
    CThing_Retail* t = ((CThing_Retail*)thing.GetPThing());
    return t && !t->m_dead;
}
class CTCParamsBase;
struct CTCMoneyReceiver_Retail {
    unsigned char _pad[0xc];
    long m_amountWanted;                           // +0x0c
};
struct CThingWithAddTC_Retail : CThing_Retail {
    CTCBase_Retail* AddTC(const CCharString& name, bool activate, CTCParamsBase* params);   // 0x004C9D60
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
            receiver = (CTCMoneyReceiver_Retail*)t->AddTC(name, false, 0);
        }
        receiver->m_amountWanted = amount;
    }
}