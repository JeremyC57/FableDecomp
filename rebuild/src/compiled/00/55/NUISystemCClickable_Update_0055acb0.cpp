// NUISystem::CClickable::Update(float deltaTime) -- while the component is not hovered, clear the left- and right-click latches; then the CHoverable update
// Leaf: ?Update@CClickable@NUISystem@@UAEXM@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0055ACB0, 49 bytes, thiscall, tail `jmp CHoverable::Update`.
// Identity (proven): slot 1 of the vtable 0x0124BD2C whose RTTI COL names .?AVCClickable@NUISystem@@ (read from Fable.exe),
//   also inherited at slot 1 by CFrontEndButton 0x01249554, CKeyRedefiner 0x0124ADBC, CNavButton 0x0124B04C and
//   CScrollBarOutside 0x0124B54C; Ego_r.pdb ?Update@CClickable@NUISystem@@UAEXM@Z, and the callee is ?Update@CHoverable@NUISystem@@
//   UAEXM@Z (0x0055B890, slot 1 of CDraggableInto 0x0124B2D4, which inherits it). Donor order agrees (Ego_r CClickable::Update
//   0x005D4100 < CHoverable::Update 0x005D4EE0; retail 0x0055ACB0 < 0x0055B890). Bound spellings
//   ?Update@CDraggable_Retail@NUISystem@@UAEXM@Z and ?UpdateClickable@CDraggable@NUISystem@@QAEXM@Z are invented (NAME_FIXES).
// Shape: two `if (!WasHovered && WasX) WasX = false;` statements: WasHovered is read once into al and VC7.1 threads the
//   hovered case of the first test straight to the tail call; each latch is stored only when set (the `cmp ; je ; mov` pairs).
// Layout (Ego_r.pdb less 0x30 from CChangingStateComponent on, as engine/CHoverable.h has it with STLport sizes):
//   CHoverable StateBeforeHovered +0x15C, WasHovered +0x160, PActionsOnHovered +0x164, PActionsOnUnhovered +0x168 (0x16C);
//   CClickable StateBeforeLeftClicked +0x16C, WasLeftClicked +0x170, PActionsOnLeftClicked/Held/Unclicked +0x174..+0x17C,
//   StateBeforeRightClicked +0x180, WasRightClicked +0x184, PActionsOnRightClicked +0x188, InputDelay +0x18C,
//   LastInputTime +0x190 (0x194; the landed CScrollBar::Update 0x00559600 reads WasLeftClicked +0x170, CDraggable starts +0x194).
//   Local model: engine/CHoverable.h is a flat struct (vptr as a data member) that cannot carry the virtual Update.
namespace NUISystem {
class CAction;
}
template <class T> class CCountedPointer;
namespace std {
template <class _Tp> class allocator;
template <class _Tp, class _Alloc> class list;
}

namespace NUISystem {
typedef std::list<CCountedPointer<CAction>, std::allocator<CCountedPointer<CAction> > > CActionList;

class CHoverable {
public:
    virtual ~CHoverable();                                   // slot 0
    virtual void Update(float deltaTime);                    // slot 1  0x0055B890
    unsigned char _pad_0x04[0x158];                          // +0x04 CChangingStateComponent / CComponent / observer bases
    long          StateBeforeHovered;                        // +0x15C
    bool          WasHovered;                                // +0x160
    CActionList*  PActionsOnHovered;                         // +0x164
    CActionList*  PActionsOnUnhovered;                       // +0x168
};

class CClickable : public CHoverable {                       // vtable 0x0124BD2C
public:
    virtual void Update(float deltaTime);
    long          StateBeforeLeftClicked;                    // +0x16C
    bool          WasLeftClicked;                            // +0x170
    CActionList*  PActionsOnLeftClicked;                     // +0x174
    CActionList*  PActionsOnLeftHeld;                        // +0x178
    CActionList*  PActionsOnLeftUnclicked;                   // +0x17C
    long          StateBeforeRightClicked;                   // +0x180
    bool          WasRightClicked;                           // +0x184
    CActionList*  PActionsOnRightClicked;                    // +0x188
    float         InputDelay;                                // +0x18C
    float         LastInputTime;                             // +0x190
};

void CClickable::Update(float deltaTime)
{
    if (!WasHovered && WasLeftClicked)
        WasLeftClicked = false;
    if (!WasHovered && WasRightClicked)
        WasRightClicked = false;
    CHoverable::Update(deltaTime);
}
}