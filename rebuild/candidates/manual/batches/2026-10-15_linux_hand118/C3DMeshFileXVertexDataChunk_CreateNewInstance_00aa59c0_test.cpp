// Behaviour test for 0x00AA59C0 C3DMeshFileXVertexDataChunk::CreateNewInstance
#include <cstdio>
#include <cstring>
struct CChunkedFileChunk {
    char ChunkType[5];
    CChunkedFileChunk(const char* t) { std::strncpy(ChunkType, t, 5); }
    virtual ~CChunkedFileChunk() {}
};
template<class T> struct CArray { T* m_begin; T* m_end; T* m_capacity; CArray() : m_begin(0), m_end(0), m_capacity(0) {} };
struct CVertexCompressionParams {
    float PosScaleX, PosScaleY, PosScaleZ, PosScaleW, PosBiasX, PosBiasY, PosBiasZ, PosBiasW;
    CVertexCompressionParams() : PosScaleX(1.0f), PosScaleY(1.0f), PosScaleZ(1.0f), PosScaleW(1.0f),
        PosBiasX(0.0f), PosBiasY(0.0f), PosBiasZ(0.0f), PosBiasW(0.0f) {}
};
struct C3DMeshFileXVertexDataChunk : CChunkedFileChunk {
    unsigned long FormatID, VertexCount, VertexSize, InitFlags;
    CVertexCompressionParams VBCParams;
    CArray<char> VertexData;
    C3DMeshFileXVertexDataChunk() : CChunkedFileChunk("XVBD") {}
};
struct CFactory {
    CChunkedFileChunk* registered;
    CFactory() : registered(0) {}
    virtual CChunkedFileChunk* RegisterNewChunk(CChunkedFileChunk* c) { registered = c; return c; }
    CChunkedFileChunk* CreateNewInstance() { return RegisterNewChunk(new C3DMeshFileXVertexDataChunk); }
};
int main()
{
    CFactory f;
    C3DMeshFileXVertexDataChunk* c = (C3DMeshFileXVertexDataChunk*)f.CreateNewInstance();
    const CVertexCompressionParams& p = c->VBCParams;
    bool ok = c == f.registered && std::strncmp(c->ChunkType, "XVBD", 4) == 0
        && p.PosScaleX == 1.0f && p.PosScaleY == 1.0f && p.PosScaleZ == 1.0f && p.PosScaleW == 1.0f
        && p.PosBiasX == 0.0f && p.PosBiasY == 0.0f && p.PosBiasZ == 0.0f && p.PosBiasW == 0.0f
        && c->VertexData.m_begin == 0 && c->VertexData.m_end == 0 && c->VertexData.m_capacity == 0;
    delete c;
    if (!ok) { std::printf("00aa59c0_TEST FAIL\n"); return 1; }
    std::printf("00aa59c0_TEST PASS\n");
    return 0;
}
