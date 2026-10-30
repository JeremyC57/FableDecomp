// CGameScriptInterface::OpenChest(const CScriptThing& chest, bool open) const -- open (hero plays the open-chest action; opening a chest owned by a living, guarded owner posts a theft deed) or close a scripted chest
// Grade: DIFFER(709v711) nd=105 (was 698v711 nd=134) -- levers: every return false through one fail: label (retail's shared false tail and up-front saves of all four registers), the 0x33 component as Ego_r.pdb CTCOwnedEntity with POwner at +0x0d under pack(1) (was a local CTCOwnership/Owner); remaining: this in edi (retail ebp) and the owner pointer in ebx (retail edi), the 0x80000 mask hoisted into ebx in retail, the open-chest action temporary at +0xc0 with a second intelligent-pointer dtor, IS_THING_ALIVE_PHYSICAL tested as a dword
// Leaf: ?OpenChest@CGameScriptInterface
extern "C" int __cdecl puts(const char* text);
extern "C" __declspec(noreturn) void __cdecl abort();
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
struct CThing_Retail {
    unsigned char  _pad00[0x10];
    unsigned char  m_type;                         // +0x10 (1 = a creature)
    unsigned char  _pad11[0xf];
    unsigned long  m_tcMask[9];                    // +0x20: which component kinds the thing holds (bitset<274>)
    CTCMap_Retail  m_tcs;                          // +0x44
    unsigned char  _pad50[0x3c];
    unsigned short m_id;                           // +0x8c
    unsigned char  _pad8e[3];
    unsigned char  m_dead : 1;                     // +0x91 bit 0
    unsigned char  m_91_1 : 7;
    void SetInLimbo(bool limbo);                   // 0x004C8CF0
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }   // the masked word
    __forceinline bool TestComponent(unsigned long position) const    // STLport bitset<274>::test with its range check
    {
        if (position >= 274) {
            puts("bitset");
            abort();
        }
        return (m_tcMask[position >> 5] & (1UL << (position & 31))) != 0;
    }
};
__declspec(noinline) STCEntry_Retail* CTCMap_Retail::LowerBound(const unsigned long& kind)
{
    STCEntry_Retail* it = m_pFirst;
    while (it != m_pLast && (unsigned long)it->m_kind < kind)
        ++it;
    return it;
}
__declspec(noinline) STCEntry_Retail* CTCMap_Retail::LowerBound(const unsigned long& kind) const
{
    STCEntry_Retail* it = m_pFirst;
    while (it != m_pLast && (unsigned long)it->m_kind < kind)
        ++it;
    return it;
}
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
};
struct CPlayerManager_Retail {
    CPlayer_Retail* GetMainPlayer();               // 0x00449970
};
struct CCharString {
    void* m_pData;
    ~CCharString();                                // 0x0099EAE0
};
struct C3DVector;
struct CThingCreature_Retail : CThing_Retail {
};
struct CBaseObject;
struct CBaseIntelligentPointer {
    void* Data;
    CBaseObject* GetPItem() const;                 // 0x00A01B50
    ~CBaseIntelligentPointer();                    // 0x00A01C10
};
struct CThingFlags_Retail {
    unsigned char _pad[0x24];
    unsigned long Flags24;                         // +0x24
    unsigned long Flags28;                         // +0x28
    unsigned char _pad2c[8];
    unsigned long Flags34;                         // +0x34
    unsigned long Flags38;                         // +0x38
};
bool __fastcall IS_THING_ALIVE(CBaseObject** thing);              // 0x0040DCE0
bool __fastcall IS_THING_ALIVE_PHYSICAL(CTCBase_Retail* pointer); // 0x007EE540
struct CCreatureActionBase {
    unsigned char _body[0xa8];
    ~CCreatureActionBase();                        // 0x00693EF0
};
struct CCreatureAction_OpenChest : CCreatureActionBase {
    CBaseIntelligentPointer Target;                // +0xa8
    CCreatureAction_OpenChest(CThing_Retail* creature, CThing_Retail* chest, const CCharString& anim);   // 0x007EE7E0
};
struct CTCChest {
    CCharString GetOpenAnimName();                 // 0x00750240
    void Close();                                  // 0x00750540
};
struct CTCHero {
    bool GiveTutorialForCategory(long category, bool force);   // 0x006C9FA0
};
struct CTCHeroOpinionDeedLog {
    long PostDeed(long type, const C3DVector& pos, CThing_Retail* thing, void* victim, long postType, bool a, bool b, void* faction, bool c, long d);   // 0x006C6990
};
#pragma pack(push, 1)
struct CTCOwnedEntity {                            // Ego_r.pdb: SwitchableNavigationTCAdded +0x0c, POwner +0x0d
    unsigned char           _pad[0xd];
    CBaseIntelligentPointer POwner;                // +0x0d
};
#pragma pack(pop)
struct CTCFlag11 {
    unsigned char _pad[0x11];
    bool          Flag;                            // +0x11
};
struct CThingCreatureMethods {
    void FinishCurrentAction();                    // 0x00662650
    bool SetCurrentAction(CCreatureActionBase* action);   // 0x006644F0
    const C3DVector& GetPosition() const;          // 0x004C73D0
};
struct CTutorialState { unsigned char _pad[0xf6]; bool TutorialsOff; };
extern CTutorialState* g_pTutorialState;            // 0x013B8790
struct CGameScriptInterface {
    virtual void v000();
    void* World; void* Component; void* DisplayEngine; void* DefinitionManager;
    CPlayerManager_Retail* PlayerManager;          // +0x14
    virtual bool OpenChest(const CScriptThing& chest, bool open) const;
};
#define FLAGS(t) ((CThingFlags_Retail*)(t))

bool CGameScriptInterface::OpenChest(const CScriptThing& chest, bool open) const
{
    if (!chest.IsValid())
        goto fail;
    CThing_Retail* thing = chest.GetThing();
    if (!(FLAGS(thing)->Flags24 & 0x10000000))
        goto fail;
    CTCChest* tc = (CTCChest*)GetTC(thing, 0x3c);
    if (open) {
    CThing_Retail* hero = PlayerManager->GetMainPlayer()->GetHeroCreature();
    if (!hero || hero->m_dead)
        goto fail;
    if (!g_pTutorialState->TutorialsOff && (FLAGS(hero)->Flags24 & 0x200)) {
        if (((CTCHero*)GetTC(hero, 0x29))->GiveTutorialForCategory(0x21, false))
            goto fail;
    }
    ((CThingCreatureMethods*)hero)->FinishCurrentAction();
    bool started = ((CThingCreatureMethods*)hero)->SetCurrentAction(&CCreatureAction_OpenChest(hero, thing, tc->GetOpenAnimName()));
    if (!started)
        goto fail;
    if (FLAGS(thing)->Flags24 & 0x80000) {
        CBaseIntelligentPointer& owner = ((CTCOwnedEntity*)GetTCConst(thing, 0x33))->POwner;
        CBaseObject* o = owner.GetPItem();
        if (IS_THING_ALIVE(&o)) {
            CThing_Retail* ot = (CThing_Retail*)owner.GetPItem();
            if ((FLAGS(ot)->Flags24 & 8) && IS_THING_ALIVE_PHYSICAL(GetTC(ot, 0x23))) {
                if ((FLAGS(owner.GetPItem())->Flags34 & 0x20000000) || (FLAGS(owner.GetPItem())->Flags28 & 0x80000)) {
                    if (!(FLAGS(owner.GetPItem())->Flags38 & 0x200000) || !((CTCFlag11*)GetTC((CThing_Retail*)owner.GetPItem(), 0xd5))->Flag) {
                        if (FLAGS(hero)->Flags28 & 0x20000) {
                            CTCHeroOpinionDeedLog* log = (CTCHeroOpinionDeedLog*)GetTC(hero, 0x51);
                            log->PostDeed(6, ((CThingCreatureMethods*)hero)->GetPosition(), thing, 0, 2, true, false, 0, false, 0);
                        }
                    }
                }
            }
        }
    }
    return true;
    }
    tc->Close();
    return true;
fail:
    return false;
}
