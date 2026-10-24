// NScript::CQ_TraderConflictGoodScript::CTC_BanditGuard::Main() -- a bandit-camp security guard in the trader-conflict (good) quest: alive filter on Me, scripted control (priority 2, retried each frame), wait until the hero is within 15 units, then speak once -- a costume remark when the hero wears the whole bandit outfit or part of it (first time only), else one of the rotating security challenges -- and idle with scripted control released
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DFB320, 1637 bytes (+ int3 padding); entity-script vtable slot 1 (Main);
//   PDB ?Main@CTC_BanditGuard@CQ_TraderConflictGoodScript@NScript@@EAEXXZ.
// Callees: CScriptThing copy 0x004ABE90 / dtor 0x004AA840 (the CIsThingAlive predicate, vtable 0x012C32F0), SetPredicate
//   0x00F35B10, IsActiveThreadTerminating 0x00F35B30, CBaseObject ctor 0x0099A380, the scripted-thing handle's IsScripting
//   0x00CD23B9 / Release 0x00CD2770 / dtor 0x007E74D0, CCharString 0x0099EBF0 / 0x0099EAE0, IsDistanceBetweenThingsUnder
//   0x00CBE2FF (__fastcall); GameCalls slots 7 NewScriptFrame, 8 StartScriptingEntity, 70 GetHero, 187
//   IsWearingClothingItem, 364 AddNewConversation, 365 AddPersonToConversation, 366 AddLineToConversation.
// Strings: the five OBJECT_HERO_*_BANDITCAMP items 0x012C8544/8524/85A4/8564/8584, the costume lines 0x012DED88 / 0x012DED4C,
//   the security lines 0x012DECF4 (_10) and 0x012DED20 (_20, used by case 1 and the default: /GF pools the literal).
// Layout: Ego_r.pdb CTC_BanditGuard (0x1c): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14, PMasterData +0x18.
//   CQ_TraderConflictGoodScript sits 0x14 below the Ego_r.pdb offsets and its AllCreatures vector is the 12-byte retail
//   vector: BanditSecurityLinesSaid +0x5c, CommentedOnBanditCostume +0x66.
// Shape: the landed CMuteGuardOutsideWoods::Main 0x00D86150 / CWaitingGuardInWoods::Main 0x00D86680 models; each costume
//   test owns five conditional CCharString temporaries (flag word in ebx) and is materialised as a bool before its branch;
//   the full- and part-costume arms share one cross-jumped tail; the security line is a switch with the default arm first.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct CScriptThing {                                  // 12 bytes, vtable 0x01238C8C
    virtual void s000();
    void* PImp_Data;                                   // +0x04
    void* PImp_Info;                                   // +0x08
    CScriptThing(const CScriptThing& other);           // 0x004ABE90
    ~CScriptThing();                                   // 0x004AA840
};
struct CBaseObject {                                   // CBaseIntelligentPointer root
    virtual void b00();
    void* m_pIntelligentPointer;                       // +0x04
    CBaseObject();                                     // 0x0099A380
};
struct CCountedPointerWords {
    void* Data;
    void* Info;
    CCountedPointerWords() : Data(0), Info(0) { }
};
struct CScriptGameResourceObjectScriptedThingBase : CBaseObject {   // vtable 0x0127094C, 0x10 bytes
    CCountedPointerWords PImp;                         // +0x08 Data, +0x0c Info
    CScriptGameResourceObjectScriptedThingBase() : CBaseObject(), PImp() { }
    ~CScriptGameResourceObjectScriptedThingBase();     // 0x007E74D0
    bool IsScripting();                                // 0x00CD23B9
    void Release();                                    // 0x00CD2770
};
typedef long EScriptAIPriority;
struct GameCalls {                                     // CGameScriptInterface vtable 0x01260F0C (ghidra_out/fable_cgsi_slots.h)
    virtual void g000(); virtual void g001(); virtual void g002(); virtual void g003(); virtual void g004(); virtual void g005(); virtual void g006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c)
    virtual bool StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& object, EScriptAIPriority priority);   // slot 8 (+0x20)
    virtual void g009(); virtual void g010(); virtual void g011(); virtual void g012(); virtual void g013(); virtual void g014(); virtual void g015(); virtual void g016();
    virtual void g017(); virtual void g018(); virtual void g019(); virtual void g020(); virtual void g021(); virtual void g022(); virtual void g023(); virtual void g024();
    virtual void g025(); virtual void g026(); virtual void g027(); virtual void g028(); virtual void g029(); virtual void g030(); virtual void g031(); virtual void g032();
    virtual void g033(); virtual void g034(); virtual void g035(); virtual void g036(); virtual void g037(); virtual void g038(); virtual void g039(); virtual void g040();
    virtual void g041(); virtual void g042(); virtual void g043(); virtual void g044(); virtual void g045(); virtual void g046(); virtual void g047(); virtual void g048();
    virtual void g049(); virtual void g050(); virtual void g051(); virtual void g052(); virtual void g053(); virtual void g054(); virtual void g055(); virtual void g056();
    virtual void g057(); virtual void g058(); virtual void g059(); virtual void g060(); virtual void g061(); virtual void g062(); virtual void g063(); virtual void g064();
    virtual void g065(); virtual void g066(); virtual void g067(); virtual void g068(); virtual void g069();
    virtual CScriptThing& GetHero();   // slot 70 (+0x118)
    virtual void g071(); virtual void g072(); virtual void g073(); virtual void g074(); virtual void g075(); virtual void g076(); virtual void g077(); virtual void g078();
    virtual void g079(); virtual void g080(); virtual void g081(); virtual void g082(); virtual void g083(); virtual void g084(); virtual void g085(); virtual void g086();
    virtual void g087(); virtual void g088(); virtual void g089(); virtual void g090(); virtual void g091(); virtual void g092(); virtual void g093(); virtual void g094();
    virtual void g095(); virtual void g096(); virtual void g097(); virtual void g098(); virtual void g099(); virtual void g100(); virtual void g101(); virtual void g102();
    virtual void g103(); virtual void g104(); virtual void g105(); virtual void g106(); virtual void g107(); virtual void g108(); virtual void g109(); virtual void g110();
    virtual void g111(); virtual void g112(); virtual void g113(); virtual void g114(); virtual void g115(); virtual void g116(); virtual void g117(); virtual void g118();
    virtual void g119(); virtual void g120(); virtual void g121(); virtual void g122(); virtual void g123(); virtual void g124(); virtual void g125(); virtual void g126();
    virtual void g127(); virtual void g128(); virtual void g129(); virtual void g130(); virtual void g131(); virtual void g132(); virtual void g133(); virtual void g134();
    virtual void g135(); virtual void g136(); virtual void g137(); virtual void g138(); virtual void g139(); virtual void g140(); virtual void g141(); virtual void g142();
    virtual void g143(); virtual void g144(); virtual void g145(); virtual void g146(); virtual void g147(); virtual void g148(); virtual void g149(); virtual void g150();
    virtual void g151(); virtual void g152(); virtual void g153(); virtual void g154(); virtual void g155(); virtual void g156(); virtual void g157(); virtual void g158();
    virtual void g159(); virtual void g160(); virtual void g161(); virtual void g162(); virtual void g163(); virtual void g164(); virtual void g165(); virtual void g166();
    virtual void g167(); virtual void g168(); virtual void g169(); virtual void g170(); virtual void g171(); virtual void g172(); virtual void g173(); virtual void g174();
    virtual void g175(); virtual void g176(); virtual void g177(); virtual void g178(); virtual void g179(); virtual void g180(); virtual void g181(); virtual void g182();
    virtual void g183(); virtual void g184(); virtual void g185(); virtual void g186();
    virtual bool IsWearingClothingItem(const CScriptThing& thing, const CCharString& item);   // slot 187 (+0x2ec)
    virtual void g188(); virtual void g189(); virtual void g190(); virtual void g191(); virtual void g192(); virtual void g193(); virtual void g194(); virtual void g195();
    virtual void g196(); virtual void g197(); virtual void g198(); virtual void g199(); virtual void g200(); virtual void g201(); virtual void g202(); virtual void g203();
    virtual void g204(); virtual void g205(); virtual void g206(); virtual void g207(); virtual void g208(); virtual void g209(); virtual void g210(); virtual void g211();
    virtual void g212(); virtual void g213(); virtual void g214(); virtual void g215(); virtual void g216(); virtual void g217(); virtual void g218(); virtual void g219();
    virtual void g220(); virtual void g221(); virtual void g222(); virtual void g223(); virtual void g224(); virtual void g225(); virtual void g226(); virtual void g227();
    virtual void g228(); virtual void g229(); virtual void g230(); virtual void g231(); virtual void g232(); virtual void g233(); virtual void g234(); virtual void g235();
    virtual void g236(); virtual void g237(); virtual void g238(); virtual void g239(); virtual void g240(); virtual void g241(); virtual void g242(); virtual void g243();
    virtual void g244(); virtual void g245(); virtual void g246(); virtual void g247(); virtual void g248(); virtual void g249(); virtual void g250(); virtual void g251();
    virtual void g252(); virtual void g253(); virtual void g254(); virtual void g255(); virtual void g256(); virtual void g257(); virtual void g258(); virtual void g259();
    virtual void g260(); virtual void g261(); virtual void g262(); virtual void g263(); virtual void g264(); virtual void g265(); virtual void g266(); virtual void g267();
    virtual void g268(); virtual void g269(); virtual void g270(); virtual void g271(); virtual void g272(); virtual void g273(); virtual void g274(); virtual void g275();
    virtual void g276(); virtual void g277(); virtual void g278(); virtual void g279(); virtual void g280(); virtual void g281(); virtual void g282(); virtual void g283();
    virtual void g284(); virtual void g285(); virtual void g286(); virtual void g287(); virtual void g288(); virtual void g289(); virtual void g290(); virtual void g291();
    virtual void g292(); virtual void g293(); virtual void g294(); virtual void g295(); virtual void g296(); virtual void g297(); virtual void g298(); virtual void g299();
    virtual void g300(); virtual void g301(); virtual void g302(); virtual void g303(); virtual void g304(); virtual void g305(); virtual void g306(); virtual void g307();
    virtual void g308(); virtual void g309(); virtual void g310(); virtual void g311(); virtual void g312(); virtual void g313(); virtual void g314(); virtual void g315();
    virtual void g316(); virtual void g317(); virtual void g318(); virtual void g319(); virtual void g320(); virtual void g321(); virtual void g322(); virtual void g323();
    virtual void g324(); virtual void g325(); virtual void g326(); virtual void g327(); virtual void g328(); virtual void g329(); virtual void g330(); virtual void g331();
    virtual void g332(); virtual void g333(); virtual void g334(); virtual void g335(); virtual void g336(); virtual void g337(); virtual void g338(); virtual void g339();
    virtual void g340(); virtual void g341(); virtual void g342(); virtual void g343(); virtual void g344(); virtual void g345(); virtual void g346(); virtual void g347();
    virtual void g348(); virtual void g349(); virtual void g350(); virtual void g351(); virtual void g352(); virtual void g353(); virtual void g354(); virtual void g355();
    virtual void g356(); virtual void g357(); virtual void g358(); virtual void g359(); virtual void g360(); virtual void g361(); virtual void g362(); virtual void g363();
    virtual long AddNewConversation(const CScriptThing& thing, bool a, bool b) const;   // slot 364 (+0x5b0)
    virtual void AddPersonToConversation(long conversation, const CScriptThing& person) const;   // slot 365 (+0x5b4)
    virtual void AddLineToConversation(long conversation, const CCharString& text, bool a, const CScriptThing& speaker, const CScriptThing& listener) const;   // slot 366 (+0x5b8)
};
struct CIsThingAlive {                                 // vtable 0x012C32F0: a predicate holding a script thing
    virtual void v00();
    CScriptThing m_thing;                              // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    ~CIsThingAlive() { }
};
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF

namespace NScript {
struct CEntityScriptBase {                             // retail 0x14 bytes
    virtual void v00();
    GameCalls*   Game;                                 // +0x04
    CScriptThing Me;                                   // +0x08
    void SetPredicate(CIsThingAlive& predicate);       // 0x00F35B10
    bool IsActiveThreadTerminating() const;            // 0x00F35B30
};
struct CQ_SunnyvaleMasterData;
struct CQ_TraderConflictGoodScript {
    unsigned char _pad[0x54];
    long TradersFollowing;                             // +0x54
    long TradersReachedTeleporter;                     // +0x58
    long BanditSecurityLinesSaid;                      // +0x5c
    bool FirstRegionDead;                              // +0x60
    bool EnteredNewRegion;                             // +0x61
    bool IntroDone;                                    // +0x62
    bool OutroStart;                                   // +0x63
    bool OutroDone;                                    // +0x64
    bool OpenedCage;                                   // +0x65
    bool CommentedOnBanditCostume;                     // +0x66
    bool MissionSucceeded;                             // +0x67
    class CTC_BanditGuard;
};
class CQ_TraderConflictGoodScript::CTC_BanditGuard : public CEntityScriptBase {
public:
    CQ_TraderConflictGoodScript* ParentClass;          // +0x14
    CQ_SunnyvaleMasterData*      PMasterData;          // +0x18
    void Main();
};
}

void NScript::CQ_TraderConflictGoodScript::CTC_BanditGuard::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    CScriptGameResourceObjectScriptedThingBase scripted;
    if (scripted.IsScripting())
        scripted.Release();
    while (!Game->StartScriptingEntity(Me, scripted, 2)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    while (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 15.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    bool fullCostume = !ParentClass->CommentedOnBanditCostume
                    && Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_SHIRT_BANDITCAMP")
                    && Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_TROUSERS_BANDITCAMP")
                    && Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_HAT_BANDITCAMP")
                    && Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_BOOTS_BANDITCAMP")
                    && Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_GLOVES_BANDITCAMP");
    if (fullCostume) {
        if (IsActiveThreadTerminating()) return;
        long conversation = Game->AddNewConversation(Me, false, false);
        Game->AddPersonToConversation(conversation, Game->GetHero());
        Game->AddLineToConversation(conversation, "TEXT_QST_B11_BANDIT_ATTACK_WEARING_BANDIT_COSTUME_10", false, Me, Game->GetHero());
        ParentClass->CommentedOnBanditCostume = true;
    }
    else {
        bool partCostume = !ParentClass->CommentedOnBanditCostume
                        && (Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_SHIRT_BANDITCAMP")
                            || Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_TROUSERS_BANDITCAMP")
                            || Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_HAT_BANDITCAMP")
                            || Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_BOOTS_BANDITCAMP")
                            || Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_GLOVES_BANDITCAMP"));
        if (partCostume) {
            if (IsActiveThreadTerminating()) return;
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            Game->AddLineToConversation(conversation, "TEXT_QST_B11_BANDIT_ATTACK_WEARING_PART_BANDIT_COSTUME_10", false, Me, Game->GetHero());
            ParentClass->CommentedOnBanditCostume = true;
        }
        else {
            if (IsActiveThreadTerminating()) return;
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            switch (ParentClass->BanditSecurityLinesSaid) {
            case 0:
                Game->AddLineToConversation(conversation, "TEXT_QST_B11_BANDIT_SECURITY_ATTACK_HERO_10", false, Me, Game->GetHero());
                break;
            case 1:
                Game->AddLineToConversation(conversation, "TEXT_QST_B11_BANDIT_SECURITY_ATTACK_HERO_20", false, Me, Game->GetHero());
                break;
            default:
                Game->AddLineToConversation(conversation, "TEXT_QST_B11_BANDIT_SECURITY_ATTACK_HERO_20", false, Me, Game->GetHero());
                break;
            }
            if (ParentClass->BanditSecurityLinesSaid < 1)
                ++ParentClass->BanditSecurityLinesSaid;
            else
                ParentClass->BanditSecurityLinesSaid = 0;
        }
    }
    if (scripted.IsScripting())
        scripted.Release();
    do {
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}