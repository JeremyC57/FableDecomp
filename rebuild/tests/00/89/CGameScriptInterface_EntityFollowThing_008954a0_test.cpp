// Behaviour test for 008954a0: src/008954a0.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CGameScriptInterface::EntityFollowThing(const CScriptThing& follower, const CScriptThing& target, float distance, bool keepUp) const
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CGameScriptInterface::EntityFollowThing(const CScriptThing& follower, const CScriptThing& target, float distance, bool keepUp) const -- when both script things are alive, let the follower follow the hero and post an EXPRESSION_FOLLOW "expression performed to" event from the target to the follower's entity-event component
// Leaf: EntityFollowThing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008954A0, length 274 bytes (+ int3 padding), `ret 10h`.
// Signature from Ego_r.pdb: ?EntityFollowThing@CGameScriptInterface@@UBEXABVCScriptThing@@0M_N@Z.
// Callees: CScriptThing slots 75 IsAlive and 11 GetThing, CGameDefinitionManager::Get @ 0x0044C6B0 (__fastcall),
//   CDefinitionManager::GetDef<CExpressionDef>(const CCharString&, CDefPointer*) @ 0x00593666, CCharString(const char*,
//   long) @ 0x0099EBF0 ("EXPRESSION_FOLLOW" @ 0x012757EC) / ~CCharString @ 0x0099EAE0, the TC map lower bound @ 0x0040F020,
//   this interface's slot 235 EntitySetAsAllowedToFollowHero (cgsi_slots_aligned.tsv), operator new @ 0x00BFEA1A (0x20),
//   __ftol2 @ 0x00BFEA70, NEntityEvents::CEventExpressionPerformedTo(CThingCreatureBase*, long, const CDefPointer&, long,
//   long) @ 0x0084B010 (landed), CTCEntityEvents::AddEvent @ 0x008113C0, the def's slot 1 on release.
// Layout: retail CThing interface bitset +0x20 (TC 0x42 = +0x28 bit 2), map +0x44.
// Shape: the target thing is parked in the `push ecx` frame word, the follower thing kept in esi; the def pointer and
//   the expression name live in the dead `follower`/`target` argument slots (the name in its own block); the component
//   lookup is the HasTC + one-ternary GetTC form; the def is released through the pointee's Release (`dec [ecx+4]`).
//   The release tests ecx straight from the slot (no eax staging) only when NEITHER callee that receives the def
//   slot's address escapes it: same-TU noinline stand-ins for GetDef (out pointer) and the event ctor (const
//   CDefPointer&) -- either alone leaves 17 lines.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    bool HasTC(long type) const { if (m_presentBits[type >> 5] & (1UL << (type & 31))) return true; return false; }
    template <class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};
class CThingCreatureBase : public CThing {};
struct CScriptThing {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010();
    virtual CThing* GetThing() const;                   // slot 11 (+0x2c)
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043();
    virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051();
    virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067();
    virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual bool IsAlive() const;                       // slot 75 (+0x12c)
    void* PImp[2];
};
struct CExpressionDef {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
};
static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
struct CDefPointerExpressionDef {
    CExpressionDef* Object;
    CDefPointerExpressionDef() : Object(0) {}
    ~CDefPointerExpressionDef() { CExpressionDef* p = Object; if (p) { if (DecRefToZero(p->RefCount)) p->Destroy(); } }
};
struct CGameDefinitionManager {
    static CGameDefinitionManager& __fastcall Get();    // 0x0044C6B0
    bool GetDef(const CCharString& name, CDefPointerExpressionDef* out);   // 0x00593666
};
namespace NEntityEvents {
struct CEventBase {};
struct CEventExpressionPerformedTo : CEventBase {       // 0x20 bytes
    unsigned char _body[0x20];
    CEventExpressionPerformedTo(CThingCreatureBase* performer, long frame, const CDefPointerExpressionDef& expression,
                                long distance, long keepUp);   // 0x0084B010
};
}
struct CTCEntityEvents {
    void AddEvent(NEntityEvents::CEventBase* event);    // 0x008113C0
};
struct CGameScriptInterface_Base {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087();
    virtual void v088(); virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095();
    virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103();
    virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111();
    virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119();
    virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127();
    virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135();
    virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143();
    virtual void v144(); virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149(); virtual void v150(); virtual void v151();
    virtual void v152(); virtual void v153(); virtual void v154(); virtual void v155(); virtual void v156(); virtual void v157(); virtual void v158(); virtual void v159();
    virtual void v160(); virtual void v161(); virtual void v162(); virtual void v163(); virtual void v164(); virtual void v165(); virtual void v166(); virtual void v167();
    virtual void v168(); virtual void v169(); virtual void v170(); virtual void v171(); virtual void v172(); virtual void v173(); virtual void v174(); virtual void v175();
    virtual void v176(); virtual void v177(); virtual void v178(); virtual void v179(); virtual void v180(); virtual void v181(); virtual void v182(); virtual void v183();
    virtual void v184(); virtual void v185(); virtual void v186(); virtual void v187(); virtual void v188(); virtual void v189(); virtual void v190(); virtual void v191();
    virtual void v192(); virtual void v193(); virtual void v194(); virtual void v195(); virtual void v196(); virtual void v197(); virtual void v198(); virtual void v199();
    virtual void v200(); virtual void v201(); virtual void v202(); virtual void v203(); virtual void v204(); virtual void v205(); virtual void v206(); virtual void v207();
    virtual void v208(); virtual void v209(); virtual void v210(); virtual void v211(); virtual void v212(); virtual void v213(); virtual void v214(); virtual void v215();
    virtual void v216(); virtual void v217(); virtual void v218(); virtual void v219(); virtual void v220(); virtual void v221(); virtual void v222(); virtual void v223();
    virtual void v224(); virtual void v225(); virtual void v226(); virtual void v227(); virtual void v228(); virtual void v229(); virtual void v230(); virtual void v231();
    virtual void v232(); virtual void v233(); virtual void v234();
    virtual void EntitySetAsAllowedToFollowHero(const CScriptThing& thing, bool allowed) const;   // slot 235 (+0x3ac)
};
struct CGameScriptInterface : CGameScriptInterface_Base {
    void EntityFollowThing(const CScriptThing& follower, const CScriptThing& target, float distance, bool keepUp) const;
};

// Same-TU stand-ins for the two callees that receive the def slot's address (bodies are not the retail ones).
CExpressionDef* LookupDef_00593666(CGameDefinitionManager* m, const CCharString& name);
#pragma auto_inline(off)
__declspec(noinline) bool CGameDefinitionManager::GetDef(const CCharString& name, CDefPointerExpressionDef* out)
{
    CExpressionDef* d = LookupDef_00593666(this, name);
    if (!d) return false;
    out->Object = d;
    return true;
}
#pragma auto_inline(on)
void InitEvent_0084b010(void* e, CThingCreatureBase* p, long f, CExpressionDef* d, long dist, long k);
#pragma auto_inline(off)
__declspec(noinline) NEntityEvents::CEventExpressionPerformedTo::CEventExpressionPerformedTo(CThingCreatureBase* performer, long frame,
    const CDefPointerExpressionDef& expression, long distance, long keepUp)
{
    InitEvent_0084b010(this, performer, frame, expression.Object, distance, keepUp);
}
#pragma auto_inline(on)

void CGameScriptInterface::EntityFollowThing(const CScriptThing& follower, const CScriptThing& target, float distance,
                                             bool keepUp) const
{
    if (!follower.IsAlive())
        return;
    if (!target.IsAlive())
        return;
    CThing* targetThing = target.GetThing();
    CThing* followerThing = follower.GetThing();
    CDefPointerExpressionDef def;
    {
        CCharString name("EXPRESSION_FOLLOW", -1);
        CGameDefinitionManager::Get().GetDef(name, &def);
    }
    if (followerThing->HasTC(0x42)) {
        CTCEntityEvents* events = followerThing->GetTC<CTCEntityEvents>(0x42);
        EntitySetAsAllowedToFollowHero(follower, true);
        events->AddEvent(new NEntityEvents::CEventExpressionPerformedTo((CThingCreatureBase*)targetThing, 0, def,
                                                                        (long)distance, keepUp));
    }
}

// ---- stubs and driver ----

static int g_strCtors = 0, g_strDtors = 0, g_destroys = 0, g_adds = 0, g_allow = 0, g_events = 0;
static const char* g_lastText = 0;
CCharString::CCharString(const char* text, long length) { ++g_strCtors; g_lastText = text; CHECK(length == -1); PStringData = (void*)text; }
CCharString::~CCharString() { ++g_strDtors; }
void CScriptThing::v000() { } void CScriptThing::v001() { } void CScriptThing::v002() { } void CScriptThing::v003() { } void CScriptThing::v004() { } void CScriptThing::v005() { } void CScriptThing::v006() { } void CScriptThing::v007() { } void CScriptThing::v008() { } void CScriptThing::v009() { } void CScriptThing::v010() { } void CScriptThing::v012() { } void CScriptThing::v013() { } void CScriptThing::v014() { } void CScriptThing::v015() { } void CScriptThing::v016() { } void CScriptThing::v017() { } void CScriptThing::v018() { } void CScriptThing::v019() { } void CScriptThing::v020() { } void CScriptThing::v021() { } void CScriptThing::v022() { } void CScriptThing::v023() { } void CScriptThing::v024() { } void CScriptThing::v025() { } void CScriptThing::v026() { } void CScriptThing::v027() { } void CScriptThing::v028() { } void CScriptThing::v029() { } void CScriptThing::v030() { } void CScriptThing::v031() { } void CScriptThing::v032() { } void CScriptThing::v033() { } void CScriptThing::v034() { } void CScriptThing::v035() { } void CScriptThing::v036() { } void CScriptThing::v037() { } void CScriptThing::v038() { } void CScriptThing::v039() { } void CScriptThing::v040() { } void CScriptThing::v041() { } void CScriptThing::v042() { } void CScriptThing::v043() { } void CScriptThing::v044() { } void CScriptThing::v045() { } void CScriptThing::v046() { } void CScriptThing::v047() { } void CScriptThing::v048() { } void CScriptThing::v049() { } void CScriptThing::v050() { } void CScriptThing::v051() { } void CScriptThing::v052() { } void CScriptThing::v053() { } void CScriptThing::v054() { } void CScriptThing::v055() { } void CScriptThing::v056() { } void CScriptThing::v057() { } void CScriptThing::v058() { } void CScriptThing::v059() { } void CScriptThing::v060() { } void CScriptThing::v061() { } void CScriptThing::v062() { } void CScriptThing::v063() { } void CScriptThing::v064() { } void CScriptThing::v065() { } void CScriptThing::v066() { } void CScriptThing::v067() { } void CScriptThing::v068() { } void CScriptThing::v069() { } void CScriptThing::v070() { } void CScriptThing::v071() { } void CScriptThing::v072() { } void CScriptThing::v073() { } void CScriptThing::v074() { }
struct ScriptThingImpl { bool alive; CThing* thing; };
static ScriptThingImpl g_impl[2];
static const CScriptThing* g_st[2];
static int idx(const CScriptThing* s) { return s == g_st[0] ? 0 : 1; }
CThing* CScriptThing::GetThing() const { return g_impl[idx(this)].thing; }
bool CScriptThing::IsAlive() const { return g_impl[idx(this)].alive; }
void CGameScriptInterface_Base::v000() { } void CGameScriptInterface_Base::v001() { } void CGameScriptInterface_Base::v002() { } void CGameScriptInterface_Base::v003() { } void CGameScriptInterface_Base::v004() { } void CGameScriptInterface_Base::v005() { } void CGameScriptInterface_Base::v006() { } void CGameScriptInterface_Base::v007() { } void CGameScriptInterface_Base::v008() { } void CGameScriptInterface_Base::v009() { } void CGameScriptInterface_Base::v010() { } void CGameScriptInterface_Base::v011() { } void CGameScriptInterface_Base::v012() { } void CGameScriptInterface_Base::v013() { } void CGameScriptInterface_Base::v014() { } void CGameScriptInterface_Base::v015() { } void CGameScriptInterface_Base::v016() { } void CGameScriptInterface_Base::v017() { } void CGameScriptInterface_Base::v018() { } void CGameScriptInterface_Base::v019() { } void CGameScriptInterface_Base::v020() { } void CGameScriptInterface_Base::v021() { } void CGameScriptInterface_Base::v022() { } void CGameScriptInterface_Base::v023() { } void CGameScriptInterface_Base::v024() { } void CGameScriptInterface_Base::v025() { } void CGameScriptInterface_Base::v026() { } void CGameScriptInterface_Base::v027() { } void CGameScriptInterface_Base::v028() { } void CGameScriptInterface_Base::v029() { } void CGameScriptInterface_Base::v030() { } void CGameScriptInterface_Base::v031() { } void CGameScriptInterface_Base::v032() { } void CGameScriptInterface_Base::v033() { } void CGameScriptInterface_Base::v034() { } void CGameScriptInterface_Base::v035() { } void CGameScriptInterface_Base::v036() { } void CGameScriptInterface_Base::v037() { } void CGameScriptInterface_Base::v038() { } void CGameScriptInterface_Base::v039() { } void CGameScriptInterface_Base::v040() { } void CGameScriptInterface_Base::v041() { } void CGameScriptInterface_Base::v042() { } void CGameScriptInterface_Base::v043() { } void CGameScriptInterface_Base::v044() { } void CGameScriptInterface_Base::v045() { } void CGameScriptInterface_Base::v046() { } void CGameScriptInterface_Base::v047() { } void CGameScriptInterface_Base::v048() { } void CGameScriptInterface_Base::v049() { } void CGameScriptInterface_Base::v050() { } void CGameScriptInterface_Base::v051() { } void CGameScriptInterface_Base::v052() { } void CGameScriptInterface_Base::v053() { } void CGameScriptInterface_Base::v054() { } void CGameScriptInterface_Base::v055() { } void CGameScriptInterface_Base::v056() { } void CGameScriptInterface_Base::v057() { } void CGameScriptInterface_Base::v058() { } void CGameScriptInterface_Base::v059() { } void CGameScriptInterface_Base::v060() { } void CGameScriptInterface_Base::v061() { } void CGameScriptInterface_Base::v062() { } void CGameScriptInterface_Base::v063() { } void CGameScriptInterface_Base::v064() { } void CGameScriptInterface_Base::v065() { } void CGameScriptInterface_Base::v066() { } void CGameScriptInterface_Base::v067() { } void CGameScriptInterface_Base::v068() { } void CGameScriptInterface_Base::v069() { } void CGameScriptInterface_Base::v070() { } void CGameScriptInterface_Base::v071() { } void CGameScriptInterface_Base::v072() { } void CGameScriptInterface_Base::v073() { } void CGameScriptInterface_Base::v074() { } void CGameScriptInterface_Base::v075() { } void CGameScriptInterface_Base::v076() { } void CGameScriptInterface_Base::v077() { } void CGameScriptInterface_Base::v078() { } void CGameScriptInterface_Base::v079() { } void CGameScriptInterface_Base::v080() { } void CGameScriptInterface_Base::v081() { } void CGameScriptInterface_Base::v082() { } void CGameScriptInterface_Base::v083() { } void CGameScriptInterface_Base::v084() { } void CGameScriptInterface_Base::v085() { } void CGameScriptInterface_Base::v086() { } void CGameScriptInterface_Base::v087() { } void CGameScriptInterface_Base::v088() { } void CGameScriptInterface_Base::v089() { } void CGameScriptInterface_Base::v090() { } void CGameScriptInterface_Base::v091() { } void CGameScriptInterface_Base::v092() { } void CGameScriptInterface_Base::v093() { } void CGameScriptInterface_Base::v094() { } void CGameScriptInterface_Base::v095() { } void CGameScriptInterface_Base::v096() { } void CGameScriptInterface_Base::v097() { } void CGameScriptInterface_Base::v098() { } void CGameScriptInterface_Base::v099() { } void CGameScriptInterface_Base::v100() { } void CGameScriptInterface_Base::v101() { } void CGameScriptInterface_Base::v102() { } void CGameScriptInterface_Base::v103() { } void CGameScriptInterface_Base::v104() { } void CGameScriptInterface_Base::v105() { } void CGameScriptInterface_Base::v106() { } void CGameScriptInterface_Base::v107() { } void CGameScriptInterface_Base::v108() { } void CGameScriptInterface_Base::v109() { } void CGameScriptInterface_Base::v110() { } void CGameScriptInterface_Base::v111() { } void CGameScriptInterface_Base::v112() { } void CGameScriptInterface_Base::v113() { } void CGameScriptInterface_Base::v114() { } void CGameScriptInterface_Base::v115() { } void CGameScriptInterface_Base::v116() { } void CGameScriptInterface_Base::v117() { } void CGameScriptInterface_Base::v118() { } void CGameScriptInterface_Base::v119() { } void CGameScriptInterface_Base::v120() { } void CGameScriptInterface_Base::v121() { } void CGameScriptInterface_Base::v122() { } void CGameScriptInterface_Base::v123() { } void CGameScriptInterface_Base::v124() { } void CGameScriptInterface_Base::v125() { } void CGameScriptInterface_Base::v126() { } void CGameScriptInterface_Base::v127() { } void CGameScriptInterface_Base::v128() { } void CGameScriptInterface_Base::v129() { } void CGameScriptInterface_Base::v130() { } void CGameScriptInterface_Base::v131() { } void CGameScriptInterface_Base::v132() { } void CGameScriptInterface_Base::v133() { } void CGameScriptInterface_Base::v134() { } void CGameScriptInterface_Base::v135() { } void CGameScriptInterface_Base::v136() { } void CGameScriptInterface_Base::v137() { } void CGameScriptInterface_Base::v138() { } void CGameScriptInterface_Base::v139() { } void CGameScriptInterface_Base::v140() { } void CGameScriptInterface_Base::v141() { } void CGameScriptInterface_Base::v142() { } void CGameScriptInterface_Base::v143() { } void CGameScriptInterface_Base::v144() { } void CGameScriptInterface_Base::v145() { } void CGameScriptInterface_Base::v146() { } void CGameScriptInterface_Base::v147() { } void CGameScriptInterface_Base::v148() { } void CGameScriptInterface_Base::v149() { } void CGameScriptInterface_Base::v150() { } void CGameScriptInterface_Base::v151() { } void CGameScriptInterface_Base::v152() { } void CGameScriptInterface_Base::v153() { } void CGameScriptInterface_Base::v154() { } void CGameScriptInterface_Base::v155() { } void CGameScriptInterface_Base::v156() { } void CGameScriptInterface_Base::v157() { } void CGameScriptInterface_Base::v158() { } void CGameScriptInterface_Base::v159() { } void CGameScriptInterface_Base::v160() { } void CGameScriptInterface_Base::v161() { } void CGameScriptInterface_Base::v162() { } void CGameScriptInterface_Base::v163() { } void CGameScriptInterface_Base::v164() { } void CGameScriptInterface_Base::v165() { } void CGameScriptInterface_Base::v166() { } void CGameScriptInterface_Base::v167() { } void CGameScriptInterface_Base::v168() { } void CGameScriptInterface_Base::v169() { } void CGameScriptInterface_Base::v170() { } void CGameScriptInterface_Base::v171() { } void CGameScriptInterface_Base::v172() { } void CGameScriptInterface_Base::v173() { } void CGameScriptInterface_Base::v174() { } void CGameScriptInterface_Base::v175() { } void CGameScriptInterface_Base::v176() { } void CGameScriptInterface_Base::v177() { } void CGameScriptInterface_Base::v178() { } void CGameScriptInterface_Base::v179() { } void CGameScriptInterface_Base::v180() { } void CGameScriptInterface_Base::v181() { } void CGameScriptInterface_Base::v182() { } void CGameScriptInterface_Base::v183() { } void CGameScriptInterface_Base::v184() { } void CGameScriptInterface_Base::v185() { } void CGameScriptInterface_Base::v186() { } void CGameScriptInterface_Base::v187() { } void CGameScriptInterface_Base::v188() { } void CGameScriptInterface_Base::v189() { } void CGameScriptInterface_Base::v190() { } void CGameScriptInterface_Base::v191() { } void CGameScriptInterface_Base::v192() { } void CGameScriptInterface_Base::v193() { } void CGameScriptInterface_Base::v194() { } void CGameScriptInterface_Base::v195() { } void CGameScriptInterface_Base::v196() { } void CGameScriptInterface_Base::v197() { } void CGameScriptInterface_Base::v198() { } void CGameScriptInterface_Base::v199() { } void CGameScriptInterface_Base::v200() { } void CGameScriptInterface_Base::v201() { } void CGameScriptInterface_Base::v202() { } void CGameScriptInterface_Base::v203() { } void CGameScriptInterface_Base::v204() { } void CGameScriptInterface_Base::v205() { } void CGameScriptInterface_Base::v206() { } void CGameScriptInterface_Base::v207() { } void CGameScriptInterface_Base::v208() { } void CGameScriptInterface_Base::v209() { } void CGameScriptInterface_Base::v210() { } void CGameScriptInterface_Base::v211() { } void CGameScriptInterface_Base::v212() { } void CGameScriptInterface_Base::v213() { } void CGameScriptInterface_Base::v214() { } void CGameScriptInterface_Base::v215() { } void CGameScriptInterface_Base::v216() { } void CGameScriptInterface_Base::v217() { } void CGameScriptInterface_Base::v218() { } void CGameScriptInterface_Base::v219() { } void CGameScriptInterface_Base::v220() { } void CGameScriptInterface_Base::v221() { } void CGameScriptInterface_Base::v222() { } void CGameScriptInterface_Base::v223() { } void CGameScriptInterface_Base::v224() { } void CGameScriptInterface_Base::v225() { } void CGameScriptInterface_Base::v226() { } void CGameScriptInterface_Base::v227() { } void CGameScriptInterface_Base::v228() { } void CGameScriptInterface_Base::v229() { } void CGameScriptInterface_Base::v230() { } void CGameScriptInterface_Base::v231() { } void CGameScriptInterface_Base::v232() { } void CGameScriptInterface_Base::v233() { } void CGameScriptInterface_Base::v234() { }
static const CScriptThing* g_allowThing = 0; static bool g_allowFlag = false;
void CGameScriptInterface_Base::EntitySetAsAllowedToFollowHero(const CScriptThing& thing, bool allowed) const { ++g_allow; g_allowThing = &thing; g_allowFlag = allowed; }
void CExpressionDef::v00() { }
void CExpressionDef::Destroy() { ++g_destroys; }
static CGameDefinitionManager g_mgr;
CGameDefinitionManager& __fastcall CGameDefinitionManager::Get() { return g_mgr; }
static CExpressionDef* g_def = 0;
CExpressionDef* LookupDef_00593666(CGameDefinitionManager* m, const CCharString& name)
{ CHECK(m == &g_mgr); CHECK(strcmp((const char*)name.PStringData, "EXPRESSION_FOLLOW") == 0); if (g_def) ++g_def->RefCount; return g_def; }
static CThingCreatureBase* g_evPerformer = 0; static long g_evFrame = -1, g_evDist = -1, g_evKeep = -1; static CExpressionDef* g_evDef = 0;
void InitEvent_0084b010(void* e, CThingCreatureBase* p, long f, CExpressionDef* d, long dist, long k)
{ ++g_events; g_evPerformer = p; g_evFrame = f; g_evDef = d; g_evDist = dist; g_evKeep = k; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{ SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
static NEntityEvents::CEventBase* g_lastEvent = 0; static CTCEntityEvents* g_addedTo = 0;
void CTCEntityEvents::AddEvent(NEntityEvents::CEventBase* ev) { ++g_adds; g_lastEvent = ev; g_addedTo = this; }
int main()
{
    static CThing follower, target; static CScriptThing sf, stt; CGameScriptInterface gsi;
    static CTCEntityEvents events; static CExpressionDef def;
    static CInterfaceMap::SEntry entries[2];
    entries[0].type = 0x10; entries[0].component = 0;
    entries[1].type = 0x42; entries[1].component = (CTCBase*)&events;
    follower.m_interfaces.first = &entries[0]; follower.m_interfaces.last = &entries[2];
    g_st[0] = &sf; g_st[1] = &stt;
    g_impl[0].thing = &follower; g_impl[1].thing = &target;
    // 1) follower dead: nothing (no name, no def lookup)
    g_impl[0].alive = false; g_impl[1].alive = true;
    gsi.EntityFollowThing(sf, stt, 3.7f, true);
    CHECK(g_strCtors == 0 && g_adds == 0 && g_allow == 0);
    // 2) target dead: nothing
    g_impl[0].alive = true; g_impl[1].alive = false;
    gsi.EntityFollowThing(sf, stt, 3.7f, true);
    CHECK(g_strCtors == 0 && g_adds == 0);
    // 3) both alive, follower lacks TC 0x42: def looked up and released, nothing queued
    g_impl[1].alive = true; g_def = &def; def.RefCount = 1;
    gsi.EntityFollowThing(sf, stt, 3.7f, true);
    CHECK(g_strCtors == 1 && g_strDtors == 1 && g_adds == 0 && g_allow == 0 && def.RefCount == 1 && g_destroys == 0);
    CHECK(g_lastText && strcmp(g_lastText, "EXPRESSION_FOLLOW") == 0);
    // 4) follower has 0x42 (+0x28 bit 2): allowed-to-follow set, the event built with truncated distance and keepUp
    follower.m_presentBits[2] = 1UL << 2;
    gsi.EntityFollowThing(sf, stt, 3.7f, true);
    CHECK(g_allow == 1 && g_allowThing == &sf && g_allowFlag == true);
    CHECK(g_events == 1 && g_evPerformer == (CThingCreatureBase*)&target && g_evFrame == 0 && g_evDef == &def);
    CHECK(g_evDist == 3 && g_evKeep == 1 && g_adds == 1 && g_addedTo == &events && g_lastEvent != 0);
    CHECK(def.RefCount == 1 && g_destroys == 0 && g_strDtors == 2);
    gsi.EntityFollowThing(sf, stt, 9.99f, false);
    CHECK(g_evDist == 9 && g_evKeep == 0 && g_adds == 2);
    // 5) last reference: the release destroys the def
    def.RefCount = 0;
    gsi.EntityFollowThing(sf, stt, 1.0f, false);
    CHECK(def.RefCount == 0 && g_destroys == 1);
    // 6) no def found: nothing released, event still built with a null def
    g_def = 0;
    gsi.EntityFollowThing(sf, stt, 2.0f, true);
    CHECK(g_evDef == 0 && g_destroys == 1 && g_adds == 4);
    if (fails == 0) printf("008954a0_TEST PASS\n");
    return fails;
}
