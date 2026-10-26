// NCreatureMode::CModeSleepBed::~CModeSleepBed() -- release PRemoveAction, destroy SleepAnimName and the filter, then the inlined CModeBase destructor
// Leaf: ??1CModeSleepBed
// Retail: Fable.exe (TLC, VC7.1) @ 0x0084B840, 98 bytes (`ret`).
// Label: the catalog's "CModeIntangible::~CModeIntangible" is a byte-twin propagation; the offsets (counted pointer
// +0x2c/+0x30, string +0x28, filter +0x1c with its string at +0x20) are CModeSleepBed's (Ego_r.pdb; constructors
// 0x0084B7C0 / 0x008D7CD0).
// Callees: operator delete @ 0x00BFE9BC (the count block), ~CCharString @ 0x0099EAE0 (three strings).
// Vtables stored: 0x0122FFCC (CAnimFilterBase, the filters at +0x1c and +8), 0x012673CC (CModeBase) -- never the
// derived 0x01275760.
// Shape: the counted pointer's inline destructor (re-read the block after the decrement, destroy through the
// block's pointer-to-member, delete the block, clear both words); filters destroyed through the implicit
// CAnimFilterOfGroup destructor (string, then the base's vtable store); a NON-virtual destructor chain, so the most
// derived vtable is never re-stored.
void __cdecl operator delete(void* block);          // 0x00BFE9BC
extern "C" void __cdecl free(void* memory);         // 0x00BFEA14

class CCharString { public:
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
class CCountedPointer { public:
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

class CAnimFilterBase { public:                             // vtable 0x0122FFCC
    virtual void v00();
    ~CAnimFilterBase() { }
};

class CAnimFilterOfGroup : public CAnimFilterBase { public:        // vtable 0x01267568 (not stored by the implicit destructor)
    CCharString   Group;                             // +4
    unsigned long GroupCRC;                          // +8
};

class CThing;
struct CModeBase {                                   // CTCCreatureModeManager::CModeBase, vtable 0x012673CC
    virtual void v00();
    CThing*            ParentThing;                  // +0x04
    CAnimFilterOfGroup WeaponBasedFilter;            // +0x08
    long               FramesBeforeRemovingSelf;     // +0x14
    bool               Active;                       // +0x18
    ~CModeBase() { }
};

class CCreatureActionBase;

namespace NCreatureMode {

struct __declspec(novtable) CModeSleepBed : CModeBase {   // vtable 0x01275760 (novtable here: the destructor never re-stores it)
    CAnimFilterOfGroup                    Filter;          // +0x1c
    CCharString                           SleepAnimName;   // +0x28
    CCountedPointer<CCreatureActionBase>  PRemoveAction;   // +0x2c
    ~CModeSleepBed();
};

CModeSleepBed::~CModeSleepBed()
{
}

}