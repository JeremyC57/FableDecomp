// CMatrix4x4::$E_Identity() -- dynamic initializer of CMatrix4x4::Identity: the inline SetIdentity loop (each column j of the four rows gets 1.0f on the diagonal, 0.0f elsewhere)
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01224340, 112 bytes, in `.text$yc`, reached from the .CRT$XCU table, plain `ret`.
// Identity (proven): Ego_r.pdb / FableWin.pdb `dynamic initializer for 'CMatrix4x4::Identity'`, in the
//   BlockedNode / Identity / DefaultUncompressedPFormat group (see 0x012243B0); the 64-byte object 0x013D2890 is read by
//   CParticleSimulation::ApplyDragging 0x00ADF300, and the code writes exactly the identity (constants 1.0f @ 0x0122DED8,
//   0.0f @ 0x0122DEDC). No atexit registration: CMatrix4x4 has a trivial destructor.
// Label: the manifest's "sub_01224340" is a gap-scan placeholder.
// Layout: Ego_r.pdb CMatrix4x4 is 64 bytes (a union over the 4x4 floats); rows are 16 bytes apart.
// Shape: VC7.1 unrolls the inner row loop (four `(i == j) ? 1 : 0` selects through x87 loads, each `fstp` indexed by the
//   column) and keeps the column loop; the EMatrix34Init constructor argument is a descriptive enumerator. SetIdentity and
//   the constructor must both be `__forceinline`: VC7.1 does not inline a plain inline function holding a loop.
enum EMatrix34Init { MATRIX34_INIT_IDENTITY };

class CMatrix4x4 {
public:
    float M[4][4];                                        // +0x00
    __forceinline CMatrix4x4(EMatrix34Init) { SetIdentity(); }
    __forceinline void SetIdentity()
    {
        for (long j = 0; j < 4; ++j)
            for (long i = 0; i < 4; ++i)
                M[i][j] = (i == j) ? 1.0f : 0.0f;
    }
    static CMatrix4x4 Identity;                           // 0x013D2890
};

CMatrix4x4 CMatrix4x4::Identity(MATRIX34_INIT_IDENTITY);