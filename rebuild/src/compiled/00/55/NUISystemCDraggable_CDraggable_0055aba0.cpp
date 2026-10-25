// NUISystem::CDraggable::~CDraggable() -- destroy a draggable control: delete its two owned action lists (on dropped, on dropped nowhere), then the CClickable base destructor (tail jump)
// Leaf: ??1CDraggable@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0055ABA0, 115 bytes; ends in `jmp` to the base destructor.
// Callees: _List_base<CCountedPointer<NUISystem::CAction> >::clear() 0x005356A0, free 0x00BFEA14 (each list's sentinel),
//   operator delete 0x00BFE9BC (each list object), NUISystem::CClickable::~CClickable 0x0055B760 (as named by the landed
//   CKeyRedefiner deleting destructor 0x00558B30).
// Callers: NUISystem::CDraggable's scalar deleting destructor 0x0055AC70 (vtable 0x0124BA94 slot 0) and 0x00559A7E (a jmp).
// Label correction: catalogued as "CHoverable_VectorDeleting_Destructor"; it is the plain destructor (`??1`) of
//   NUISystem::CDraggable (vtables 0x0124BA94 / 0x0124BA70 / 0x0124BA68 in rtti-vtables.tsv).
// Layout: the three-vptr component bases (+0x00, +0x04, +0x18) of the landed CClickable model; Ego_r.pdb CDraggable
//   (0x1dc): PositionBeforeDrag +0x1c4, WasDragged +0x1cc, PActionsOnDropped +0x1d0, PActionsOnDroppedNowhere +0x1d4,
//   SnapBackIfDroppedNowhere +0x1d8, ChangeLayer +0x1d9 -> retail 0x30 lower (the lists at +0x1a0 / +0x1a4).
// Shape: `delete PActionsOnDropped; delete PActionsOnDroppedNowhere;` -- each member loaded into edi and null-tested around
//   the derived vtable stores (the first) / on its own (the second), the inline STLport list destructor (out-of-line
//   clear, `if (_M_node) free`) and operator delete inlined; the base destructor is a tail jump.
extern "C" void __cdecl free(void* block);                // 0x00BFEA14
void __cdecl operator delete(void* block);                // 0x00BFE9BC

namespace NUISystem {

class CAction;

struct C2DVector {
    float X, Y;
};

struct CActionList {                                      // std::list<CCountedPointer<NUISystem::CAction> >
    void* _M_node;                                        // +0x00 the sentinel node
    void clear();                                         // 0x005356A0
    ~CActionList()
    {
        clear();
        if (_M_node != 0)
            free(_M_node);
    }
};

struct CComponentRoot {                                   // primary base (vptr at +0x00)
    virtual ~CComponentRoot();
};
struct CGuiControl {                                      // control base (vptr at +0x04)
    virtual ~CGuiControl();
    unsigned char _body[0x10];                            // +0x08 .. +0x17
};
struct CHoverableBase {                                   // third base (vptr at +0x18)
    virtual ~CHoverableBase();
};

struct CClickable : CComponentRoot, CGuiControl, CHoverableBase {
    unsigned char _body[0x194 - 0x1c];                    // +0x01c .. +0x193 (retail)
    virtual ~CClickable();                                // 0x0055B760
};

struct CDraggable : CClickable {                          // vtables 0x0124BA94 / 0x0124BA70 / 0x0124BA68
    C2DVector    PositionBeforeDrag;                      // +0x194
    bool         WasDragged;                              // +0x19c
    CActionList* PActionsOnDropped;                       // +0x1a0
    CActionList* PActionsOnDroppedNowhere;                // +0x1a4
    bool         SnapBackIfDroppedNowhere;                // +0x1a8
    bool         ChangeLayer;                             // +0x1a9
    virtual ~CDraggable();
};

CDraggable::~CDraggable()
{
    delete PActionsOnDropped;
    delete PActionsOnDroppedNowhere;
}

}