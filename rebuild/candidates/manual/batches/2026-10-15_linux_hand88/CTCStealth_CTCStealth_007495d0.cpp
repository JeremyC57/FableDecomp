// CTCStealth::~CTCStealth() -- destroy the danger-things vector (element pointers, then the block), release the stealth def, then the thing-component base
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007495D0, length 80 bytes. Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer
// @ 0x00A01C10 (per 16-byte element), free @ 0x00BFEA14 (the retail allocator), the def's virtual Destroy (slot 1),
// CTCBase::~CTCBase @ 0x00686830 (tail jump; manifest "~CChunkedFileChunk", propagated).
// Label correction: the manifest calls this ~CTCGraphicAppearanceReplaceableMesh; its only caller is 0x007495B0, slot 0 of
// vtable 0x012689EC = CTCStealth (rtti-vtables.tsv), and the PDB CTCStealth fits every touched member.
// Layout (Ego_r.pdb CTCStealth, retail offsets with the 12-byte retail vector): CTCBase +0x00..+0x0b, five bools +0x0c..+0x10,
// DistanceCanBeHeardFrom +0x14, MaxSneakingSpeed +0x18, PDef (CDefPointer<CStealthDef const>) +0x1c, six longs +0x20..+0x34,
// DangerThings (vector<pair<CIntelligentPointer<CThing const>, NStealth::CDangerStatus> >, 16-byte elements)
// first/last/end +0x38/+0x3c/+0x40, MaxNumberOfDangerThings +0x44, RegionBasedMaxNumberOfDangerThings +0x48.
// Shape: the destructor stores no vtable, so the class is `__declspec(novtable)` over CTCBase's out-of-line virtual
// destructor (reached by tail jump); members die in reverse order: the vector (range destroy written with `last`
// evaluated first -- right-to-left argument order -- then the null-tested free of the block), then the def pointer
// (`if (p) { p->Release(); p = 0; }`, the release a member of the def so the pointer stays in ecx).
// The engine header CTCBase.h is a flattened POD (`void* __vftable`), unusable as a polymorphic base: local model.
// Leaf: ??1CTCStealth
#include <stdlib.h>
struct CThing;

struct CTCBase {                                        // 0x0c bytes: vptr, ParentThing, ToBeRemoved, IsInGlobalUpdateTCs
    virtual ~CTCBase();                                 // 0x00686830
    CThing* ParentThing;                                // +0x04
    bool    ToBeRemoved;                                // +0x08
    bool    IsInGlobalUpdateTCs;                        // +0x09
};

struct CBaseIntelligentPointer {                        // 8 bytes: vptr + the pointee link
    void* m_vtable;
    void* m_pItem;
    ~CBaseIntelligentPointer();                         // 0x00A01C10
};

struct SDangerThing {                                   // pair<CIntelligentPointer<CThing const>, NStealth::CDangerStatus>
    CBaseIntelligentPointer first;                      // +0x00
    long                    secondStatus;               // +0x08
    long                    secondTimer;                // +0x0c
};

struct CRetailVector_SDangerThing {                     // retail std::vector: first, last, end (12 bytes)
    SDangerThing* m_pFirst;
    SDangerThing* m_pLast;
    SDangerThing* m_pEnd;
    static void Destroy(SDangerThing* first, SDangerThing* last)
    {
        for (; first != last; ++first)
            first->~SDangerThing();
    }
    ~CRetailVector_SDangerThing()
    {
        Destroy(m_pFirst, m_pLast);
        if (m_pFirst)
            free(m_pFirst);
    }
};

struct CStealthDef {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long m_refCount;                                    // +0x04
    void Release() { if (--m_refCount == 0) Destroy(); }
};

struct CDefPointer_CStealthDef {
    const CStealthDef* m_pDef;
    ~CDefPointer_CStealthDef()
    {
        if (m_pDef) {
            const_cast<CStealthDef*>(m_pDef)->Release();
            m_pDef = 0;
        }
    }
};

struct __declspec(novtable) CTCStealth : CTCBase {
    bool  Viewable;                                     // +0x0c
    bool  Smellable;                                    // +0x0d
    bool  Hearable;                                     // +0x0e
    bool  Sneaking;                                     // +0x0f
    bool  SneakingToggleSet;                            // +0x10
    float DistanceCanBeHeardFrom;                       // +0x14
    float MaxSneakingSpeed;                             // +0x18
    CDefPointer_CStealthDef PDef;                       // +0x1c
    long  FrameStartedWalking;                          // +0x20
    long  FrameStartedSneakingAndMoving;                // +0x24
    long  SneakingAndMovingDeedHandle;                  // +0x28
    long  ActionPermittedToSneakID;                     // +0x2c
    long  RunningAwayFromCombatTimer;                   // +0x30
    long  InDangerCoolOffTimer;                         // +0x34
    CRetailVector_SDangerThing DangerThings;            // +0x38
    long  MaxNumberOfDangerThings;                      // +0x44
    long  RegionBasedMaxNumberOfDangerThings;           // +0x48
    ~CTCStealth();
};

CTCStealth::~CTCStealth()
{
}
