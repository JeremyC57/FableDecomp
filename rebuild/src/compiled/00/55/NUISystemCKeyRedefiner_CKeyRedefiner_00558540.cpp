// NUISystem::CKeyRedefiner::CKeyRedefiner(CComponentManager* manager) -- CClickable(manager), no action/key text, an empty action list, sub type 0 and a cleared input definition
// Leaf: ??0CKeyRedefiner
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00558540, length 130 bytes (`ret 4`).
// Label correction: the manifest calls this CTCTavernGameCardBase::CTCTavernGameCardBase (propagated); it stores NUISystem::CKeyRedefiner's
// three vtables 0x0124ADBC (+0x00), 0x0124AD98 (+0x04) and 0x0124AD90 (+0x18) (rtti-vtables.tsv; the same set the landed scalar deleting
// destructor 0x00558B30 uses).
// Callees: NUISystem::CClickable::CClickable(...) @ 0x0055B460 (one argument), malloc @ 0x00BFEA0E (the action list's 12-byte sentinel node).
// Layout (Ego_r.pdb CKeyRedefiner, retail with the 0x194-byte retail CClickable): PActionText +0x194, PKeyText +0x198, Actions
// (list<EGameAction>, one sentinel pointer) +0x19C, SubType +0x1A0, Definition (CActionInputControl, 28 bytes: GameAction, ControllerType,
// KeyboardKey, XboxButton, MouseButton, ControlDirection) +0x1A4..+0x1BF.
// Shape: init list in declaration order with one zero register (edi); the list ctor nulls its head, allocates and self-links the node, then
// stores it; the input definition's inline ctor zeroes all seven words (the C2DVector through its two-float ctor).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
namespace NUISystem {
struct CComponentManager;
struct CText;
struct CComponentRoot {                         // primary base (vptr at +0x00)
    virtual ~CComponentRoot();
};
struct CGuiControl {                            // control base (vptr at +0x04)
    virtual ~CGuiControl();
    unsigned char _body[0x10];                  // +0x08 .. +0x17
};
struct CHoverable {                             // third base (vptr at +0x18)
    virtual ~CHoverable();
};
struct CClickable : CComponentRoot, CGuiControl, CHoverable {
    unsigned char _body[0x194 - 0x1c];          // +0x01c .. +0x193
    CClickable(CComponentManager* manager);     // 0x0055B460
    virtual ~CClickable();
};
struct CActionList {                            // std::list<EGameAction>: one sentinel pointer
    struct node { node* next; node* prev; long value; };
    node* head;
    CActionList() : head(0)
    {
        node* sentinel = (node*)malloc(sizeof(node));
        sentinel->next = sentinel;
        sentinel->prev = sentinel;
        head = sentinel;
    }
};
}
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
};
struct CActionInputControl {                    // 28 bytes
    long GameAction;                            // +0x00
    long ControllerType;                        // +0x04
    long KeyboardKey;                           // +0x08
    long XboxButton;                            // +0x0c
    long MouseButton;                           // +0x10
    C2DVector ControlDirection;                 // +0x14
    CActionInputControl()
        : GameAction(0), ControllerType(0), KeyboardKey(0), XboxButton(0), MouseButton(0), ControlDirection(0, 0) { }
};
namespace NUISystem {
struct CKeyRedefiner : CClickable {             // vtables 0x0124ADBC / 0x0124AD98 / 0x0124AD90
    CText* PActionText;                         // +0x194
    CText* PKeyText;                            // +0x198
    CActionList Actions;                        // +0x19c
    long SubType;                               // +0x1a0 EGameActionSubType
    CActionInputControl Definition;             // +0x1a4
    CKeyRedefiner(CComponentManager* manager);
    virtual ~CKeyRedefiner();
};

CKeyRedefiner::CKeyRedefiner(CComponentManager* manager)
    : CClickable(manager)
    , PActionText(0)
    , PKeyText(0)
    , SubType(0)
{
}
}