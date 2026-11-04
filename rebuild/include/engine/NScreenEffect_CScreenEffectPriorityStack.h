#ifndef FABLE_ENGINE_NSCREENEFFECT_CSCREENEFFECTPRIORITYSTACK_H
#define FABLE_ENGINE_NSCREENEFFECT_CSCREENEFFECTPRIORITYSTACK_H
// NScreenEffect::CScreenEffectPriorityStack (new, chunk 765): only the ScreenFilterSetFlags entry row 0x006B83E0 forwards to is
// modelled. Ego_r.pdb / FableWin.pdb ?ScreenFilterSetFlags@CScreenEffectPriorityStack@NScreenEffect@@QAEXABV?$vector@
// UCThingGraphicArgPasser@CScreenEffectPriorityStack@NScreenEffect@@V?$allocator@..@std@@@std@@PAX@Z = retail 0x00881A40.
// Header debt: the members (Ego_r 0x120 bytes of STLport list[3] / set stacks, whose retail sizes shrink) are not modelled, so the
// class here is member-less (sizeof 1, not the retail size): an includer may hold it only as a trailing member it never sizes.
namespace std { template <class T> class allocator; template <class T, class A> class vector; }
namespace NScreenEffect {
class CScreenEffectPriorityStack {
public:
    struct CThingGraphicArgPasser;
    typedef std::vector<CThingGraphicArgPasser, std::allocator<CThingGraphicArgPasser> > ThingGraphicArgPassers;
    void ScreenFilterSetFlags(const ThingGraphicArgPassers& things, void* handle);   // 0x00881A40
};
}
#endif
