// NUISystem::CText::CanScroll(bool down, const C2DBoxF& bounds) -- after the slot-135 refresh, reports how far the text could still scroll inside the bounds: scrolling down, the UI-scaled text height plus the text position measured against the bottom edge; otherwise the position against the top edge; zero when no room
// Leaf: CanScroll
// Retail: Fable.exe (TLC, VC7.1) @ 0x0054ECB0, 207 bytes (+13 int3). `ret 8`, float on the FPU stack.
// Callees: own slot 135 (+0x21C) with the +0x148 word, CFrontEndManager::GetInstance 0x0041E5F2 (`__fastcall`),
//          NUISystem::CManager::GetUIScale 0x0041CF47 (C2DVector by hidden pointer), own slot 112 (+0x1C0) GetPosition
//          (C2DVector by hidden pointer). Float pool: 0.0f @ 0x0122DEDC.
// Shape: the hidden-result temporary is shared by GetUIScale and GetPosition (one `lea eax,[esp+10h] ; push eax` hoisted
//        above the `down` branch); the height copy sits in the frame word below it, scaled in place (`fstp` back) before
//        the position call; `fcomp ; fnstsw ; test ah,41h` for the `>` test and `test ah,5 ; jp` for the `<` test; each
//        return re-calls GetPosition (no CSE across a virtual call); the shared `fld 0.0f` exit.
// Layout: retail CDraggableInto 0x15C (PDB minus 0x30): the height word at +0xD0 and the pointer at +0x148 are named
//         descriptively (engine/CText.h pads them).
struct C2DVector {
    float X, Y;
    C2DVector() { }
};
struct C2DBoxF { float TLX, TLY, BRX, BRY; };
extern const float g_zero;                                 // 0x0122DEDC
namespace NUISystem {
struct CManager {
    C2DVector GetUIScale();                                // 0x0041CF47
};
}
struct CFrontEndManager {
    static NUISystem::CManager* __fastcall GetInstance();  // 0x0041E5F2
};
namespace NUISystem {
class CText {
public:
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
    virtual C2DVector GetPosition();                       // slot 112 (+0x1C0)
    virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119();
    virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127();
    virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134();
    virtual void vslot135(void* context);                  // slot 135 (+0x21C): refresh before measuring
    unsigned char _pad04[0xd0 - 0x04];
    float         m_textHeight;                            // +0xD0 (descriptive)
    unsigned char _padd4[0x148 - 0xd4];
    void*         m_pScrollContext;                        // +0x148 (descriptive)
    float CanScroll(bool down, const C2DBoxF& bounds);
};
}
float NUISystem::CText::CanScroll(bool down, const C2DBoxF& bounds)
{
    vslot135(m_pScrollContext);
    if (down) {
        C2DVector extent;                                  // only Y is ever set: the frame keeps the unused X word
        extent.Y = m_textHeight;
        extent.Y *= CFrontEndManager::GetInstance()->GetUIScale().Y;
        if (extent.Y + GetPosition().Y > bounds.BRY)
            return bounds.BRY - (extent.Y + GetPosition().Y);
    } else {
        if (GetPosition().Y < bounds.TLY)
            return bounds.TLY - GetPosition().Y;
    }
    return g_zero;
}