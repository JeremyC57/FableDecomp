// CCountedPointer<CShaderResource>::operator=(const CCountedPointer&) -- share the other pointer: ShareData(Data, Info), return *this
// Leaf: ??4?$CCountedPointer@VCShaderResource@@@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009DE7C0, 72 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// Was landed as _global::CCountedPointer_Set_009de7c0 (stand-in name).
// T = CShaderResource, proven by: CRenderManager2D::CRenderManager2D 0x009D9A30 assigns VertexShaderResource through it (Ego_r CRenderManager2D: CCountedPointer<CShaderResource> VertexShaderResource; the landed ctor models a CShaderResource pointer); FableWin.pdb ??4?$CCountedPointer@VCShaderResource@@ name.
// Member: the in-class operator= of engine/CCountedPointer.h (loads Data / Info, then the ShareData body), emitted as its own COMDAT.
// Model: opt-in FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (ReleaseData zeroes Data first; its stores die under ShareData's own).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
class CShaderResource;
template CCountedPointer<CShaderResource>& CCountedPointer<CShaderResource>::operator=(const CCountedPointer<CShaderResource>&);
