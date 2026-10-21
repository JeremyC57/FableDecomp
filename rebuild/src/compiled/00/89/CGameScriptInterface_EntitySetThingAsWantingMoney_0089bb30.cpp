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
__forceinline CTCBase_Retail* GetTC(CThing_Retail* thing, long kind) { return thing->m_tcs.Find(kind); }
__forceinline CTCBase_Retail* GetTCConst(const CThing_Retail* thing, long kind) { return thing->m_tcs.Find(kind); }
struct CScriptThing {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010();
    virtual CThing_Retail* GetThing() const;       // slot 11 (+0x2c)
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043();
    virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051();
    virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067();
    virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual bool IsValid() const;                  // slot 75 (+0x12c)
    CScriptThing();                                // 0x008CFF40 (the null script thing)
};
struct CPlayer_Retail {
    CThing_Retail* GetHeroCreature();              // 0x00487DC0
    CThing_Retail* GetControlledCreature();        // 0x00487DD0
};
struct CPlayerManager_Retail {
    CPlayer_Retail* GetMainPlayer();               // 0x00449970
};
struct CCharString;
struct CDefinitionManager_Retail {
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
struct CWorld; struct CMainGameComponent; struct CDisplayEngine; struct CThingSearchTools;
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
struct CCharString {                               // fable_string.h layout: one data-block pointer
    void* m_pData;
    CCharString(const char* text, long length);    // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
__forceinline bool IsThingAlive(const CScriptThing& thing)   // the thing exists and is not dead
{
    CThing_Retail* t = thing.GetThing();
    return t && !t->m_dead;
}
struct CTCParamsBase;
struct CTCMoneyReceiver_Retail {
    unsigned char _pad[0xc];
    long m_amountWanted;                           // +0x0c
};
struct CThingWithAddTC_Retail : CThing_Retail {
    CTCBase_Retail* AddTC(const CCharString& name, bool activate, CTCParamsBase* params);   // 0x004C9D60
};
struct CGameScriptInterface {
    virtual void v000();
    CWorld*                     World;             // +0x04 (engine/CGameScriptInterface.h)
    CMainGameComponent*         Component;         // +0x08
    CDisplayEngine*             DisplayEngine;     // +0x0c
    CDefinitionManager_Retail*  DefinitionManager; // +0x10
    CPlayerManager_Retail*      PlayerManager;     // +0x14
    CThingSearchTools*          ThingSearchTools;  // +0x18
    virtual void EntitySetThingAsWantingMoney(const CScriptThing& thing, long amount) const;
};

void CGameScriptInterface::EntitySetThingAsWantingMoney(const CScriptThing& thing, long amount) const
{
    if (thing.IsValid()) {
        CThingWithAddTC_Retail* t = (CThingWithAddTC_Retail*)thing.GetThing();
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