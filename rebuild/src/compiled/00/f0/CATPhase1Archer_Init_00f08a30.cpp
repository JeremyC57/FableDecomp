// CAT_Phase1Archer::Init() -- make the archer and the hero enemies of each other, then every living bandit in the
// parent's AllBandits (stopping if the entity script is done), make the archer aware of the hero and register the
// parent's WatchForEnemyDeath callback over a copy of the archer's script thing
// Leaf: Init
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F08A30, length 507 bytes.
// Callees: CGameScriptInterface (+4) virtual slots 70 GetHero (+0x118), 597 EntitySetThingAsEnemyOfThing (+0x954),
//          637 EntitySetAsAwareOfThing (+0x9f4) (names from ghidra_out/cgsi_slots_aligned.tsv); CScriptThing::IsAlive
//          slot 75 (+0x12c, 0x004AB130); IsDone @ 0x00F35B30 (manifest "CCreatureAction_TrollWhackGroundBase::
//          Initialise" is wrong); operator new @ 0x00BFEA1A (0x48), CCharString @ 0x0099EBF0 / 0x0099EAE0,
//          operator+ @ 0x0099F570, CSpawnedFuncBase ctor @ 0x00CDD450, CScriptThing copy @ 0x004ABE90 / dtor
//          @ 0x004AA840, CQ_AmbushTradersScript::Add @ 0x00CB7E50. Strings "ParentClass." 0x012C9934,
//          "WatchForEnemyDeath" 0x012F8114, "" 0x0122D70E. Spawned-func vtable 0x012F7EBC, callback 0x00F08590.
// Layout: as CAT_BanditContact::Init (0x00F09640) plus CQ_AmbushTradersScript::AllBandits at +0xa8 (Ego_r.pdb +0xbc
//         - 0x14 for the retail 0x48-byte quest base, as the landed SetBanditAllegiences 0x00F060D0), STLport 12-byte
//         vector.
// Shape: the loop is the landed SetBanditAllegiences loop (size() re-tested, `jb`). The CScriptThing handle's counted
//         pair is a member struct with its own copy ctor (as the landed CAttackingBandit::Init 0x00E1F3E0): with the
//         flat PImp_Data/PImp_Info copy, VC keeps &Thing in edi across the loop and swaps edi/ebp in the Add tail.
#include <new>
#include "fable_string.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CScriptThing.h"
#include "engine/CASuspendableProcess.h"

namespace NScript { class CPredicateBase; }
class CCPPointerInfo { public: long References; };
class C3DVector { public: float X, Y, Z; };

struct CScriptThingRuntime {
    virtual ~CScriptThingRuntime(); // CScriptThing RTTI slot 0, 0x004AB170
    struct CCounted { CScriptThingRuntime* Data; CCPPointerInfo* Info;
        CCounted() : Data(0), Info(0) {}
        CCounted(const CCounted& other) { Data = other.Data; Info = other.Info; if (Info) ++Info->References; } } PImp;

    virtual const CCharString& GetName() const;       // slot 1, 0x004AA8B0
    virtual CCharString GetDefName() const;           // slot 2, 0x004AA8D0
    virtual CCharString GetDataString() const;        // slot 3, 0x004AA900
    virtual void SetDataString(const CCharString&);   // slot 4, 0x004AA930
    virtual C3DVector GetFocalPos() const;            // slot 5, 0x004AA940
    virtual const C3DVector& GetPos() const;          // slot 6, 0x004AA980
    virtual void s07();
    virtual void s08();
    virtual void s09();
    virtual void s10();
    virtual void s11();
    virtual void s12();
    virtual void s13();
    virtual void s14();
    virtual void s15();
    virtual void s16();
    virtual void s17();
    virtual void s18();
    virtual void s19();
    virtual void s20();
    virtual void s21();
    virtual void s22();
    virtual void s23();
    virtual void s24();
    virtual void s25();
    virtual void s26();
    virtual void s27();
    virtual void s28();
    virtual void s29();
    virtual void s30();
    virtual void s31();
    virtual void s32();
    virtual void s33();
    virtual void s34();
    virtual void s35();
    virtual void s36();
    virtual void s37();
    virtual void s38();
    virtual void s39();
    virtual void s40();
    virtual void s41();
    virtual void s42();
    virtual void s43();
    virtual void s44();
    virtual void s45();
    virtual void s46();
    virtual void s47();
    virtual void s48();
    virtual void s49();
    virtual void s50();
    virtual void s51();
    virtual void s52();
    virtual void s53();
    virtual void s54();
    virtual void s55();
    virtual void s56();
    virtual void s57();
    virtual void s58();
    virtual void s59();
    virtual void s60();
    virtual void s61();
    virtual void s62();
    virtual void s63();
    virtual void s64();
    virtual void s65();
    virtual void s66();
    virtual void s67();
    virtual void s68();
    virtual void s69();
    virtual void s70();
    virtual void s71();
    virtual void s72();
    virtual void s73();
    virtual void s74();
    virtual bool IsAlive() const;                     // slot 75 (+0x12c), 0x004AB130
    CScriptThingRuntime() {}
    CScriptThingRuntime(const CScriptThingRuntime& other)
        : PImp(other.PImp)
    { }
    CScriptThingRuntime(const struct CScriptThingStored& other);
};
#if defined(_MSC_VER)
FABLE_STATIC_ASSERT(sizeof(CScriptThingRuntime) == sizeof(CScriptThing));
#endif

// Same retail CScriptThing view, using the native out-of-line copy constructor
// for CSpawnedFunc1::Par1; the caller's by-value argument uses the inline copy above.
struct CScriptThingStored {
    virtual ~CScriptThingStored();
    CScriptThingRuntime* PImp_Data;
    CCPPointerInfo* PImp_Info;
    virtual const CCharString& GetName() const;
    virtual CCharString GetDefName() const;
    virtual CCharString GetDataString() const;
    virtual void SetDataString(const CCharString&);
    virtual C3DVector GetFocalPos() const;
    virtual const C3DVector& GetPos() const;
    CScriptThingStored(const CScriptThingRuntime& other); // retail 0x004ABE90
};
inline CScriptThingRuntime::CScriptThingRuntime(const CScriptThingStored& other)
{ PImp.Data = other.PImp_Data; PImp.Info = other.PImp_Info; if (PImp.Info) ++PImp.Info->References; }

struct CScriptProcessBaseRuntime {
    virtual ~CScriptProcessBaseRuntime();
    virtual void RunSuspendableProcess();
    virtual void SuspendSuspendableProcess(bool suspend);
    virtual void Restart();
    virtual void SuspendableProcess() = 0;
    virtual void RuntimeHookCDD410(); // native one-byte ret at 0x00CDD410; original name/signature unresolved
    virtual void RuntimeHookCDD420(); // native one-byte ret at 0x00CDD420; original name/signature unresolved
    bool Running;
    bool StopRunning;
    bool TerminateUsingExceptions;
    unsigned char _pad_07;
    float TimeProcessSliceStarted;
    float TimesliceLength;
    CScriptProcessBaseRuntime(bool running, long stackSize, float fraction); // 0x00A44740
};

struct CSpawnedFuncBaseRuntime : CScriptProcessBaseRuntime {
    virtual ~CSpawnedFuncBaseRuntime(); // ordinary base destructor 0x00CDD4C0 cleans PPredicate
    unsigned long SpawnedFuncBaseState; // +0x10 word in the retail CSpawnedFuncBase subobject
    unsigned long ID;
    long LastScriptFrame;
    long CurrentScriptFrame;
    CCharString Name;
    CCharString AssociatedRegionName;
    bool FuncPaused;
    bool Interrupted;
    unsigned char _pad_2a[2];
    void* PPredicate_Data;
    CCPPointerInfo* PPredicate_Info;
    CSpawnedFuncBaseRuntime(const CCharString& name, unsigned long id); // 0x00CDD450
};

template <class T> struct vector {                     // STLport vector (retail 12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    T& operator[](unsigned int n) { return *(_M_start + n); }
};
class CGameScriptInterface { public:
    virtual void g000();
    virtual void g001();
    virtual void g002();
    virtual void g003();
    virtual void g004();
    virtual void g005();
    virtual void g006();
    virtual void g007();
    virtual void g008();
    virtual void g009();
    virtual void g010();
    virtual void g011();
    virtual void g012();
    virtual void g013();
    virtual void g014();
    virtual void g015();
    virtual void g016();
    virtual void g017();
    virtual void g018();
    virtual void g019();
    virtual void g020();
    virtual void g021();
    virtual void g022();
    virtual void g023();
    virtual void g024();
    virtual void g025();
    virtual void g026();
    virtual void g027();
    virtual void g028();
    virtual void g029();
    virtual void g030();
    virtual void g031();
    virtual void g032();
    virtual void g033();
    virtual void g034();
    virtual void g035();
    virtual void g036();
    virtual void g037();
    virtual void g038();
    virtual void g039();
    virtual void g040();
    virtual void g041();
    virtual void g042();
    virtual void g043();
    virtual void g044();
    virtual void g045();
    virtual void g046();
    virtual void g047();
    virtual void g048();
    virtual void g049();
    virtual void g050();
    virtual void g051();
    virtual void g052();
    virtual void g053();
    virtual void g054();
    virtual void g055();
    virtual void g056();
    virtual void g057();
    virtual void g058();
    virtual void g059();
    virtual void g060();
    virtual void g061();
    virtual void g062();
    virtual void g063();
    virtual void g064();
    virtual void g065();
    virtual void g066();
    virtual void g067();
    virtual void g068();
    virtual void g069();
    virtual CScriptThingRuntime& GetHero();                     // slot 70 (+0x118) 0x00891CA0
    virtual void g071();
    virtual void g072();
    virtual void g073();
    virtual void g074();
    virtual void g075();
    virtual void g076();
    virtual void g077();
    virtual void g078();
    virtual void g079();
    virtual void g080();
    virtual void g081();
    virtual void g082();
    virtual void g083();
    virtual void g084();
    virtual void g085();
    virtual void g086();
    virtual void g087();
    virtual void g088();
    virtual void g089();
    virtual void g090();
    virtual void g091();
    virtual void g092();
    virtual void g093();
    virtual void g094();
    virtual void g095();
    virtual void g096();
    virtual void g097();
    virtual void g098();
    virtual void g099();
    virtual void g100();
    virtual void g101();
    virtual void g102();
    virtual void g103();
    virtual void g104();
    virtual void g105();
    virtual void g106();
    virtual void g107();
    virtual void g108();
    virtual void g109();
    virtual void g110();
    virtual void g111();
    virtual void g112();
    virtual void g113();
    virtual void g114();
    virtual void g115();
    virtual void g116();
    virtual void g117();
    virtual void g118();
    virtual void g119();
    virtual void g120();
    virtual void g121();
    virtual void g122();
    virtual void g123();
    virtual void g124();
    virtual void g125();
    virtual void g126();
    virtual void g127();
    virtual void g128();
    virtual void g129();
    virtual void g130();
    virtual void g131();
    virtual void g132();
    virtual void g133();
    virtual void g134();
    virtual void g135();
    virtual void g136();
    virtual void g137();
    virtual void g138();
    virtual void g139();
    virtual void g140();
    virtual void g141();
    virtual void g142();
    virtual void g143();
    virtual void g144();
    virtual void g145();
    virtual void g146();
    virtual void g147();
    virtual void g148();
    virtual void g149();
    virtual void g150();
    virtual void g151();
    virtual void g152();
    virtual void g153();
    virtual void g154();
    virtual void g155();
    virtual void g156();
    virtual void g157();
    virtual void g158();
    virtual void g159();
    virtual void g160();
    virtual void g161();
    virtual void g162();
    virtual void g163();
    virtual void g164();
    virtual void g165();
    virtual void g166();
    virtual void g167();
    virtual void g168();
    virtual void g169();
    virtual void g170();
    virtual void g171();
    virtual void g172();
    virtual void g173();
    virtual void g174();
    virtual void g175();
    virtual void g176();
    virtual void g177();
    virtual void g178();
    virtual void g179();
    virtual void g180();
    virtual void g181();
    virtual void g182();
    virtual void g183();
    virtual void g184();
    virtual void g185();
    virtual void g186();
    virtual void g187();
    virtual void g188();
    virtual void g189();
    virtual void g190();
    virtual void g191();
    virtual void g192();
    virtual void g193();
    virtual void g194();
    virtual void g195();
    virtual void g196();
    virtual void g197();
    virtual void g198();
    virtual void g199();
    virtual void g200();
    virtual void g201();
    virtual void g202();
    virtual void g203();
    virtual void g204();
    virtual void g205();
    virtual void g206();
    virtual void g207();
    virtual void g208();
    virtual void g209();
    virtual void g210();
    virtual void g211();
    virtual void g212();
    virtual void g213();
    virtual void g214();
    virtual void g215();
    virtual void g216();
    virtual void g217();
    virtual void g218();
    virtual void g219();
    virtual void g220();
    virtual void g221();
    virtual void g222();
    virtual void g223();
    virtual void g224();
    virtual void g225();
    virtual void g226();
    virtual void g227();
    virtual void g228();
    virtual void g229();
    virtual void g230();
    virtual void g231();
    virtual void g232();
    virtual void g233();
    virtual void g234();
    virtual void g235();
    virtual void g236();
    virtual void g237();
    virtual void g238();
    virtual void g239();
    virtual void g240();
    virtual void g241();
    virtual void g242();
    virtual void g243();
    virtual void g244();
    virtual void g245();
    virtual void g246();
    virtual void g247();
    virtual void g248();
    virtual void g249();
    virtual void g250();
    virtual void g251();
    virtual void g252();
    virtual void g253();
    virtual void g254();
    virtual void g255();
    virtual void g256();
    virtual void g257();
    virtual void g258();
    virtual void g259();
    virtual void g260();
    virtual void g261();
    virtual void g262();
    virtual void g263();
    virtual void g264();
    virtual void g265();
    virtual void g266();
    virtual void g267();
    virtual void g268();
    virtual void g269();
    virtual void g270();
    virtual void g271();
    virtual void g272();
    virtual void g273();
    virtual void g274();
    virtual void g275();
    virtual void g276();
    virtual void g277();
    virtual void g278();
    virtual void g279();
    virtual void g280();
    virtual void g281();
    virtual void g282();
    virtual void g283();
    virtual void g284();
    virtual void g285();
    virtual void g286();
    virtual void g287();
    virtual void g288();
    virtual void g289();
    virtual void g290();
    virtual void g291();
    virtual void g292();
    virtual void g293();
    virtual void g294();
    virtual void g295();
    virtual void g296();
    virtual void g297();
    virtual void g298();
    virtual void g299();
    virtual void g300();
    virtual void g301();
    virtual void g302();
    virtual void g303();
    virtual void g304();
    virtual void g305();
    virtual void g306();
    virtual void g307();
    virtual void g308();
    virtual void g309();
    virtual void g310();
    virtual void g311();
    virtual void g312();
    virtual void g313();
    virtual void g314();
    virtual void g315();
    virtual void g316();
    virtual void g317();
    virtual void g318();
    virtual void g319();
    virtual void g320();
    virtual void g321();
    virtual void g322();
    virtual void g323();
    virtual void g324();
    virtual void g325();
    virtual void g326();
    virtual void g327();
    virtual void g328();
    virtual void g329();
    virtual void g330();
    virtual void g331();
    virtual void g332();
    virtual void g333();
    virtual void g334();
    virtual void g335();
    virtual void g336();
    virtual void g337();
    virtual void g338();
    virtual void g339();
    virtual void g340();
    virtual void g341();
    virtual void g342();
    virtual void g343();
    virtual void g344();
    virtual void g345();
    virtual void g346();
    virtual void g347();
    virtual void g348();
    virtual void g349();
    virtual void g350();
    virtual void g351();
    virtual void g352();
    virtual void g353();
    virtual void g354();
    virtual void g355();
    virtual void g356();
    virtual void g357();
    virtual void g358();
    virtual void g359();
    virtual void g360();
    virtual void g361();
    virtual void g362();
    virtual void g363();
    virtual void g364();
    virtual void g365();
    virtual void g366();
    virtual void g367();
    virtual void g368();
    virtual void g369();
    virtual void g370();
    virtual void g371();
    virtual void g372();
    virtual void g373();
    virtual void g374();
    virtual void g375();
    virtual void g376();
    virtual void g377();
    virtual void g378();
    virtual void g379();
    virtual void g380();
    virtual void g381();
    virtual void g382();
    virtual void g383();
    virtual void g384();
    virtual void g385();
    virtual void g386();
    virtual void g387();
    virtual void g388();
    virtual void g389();
    virtual void g390();
    virtual void g391();
    virtual void g392();
    virtual void g393();
    virtual void g394();
    virtual void g395();
    virtual void g396();
    virtual void g397();
    virtual void g398();
    virtual void g399();
    virtual void g400();
    virtual void g401();
    virtual void g402();
    virtual void g403();
    virtual void g404();
    virtual void g405();
    virtual void g406();
    virtual void g407();
    virtual void g408();
    virtual void g409();
    virtual void g410();
    virtual void g411();
    virtual void g412();
    virtual void g413();
    virtual void g414();
    virtual void g415();
    virtual void g416();
    virtual void g417();
    virtual void g418();
    virtual void g419();
    virtual void g420();
    virtual void g421();
    virtual void g422();
    virtual void g423();
    virtual void g424();
    virtual void g425();
    virtual void g426();
    virtual void g427();
    virtual void g428();
    virtual void g429();
    virtual void g430();
    virtual void g431();
    virtual void g432();
    virtual void g433();
    virtual void g434();
    virtual void g435();
    virtual void g436();
    virtual void g437();
    virtual void g438();
    virtual void g439();
    virtual void g440();
    virtual void g441();
    virtual void g442();
    virtual void g443();
    virtual void g444();
    virtual void g445();
    virtual void g446();
    virtual void g447();
    virtual void g448();
    virtual void g449();
    virtual void g450();
    virtual void g451();
    virtual void g452();
    virtual void g453();
    virtual void g454();
    virtual void g455();
    virtual void g456();
    virtual void g457();
    virtual void g458();
    virtual void g459();
    virtual void g460();
    virtual void g461();
    virtual void g462();
    virtual void g463();
    virtual void g464();
    virtual void g465();
    virtual void g466();
    virtual void g467();
    virtual void g468();
    virtual void g469();
    virtual void g470();
    virtual void g471();
    virtual void g472();
    virtual void g473();
    virtual void g474();
    virtual void g475();
    virtual void g476();
    virtual void g477();
    virtual void g478();
    virtual void g479();
    virtual void g480();
    virtual void g481();
    virtual void g482();
    virtual void g483();
    virtual void g484();
    virtual void g485();
    virtual void g486();
    virtual void g487();
    virtual void g488();
    virtual void g489();
    virtual void g490();
    virtual void g491();
    virtual void g492();
    virtual void g493();
    virtual void g494();
    virtual void g495();
    virtual void g496();
    virtual void g497();
    virtual void g498();
    virtual void g499();
    virtual void g500();
    virtual void g501();
    virtual void g502();
    virtual void g503();
    virtual void g504();
    virtual void g505();
    virtual void g506();
    virtual void g507();
    virtual void g508();
    virtual void g509();
    virtual void g510();
    virtual void g511();
    virtual void g512();
    virtual void g513();
    virtual void g514();
    virtual void g515();
    virtual void g516();
    virtual void g517();
    virtual void g518();
    virtual void g519();
    virtual void g520();
    virtual void g521();
    virtual void g522();
    virtual void g523();
    virtual void g524();
    virtual void g525();
    virtual void g526();
    virtual void g527();
    virtual void g528();
    virtual void g529();
    virtual void g530();
    virtual void g531();
    virtual void g532();
    virtual void g533();
    virtual void g534();
    virtual void g535();
    virtual void g536();
    virtual void g537();
    virtual void g538();
    virtual void g539();
    virtual void g540();
    virtual void g541();
    virtual void g542();
    virtual void g543();
    virtual void g544();
    virtual void g545();
    virtual void g546();
    virtual void g547();
    virtual void g548();
    virtual void g549();
    virtual void g550();
    virtual void g551();
    virtual void g552();
    virtual void g553();
    virtual void g554();
    virtual void g555();
    virtual void g556();
    virtual void g557();
    virtual void g558();
    virtual void g559();
    virtual void g560();
    virtual void g561();
    virtual void g562();
    virtual void g563();
    virtual void g564();
    virtual void g565();
    virtual void g566();
    virtual void g567();
    virtual void g568();
    virtual void g569();
    virtual void g570();
    virtual void g571();
    virtual void g572();
    virtual void g573();
    virtual void g574();
    virtual void g575();
    virtual void g576();
    virtual void g577();
    virtual void g578();
    virtual void g579();
    virtual void g580();
    virtual void g581();
    virtual void g582();
    virtual void g583();
    virtual void g584();
    virtual void g585();
    virtual void g586();
    virtual void g587();
    virtual void g588();
    virtual void g589();
    virtual void g590();
    virtual void g591();
    virtual void g592();
    virtual void g593();
    virtual void g594();
    virtual void g595();
    virtual void g596();
    virtual void EntitySetThingAsEnemyOfThing(const CScriptThingRuntime& a, const CScriptThingRuntime& b) const;   // slot 597 (+0x954) 0x0089C000
    virtual void g598();
    virtual void g599();
    virtual void g600();
    virtual void g601();
    virtual void g602();
    virtual void g603();
    virtual void g604();
    virtual void g605();
    virtual void g606();
    virtual void g607();
    virtual void g608();
    virtual void g609();
    virtual void g610();
    virtual void g611();
    virtual void g612();
    virtual void g613();
    virtual void g614();
    virtual void g615();
    virtual void g616();
    virtual void g617();
    virtual void g618();
    virtual void g619();
    virtual void g620();
    virtual void g621();
    virtual void g622();
    virtual void g623();
    virtual void g624();
    virtual void g625();
    virtual void g626();
    virtual void g627();
    virtual void g628();
    virtual void g629();
    virtual void g630();
    virtual void g631();
    virtual void g632();
    virtual void g633();
    virtual void g634();
    virtual void g635();
    virtual void g636();
    virtual void EntitySetAsAwareOfThing(const CScriptThingRuntime& a, const CScriptThingRuntime& b) const;   // slot 637 (+0x9f4) 0x0089D0E0
};
struct CSpawnedFuncAmbush;
struct CQ_AmbushTradersScript {
    unsigned char _pad[0xa8];
    vector<CScriptThingRuntime> AllBandits;    // +0xa8 (Ego_r.pdb +0xbc - 0x14)
    void WatchForEnemyDeath(CScriptThingRuntime thing);
    void Add(CSpawnedFuncAmbush* function, const CCharString& name); // 0x00CB7E50; observed function at +4, name at +8
};
typedef void (CQ_AmbushTradersScript::*SpawnedScriptCallback)(CScriptThingRuntime);
struct CSpawnedFuncAmbush : CSpawnedFuncBaseRuntime {
    SpawnedScriptCallback Func;
    CQ_AmbushTradersScript* PClass;
    CScriptThingStored Par1;
    virtual void SuspendableProcess(); // 0x00F06370 (vtable 0x012F7EBC)
    CSpawnedFuncAmbush(const CCharString& name, unsigned long id,
                       SpawnedScriptCallback func, CQ_AmbushTradersScript* owner,
                       CScriptThingRuntime par1);
};
#if defined(_MSC_VER)
FABLE_STATIC_ASSERT(sizeof(CSpawnedFuncAmbush) == 0x48);
#endif

class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
};

struct CAT_Phase1Archer : public CEntityScriptBase {
    void* __vftable;
    CGameScriptInterface* GSI;
    CScriptThingRuntime Thing;
    CQ_AmbushTradersScript* ParentClass; // PDB +0x14
    void* PMasterData;                   // PDB +0x18
    void Init();
};
#if defined(_MSC_VER)
FABLE_STATIC_ASSERT(offsetof(CAT_Phase1Archer, ParentClass) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CAT_Phase1Archer, PMasterData) == 0x18);
#endif

void CScriptProcessBaseRuntime::RuntimeHookCDD410() {}
void CScriptProcessBaseRuntime::RuntimeHookCDD420() {}
// Runtime slot body is external: retail D16780 copies Par1 directly into the
// callback's by-value argument, then sets StopRunning. The callback's native
// copy/destruction behavior is not synthesized into this Init translation unit.
CSpawnedFuncAmbush::CSpawnedFuncAmbush(const CCharString& name, unsigned long id,
                                       SpawnedScriptCallback func, CQ_AmbushTradersScript* owner,
                                       CScriptThingRuntime par1)
    : CSpawnedFuncBaseRuntime(name, id), Func(func), PClass(owner), Par1(par1)
{
}

CCharString FABLE_FASTCALL operator+(const CCharString& left, const CCharString& right);

void CAT_Phase1Archer::Init()
{
    GSI->EntitySetThingAsEnemyOfThing(Thing, GSI->GetHero());
    GSI->EntitySetThingAsEnemyOfThing(GSI->GetHero(), Thing);
    for (unsigned int i = 0; i < ParentClass->AllBandits.size(); ++i) {
        if (ParentClass->AllBandits[i].IsAlive()) {
            if (IsActiveThreadTerminating())
                return;
            GSI->EntitySetThingAsEnemyOfThing(Thing, ParentClass->AllBandits[i]);
            GSI->EntitySetThingAsEnemyOfThing(ParentClass->AllBandits[i], Thing);
        }
    }
    GSI->EntitySetAsAwareOfThing(Thing, GSI->GetHero());
    ParentClass->Add(
        new CSpawnedFuncAmbush(
            CCharString("ParentClass.", -1) + CCharString("WatchForEnemyDeath", -1),
            0, &CQ_AmbushTradersScript::WatchForEnemyDeath, ParentClass, Thing),
        CCharString("", -1));
}