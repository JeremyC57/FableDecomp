// CTCBuyableHouse::~CTCBuyableHouse() -- destroy the wife and sign-post thing pointers and the def pointer, then CTCBase
// Leaf: ??1CTCBuyableHouse
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C1BE0, length 59 bytes (+5 int3; speed-optimised: the def pointer's zero
// store is `mov dword ptr [esi+0Ch],0`). Called by the scalar deleting destructor 0x006C1BC0 (CTCBuyableHouse vtable
// 0x0125D6A4 slot 0); the manifest label "~CTCDExplosion" is propagated.
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10 (x3), CTCBase::~CTCBase @ 0x00686830 (tail jump).
// Shape: reverse member order (PDB names and offsets, identical in retail): WifeLivingHere +0x38, DressingSignPost +0x2c,
// MainSignPost +0x24, then the inlined CDefPointer release of PDef (+0x0c).
#define FORCEINLINE inline

struct CThing;

struct CTCBase {                            // CTCBase root: vptr, ParentThing, two flags (0x0c bytes)
    void* __vftable;                        // +0x00
    CThing* ParentThing;                    // +0x04
    bool ToBeRemoved;                       // +0x08
    bool IsInGlobalUpdateTCs;               // +0x09
    ~CTCBase();                             // 0x00686830 (stores the CTCBase vtable, then the CBaseObject destructor)
};

struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy();                 // slot 1
    long RefCount;                          // +0x04
};

struct CDefPointer {                        // CDefPointer<T>: the pointee's count at +4, destroyed through slot 1
    CDefPointeeBase* Object;
    FORCEINLINE ~CDefPointer()
    {
        CDefPointeeBase* object = Object;
        if (object) {
            if (--object->RefCount == 0)
                object->Destroy();
            Object = 0;
        }
    }
};

struct CIntelligentPointer {                // CIntelligentPointer<CThing>
    void* __vftable;                        // +0x00
    void* PData;                            // +0x04
    ~CIntelligentPointer();                 // 0x00A01C10 (CBaseIntelligentPointer::~CBaseIntelligentPointer)
};

struct CTCBuyableHouse : CTCBase {
    CDefPointer PDef;                       // +0x0c
    bool IsScripted;                        // +0x10
    bool OwnedByPlayer;                     // +0x11
    bool Rented;                            // +0x12
    long DaySomeoneWillMoveIn;              // +0x14
    long DayNextRentIsDue;                  // +0x18
    long CurrentDressLevel;                 // +0x1c
    long MaxDressLevel;                     // +0x20
    CIntelligentPointer MainSignPost;       // +0x24
    CIntelligentPointer DressingSignPost;   // +0x2c
    long FrameToNextCheckIfWeNeedToCreateAnything;  // +0x34
    CIntelligentPointer WifeLivingHere;     // +0x38
    bool InitiallySetupDressLevel;          // +0x40
    long VirtualMoneyBags;                  // +0x44
    bool HasPreviouslyBeenBuyable;          // +0x48
    bool IsResidential;                     // +0x49
    ~CTCBuyableHouse();
};

CTCBuyableHouse::~CTCBuyableHouse()
{
}