// NUISystem::CTextSlider::OnDeactivate() -- stop observing UI event 0x19 through the slider's CObserver base
// Leaf: ?OnDeactivate@CTextSlider@NUISystem@@MAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00548780, 12 bytes, plain `ret`: mov eax,[ecx+4] ; add ecx,4 ; push 19h ;
//   call [eax+10h] ; ret.
// Identity: NUISystem::CTextSlider vtable 0x01248A8C slot 141, right after the landed CTextSlider::OnActivate 0x00548760
//   (slot 140, which calls the same base's slot 3 with 0x19). Ego_r.pdb declares the protected virtuals
//   ?OnActivate@CTextSlider@NUISystem@@MAEXXZ and ?OnDeactivate@CTextSlider@NUISystem@@MAEXXZ; CChangingStateComponent's
//   slot 141 is the empty landed OnDeactivate 0x0041C650. The base at +4 is NUISystem::CObserver (Fable.exe RTTI:
//   CComponent = CUIInterface@0, CObserver@4, CActionParam@24); its vtable 0x01246020 holds the landed
//   IsObservingEvent / ObserveEvent / IgnoreEvent at slots 2 / 3 / 4 (Ego_r.pdb ?IgnoreEvent@CObserver@NUISystem@@UAEXW4EEvent@2@@Z).
//   The catalog "CTextSlider::vfunc_141" is the slot name.
// Shape: the CObserver subobject is formed with `add ecx,4` (no null test: `this` is never null in a member call); the
//   call cannot be a tail jump because the event is pushed, so `call ; ret`.
namespace NUISystem {

enum EEvent { EEVENT_19 = 0x19 };

class CUIInterface {                            // primary base: slots 0..139 (CComponent and CChangingStateComponent fill them)
public:
    virtual ~CUIInterface();                    // slot 0
    virtual void _v001(); virtual void _v002(); virtual void _v003(); virtual void _v004(); virtual void _v005();
    virtual void _v006(); virtual void _v007(); virtual void _v008(); virtual void _v009(); virtual void _v010();
    virtual void _v011(); virtual void _v012(); virtual void _v013(); virtual void _v014(); virtual void _v015();
    virtual void _v016(); virtual void _v017(); virtual void _v018(); virtual void _v019(); virtual void _v020();
    virtual void _v021(); virtual void _v022(); virtual void _v023(); virtual void _v024(); virtual void _v025();
    virtual void _v026(); virtual void _v027(); virtual void _v028(); virtual void _v029(); virtual void _v030();
    virtual void _v031(); virtual void _v032(); virtual void _v033(); virtual void _v034(); virtual void _v035();
    virtual void _v036(); virtual void _v037(); virtual void _v038(); virtual void _v039(); virtual void _v040();
    virtual void _v041(); virtual void _v042(); virtual void _v043(); virtual void _v044(); virtual void _v045();
    virtual void _v046(); virtual void _v047(); virtual void _v048(); virtual void _v049(); virtual void _v050();
    virtual void _v051(); virtual void _v052(); virtual void _v053(); virtual void _v054(); virtual void _v055();
    virtual void _v056(); virtual void _v057(); virtual void _v058(); virtual void _v059(); virtual void _v060();
    virtual void _v061(); virtual void _v062(); virtual void _v063(); virtual void _v064(); virtual void _v065();
    virtual void _v066(); virtual void _v067(); virtual void _v068(); virtual void _v069(); virtual void _v070();
    virtual void _v071(); virtual void _v072(); virtual void _v073(); virtual void _v074(); virtual void _v075();
    virtual void _v076(); virtual void _v077(); virtual void _v078(); virtual void _v079(); virtual void _v080();
    virtual void _v081(); virtual void _v082(); virtual void _v083(); virtual void _v084(); virtual void _v085();
    virtual void _v086(); virtual void _v087(); virtual void _v088(); virtual void _v089(); virtual void _v090();
    virtual void _v091(); virtual void _v092(); virtual void _v093(); virtual void _v094(); virtual void _v095();
    virtual void _v096(); virtual void _v097(); virtual void _v098(); virtual void _v099(); virtual void _v100();
    virtual void _v101(); virtual void _v102(); virtual void _v103(); virtual void _v104(); virtual void _v105();
    virtual void _v106(); virtual void _v107(); virtual void _v108(); virtual void _v109(); virtual void _v110();
    virtual void _v111(); virtual void _v112(); virtual void _v113(); virtual void _v114(); virtual void _v115();
    virtual void _v116(); virtual void _v117(); virtual void _v118(); virtual void _v119(); virtual void _v120();
    virtual void _v121(); virtual void _v122(); virtual void _v123(); virtual void _v124(); virtual void _v125();
    virtual void _v126(); virtual void _v127(); virtual void _v128(); virtual void _v129(); virtual void _v130();
    virtual void _v131(); virtual void _v132(); virtual void _v133(); virtual void _v134(); virtual void _v135();
    virtual void _v136(); virtual void _v137(); virtual void _v138(); virtual void _v139();
};

class CObserver {                               // +0x04, vtable 0x01246020
public:
    virtual ~CObserver();                       // slot 0
    virtual void _v1();                         // slot 1 (_purecall)
    virtual bool IsObservingEvent(EEvent event);   // slot 2
    virtual void ObserveEvent(EEvent event);    // slot 3
    virtual void IgnoreEvent(EEvent event);     // slot 4 (+0x10)
};

class CComponent : public CUIInterface, public CObserver {
public:
};

class CChangingStateComponent : public CComponent {
public:
};

class CTextSlider : public CChangingStateComponent {
protected:
    virtual void OnActivate();                  // slot 140 (+0x230), 0x00548760
    virtual void OnDeactivate();                // slot 141 (+0x234)
};

} // namespace NUISystem

void NUISystem::CTextSlider::OnDeactivate()
{
    IgnoreEvent(EEVENT_19);
}