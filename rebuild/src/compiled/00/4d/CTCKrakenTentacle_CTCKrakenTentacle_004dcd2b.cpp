// CTCKrakenTentacle::~CTCKrakenTentacle() -- destroy the dummy name, the three thing pointers and the def pointer, then CTCBase
// Leaf: ??1CTCKrakenTentacle
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DCD2B, length 64 bytes (size-optimised def/factory module: `and dword ptr
// [esi+0Ch],0` -> #pragma optimize("s", on)). Called by the scalar deleting destructor 0x004DCD0F (CTCKrakenTentacle vtable
// 0x012407BC slot 0); the manifest label "~CTCDExplosion" is propagated.
// Callees: CCharString::~CCharString @ 0x0099EAE0, CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10 (x3),
// CTCBase::~CTCBase @ 0x00686830 (tail jump).
// Shape: members in reverse declaration order (PDB names and offsets, identical in retail): DummyName +0x48,
// PRippleEffect +0x20, PObstruction +0x18, PKraken +0x10, then the inlined CDefPointer release of PTentacleDef +0x0c
// (`dec [ecx+4] ; jne ; call [vt+4]`, then the zero store). No vtable store of its own: the model carries no virtuals
// (the destructor is the plain ??1 the deleting destructor calls).
#pragma optimize("s", on)
#define FORCEINLINE __forceinline

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

struct CCharString {
    void* PStringData;                      // +0x00
    ~CCharString();                         // 0x0099EAE0
};

struct C3DVector { float X, Y, Z; };

struct CTCKrakenTentacle : CTCBase {
    CDefPointer PTentacleDef;               // +0x0c
    CIntelligentPointer PKraken;            // +0x10
    CIntelligentPointer PObstruction;       // +0x18
    CIntelligentPointer PRippleEffect;      // +0x20
    bool CanStrike;                         // +0x28
    bool Tracking;                          // +0x29
    bool IsStriking;                        // +0x2a
    long QueuedStrikeTick;                  // +0x2c
    long LastStrikeTick;                    // +0x30
    long TickToRise;                        // +0x34
    C3DVector DesiredPos;                   // +0x38
    float FacingAngle;                      // +0x44
    CCharString DummyName;                  // +0x48
    bool DoneDeathStuff;                    // +0x4c
    ~CTCKrakenTentacle();
};

CTCKrakenTentacle::~CTCKrakenTentacle()
{
}