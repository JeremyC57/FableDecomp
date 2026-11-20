// CEngineSkyRenderer::BuildStarFieldVB() -- loads the star-field vertex buffer from its cache file (star count, then StarCount 24-byte vertices)
// Leaf: BuildStarFieldVB
// Naming (naming wave 24): ~CBaseClassNonCopyable 0x0099A300 is protected virtual (Ego_r.pdb / FableWin.pdb ??1CBaseClassNonCopyable@@MAE@XZ, engine/CBaseClassNonCopyable.h).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B61FF0, 165 bytes. Label correction: the catalog calls this row
// "CTCHeroRangedAttacks::ContinueAimingRangedWeapon" (propagated; that member is 0x00789FD0) and the landed
// CEngineSkyRenderer::BuildBuffers (0x00B620A0, its only caller) names it "BuildSolidBaseBand"; the body reads
// StarVBFilePath (+0x158), StarCount (+0x150) and StarFieldVertexBuffer (+0x24), and Ego_r.pdb lists
// ?BuildStarFieldVB@CEngineSkyRenderer@@AAEXXZ.
// Callees: PathExists(const CWideString&) @ 0x00999230 (__fastcall), CDiskFileWin32::CDiskFileWin32(const CWideString&,
//          FileAccessType, ulong) @ 0x0099AD80, CDiskFileWin32::Read(void*, long, CASuspendableProcess*) @ 0x0099A960
//          (x2, devirtualised on the local file), CVertexBufferWin32::Initialise(EVertexType, long, long, ulong,
//          EVertexBufferPool) @ 0x00A63150, ::Lock(ulong, ulong, ulong) @ 0x00AC20A0, ::Unlock() @ 0x00AC20B0,
//          CDiskFileWin32::Close() @ 0x0099A920, ~CWideString @ 0x0099B510, ~CBaseClassNonCopyable @ 0x0099A300;
//          CDiskFileWin32 vtable 0x0122D06C.
// Shape: the file is a 0x1C-byte local whose inline destructor stores its vtable, closes the file and destroys the
// path string and the non-copyable root; the vertex byte count (`lea ; shl 3`) is a named local computed before the locked
// pointer (also a named local) and kept in esi across the Lock call.
// Layout: engine/CEngineSkyRenderer.h (PDB offsets match retail); CDiskFileWin32 is modelled locally (the engine
// header is a flat POD and the local needs its polymorphic destructor chain).
#include "engine/CEngineSkyRenderer.h"
// NF149 (q4552 / q4559 / q4560): the star-field buffer (flat storage in the sky header) is reached as the shared retail
// CVertexBufferWin32 (engine/CVertexBufferWin32.h; Ego_r.pdb + FableWin.pdb Initialise 0x00A63150, CVertexBufferBase::Lock / Unlock
// 0x00AC20A0 / 0x00AC20B0) with the shared buffer enums (VERTEX_COL = 0x42); was the stand-in CStarFieldVertexBufferCalls. The local
// CBaseClassNonCopyable model gives way to engine/CBaseClassNonCopyable.h, which the buffer header includes.
#define FABLE_RETAIL_CVERTEXBUFFERWIN32
#include "engine/EVertexType.h"
#include "engine/EVertexBufferPool.h"
#include "engine/CVertexBufferWin32.h"

class CWideString { public:
    CWideStringData* PStringData;
    ~CWideString();                                  // 0x0099B510
};

class CAFile {
public:
    static bool __fastcall PathExists(const CWideString&);   // 0x00999230
}; // 0x00999230

class CASuspendableProcess;
enum FileAccessType { FILE_ACCESS_READ = 1 };

class CDiskFileWin32 : public CBaseClassNonCopyable { public:      // vtable 0x0122D06C
    unsigned char _pad_0x04[0x14];
    CWideString   Pathname;                          // +0x18
    CDiskFileWin32(const CWideString& pathname, FileAccessType access, unsigned long flags);   // 0x0099AD80
    virtual void Open();                             // slot 1
    virtual void Close();                            // slot 2, 0x0099A920
    virtual void Read(void* buffer, long size, CASuspendableProcess* process);   // slot 3, 0x0099A960
    ~CDiskFileWin32() { Close(); }
};

struct CSkyRendererMethods : CEngineSkyRenderer {
    CVertexBufferWin32& StarField() { return *reinterpret_cast<CVertexBufferWin32*>(StarFieldVertexBuffer); }
    const CWideString& StarVBFilePath() const { return *reinterpret_cast<const CWideString*>(&StarVBFilePath_PStringData); }
    void BuildStarFieldVB();
};

void CSkyRendererMethods::BuildStarFieldVB()
{
    if (CAFile::PathExists(StarVBFilePath())) {
        CDiskFileWin32 file(StarVBFilePath(), FILE_ACCESS_READ, 2);
        file.Read(&StarCount, 4, 0);
        StarField().Initialise(VERTEX_COL, StarCount, 0x18, 0, VB_POOL_MANAGED);
        long bytes = StarCount * 0x18;
        void* vertices = StarField().Lock(0, 0, 0);
        file.Read(vertices, bytes, 0);
        StarField().Unlock();
    }
}