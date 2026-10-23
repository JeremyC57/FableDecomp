#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __cdecl
#define __declspec(x)
typedef long long __int64;
#endif
// behaviour test: ShowTargetedThingsHealth swaps the health bar on a new living target and removes it when the target dies


struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
void operator delete(void* p);                          // 0x00BFE9BC
struct CCPPointerInfo {
    long RefCount;                                      // +0
    void (__fastcall* DeleteFunc)(void* data);          // +4
    void* Data;                                         // +8
};
struct CCountedPointer {
    void*           Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
    void Release()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;              // the source's fields are read first
        void* data = other.Data;
        if (Info != info) {
            Release();
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
    ~CCountedPointer()
    {
        Release();
        Data = 0;
        Info = 0;
    }
};
struct CScriptThingImp {
    virtual void i000() const {} virtual void i001() const {} virtual void i002() const {} virtual void i003() const {} virtual void i004() const {} virtual void i005() const {}
    virtual void i006() const {} virtual void i007() const {} virtual void i008() const {} virtual void i009() const {} virtual void i010() const {} virtual void i011() const {}
    virtual void i012() const {} virtual void i013() const {} virtual void i014() const {} virtual void i015() const {} virtual void i016() const {} virtual void i017() const {}
    virtual void i018() const {} virtual void i019() const {} virtual void i020() const {} virtual void i021() const {} virtual void i022() const {} virtual void i023() const {}
    virtual void i024() const {} virtual void i025() const {} virtual void i026() const {} virtual void i027() const {} virtual void i028() const {} virtual void i029() const {}
    virtual void i030() const {} virtual void i031() const {} virtual void i032() const {} virtual void i033() const {} virtual void i034() const {} virtual void i035() const {}
    virtual void i036() const {} virtual void i037() const {} virtual void i038() const {} virtual void i039() const {} virtual void i040() const {} virtual void i041() const {}
    virtual void i042() const {} virtual void i043() const {} virtual void i044() const {} virtual void i045() const {} virtual void i046() const {} virtual void i047() const {}
    virtual void i048() const {} virtual void i049() const {} virtual void i050() const {} virtual void i051() const {} virtual void i052() const {} virtual void i053() const {}
    virtual void i054() const {} virtual void i055() const {} virtual void i056() const {} virtual void i057() const {} virtual void i058() const {} virtual void i059() const {}
    virtual void i060() const {} virtual void i061() const {} virtual void i062() const {} virtual void i063() const {} virtual void i064() const {} virtual void i065() const {}
    virtual void i066() const {} virtual void i067() const {} virtual void i068() const {} virtual void i069() const {} virtual void i070() const {} virtual void i071() const {}
    virtual void i072() const {} virtual void i073() const {} virtual void i074() const {}
    virtual bool IsAlive() const;                                       // slot 75 (+0x12c)
    virtual void i076() const {} virtual void i077() const {}
    virtual bool IsEqualTo(const CScriptThingImp* other) const;         // slot 78 (+0x138)
};
struct CBase {
    CBase();                                            // 0x0099A2D0
    virtual ~CBase();                                   // 0x0099A2E0
};
struct CScriptThing : CBase {
    virtual void s001() const {} virtual void s002() const {} virtual void s003() const {} virtual void s004() const {} virtual void s005() const {} virtual void s006() const {}
    virtual void s007() const {} virtual void s008() const {} virtual void s009() const {} virtual void s010() const {} virtual void s011() const {} virtual void s012() const {}
    virtual void s013() const {} virtual void s014() const {} virtual void s015() const {} virtual void s016() const {} virtual void s017() const {} virtual void s018() const {}
    virtual void s019() const {} virtual void s020() const {} virtual void s021() const {} virtual void s022() const {} virtual void s023() const {} virtual void s024() const {}
    virtual void s025() const {} virtual void s026() const {} virtual void s027() const {} virtual void s028() const {} virtual void s029() const {} virtual void s030() const {}
    virtual void s031() const {} virtual void s032() const {} virtual void s033() const {} virtual void s034() const {} virtual void s035() const {} virtual void s036() const {}
    virtual void s037() const {} virtual void s038() const {} virtual void s039() const {} virtual void s040() const {} virtual void s041() const {} virtual void s042() const {}
    virtual void s043() const {} virtual void s044() const {} virtual void s045() const {} virtual void s046() const {} virtual void s047() const {} virtual void s048() const {}
    virtual void s049() const {} virtual void s050() const {} virtual void s051() const {} virtual void s052() const {} virtual void s053() const {} virtual void s054() const {}
    virtual void s055() const {} virtual void s056() const {} virtual void s057() const {} virtual void s058() const {} virtual void s059() const {} virtual void s060() const {}
    virtual void s061() const {} virtual void s062() const {} virtual void s063() const {} virtual void s064() const {} virtual void s065() const {} virtual void s066() const {}
    virtual void s067() const {} virtual void s068() const {} virtual void s069() const {} virtual void s070() const {} virtual void s071() const {} virtual void s072() const {}
    virtual void s073() const {} virtual void s074() const {}
    virtual bool IsAlive() const { return PImp.Data && Imp()->IsAlive(); }   // slot 75 (+0x12c) 0x004AB130
    virtual bool IsDead() const { return !IsAlive(); }                       // slot 76 (+0x130) 0x004AA890
    CCountedPointer PImp;                               // +0x04 Data, +0x08 Info
    CScriptThing() {}
    CScriptThing(const CScriptThing& o) : CBase() { PImp = o.PImp; }
    __forceinline ~CScriptThing() {}                    // out-of-line copy 0x004AA840
    const CScriptThingImp* Imp() const { return (const CScriptThingImp*)PImp.Data; }
};
inline bool operator==(const CScriptThing& a, const CScriptThing& b)
{
    return a.Imp()->IsEqualTo(b.Imp());
}
struct CGameScriptInterface {
    virtual void v000() const {} virtual void v001() const {} virtual void v002() const {} virtual void v003() const {} virtual void v004() const {} virtual void v005() const {}
    virtual void v006() const {}
    virtual void NewScriptFrame() const;                                // slot 7 (+0x1c)
    virtual void v008() const {} virtual void v009() const {} virtual void v010() const {} virtual void v011() const {} virtual void v012() const {} virtual void v013() const {}
    virtual void v014() const {} virtual void v015() const {} virtual void v016() const {} virtual void v017() const {} virtual void v018() const {} virtual void v019() const {}
    virtual void v020() const {} virtual void v021() const {} virtual void v022() const {} virtual void v023() const {} virtual void v024() const {} virtual void v025() const {}
    virtual void v026() const {} virtual void v027() const {} virtual void v028() const {} virtual void v029() const {} virtual void v030() const {} virtual void v031() const {}
    virtual void v032() const {} virtual void v033() const {} virtual void v034() const {} virtual void v035() const {} virtual void v036() const {} virtual void v037() const {}
    virtual void v038() const {} virtual void v039() const {} virtual void v040() const {} virtual void v041() const {} virtual void v042() const {} virtual void v043() const {}
    virtual void v044() const {} virtual void v045() const {} virtual void v046() const {} virtual void v047() const {} virtual void v048() const {} virtual void v049() const {}
    virtual void v050() const {} virtual void v051() const {} virtual void v052() const {} virtual void v053() const {} virtual void v054() const {} virtual void v055() const {}
    virtual void v056() const {} virtual void v057() const {} virtual void v058() const {} virtual void v059() const {} virtual void v060() const {} virtual void v061() const {}
    virtual void v062() const {} virtual void v063() const {} virtual void v064() const {} virtual void v065() const {} virtual void v066() const {} virtual void v067() const {}
    virtual void v068() const {} virtual void v069() const {} virtual void v070() const {}
    virtual CScriptThing GetHeroTargetedThing() const;                  // slot 71 (+0x11c)
    virtual void v072() const {} virtual void v073() const {} virtual void v074() const {} virtual void v075() const {} virtual void v076() const {} virtual void v077() const {}
    virtual void v078() const {} virtual void v079() const {} virtual void v080() const {} virtual void v081() const {} virtual void v082() const {} virtual void v083() const {}
    virtual void v084() const {} virtual void v085() const {} virtual void v086() const {} virtual void v087() const {} virtual void v088() const {} virtual void v089() const {}
    virtual void v090() const {} virtual void v091() const {} virtual void v092() const {} virtual void v093() const {} virtual void v094() const {} virtual void v095() const {}
    virtual void v096() const {} virtual void v097() const {} virtual void v098() const {} virtual void v099() const {} virtual void v100() const {} virtual void v101() const {}
    virtual void v102() const {} virtual void v103() const {} virtual void v104() const {} virtual void v105() const {} virtual void v106() const {} virtual void v107() const {}
    virtual void v108() const {} virtual void v109() const {} virtual void v110() const {} virtual void v111() const {} virtual void v112() const {} virtual void v113() const {}
    virtual void v114() const {} virtual void v115() const {} virtual void v116() const {} virtual void v117() const {} virtual void v118() const {} virtual void v119() const {}
    virtual void v120() const {} virtual void v121() const {} virtual void v122() const {} virtual void v123() const {} virtual void v124() const {} virtual void v125() const {}
    virtual void v126() const {} virtual void v127() const {} virtual void v128() const {} virtual void v129() const {} virtual void v130() const {} virtual void v131() const {}
    virtual void v132() const {} virtual void v133() const {} virtual void v134() const {} virtual void v135() const {} virtual void v136() const {} virtual void v137() const {}
    virtual void v138() const {} virtual void v139() const {} virtual void v140() const {} virtual void v141() const {} virtual void v142() const {} virtual void v143() const {}
    virtual void v144() const {} virtual void v145() const {} virtual void v146() const {} virtual void v147() const {} virtual void v148() const {} virtual void v149() const {}
    virtual void v150() const {} virtual void v151() const {} virtual void v152() const {} virtual void v153() const {} virtual void v154() const {} virtual void v155() const {}
    virtual void v156() const {} virtual void v157() const {} virtual void v158() const {} virtual void v159() const {} virtual void v160() const {} virtual void v161() const {}
    virtual void v162() const {} virtual void v163() const {} virtual void v164() const {} virtual void v165() const {} virtual void v166() const {} virtual void v167() const {}
    virtual void v168() const {} virtual void v169() const {} virtual void v170() const {} virtual void v171() const {} virtual void v172() const {} virtual void v173() const {}
    virtual void v174() const {} virtual void v175() const {} virtual void v176() const {} virtual void v177() const {} virtual void v178() const {} virtual void v179() const {}
    virtual void v180() const {} virtual void v181() const {} virtual void v182() const {} virtual void v183() const {} virtual void v184() const {} virtual void v185() const {}
    virtual void v186() const {} virtual void v187() const {} virtual void v188() const {} virtual void v189() const {} virtual void v190() const {} virtual void v191() const {}
    virtual void v192() const {} virtual void v193() const {} virtual void v194() const {} virtual void v195() const {} virtual void v196() const {} virtual void v197() const {}
    virtual void v198() const {} virtual void v199() const {} virtual void v200() const {} virtual void v201() const {} virtual void v202() const {} virtual void v203() const {}
    virtual void v204() const {} virtual void v205() const {} virtual void v206() const {} virtual void v207() const {} virtual void v208() const {} virtual void v209() const {}
    virtual void v210() const {} virtual void v211() const {} virtual void v212() const {} virtual void v213() const {} virtual void v214() const {} virtual void v215() const {}
    virtual void v216() const {} virtual void v217() const {} virtual void v218() const {} virtual void v219() const {} virtual void v220() const {} virtual void v221() const {}
    virtual void v222() const {} virtual void v223() const {} virtual void v224() const {} virtual void v225() const {} virtual void v226() const {} virtual void v227() const {}
    virtual void v228() const {} virtual void v229() const {} virtual void v230() const {} virtual void v231() const {} virtual void v232() const {} virtual void v233() const {}
    virtual void v234() const {} virtual void v235() const {} virtual void v236() const {} virtual void v237() const {} virtual void v238() const {} virtual void v239() const {}
    virtual void v240() const {} virtual void v241() const {} virtual void v242() const {} virtual void v243() const {} virtual void v244() const {} virtual void v245() const {}
    virtual void v246() const {} virtual void v247() const {} virtual void v248() const {} virtual void v249() const {} virtual void v250() const {} virtual void v251() const {}
    virtual void v252() const {} virtual void v253() const {} virtual void v254() const {} virtual void v255() const {} virtual void v256() const {} virtual void v257() const {}
    virtual void v258() const {} virtual void v259() const {} virtual void v260() const {} virtual void v261() const {} virtual void v262() const {} virtual void v263() const {}
    virtual void v264() const {} virtual void v265() const {} virtual void v266() const {} virtual void v267() const {} virtual void v268() const {} virtual void v269() const {}
    virtual void v270() const {} virtual void v271() const {} virtual void v272() const {} virtual void v273() const {} virtual void v274() const {} virtual void v275() const {}
    virtual void v276() const {} virtual void v277() const {} virtual void v278() const {} virtual void v279() const {} virtual void v280() const {} virtual void v281() const {}
    virtual void v282() const {} virtual void v283() const {} virtual void v284() const {} virtual void v285() const {} virtual void v286() const {} virtual void v287() const {}
    virtual void v288() const {} virtual void v289() const {} virtual void v290() const {} virtual void v291() const {} virtual void v292() const {} virtual void v293() const {}
    virtual void v294() const {} virtual void v295() const {} virtual void v296() const {} virtual void v297() const {} virtual void v298() const {} virtual void v299() const {}
    virtual void v300() const {} virtual void v301() const {} virtual void v302() const {} virtual void v303() const {} virtual void v304() const {} virtual void v305() const {}
    virtual void v306() const {} virtual void v307() const {} virtual void v308() const {} virtual void v309() const {} virtual void v310() const {} virtual void v311() const {}
    virtual void v312() const {} virtual void v313() const {} virtual void v314() const {} virtual void v315() const {} virtual void v316() const {} virtual void v317() const {}
    virtual void v318() const {} virtual void v319() const {} virtual void v320() const {}
    virtual void DisplayQuestInfo(bool show) const;                     // slot 321 (+0x504)
    virtual void v322() const {} virtual void v323() const {} virtual void v324() const {}
    virtual long AddQuestInfoBarHealth(const CScriptThing& thing, const CRGBColour& colour, const CCharString& name, float scale) const; // slot 325 (+0x514)
    virtual void v326() const {} virtual void v327() const {} virtual void v328() const {} virtual void v329() const {} virtual void v330() const {} virtual void v331() const {}
    virtual void v332() const {} virtual void v333() const {} virtual void v334() const {} virtual void v335() const {} virtual void v336() const {} virtual void v337() const {}
    virtual void RemoveQuestInfoElement(long element) const;            // slot 338 (+0x548)
};
struct CScriptBase {                                    // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                   // 0x00CB7940
};
struct CShowTargetedThingHealthScript : CScriptBase {
    CGameScriptInterface* GSI;                          // +0x40
    void ShowTargetedThingsHealth();
};

void CShowTargetedThingHealthScript::ShowTargetedThingsHealth()
{
    CScriptThing target;
    CScriptThing previous;
    long healthBar = -1;
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        target = GSI->GetHeroTargetedThing();
        if ((target.IsAlive() && previous.IsAlive() && !(target == previous)) || (target.IsAlive() && previous.IsDead())) {
            if (IsActiveThreadTerminating())
                return;
            if (healthBar >= 0) {
#pragma inline_depth(0)
                if (IsActiveThreadTerminating())
                    return;
#pragma inline_depth()
                GSI->RemoveQuestInfoElement(healthBar);
            }
            GSI->DisplayQuestInfo(true);
            healthBar = GSI->AddQuestInfoBarHealth(target, CRGBColour(255, 255, 255, 255), "", 1.0f);
        } else if (previous.IsAlive() && target.IsDead()) {
#pragma inline_depth(0)
            if (IsActiveThreadTerminating())
                return;
#pragma inline_depth()
            GSI->RemoveQuestInfoElement(healthBar);
        }
        previous = target;
        GSI->NewScriptFrame();
    }
}

#include <cstdlib>
CCharString::CCharString(const char* t, long) { PStringData = (void*)t; }
CCharString::~CCharString() {}
void operator delete(void* p) { std::free(p); }
CBase::CBase() {}
CBase::~CBase() {}
struct TestImp : CScriptThingImp { int Id; bool Alive; };
bool CScriptThingImp::IsAlive() const { return static_cast<const TestImp*>(this)->Alive; }
bool CScriptThingImp::IsEqualTo(const CScriptThingImp* o) const { return o && static_cast<const TestImp*>(this)->Id == static_cast<const TestImp*>(o)->Id; }
static int g_deletes = 0;
static void __fastcall DeleteImp(void*) { ++g_deletes; }
static TestImp g_x, g_y;
static int g_frame = 0, g_checks = 0, g_terminateAfter = 0;
bool CScriptBase::IsActiveThreadTerminating() { ++g_checks; return g_frame >= g_terminateAfter; }
void CGameScriptInterface::NewScriptFrame() const { ++g_frame; }
static TestImp* g_seq[8] = { 0, &g_x, &g_x, &g_y, 0, 0, 0, 0 };
static CCPPointerInfo* g_infos[8];
CScriptThing CGameScriptInterface::GetHeroTargetedThing() const
{
    CScriptThing t;
    TestImp* imp = g_seq[g_frame];
    if (imp) {
        CCPPointerInfo* info = (CCPPointerInfo*)std::malloc(sizeof(CCPPointerInfo));
        info->RefCount = 1; info->DeleteFunc = DeleteImp; info->Data = imp;
        t.PImp.Data = imp; t.PImp.Info = info;
        g_infos[g_frame] = info;
    }
    return t;
}
static int g_log[32]; static int g_n = 0;
static void Log(int v) { if (g_n < 32) g_log[g_n++] = v; }
static long g_nextBar = 100;
void CGameScriptInterface::DisplayQuestInfo(bool show) const { Log(show ? 1 : 0); }
long CGameScriptInterface::AddQuestInfoBarHealth(const CScriptThing& t, const CRGBColour& c, const CCharString&, float s) const
{
    Log(1000 + static_cast<const TestImp*>(t.Imp())->Id);
    if (c.R != 255 || c.G != 255 || c.B != 255 || c.A != 255 || s != 1.0f) Log(-1);
    return g_nextBar++;
}
void CGameScriptInterface::RemoveQuestInfoElement(long e) const { Log(2000 + (int)e); }
static int fails = 0;
static void Check(bool c, int line) { if (!c) { std::printf("fail line %d\n", line); ++fails; } }
int main()
{
    g_x.Id = 1; g_x.Alive = true; g_y.Id = 2; g_y.Alive = true;
    static CGameScriptInterface game;
    static CShowTargetedThingHealthScript script; script.GSI = &game;
    g_terminateAfter = 5;           // five iterations
    script.ShowTargetedThingsHealth();
    // iter 2: X shown; iter 4: bar 100 removed, Y shown; iter 5: Y lost while alive -> bar 101 removed
    Check(g_n == 6, __LINE__);
    Check(g_log[0] == 1 && g_log[1] == 1001 && g_log[2] == 2100 && g_log[3] == 1 && g_log[4] == 1002 && g_log[5] == 2101, __LINE__);
    Check(g_frame == 5, __LINE__);
    // every counted reference released: X's two handles and Y's one
    Check(g_deletes == 3, __LINE__);
    // a dead target never gets a bar
    g_n = 0; g_frame = 0; g_checks = 0; g_deletes = 0; g_x.Alive = false; g_seq[3] = &g_x;
    script.ShowTargetedThingsHealth();
    Check(g_n == 0 && g_deletes == 3, __LINE__);
    if (fails) { std::printf("00eff930_TEST FAIL\n"); return 1; }
    std::printf("00eff930_TEST PASS\n");
    return 0;
}