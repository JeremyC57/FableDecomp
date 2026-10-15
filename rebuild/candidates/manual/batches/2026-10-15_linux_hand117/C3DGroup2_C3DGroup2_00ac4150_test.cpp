#include <cstdio>
#include <cstdlib>
#include <new>
struct C3DVertexBlend2 { unsigned long VertexIndex; float BlendValue; };
struct SScalarTag { SScalarTag() { } };
template <class T>
struct CArray {
    T* m_pBegin; T* m_pEnd; T* m_pEndOfStorage;
    CArray() : m_pBegin(0), m_pEnd(0), m_pEndOfStorage(0) {}
    ~CArray() { std::free(m_pBegin); }
    void InsertGrow(T* where, const T& value, const SScalarTag&, unsigned long count, bool)
    {
        unsigned long n = (unsigned long)(m_pEnd - m_pBegin), cap = n ? n * 2 : 2;
        T* p = (T*)std::malloc(cap * sizeof(T));
        for (unsigned long i = 0; i < n; ++i) p[i] = m_pBegin[i];
        p[n] = value;
        std::free(m_pBegin);
        m_pBegin = p; m_pEnd = p + n + count; m_pEndOfStorage = p + cap;
    }
    void push_back(const T& value)
    {
        if (m_pEnd != m_pEndOfStorage) {
            new (m_pEnd) T(value);
            ++m_pEnd;
        }
        else
            InsertGrow(m_pEnd, value, SScalarTag(), 1, true);
    }
};
struct C3DMeshFileGroupChunk {
    long m_bone; C3DVertexBlend2 m_list[5];
    long GetBoneIndex() const { return m_bone; }
    unsigned long GetVertexCount() const { return 5; }
    const C3DVertexBlend2* GetList() const { return m_list; }
};
struct C3DGroup2 {
    long BoneIndex;
    CArray<C3DVertexBlend2> VertexBlends;
    C3DGroup2(const C3DMeshFileGroupChunk& chunk)
    {
        BoneIndex = chunk.GetBoneIndex();
        unsigned long count = chunk.GetVertexCount();
        const C3DVertexBlend2* blends = chunk.GetList();
        for (unsigned long i = 0; i < count; ++i)
            VertexBlends.push_back(blends[i]);
    }
};
int main()
{
    C3DMeshFileGroupChunk chunk; chunk.m_bone = 4;
    for (int i = 0; i < 5; ++i) { chunk.m_list[i].VertexIndex = 10 + i; chunk.m_list[i].BlendValue = 0.5f * i; }
    C3DGroup2 g(chunk);
    int ok = g.BoneIndex == 4 && g.VertexBlends.m_pEnd - g.VertexBlends.m_pBegin == 5;
    for (int i = 0; ok && i < 5; ++i)
        ok = g.VertexBlends.m_pBegin[i].VertexIndex == (unsigned long)(10 + i) && g.VertexBlends.m_pBegin[i].BlendValue == 0.5f * i;
    if (!ok) { std::printf("00ac4150_TEST FAIL\n"); return 1; }
    std::printf("00ac4150_TEST PASS\n");
    return 0;
}
