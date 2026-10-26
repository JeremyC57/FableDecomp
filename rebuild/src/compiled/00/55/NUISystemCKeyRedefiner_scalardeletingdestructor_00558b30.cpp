// NUISystem::CKeyRedefiner::`scalar deleting destructor'(unsigned int flags) -- destroy a key-redefiner control (drop it as the selected redefiner, destroy its list member, then the CClickable base) and free it when bit 0 is set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00558B30, length 81 bytes (`ret 4`). Vtable slot 0 of NUISystem::CKeyRedefiner
// (0x0124ADBC, rtti-vtables.tsv); the adjustor thunks 0x00558B10 (vtable 0x0124AD98) and 0x00558B20 (0x0124AD90) jump here.
// Callees: the list member's destructor @ 0x00558AA0 (frees every node, relinks and frees the head), CClickable::~CClickable
// @ 0x0055B760 (stores CClickable's vtables 0x0124BD2C/..08/..00), operator delete @ 0x00BFE9BC.
// Label correction: the manifest calls this a CRedefinerList `vector_deleting_destructor' (propagated); its vtables are
// NUISystem::CKeyRedefiner's. The selected-redefiner global 0x013B8AC8 is the one the landed CKeyRedefiner::CancelSelection
// @ 0x00557BD0 clears.
// Layout: CClickable carries the three vptrs (+0x00, +0x04, +0x18); the list member sits at +0x19c.
// Shape: the derived vtable stores, the body `if (s_pSelected == this) s_pSelected = 0;` (global on the left), the member
// destructor (`lea ecx,[esi+19Ch]`), the base destructor, then the flag-tested delete.
// Leaf: ??_GCKeyRedefiner
namespace NUISystem {

struct CComponentRoot_00558b30 {                        // primary base (vptr at +0x00)
    virtual ~CComponentRoot_00558b30();
};
struct CGuiControl_00558b30 {                           // control base (vptr at +0x04)
    virtual ~CGuiControl_00558b30();
    unsigned char _body[0x10];                          // +0x08 .. +0x17
};
struct CHoverable_00558b30 {                            // third base (vptr at +0x18)
    virtual ~CHoverable_00558b30();
};

class CClickable : public CComponentRoot_00558b30, public CGuiControl_00558b30, public CHoverable_00558b30 { public:
    unsigned char _body[0x180];                         // +0x01c .. +0x19b
    virtual ~CClickable();                              // 0x0055B760
};

struct CKeyList_00558b30 {                              // VC6-era list: head node pointer, count
    void*         m_pHead;
    unsigned long m_size;
    ~CKeyList_00558b30();                               // 0x00558AA0
};

class CKeyRedefiner : public CClickable { public:
    CKeyList_00558b30     m_keys;                       // +0x19c
    static CKeyRedefiner* s_pSelected;                  // 0x013B8AC8
    virtual ~CKeyRedefiner();
};

CKeyRedefiner::~CKeyRedefiner()
{
    if (s_pSelected == this)
        s_pSelected = 0;
}

}