// CObjectCacheGroupCollection::LoadHeader -- read the 40-byte cache-group file header.
// Retail00bde190 length242. Native class and CFileHeader PDB fields agree;
// no generated target header exists. Actual input-stream header is used.
// ReadWithSrcChunkOverflow00993ca0 audited as fallback, not a virtual call.
#include <string.h>
#define CLocalDetailCacheMap CLocalDetailCacheMapLayout
#include "engine/CLocalDetailCacheMap.h"
#undef CLocalDetailCacheMap
#define CDataInputStream CDataInputStreamLayout
#include "engine/CDataInputStream.h"
#undef CDataInputStream
#define CMemoryDataInputStream CMemoryDataInputStreamLayout
#include "engine/CMemoryDataInputStream.h"
#undef CMemoryDataInputStream
#ifdef _MSC_VER
#define INLINE __forceinline
typedef long S32; typedef unsigned long U32; typedef __int64 S64;
#else
#define INLINE inline
typedef int S32; typedef unsigned int U32; typedef long long S64;
#endif
class CDataInputStream : public CDataInputStreamLayout { public:
    void ReadWithSrcChunkOverflow(unsigned char*,long); // 0x00993ca0
    INLINE void Read(void* destination,long bytes) {
        S64 next=(U32)(StreamPos+bytes);
        if(next>=0 && next<=0x7fffffff) {
            if(SourceChunkBytesRemaining>=bytes) {
                memcpy(destination,CurrentSourcePtr,bytes);
                CurrentSourcePtr=(unsigned char*)CurrentSourcePtr+bytes;
                SourceChunkBytesRemaining-=bytes;
                StreamPos+=bytes;
            } else ReadWithSrcChunkOverflow((unsigned char*)destination,bytes);
        }
    }
};
// Native memory stream derives from the six-field CDataInputStream base;
// donor flattened chunkBase is incorrectly long, native +0c is a pointer.
class CMemoryDataInputStream : public CDataInputStream { public:unsigned char* StreamSource;};
struct BoundingSphere { float X,Y,Z,Radius; BoundingSphere():X(0),Y(0),Z(0),Radius(0) {} };
struct CacheGroupFileHeader {
    S32 FileBlockPos,FileBlockSize,OffsetIntoFileBlock;
    BoundingSphere Sphere; float MaxFadeDistance; U32 PrimitiveTypeMask;
    unsigned short CacheGroupIndex; bool RenderUpdateNeeded;
};
class CLocalDetailCacheMap { public:class CObjectCacheGroupCollection { public:
    void* VTable; CObjectCacheGroupCollection* Next; BoundingSphere Sphere;
    float MaxFadeDistance; U32 PrimitiveTypeMask; void* ObjectTypeCollections; void* Map;
    S32 FileBlockPos,FileBlockSize,OffsetIntoFileBlock;
    unsigned short CacheGroupIndex; unsigned char RenderUpdateNeeded:1,Loading:1;
    S32 SourceObjectCount;
    void LoadHeader(CMemoryDataInputStream&);
};};
typedef CLocalDetailCacheMap::CObjectCacheGroupCollection CObjectCacheGroupCollection;
void CLocalDetailCacheMap::CObjectCacheGroupCollection::LoadHeader(CMemoryDataInputStream& stream) {
    CacheGroupFileHeader h;
    stream.Read(&h,sizeof h);
    FileBlockPos=h.FileBlockPos;
    FileBlockSize=h.FileBlockSize;
    OffsetIntoFileBlock=h.OffsetIntoFileBlock;
    CacheGroupIndex=h.CacheGroupIndex;
    Sphere=h.Sphere;
    MaxFadeDistance=h.MaxFadeDistance;
    PrimitiveTypeMask=h.PrimitiveTypeMask;
    RenderUpdateNeeded=h.RenderUpdateNeeded;
}