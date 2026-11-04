// [thunk] NUISystem::CDraggableInto::`vector deleting destructor' `adjustor{24}'(unsigned int) -- secondary-vtable (+0x18) entry: this -= 0x18, jump to the deleting destructor 0x00559040
// Leaf: ??_ECDraggableInto@NUISystem@@WBI@AEPAXI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00559030, 8 bytes: `sub ecx,0x18 ; jmp 0x00559040'. Slot 0 of NUISystem::CDraggableInto's vtable 0x0124B2A8
//   (rtti-vtables.tsv; the catalog's "vfunc_0"). Compiler-generated adjustor thunk: the class's virtual destructor overrides the
//   destructor of the base whose vptr sits at +0x18, so that base's vtable gets a thunk that rebases `this' to the full object and
//   tail-jumps to NUISystem::CDraggableInto::`scalar deleting destructor' 0x00559040 (landed, slot 0 of the primary vtable 0x0124B2D4).
//   Thunks are not in the PDB name sets (no symbol records); the spelling is VC7.1's own: the vtable entry is the weak ??_E alias of the ??_G, so the thunk is ??_E...W<adj>AE and jumps to ??_E (resolved to ??_G 0x00559040; no delete[] of the class exists, so no vector form).
// Model: the landed 0x00559040 source (defining the destructor emits the vtables, the ??_G and both thunks in this TU).
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