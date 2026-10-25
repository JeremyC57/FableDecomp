// NUISystem::CDraggableInto::`scalar deleting destructor'(unsigned int flags) -- destroy a drop-target control (delete its owned action list, then the CHoverable base) and free it when flags bit 0 is set
// Leaf: ??_GCDraggableInto
// Retail: Fable.exe (TLC, VC7.1) @ 0x00559040, 95 bytes, `ret 4`. Vtable slot 0 of NUISystem::CDraggableInto 0x0124B2D4
//   (rtti-vtables.tsv); the adjustor thunks 0x00559020 (vtable 0x0124B2B0) and 0x00559030 (0x0124B2A8) jump here.
// Callees: _List_base<CCountedPointer<NUISystem::CAction> >::clear() 0x005356A0, free 0x00BFEA14 (the list's sentinel),
//   operator delete 0x00BFE9BC (the list object, then `this`), NUISystem::CHoverable::~CHoverable 0x0055BC30 (landed).
// Label correction: catalogued as "CHoverable_Destructor"; the vtables are NUISystem::CDraggableInto's.
// Layout: the three-vptr component bases of the landed CHoverable / CKeyRedefiner models (+0x00, +0x04, +0x18);
//   Ego_r.pdb CDraggableInto (0x1a0) puts PActions at +0x19c, retail +0x16c (the CHoverable base is 0x30 bytes smaller
//   in retail, as its other landed members show); the engine headers are flattened PDB blobs, so the bases are local.
// Shape: the user destructor `delete PActions;` -- the member is loaded and null-tested around the three derived vtable
//   stores, the list's inline destructor (STLport `~_List_base`: out-of-line clear, then `if (_M_node) free`) and the
//   operator delete are inlined; then the CHoverable base destructor and the flag-tested delete of the `??_G`.
extern "C" void __cdecl free(void* block);                // 0x00BFEA14
void __cdecl operator delete(void* block);                // 0x00BFE9BC

namespace NUISystem {

class CAction;

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

struct CHoverable : CComponentRoot, CGuiControl, CHoverableBase {
    unsigned char _body[0x16c - 0x1c];                    // +0x01c .. +0x16b (retail)
    virtual ~CHoverable();                                // 0x0055BC30
};

struct CDraggableInto : CHoverable {                      // vtables 0x0124B2D4 / 0x0124B2B0 / 0x0124B2A8
    CActionList* PActions;                                // +0x16c (PDB +0x19c)
    virtual ~CDraggableInto();
};

CDraggableInto::~CDraggableInto()
{
    delete PActions;
}

}