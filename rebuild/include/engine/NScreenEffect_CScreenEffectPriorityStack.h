#ifndef FABLE_ENGINE_NSCREENEFFECT_CSCREENEFFECTPRIORITYSTACK_H
#define FABLE_ENGINE_NSCREENEFFECT_CSCREENEFFECTPRIORITYSTACK_H
#ifdef FABLE_RETAIL_CSCREENEFFECTPRIORITYSTACK_FULL
// Retail opt-in (Codex pass18): original full stack fields, native offsets and
// actual STLport list/set representations; existing declaration branch is untouched.
#include "engine/CEnvironmentSpellEffectInterface.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
namespace NScreenEffect {
enum ScreenEffectPriority {SCREEN_EFFECT_PRIORITY_OTHER=0,SCREEN_EFFECT_PRIORITY_SCRIPT=1,SCREEN_EFFECT_PRIORITY_SPELL=2,NUM_SCREEN_EFFECT_PRIORITIES=3};
class CScreenEffectPriorityStack {
public:
 struct CStackEntry {CStackEntry(long frames):curdurationcount(0),durationcount(frames),fadingout(false){}virtual ~CStackEntry(){}long curdurationcount,durationcount;bool fadingout;};
 struct CRadialBlurEntry:public CStackEntry {
  CRadialBlurEntry(const CEnvironmentSpellEffectInterface::CRadialBlurParams& p,const CEnvironmentSpellEffectInterface::CRadialBlurPositionParams& pos,long frames):CStackEntry(frames),param(p),posparam(pos){}
  CRadialBlurEntry(const CRadialBlurEntry&);
  virtual ~CRadialBlurEntry(){}
  CEnvironmentSpellEffectInterface::CRadialBlurParams param;
  CEnvironmentSpellEffectInterface::CRadialBlurPositionParams posparam;
 };
 struct CDisplacementMonochromeEffectColourEntry:public CStackEntry {CRGBFloatColour paramcol;};
 struct CScreenFilterEntry;
 struct CThingGraphicArgPasser;
 typedef std::vector<CThingGraphicArgPasser> ThingGraphicArgPassers;
 virtual ~CScreenEffectPriorityStack();
 void* InsertRadialBlurEntry(float,const CEnvironmentSpellEffectInterface::CRadialBlurParams&,const CEnvironmentSpellEffectInterface::CRadialBlurPositionParams&,ScreenEffectPriority);
 void ScreenFilterSetFlags(const ThingGraphicArgPassers&,void*);
private:
 std::list<CRadialBlurEntry> RadialParamStack[NUM_SCREEN_EFFECT_PRIORITIES];
 std::set<CStackEntry*> RadialBlurKeys;
 CEnvironmentSpellEffectInterface::CRadialBlurParams CurrentRadialBlurParams;
 CEnvironmentSpellEffectInterface::CRadialBlurPositionParams CurrentRadialBlurPosParams;
 std::list<CDisplacementMonochromeEffectColourEntry> DisplacementParamStack[NUM_SCREEN_EFFECT_PRIORITIES];
 std::set<CStackEntry*> DisplacementKeys;
 CRGBFloatColour CurrentDisplacementParam;
 std::list<CScreenFilterEntry> ScreenFilterParamStack[NUM_SCREEN_EFFECT_PRIORITIES];
 std::set<CStackEntry*> ScreenFilterKeys;
 CEnvironmentSpellEffectInterface::CScreenFilterParams CurrentScreenFilterParam;
};
#ifdef FABLE_RETAIL_CSCREENEFFECTPRIORITYSTACK_SCREENFILTERENTRY
// Row 0x00881A40 opt-in (ScreenFilterSetFlags): the screen-filter records. Ego_r.pdb struct_layouts: CThingGraphicArgPasser 9 bytes
// {CIntelligentPointer<CThing> thing +0; bool bypassSet +8} (byte-packed: `imul 38E38E39h` /9); CScreenFilterEntry 64 bytes
// {CStackEntry +0; CScreenFilterParams param +0x10; map<CThing*,CThingGraphicByPassFlagState> reset +0x30}; CThingGraphicByPassFlagState
// 9 bytes {CIntelligentPointer<CThing> thing +0; bool bypassReset +8}. The includer supplies engine/CIntelligentPointer.h
// (FABLE_RETAIL_CINTELLIGENTPOINTER) and a complete CThing / NGraphicAppearance::CTCGraphicAppearance first.
#pragma pack(push, 1)
struct CScreenEffectPriorityStack::CThingGraphicArgPasser {
 CIntelligentPointer<CThing> thing;
 bool bypassSet;
};
#pragma pack(pop)
struct CScreenEffectPriorityStack::CScreenFilterEntry : public CStackEntry {
#pragma pack(push, 1)
 // class, not struct: Ego_r.pdb / FableWin.pdb mangle it VCThingGraphicByPassFlagState@CScreenFilterEntry@... (class key V).
 class CThingGraphicByPassFlagState {
 public:
  CThingGraphicByPassFlagState():thing(0),bypassReset(false){}
  void Reset(){
   if(thing.GetPItem()!=0){
    if(!static_cast<CThing*>(thing.GetPItem())->IsDead()){
     NGraphicAppearance::CTCGraphicAppearance* graphic=static_cast<CThing*>(thing.GetPItem())->GetTCGraphicAppearance();
     if(graphic) graphic->SetByPassColorFilter(bypassReset);
    }
   }
  }
  CIntelligentPointer<CThing> thing;
  bool bypassReset;
 };
#pragma pack(pop)
 CEnvironmentSpellEffectInterface::CScreenFilterParams param;
 std::map<CThing*,CThingGraphicByPassFlagState> reset;
};
#endif
}
#else
// NScreenEffect::CScreenEffectPriorityStack (new, chunk 765): only the ScreenFilterSetFlags entry row 0x006B83E0 forwards to is
// modelled. Ego_r.pdb / FableWin.pdb ?ScreenFilterSetFlags@CScreenEffectPriorityStack@NScreenEffect@@QAEXABV?$vector@
// UCThingGraphicArgPasser@CScreenEffectPriorityStack@NScreenEffect@@V?$allocator@..@std@@@std@@PAX@Z = retail 0x00881A40.
// Header debt: the members (Ego_r 0x120 bytes of STLport list[3] / set stacks, whose retail sizes shrink) are not modelled, so the
// class here is member-less (sizeof 1, not the retail size): an includer may hold it only as a trailing member it never sizes.
namespace std { template <class T> class allocator; template <class T, class A> class vector; }
#ifdef FABLE_RETAIL_CSCREENEFFECTPRIORITYSTACK_ARGPASSER
// Chunk 1010 (row 0x008A9880 CGameScriptInterface::ScreenFilterFadeTo): the record and the priority enum.
// CThingGraphicArgPasser: Ego_r.pdb 9 bytes {CIntelligentPointer<CThing> thing +0x0; bool bypassSet +0x8} (retail walks it with
// `add esi,9`, 0x008A99C7, and `imul 38E38E39h` in _M_insert_overflow 0x006A3950), so the struct is byte-packed; FableWin.pdb
// ??0CThingGraphicArgPasser@CScreenEffectPriorityStack@NScreenEffect@@QAE@PAVCThing@@_N@Z (inline in retail: the base ctor 0x00A01B10,
// the vptr, SetPItem 0x00A01B90, the flag store), the copy ctor ..@ABU012@ and ??1 (implicit, ~CBaseIntelligentPointer 0x00A01C10).
// The includer supplies engine/CIntelligentPointer.h (FABLE_RETAIL_CINTELLIGENTPOINTER + _COPY) and a complete CThing first.
// ScreenEffectPriority: the enum of the PDB's W4ScreenEffectPriority@NScreenEffect@@ parameters; the script interface passes 1
// (enumerator names are not in the PDB dumps on disk: SCREEN_EFFECT_PRIORITY_SCRIPT is the landed 0x008901C0 spelling).
namespace NScreenEffect {
enum ScreenEffectPriority { SCREEN_EFFECT_PRIORITY_SCRIPT = 1, SCREEN_EFFECT_PRIORITY_FORCE_DWORD = 0x7fffffff };
class CScreenEffectPriorityStack {
public:
#pragma pack(push, 1)
    struct CThingGraphicArgPasser {
        CThingGraphicArgPasser(CThing* thing_, bool bypass_set) : thing(thing_), bypassSet(bypass_set) { }
        CIntelligentPointer<CThing> thing;                               // +0x00
        bool bypassSet;                                                  // +0x08
    };
#pragma pack(pop)
#else
namespace NScreenEffect {
class CScreenEffectPriorityStack {
public:
    struct CThingGraphicArgPasser;
#endif
    typedef std::vector<CThingGraphicArgPasser, std::allocator<CThingGraphicArgPasser> > ThingGraphicArgPassers;
    void ScreenFilterSetFlags(const ThingGraphicArgPassers& things, void* handle);   // 0x00881A40
#ifdef FABLE_RETAIL_CSCREENEFFECTPRIORITYSTACK_PROCESSES
// agentFAM05 (2026-11-11): the per-effect custom process interface and its three processes (Ego_r / FableWin
// name sets: ??_EICustomProcessInterface@CScreenEffectPriorityStack@NScreenEffect@@, ?OnErase@CScreenFilterProcess@..@@UAEXPAUCStackEntry@23@@Z,
// ?Process@..@@UAEXMPBUCStackEntry@23@@Z, ?ResetToDefault@..@@UAEXXZ; RTTI CDisplacementProcess / CRadialBlurProcess / CScreenFilterProcess :
// ICustomProcessInterface). OnErase is slot 2 of each process vtable (agentFAM03 proof, re-verified by agentFAM05); the other slots'
// order is not proven, so only leaf bodies may use these declarations. CStackEntry / CScreenFilterEntry as in the FULL branch (the
// filter entry is the list element the filter stack erases; ?Reset@CScreenFilterEntry@..@@QAEXXZ is out of line at 0x00883AE0).
    struct CStackEntry {
        virtual ~CStackEntry();
        long curdurationcount, durationcount;
        bool fadingout;
    };
    struct CScreenFilterEntry : public CStackEntry {
        void Reset();                                                    // 0x00883AE0
    };
    class ICustomProcessInterface {
    public:
        virtual ~ICustomProcessInterface();
        virtual void Process(float, const CStackEntry*) = 0;
        virtual void OnErase(CStackEntry*) = 0;                          // slot 2
        virtual void ResetToDefault() = 0;
    };
    class CDisplacementProcess : public ICustomProcessInterface {
    public:
        virtual void OnErase(CStackEntry*);                              // slot 2 0x00882000
    };
    class CRadialBlurProcess : public ICustomProcessInterface {
    public:
        virtual void OnErase(CStackEntry*);                              // slot 2 0x008826E0
    };
    class CScreenFilterProcess : public ICustomProcessInterface {
    public:
        virtual void OnErase(CStackEntry*);                              // slot 2 0x00880F50
    };
#endif // FABLE_RETAIL_CSCREENEFFECTPRIORITYSTACK_PROCESSES
};
}
#endif // full original stack opt-in
#endif
