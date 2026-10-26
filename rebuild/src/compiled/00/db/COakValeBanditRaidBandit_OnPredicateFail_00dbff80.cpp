// COakValeBanditRaidBandit::OnPredicateFail() -- when the bandit is killed: flag the raid's bandit-killed byte, and unless the raid is over, re-target the raid's bandit thing to the one nearest the hero and flag it when that bandit is within 8 m of this one
// Leaf: OnPredicateFail
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DBFF80, 259 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CScriptThing slot 18 MsgIsKilledBy(const CCharString&) (+0x48) and slot 6 GetPos() (+0x18, const C3DVector&)
//   on the entity's own thing (+8); CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; CGameScriptInterface
//   (ghidra_out/cgsi_slots_aligned.tsv) GetHero 70 (+0x118) 0x00891CA0 and GetNearestWithScriptName 77 (+0x134) 0x0089A510
//   (CScriptThing by value); the CScriptThing temporary's destructor ~CScriptThing 0x004AA840; operator delete 0x00BFE9BC
//   (the inline counted-pointer assignment); __fastcall IsDistanceFromThingToPositionUnder(const CScriptThing&, const
//   C3DVector&, float) 0x00CBE4B7.
// Layout: entity-script base as the landed COakValeBanditRaidBandit::Init 0x00DBFEA0 (Game +4, Me +8, ParentClass +0x14);
//   the raid script (retail): RaidOver +0x48, BanditKilled +0x61, BanditInRange +0x62, Bandit (CScriptThing) +0x64.
// Shape: the CScriptThing assignment is the inline CCountedPointer operator= (source words read first); the name string
//   is the implicit conversion temporary built before the nested GetHero call.
void __cdecl operator delete(void* p);             // 0x00BFE9BC
class C3DVector { public: float X, Y, Z; };
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
};
struct SCountBlock {
    long  m_references;                            // +0
    void (__fastcall* m_pfnDestroy)(void*);        // +4
    void* m_pObject;                               // +8
};
struct CScriptThingImp;
struct CScriptThingPImp {                          // CCountedPointer<CScriptThingImp>
    CScriptThingImp* Data;
    SCountBlock*     Info;
    __forceinline CScriptThingPImp& operator=(const CScriptThingPImp& other)
    {
        SCountBlock* info = other.Info;
        CScriptThingImp* data = other.Data;
        if (Info != info) {
            if (Info) {
                --Info->m_references;
                if (Info->m_references == 0) {
                    Info->m_pfnDestroy(Info->m_pObject);
                    operator delete(Info);
                }
            }
            Data = data;
            Info = info;
            if (info)
                ++info->m_references;
        }
        return *this;
    }
};
class CScriptThing { public:                              // 12 bytes, vtable 0x01238C8C
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual const C3DVector& GetPos() const;       // slot 6 (+0x18) 0x004AA980
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual bool MsgIsKilledBy(const CCharString& killer) const;   // slot 18 (+0x48)
    CScriptThingPImp PImp;                         // +0x04
    ~CScriptThing();                               // 0x004AA840
    __forceinline CScriptThing& operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
};
bool __fastcall IsDistanceFromThingToPositionUnder(const CScriptThing& thing, const C3DVector& position, float distance);   // 0x00CBE4B7
class CGameScriptInterface { public:
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void v022();
    virtual void v023();
    virtual void v024();
    virtual void v025();
    virtual void v026();
    virtual void v027();
    virtual void v028();
    virtual void v029();
    virtual void v030();
    virtual void v031();
    virtual void v032();
    virtual void v033();
    virtual void v034();
    virtual void v035();
    virtual void v036();
    virtual void v037();
    virtual void v038();
    virtual void v039();
    virtual void v040();
    virtual void v041();
    virtual void v042();
    virtual void v043();
    virtual void v044();
    virtual void v045();
    virtual void v046();
    virtual void v047();
    virtual void v048();
    virtual void v049();
    virtual void v050();
    virtual void v051();
    virtual void v052();
    virtual void v053();
    virtual void v054();
    virtual void v055();
    virtual void v056();
    virtual void v057();
    virtual void v058();
    virtual void v059();
    virtual void v060();
    virtual void v061();
    virtual void v062();
    virtual void v063();
    virtual void v064();
    virtual void v065();
    virtual void v066();
    virtual void v067();
    virtual void v068();
    virtual void v069();
    virtual CScriptThing& GetHero();               // slot 70 (+0x118)
    virtual void v071();
    virtual void v072();
    virtual void v073();
    virtual void v074();
    virtual void v075();
    virtual void v076();
    virtual CScriptThing GetNearestWithScriptName(const CScriptThing& near, const CCharString& name) const;   // slot 77 (+0x134)
};
struct CQ_OakValeBanditRaidScript {
    unsigned char _pad00[0x48];
    bool          RaidOver;                        // +0x48
    unsigned char _pad49[0x61 - 0x49];
    bool          BanditKilled;                    // +0x61
    bool          BanditInRange;                   // +0x62
    CScriptThing  Bandit;                          // +0x64
};
struct CQ_SunnyvaleMasterData;
struct COakValeBanditRaidBandit {
    virtual void v00();
    CGameScriptInterface*       Game;              // +0x04
    CScriptThing                Me;                // +0x08
    CQ_OakValeBanditRaidScript* ParentClass;       // +0x14
    CQ_SunnyvaleMasterData*     PMasterData;       // +0x18
    virtual void OnPredicateFail();
};

void COakValeBanditRaidBandit::OnPredicateFail()
{
    if (Me.MsgIsKilledBy("")) {
        ParentClass->BanditKilled = true;
        if (!ParentClass->RaidOver) {
            ParentClass->Bandit = Game->GetNearestWithScriptName(Game->GetHero(), "OakValeBanditRaidBandit");
            if (IsDistanceFromThingToPositionUnder(ParentClass->Bandit, Me.GetPos(), 8.0f))
                ParentClass->BanditInRange = true;
        }
    }
}