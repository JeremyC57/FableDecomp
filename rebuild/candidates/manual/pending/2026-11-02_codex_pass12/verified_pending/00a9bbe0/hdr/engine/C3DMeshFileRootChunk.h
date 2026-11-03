#ifndef FABLE_ENGINE_C3DMESHFILEROOTCHUNK_H
#define FABLE_ENGINE_C3DMESHFILEROOTCHUNK_H
// Retail opt-in (Codex pass 12): original PDB types and native chunk ctor/dtor offsets.
#include "engine/CChunkedFileSuperChunk.h"
#include "engine/CArray.h"
#include "engine/CMatrix3x4.h"
#include "engine/C3DVector.h"
#include "engine/CPlane.h"
#include "engine/C3DVertex2.h"
#include "engine/C3DVertexBlend2.h"
#include "engine/C3DTriangle2.h"
#include "engine/CRGBColour.h"
#include "engine/CIVCountedPointer.h"
class C3DMeshFileRootChunk:public CChunkedFileSuperChunk {
public:
 C3DMeshFileRootChunk():CChunkedFileSuperChunk("3DRT"){}
public: virtual const CCharString GetChunkDescription() const;
public: virtual CChunkedFileChunk* CreateNewInstance() const;
};

#endif
