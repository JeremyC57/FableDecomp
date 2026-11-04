// NUISystem::CDraggableInto::~CDraggableInto() -- destroy a drop-target control: delete its owned action list (PActions), then the CHoverable base
// Leaf: ??1CDraggableInto@NUISystem@@UAE@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00558FD0, 74 bytes, tail-jumps to NUISystem::CHoverable::~CHoverable 0x0055BC30.
// Identity: stores the three NUISystem::CDraggableInto vtables 0x0124B2D4 / 0x0124B2B0 (+4) / 0x0124B2A8 (+0x18)
//   (rtti-vtables.tsv; slot 0 of the primary vtable is the landed `scalar deleting destructor' 0x00559040, which inlines
//   this same body); Ego_r.pdb / FableWin.pdb ??1CDraggableInto@NUISystem@@UAE@XZ. Catalog label "~CDraggableInto" right,
//   module "CDraggableInto" missing its namespace.
// Callees: _List_base<CCountedPointer<NUISystem::CAction> >::clear() 0x005356A0, free 0x00BFEA14 (the list's sentinel),
//   operator delete 0x00BFE9BC (the list object), NUISystem::CHoverable::~CHoverable 0x0055BC30 (landed).
// Layout / shape: the landed 0x00559040 model (three-vptr CHoverable base, PActions +0x16c retail = PDB +0x19c minus the
//   0x30 bytes the retail CHoverable base is smaller); the user destructor is `delete PActions;`, the STLport list
//   destructor inline (out-of-line clear, then `if (_M_node) free`).
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

class CHoverable : public CComponentRoot, public CGuiControl, public CHoverableBase { public:
    unsigned char _body[0x16c - 0x1c];                    // +0x01c .. +0x16b (retail)
    virtual ~CHoverable();                                // 0x0055BC30
};

class CDraggableInto : public CHoverable { public:                      // vtables 0x0124B2D4 / 0x0124B2B0 / 0x0124B2A8
    CActionList* PActions;                                // +0x16c (PDB +0x19c)
    virtual ~CDraggableInto();
};

CDraggableInto::~CDraggableInto()
{
    delete PActions;
}

}