// CACompressor::CACompressor(long) -- construct a compressor that owns a zero-filled workspace of the given size
// Leaf: ??0CACompressor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A3AEA0, length 68 bytes.
// Callees: CBaseClass::CBaseClass @ 0x0099A2D0 (vtable 0x01231710), std::vector<unsigned char>::resize(n,
// const value&) @ 0x00486189. Vtable 0x0129B424 = CACompressor (RTTI); Ego_r.pdb ??0CACompressor@@QAE@J@Z.
// Identity: the manifest label CPreallocTexturePool::CPreallocTexturePool is propagated.
// Layout (Ego_r.pdb): Workspace +4, WorkspaceSize +8, CompressionOn +0xc (not touched), OwnWorkspace +0x10.
// The engine header CACompressor.h flattens the vptr into a plain member and the vector into opaque PDB bytes,
// so the constructor (base call, vtable, member constructor) is modelled locally on the same offsets.
// Shape: the vector's inline constructor zeroes its three words through edi; the `0` fill value is a byte
// temporary in the `push ecx` frame slot (`mov [esp+0Fh],al`); Workspace = the vector's first pointer and
// WorkspaceSize stored after the resize.
struct CBaseClass {                                         // vtable 0x01231710
    CBaseClass();                                           // 0x0099A2D0
    virtual void v00();
};

struct CByteVector {                                        // retail std::vector<unsigned char> (12 bytes)
    unsigned char* m_pFirst;
    unsigned char* m_pLast;
    unsigned char* m_pEnd;
    CByteVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
    void resize(unsigned int size, const unsigned char& value);   // 0x00486189
};

struct CACompressor : CBaseClass {                          // vtable 0x0129B424
    void*         Workspace;                                // +0x04
    unsigned long WorkspaceSize;                            // +0x08
    bool          CompressionOn;                            // +0x0c
    CByteVector   OwnWorkspace;                             // +0x10
    CACompressor(long workspaceSize);
    virtual void v00();
};

CACompressor::CACompressor(long workspaceSize)
{
    OwnWorkspace.resize(workspaceSize, 0);
    Workspace = OwnWorkspace.m_pFirst;
    WorkspaceSize = workspaceSize;
}
