// CLayer::SetAllPolys() -- reset all 256 polygons of the layer (0x84 bytes each): the trailing count (+0x80) cleared, then the 0x80-byte vertex block zeroed; returns `this`
// Leaf: SetAllPolys
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0050EE40, length 52 bytes.
// Shape: `this` in ebp, the polygon pointer in edx, a count-down in esi, the block zeroed with `rep stosd` (the intrinsic memset of 0x80 bytes), `ret`.
extern "C" void* __cdecl memset(void* block, int value, unsigned int size);
#pragma intrinsic(memset)

struct SLayerPoly_0050ee40 {                       // 0x84 bytes
    unsigned char m_vertices[0x80];                // +0x00
    long          m_count;                         // +0x80
};

struct CLayer {
    SLayerPoly_0050ee40 m_polys[256];              // +0x00
    CLayer* SetAllPolys();
};

CLayer* CLayer::SetAllPolys()
{
    SLayerPoly_0050ee40* poly = m_polys;
    for (long n = 256; n; --n, ++poly) {
        poly->m_count = 0;
        memset(poly->m_vertices, 0, sizeof(poly->m_vertices));
    }
    return this;
}