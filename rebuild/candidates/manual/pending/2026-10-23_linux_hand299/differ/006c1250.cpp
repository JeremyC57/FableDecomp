// Grade: DIFFER(638v638): register rotation only -- retail picks edx/eax where ours picks eax/ecx for the subtitle font, the price delta, the 0x23 key address and both day reads (same rotation as sibling 006c0c90)
// CTCBuyableHouse::Update() -- (vfunc 9) set up the dressing once, then every 75 frames: create or kill the main sign post (announcing a newly buyable house on the subtitle line), create or kill the dressing sign post for an owned house with a dearer next level, keep the next rent day while the house is rented and lived in, and pay out rent when due
// Leaf: CTCBuyableHouse
// Retail 0x006C1250, 638 bytes, `ret`. Callees: SetupDressingAtLevel 0x006C0330, GFGetCurrentFrame 0x0049D870
// (__fastcall), NeedsMainSignPost 0x006BFB90, CIntelligentPointer::GetPItem 0x00A01B50, CTCBase::GetWorld (non-const)
// 0x00686A70 (world +0x38 virtual +0x628), CreateMainSignPost 0x006C0F00, IS_THING_ALIVE 0x004CC340 (__fastcall),
// CPlayerGui::Refresh 0x004378C0 on 0x013B8790, CCharString 0x0099EBF0 / operator= 0x0099EFE0 / dtor 0x0099EAE0,
// CThing::GetDisplayEngine 0x004C7A70 (+0x1614C font), CSubtitleRenderer::SetText 0x00492ED1 and 0x0048DD94 on the
// GUI's renderer +0x18, CThing::Kill 0x004C9B80, CreateDressingSignPost 0x006C0F70, LowerBound const 0x004365B0,
// CTCVillageMember::GetPVillage 0x007EE540, count_if<HasBuildingAsHomeOrWork> 0x006600EC (__stdcall),
// CTCBase::GetWorld const 0x00686A80, CreateRentMoney 0x006C0C90.
// Layout: Ego_r.pdb CTCBuyableHouse (PDef +0x0c, OwnedByPlayer +0x11, Rented +0x12, DayNextRentIsDue +0x18,
// CurrentDressLevel +0x1c, MainSignPost +0x24, DressingSignPost +0x2c, FrameToNextCheck +0x34,
// InitiallySetupDressLevel +0x40, VirtualMoneyBags +0x44, HasPreviouslyBeenBuyable +0x48); retail CBuyableHouseDef
// Price vector +0x28, DaysBetweenRentPayments +0x40; village people list +0x2c (landed IsHouseBeingUsed 0x006BFB10).
struct CTCBase;
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const char* text);           // 0x0099EFE0
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
struct CFontBank;
struct CDisplayEngine { unsigned char _pad[0x1614c]; CFontBank* SubtitleFont; };
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    void Kill(bool immediately);                        // 0x004C9B80
    CDisplayEngine* GetDisplayEngine();                 // 0x004C7A70
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;
    CThing* GetPItem() const;                           // 0x00A01B50
};
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);   // 0x004CC340
long __fastcall GFGetCurrentFrame();                    // 0x0049D870

struct SPeopleNode { SPeopleNode* _M_next; SPeopleNode* _M_prev; };
struct SPeopleIter { SPeopleNode* _M_node; SPeopleIter(SPeopleNode* n) : _M_node(n) {} };
struct CPeopleList {
    SPeopleNode* _M_node;
    SPeopleIter begin() const { return SPeopleIter(_M_node->_M_next); }
    SPeopleIter end() const { return SPeopleIter(_M_node); }
};
struct HasBuildingAsHomeOrWork { const CThing* Building; HasBuildingAsHomeOrWork(const CThing* b) : Building(b) {} };
long __stdcall CountIfHomeOrWork(SPeopleIter first, SPeopleIter last, HasBuildingAsHomeOrWork pred);  // 0x006600EC
struct CTCVillage { unsigned char _pad[0x2c]; CPeopleList People; };
struct CTCVillageMember { CTCVillage* GetPVillage() const; };   // 0x007EE540

struct CSubtitleRenderer {
    void SetText(CFontBank* font, const CCharString& text, unsigned long colour, float time, bool a, bool b);   // 0x00492ED1
    void Pause(bool pause);                             // 0x0048DD94
};
struct CPlayerGui {
    unsigned char     _pad00[0x18];
    CSubtitleRenderer Subtitles;                        // +0x18
    unsigned char     _pad19[0xfd - 0x19];
    bool              Busy;                             // +0xfd
    void Refresh(long flag);                            // 0x004378C0
};
extern CPlayerGui* g_pPlayerGui;                        // 0x013B8790

struct CGameState {
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
    virtual void v070();
    virtual void v071();
    virtual void v072();
    virtual void v073();
    virtual void v074();
    virtual void v075();
    virtual void v076();
    virtual void v077();
    virtual void v078();
    virtual void v079();
    virtual void v080();
    virtual void v081();
    virtual void v082();
    virtual void v083();
    virtual void v084();
    virtual void v085();
    virtual void v086();
    virtual void v087();
    virtual void v088();
    virtual void v089();
    virtual void v090();
    virtual void v091();
    virtual void v092();
    virtual void v093();
    virtual void v094();
    virtual void v095();
    virtual void v096();
    virtual void v097();
    virtual void v098();
    virtual void v099();
    virtual void v100();
    virtual void v101();
    virtual void v102();
    virtual void v103();
    virtual void v104();
    virtual void v105();
    virtual void v106();
    virtual void v107();
    virtual void v108();
    virtual void v109();
    virtual void v110();
    virtual void v111();
    virtual void v112();
    virtual void v113();
    virtual void v114();
    virtual void v115();
    virtual void v116();
    virtual void v117();
    virtual void v118();
    virtual void v119();
    virtual void v120();
    virtual void v121();
    virtual void v122();
    virtual void v123();
    virtual void v124();
    virtual void v125();
    virtual void v126();
    virtual void v127();
    virtual void v128();
    virtual void v129();
    virtual void v130();
    virtual void v131();
    virtual void v132();
    virtual void v133();
    virtual void v134();
    virtual void v135();
    virtual void v136();
    virtual void v137();
    virtual void v138();
    virtual void v139();
    virtual void v140();
    virtual void v141();
    virtual void v142();
    virtual void v143();
    virtual void v144();
    virtual void v145();
    virtual void v146();
    virtual void v147();
    virtual void v148();
    virtual void v149();
    virtual void v150();
    virtual void v151();
    virtual void v152();
    virtual void v153();
    virtual void v154();
    virtual void v155();
    virtual void v156();
    virtual void v157();
    virtual void v158();
    virtual void v159();
    virtual void v160();
    virtual void v161();
    virtual void v162();
    virtual void v163();
    virtual void v164();
    virtual void v165();
    virtual void v166();
    virtual void v167();
    virtual void v168();
    virtual void v169();
    virtual void v170();
    virtual void v171();
    virtual void v172();
    virtual void v173();
    virtual void v174();
    virtual void v175();
    virtual void v176();
    virtual void v177();
    virtual void v178();
    virtual void v179();
    virtual void v180();
    virtual void v181();
    virtual void v182();
    virtual void v183();
    virtual void v184();
    virtual void v185();
    virtual void v186();
    virtual void v187();
    virtual void v188();
    virtual void v189();
    virtual void v190();
    virtual void v191();
    virtual void v192();
    virtual void v193();
    virtual void v194();
    virtual void v195();
    virtual void v196();
    virtual void v197();
    virtual void v198();
    virtual void v199();
    virtual void v200();
    virtual void v201();
    virtual void v202();
    virtual void v203();
    virtual void v204();
    virtual void v205();
    virtual void v206();
    virtual void v207();
    virtual void v208();
    virtual void v209();
    virtual void v210();
    virtual void v211();
    virtual void v212();
    virtual void v213();
    virtual void v214();
    virtual void v215();
    virtual void v216();
    virtual void v217();
    virtual void v218();
    virtual void v219();
    virtual void v220();
    virtual void v221();
    virtual void v222();
    virtual void v223();
    virtual void v224();
    virtual void v225();
    virtual void v226();
    virtual void v227();
    virtual void v228();
    virtual void v229();
    virtual void v230();
    virtual void v231();
    virtual void v232();
    virtual void v233();
    virtual void v234();
    virtual void v235();
    virtual void v236();
    virtual void v237();
    virtual void v238();
    virtual void v239();
    virtual void v240();
    virtual void v241();
    virtual void v242();
    virtual void v243();
    virtual void v244();
    virtual void v245();
    virtual void v246();
    virtual void v247();
    virtual void v248();
    virtual void v249();
    virtual void v250();
    virtual void v251();
    virtual void v252();
    virtual void v253();
    virtual void v254();
    virtual void v255();
    virtual void v256();
    virtual void v257();
    virtual void v258();
    virtual void v259();
    virtual void v260();
    virtual void v261();
    virtual void v262();
    virtual void v263();
    virtual void v264();
    virtual void v265();
    virtual void v266();
    virtual void v267();
    virtual void v268();
    virtual void v269();
    virtual void v270();
    virtual void v271();
    virtual void v272();
    virtual void v273();
    virtual void v274();
    virtual void v275();
    virtual void v276();
    virtual void v277();
    virtual void v278();
    virtual void v279();
    virtual void v280();
    virtual void v281();
    virtual void v282();
    virtual void v283();
    virtual void v284();
    virtual void v285();
    virtual void v286();
    virtual void v287();
    virtual void v288();
    virtual void v289();
    virtual void v290();
    virtual void v291();
    virtual void v292();
    virtual void v293();
    virtual void v294();
    virtual void v295();
    virtual void v296();
    virtual void v297();
    virtual void v298();
    virtual void v299();
    virtual void v300();
    virtual void v301();
    virtual void v302();
    virtual void v303();
    virtual void v304();
    virtual void v305();
    virtual void v306();
    virtual void v307();
    virtual void v308();
    virtual void v309();
    virtual void v310();
    virtual void v311();
    virtual void v312();
    virtual void v313();
    virtual void v314();
    virtual void v315();
    virtual void v316();
    virtual void v317();
    virtual void v318();
    virtual void v319();
    virtual void v320();
    virtual void v321();
    virtual void v322();
    virtual void v323();
    virtual void v324();
    virtual void v325();
    virtual void v326();
    virtual void v327();
    virtual void v328();
    virtual void v329();
    virtual void v330();
    virtual void v331();
    virtual void v332();
    virtual void v333();
    virtual void v334();
    virtual void v335();
    virtual void v336();
    virtual void v337();
    virtual void v338();
    virtual void v339();
    virtual void v340();
    virtual void v341();
    virtual void v342();
    virtual void v343();
    virtual void v344();
    virtual void v345();
    virtual void v346();
    virtual void v347();
    virtual void v348();
    virtual void v349();
    virtual void v350();
    virtual void v351();
    virtual void v352();
    virtual void v353();
    virtual void v354();
    virtual void v355();
    virtual void v356();
    virtual void v357();
    virtual void v358();
    virtual void v359();
    virtual void v360();
    virtual void v361();
    virtual void v362();
    virtual void v363();
    virtual void v364();
    virtual void v365();
    virtual void v366();
    virtual void v367();
    virtual void v368();
    virtual void v369();
    virtual void v370();
    virtual void v371();
    virtual void v372();
    virtual void v373();
    virtual void v374();
    virtual void v375();
    virtual void v376();
    virtual void v377();
    virtual void v378();
    virtual void v379();
    virtual void v380();
    virtual void v381();
    virtual void v382();
    virtual void v383();
    virtual void v384();
    virtual void v385();
    virtual void v386();
    virtual void v387();
    virtual void v388();
    virtual void v389();
    virtual void v390();
    virtual void v391();
    virtual void v392();
    virtual void v393();
    virtual bool CanShowSignPosts();                   // +0x628
};
struct CWorldNC {
    unsigned char _pad[0x38];
    CGameState*   State;                                // +0x38
    bool CanShowSignPosts() { return State->CanShowSignPosts(); }
};
struct CGameTime { unsigned char _pad[0x14]; long Day; };
struct CWorld {
    unsigned char _pad[0x1c];
    CGameTime*    Time;                                 // +0x1c
    long GetDay() const { return Time->Day; }
};
struct CBuyableHouseDef {
    virtual void v00();
    long          m_refCount;                           // +0x04
    unsigned char _pad08[0x20];
    long*         Price_First;                          // +0x28
    long*         Price_Last;                           // +0x2c
    long*         Price_End;                            // +0x30
    long*         Rent_First;                           // +0x34
    long*         Rent_Last;                            // +0x38
    long*         Rent_End;                             // +0x3c
    long          DaysBetweenRentPayments;              // +0x40
    long          MaxRentBags;                          // +0x44
};

struct CTCBuyableHouse {
    CThing*           ParentThing;                      // +0x04
    unsigned char     _pad08[0x4];
    CBuyableHouseDef* PDef;                             // +0x0c
    bool              IsScripted;                       // +0x10
    bool              OwnedByPlayer;                    // +0x11
    bool              Rented;                           // +0x12
    long              DaySomeoneWillMoveIn;             // +0x14
    long              DayNextRentIsDue;                 // +0x18
    long              CurrentDressLevel;                // +0x1c
    long              MaxDressLevel;                    // +0x20
    CBaseIntelligentPointer MainSignPost;               // +0x24
    CBaseIntelligentPointer DressingSignPost;           // +0x2c
    long              FrameToNextCheckIfWeNeedToCreateAnything;   // +0x34
    CBaseIntelligentPointer WifeLivingHere;             // +0x38
    bool              InitiallySetupDressLevel;         // +0x40
    long              VirtualMoneyBags;                 // +0x44
    bool              HasPreviouslyBeenBuyable;         // +0x48
    virtual void Update();
    CWorldNC* GetWorldNC();                             // 0x00686A70
    CWorld* GetWorld() const;                           // 0x00686A80
    void SetupDressingAtLevel(long level);              // 0x006C0330
    bool NeedsMainSignPost();                           // 0x006BFB90
    void CreateMainSignPost();                          // 0x006C0F00
    void CreateDressingSignPost();                      // 0x006C0F70
    void CreateRentMoney();                             // 0x006C0C90
    __forceinline bool IsHouseBeingUsed() const
    {
        CTCVillageMember* member;
        if (ParentThing->GetInterface(&member, 0x23)) {
            CTCVillage* village = member->GetPVillage();
            if (village) {
                if (CountIfHomeOrWork(village->People.begin(), village->People.end(), HasBuildingAsHomeOrWork(ParentThing)) > 0)
                    return true;
            }
        }
        return false;
    }
    long GetNumberOfRentBagsDue() const
    {
        if (PDef && Rented && GetWorld()->GetDay() >= DayNextRentIsDue) {
            long due = (GetWorld()->GetDay() - DayNextRentIsDue) / PDef->DaysBetweenRentPayments + 1;
            if (due > 0)
                return due;
        }
        return 0;
    }
};

void CTCBuyableHouse::Update()
{
    if (!InitiallySetupDressLevel) {
        SetupDressingAtLevel(CurrentDressLevel);
        InitiallySetupDressLevel = true;
    }
    long frame = GFGetCurrentFrame();
    if (frame < FrameToNextCheckIfWeNeedToCreateAnything)
        return;
    FrameToNextCheckIfWeNeedToCreateAnything = frame + 75;
    if (NeedsMainSignPost()) {
        if (!MainSignPost.GetPItem() && GetWorldNC()->CanShowSignPosts() && !g_pPlayerGui->Busy) {
            CreateMainSignPost();
            if (IS_THING_ALIVE(MainSignPost) && !HasPreviouslyBeenBuyable) {
                g_pPlayerGui->Refresh(0);
                CCharString text("TEXT_GUI_BUYABLE_HOUSE_AVAILABLE");
                if (ParentThing->HasTC(0x53) && !ParentThing->HasTC(0xab))
                    text = "TEXT_GUI_BUYABLE_HOUSE_AVAILABLE_SHOP";
                CFontBank* font = ParentThing->GetDisplayEngine()->SubtitleFont;
                g_pPlayerGui->Subtitles.SetText(font, text, 0x22, 0.0f, false, true);
                g_pPlayerGui->Subtitles.Pause(true);
            }
            HasPreviouslyBeenBuyable = true;
        }
    } else if (MainSignPost.GetPItem()) {
        MainSignPost.GetPItem()->Kill(true);
        g_pPlayerGui->Refresh(0);
    }
    if (OwnedByPlayer && PDef && CurrentDressLevel >= 0
        && (unsigned long)(CurrentDressLevel + 1) < (unsigned long)(PDef->Price_Last - PDef->Price_First)
        && PDef->Price_First[CurrentDressLevel + 1] - PDef->Price_First[CurrentDressLevel] > 0) {
        if (!DressingSignPost.GetPItem())
            CreateDressingSignPost();
    } else if (DressingSignPost.GetPItem()) {
        DressingSignPost.GetPItem()->Kill(true);
    }
    if (IsHouseBeingUsed() && Rented) {
        if (DayNextRentIsDue == 0x7fffffff)
            DayNextRentIsDue = GetWorld()->GetDay() + 1;
    } else {
        DayNextRentIsDue = 0x7fffffff;
    }
    if (GetNumberOfRentBagsDue() > 0 || VirtualMoneyBags > 0)
        CreateRentMoney();
}
