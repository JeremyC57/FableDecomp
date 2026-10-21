// NUISystem::CFrontEndList::InitialiseOffsets(unsigned long count) -- next scrolling child fully opaque at the origin
// Leaf: ?InitialiseOffsets@CFrontEndList@NUISystem@@QAEXK@Z
// Retail: Fable.exe @ 0x0054C480, 55 bytes, `ret 4` (the count is unused); slot 156 of the primary CFrontEndList vtable
// 0x01249224 (reference at 0x01249494); Ego_r.pdb ?InitialiseOffsets@CFrontEndList@NUISystem@@UAEXK@Z. Body: the
// virtual SetNextScrollingChildAlpha(1.0f) (slot 150, NUISystem::CList 0x0053CDF2: +0x1a0) then
// SetNextScrollingChildPosition(C2DVector(0, 0)) (slot 151, 0x0053CDFF: +0x1a4/+0x1a8; Ego_r.pdb takes C2DVector&). The
// zero vector is built in its stack slot before the first call. Modelled with a non-virtual leaf (the vtable slot is
// the only reference).
// Layout: only the vtable is used (slots 150 / 151 = +0x258 / +0x25c).
// Shape: /O2; the position is a named local declared first (a temporary at the call is built after the first call).
namespace NUISystem {
struct C2DVector {
    float X;
    float Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
};
class CFrontEndList {
public:
    virtual void _slot000(); virtual void _slot001(); virtual void _slot002(); virtual void _slot003();
    virtual void _slot004(); virtual void _slot005(); virtual void _slot006(); virtual void _slot007();
    virtual void _slot008(); virtual void _slot009(); virtual void _slot010(); virtual void _slot011();
    virtual void _slot012(); virtual void _slot013(); virtual void _slot014(); virtual void _slot015();
    virtual void _slot016(); virtual void _slot017(); virtual void _slot018(); virtual void _slot019();
    virtual void _slot020(); virtual void _slot021(); virtual void _slot022(); virtual void _slot023();
    virtual void _slot024(); virtual void _slot025(); virtual void _slot026(); virtual void _slot027();
    virtual void _slot028(); virtual void _slot029(); virtual void _slot030(); virtual void _slot031();
    virtual void _slot032(); virtual void _slot033(); virtual void _slot034(); virtual void _slot035();
    virtual void _slot036(); virtual void _slot037(); virtual void _slot038(); virtual void _slot039();
    virtual void _slot040(); virtual void _slot041(); virtual void _slot042(); virtual void _slot043();
    virtual void _slot044(); virtual void _slot045(); virtual void _slot046(); virtual void _slot047();
    virtual void _slot048(); virtual void _slot049(); virtual void _slot050(); virtual void _slot051();
    virtual void _slot052(); virtual void _slot053(); virtual void _slot054(); virtual void _slot055();
    virtual void _slot056(); virtual void _slot057(); virtual void _slot058(); virtual void _slot059();
    virtual void _slot060(); virtual void _slot061(); virtual void _slot062(); virtual void _slot063();
    virtual void _slot064(); virtual void _slot065(); virtual void _slot066(); virtual void _slot067();
    virtual void _slot068(); virtual void _slot069(); virtual void _slot070(); virtual void _slot071();
    virtual void _slot072(); virtual void _slot073(); virtual void _slot074(); virtual void _slot075();
    virtual void _slot076(); virtual void _slot077(); virtual void _slot078(); virtual void _slot079();
    virtual void _slot080(); virtual void _slot081(); virtual void _slot082(); virtual void _slot083();
    virtual void _slot084(); virtual void _slot085(); virtual void _slot086(); virtual void _slot087();
    virtual void _slot088(); virtual void _slot089(); virtual void _slot090(); virtual void _slot091();
    virtual void _slot092(); virtual void _slot093(); virtual void _slot094(); virtual void _slot095();
    virtual void _slot096(); virtual void _slot097(); virtual void _slot098(); virtual void _slot099();
    virtual void _slot100(); virtual void _slot101(); virtual void _slot102(); virtual void _slot103();
    virtual void _slot104(); virtual void _slot105(); virtual void _slot106(); virtual void _slot107();
    virtual void _slot108(); virtual void _slot109(); virtual void _slot110(); virtual void _slot111();
    virtual void _slot112(); virtual void _slot113(); virtual void _slot114(); virtual void _slot115();
    virtual void _slot116(); virtual void _slot117(); virtual void _slot118(); virtual void _slot119();
    virtual void _slot120(); virtual void _slot121(); virtual void _slot122(); virtual void _slot123();
    virtual void _slot124(); virtual void _slot125(); virtual void _slot126(); virtual void _slot127();
    virtual void _slot128(); virtual void _slot129(); virtual void _slot130(); virtual void _slot131();
    virtual void _slot132(); virtual void _slot133(); virtual void _slot134(); virtual void _slot135();
    virtual void _slot136(); virtual void _slot137(); virtual void _slot138(); virtual void _slot139();
    virtual void _slot140(); virtual void _slot141(); virtual void _slot142(); virtual void _slot143();
    virtual void _slot144(); virtual void _slot145(); virtual void _slot146(); virtual void _slot147();
    virtual void _slot148(); virtual void _slot149();
    virtual void SetNextScrollingChildAlpha(float alpha);           // slot 150 (+0x258)
    virtual void SetNextScrollingChildPosition(C2DVector& position);  // slot 151 (+0x25c)
    void InitialiseOffsets(unsigned long count);
};
void CFrontEndList::InitialiseOffsets(unsigned long)
{
    C2DVector position(0.0f, 0.0f);
    SetNextScrollingChildAlpha(1.0f);
    SetNextScrollingChildPosition(position);
}
}