// CMouseCursor::Initialize() -- take an input snapshot from the cursor's input source, then hand the pointer turn (TurnToPointer) and the snapshot's pressed state for 0 to the renderer object at the end of the cursor's owner chain
// Leaf: Initialize
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C44AB0, 134 bytes (`ret`).
// Callees: the snapshot's two out-of-line part ctors @ 0x00C203A0 (0x1c bytes) / 0x00C20390 (0x10 bytes) (catalog labels
// "CAtmosProcess" / "CNavigationLayer" are propagated), the input source's GetSnapshot(out, 0, 0) @ 0x00C383D0
// (returns `out`, `ret 0Ch`), the snapshot's GetPressed(long) @ 0x00C27BC0, CMouseCursor::TurnToPointer @ 0x00C44A90
// (float), the renderer call (float, long) @ 0x00C0B420.
// Names are descriptive (no PDB layout for this CMouseCursor). GetPressed returns a 16-bit mask (`or ax,word ptr [esi+4]`):
// declared `long` the pushed result let `mov ecx,esi` sink below the push.
// Shape: one 0x50-byte frame record with an inline ctor (the two part ctors plus integer zero stores sharing edi and
// two 1.0f immediates; the stores after the second part ctor sink below the next call's pushes); the renderer call's
// arguments evaluate right to left (GetPressed, then TurnToPointer) and the owner chain is fetched last.
struct CSnapshotPartA {                     // 0x1c bytes
    unsigned char _body[0x1c];
    CSnapshotPartA();                       // 0x00C203A0
};
struct CSnapshotPartB {                     // 0x10 bytes
    long          a;
    long          b;
    long          c;
    unsigned char d;
    CSnapshotPartB();                       // 0x00C20390
};
struct SInputSnapshot {                     // 0x50 bytes
    CSnapshotPartA m_partA;                 // +0x00
    long           m_1c;                    // +0x1c
    long           m_20;                    // +0x20
    float          m_scaleX;                // +0x24
    float          m_scaleY;                // +0x28
    long           m_2c;                    // +0x2c
    CSnapshotPartB m_partB;                 // +0x30
    long           m_40;                    // +0x40
    short          m_44;                    // +0x44
    long           m_48;                    // +0x48
    long           m_4c;                    // +0x4c
    SInputSnapshot()
        : m_1c(0), m_20(0), m_scaleX(1.0f), m_scaleY(1.0f), m_2c(0), m_40(0), m_44(0), m_48(0), m_4c(0)
    {
    }
    unsigned short GetPressed(long index);  // 0x00C27BC0 (returns the 16-bit pressed mask in ax)
};

struct CInputSource {
    SInputSnapshot& GetSnapshot(SInputSnapshot& out, long a, long b);   // 0x00C383D0
};
struct CCursorRenderer {
    void Render(float turn, unsigned short pressed);  // 0x00C0B420
};
struct CChain3 { unsigned char _pad[0x34]; CCursorRenderer* m_pRenderer; };   // renderer at +0x34
struct CChain2 { unsigned char _pad[4]; CChain3* m_p; };                       // +0x04
struct CChain1 { CChain2* m_p; };
struct CChain0 { CChain1* m_p; };
struct COwner  { CChain0* m_p; };

struct CMouseCursor {
    COwner*       m_pOwner;                 // +0x00
    CInputSource* m_pInput;                 // +0x04
    float TurnToPointer();                  // 0x00C44A90
    void Initialize();
};

void CMouseCursor::Initialize()
{
    SInputSnapshot snapshot;
    m_pInput->GetSnapshot(snapshot, 0, 0);
    m_pOwner->m_p->m_p->m_p->m_p->m_pRenderer->Render(TurnToPointer(), snapshot.GetPressed(0));
}