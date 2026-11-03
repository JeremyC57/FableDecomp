#ifndef FABLE_ENGINE_C3DMESHFILEMAPCHUNK_H
#define FABLE_ENGINE_C3DMESHFILEMAPCHUNK_H
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
enum EMaterialMapType;
class C3DMeshFileMapChunk:public CChunkedFileSuperChunk {
public:
 C3DMeshFileMapChunk();
public: virtual const CCharString GetChunkDescription() const;
public: virtual const CCharString GetChunkInfo() const;
public: virtual void ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&);
public: virtual void WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const;
public: virtual CChunkedFileChunk* CreateNewInstance() const;
private: EMaterialMapType MapType;
private: CCharString MapDescription;
private: CCharString Filename;
private: unsigned long CRC;
};

#endif
