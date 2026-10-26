// CQ_AmbushTradersScript::BanditHireSetup(const CScriptThing& bandit, CScriptGameResourceObjectScriptedThingBase& banditObject) -- a living bandit confirms the hire to the hero and finishes speaking; then the hire count rises, WatchForBanditSpeechMarkers is spawned for the bandit and its health bar is added
// Leaf: ?BanditHireSetup@CQ_AmbushTradersScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F081E0, 440 bytes, `ret 8`; Ego_r.pdb
//   ?BanditHireSetup@CQ_AmbushTradersScript@NScript@@QAEJABVCScriptThing@@AAVCScriptGameResourceObjectScriptedThingBase@@@Z.
// Callees: the object's slots 12 GetScriptThing (hidden result), 13 Speak(const CScriptThing&, const char*, method, bool x3),
//   26 IsPerformingScriptTask; interface slots 264 GetHealth (> 0.0f 0x0122DEDC), 70 (+0x118, returns the hero; manifest
//   "GetHero"), 7 NewScriptFrame; IsActiveThreadTerminating 0x00CB7940; operator new 0x00BFEA1A (0x48 bytes),
//   CSpawnedFuncBase(name, 0) 0x00CDD450, CSpawnedFunc1 vtable 0x012F7EBC, WatchForBanditSpeechMarkers 0x00F07B80,
//   CScriptThing copy 0x004ABE90 / out-of-line dtor 0x004AA840 (the by-value parameter inside the inlined ctor),
//   AddSpawnedFunction 0x00CB7E50 ("" region), AddBanditHealthBar 0x00F07A80 (its result is returned).
// Layout (retail = Ego_r.pdb - 0x14 for the script; quest data +0xD0 counter, unnamed).
// Shape: SpawnFunction takes the region as `const CCharString& region = CCharString("")`: the default-argument temporary is
//   built at the call site and gets its own frame slot (frame+4) as in retail; a `const char*` default converted inside
//   the inline helper lets VC put the string in the dead `bandit` argument slot (0x10 frame).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
struct CBaseClass {
    virtual ~CBaseClass();                                  // 0x0099A2E0
    CBaseClass();                                           // 0x0099A2D0
};
struct CCPPointerInfo {
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> struct CCountedPointer {
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
};
struct CScriptThingImpl;
struct CScriptThing : CBaseClass {                          // vtable 0x01238C8C, 12 bytes
    CCountedPointer<CScriptThingImpl> PImp;
    CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
    ~CScriptThing() { }
};
struct CScriptThingParam : CBaseClass {                     // the spawned function's by-value parameter: same object, out-of-line dtor
    CCountedPointer<CScriptThingImpl> PImp;
    CScriptThingParam(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
    ~CScriptThingParam();                                   // 0x004AA840
};
struct CScriptThingStored : CBaseClass {
    CCountedPointer<CScriptThingImpl> PImp;
    CScriptThingStored(const CScriptThingParam& other);     // 0x004ABE90
};

enum ETextGroupSelectionMethod { TEXT_GROUP_SELECTION_2 = 2 };
struct CScriptGameResourceObjectScriptedThingBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual CScriptThing GetScriptThing() const;            // slot 12 (+0x30)
    virtual void Speak(const CScriptThing& listener, const char* text, ETextGroupSelectionMethod method, bool a, bool b, bool c);   // slot 13 (+0x34)
    virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25();
    virtual bool IsPerformingScriptTask() const;            // slot 26 (+0x68)
};
struct CGameScriptInterface {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c)
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual const CScriptThing& GetHero() const;            // slot 70 (+0x118)
    virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089();
    virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099();
    virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109();
    virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119();
    virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129();
    virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139();
    virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143(); virtual void v144(); virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149();
    virtual void v150(); virtual void v151(); virtual void v152(); virtual void v153(); virtual void v154(); virtual void v155(); virtual void v156(); virtual void v157(); virtual void v158(); virtual void v159();
    virtual void v160(); virtual void v161(); virtual void v162(); virtual void v163(); virtual void v164(); virtual void v165(); virtual void v166(); virtual void v167(); virtual void v168(); virtual void v169();
    virtual void v170(); virtual void v171(); virtual void v172(); virtual void v173(); virtual void v174(); virtual void v175(); virtual void v176(); virtual void v177(); virtual void v178(); virtual void v179();
    virtual void v180(); virtual void v181(); virtual void v182(); virtual void v183(); virtual void v184(); virtual void v185(); virtual void v186(); virtual void v187(); virtual void v188(); virtual void v189();
    virtual void v190(); virtual void v191(); virtual void v192(); virtual void v193(); virtual void v194(); virtual void v195(); virtual void v196(); virtual void v197(); virtual void v198(); virtual void v199();
    virtual void v200(); virtual void v201(); virtual void v202(); virtual void v203(); virtual void v204(); virtual void v205(); virtual void v206(); virtual void v207(); virtual void v208(); virtual void v209();
    virtual void v210(); virtual void v211(); virtual void v212(); virtual void v213(); virtual void v214(); virtual void v215(); virtual void v216(); virtual void v217(); virtual void v218(); virtual void v219();
    virtual void v220(); virtual void v221(); virtual void v222(); virtual void v223(); virtual void v224(); virtual void v225(); virtual void v226(); virtual void v227(); virtual void v228(); virtual void v229();
    virtual void v230(); virtual void v231(); virtual void v232(); virtual void v233(); virtual void v234(); virtual void v235(); virtual void v236(); virtual void v237(); virtual void v238(); virtual void v239();
    virtual void v240(); virtual void v241(); virtual void v242(); virtual void v243(); virtual void v244(); virtual void v245(); virtual void v246(); virtual void v247(); virtual void v248(); virtual void v249();
    virtual void v250(); virtual void v251(); virtual void v252(); virtual void v253(); virtual void v254(); virtual void v255(); virtual void v256(); virtual void v257(); virtual void v258(); virtual void v259();
    virtual void v260(); virtual void v261(); virtual void v262(); virtual void v263();
    virtual float GetHealth(const CScriptThing& thing) const;   // slot 264 (+0x420)
};

struct CSpawnedFuncBase {                                    // 0x34 bytes
    CSpawnedFuncBase(const CCharString& name, unsigned long id);   // 0x00CDD450
    virtual void v00();
    unsigned char _body[0x30];
};
template <class C, class P> struct CSpawnedFunc1 : CSpawnedFuncBase {
    typedef void (C::*TFunc)(P);
    TFunc               Func;                              // +0x34
    C*                  PClass;                            // +0x38
    CScriptThingStored  Par1;                              // +0x3c
    virtual void v00();
    __forceinline CSpawnedFunc1(const CCharString& name, unsigned long id, TFunc func, C* owner, CScriptThingParam par)
        : CSpawnedFuncBase(name, id), Func(func), PClass(owner), Par1(par) { }
};

struct CQuestData {
    unsigned char _pad00[0xd0];
    long          BanditsHired;                            // +0xd0
};
struct CScriptBase {                                        // retail 0x40 bytes
    virtual void v00();
    unsigned char _body[0x3c];
    bool IsActiveThreadTerminating();                       // 0x00CB7940
    void Add(CSpawnedFuncBase* func, const CCharString& name);   // 0x00CB7E50
    __forceinline void SpawnFunction(CSpawnedFuncBase* func, const CCharString& region = CCharString("")) { Add(func, region); }
};
struct CQuestScriptBase : CScriptBase {
    CGameScriptInterface* Game;                             // +0x40
    CQuestData*           Data;                             // +0x44
};
namespace NScript {
class CQ_AmbushTradersScript : public CQuestScriptBase {
public:
    void WatchForBanditSpeechMarkers(CScriptThingParam bandit);   // 0x00F07B80
    long AddBanditHealthBar(const CScriptThing& bandit);    // 0x00F07A80
    long BanditHireSetup(const CScriptThing& bandit, CScriptGameResourceObjectScriptedThingBase& banditObject);
};
} // namespace NScript
using NScript::CQ_AmbushTradersScript;

long NScript::CQ_AmbushTradersScript::BanditHireSetup(const CScriptThing& bandit, CScriptGameResourceObjectScriptedThingBase& banditObject)
{
    if (Game->GetHealth(banditObject.GetScriptThing()) > 0.0f) {
        banditObject.Speak(Game->GetHero(), "TEXT_QST_003_BANDIT_HIRE_CONFIRM", TEXT_GROUP_SELECTION_2, false, true, false);
        while (banditObject.IsPerformingScriptTask()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return 0;
        }
        if (IsActiveThreadTerminating())
            return 0;
    }
    ++Data->BanditsHired;
    SpawnFunction(new CSpawnedFunc1<CQ_AmbushTradersScript, CScriptThingParam>("WatchForBanditSpeechMarkers", 0,
        &NScript::CQ_AmbushTradersScript::WatchForBanditSpeechMarkers, this, bandit));
    return AddBanditHealthBar(bandit);
}