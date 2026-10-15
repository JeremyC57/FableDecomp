// CTCBetting::CTCBetting(CThing* thing) -- CTCBase(thing), own vtable, two empty primitive-handle vectors resized to 5, Active cleared, the frame counters and PreviousDonation zeroed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E57C0, length 81 bytes (+ int3 padding).
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, vector<CEnginePrimitiveHandle>::Resize(long) @ 0x00443F90
// (x2, count 5; the manifest label "CEnginePrimitiveRenderer::AddPrimitive" is propagated).
// Vtable 0x01270624 = CTCBetting (rtti-vtables.tsv).
// Layout = Ego_r.pdb CTCBetting with the retail 12-byte vectors: Active +0xc, BetHandles +0x10,
// MoneyHandles +0x1c, FrameBetWasLastChanged +0x28, FrameBetChangeStarted +0x2c, Mult +0x30,
// FrameToStart +0x34, PreviousDonation +0x38 (PDB +0x30..+0x40, 8 higher).
// Shape: the vtable store precedes the vector zeroing (inline member constructors through each
// vector's address register, all sharing the ebx zero); Active is the first BODY assignment (an
// initialiser stores it before the vectors), then the Resize calls and the three counter zeroes (Mult and FrameToStart are left untouched).
struct CThing;

struct CTCBase_Calls {                                      // engine/CTCBase.h layout (0xc bytes)
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    CTCBase_Calls(CThing* thing);                           // 0x00686800
};

struct CEnginePrimitiveHandle;

struct CRetailHandleVector {                                // VC6 Dinkumware vector<CEnginePrimitiveHandle>: three words
    CEnginePrimitiveHandle* m_pFirst;
    CEnginePrimitiveHandle* m_pLast;
    CEnginePrimitiveHandle* m_pEnd;
    CRetailHandleVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
    void Resize(long count);                                // 0x00443F90
};

struct CTCBetting : CTCBase_Calls {                         // vtable 0x01270624
    bool                Active;                             // +0x0c
    CRetailHandleVector BetHandles;                         // +0x10
    CRetailHandleVector MoneyHandles;                       // +0x1c
    long                FrameBetWasLastChanged;             // +0x28
    long                FrameBetChangeStarted;              // +0x2c
    long                Mult;                               // +0x30
    long                FrameToStart;                       // +0x34
    long                PreviousDonation;                   // +0x38
    virtual void v00();
    CTCBetting(CThing* thing);
};

CTCBetting::CTCBetting(CThing* thing)
    : CTCBase_Calls(thing)
{
    Active = false;
    BetHandles.Resize(5);
    MoneyHandles.Resize(5);
    FrameBetWasLastChanged = 0;
    FrameBetChangeStarted = 0;
    PreviousDonation = 0;
}
