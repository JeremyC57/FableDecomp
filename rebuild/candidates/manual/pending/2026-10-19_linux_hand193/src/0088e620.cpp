// CGameScriptInterface::EntitySetFacingAngleTowardsThing(const CScriptThing& entity, const CScriptThing& target, bool
// immediate) const -- turn the entity to face the target: the XY bearing between their positions as a fraction of a
// full turn, wrapped into [0, 1), handed to EntitySetFacingAngle
// Leaf: ?EntitySetFacingAngleTowardsThing@CGameScriptInterface
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0088E620, length 208 bytes, `ret 0Ch`.
// Callees: CScriptThing virtual slot 75 IsValid (+0x12C) and slot 6 GetPos (+0x18, const C3DVector&), the CRT
// `_CIfmod` @ 0x00BFEB28 (fmod intrinsic), CGameScriptInterface virtual slot 474 EntitySetFacingAngle (+0x768).
// Constants: 0x01237F08 = 1/(2*pi) (float), 0x0122DEDC = 0.0f, 0x0122DED8 = 1.0f, 0x01237F00 = 1.0 (double).
// Shape: nested validity tests (target first); atan2 is the `fpatan` intrinsic over (dx, dy); the angle is a named
// float local homed in the dead `target` argument slot; the wrap is `if (a < 0 || a >= 1) { a = fmod(a, 1.0);
// if (a < 0) a += 1; }`.
#include <math.h>
#pragma intrinsic(atan2, fmod)

struct C3DVector { float X, Y, Z; };

struct CScriptThing {
    virtual void s000() const; virtual void s001() const; virtual void s002() const; virtual void s003() const;
    virtual void s004() const; virtual void s005() const;
    virtual const C3DVector& GetPos() const;       // slot 6 (+0x18)
    virtual void s007() const; 
    virtual void s008() const; virtual void s009() const; virtual void s010() const; virtual void s011() const; virtual void s012() const; virtual void s013() const; virtual void s014() const; virtual void s015() const; 
    virtual void s016() const; virtual void s017() const; virtual void s018() const; virtual void s019() const; virtual void s020() const; virtual void s021() const; virtual void s022() const; virtual void s023() const; 
    virtual void s024() const; virtual void s025() const; virtual void s026() const; virtual void s027() const; virtual void s028() const; virtual void s029() const; virtual void s030() const; virtual void s031() const; 
    virtual void s032() const; virtual void s033() const; virtual void s034() const; virtual void s035() const; virtual void s036() const; virtual void s037() const; virtual void s038() const; virtual void s039() const; 
    virtual void s040() const; virtual void s041() const; virtual void s042() const; virtual void s043() const; virtual void s044() const; virtual void s045() const; virtual void s046() const; virtual void s047() const; 
    virtual void s048() const; virtual void s049() const; virtual void s050() const; virtual void s051() const; virtual void s052() const; virtual void s053() const; virtual void s054() const; virtual void s055() const; 
    virtual void s056() const; virtual void s057() const; virtual void s058() const; virtual void s059() const; virtual void s060() const; virtual void s061() const; virtual void s062() const; virtual void s063() const; 
    virtual void s064() const; virtual void s065() const; virtual void s066() const; virtual void s067() const; virtual void s068() const; virtual void s069() const; virtual void s070() const; virtual void s071() const; 
    virtual void s072() const; virtual void s073() const; virtual void s074() const; 
    virtual bool IsValid() const;                  // slot 75 (+0x12c)
};

struct CGameScriptInterface {
    virtual void g000() const; virtual void g001() const; virtual void g002() const; virtual void g003() const; virtual void g004() const; virtual void g005() const; virtual void g006() const; virtual void g007() const; 
    virtual void g008() const; virtual void g009() const; virtual void g010() const; virtual void g011() const; virtual void g012() const; virtual void g013() const; virtual void g014() const; virtual void g015() const; 
    virtual void g016() const; virtual void g017() const; virtual void g018() const; virtual void g019() const; virtual void g020() const; virtual void g021() const; virtual void g022() const; virtual void g023() const; 
    virtual void g024() const; virtual void g025() const; virtual void g026() const; virtual void g027() const; virtual void g028() const; virtual void g029() const; virtual void g030() const; virtual void g031() const; 
    virtual void g032() const; virtual void g033() const; virtual void g034() const; virtual void g035() const; virtual void g036() const; virtual void g037() const; virtual void g038() const; virtual void g039() const; 
    virtual void g040() const; virtual void g041() const; virtual void g042() const; virtual void g043() const; virtual void g044() const; virtual void g045() const; virtual void g046() const; virtual void g047() const; 
    virtual void g048() const; virtual void g049() const; virtual void g050() const; virtual void g051() const; virtual void g052() const; virtual void g053() const; virtual void g054() const; virtual void g055() const; 
    virtual void g056() const; virtual void g057() const; virtual void g058() const; virtual void g059() const; virtual void g060() const; virtual void g061() const; virtual void g062() const; virtual void g063() const; 
    virtual void g064() const; virtual void g065() const; virtual void g066() const; virtual void g067() const; virtual void g068() const; virtual void g069() const; virtual void g070() const; virtual void g071() const; 
    virtual void g072() const; virtual void g073() const; virtual void g074() const; virtual void g075() const; virtual void g076() const; virtual void g077() const; virtual void g078() const; virtual void g079() const; 
    virtual void g080() const; virtual void g081() const; virtual void g082() const; virtual void g083() const; virtual void g084() const; virtual void g085() const; virtual void g086() const; virtual void g087() const; 
    virtual void g088() const; virtual void g089() const; virtual void g090() const; virtual void g091() const; virtual void g092() const; virtual void g093() const; virtual void g094() const; virtual void g095() const; 
    virtual void g096() const; virtual void g097() const; virtual void g098() const; virtual void g099() const; virtual void g100() const; virtual void g101() const; virtual void g102() const; virtual void g103() const; 
    virtual void g104() const; virtual void g105() const; virtual void g106() const; virtual void g107() const; virtual void g108() const; virtual void g109() const; virtual void g110() const; virtual void g111() const; 
    virtual void g112() const; virtual void g113() const; virtual void g114() const; virtual void g115() const; virtual void g116() const; virtual void g117() const; virtual void g118() const; virtual void g119() const; 
    virtual void g120() const; virtual void g121() const; virtual void g122() const; virtual void g123() const; virtual void g124() const; virtual void g125() const; virtual void g126() const; virtual void g127() const; 
    virtual void g128() const; virtual void g129() const; virtual void g130() const; virtual void g131() const; virtual void g132() const; virtual void g133() const; virtual void g134() const; virtual void g135() const; 
    virtual void g136() const; virtual void g137() const; virtual void g138() const; virtual void g139() const; virtual void g140() const; virtual void g141() const; virtual void g142() const; virtual void g143() const; 
    virtual void g144() const; virtual void g145() const; virtual void g146() const; virtual void g147() const; virtual void g148() const; virtual void g149() const; virtual void g150() const; virtual void g151() const; 
    virtual void g152() const; virtual void g153() const; virtual void g154() const; virtual void g155() const; virtual void g156() const; virtual void g157() const; virtual void g158() const; virtual void g159() const; 
    virtual void g160() const; virtual void g161() const; virtual void g162() const; virtual void g163() const; virtual void g164() const; virtual void g165() const; virtual void g166() const; virtual void g167() const; 
    virtual void g168() const; virtual void g169() const; virtual void g170() const; virtual void g171() const; virtual void g172() const; virtual void g173() const; virtual void g174() const; virtual void g175() const; 
    virtual void g176() const; virtual void g177() const; virtual void g178() const; virtual void g179() const; virtual void g180() const; virtual void g181() const; virtual void g182() const; virtual void g183() const; 
    virtual void g184() const; virtual void g185() const; virtual void g186() const; virtual void g187() const; virtual void g188() const; virtual void g189() const; virtual void g190() const; virtual void g191() const; 
    virtual void g192() const; virtual void g193() const; virtual void g194() const; virtual void g195() const; virtual void g196() const; virtual void g197() const; virtual void g198() const; virtual void g199() const; 
    virtual void g200() const; virtual void g201() const; virtual void g202() const; virtual void g203() const; virtual void g204() const; virtual void g205() const; virtual void g206() const; virtual void g207() const; 
    virtual void g208() const; virtual void g209() const; virtual void g210() const; virtual void g211() const; virtual void g212() const; virtual void g213() const; virtual void g214() const; virtual void g215() const; 
    virtual void g216() const; virtual void g217() const; virtual void g218() const; virtual void g219() const; virtual void g220() const; virtual void g221() const; virtual void g222() const; virtual void g223() const; 
    virtual void g224() const; virtual void g225() const; virtual void g226() const; virtual void g227() const; virtual void g228() const; virtual void g229() const; virtual void g230() const; virtual void g231() const; 
    virtual void g232() const; virtual void g233() const; virtual void g234() const; virtual void g235() const; virtual void g236() const; virtual void g237() const; virtual void g238() const; virtual void g239() const; 
    virtual void g240() const; virtual void g241() const; virtual void g242() const; virtual void g243() const; virtual void g244() const; virtual void g245() const; virtual void g246() const; virtual void g247() const; 
    virtual void g248() const; virtual void g249() const; virtual void g250() const; virtual void g251() const; virtual void g252() const; virtual void g253() const; virtual void g254() const; virtual void g255() const; 
    virtual void g256() const; virtual void g257() const; virtual void g258() const; virtual void g259() const; virtual void g260() const; virtual void g261() const; virtual void g262() const; virtual void g263() const; 
    virtual void g264() const; virtual void g265() const; virtual void g266() const; virtual void g267() const; virtual void g268() const; virtual void g269() const; virtual void g270() const; virtual void g271() const; 
    virtual void g272() const; virtual void g273() const; virtual void g274() const; virtual void g275() const; virtual void g276() const; virtual void g277() const; virtual void g278() const; virtual void g279() const; 
    virtual void g280() const; virtual void g281() const; virtual void g282() const; virtual void g283() const; virtual void g284() const; virtual void g285() const; virtual void g286() const; virtual void g287() const; 
    virtual void g288() const; virtual void g289() const; virtual void g290() const; virtual void g291() const; virtual void g292() const; virtual void g293() const; virtual void g294() const; virtual void g295() const; 
    virtual void g296() const; virtual void g297() const; virtual void g298() const; virtual void g299() const; virtual void g300() const; virtual void g301() const; virtual void g302() const; virtual void g303() const; 
    virtual void g304() const; virtual void g305() const; virtual void g306() const; virtual void g307() const; virtual void g308() const; virtual void g309() const; virtual void g310() const; virtual void g311() const; 
    virtual void g312() const; virtual void g313() const; virtual void g314() const; virtual void g315() const; virtual void g316() const; virtual void g317() const; virtual void g318() const; virtual void g319() const; 
    virtual void g320() const; virtual void g321() const; virtual void g322() const; virtual void g323() const; virtual void g324() const; virtual void g325() const; virtual void g326() const; virtual void g327() const; 
    virtual void g328() const; virtual void g329() const; virtual void g330() const; virtual void g331() const; virtual void g332() const; virtual void g333() const; virtual void g334() const; virtual void g335() const; 
    virtual void g336() const; virtual void g337() const; virtual void g338() const; virtual void g339() const; virtual void g340() const; virtual void g341() const; virtual void g342() const; virtual void g343() const; 
    virtual void g344() const; virtual void g345() const; virtual void g346() const; virtual void g347() const; virtual void g348() const; virtual void g349() const; virtual void g350() const; virtual void g351() const; 
    virtual void g352() const; virtual void g353() const; virtual void g354() const; virtual void g355() const; virtual void g356() const; virtual void g357() const; virtual void g358() const; virtual void g359() const; 
    virtual void g360() const; virtual void g361() const; virtual void g362() const; virtual void g363() const; virtual void g364() const; virtual void g365() const; virtual void g366() const; virtual void g367() const; 
    virtual void g368() const; virtual void g369() const; virtual void g370() const; virtual void g371() const; virtual void g372() const; virtual void g373() const; virtual void g374() const; virtual void g375() const; 
    virtual void g376() const; virtual void g377() const; virtual void g378() const; virtual void g379() const; virtual void g380() const; virtual void g381() const; virtual void g382() const; virtual void g383() const; 
    virtual void g384() const; virtual void g385() const; virtual void g386() const; virtual void g387() const; virtual void g388() const; virtual void g389() const; virtual void g390() const; virtual void g391() const; 
    virtual void g392() const; virtual void g393() const; virtual void g394() const; virtual void g395() const; virtual void g396() const; virtual void g397() const; virtual void g398() const; virtual void g399() const; 
    virtual void g400() const; virtual void g401() const; virtual void g402() const; virtual void g403() const; virtual void g404() const; virtual void g405() const; virtual void g406() const; virtual void g407() const; 
    virtual void g408() const; virtual void g409() const; virtual void g410() const; virtual void g411() const; virtual void g412() const; virtual void g413() const; virtual void g414() const; virtual void g415() const; 
    virtual void g416() const; virtual void g417() const; virtual void g418() const; virtual void g419() const; virtual void g420() const; virtual void g421() const; virtual void g422() const; virtual void g423() const; 
    virtual void g424() const; virtual void g425() const; virtual void g426() const; virtual void g427() const; virtual void g428() const; virtual void g429() const; virtual void g430() const; virtual void g431() const; 
    virtual void g432() const; virtual void g433() const; virtual void g434() const; virtual void g435() const; virtual void g436() const; virtual void g437() const; virtual void g438() const; virtual void g439() const; 
    virtual void g440() const; virtual void g441() const; virtual void g442() const; virtual void g443() const; virtual void g444() const; virtual void g445() const; virtual void g446() const; virtual void g447() const; 
    virtual void g448() const; virtual void g449() const; virtual void g450() const; virtual void g451() const; virtual void g452() const; virtual void g453() const; virtual void g454() const; virtual void g455() const; 
    virtual void g456() const; virtual void g457() const; virtual void g458() const; virtual void g459() const; virtual void g460() const; virtual void g461() const; virtual void g462() const; virtual void g463() const; 
    virtual void g464() const; virtual void g465() const; virtual void g466() const; virtual void g467() const; virtual void g468() const; virtual void g469() const; virtual void g470() const; virtual void g471() const; 
    virtual void g472() const; virtual void g473() const; 
    virtual void EntitySetFacingAngle(const CScriptThing& entity, float angle, bool immediate) const;   // slot 474 (+0x768)
    virtual void EntitySetFacingAngleTowardsThing(const CScriptThing& entity, const CScriptThing& target, bool immediate) const;
};

void CGameScriptInterface::EntitySetFacingAngleTowardsThing(const CScriptThing& entity, const CScriptThing& target, bool immediate) const
{
    if (target.IsValid()) {
        if (entity.IsValid()) {
            const C3DVector& from = entity.GetPos();
            const C3DVector& to = target.GetPos();
            float angle = (float)atan2(to.X - from.X, to.Y - from.Y) * 0.15915494f;
            if (angle < 0.0f || angle >= 1.0f) {
                angle = (float)fmod((double)angle, 1.0);
                if (angle < 0.0f)
                    angle += 1.0f;
            }
            EntitySetFacingAngle(entity, angle, immediate);
        }
    }
}
