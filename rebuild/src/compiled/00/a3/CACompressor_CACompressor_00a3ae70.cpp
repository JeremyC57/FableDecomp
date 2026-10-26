// CACompressor::CACompressor(void* workspace, unsigned long workspaceSize) -- base CBase(), own vtable, empty own-workspace vector, workspace pointer/size
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A3AE70, length 45 bytes.
// Callees: CBase::CBase @ 0x0099A2D0 (stores the CBase vtable only). This class's vtable is
// 0x0129B424 (rtti-vtables.tsv: CACompressor).
// Layout from engine/CACompressor.h: Workspace +0x04, WorkspaceSize +0x08, CompressionOn +0x0c
// (left uninitialised), OwnWorkspace vector at +0x10. The retail vector is the VC6-era three-pointer
// form (first/last/end at +0x10/+0x14/+0x18, no allocator slot) -- the PDB's 0x10-byte vector
// type disagrees by one dword, so the vector is modelled locally. The vtable store precedes the
// vector's register-zeroed stores (an embedded member with an inline constructor), and the two
// arguments are assigned in the body (init-list members would sink the vtable store below them).
namespace N_0099a2d0 { struct CBase {
    virtual void v00();
    CBase();                                // 0x0099A2D0
}; } using N_0099a2d0::CBase;

struct CRetailByteVector {                  // VC6-era vector<unsigned char>: three raw pointers
    unsigned char* first;
    unsigned char* last;
    unsigned char* end;
    CRetailByteVector() : first(0), last(0), end(0) { }
};

class CACompressor : public CBase { public:               // vtable 0x0129B424
    void*             Workspace;            // +0x04
    unsigned long     WorkspaceSize;        // +0x08
    bool              CompressionOn;        // +0x0c (not initialised here)
    CRetailByteVector OwnWorkspace;         // +0x10
    virtual void v00();
    CACompressor(void* workspace, unsigned long workspaceSize);
};

CACompressor::CACompressor(void* workspace, unsigned long workspaceSize)
    : CBase()
    , OwnWorkspace()
{
    Workspace = workspace;
    WorkspaceSize = workspaceSize;
}