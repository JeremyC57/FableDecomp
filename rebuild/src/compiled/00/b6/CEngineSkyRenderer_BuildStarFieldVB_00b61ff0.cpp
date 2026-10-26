// CEngineSkyRenderer::BuildStarFieldVB() -- loads the star-field vertex buffer from its cache file (star count, then StarCount 24-byte vertices)
// Leaf: BuildStarFieldVB
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

class CWideString { public:
    CWideStringData* PStringData;
    ~CWideString();                                  // 0x0099B510
};

bool __fastcall PathExists(const CWideString& path); // 0x00999230

class CASuspendableProcess;
enum FileAccessType { FILE_ACCESS_READ = 1 };

struct CBaseClassNonCopyable {
    virtual ~CBaseClassNonCopyable();                // 0x0099A300
};

class CDiskFileWin32 : public CBaseClassNonCopyable { public:      // vtable 0x0122D06C
    unsigned char _pad_0x04[0x14];
    CWideString   Pathname;                          // +0x18
    CDiskFileWin32(const CWideString& pathname, FileAccessType access, unsigned long flags);   // 0x0099AD80
    virtual void Open();                             // slot 1
    virtual void Close();                            // slot 2, 0x0099A920
    virtual void Read(void* buffer, long size, CASuspendableProcess* process);   // slot 3, 0x0099A960
    ~CDiskFileWin32() { Close(); }
};

enum EVertexType { VERTEX_TYPE_STAR_FIELD = 0x42 };
enum EVertexBufferPool { VERTEX_BUFFER_POOL_MANAGED = 1 };

struct CStarFieldVertexBufferCalls {                 // CVertexBufferWin32 (0x34 bytes)
    bool Initialise(EVertexType type, long count, long stride, unsigned long usage, EVertexBufferPool pool);   // 0x00A63150
    void* Lock(unsigned long offset, unsigned long size, unsigned long flags);   // 0x00AC20A0
    void Unlock();                                   // 0x00AC20B0
};

struct CSkyRendererMethods : CEngineSkyRenderer {
    CStarFieldVertexBufferCalls& StarField() { return *reinterpret_cast<CStarFieldVertexBufferCalls*>(StarFieldVertexBuffer); }
    const CWideString& StarVBFilePath() const { return *reinterpret_cast<const CWideString*>(&StarVBFilePath_PStringData); }
    void BuildStarFieldVB();
};

void CSkyRendererMethods::BuildStarFieldVB()
{
    if (PathExists(StarVBFilePath())) {
        CDiskFileWin32 file(StarVBFilePath(), FILE_ACCESS_READ, 2);
        file.Read(&StarCount, 4, 0);
        StarField().Initialise(VERTEX_TYPE_STAR_FIELD, StarCount, 0x18, 0, VERTEX_BUFFER_POOL_MANAGED);
        long bytes = StarCount * 0x18;
        void* vertices = StarField().Lock(0, 0, 0);
        file.Read(vertices, bytes, 0);
        StarField().Unlock();
    }
}