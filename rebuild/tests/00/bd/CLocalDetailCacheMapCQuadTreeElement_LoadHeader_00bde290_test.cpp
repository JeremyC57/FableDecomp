// Behaviour test for CLocalDetailCacheMap::CQuadTreeElement::LoadHeader (0x00BDE290): a 44-byte header in the
// stream's chunk is consumed (pointer/remaining/position advance) and copied field by field; with a short chunk the
// overflow reader is used; an out-of-range position leaves the zeroed sphere and skips the read.
#include <cstdio>
#include <cstring>

class CDataInputStream { public:
    unsigned long StreamPos; long StreamSize; const unsigned char* CurrentSourcePtr; unsigned long SourceChunkStreamPos; long SourceChunkBytesRemaining;
    int overflowCalls;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size) { ++overflowCalls; std::memcpy(buffer, CurrentSourcePtr, size); }
    void Read(void* destination, long bytes)
    {
        long long next = (unsigned long)(StreamPos + bytes);
        if (next >= 0 && next <= 0x7fffffff) {
            if (SourceChunkBytesRemaining >= bytes) {
                std::memcpy(destination, CurrentSourcePtr, bytes);
                CurrentSourcePtr += bytes; SourceChunkBytesRemaining -= bytes; StreamPos += bytes;
            } else
                ReadWithSrcChunkOverflow((unsigned char*)destination, bytes);
        }
    }
};
class CMemoryDataInputStream : public CDataInputStream { public: unsigned char* StreamSource; };
class C3DBoundingSphere { public: float X, Y, Z, Radius; C3DBoundingSphere() : X(0), Y(0), Z(0), Radius(0) { } };
struct CQuadTreeElementFileHeader {
    C3DBoundingSphere BoundingSphere; float MaxFadeDistance; unsigned int PrimitiveTypeMask;
    int FileBlockPos, FileBlockSize, OffsetIntoFileBlock; unsigned short MapX, MapY, Width, Height;
};
struct CQuadTreeElement {
    C3DBoundingSphere BoundingSphere; float MaxFadeDistance; unsigned int PrimitiveTypeMask;
    int FileBlockPos, FileBlockSize, OffsetIntoFileBlock; unsigned short MapX, MapY, Width, Height;
    void LoadHeader(CMemoryDataInputStream& stream);
};
void CQuadTreeElement::LoadHeader(CMemoryDataInputStream& stream)
{
    CQuadTreeElementFileHeader header;
    stream.Read(&header, sizeof(header));
    MapX = header.MapX; MapY = header.MapY; Width = header.Width; Height = header.Height;
    FileBlockPos = header.FileBlockPos; FileBlockSize = header.FileBlockSize; OffsetIntoFileBlock = header.OffsetIntoFileBlock;
    MaxFadeDistance = header.MaxFadeDistance; PrimitiveTypeMask = header.PrimitiveTypeMask;
    BoundingSphere = header.BoundingSphere;
}
int main()
{
    int bad = 0;
    CQuadTreeElementFileHeader h;
    h.BoundingSphere.X = 1; h.BoundingSphere.Y = 2; h.BoundingSphere.Z = 3; h.BoundingSphere.Radius = 4;
    h.MaxFadeDistance = 50; h.PrimitiveTypeMask = 0x55; h.FileBlockPos = 100; h.FileBlockSize = 200; h.OffsetIntoFileBlock = 8;
    h.MapX = 10; h.MapY = 11; h.Width = 12; h.Height = 13;
    if (sizeof(h) != 44) bad = 1;
    unsigned char buf[64]; std::memcpy(buf, &h, sizeof(h));
    CMemoryDataInputStream s; s.StreamPos = 16; s.CurrentSourcePtr = buf; s.SourceChunkBytesRemaining = 60; s.overflowCalls = 0;
    CQuadTreeElement e; std::memset(&e, 0, sizeof(e));
    e.LoadHeader(s);
    if (e.MapX != 10 || e.Height != 13 || e.FileBlockPos != 100 || e.OffsetIntoFileBlock != 8 || e.MaxFadeDistance != 50.0f ||
        e.PrimitiveTypeMask != 0x55 || e.BoundingSphere.Radius != 4.0f || e.BoundingSphere.X != 1.0f) bad = 1;
    if (s.StreamPos != 60 || s.SourceChunkBytesRemaining != 16 || s.CurrentSourcePtr != buf + 44 || s.overflowCalls) bad = 1;
    // short chunk: the overflow reader fills the header
    s.CurrentSourcePtr = buf; s.SourceChunkBytesRemaining = 10; s.StreamPos = 0; std::memset(&e, 0, sizeof(e));
    e.LoadHeader(s);
    if (s.overflowCalls != 1 || e.Width != 12) bad = 1;
    // out of range: nothing read, sphere stays zero
    s.StreamPos = 0x7ffffff0UL; s.overflowCalls = 0; e.BoundingSphere.X = 9;
    e.LoadHeader(s);
    if (s.overflowCalls != 0 || e.BoundingSphere.X != 0.0f) bad = 1;
    if (bad) { std::printf("00bde290_TEST FAIL\n"); return 1; }
    std::printf("00bde290_TEST PASS\n");
    return 0;
}