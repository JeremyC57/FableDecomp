#include <cstdio>
#include <cstdlib>
#include <cstring>
// C3DMeshFileXAnimatedPrimitiveBlockChunk::~...: bone weights freed, vertex list freed, group block destroyed, then the super-chunk base
static char g_log[32];
static void Log(const char* s) { std::strcat(g_log, s); }
template <class T> class CArray { public:
    T* m_pFirst; T* m_pLast; T* m_pEnd; char tag;
    CArray() : m_pFirst(0), m_pLast(0), m_pEnd(0), tag('?') { }
    ~CArray() { if (m_pFirst) { std::free(m_pFirst); char t[2] = { tag, 0 }; Log(t); } }
};
class CChunkedFileSuperChunk { public: virtual void v00() { } ~CChunkedFileSuperChunk() { Log("B"); } };
class CVertexGroupBlock { public: ~CVertexGroupBlock() { Log("G"); } };
class C3DIndexedBoneWeight { public: long bone; float weight; };
class C3DMeshFileXAnimatedPrimitiveBlockChunk : public CChunkedFileSuperChunk { public:
    CVertexGroupBlock GroupBlock; unsigned long BonesPerVertex;
    CArray<unsigned long> VertexList; CArray<C3DIndexedBoneWeight> BoneWeights;
    virtual void v00() { }
    ~C3DMeshFileXAnimatedPrimitiveBlockChunk() { }
};
int main()
{
    {
        C3DMeshFileXAnimatedPrimitiveBlockChunk c;
        c.VertexList.tag = 'v'; c.VertexList.m_pFirst = (unsigned long*)std::malloc(16);
        c.BoneWeights.tag = 'w'; c.BoneWeights.m_pFirst = (C3DIndexedBoneWeight*)std::malloc(16);
    }
    if (std::strcmp(g_log, "wvGB") != 0) { std::printf("00aa7480_TEST FAIL\n"); return 1; }
    std::printf("00aa7480_TEST PASS\n");
    return 0;
}