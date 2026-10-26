// NScript::CV_TravellingHeroesScript::CTH_MagicBarrier::Main() -- the red magic barrier at the snow troll: alive filter on Me, raise the two red forcefield effects at BarrierFX_Here facing Me's angle; while the snow troll lives keep them up (recreating the idle pair whenever both have gone); once the troll is dead remove the forcefields, play the two switch-off effects and remove Me
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EE1500, 1519 bytes (+ int3 padding); entity-script vtable slot 1 (Main);
//   PDB ?Main@CTH_MagicBarrier@CV_TravellingHeroesScript@NScript@@EAEXXZ.
// Callees: CScriptThing copy 0x004ABE90 / dtor 0x004AA840, ~CBaseClass 0x0099A2E0, ~CIsThingAlive 0x00E241B0 (predicate
//   vtable 0x012C32F0), SetPredicate 0x00F35B10, IsActiveThreadTerminating 0x00F35B30, CCharString 0x0099EBF0 / 0x0099EAE0,
//   operator delete 0x00BFE9BC; CScriptThing slots 10 GetAngleXY, 75 IsAlive (and the implementation's slots 6 GetPos,
//   75 IsAlive through the inline forwarders); GameCalls slots 7, 72 GetThingWithScriptName, 100 CreateEffect, 108
//   RemoveThing, 474 EntitySetFacingAngle; the zero vector 0x0143E8E0.
// Strings: "BarrierFX_Here" 0x012F4428, "" 0x0122D70E, "NEW_RED_FORCEFIELD_01" 0x012C7D6C, "NEW_RED_FORCEFIELD_02" 0x012C7D54,
//   "TH_SnowTroll" 0x012F4150, "NEW_RED_FORCEFIELD_IDLE_01" 0x012C354C, "NEW_RED_FORCEFIELD_IDLE_02" 0x012C3530,
//   "NEW_RED_FORCEFIELD_IDLE_01_OFF" 0x012C3470, "NEW_RED_FORCEFIELD_IDLE_02_OFF" 0x012C3450.
// Layout: Ego_r.pdb CTH_MagicBarrier (0x1c): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14, PMasterData +0x18.
// Shape: the CTH_ThunderBoy row 0x00EDF3A0 (same TU) CScriptThing model with the inline GetPos/IsAlive forwarders: on the
//   value locals (the barrier marker, the snow troll) VC7.1 devirtualises and inlines them; the two forcefields are
//   references bound to the CreateEffect results (the VC non-const reference extension), so their IsAlive stays virtual
//   and the recreated pair is assigned through the inline counted-pointer operator=.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void operator delete(void* p);                         // 0x00BFE9BC (MSVCR71)
class C3DVector { public: float X, Y, Z; };
extern C3DVector g_ZeroVector;                         // 0x0143E8E0
class CCPPointerInfo { public:
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
struct CScriptThingImpl {                              // the polymorphic implementation behind a script thing
    virtual void i000(); virtual void i001(); virtual void i002(); virtual void i003(); virtual void i004(); virtual void i005();
    virtual const C3DVector& GetPos() const;   // slot 6 (+0x18)
    virtual void i007(); virtual void i008(); virtual void i009(); virtual void i010(); virtual void i011(); virtual void i012(); virtual void i013(); virtual void i014();
    virtual void i015(); virtual void i016(); virtual void i017(); virtual void i018(); virtual void i019(); virtual void i020(); virtual void i021(); virtual void i022();
    virtual void i023(); virtual void i024(); virtual void i025(); virtual void i026(); virtual void i027(); virtual void i028(); virtual void i029(); virtual void i030();
    virtual void i031(); virtual void i032(); virtual void i033(); virtual void i034(); virtual void i035(); virtual void i036(); virtual void i037(); virtual void i038();
    virtual void i039(); virtual void i040(); virtual void i041(); virtual void i042(); virtual void i043(); virtual void i044(); virtual void i045(); virtual void i046();
    virtual void i047(); virtual void i048(); virtual void i049(); virtual void i050(); virtual void i051(); virtual void i052(); virtual void i053(); virtual void i054();
    virtual void i055(); virtual void i056(); virtual void i057(); virtual void i058(); virtual void i059(); virtual void i060(); virtual void i061(); virtual void i062();
    virtual void i063(); virtual void i064(); virtual void i065(); virtual void i066(); virtual void i067(); virtual void i068(); virtual void i069(); virtual void i070();
    virtual void i071(); virtual void i072(); virtual void i073(); virtual void i074();
    virtual bool IsAlive() const;   // slot 75 (+0x12c)
};
class CBaseClass { public:                                    // vtable 0x01231710
    CBaseClass() { }
    virtual ~CBaseClass();                             // 0x0099A2E0
};
template <class T> class CCountedPointer { public:            // {Data, Info}
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    ~CCountedPointer()
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
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        T* data = other.Data;
        CCPPointerInfo* info = other.Info;
        if (Info != info) {
            if (Info) {
                --Info->RefCount;
                if (Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    delete Info;
                }
            }
            Data = data;
            Info = info;
            if (Info)
                ++Info->RefCount;
        }
        return *this;
    }
};
class CScriptThing : public CBaseClass { public:                     // 12 bytes, vtable 0x01238C8C
    virtual void s001(); virtual void s002(); virtual void s003(); virtual void s004(); virtual void s005();
    virtual const C3DVector& GetPos() const { return !PImp.Data ? g_ZeroVector : PImp.Data->GetPos(); }   // slot 6 (+0x18)
    virtual void s007(); virtual void s008(); virtual void s009();
    virtual float GetAngleXY() const;   // slot 10 (+0x28)
    virtual void s011(); virtual void s012(); virtual void s013(); virtual void s014(); virtual void s015(); virtual void s016(); virtual void s017(); virtual void s018();
    virtual void s019(); virtual void s020(); virtual void s021(); virtual void s022(); virtual void s023(); virtual void s024(); virtual void s025(); virtual void s026();
    virtual void s027(); virtual void s028(); virtual void s029(); virtual void s030(); virtual void s031(); virtual void s032(); virtual void s033(); virtual void s034();
    virtual void s035(); virtual void s036(); virtual void s037(); virtual void s038(); virtual void s039(); virtual void s040(); virtual void s041(); virtual void s042();
    virtual void s043(); virtual void s044(); virtual void s045(); virtual void s046(); virtual void s047(); virtual void s048(); virtual void s049(); virtual void s050();
    virtual void s051(); virtual void s052(); virtual void s053(); virtual void s054(); virtual void s055(); virtual void s056(); virtual void s057(); virtual void s058();
    virtual void s059(); virtual void s060(); virtual void s061(); virtual void s062(); virtual void s063(); virtual void s064(); virtual void s065(); virtual void s066();
    virtual void s067(); virtual void s068(); virtual void s069(); virtual void s070(); virtual void s071(); virtual void s072(); virtual void s073(); virtual void s074();
    virtual bool IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }   // slot 75 (+0x12c)
    CCountedPointer<CScriptThingImpl> PImp;            // +0x04 Data, +0x08 Info
    CScriptThing(const CScriptThing& other);           // 0x004ABE90 (the out-of-line copy)
    ~CScriptThing() { }                                // out-of-line copy 0x004AA840
    CScriptThing& operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
};
struct CIsThingAlive {                                 // vtable 0x012C32F0
    virtual void v00();
    CScriptThing m_thing;                              // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    ~CIsThingAlive();                                  // 0x00E241B0
};
struct GameCalls {                                     // CGameScriptInterface vtable 0x01260F0C (ghidra_out/fable_cgsi_slots.h)
    virtual void g000(); virtual void g001(); virtual void g002(); virtual void g003(); virtual void g004(); virtual void g005(); virtual void g006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c)
    virtual void g008(); virtual void g009(); virtual void g010(); virtual void g011(); virtual void g012(); virtual void g013(); virtual void g014(); virtual void g015();
    virtual void g016(); virtual void g017(); virtual void g018(); virtual void g019(); virtual void g020(); virtual void g021(); virtual void g022(); virtual void g023();
    virtual void g024(); virtual void g025(); virtual void g026(); virtual void g027(); virtual void g028(); virtual void g029(); virtual void g030(); virtual void g031();
    virtual void g032(); virtual void g033(); virtual void g034(); virtual void g035(); virtual void g036(); virtual void g037(); virtual void g038(); virtual void g039();
    virtual void g040(); virtual void g041(); virtual void g042(); virtual void g043(); virtual void g044(); virtual void g045(); virtual void g046(); virtual void g047();
    virtual void g048(); virtual void g049(); virtual void g050(); virtual void g051(); virtual void g052(); virtual void g053(); virtual void g054(); virtual void g055();
    virtual void g056(); virtual void g057(); virtual void g058(); virtual void g059(); virtual void g060(); virtual void g061(); virtual void g062(); virtual void g063();
    virtual void g064(); virtual void g065(); virtual void g066(); virtual void g067(); virtual void g068(); virtual void g069(); virtual void g070(); virtual void g071();
    virtual CScriptThing GetThingWithScriptName(const CCharString& name) const;   // slot 72 (+0x120)
    virtual void g073(); virtual void g074(); virtual void g075(); virtual void g076(); virtual void g077(); virtual void g078(); virtual void g079(); virtual void g080();
    virtual void g081(); virtual void g082(); virtual void g083(); virtual void g084(); virtual void g085(); virtual void g086(); virtual void g087(); virtual void g088();
    virtual void g089(); virtual void g090(); virtual void g091(); virtual void g092(); virtual void g093(); virtual void g094(); virtual void g095(); virtual void g096();
    virtual void g097(); virtual void g098(); virtual void g099();
    virtual CScriptThing CreateEffect(const CCharString& name, const C3DVector& position, const CCharString& bone, float angle, bool a, bool b) const;   // slot 100 (+0x190)
    virtual void g101(); virtual void g102(); virtual void g103(); virtual void g104(); virtual void g105(); virtual void g106(); virtual void g107();
    virtual void RemoveThing(const CScriptThing& thing, bool a, bool b) const;   // slot 108 (+0x1b0)
    virtual void g109(); virtual void g110(); virtual void g111(); virtual void g112(); virtual void g113(); virtual void g114(); virtual void g115(); virtual void g116();
    virtual void g117(); virtual void g118(); virtual void g119(); virtual void g120(); virtual void g121(); virtual void g122(); virtual void g123(); virtual void g124();
    virtual void g125(); virtual void g126(); virtual void g127(); virtual void g128(); virtual void g129(); virtual void g130(); virtual void g131(); virtual void g132();
    virtual void g133(); virtual void g134(); virtual void g135(); virtual void g136(); virtual void g137(); virtual void g138(); virtual void g139(); virtual void g140();
    virtual void g141(); virtual void g142(); virtual void g143(); virtual void g144(); virtual void g145(); virtual void g146(); virtual void g147(); virtual void g148();
    virtual void g149(); virtual void g150(); virtual void g151(); virtual void g152(); virtual void g153(); virtual void g154(); virtual void g155(); virtual void g156();
    virtual void g157(); virtual void g158(); virtual void g159(); virtual void g160(); virtual void g161(); virtual void g162(); virtual void g163(); virtual void g164();
    virtual void g165(); virtual void g166(); virtual void g167(); virtual void g168(); virtual void g169(); virtual void g170(); virtual void g171(); virtual void g172();
    virtual void g173(); virtual void g174(); virtual void g175(); virtual void g176(); virtual void g177(); virtual void g178(); virtual void g179(); virtual void g180();
    virtual void g181(); virtual void g182(); virtual void g183(); virtual void g184(); virtual void g185(); virtual void g186(); virtual void g187(); virtual void g188();
    virtual void g189(); virtual void g190(); virtual void g191(); virtual void g192(); virtual void g193(); virtual void g194(); virtual void g195(); virtual void g196();
    virtual void g197(); virtual void g198(); virtual void g199(); virtual void g200(); virtual void g201(); virtual void g202(); virtual void g203(); virtual void g204();
    virtual void g205(); virtual void g206(); virtual void g207(); virtual void g208(); virtual void g209(); virtual void g210(); virtual void g211(); virtual void g212();
    virtual void g213(); virtual void g214(); virtual void g215(); virtual void g216(); virtual void g217(); virtual void g218(); virtual void g219(); virtual void g220();
    virtual void g221(); virtual void g222(); virtual void g223(); virtual void g224(); virtual void g225(); virtual void g226(); virtual void g227(); virtual void g228();
    virtual void g229(); virtual void g230(); virtual void g231(); virtual void g232(); virtual void g233(); virtual void g234(); virtual void g235(); virtual void g236();
    virtual void g237(); virtual void g238(); virtual void g239(); virtual void g240(); virtual void g241(); virtual void g242(); virtual void g243(); virtual void g244();
    virtual void g245(); virtual void g246(); virtual void g247(); virtual void g248(); virtual void g249(); virtual void g250(); virtual void g251(); virtual void g252();
    virtual void g253(); virtual void g254(); virtual void g255(); virtual void g256(); virtual void g257(); virtual void g258(); virtual void g259(); virtual void g260();
    virtual void g261(); virtual void g262(); virtual void g263(); virtual void g264(); virtual void g265(); virtual void g266(); virtual void g267(); virtual void g268();
    virtual void g269(); virtual void g270(); virtual void g271(); virtual void g272(); virtual void g273(); virtual void g274(); virtual void g275(); virtual void g276();
    virtual void g277(); virtual void g278(); virtual void g279(); virtual void g280(); virtual void g281(); virtual void g282(); virtual void g283(); virtual void g284();
    virtual void g285(); virtual void g286(); virtual void g287(); virtual void g288(); virtual void g289(); virtual void g290(); virtual void g291(); virtual void g292();
    virtual void g293(); virtual void g294(); virtual void g295(); virtual void g296(); virtual void g297(); virtual void g298(); virtual void g299(); virtual void g300();
    virtual void g301(); virtual void g302(); virtual void g303(); virtual void g304(); virtual void g305(); virtual void g306(); virtual void g307(); virtual void g308();
    virtual void g309(); virtual void g310(); virtual void g311(); virtual void g312(); virtual void g313(); virtual void g314(); virtual void g315(); virtual void g316();
    virtual void g317(); virtual void g318(); virtual void g319(); virtual void g320(); virtual void g321(); virtual void g322(); virtual void g323(); virtual void g324();
    virtual void g325(); virtual void g326(); virtual void g327(); virtual void g328(); virtual void g329(); virtual void g330(); virtual void g331(); virtual void g332();
    virtual void g333(); virtual void g334(); virtual void g335(); virtual void g336(); virtual void g337(); virtual void g338(); virtual void g339(); virtual void g340();
    virtual void g341(); virtual void g342(); virtual void g343(); virtual void g344(); virtual void g345(); virtual void g346(); virtual void g347(); virtual void g348();
    virtual void g349(); virtual void g350(); virtual void g351(); virtual void g352(); virtual void g353(); virtual void g354(); virtual void g355(); virtual void g356();
    virtual void g357(); virtual void g358(); virtual void g359(); virtual void g360(); virtual void g361(); virtual void g362(); virtual void g363(); virtual void g364();
    virtual void g365(); virtual void g366(); virtual void g367(); virtual void g368(); virtual void g369(); virtual void g370(); virtual void g371(); virtual void g372();
    virtual void g373(); virtual void g374(); virtual void g375(); virtual void g376(); virtual void g377(); virtual void g378(); virtual void g379(); virtual void g380();
    virtual void g381(); virtual void g382(); virtual void g383(); virtual void g384(); virtual void g385(); virtual void g386(); virtual void g387(); virtual void g388();
    virtual void g389(); virtual void g390(); virtual void g391(); virtual void g392(); virtual void g393(); virtual void g394(); virtual void g395(); virtual void g396();
    virtual void g397(); virtual void g398(); virtual void g399(); virtual void g400(); virtual void g401(); virtual void g402(); virtual void g403(); virtual void g404();
    virtual void g405(); virtual void g406(); virtual void g407(); virtual void g408(); virtual void g409(); virtual void g410(); virtual void g411(); virtual void g412();
    virtual void g413(); virtual void g414(); virtual void g415(); virtual void g416(); virtual void g417(); virtual void g418(); virtual void g419(); virtual void g420();
    virtual void g421(); virtual void g422(); virtual void g423(); virtual void g424(); virtual void g425(); virtual void g426(); virtual void g427(); virtual void g428();
    virtual void g429(); virtual void g430(); virtual void g431(); virtual void g432(); virtual void g433(); virtual void g434(); virtual void g435(); virtual void g436();
    virtual void g437(); virtual void g438(); virtual void g439(); virtual void g440(); virtual void g441(); virtual void g442(); virtual void g443(); virtual void g444();
    virtual void g445(); virtual void g446(); virtual void g447(); virtual void g448(); virtual void g449(); virtual void g450(); virtual void g451(); virtual void g452();
    virtual void g453(); virtual void g454(); virtual void g455(); virtual void g456(); virtual void g457(); virtual void g458(); virtual void g459(); virtual void g460();
    virtual void g461(); virtual void g462(); virtual void g463(); virtual void g464(); virtual void g465(); virtual void g466(); virtual void g467(); virtual void g468();
    virtual void g469(); virtual void g470(); virtual void g471(); virtual void g472(); virtual void g473();
    virtual void EntitySetFacingAngle(const CScriptThing& thing, float angle, bool a) const;   // slot 474 (+0x768)
};

namespace NScript {
struct CEntityScriptBase {                             // retail 0x14 bytes
    virtual void e00();
    GameCalls*   Game;                                 // +0x04
    CScriptThing Me;                                   // +0x08
    void SetPredicate(CIsThingAlive& predicate);       // 0x00F35B10
    bool IsActiveThreadTerminating() const;            // 0x00F35B30
};
class CQ_SunnyvaleMasterData;
class CV_TravellingHeroesScript { public:
    class CTH_MagicBarrier;
};
class CV_TravellingHeroesScript::CTH_MagicBarrier : public CEntityScriptBase {
public:
    CV_TravellingHeroesScript* ParentClass;            // +0x14
    CQ_SunnyvaleMasterData*    PMasterData;            // +0x18
    void Main();
};
}

void NScript::CV_TravellingHeroesScript::CTH_MagicBarrier::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    CScriptThing barrier = Game->GetThingWithScriptName("BarrierFX_Here");
    CScriptThing& forcefield1 = Game->CreateEffect("NEW_RED_FORCEFIELD_01", barrier.GetPos(), "", Me.GetAngleXY(), false, false);
    CScriptThing& forcefield2 = Game->CreateEffect("NEW_RED_FORCEFIELD_02", barrier.GetPos(), "", Me.GetAngleXY(), false, false);
    CScriptThing snowTroll = Game->GetThingWithScriptName("TH_SnowTroll");
    Game->EntitySetFacingAngle(forcefield1, Me.GetAngleXY(), true);
    Game->EntitySetFacingAngle(forcefield2, Me.GetAngleXY(), true);
    while (snowTroll.IsAlive()) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (!forcefield1.IsAlive() && !forcefield2.IsAlive()) {
            if (IsActiveThreadTerminating()) return;
            forcefield1 = Game->CreateEffect("NEW_RED_FORCEFIELD_IDLE_01", barrier.GetPos(), "", Me.GetAngleXY(), false, false);
            forcefield2 = Game->CreateEffect("NEW_RED_FORCEFIELD_IDLE_02", barrier.GetPos(), "", Me.GetAngleXY(), false, false);
            Game->EntitySetFacingAngle(forcefield1, Me.GetAngleXY(), true);
            Game->EntitySetFacingAngle(forcefield2, Me.GetAngleXY(), true);
        }
    }
    if (IsActiveThreadTerminating()) return;
    Game->RemoveThing(forcefield1, false, true);
    Game->RemoveThing(forcefield2, false, true);
    Game->CreateEffect("NEW_RED_FORCEFIELD_IDLE_01_OFF", barrier.GetPos(), "", Me.GetAngleXY(), false, false);
    Game->CreateEffect("NEW_RED_FORCEFIELD_IDLE_02_OFF", barrier.GetPos(), "", Me.GetAngleXY(), false, false);
    Game->EntitySetFacingAngle(forcefield1, Me.GetAngleXY(), true);
    Game->EntitySetFacingAngle(forcefield2, Me.GetAngleXY(), true);
    Game->RemoveThing(Me, false, true);
}