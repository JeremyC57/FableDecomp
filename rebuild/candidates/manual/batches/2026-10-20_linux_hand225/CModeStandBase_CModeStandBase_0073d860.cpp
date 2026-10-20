// CModeStandBase::~CModeStandBase() -- release the six stand/walk/jog/run animation counted pointers (last first), destroy PoseAnimName, then the inlined CModeBase destructor (weapon filter's group string and both vtable stores)
// Leaf: ??1CModeStandBase
// Retail: Fable.exe (TLC, VC7.1) @ 0x0073D860, 278 bytes (`ret`).
// Callees: operator delete @ 0x00BFE9BC (each count block), ~CCharString @ 0x0099EAE0 (PoseAnimName, the filter's group).
// Vtables stored: 0x012673CC (CTCCreatureModeManager::CModeBase), 0x0122FFCC (CAnimFilterBase, the filter at +8) -- never
// CModeStandBase's own.
// Layout: Ego_r.pdb CModeStandBase (0x68; rebuild/include/engine/CModeStandBase.h has the same offsets as a flat record,
// so the base/novtable chain is modelled locally as in the landed CModeSleepBed dtor 0x0084B840): IdleCounter +0x1c,
// FramesToKeepUpdatingMovement +0x20, ForceAddNewAnims +0x24, Max{Run,Jog,Walk}Speed +0x28..+0x30, PoseAnimName +0x34,
// PStand/PSlowWalk/PWalk/PJog/PRun/PSprintAnimation +0x38..+0x64.
// Shape: CCountedPointer's inline destructor (re-read the block after the decrement, destroy through the block's
// pointer-to-member, delete the block, clear both words); a NON-virtual destructor on a `__declspec(novtable)` class.
void __cdecl operator delete(void* block);          // 0x00BFE9BC

struct CCharString {
    void* m_pData;
    ~CCharString();                                  // 0x0099EAE0
};

struct CCountedItem { };
struct SCountBlock {
    long m_references;                               // +0x00
    void (CCountedItem::*m_pfnDestroy)();            // +0x04 (called with the item as `this`)
    CCountedItem* m_pItem;                           // +0x08
};

template <class T>
struct CCountedPointer {
    T* m_pObject;                                    // +0x00
    SCountBlock* m_pBlock;                           // +0x04
    CCountedPointer() : m_pObject(0), m_pBlock(0) { }
    ~CCountedPointer()
    {
        if (m_pBlock) {
            --m_pBlock->m_references;
            if (m_pBlock->m_references == 0) {
                (m_pBlock->m_pItem->*m_pBlock->m_pfnDestroy)();
                operator delete(m_pBlock);
            }
        }
        m_pObject = 0;
        m_pBlock = 0;
    }
};

struct CAnimFilterBase {                             // vtable 0x0122FFCC
    virtual void v00();
    ~CAnimFilterBase() { }
};

struct CAnimFilterOfGroup : CAnimFilterBase {        // vtable 0x01267568 (not stored by the implicit destructor)
    CCharString   Group;                             // +4
    unsigned long GroupCRC;                          // +8
};

struct CThing;
struct CModeBase {                                   // CTCCreatureModeManager::CModeBase, vtable 0x012673CC
    virtual void v00();
    CThing*            ParentThing;                  // +0x04
    CAnimFilterOfGroup WeaponBasedFilter;            // +0x08
    long               FramesBeforeRemovingSelf;     // +0x14
    bool               Active;                       // +0x18
    ~CModeBase() { }
};

namespace NAnimationEntryBodge { struct CAnimEntryLite; }

struct __declspec(novtable) CModeStandBase : CModeBase {
    long          IdleCounter;                       // +0x1c
    long          FramesToKeepUpdatingMovement;      // +0x20
    bool          ForceAddNewAnims;                  // +0x24
    float         MaxRunSpeed;                       // +0x28
    float         MaxJogSpeed;                       // +0x2c
    float         MaxWalkSpeed;                      // +0x30
    CCharString   PoseAnimName;                      // +0x34
    CCountedPointer<NAnimationEntryBodge::CAnimEntryLite> PStandAnimation;      // +0x38
    CCountedPointer<NAnimationEntryBodge::CAnimEntryLite> PSlowWalkAnimation;   // +0x40
    CCountedPointer<NAnimationEntryBodge::CAnimEntryLite> PWalkAnimation;       // +0x48
    CCountedPointer<NAnimationEntryBodge::CAnimEntryLite> PJogAnimation;        // +0x50
    CCountedPointer<NAnimationEntryBodge::CAnimEntryLite> PRunAnimation;        // +0x58
    CCountedPointer<NAnimationEntryBodge::CAnimEntryLite> PSprintAnimation;     // +0x60
    ~CModeStandBase();
};

CModeStandBase::~CModeStandBase()
{
}
