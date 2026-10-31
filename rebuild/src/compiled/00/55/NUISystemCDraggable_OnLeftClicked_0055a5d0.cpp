// NUISystem::CDraggable::OnLeftClicked() -- the clickable left-click handling, then (once per drag) remember the position the drag started from, mark the component dragged and lift it one layer; finally turn the mouse cursor into the closed hand
// Grade: RELOCATION_MATCH over the true extent (82 bytes; BOUNDS_FIX.tsv: catalog 697)
// Leaf: ?OnLeftClicked@CDraggable@NUISystem@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0055A5D0, 82 bytes (0x0055A5D0..0x0055A621, then int3 padding to 0x0055A630); the
//   catalog row runs 697 bytes on into CScrollBar's ChangePosition / OnLeftUnclicked / OnUnhovered / UpdateMouseCursor
//   overrides (CScrollBar vtable 0x0124B7E4 slots 31 / 147 / 144 / 153 at 0x0055A630 / 0x0055A660 / 0x0055A740 / 0x0055A7C0).
// Identity (proven): slot 146 of vtable 0x0124BA94 (RTTI .?AVCDraggable@NUISystem@@); Ego_r.pdb and FableWin.pdb
//   ?OnLeftClicked@CDraggable@NUISystem@@UAEXXZ, donor order OnHovered / OnLeftClicked / ChangePosition / OnLeftUnclicked;
//   it starts with the direct call of the base ?OnLeftClicked@CClickable@NUISystem@@UAEXXZ (0x0055AF60, landed).
// Callees: NUISystem::CClickable::OnLeftClicked 0x0055AF60, NUISystem::CManager::GetInstance 0x0041E5F2 (static fastcall),
//   NUISystem::CMouseCursor::TurnToClosedHand (vtable 0x0124C22C slot 150, +0x258; 0x0055C860, landed).
// Layout (retail = Ego_r.pdb - 0x30 for the CDraggable members, the 12-byte STLport containers of the CComponent base):
//   CComponent::TargetPosition +0x3C (Ego_r +0x44), CComponent::Layer +0x12F (Ego_r +0x157), CDraggable::PositionBeforeDrag
//   +0x194 (Ego_r +0x1C4), CDraggable::WasDragged +0x19C (Ego_r +0x1CC); NUISystem::CManager::PMouseCursor +0xB8 (Ego_r +0xE8).
// Shape: `WasDragged = true` written before the position copy gives retail's store schedule (X, Layer load, flag, Y,
//   Layer); the cursor through an inline accessor straight into ecx, null test, virtual tail jump (landed 0x0055A5B0 shape).
class C2DVector {
public:
    float X;
    float Y;
};

namespace NUISystem {

class CMouseCursor {                                        // vtable 0x0124C22C
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041();
    virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053();
    virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065();
    virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077();
    virtual void v078(); virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083();
    virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089();
    virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095();
    virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101();
    virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107();
    virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113();
    virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119();
    virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125();
    virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131();
    virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137();
    virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143();
    virtual void v144(); virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148();
    virtual void TurnToHand();                              // slot 149 (+0x254) 0x0055C810
    virtual void TurnToClosedHand();                        // slot 150 (+0x258) 0x0055C860
};

class CManager {
public:
    unsigned char _pad00[0xb8];
    CMouseCursor* PMouseCursor;                             // +0xB8
    static CManager* __fastcall GetInstance();              // 0x0041E5F2
    CMouseCursor* GetMouseCursor() const { return PMouseCursor; }
};

class CComponent {
public:
    virtual ~CComponent();
    unsigned char _pad004[0x3c - 4];
    C2DVector     TargetPosition;                           // +0x3C
    unsigned char _pad044[0x12f - 0x44];
    char          Layer;                                    // +0x12F
};

class CChangingStateComponent : public CComponent {
public:
};

class CHoverable : public CChangingStateComponent {
public:
};

class CClickable : public CHoverable {
public:
    virtual void OnLeftClicked();                           // 0x0055AF60
};

class CDraggable : public CClickable {
public:
    unsigned char _pad130[0x194 - 0x130];
    C2DVector     PositionBeforeDrag;                       // +0x194
    bool          WasDragged;                               // +0x19C
    virtual void OnLeftClicked();
};

void CDraggable::OnLeftClicked()
{
    CClickable::OnLeftClicked();
    if (!WasDragged) {
        WasDragged = true;
        PositionBeforeDrag = TargetPosition;
        --Layer;
    }
    CMouseCursor* cursor = CManager::GetInstance()->GetMouseCursor();
    if (cursor)
        cursor->TurnToClosedHand();
}

}   // namespace NUISystem