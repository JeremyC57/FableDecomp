// CTCCreatureOpinionOfHero::GetGreetDialogueGroupIndex(EOpinionAttitudeType attitude) -- pick the text-bank group a creature greets the hero with: guards use the guard (re)greet group (+ "_ASBOY"/"_ASAPPRENTICE" by the hero's life stage); drunks the drunk greet; spouses their own group; otherwise, unless the hero is disguised, a strongly-opinioned creature may use the hero's title group; then the stage group, else the attitude-suffixed group (0 when none)
// Grade: DIFFER(1085v1085) -- same size; retail places the guard (life state 3) block at the function end (`je` far) with this in ebx, the bank in edi and the hero spilled to the slot the guard string later reuses; ours lays the guard block out inline after `jne` (tried: if/else both ways, goto -- the else form grows the frame to 0x18)
// Leaf: GetGreetDialogueGroupIndex
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00683C10, 1088 bytes, `ret 4`, no EH frame.
// Callees: CTCBase::GetWorld 0x00686A70 / GetWorldConst 0x00686A80 / GetPRandomSeed 0x00686DA0, CWorld::GetCurrentPlayer
// 0x0049D9D0, CPlayer::GetControlledCreature 0x00487DD0, LowerBound 0x0040F020 / const 0x004365B0,
// CThingCreatureBase::GetLifeState 0x00662010, CCharString ctor 0x0099EBF0 / dtor 0x0099EAE0 / operator=(const char*)
// 0x0099EFE0 / +=(const char*) 0x0099F100 / +=(const CCharString&) 0x0099F0A0 / !=(const char*) 0x0099E960 / GetCRC
// 0x00404310 (inlined once: CRC Calc 0x004014A0), CDataBank::LookupValue 0x009CC410, COpinionReactionManager::GetAttitudeType
// 0x00681500 / GetBlurredAttitude 0x00700120 / GetOpinionValue 0x00701750, GFFloatRandom 0x004978A0, CTCWife::
// IsMarriedToThePlayer 0x006CEC80 / GetGreetDialogueGroupName 0x006D0410. Globals: g_pMainGameComponent 0x013B86A0
// (+0x14 the text bank), the attitude suffix strings 0x013BABD0, 0.35f 0x0125BAA0.
// Layout: engine/CTCCreatureOpinionOfHero.h is the Ego_r.pdb layout and does not fit retail (STLport containers shrink it):
// a local model keeps ParentThing +0x04 and the retail greeted flag at +0x341.
struct CStringData { const char* Data; unsigned long Length; };
unsigned long __fastcall Calc(unsigned long seed, const void* data, unsigned long length);   // 0x004014A0
struct CCharString {
    CStringData* PStringData;
    CCharString(const char* text, long length = -1);         // 0x0099EBF0
    ~CCharString();                                          // 0x0099EAE0
    CCharString& operator=(const char* text);                // 0x0099EFE0
    CCharString& operator+=(const char* text);               // 0x0099F100
    CCharString& operator+=(const CCharString& other);       // 0x0099F0A0
    bool operator!=(const char* text) const;                 // 0x0099E960
    unsigned long GetCRC() const
    {
        const char* data;
        unsigned long length;
        if (!PStringData) {
            length = 0;
            data = "";
        } else {
            length = PStringData->Length;
            data = PStringData->Data;
        }
        return Calc(0, data, length);
    }
};
extern CCharString g_OpinionAttitudeSuffixes[];              // 0x013BABD0
struct CDataBank {
    unsigned long LookupValue(unsigned long crc) const;      // 0x009CC410
};
struct CMainGameComponent {
    unsigned char _pad00[0x14];
    CDataBank* TextBank;                                     // +0x14
};
extern CMainGameComponent* g_pMainGameComponent;             // 0x013B86A0
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0

struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                    // 0x0040F020
    const SEntry* LowerBound(const long* type) const;        // 0x004365B0
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                          // +0x20
    CInterfaceMap m_interfaces;                              // +0x44
    unsigned char _pad4c[0x6c - 0x4c];
    unsigned char m_flags6c;                                 // +0x6c bit 3: can greet
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        T* tc;
        if (HasTC(interfaceType)) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            tc = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
        }
        return tc;
    }
    template<class T> __forceinline T* GetTCConst(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};
struct CThingCreatureBase : CThing {
    long GetLifeState() const;                               // 0x00662010
};
struct CTCHeroStats { unsigned char _pad[0x114]; long LifeStage; };     // interface 4, +0x114
struct CTCDrunk { unsigned char _pad[0x0c]; bool Drunk; };              // interface 0xc3, +0x0c
struct CTCHeroDisguise;                                                  // interface 0x50
struct CTitleInfo { unsigned char _pad[0x30]; unsigned long GreetGroup; };
struct CTCHeroTitle { unsigned char _pad[0x18]; CTitleInfo* Title; };   // interface 0x29, +0x18
struct CTCWife {
    bool IsMarriedToThePlayer() const;                       // 0x006CEC80
    bool GetGreetDialogueGroupName(CCharString* name) const; // 0x006D0410
};
struct COpinionReactionManager {
    long GetAttitudeType(const CThing* creature, const CTCHeroDisguise* disguise);   // 0x00681500
    long GetBlurredAttitude(long attitude);                  // 0x00700120
    float GetOpinionValue(const CThing* creature, bool a);   // 0x00701750
};
struct CPlayer { CThing* GetControlledCreature(); };        // 0x00487DD0
struct CWorld {
    unsigned char _pad00[0x74];
    COpinionReactionManager* OpinionReactionManager;         // +0x74
    CPlayer* GetCurrentPlayer() const;                       // 0x0049D9D0
};
struct CTCCreatureOpinionOfHero {
    virtual void v00();
    CThingCreatureBase* ParentThing;                         // +0x04
    unsigned char _pad08[0x341 - 0x08];
    bool GreetedFlag;                                        // +0x341
    const CWorld* GetWorldConst() const;                     // 0x00686A80
    CWorld* GetWorld();                                      // 0x00686A70
    unsigned long* GetPRandomSeed();                         // 0x00686DA0
    unsigned long GetGreetDialogueGroupIndex(long attitude);
};

unsigned long CTCCreatureOpinionOfHero::GetGreetDialogueGroupIndex(long attitude)
{
    if (!(ParentThing->m_flags6c & 8))
        return 0;
    CDataBank* bank = g_pMainGameComponent->TextBank;
    CThing* hero = GetWorld()->GetCurrentPlayer()->GetControlledCreature();
    long lifeStage;
    if (hero && hero->HasTC(4))
        lifeStage = hero->GetTCConst<CTCHeroStats>(4)->LifeStage;
    else
        lifeStage = 0;
    if (ParentThing->GetLifeState() == 3) {
        CCharString guardName(GreetedFlag ? "TEXT_AI_GUARD_REGREET_TO_HERO" : "TEXT_AI_GUARD_GREET_TO_HERO");
        if (lifeStage == 2)
            guardName += "_ASBOY";
        else if (lifeStage == 1)
            guardName += "_ASAPPRENTICE";
        return bank->LookupValue(guardName.GetCRC());
    }
    CCharString name("");
    if (ParentThing->HasTC(0xc3) && ParentThing->GetTCConst<CTCDrunk>(0xc3)->Drunk) {
        name = "TEXT_AI_OPINION_ACT_GREET_TO_HERO_DRUNK";
        return bank->LookupValue(name.GetCRC());
    }
    const CTCHeroDisguise* disguise = 0;
    if (hero && hero->HasTC(0x50))
        disguise = hero->GetTCConst<CTCHeroDisguise>(0x50);
    COpinionReactionManager* manager = GetWorldConst()->OpinionReactionManager;
    long type = attitude;
    if (attitude == 0x12) {
        type = manager->GetAttitudeType(ParentThing, disguise);
        if (!disguise)
            type = manager->GetBlurredAttitude(type);
    }
    if (type == 3 || type == 8 || type == 4) {
        if (ParentThing->HasTC(0xd7)) {
            const CTCWife* wife = ParentThing->GetTCConst<CTCWife>(0xd7);
            if (wife->IsMarriedToThePlayer() && wife->GetGreetDialogueGroupName(&name))
                return bank->LookupValue(name.GetCRC());
        }
    } else {
        float opinion = manager->GetOpinionValue(ParentThing, true);
        if (!disguise && GFFloatRandom(1.0f, GetPRandomSeed()) < opinion * opinion * 0.35f) {
            if (!(ParentThing->HasTC(0xd7) && ParentThing->GetTC<CTCWife>(0xd7)->IsMarriedToThePlayer())) {
                if (hero->HasTC(0x29)) {
                    CTitleInfo* title = hero->GetTCConst<CTCHeroTitle>(0x29)->Title;
                    if (title && title->GreetGroup > 0)
                        return title->GreetGroup;
                }
            }
        }
    }
    if (lifeStage) {
        name = GreetedFlag ? "TEXT_AI_OPINION_ACT_REGREET_TO_HERO" : "TEXT_AI_OPINION_ACT_GREET_TO_HERO";
        if (lifeStage == 2)
            name += "_ASBOY";
        else if (lifeStage == 1)
            name += "_ASAPPRENTICE";
        return bank->LookupValue(name.GetCRC());
    }
    const CCharString& suffix = g_OpinionAttitudeSuffixes[type];
    if (suffix != "") {
        name = GreetedFlag ? "TEXT_AI_OPINION_ACT_REGREET_TO_HERO_" : "TEXT_AI_OPINION_ACT_GREET_TO_HERO_";
        name += suffix;
        unsigned long group = bank->LookupValue(name.GetCRC());
        if (group > 0)
            return group;
    }
    return 0;
}
