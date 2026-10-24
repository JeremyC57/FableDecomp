// NUISystem::CDraggableInto::ProcessEvent(EEvent event) -- on event 0x1E while the pointer is over this drop target, run its drop actions and clear the front end's dragged-component word; then the hoverable handling
// Leaf: ?ProcessEvent@CDraggableInto
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00558D90, 70 bytes (`ret 4`, +10 int3 padding); slot 1 of the CDraggableInto
//          CObserver vtable 0x0124B2B0, so `this` is the CObserver subobject at +4 (members at -4 from their object offsets).
// Identity: FableWin.pdb ?ProcessEvent@CDraggableInto@NUISystem@@UAEXW4EEvent@2@@Z; RTTI 0x0133AE90 gives CDraggableInto :
//          CHoverable : CChangingStateComponent : CComponent with CObserver at +4 and CActionParam at +0x18 (COLs 0x0133AEA0 /
//          0x0133AE44 / 0x0133AE30), as the landed CDraggable::ProcessEvent @ 0x0055A510 models its own chain.
// Callees: the primary vtable's slot 131 (+0x20C) = ExecuteActions(list*) (CDraggableInto's override is 0x00558DE0;
//          FableWin.pdb ?ExecuteActions@CDraggableInto@NUISystem@@MAEXPAV?$list@...@@@Z), CFrontEndManager::GetInstance
//          @ 0x0041E5F2 (static `__fastcall`, no arguments), CHoverable::ProcessEvent @ 0x0055B9D0.
// Layout: retail CHoverable WasHovered +0x160 and CDraggableInto PActions (list pointer) +0x16C -- Ego_r.pdb +0x190 /
//          +0x19C, a constant -0x30; the front-end manager word +0xBC is unnamed here (role inferred from the drop: the
//          component being dragged).
// Shape: the event stays in edi for the base call; `event == 0x1E && WasHovered`; ExecuteActions is a virtual call on the
//          primary base through this-4.
struct CFrontEndManager {
    unsigned char _pad[0xbc];
    void*         PDraggedComponent;               // +0xBC (role inferred)
    static CFrontEndManager* __fastcall GetInstance();   // 0x0041E5F2
};
struct CActionList;                                // list<CCountedPointer<NUISystem::CAction>>

namespace NUISystem {

enum EEvent { EVENT_MOUSE_MOVED = 0x19, EVENT_DROP = 0x1e };   // 0x1E: name inferred from this handler

struct CComponentInterface {                       // primary vtable (CDraggableInto: 0x0124B2D4)
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
    virtual void v128(); virtual void v129(); virtual void v130();
    virtual void ExecuteActions(CActionList* actions);   // slot 131 (+0x20C)
};

struct CObserver_Retail {                          // vtable at +4 (CDraggableInto: 0x0124B2B0)
    virtual void v00();
    virtual void ProcessEvent(EEvent event);       // slot 1
    unsigned char _body[0x10];                     // EventsToObserve (12-byte set), PreventObservation
};

struct CActionParamInterface {                     // vtable at +0x18 (CDraggableInto: 0x0124B2A8)
    virtual void v00();
};

struct CHoverable : CComponentInterface, CObserver_Retail, CActionParamInterface {
    unsigned char _body1c[0x160 - 0x1c];
    bool          WasHovered;                      // +0x160
    unsigned char _pad161[0x16c - 0x161];          // PActionsOnHovered, PActionsOnUnhovered
    virtual void ProcessEvent(EEvent event);       // 0x0055B9D0
};

struct CDraggableInto : CHoverable {
    CActionList* PActions;                         // +0x16C
    virtual void ProcessEvent(EEvent event);
};

void CDraggableInto::ProcessEvent(EEvent event)
{
    if (event == EVENT_DROP && WasHovered) {
        ExecuteActions(PActions);
        CFrontEndManager::GetInstance()->PDraggedComponent = 0;
    }
    CHoverable::ProcessEvent(event);
}

}