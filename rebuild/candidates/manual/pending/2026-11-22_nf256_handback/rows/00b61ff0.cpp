// CEngineSkyRenderer::BuildStarFieldVB() -- loads the star-field vertex buffer from its cache file (star count, then StarCount 24-byte vertices)
// Leaf: ?BuildStarFieldVB@CEngineSkyRenderer@@AAEXXZ
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
// Layout: engine/CEngineSkyRenderer.h retail branch (Ego_r offsets: StarFieldVertexBuffer +0x24 CVertexBufferWin32, StarCount +0x150,
//   StarVBFilePath +0x158 CWideString); CDiskFileWin32 / CAFile / CWideString from engine/CDiskFileWin32.h, engine/CAFile.h, fable_string.h.
// NF256 (2026-11-22, nf254 L120 follow-on): self off the CSkyRendererMethods stand-in (?BuildStarFieldVB@CSkyRendererMethods@@QAEXXZ) onto
//   the header's private ?BuildStarFieldVB@CEngineSkyRenderer@@AAEXXZ (Ego_r.pdb + FableWin.pdb; the spelling caller 0x00B620A0 binds),
//   FABLE_RETAIL_CENGINESKYRENDERER_BUILDBUFFERS_CALLEES; the row-local CWideString / CAFile / CDiskFileWin32 models give way to the shared
//   headers (the 0x009D0380 CBankFile::ReadRecoveryFile define set).
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_CDISKFILEWIN32
#define FABLE_RETAIL_CVERTEXBUFFERWIN32
#define FABLE_RETAIL_CENGINESKYRENDERER
#define FABLE_RETAIL_CENGINESKYRENDERER_BUILDBUFFERS_CALLEES
#include "fable_string.h"
#include "engine/CAFile.h"
#include "engine/CDiskFileWin32.h"
#include "engine/EVertexType.h"
#include "engine/EVertexBufferPool.h"
#include "engine/CVertexBufferWin32.h"
#include "engine/CEngineSkyRenderer.h"

void CEngineSkyRenderer::BuildStarFieldVB()
{
    if (CAFile::PathExists(StarVBFilePath)) {
        CDiskFileWin32 file(StarVBFilePath, FILE_ACCESS_READ, 2);
        file.Read(&StarCount, 4, 0);
        StarFieldVertexBuffer.Initialise(VERTEX_COL, StarCount, 0x18, 0, VB_POOL_MANAGED);
        long bytes = StarCount * 0x18;
        void* vertices = StarFieldVertexBuffer.Lock(0, 0, 0);
        file.Read(vertices, bytes, 0);
        StarFieldVertexBuffer.Unlock();
    }
}
