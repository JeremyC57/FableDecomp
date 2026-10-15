// NUISystem::CRedefinerList::`scalar deleting destructor'(unsigned int flags) -- destroy the key-redefinition list (forget it as the active list, then the scrollable-list destructor) and free it when bit 0 is set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00557360, length 70 bytes (`ret 4`). Vtable slot 0 of NUISystem::CRedefinerList
// (0x0124A994); the adjustor thunks 0x00557340 (`sub ecx,4`, vtable 0x0124A970) and 0x00557350 (vtable 0x0124A968) jump here.
// Callees: CScrollableList::~CScrollableList @ 0x00555390, operator delete @ 0x00BFE9BC.
// Layout: three polymorphic bases (vptrs at +0x00, +0x04 and +0x18: the destructor stores CRedefinerList's three vtables
// 0x0124A994 / 0x0124A970 / 0x0124A968 first); the active-list global is 0x013B8AC4.
// Shape: the implicit `??_G` around an out-of-line-callable destructor whose body is `if (s_pActiveList == this)
// s_pActiveList = 0;` (`cmp [global],esi`: the global on the left), followed by the base destructor call; the vtable stores
// precede the body.
// Leaf: ??_GCRedefinerList
namespace NUISystem {

struct CListRoot_00557360 {                             // primary base (vptr at +0x00)
    virtual ~CListRoot_00557360();
};
struct CGuiControl_00557360 {                           // control base (vptr at +0x04)
    virtual ~CGuiControl_00557360();
    unsigned char _body[0x10];                          // +0x08 .. +0x17
};
struct CHoverable_00557360 {                            // third base (vptr at +0x18)
    virtual ~CHoverable_00557360();
};

struct CScrollableList : CListRoot_00557360, CGuiControl_00557360, CHoverable_00557360 {
    unsigned char _body[0x160];                         // +0x01c .. +0x17b
    virtual ~CScrollableList();                         // 0x00555390
};

struct CRedefinerList : CScrollableList {
    bool  m_bActive;                                    // +0x17c
    bool  m_bSelecting;                                 // +0x17d
    float m_fadeTime;                                   // +0x180
    static CRedefinerList* s_pActiveList;               // 0x013B8AC4
    virtual ~CRedefinerList();
};

CRedefinerList::~CRedefinerList()
{
    if (s_pActiveList == this)
        s_pActiveList = 0;
}

}
