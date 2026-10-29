// NScript::CQ_AmbushTradersScript::CAT_Sentry::Init() -- the ambush-traders sentry: wander about where it stands (minimum 0, maximum the def's AmbushTradersSentryWanderDistance) in scripting state group 4, make it and the hero enemies of each other, then each living bandit of the parent's AllBandits (stopping if the thread is done), spawn the parent's WatchForHeroAttackOnSentry and WatchForEnemyDeath threads on it, and clear SentrySeenSpy
// Leaf: ?Init@CAT_Sentry@CQ_AmbushTradersScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F08E90, 961 bytes, plain `ret`; entity-script vtable 0x012F7E30 slot 2 (Init), stored
//   by the landed CAT_Sentry::Alloc 0x00F04510 (Main 0x00F045E0 is slot 1); PDB
//   ?Init@CAT_Sentry@CQ_AmbushTradersScript@NScript@@EAEXXZ (Ego_r.pdb 0x00F89B40 + FableWin.pdb 0x0363A6D0, both between
//   Alloc/GetParentScript and OnPersist/Main as here). Catalog label is a propagated sub_.
// Callees: CScriptThing::GetPos slot 6 (+0x18) / IsAlive slot 75 (+0x12c), CScriptThing copy 0x004ABE90 / dtor 0x004AA840,
//   CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30, operator new 0x00BFEA1A, CCharString 0x0099EBF0 / 0x0099EAE0,
//   operator+ 0x0099F570, CSpawnedFuncBase ctor 0x00CDD450, CScriptBase::AddSpawnedFunction 0x00CB7E50;
//   CSpawnedFunc1<CQ_AmbushTradersScript, CScriptThing> vtable 0x012F7EBC; the spawned functions are
//   CQ_AmbushTradersScript::WatchForHeroAttackOnSentry 0x00F09260 and ::WatchForEnemyDeath 0x00F08590 (both landed);
//   CGameScriptInterface slots 70, 597, 763 SetWanderCentrePoint, 766 SetWanderMinDistance, 769 SetWanderMaxDistance,
//   784 SetScriptingStateGroup.
// Strings: "WatchForHeroAttackOnSentry" 0x012F8128, "WatchForEnemyDeath" 0x012F8114, "ParentClass." 0x012C9934, "" 0x0122D70E.
// Layout: Ego_r.pdb CAT_Sentry (0x20): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14, PMasterData +0x18,
//   SentrySeenSpy +0x1c. CQ_AmbushTradersScript::AllBandits retail +0xa8 (Ego_r.pdb +0xbc). pGlobalScriptDef 0x0143E90C:
//   retail +0x9f0 = Ego_r.pdb 0xa90 AmbushTradersSentryWanderDistance (the def's retail offsets sit 0xa0 below the PDB's).
//   CSpawnedFuncBase (0x34, Ego_r.pdb) is modelled locally: engine/CSpawnedFuncBase.h flattens the vptr into a data
//   member and has no constructor, so the inlined CSpawnedFunc1 ctor (base ctor 0x00CDD450, then the derived vtable
//   store) cannot be written against it (NOTES.md header debt).
// Shape: the landed CAT_Phase1Guard::Init 0x00F083A0 (same TU) spawn block; by-value CScriptThing arguments are the inline
//   copy (InlineCopy tag, as the landed CAT_Sentry::Main), CSpawnedFunc1::Par1 the out-of-line one.
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
void* __cdecl operator new(unsigned int bytes);        // 0x00BFEA1A (MSVCR71)
void operator delete(void* p);                         // 0x00BFE9BC (MSVCR71)
class C3DVector { public: float X, Y, Z; C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { } };
class CCPPointerInfo { public: long References; void (__fastcall* DeleteFunc)(void* data); void* Data; };
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    __forceinline CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->References; }
    __forceinline ~CCountedPointer()
    {
        if (Info) {
            --Info->References;
            if (Info->References == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
class CBaseClass { public:                                    // vtable 0x01231710
    virtual ~CBaseClass();                             // 0x0099A2E0
    __forceinline CBaseClass() { }
};
class CScriptThing : public CBaseClass { public:                     // vtable 0x01238C8C, 12 bytes
    virtual void t01(); virtual void t02(); virtual void t03(); virtual void t04(); virtual void t05();
    virtual const C3DVector& GetPos() const;           // slot 6 (+0x18)
    virtual void t07(); virtual void t08(); virtual void t09(); virtual void t10(); virtual void t11(); virtual void t12(); virtual void t13(); virtual void t14();
    virtual void t15(); virtual void t16(); virtual void t17(); virtual void t18(); virtual void t19(); virtual void t20(); virtual void t21(); virtual void t22();
    virtual void t23(); virtual void t24(); virtual void t25(); virtual void t26(); virtual void t27(); virtual void t28(); virtual void t29(); virtual void t30();
    virtual void t31(); virtual void t32(); virtual void t33(); virtual void t34(); virtual void t35(); virtual void t36(); virtual void t37(); virtual void t38();
    virtual void t39(); virtual void t40(); virtual void t41(); virtual void t42(); virtual void t43(); virtual void t44(); virtual void t45(); virtual void t46();
    virtual void t47(); virtual void t48(); virtual void t49(); virtual void t50(); virtual void t51(); virtual void t52(); virtual void t53(); virtual void t54();
    virtual void t55(); virtual void t56(); virtual void t57(); virtual void t58(); virtual void t59(); virtual void t60(); virtual void t61(); virtual void t62();
    virtual void t63(); virtual void t64(); virtual void t65(); virtual void t66(); virtual void t67(); virtual void t68(); virtual void t69(); virtual void t70();
    virtual void t71(); virtual void t72(); virtual void t73(); virtual void t74();
    virtual bool IsAlive() const;                      // slot 75 (+0x12c)
    CCountedPointer<CScriptThing>     PImp;            // +0x04
    struct InlineCopy { };                             // tag: the inline copy retail expands at by-value call sites
    CScriptThing(const CScriptThing& other);           // 0x004ABE90 (the out-of-line copy)
    __forceinline CScriptThing(const CScriptThing& other, InlineCopy) : CBaseClass(), PImp(other.PImp) { }
    ~CScriptThing();                                   // 0x004AA840
};
typedef long EScriptingStateGroups;
class CGameScriptInterfaceBase { public:              // the interface CGameScriptInterface (vtable 0x01260F0C) implements; (ghidra_out/cgsi_slots_aligned.tsv)
    virtual void g000(); virtual void g001(); virtual void g002(); virtual void g003(); virtual void g004(); virtual void g005(); virtual void g006(); virtual void g007();
    virtual void g008(); virtual void g009(); virtual void g010(); virtual void g011(); virtual void g012(); virtual void g013(); virtual void g014(); virtual void g015();
    virtual void g016(); virtual void g017(); virtual void g018(); virtual void g019(); virtual void g020(); virtual void g021(); virtual void g022(); virtual void g023();
    virtual void g024(); virtual void g025(); virtual void g026(); virtual void g027(); virtual void g028(); virtual void g029(); virtual void g030(); virtual void g031();
    virtual void g032(); virtual void g033(); virtual void g034(); virtual void g035(); virtual void g036(); virtual void g037(); virtual void g038(); virtual void g039();
    virtual void g040(); virtual void g041(); virtual void g042(); virtual void g043(); virtual void g044(); virtual void g045(); virtual void g046(); virtual void g047();
    virtual void g048(); virtual void g049(); virtual void g050(); virtual void g051(); virtual void g052(); virtual void g053(); virtual void g054(); virtual void g055();
    virtual void g056(); virtual void g057(); virtual void g058(); virtual void g059(); virtual void g060(); virtual void g061(); virtual void g062(); virtual void g063();
    virtual void g064(); virtual void g065(); virtual void g066(); virtual void g067(); virtual void g068(); virtual void g069();
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
    virtual void g183(); virtual void g184(); virtual void g185(); virtual void g186(); virtual void g187(); virtual void g188(); virtual void g189(); virtual void g190();
    virtual void g191(); virtual void g192(); virtual void g193(); virtual void g194(); virtual void g195(); virtual void g196(); virtual void g197(); virtual void g198();
    virtual void g199(); virtual void g200(); virtual void g201(); virtual void g202(); virtual void g203(); virtual void g204(); virtual void g205(); virtual void g206();
    virtual void g207(); virtual void g208(); virtual void g209(); virtual void g210(); virtual void g211(); virtual void g212(); virtual void g213(); virtual void g214();
    virtual void g215(); virtual void g216(); virtual void g217(); virtual void g218(); virtual void g219(); virtual void g220(); virtual void g221(); virtual void g222();
    virtual void g223(); virtual void g224(); virtual void g225(); virtual void g226(); virtual void g227(); virtual void g228(); virtual void g229(); virtual void g230();
    virtual void g231(); virtual void g232(); virtual void g233(); virtual void g234(); virtual void g235(); virtual void g236(); virtual void g237(); virtual void g238();
    virtual void g239(); virtual void g240(); virtual void g241(); virtual void g242(); virtual void g243(); virtual void g244(); virtual void g245(); virtual void g246();
    virtual void g247(); virtual void g248(); virtual void g249(); virtual void g250(); virtual void g251(); virtual void g252(); virtual void g253(); virtual void g254();
    virtual void g255(); virtual void g256(); virtual void g257(); virtual void g258(); virtual void g259(); virtual void g260(); virtual void g261(); virtual void g262();
    virtual void g263(); virtual void g264(); virtual void g265(); virtual void g266(); virtual void g267(); virtual void g268(); virtual void g269(); virtual void g270();
    virtual void g271(); virtual void g272(); virtual void g273(); virtual void g274(); virtual void g275(); virtual void g276(); virtual void g277(); virtual void g278();
    virtual void g279(); virtual void g280(); virtual void g281(); virtual void g282(); virtual void g283(); virtual void g284(); virtual void g285(); virtual void g286();
    virtual void g287(); virtual void g288(); virtual void g289(); virtual void g290(); virtual void g291(); virtual void g292(); virtual void g293(); virtual void g294();
    virtual void g295(); virtual void g296(); virtual void g297(); virtual void g298(); virtual void g299(); virtual void g300(); virtual void g301(); virtual void g302();
    virtual void g303(); virtual void g304(); virtual void g305(); virtual void g306(); virtual void g307(); virtual void g308(); virtual void g309(); virtual void g310();
    virtual void g311(); virtual void g312(); virtual void g313(); virtual void g314(); virtual void g315(); virtual void g316(); virtual void g317(); virtual void g318();
    virtual void g319(); virtual void g320(); virtual void g321(); virtual void g322(); virtual void g323(); virtual void g324(); virtual void g325(); virtual void g326();
    virtual void g327(); virtual void g328(); virtual void g329(); virtual void g330(); virtual void g331(); virtual void g332(); virtual void g333(); virtual void g334();
    virtual void g335(); virtual void g336(); virtual void g337(); virtual void g338(); virtual void g339(); virtual void g340(); virtual void g341(); virtual void g342();
    virtual void g343(); virtual void g344(); virtual void g345(); virtual void g346(); virtual void g347(); virtual void g348(); virtual void g349(); virtual void g350();
    virtual void g351(); virtual void g352(); virtual void g353(); virtual void g354(); virtual void g355(); virtual void g356(); virtual void g357(); virtual void g358();
    virtual void g359(); virtual void g360(); virtual void g361(); virtual void g362(); virtual void g363(); virtual void g364(); virtual void g365(); virtual void g366();
    virtual void g367(); virtual void g368(); virtual void g369(); virtual void g370(); virtual void g371(); virtual void g372(); virtual void g373(); virtual void g374();
    virtual void g375(); virtual void g376(); virtual void g377(); virtual void g378(); virtual void g379(); virtual void g380(); virtual void g381(); virtual void g382();
    virtual void g383(); virtual void g384(); virtual void g385(); virtual void g386(); virtual void g387(); virtual void g388(); virtual void g389(); virtual void g390();
    virtual void g391(); virtual void g392(); virtual void g393(); virtual void g394(); virtual void g395(); virtual void g396(); virtual void g397(); virtual void g398();
    virtual void g399(); virtual void g400(); virtual void g401(); virtual void g402(); virtual void g403(); virtual void g404(); virtual void g405(); virtual void g406();
    virtual void g407(); virtual void g408(); virtual void g409(); virtual void g410(); virtual void g411(); virtual void g412(); virtual void g413(); virtual void g414();
    virtual void g415(); virtual void g416(); virtual void g417(); virtual void g418(); virtual void g419(); virtual void g420(); virtual void g421(); virtual void g422();
    virtual void g423(); virtual void g424(); virtual void g425(); virtual void g426(); virtual void g427(); virtual void g428(); virtual void g429(); virtual void g430();
    virtual void g431(); virtual void g432(); virtual void g433(); virtual void g434(); virtual void g435(); virtual void g436(); virtual void g437(); virtual void g438();
    virtual void g439(); virtual void g440(); virtual void g441(); virtual void g442(); virtual void g443(); virtual void g444(); virtual void g445(); virtual void g446();
    virtual void g447(); virtual void g448(); virtual void g449(); virtual void g450(); virtual void g451(); virtual void g452(); virtual void g453(); virtual void g454();
    virtual void g455(); virtual void g456(); virtual void g457(); virtual void g458(); virtual void g459(); virtual void g460(); virtual void g461(); virtual void g462();
    virtual void g463(); virtual void g464(); virtual void g465(); virtual void g466(); virtual void g467(); virtual void g468(); virtual void g469(); virtual void g470();
    virtual void g471(); virtual void g472(); virtual void g473(); virtual void g474(); virtual void g475(); virtual void g476(); virtual void g477(); virtual void g478();
    virtual void g479(); virtual void g480(); virtual void g481(); virtual void g482(); virtual void g483(); virtual void g484(); virtual void g485(); virtual void g486();
    virtual void g487(); virtual void g488(); virtual void g489(); virtual void g490(); virtual void g491(); virtual void g492(); virtual void g493(); virtual void g494();
    virtual void g495(); virtual void g496(); virtual void g497(); virtual void g498(); virtual void g499(); virtual void g500(); virtual void g501(); virtual void g502();
    virtual void g503(); virtual void g504(); virtual void g505(); virtual void g506(); virtual void g507(); virtual void g508(); virtual void g509(); virtual void g510();
    virtual void g511(); virtual void g512(); virtual void g513(); virtual void g514(); virtual void g515(); virtual void g516(); virtual void g517(); virtual void g518();
    virtual void g519(); virtual void g520(); virtual void g521(); virtual void g522(); virtual void g523(); virtual void g524(); virtual void g525(); virtual void g526();
    virtual void g527(); virtual void g528(); virtual void g529(); virtual void g530(); virtual void g531(); virtual void g532(); virtual void g533(); virtual void g534();
    virtual void g535(); virtual void g536(); virtual void g537(); virtual void g538(); virtual void g539(); virtual void g540(); virtual void g541(); virtual void g542();
    virtual void g543(); virtual void g544(); virtual void g545(); virtual void g546(); virtual void g547(); virtual void g548(); virtual void g549(); virtual void g550();
    virtual void g551(); virtual void g552(); virtual void g553(); virtual void g554(); virtual void g555(); virtual void g556(); virtual void g557(); virtual void g558();
    virtual void g559(); virtual void g560(); virtual void g561(); virtual void g562(); virtual void g563(); virtual void g564(); virtual void g565(); virtual void g566();
    virtual void g567(); virtual void g568(); virtual void g569(); virtual void g570(); virtual void g571(); virtual void g572(); virtual void g573(); virtual void g574();
    virtual void g575(); virtual void g576(); virtual void g577(); virtual void g578(); virtual void g579(); virtual void g580(); virtual void g581(); virtual void g582();
    virtual void g583(); virtual void g584(); virtual void g585(); virtual void g586(); virtual void g587(); virtual void g588(); virtual void g589(); virtual void g590();
    virtual void g591(); virtual void g592(); virtual void g593(); virtual void g594(); virtual void g595(); virtual void g596();
    virtual void EntitySetThingAsEnemyOfThing(const CScriptThing& thing, const CScriptThing& enemy) const;   // slot 597 (+0x954)
    virtual void g598(); virtual void g599(); virtual void g600(); virtual void g601(); virtual void g602(); virtual void g603(); virtual void g604(); virtual void g605();
    virtual void g606(); virtual void g607(); virtual void g608(); virtual void g609(); virtual void g610(); virtual void g611(); virtual void g612(); virtual void g613();
    virtual void g614(); virtual void g615(); virtual void g616(); virtual void g617(); virtual void g618(); virtual void g619(); virtual void g620(); virtual void g621();
    virtual void g622(); virtual void g623(); virtual void g624(); virtual void g625(); virtual void g626(); virtual void g627(); virtual void g628(); virtual void g629();
    virtual void g630(); virtual void g631(); virtual void g632(); virtual void g633(); virtual void g634(); virtual void g635(); virtual void g636(); virtual void g637();
    virtual void g638(); virtual void g639(); virtual void g640(); virtual void g641(); virtual void g642(); virtual void g643(); virtual void g644(); virtual void g645();
    virtual void g646(); virtual void g647(); virtual void g648(); virtual void g649(); virtual void g650(); virtual void g651(); virtual void g652(); virtual void g653();
    virtual void g654(); virtual void g655(); virtual void g656(); virtual void g657(); virtual void g658(); virtual void g659(); virtual void g660(); virtual void g661();
    virtual void g662(); virtual void g663(); virtual void g664(); virtual void g665(); virtual void g666(); virtual void g667(); virtual void g668(); virtual void g669();
    virtual void g670(); virtual void g671(); virtual void g672(); virtual void g673(); virtual void g674(); virtual void g675(); virtual void g676(); virtual void g677();
    virtual void g678(); virtual void g679(); virtual void g680(); virtual void g681(); virtual void g682(); virtual void g683(); virtual void g684(); virtual void g685();
    virtual void g686(); virtual void g687(); virtual void g688(); virtual void g689(); virtual void g690(); virtual void g691(); virtual void g692(); virtual void g693();
    virtual void g694(); virtual void g695(); virtual void g696(); virtual void g697(); virtual void g698(); virtual void g699(); virtual void g700(); virtual void g701();
    virtual void g702(); virtual void g703(); virtual void g704(); virtual void g705(); virtual void g706(); virtual void g707(); virtual void g708(); virtual void g709();
    virtual void g710(); virtual void g711(); virtual void g712(); virtual void g713(); virtual void g714(); virtual void g715(); virtual void g716(); virtual void g717();
    virtual void g718(); virtual void g719(); virtual void g720(); virtual void g721(); virtual void g722(); virtual void g723(); virtual void g724(); virtual void g725();
    virtual void g726(); virtual void g727(); virtual void g728(); virtual void g729(); virtual void g730(); virtual void g731(); virtual void g732(); virtual void g733();
    virtual void g734(); virtual void g735(); virtual void g736(); virtual void g737(); virtual void g738(); virtual void g739(); virtual void g740(); virtual void g741();
    virtual void g742(); virtual void g743(); virtual void g744(); virtual void g745(); virtual void g746(); virtual void g747(); virtual void g748(); virtual void g749();
    virtual void g750(); virtual void g751(); virtual void g752(); virtual void g753(); virtual void g754(); virtual void g755(); virtual void g756(); virtual void g757();
    virtual void g758(); virtual void g759(); virtual void g760(); virtual void g761(); virtual void g762();
    virtual void SetWanderCentrePoint(CScriptThing thing, C3DVector centre);   // slot 763 (+0xbec)
    virtual void g764(); virtual void g765();
    virtual void SetWanderMinDistance(CScriptThing thing, float distance);   // slot 766 (+0xbf8)
    virtual void g767(); virtual void g768();
    virtual void SetWanderMaxDistance(CScriptThing thing, float distance);   // slot 769 (+0xc04)
    virtual void g770(); virtual void g771(); virtual void g772(); virtual void g773(); virtual void g774(); virtual void g775(); virtual void g776(); virtual void g777();
    virtual void g778(); virtual void g779(); virtual void g780(); virtual void g781(); virtual void g782(); virtual void g783();
    virtual void SetScriptingStateGroup(CScriptThing thing, EScriptingStateGroups group);   // slot 784 (+0xc40)
};
class CScriptDef { public:
    unsigned char _pad[0x9f0];
    float AmbushTradersSentryWanderDistance;           // +0x9f0 (retail; Ego_r.pdb 0xa90)
};
extern CScriptDef* pGlobalScriptDef;                   // 0x0143E90C

class CSpawnedFuncBase { public:                       // 0x34 bytes
    unsigned char _base[0x10];                         // CScriptProcessBase +0x04 .. +0x13 (vptr +0x00)
    unsigned long ID;                                  // +0x14
    long LastScriptFrame;                              // +0x18
    long CurrentScriptFrame;                           // +0x1c
    CCharString Name;                                  // +0x20
    CCharString AssociatedRegionName;                  // +0x24
    bool FuncPaused;                                   // +0x28
    bool Interrupted;                                  // +0x29
    CCountedPointer<NScript::CPredicateBase> PPredicate;   // +0x2c
    CSpawnedFuncBase(const CCharString& name, unsigned long id);   // 0x00CDD450
    virtual ~CSpawnedFuncBase();                       // 0x00CC2AB0 (Ego_r.pdb)
    virtual void SuspendableProcess() = 0;
};
template <class TClass, class TPar1> class CSpawnedFunc1 : public CSpawnedFuncBase { public:   // 0x48 bytes
    void (TClass::*Func)(TPar1);                       // +0x34
    TClass* PClass;                                    // +0x38
    TPar1   Par1;                                      // +0x3c
    __forceinline CSpawnedFunc1(const CCharString& name, unsigned long id, TClass* pclass, void (TClass::*func)(TPar1), TPar1 par1)
        : CSpawnedFuncBase(name, id), Func(func), PClass(pclass), Par1(par1) { }
    virtual void SuspendableProcess();
};
class CScriptBase { public:                            // retail 0x40 bytes
    unsigned char _body[0x40];
    void AddSpawnedFunction(CSpawnedFuncBase* func, const CCharString& name);   // 0x00CB7E50
};
namespace std {                                        // STLport 4.6 vector (12 bytes)
template <class _Tp> class allocator { public:
    allocator() { }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base { public:
    _Tp* _M_start;                                     // +0x00
    _Tp* _M_finish;                                    // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> { public:
    typedef unsigned int size_type;
    typedef _Tp& reference;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    reference operator[](size_type __n) { return *(this->_M_start + __n); }
};
}
class CEntityScriptBase { public:                     // retail 0x14 bytes
    virtual void e00();
    CGameScriptInterfaceBase* Game;                       // +0x04
    CScriptThing          Me;                          // +0x08
    bool IsActiveThreadTerminating() const;            // 0x00F35B30
};
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_AmbushTradersScript : public CScriptBase { public:
    unsigned char _pad40[0x68];
    std::vector<CScriptThing> AllBandits;                  // +0xa8 (retail; Ego_r.pdb +0xbc)
    void WatchForEnemyDeath(CScriptThing thing);       // 0x00F08590
    void WatchForHeroAttackOnSentry(CScriptThing thing);   // 0x00F09260
    class CAT_Sentry;
};
class CQ_AmbushTradersScript::CAT_Sentry : public CEntityScriptBase {
public:
    CQ_AmbushTradersScript* ParentClass;               // +0x14
    CQ_SunnyvaleMasterData* PMasterData;               // +0x18
    bool                    SentrySeenSpy;             // +0x1c
private:
    virtual void Main();                               // slot 1 (+0x04)
    virtual void Init();                               // slot 2 (+0x08)
};
}

void NScript::CQ_AmbushTradersScript::CAT_Sentry::Init()
{
    Game->SetWanderCentrePoint(CScriptThing(Me, CScriptThing::InlineCopy()), Me.GetPos());
    Game->SetWanderMinDistance(CScriptThing(Me, CScriptThing::InlineCopy()), 0.0f);
    Game->SetWanderMaxDistance(CScriptThing(Me, CScriptThing::InlineCopy()), pGlobalScriptDef->AmbushTradersSentryWanderDistance);
    Game->SetScriptingStateGroup(CScriptThing(Me, CScriptThing::InlineCopy()), 4);
    Game->EntitySetThingAsEnemyOfThing(Me, Game->GetHero());
    Game->EntitySetThingAsEnemyOfThing(Game->GetHero(), Me);
    for (unsigned long i = 0; i < ParentClass->AllBandits.size(); ++i) {
        if (ParentClass->AllBandits[i].IsAlive()) {
            if (IsActiveThreadTerminating())
                return;
            Game->EntitySetThingAsEnemyOfThing(Me, ParentClass->AllBandits[i]);
            Game->EntitySetThingAsEnemyOfThing(ParentClass->AllBandits[i], Me);
        }
    }
    ParentClass->AddSpawnedFunction(
        new CSpawnedFunc1<CQ_AmbushTradersScript, CScriptThing>(
            CCharString("ParentClass.") + CCharString("WatchForHeroAttackOnSentry"), 0, ParentClass,
            &CQ_AmbushTradersScript::WatchForHeroAttackOnSentry, CScriptThing(Me, CScriptThing::InlineCopy())),
        CCharString(""));
    ParentClass->AddSpawnedFunction(
        new CSpawnedFunc1<CQ_AmbushTradersScript, CScriptThing>(
            CCharString("ParentClass.") + CCharString("WatchForEnemyDeath"), 0, ParentClass,
            &CQ_AmbushTradersScript::WatchForEnemyDeath, CScriptThing(Me, CScriptThing::InlineCopy())),
        CCharString(""));
    SentrySeenSpy = false;
}