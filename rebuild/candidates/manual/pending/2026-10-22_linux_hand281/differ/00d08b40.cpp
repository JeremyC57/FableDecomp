// Grade: DIFFER(778v778) -- every byte matches but one jump: the pre-guard early return targets the inline-dtor block's duplicated epilogue (0xd08de2) instead of the final epilogue after the out-of-line exit (retail 0xd08e42); tried goto/label/nesting/forceinline-helper variants (probe/chd_*.cpp)
// CCampHostageDoor::Main() -- bandit-camp boss battle, the hostage cage door: hold an alive filter over Me, look up the
//   CampHostageGuard; each frame until the hostages are rescued, when the hero uses the door: with the hostage key, either
//   (guard alive and within 5 units of the hero) raise PlayGuardTooCloseCutscene and wait for it to clear, or award the
//   rescue experience, mark the hostages rescued, take the key and stop; without the key show TEXT_QST_009_NEED_KEY and
//   wait for it to be clicked past
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D08B40, 778 bytes (784 with padding); entity-script vtable slot 1 (Main).
// Callees: CScriptThing copy ctor 0x004ABE90 / dtor 0x004AA840 / slot 20 MsgIsUsedBy, the thing implementation's slot 75
//   IsAlive (through the guard's inline CScriptThing::IsAlive), ~CBase 0x0099A2E0 and operator delete 0x00BFE9BC in the
//   expanded guard destructor, CIsThingAlive filter (vtable 0x012C32F0, inline dtor), CEntityScriptBase::SetPredicate
//   0x00F35B10 / IsActiveThreadTerminating 0x00F35B30, IsDistanceBetweenThingsUnder 0x00CBE2FF (__fastcall), CCharString
//   ctor 0x0099EBF0 / dtor 0x0099EAE0; CGameScriptInterface slots 7 NewScriptFrame, 40 MsgIsGameInfoClickedPast, 70 GetHero,
//   72 GetThingWithScriptName, 115 DisplayGameInfo, 125 TakeObjectFromHero, 128 GiveHeroExperience,
//   184 IsObjectInThingsPossession; the rescue experience is the long at +0x44 of the script def 0x0143E90C (unnamed).
//   Literals "CampHostageGuard" 0x012C89DC, "SCRIPT_NAME_HERO" 0x0125D1C8, "OBJECT_BANDIT_CAMP_HOSTAGE_KEY" 0x012C8430,
//   "TEXT_QST_009_NEED_KEY" 0x012C9118.
// Layout: Ego_r.pdb CCampHostageDoor (28 bytes, the CQ_BanditCampBossBattleScript one): CEntityScriptBase {vptr, Game +4,
//   Me +8}, ParentClass +0x14, PMasterData +0x18; HostagesRescued PDB +0x62 / PlayGuardTooCloseCutscene +0x77 = retail
//   +0x4e / +0x63.
// Shape: the inline-once destructor band (landed 0x00EDD060 idiom): the guard's destructor is expanded at the first return
//   and at the loop's normal end (one shared block), every other exit calls the out-of-line copy 0x004AA840; the guard is a
//   destructor-less view destroyed by hand per exit. The short-circuit SCRIPT_NAME_HERO temporary carries VC's
//   conditional-destruction flag (frame +0x10).
struct C3DVector { float X, Y, Z; };
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void operator delete(void* p);                         // 0x00BFE9BC (MSVCR71)
struct CCPPointerInfo {                                // CCountedPointer's shared block
    long RefCount;                                     // +0
    void (__fastcall* DeleteFunc)(void* data);         // +4
    void* Data;                                        // +8
};
struct CScriptThingImpl {                              // the thing implementation behind a CScriptThing
    virtual void i000(); virtual void i001(); virtual void i002(); virtual void i003(); virtual void i004(); virtual void i005(); virtual void i006(); virtual void i007();
    virtual void i008(); virtual void i009(); virtual void i010(); virtual void i011(); virtual void i012(); virtual void i013(); virtual void i014(); virtual void i015();
    virtual void i016(); virtual void i017(); virtual void i018(); virtual void i019(); virtual void i020(); virtual void i021(); virtual void i022(); virtual void i023();
    virtual void i024(); virtual void i025(); virtual void i026(); virtual void i027(); virtual void i028(); virtual void i029(); virtual void i030(); virtual void i031();
    virtual void i032(); virtual void i033(); virtual void i034(); virtual void i035(); virtual void i036(); virtual void i037(); virtual void i038(); virtual void i039();
    virtual void i040(); virtual void i041(); virtual void i042(); virtual void i043(); virtual void i044(); virtual void i045(); virtual void i046(); virtual void i047();
    virtual void i048(); virtual void i049(); virtual void i050(); virtual void i051(); virtual void i052(); virtual void i053(); virtual void i054(); virtual void i055();
    virtual void i056(); virtual void i057(); virtual void i058(); virtual void i059(); virtual void i060(); virtual void i061(); virtual void i062(); virtual void i063();
    virtual void i064(); virtual void i065(); virtual void i066(); virtual void i067(); virtual void i068(); virtual void i069(); virtual void i070(); virtual void i071();
    virtual void i072(); virtual void i073(); virtual void i074();
    virtual bool IsAlive() const;   // slot 75 (+0x12c)
};
struct CCountedPointerWords {                          // CCountedPointer<T> {Data, Info}
    CScriptThingImpl* Data;
    CCPPointerInfo*   Info;
    __forceinline void ReleaseInline()                 // ~CCountedPointer body
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
struct CBase {
    virtual ~CBase();                                  // 0x0099A2E0
};
struct CScriptThing : CBase {                          // vtable 0x01238C8C, 12 bytes
    virtual void t001(); virtual void t002(); virtual void t003(); virtual void t004(); virtual void t005(); virtual void t006(); virtual void t007(); virtual void t008();
    virtual void t009(); virtual void t010(); virtual void t011(); virtual void t012(); virtual void t013(); virtual void t014(); virtual void t015(); virtual void t016();
    virtual void t017(); virtual void t018(); virtual void t019();
    virtual bool MsgIsUsedBy(const CCharString& name) const;   // slot 20 (+0x50)
    CCountedPointerWords PImp;                         // +0x04
    CScriptThing(const CScriptThing& other);           // 0x004ABE90
    ~CScriptThing();                                   // 0x004AA840
};
struct CScriptThingInline : CBase {                    // the same object, destroyed by the inline body
    CCountedPointerWords PImp;
    __forceinline ~CScriptThingInline() { PImp.ReleaseInline(); }
};
struct CScriptThingLocal {                             // a by-value CScriptThing result held without an implicit destructor
    void*                vptr;
    CCountedPointerWords PImp;
    const CScriptThing& Thing() const { return *reinterpret_cast<const CScriptThing*>(this); }
    bool IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }   // CScriptThing::IsAlive (slot 75), devirtualised
    void Destroy() { reinterpret_cast<CScriptThing*>(this)->CScriptThing::~CScriptThing(); }
    __forceinline void DestroyInline() { reinterpret_cast<CScriptThingInline*>(this)->CScriptThingInline::~CScriptThingInline(); }
};
struct CGameScriptInterface {                          // vtable 0x01260F0C (ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c)
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual bool MsgIsGameInfoClickedPast() const;   // slot 40 (+0xa0)
    virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048();
    virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056();
    virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064();
    virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual CScriptThing& GetHero();   // slot 70 (+0x118)
    virtual void v071();
    virtual CScriptThingLocal GetThingWithScriptName(const CCharString& name) const;   // slot 72 (+0x120)
    virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080();
    virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088();
    virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096();
    virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104();
    virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112();
    virtual void v113(); virtual void v114();
    virtual void DisplayGameInfo(const CCharString& text) const;   // slot 115 (+0x1cc)
    virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123();
    virtual void v124();
    virtual void TakeObjectFromHero(const CCharString& object) const;   // slot 125 (+0x1f4)
    virtual void v126(); virtual void v127();
    virtual void GiveHeroExperience(long amount) const;   // slot 128 (+0x200)
    virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136();
    virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143(); virtual void v144();
    virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149(); virtual void v150(); virtual void v151(); virtual void v152();
    virtual void v153(); virtual void v154(); virtual void v155(); virtual void v156(); virtual void v157(); virtual void v158(); virtual void v159(); virtual void v160();
    virtual void v161(); virtual void v162(); virtual void v163(); virtual void v164(); virtual void v165(); virtual void v166(); virtual void v167(); virtual void v168();
    virtual void v169(); virtual void v170(); virtual void v171(); virtual void v172(); virtual void v173(); virtual void v174(); virtual void v175(); virtual void v176();
    virtual void v177(); virtual void v178(); virtual void v179(); virtual void v180(); virtual void v181(); virtual void v182(); virtual void v183();
    virtual bool IsObjectInThingsPossession(const CCharString& object, const CScriptThing& thing) const;   // slot 184 (+0x2e0)
};
struct CIsThingAlive {                                 // vtable 0x012C32F0
    virtual void f00();
    CScriptThing m_thing;                              // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    ~CIsThingAlive() { }
};
struct CEntityScriptBase {                             // retail 0x14 bytes
    virtual void e00();
    CGameScriptInterface* Game;                        // +0x04
    CScriptThing          Me;                          // +0x08
    void SetPredicate(CIsThingAlive& predicate);       // 0x00F35B10
    bool IsActiveThreadTerminating() const;            // 0x00F35B30
};
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
struct CScriptDef {                                    // the global script def (0x0143E90C)
    char _pad[0x44];
    long HostageRescueExperience;                      // +0x44 (name descriptive)
};
extern CScriptDef* g_pScriptDef;                       // 0x0143E90C
struct CQ_BanditCampBossBattleScript {
    char _pad[0x4e];
    bool HostagesRescued;                              // +0x4e
    char _pad2[0x63 - 0x4f];
    bool PlayGuardTooCloseCutscene;                    // +0x63
};
struct CCampHostageDoor : CEntityScriptBase {
    CQ_BanditCampBossBattleScript* ParentClass;        // +0x14
    void*                          PMasterData;        // +0x18 CQ_SunnyvaleMasterData*
    void Main();
};

void CCampHostageDoor::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptThingLocal guard = Game->GetThingWithScriptName("CampHostageGuard");
    if (!IsActiveThreadTerminating()) {
        do {
            if (!ParentClass->HostagesRescued && Me.MsgIsUsedBy("SCRIPT_NAME_HERO")) {
                if (IsActiveThreadTerminating()) {
                    guard.Destroy();
                    return;
                }
                if (Game->IsObjectInThingsPossession("OBJECT_BANDIT_CAMP_HOSTAGE_KEY", Game->GetHero())) {
                    if (IsActiveThreadTerminating()) {
                        guard.Destroy();
                        return;
                    }
                    if (guard.IsAlive() && IsDistanceBetweenThingsUnder(Game->GetHero(), guard.Thing(), 5.0f)) {
                        if (IsActiveThreadTerminating()) {
                            guard.Destroy();
                            return;
                        }
                        ParentClass->PlayGuardTooCloseCutscene = true;
                        while (ParentClass->PlayGuardTooCloseCutscene) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) {
                                guard.Destroy();
                                return;
                            }
                        }
                    } else {
                        if (IsActiveThreadTerminating()) {
                            guard.Destroy();
                            return;
                        }
                        Game->GiveHeroExperience(g_pScriptDef->HostageRescueExperience);
                        ParentClass->HostagesRescued = true;
                        Game->TakeObjectFromHero("OBJECT_BANDIT_CAMP_HOSTAGE_KEY");
                        guard.Destroy();
                        return;
                    }
                } else {
                    if (IsActiveThreadTerminating()) {
                        guard.Destroy();
                        return;
                    }
                    Game->DisplayGameInfo("TEXT_QST_009_NEED_KEY");
                    while (!Game->MsgIsGameInfoClickedPast()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) {
                            guard.Destroy();
                            return;
                        }
                    }
                }
                if (IsActiveThreadTerminating()) {
                    guard.Destroy();
                    return;
                }
            }
            Game->NewScriptFrame();
        } while (!IsActiveThreadTerminating());
    }
    guard.DestroyInline();
}
