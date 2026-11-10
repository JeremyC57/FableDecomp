#ifndef FABLE_ENGINE_NSCREENEFFECT_CSCREENEFFECTPRIORITYSTACK_H
#define FABLE_ENGINE_NSCREENEFFECT_CSCREENEFFECTPRIORITYSTACK_H
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
};
}
#endif
