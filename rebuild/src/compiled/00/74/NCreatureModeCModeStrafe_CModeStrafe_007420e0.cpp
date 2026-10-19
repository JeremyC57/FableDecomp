// NCreatureMode::CModeStrafe::~CModeStrafe() -- release the four projectile-anim counted pointers, destroy the filter and the strafe anim name, then the inlined CModeBase destructor
// Leaf: ??1CModeStrafe
// Retail: Fable.exe (TLC, VC7.1) @ 0x007420E0, 210 bytes (`ret`).
// Callees: operator delete 0x00BFE9BC (the count block), ~CCharString 0x0099EAE0 (filter group, StrafeAnimName,
// the base's WeaponBasedFilter group).
// Vtables stored: 0x0122FFCC (CAnimFilterBase, at +0x2c and +8; held in edi), 0x012673CC (CModeBase); the class's own
// vtable 0x01268588 is not re-stored (modelled `__declspec(novtable)` with a non-virtual destructor, as the landed
// CModeDragonFlying destructor 0x00743000).
// Layout (PDB 0x70, matches retail; see the ctor 0x00741FE0): StrafeAnimName +0x20, Filter +0x2c,
// PAnimProjectileAim +0x50, PAnimProjectileBuildUp +0x58, PAnimProjectilePullBack +0x60, PAnimStrafeUpperBody +0x68.
// Shape: each CCountedPointer dtor drops a reference, calls the block's member-function destroy on the item when it
// reaches zero, deletes the block and zeroes both words; members are destroyed in reverse order.
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

namespace NCreatureMode {

struct __declspec(novtable) CModeStrafe : CModeBase {   // vtable 0x01268588
    float              AnimMovementDistance;   // +0x1c
    CCharString        StrafeAnimName;         // +0x20
    long               AnimIndex;              // +0x24
    bool               LastUsable;             // +0x28
    CAnimFilterOfGroup Filter;                 // +0x2c
    long               ActiveWeapon;           // +0x38
    long               AimingNumFrames;        // +0x3c
    long               AimingCenterFrame;      // +0x40
    float              RumbleParam;            // +0x44
    float              PullBackAmount;         // +0x48
    bool               ForceAnimNoBlendIn;     // +0x4c
    CCountedPointer<NAnimationEntryBodge::CAnimEntryLite> PAnimProjectileAim;       // +0x50
    CCountedPointer<NAnimationEntryBodge::CAnimEntryLite> PAnimProjectileBuildUp;   // +0x58
    CCountedPointer<NAnimationEntryBodge::CAnimEntryLite> PAnimProjectilePullBack;  // +0x60
    CCountedPointer<NAnimationEntryBodge::CAnimEntryLite> PAnimStrafeUpperBody;     // +0x68
    ~CModeStrafe();
};

CModeStrafe::~CModeStrafe()
{
}

}