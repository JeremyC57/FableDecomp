// CThingSearchTools::GetAllThings<CThingAICreature, std::list<CIntelligentPointer<CThingAICreature> >, CFilter_Bind5<CThingFilter_HasTC, CThingFilter_HasTC, CThingFilter_HasScriptSupervision, CFilter_Not<CThingFilter_AICreatureHasProperty>, CFilter_Not<CThingFilter_IsLyingDown> > >(const CFilter_Bind5<..>&, std::list<..>&) const -- every scan-visible AI creature that passes the five filters, appended as intelligent pointers
// Leaf: ??$GetAllThings@VCThingAICreature@@V?$list@V?$CIntelligentPointer@VCThingAICreature@@@@V?$allocator@V?$CIntelligentPointer@VCThingAICreature@@@@@std@@@std@@V?$CFilter_Bind5@VCThingFilter_HasTC@@V1@VCThingFilter_HasScriptSupervision@@V?$CFilter_Not@VCThingFilter_AICreatureHasProperty@@@@V?$CFilter_Not@VCThingFilter_IsLyingDown@@@@@@@CThingSearchTools@@QBEXABV?$CFilter_Bind5@VCThingFilter_HasTC@@V1@VCThingFilter_HasScriptSupervision@@V?$CFilter_Not@VCThingFilter_AICreatureHasProperty@@@@V?$CFilter_Not@VCThingFilter_IsLyingDown@@@@@@AAV?$list@V?$CIntelligentPointer@VCThingAICreature@@@@V?$allocator@V?$CIntelligentPointer@VCThingAICreature@@@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006DFA50, 233 bytes, `ret 8`.
// Identity: Ego_r.pdb 0x0070D2A0 + FableWin.pdb 0x0249F8D0 (entry q11333); the landed caller 0x006DDD30 binds this spelling.
// NF98 (round 8, q11333): ported off its local model onto engine/CThingSearchTools.h (the ALLTHINGSBODY template, TYPEMASK) as an explicit
//   instantiation, with the define set of its landed caller 0x006DDD30 (same retail TU) and that row's row-local filter classes, plus
//   STLP_LIST_PUSHBACK_NOINLINE (push_back called at 0x006DFB14), CTHINGAICREATURE_STATTYPEFLAGS (`and edx,1Fh`, 0x006DFAA5) and
//   CTHING_SCANINCLUSION_ISALIVE (`not cl ; test cl,1`, 0x006DFA8F).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHINGCREATUREBASE_SETCURRENTACTION_COUNTED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_SHOWTROPHY_USE
#define FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING
#define FABLE_RETAIL_CTROPHYDEF
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_QUEST_DRAW
#define FABLE_RETAIL_CPLAYERGUI_MEMBERS
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CDRAWQUESTINFO_MINIGAMETYPE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER_LAYOUT
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER_REGIONLOAD
#define FABLE_RETAIL_NGAMETEXT_CONSTRUCTSTRING_LONGS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_LIST_SIZE_DECL
#define FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY_TYPEMASK
#define FABLE_RETAIL_CTHINGCREATUREBASE_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGFILTER_HASTC_OUTOFLINE
#define FABLE_RETAIL_STLP_LIST_PUSHBACK_NOINLINE
#define FABLE_RETAIL_CTHINGAICREATURE_STATTYPEFLAGS
#define FABLE_RETAIL_CTHING_SCANINCLUSION_ISALIVE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_HasTC.h"
#include "engine/CThingFilter_AICreatureHasProperty.h"

// No shared headers for these (as in the landed caller 0x006DDD30); the out-of-line call operators are landed rows.
class CThingFilter_HasScriptSupervision : public std::unary_function<CThing, bool> {
    bool CheckForActive;
public:
    explicit CThingFilter_HasScriptSupervision(bool active) : CheckForActive(active) {}
    bool operator()(const CThing& thing) const;                          // 0x006A7530
};
class CThingFilter_IsLyingDown : public std::unary_function<CThing, bool> {
public:
    bool operator()(const CThing& thing) const;                          // 0x006A7400
};
template<class F> class CFilter_Not {
    const F& Filter;
public:
    explicit CFilter_Not(const F& filter) : Filter(filter) {}
    template<class T> bool operator()(const T& thing) const { return !Filter(thing); }
};
template<class A, class B, class C, class D, class E> class CFilter_Bind5 {
    const A& First;
    const B& Second;
    const C& Third;
    const D& Fourth;
    const E& Fifth;
public:
    CFilter_Bind5(const A& a, const B& b, const C& c, const D& d, const E& e)
        : First(a), Second(b), Third(c), Fourth(d), Fifth(e) {}
    // Inline-budget: retail expands the five filter calls in the scan loop (0x006DFAAD..0x006DFAEF) while it calls push_back out of line
    // (0x006DFB14); with plain inline VC7.1 calls this operator out of line (the landed model of this row forced it the same way).
    template<class T> FABLE_FORCEINLINE bool operator()(const T& thing) const
    { return First(thing) && Second(thing) && Third(thing) && Fourth(thing) && Fifth(thing); }
};

typedef CFilter_Bind5<CThingFilter_HasTC, CThingFilter_HasTC, CThingFilter_HasScriptSupervision,
    CFilter_Not<CThingFilter_AICreatureHasProperty>, CFilter_Not<CThingFilter_IsLyingDown> > TWitnessFilter;
template void CThingSearchTools::GetAllThings<CThingAICreature, std::list<CIntelligentPointer<CThingAICreature> >, TWitnessFilter>(
    const TWitnessFilter&, std::list<CIntelligentPointer<CThingAICreature> >&) const;
