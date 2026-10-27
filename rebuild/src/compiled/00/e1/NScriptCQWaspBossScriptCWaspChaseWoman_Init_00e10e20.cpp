// NScript::CQ_WaspBossScript::CWaspChaseWoman::Init() -- EntitySetAsScared(Me, true) through the game interface
// Leaf: ?Init@CWaspChaseWoman@CQ_WaspBossScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E10E20, 20 bytes, thiscall, plain `ret`.
// Identity (proven): slot 2 (Init) of the entity vtable 0x012E0408 that the landed CWaspChaseWoman::Alloc 0x00E10D70 stores; FableWin.pdb
//   ?Init@CWaspChaseWoman@CQ_WaspBossScript@NScript@@EAEXXZ (private virtual).
// Callees: CGameScriptInterface::EntitySetAsScared(const CScriptThing&, bool) const -- slot 496 (+0x7C0) of the interface at +4
//   (ghidra_out/cgsi_slots_aligned.tsv).
// Shape (family K069, representative 0x00DD4480): `this` copied to eax, Game loaded through it, the constant pushed, then &Me.
class CScriptThing {                                    // 12 bytes (Ego_r): vptr, PImp
public:
    virtual ~CScriptThing();
    void* PImpData;                                     // +0x04
    void* PImpInfo;                                     // +0x08
};
class CGameScriptInterface {
public:
    virtual void s000(); virtual void s001(); virtual void s002(); virtual void s003(); virtual void s004(); virtual void s005(); virtual void s006(); virtual void s007();
    virtual void s008(); virtual void s009(); virtual void s010(); virtual void s011(); virtual void s012(); virtual void s013(); virtual void s014(); virtual void s015();
    virtual void s016(); virtual void s017(); virtual void s018(); virtual void s019(); virtual void s020(); virtual void s021(); virtual void s022(); virtual void s023();
    virtual void s024(); virtual void s025(); virtual void s026(); virtual void s027(); virtual void s028(); virtual void s029(); virtual void s030(); virtual void s031();
    virtual void s032(); virtual void s033(); virtual void s034(); virtual void s035(); virtual void s036(); virtual void s037(); virtual void s038(); virtual void s039();
    virtual void s040(); virtual void s041(); virtual void s042(); virtual void s043(); virtual void s044(); virtual void s045(); virtual void s046(); virtual void s047();
    virtual void s048(); virtual void s049(); virtual void s050(); virtual void s051(); virtual void s052(); virtual void s053(); virtual void s054(); virtual void s055();
    virtual void s056(); virtual void s057(); virtual void s058(); virtual void s059(); virtual void s060(); virtual void s061(); virtual void s062(); virtual void s063();
    virtual void s064(); virtual void s065(); virtual void s066(); virtual void s067(); virtual void s068(); virtual void s069(); virtual void s070(); virtual void s071();
    virtual void s072(); virtual void s073(); virtual void s074(); virtual void s075(); virtual void s076(); virtual void s077(); virtual void s078(); virtual void s079();
    virtual void s080(); virtual void s081(); virtual void s082(); virtual void s083(); virtual void s084(); virtual void s085(); virtual void s086(); virtual void s087();
    virtual void s088(); virtual void s089(); virtual void s090(); virtual void s091(); virtual void s092(); virtual void s093(); virtual void s094(); virtual void s095();
    virtual void s096(); virtual void s097(); virtual void s098(); virtual void s099(); virtual void s100(); virtual void s101(); virtual void s102(); virtual void s103();
    virtual void s104(); virtual void s105(); virtual void s106(); virtual void s107(); virtual void s108(); virtual void s109(); virtual void s110(); virtual void s111();
    virtual void s112(); virtual void s113(); virtual void s114(); virtual void s115(); virtual void s116(); virtual void s117(); virtual void s118(); virtual void s119();
    virtual void s120(); virtual void s121(); virtual void s122(); virtual void s123(); virtual void s124(); virtual void s125(); virtual void s126(); virtual void s127();
    virtual void s128(); virtual void s129(); virtual void s130(); virtual void s131(); virtual void s132(); virtual void s133(); virtual void s134(); virtual void s135();
    virtual void s136(); virtual void s137(); virtual void s138(); virtual void s139(); virtual void s140(); virtual void s141(); virtual void s142(); virtual void s143();
    virtual void s144(); virtual void s145(); virtual void s146(); virtual void s147(); virtual void s148(); virtual void s149(); virtual void s150(); virtual void s151();
    virtual void s152(); virtual void s153(); virtual void s154(); virtual void s155(); virtual void s156(); virtual void s157(); virtual void s158(); virtual void s159();
    virtual void s160(); virtual void s161(); virtual void s162(); virtual void s163(); virtual void s164(); virtual void s165(); virtual void s166(); virtual void s167();
    virtual void s168(); virtual void s169(); virtual void s170(); virtual void s171(); virtual void s172(); virtual void s173(); virtual void s174(); virtual void s175();
    virtual void s176(); virtual void s177(); virtual void s178(); virtual void s179(); virtual void s180(); virtual void s181(); virtual void s182(); virtual void s183();
    virtual void s184(); virtual void s185(); virtual void s186(); virtual void s187(); virtual void s188(); virtual void s189(); virtual void s190(); virtual void s191();
    virtual void s192(); virtual void s193(); virtual void s194(); virtual void s195(); virtual void s196(); virtual void s197(); virtual void s198(); virtual void s199();
    virtual void s200(); virtual void s201(); virtual void s202(); virtual void s203(); virtual void s204(); virtual void s205(); virtual void s206(); virtual void s207();
    virtual void s208(); virtual void s209(); virtual void s210(); virtual void s211(); virtual void s212(); virtual void s213(); virtual void s214(); virtual void s215();
    virtual void s216(); virtual void s217(); virtual void s218(); virtual void s219(); virtual void s220(); virtual void s221(); virtual void s222(); virtual void s223();
    virtual void s224(); virtual void s225(); virtual void s226(); virtual void s227(); virtual void s228(); virtual void s229(); virtual void s230(); virtual void s231();
    virtual void s232(); virtual void s233(); virtual void s234(); virtual void s235(); virtual void s236(); virtual void s237(); virtual void s238(); virtual void s239();
    virtual void s240(); virtual void s241(); virtual void s242(); virtual void s243(); virtual void s244(); virtual void s245(); virtual void s246(); virtual void s247();
    virtual void s248(); virtual void s249(); virtual void s250(); virtual void s251(); virtual void s252(); virtual void s253(); virtual void s254(); virtual void s255();
    virtual void s256(); virtual void s257(); virtual void s258(); virtual void s259(); virtual void s260(); virtual void s261(); virtual void s262(); virtual void s263();
    virtual void s264(); virtual void s265(); virtual void s266(); virtual void s267(); virtual void s268(); virtual void s269(); virtual void s270(); virtual void s271();
    virtual void s272(); virtual void s273(); virtual void s274(); virtual void s275(); virtual void s276(); virtual void s277(); virtual void s278(); virtual void s279();
    virtual void s280(); virtual void s281(); virtual void s282(); virtual void s283(); virtual void s284(); virtual void s285(); virtual void s286(); virtual void s287();
    virtual void s288(); virtual void s289(); virtual void s290(); virtual void s291(); virtual void s292(); virtual void s293(); virtual void s294(); virtual void s295();
    virtual void s296(); virtual void s297(); virtual void s298(); virtual void s299(); virtual void s300(); virtual void s301(); virtual void s302(); virtual void s303();
    virtual void s304(); virtual void s305(); virtual void s306(); virtual void s307(); virtual void s308(); virtual void s309(); virtual void s310(); virtual void s311();
    virtual void s312(); virtual void s313(); virtual void s314(); virtual void s315(); virtual void s316(); virtual void s317(); virtual void s318(); virtual void s319();
    virtual void s320(); virtual void s321(); virtual void s322(); virtual void s323(); virtual void s324(); virtual void s325(); virtual void s326(); virtual void s327();
    virtual void s328(); virtual void s329(); virtual void s330(); virtual void s331(); virtual void s332(); virtual void s333(); virtual void s334(); virtual void s335();
    virtual void s336(); virtual void s337(); virtual void s338(); virtual void s339(); virtual void s340(); virtual void s341(); virtual void s342(); virtual void s343();
    virtual void s344(); virtual void s345(); virtual void s346(); virtual void s347(); virtual void s348(); virtual void s349(); virtual void s350(); virtual void s351();
    virtual void s352(); virtual void s353(); virtual void s354(); virtual void s355(); virtual void s356(); virtual void s357(); virtual void s358(); virtual void s359();
    virtual void s360(); virtual void s361(); virtual void s362(); virtual void s363(); virtual void s364(); virtual void s365(); virtual void s366(); virtual void s367();
    virtual void s368(); virtual void s369(); virtual void s370(); virtual void s371(); virtual void s372(); virtual void s373(); virtual void s374(); virtual void s375();
    virtual void s376(); virtual void s377(); virtual void s378(); virtual void s379(); virtual void s380(); virtual void s381(); virtual void s382(); virtual void s383();
    virtual void s384(); virtual void s385(); virtual void s386(); virtual void s387(); virtual void s388(); virtual void s389(); virtual void s390(); virtual void s391();
    virtual void s392(); virtual void s393(); virtual void s394(); virtual void s395(); virtual void s396(); virtual void s397(); virtual void s398(); virtual void s399();
    virtual void s400(); virtual void s401(); virtual void s402(); virtual void s403(); virtual void s404(); virtual void s405(); virtual void s406(); virtual void s407();
    virtual void s408(); virtual void s409(); virtual void s410(); virtual void s411(); virtual void s412(); virtual void s413(); virtual void s414(); virtual void s415();
    virtual void s416(); virtual void s417(); virtual void s418(); virtual void s419(); virtual void s420(); virtual void s421(); virtual void s422(); virtual void s423();
    virtual void s424(); virtual void s425(); virtual void s426(); virtual void s427(); virtual void s428(); virtual void s429(); virtual void s430(); virtual void s431();
    virtual void s432(); virtual void s433(); virtual void s434(); virtual void s435(); virtual void s436(); virtual void s437(); virtual void s438(); virtual void s439();
    virtual void s440(); virtual void s441(); virtual void s442(); virtual void s443(); virtual void s444(); virtual void s445(); virtual void s446(); virtual void s447();
    virtual void s448(); virtual void s449(); virtual void s450(); virtual void s451(); virtual void s452(); virtual void s453(); virtual void s454(); virtual void s455();
    virtual void s456(); virtual void s457(); virtual void s458(); virtual void s459(); virtual void s460(); virtual void s461(); virtual void s462(); virtual void s463();
    virtual void s464(); virtual void s465(); virtual void s466(); virtual void s467(); virtual void s468(); virtual void s469(); virtual void s470(); virtual void s471();
    virtual void s472(); virtual void s473(); virtual void s474(); virtual void s475(); virtual void s476(); virtual void s477(); virtual void s478(); virtual void s479();
    virtual void s480(); virtual void s481(); virtual void s482(); virtual void s483(); virtual void s484(); virtual void s485(); virtual void s486(); virtual void s487();
    virtual void s488(); virtual void s489(); virtual void s490(); virtual void s491(); virtual void s492(); virtual void s493(); virtual void s494(); virtual void s495();
    virtual void EntitySetAsScared(const CScriptThing& thing, bool value) const;   // slot 496 (+0x7C0)
};
class CEntityScriptBase {                               // 20 bytes (Ego_r)
public:
    virtual ~CEntityScriptBase();
    virtual void GetParentScript() const;               // slot 1
private:
    virtual void Init();                                // slot 2
public:
    CGameScriptInterface* Game;                         // +0x04 (CGameScriptInterfaceBase* in the PDB; the derived interface here)
    CScriptThing Me;                                    // +0x08
};
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_WaspBossScript {
public:
    class CWaspChaseWoman : public CEntityScriptBase {
    private:
        virtual void Init();
    public:
        CQ_WaspBossScript* ParentClass;                             // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
    };
};
void CQ_WaspBossScript::CWaspChaseWoman::Init()
{
    Game->EntitySetAsScared(Me, true);
}
}