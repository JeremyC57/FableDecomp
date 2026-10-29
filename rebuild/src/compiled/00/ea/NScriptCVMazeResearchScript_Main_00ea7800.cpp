// NScript::CV_MazeResearchScript::Main() -- the Maze-research quest main: bind the EmptyGrave and HistoryBookcase entity scripts, post-add the scripted entities, then set the quest card objective (TEXT_QUEST_MAZE_RESEARCH_OBJECTIVE_01, HeroGuildComplexInside)
// Leaf: ?Main@CV_MazeResearchScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EA7800, 374 bytes, thiscall, plain `ret`.
// Identity (proven): slot 2 (Main) of the quest-script vtable 0x012EE444 [dtor 0x00EA86C0, RegisterMain 0x00EA7760 (landed
//   NScript::CV_MazeResearchScript::RegisterMain, which spawns slot 2 as "Main"), Main 0x00EA7800, Init 0x00EA7750,
//   OnPersist 0x00EA80A0]; the only reference to the row is that slot. Ego_r.pdb ?Main@CV_MazeResearchScript@NScript@@UAEXXZ
//   (donor 0x00F117B0, after RegisterMain 0x00F11680 as here). The two binding factories are the landed CEmptyGrave::Alloc
//   0x00EA7990 and CHistoryBookcase::Alloc 0x00EA7A60 (Ego_r.pdb Alloc statics of the class). Catalog label: sub_00ea7800.
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0,
//   CScriptBase::AddEntityScriptBinding 0x00CB8230 / PostAddScriptedEntities 0x00CB8930; CEntityScriptBinding<T> vtable
//   0x012EE4AC; CGameScriptInterface slots 64 PostAddScriptedEntities, 296 SetQuestCardObjective, 655 GetActiveQuestName
//   (ghidra_out/cgsi_slots_aligned.tsv).
// Literals: "EmptyGrave" 0x012EE4B4, "HistoryBookcase" 0x012EE49C, "" 0x0122D70E, "HeroGuildComplexInside" 0x01238FCC,
//   "TEXT_QUEST_MAZE_RESEARCH_OBJECTIVE_01" 0x012EE474.
// Layout: CScriptBase retail 0x40 bytes (polymorphic), Game +0x40; CEntityScriptBinding 0x1c bytes (Flags 0).
// Model: the landed NScript::CQS_MeetSisterScript::Main 0x00E28DE0 (this chunk) / CV_IntroductionToTrophiesScript::Main 0x00E9C4B0.
// Shape: every new-expression's name string is a conditional temporary with its own ebx flag bit; the SetQuestCardObjective
//   strings are built right to left, the GetActiveQuestName result last.
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
void* __cdecl operator new(unsigned int bytes);         // 0x00BFEA1A (MSVCR71)
class CScriptThing;
class CScriptDataBase;
class CEntityScriptBase;
template <class T> class CCountedPointer {
public:
    T* Data;
    void* Info;
};
class __declspec(novtable) CEntityScriptBindingBase {
public:
    virtual const CCharString& GetEntityScriptName() const = 0;
};
template <class T> class CEntityScriptBinding : public CEntityScriptBindingBase {   // 0x1c bytes
public:
    typedef CCountedPointer<CEntityScriptBase> (__fastcall *TAllocFunc)(T& parent, CScriptDataBase* data, const CScriptThing& thing);
    CCharString     EntityScriptName;                   // +0x04
    T*              PParentScript;                      // +0x08
    void (T::*Func)();                                  // +0x0c
    TAllocFunc      ClassAllocFunc;                     // +0x10
    bool            ClassAllocValid;                    // +0x14
    unsigned long   Flags;                              // +0x18
    CEntityScriptBinding(const CCharString& name, T* parent, TAllocFunc alloc, unsigned long flags)
        : EntityScriptName(name), PParentScript(parent), ClassAllocFunc(alloc), ClassAllocValid(true), Flags(flags) { }
    virtual const CCharString& GetEntityScriptName() const { return EntityScriptName; }
};
class CGameScriptInterfaceBase {
public:          // slots: ghidra_out/cgsi_slots_aligned.tsv (CGameScriptInterface vtable 0x01260F0C)
    virtual void g000(); virtual void g001(); virtual void g002(); virtual void g003(); virtual void g004(); virtual void g005(); virtual void g006(); virtual void g007(); virtual void g008(); virtual void g009();
    virtual void g010(); virtual void g011(); virtual void g012(); virtual void g013(); virtual void g014(); virtual void g015(); virtual void g016(); virtual void g017(); virtual void g018(); virtual void g019();
    virtual void g020(); virtual void g021(); virtual void g022(); virtual void g023(); virtual void g024(); virtual void g025(); virtual void g026(); virtual void g027(); virtual void g028(); virtual void g029();
    virtual void g030(); virtual void g031(); virtual void g032(); virtual void g033(); virtual void g034(); virtual void g035(); virtual void g036(); virtual void g037(); virtual void g038(); virtual void g039();
    virtual void g040(); virtual void g041(); virtual void g042(); virtual void g043(); virtual void g044(); virtual void g045(); virtual void g046(); virtual void g047(); virtual void g048(); virtual void g049();
    virtual void g050(); virtual void g051(); virtual void g052(); virtual void g053(); virtual void g054(); virtual void g055(); virtual void g056(); virtual void g057(); virtual void g058(); virtual void g059();
    virtual void g060(); virtual void g061(); virtual void g062(); virtual void g063();
    virtual void PostAddScriptedEntities() const;   // slot 64 (+0x100)
    virtual void g065(); virtual void g066(); virtual void g067(); virtual void g068(); virtual void g069(); virtual void g070(); virtual void g071(); virtual void g072(); virtual void g073(); virtual void g074();
    virtual void g075(); virtual void g076(); virtual void g077(); virtual void g078(); virtual void g079(); virtual void g080(); virtual void g081(); virtual void g082(); virtual void g083(); virtual void g084();
    virtual void g085(); virtual void g086(); virtual void g087(); virtual void g088(); virtual void g089(); virtual void g090(); virtual void g091(); virtual void g092(); virtual void g093(); virtual void g094();
    virtual void g095(); virtual void g096(); virtual void g097(); virtual void g098(); virtual void g099(); virtual void g100(); virtual void g101(); virtual void g102(); virtual void g103(); virtual void g104();
    virtual void g105(); virtual void g106(); virtual void g107(); virtual void g108(); virtual void g109(); virtual void g110(); virtual void g111(); virtual void g112(); virtual void g113(); virtual void g114();
    virtual void g115(); virtual void g116(); virtual void g117(); virtual void g118(); virtual void g119(); virtual void g120(); virtual void g121(); virtual void g122(); virtual void g123(); virtual void g124();
    virtual void g125(); virtual void g126(); virtual void g127(); virtual void g128(); virtual void g129(); virtual void g130(); virtual void g131(); virtual void g132(); virtual void g133(); virtual void g134();
    virtual void g135(); virtual void g136(); virtual void g137(); virtual void g138(); virtual void g139(); virtual void g140(); virtual void g141(); virtual void g142(); virtual void g143(); virtual void g144();
    virtual void g145(); virtual void g146(); virtual void g147(); virtual void g148(); virtual void g149(); virtual void g150(); virtual void g151(); virtual void g152(); virtual void g153(); virtual void g154();
    virtual void g155(); virtual void g156(); virtual void g157(); virtual void g158(); virtual void g159(); virtual void g160(); virtual void g161(); virtual void g162(); virtual void g163(); virtual void g164();
    virtual void g165(); virtual void g166(); virtual void g167(); virtual void g168(); virtual void g169(); virtual void g170(); virtual void g171(); virtual void g172(); virtual void g173(); virtual void g174();
    virtual void g175(); virtual void g176(); virtual void g177(); virtual void g178(); virtual void g179(); virtual void g180(); virtual void g181(); virtual void g182(); virtual void g183(); virtual void g184();
    virtual void g185(); virtual void g186(); virtual void g187(); virtual void g188(); virtual void g189(); virtual void g190(); virtual void g191(); virtual void g192(); virtual void g193(); virtual void g194();
    virtual void g195(); virtual void g196(); virtual void g197(); virtual void g198(); virtual void g199(); virtual void g200(); virtual void g201(); virtual void g202(); virtual void g203(); virtual void g204();
    virtual void g205(); virtual void g206(); virtual void g207(); virtual void g208(); virtual void g209(); virtual void g210(); virtual void g211(); virtual void g212(); virtual void g213(); virtual void g214();
    virtual void g215(); virtual void g216(); virtual void g217(); virtual void g218(); virtual void g219(); virtual void g220(); virtual void g221(); virtual void g222(); virtual void g223(); virtual void g224();
    virtual void g225(); virtual void g226(); virtual void g227(); virtual void g228(); virtual void g229(); virtual void g230(); virtual void g231(); virtual void g232(); virtual void g233(); virtual void g234();
    virtual void g235(); virtual void g236(); virtual void g237(); virtual void g238(); virtual void g239(); virtual void g240(); virtual void g241(); virtual void g242(); virtual void g243(); virtual void g244();
    virtual void g245(); virtual void g246(); virtual void g247(); virtual void g248(); virtual void g249(); virtual void g250(); virtual void g251(); virtual void g252(); virtual void g253(); virtual void g254();
    virtual void g255(); virtual void g256(); virtual void g257(); virtual void g258(); virtual void g259(); virtual void g260(); virtual void g261(); virtual void g262(); virtual void g263(); virtual void g264();
    virtual void g265(); virtual void g266(); virtual void g267(); virtual void g268(); virtual void g269(); virtual void g270(); virtual void g271(); virtual void g272(); virtual void g273(); virtual void g274();
    virtual void g275(); virtual void g276(); virtual void g277(); virtual void g278(); virtual void g279(); virtual void g280(); virtual void g281(); virtual void g282(); virtual void g283(); virtual void g284();
    virtual void g285(); virtual void g286(); virtual void g287(); virtual void g288(); virtual void g289(); virtual void g290(); virtual void g291(); virtual void g292(); virtual void g293(); virtual void g294();
    virtual void g295();
    virtual void SetQuestCardObjective(const CCharString& quest, const CCharString& objective, const CCharString& region1, const CCharString& region2) const;   // slot 296 (+0x4a0)
    virtual void g297(); virtual void g298(); virtual void g299(); virtual void g300(); virtual void g301(); virtual void g302(); virtual void g303(); virtual void g304(); virtual void g305(); virtual void g306();
    virtual void g307(); virtual void g308(); virtual void g309(); virtual void g310(); virtual void g311(); virtual void g312(); virtual void g313(); virtual void g314(); virtual void g315(); virtual void g316();
    virtual void g317(); virtual void g318(); virtual void g319(); virtual void g320(); virtual void g321(); virtual void g322(); virtual void g323(); virtual void g324(); virtual void g325(); virtual void g326();
    virtual void g327(); virtual void g328(); virtual void g329(); virtual void g330(); virtual void g331(); virtual void g332(); virtual void g333(); virtual void g334(); virtual void g335(); virtual void g336();
    virtual void g337(); virtual void g338(); virtual void g339(); virtual void g340(); virtual void g341(); virtual void g342(); virtual void g343(); virtual void g344(); virtual void g345(); virtual void g346();
    virtual void g347(); virtual void g348(); virtual void g349(); virtual void g350(); virtual void g351(); virtual void g352(); virtual void g353(); virtual void g354(); virtual void g355(); virtual void g356();
    virtual void g357(); virtual void g358(); virtual void g359(); virtual void g360(); virtual void g361(); virtual void g362(); virtual void g363(); virtual void g364(); virtual void g365(); virtual void g366();
    virtual void g367(); virtual void g368(); virtual void g369(); virtual void g370(); virtual void g371(); virtual void g372(); virtual void g373(); virtual void g374(); virtual void g375(); virtual void g376();
    virtual void g377(); virtual void g378(); virtual void g379(); virtual void g380(); virtual void g381(); virtual void g382(); virtual void g383(); virtual void g384(); virtual void g385(); virtual void g386();
    virtual void g387(); virtual void g388(); virtual void g389(); virtual void g390(); virtual void g391(); virtual void g392(); virtual void g393(); virtual void g394(); virtual void g395(); virtual void g396();
    virtual void g397(); virtual void g398(); virtual void g399(); virtual void g400(); virtual void g401(); virtual void g402(); virtual void g403(); virtual void g404(); virtual void g405(); virtual void g406();
    virtual void g407(); virtual void g408(); virtual void g409(); virtual void g410(); virtual void g411(); virtual void g412(); virtual void g413(); virtual void g414(); virtual void g415(); virtual void g416();
    virtual void g417(); virtual void g418(); virtual void g419(); virtual void g420(); virtual void g421(); virtual void g422(); virtual void g423(); virtual void g424(); virtual void g425(); virtual void g426();
    virtual void g427(); virtual void g428(); virtual void g429(); virtual void g430(); virtual void g431(); virtual void g432(); virtual void g433(); virtual void g434(); virtual void g435(); virtual void g436();
    virtual void g437(); virtual void g438(); virtual void g439(); virtual void g440(); virtual void g441(); virtual void g442(); virtual void g443(); virtual void g444(); virtual void g445(); virtual void g446();
    virtual void g447(); virtual void g448(); virtual void g449(); virtual void g450(); virtual void g451(); virtual void g452(); virtual void g453(); virtual void g454(); virtual void g455(); virtual void g456();
    virtual void g457(); virtual void g458(); virtual void g459(); virtual void g460(); virtual void g461(); virtual void g462(); virtual void g463(); virtual void g464(); virtual void g465(); virtual void g466();
    virtual void g467(); virtual void g468(); virtual void g469(); virtual void g470(); virtual void g471(); virtual void g472(); virtual void g473(); virtual void g474(); virtual void g475(); virtual void g476();
    virtual void g477(); virtual void g478(); virtual void g479(); virtual void g480(); virtual void g481(); virtual void g482(); virtual void g483(); virtual void g484(); virtual void g485(); virtual void g486();
    virtual void g487(); virtual void g488(); virtual void g489(); virtual void g490(); virtual void g491(); virtual void g492(); virtual void g493(); virtual void g494(); virtual void g495(); virtual void g496();
    virtual void g497(); virtual void g498(); virtual void g499(); virtual void g500(); virtual void g501(); virtual void g502(); virtual void g503(); virtual void g504(); virtual void g505(); virtual void g506();
    virtual void g507(); virtual void g508(); virtual void g509(); virtual void g510(); virtual void g511(); virtual void g512(); virtual void g513(); virtual void g514(); virtual void g515(); virtual void g516();
    virtual void g517(); virtual void g518(); virtual void g519(); virtual void g520(); virtual void g521(); virtual void g522(); virtual void g523(); virtual void g524(); virtual void g525(); virtual void g526();
    virtual void g527(); virtual void g528(); virtual void g529(); virtual void g530(); virtual void g531(); virtual void g532(); virtual void g533(); virtual void g534(); virtual void g535(); virtual void g536();
    virtual void g537(); virtual void g538(); virtual void g539(); virtual void g540(); virtual void g541(); virtual void g542(); virtual void g543(); virtual void g544(); virtual void g545(); virtual void g546();
    virtual void g547(); virtual void g548(); virtual void g549(); virtual void g550(); virtual void g551(); virtual void g552(); virtual void g553(); virtual void g554(); virtual void g555(); virtual void g556();
    virtual void g557(); virtual void g558(); virtual void g559(); virtual void g560(); virtual void g561(); virtual void g562(); virtual void g563(); virtual void g564(); virtual void g565(); virtual void g566();
    virtual void g567(); virtual void g568(); virtual void g569(); virtual void g570(); virtual void g571(); virtual void g572(); virtual void g573(); virtual void g574(); virtual void g575(); virtual void g576();
    virtual void g577(); virtual void g578(); virtual void g579(); virtual void g580(); virtual void g581(); virtual void g582(); virtual void g583(); virtual void g584(); virtual void g585(); virtual void g586();
    virtual void g587(); virtual void g588(); virtual void g589(); virtual void g590(); virtual void g591(); virtual void g592(); virtual void g593(); virtual void g594(); virtual void g595(); virtual void g596();
    virtual void g597(); virtual void g598(); virtual void g599(); virtual void g600(); virtual void g601(); virtual void g602(); virtual void g603(); virtual void g604(); virtual void g605(); virtual void g606();
    virtual void g607(); virtual void g608(); virtual void g609(); virtual void g610(); virtual void g611(); virtual void g612(); virtual void g613(); virtual void g614(); virtual void g615(); virtual void g616();
    virtual void g617(); virtual void g618(); virtual void g619(); virtual void g620(); virtual void g621(); virtual void g622(); virtual void g623(); virtual void g624(); virtual void g625(); virtual void g626();
    virtual void g627(); virtual void g628(); virtual void g629(); virtual void g630(); virtual void g631(); virtual void g632(); virtual void g633(); virtual void g634(); virtual void g635(); virtual void g636();
    virtual void g637(); virtual void g638(); virtual void g639(); virtual void g640(); virtual void g641(); virtual void g642(); virtual void g643(); virtual void g644(); virtual void g645(); virtual void g646();
    virtual void g647(); virtual void g648(); virtual void g649(); virtual void g650(); virtual void g651(); virtual void g652(); virtual void g653(); virtual void g654();
    virtual CCharString GetActiveQuestName() const;   // slot 655 (+0xa3c)
};
class CGameScriptInterface : public CGameScriptInterfaceBase {
public:
};
class CScriptBase {                                     // retail 0x40 bytes
public:
    virtual ~CScriptBase();
    virtual void RegisterMain();                        // slot 1
    virtual void Main() = 0;                            // slot 2
    virtual void Init();                                // slot 3
    unsigned char _body[0x3c];                          // +0x04 .. +0x40
    void AddEntityScriptBinding(CEntityScriptBindingBase* binding);                   // 0x00CB8230
    void PostAddScriptedEntities();                                                   // 0x00CB8930
};
namespace NScript {
class CV_MazeResearchScript : public CScriptBase {
public:
    class CEmptyGrave {
    public:
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_MazeResearchScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00EA7990
    };
    class CHistoryBookcase {
    public:
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_MazeResearchScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00EA7A60
    };
    CGameScriptInterface* Game;                         // +0x40
    virtual void Main();
};

void CV_MazeResearchScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CV_MazeResearchScript>("EmptyGrave", this, &CEmptyGrave::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_MazeResearchScript>("HistoryBookcase", this, &CHistoryBookcase::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_MAZE_RESEARCH_OBJECTIVE_01", "HeroGuildComplexInside", "");
}
}   // namespace NScript