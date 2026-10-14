// NUISystem::CHoverable::Update(float dt) -- the base update (0x0052C7E0), then virtual slot 145 when the hover position (+0x3c/+0x40) moved more than 1e-4 from the stored one (+0x34/+0x38) or the hover timer (+0x30) minus `dt` is exactly zero
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0055B890, length 96 bytes.
// Floats 0x0129BA3C (1e-4f), 0x0122DEDC (0.0f). Shape: `fabs ; fcomp ; test ah,41h ; jp` twice, `fucompp ; test ah,44h ; jp`, `ret 4`.
extern const float g_epsilon_0129ba3c;            // 0x0129BA3C (1e-4f)
extern const float g_zero_0122dedc;               // 0x0122DEDC (0.0f)
extern "C" double __cdecl fabs(double);
#pragma intrinsic(fabs)
namespace NUISystem {
struct IComponentA {                               // the interface base at +0x04 (vtable, 0x10 bytes of state)
    virtual void va00();
    unsigned char _state[0x10];
};
struct IComponentB {                               // the interface base at +0x18 (vtable only)
    virtual void vb00();
};
struct CComponentRoot {                            // the primary base: vptr at +0
    virtual void v00();
};
struct CChangingStateComponent : CComponentRoot, IComponentA, IComponentB {
    unsigned char _body[0x30 - 0x1c];
    float m_hoverTimer;                            // +0x30
    float m_storedX;                               // +0x34
    float m_storedY;                               // +0x38
    float m_hoverX;                                // +0x3c
    float m_hoverY;                                // +0x40
    void Update(float dt);                         // 0x0052C7E0
};
struct CHoverable : CChangingStateComponent {
    virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010();
    virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020();
    virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030();
    virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040();
    virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050();
    virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060();
    virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070();
    virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080();
    virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089(); virtual void v090();
    virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100();
    virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110();
    virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120();
    virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130();
    virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140();
    virtual void v141(); virtual void v142(); virtual void v143(); virtual void v144();
    virtual void OnHoverMoved();                   // slot 145 (+0x244)
    void Update(float dt);
};

void CHoverable::Update(float dt)
{
    CChangingStateComponent::Update(dt);
    if (!((float)fabs(m_hoverX - m_storedX) <= g_epsilon_0129ba3c) || !((float)fabs(m_hoverY - m_storedY) <= g_epsilon_0129ba3c) || m_hoverTimer - dt == g_zero_0122dedc)
        OnHoverMoved();
}
}
