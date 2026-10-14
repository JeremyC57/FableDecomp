// C3DMeshFileGroupChunk::AddVertex(const C3DVertexBlend2&) -- merge the weight into the last entry when it has the same bone, else push_back
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ABCC20, length 96 bytes.
// Callees: 0x00ABEB40 CArray<C3DVertexBlend2>::Insert(where, value, tag, count, flag)
// push_back is inlined: `new (m_end) T(v)` carries the compiler's placement-new null test, the grow path
// passes an empty tag temporary homed in the dead argument slot (lea esi,[esp+10h]).
//
#include "rebuild_abi.h"
struct C3DVertexBlend2 { long BoneIndex; float Weight; };
void* operator new(unsigned int, void* p) { return p; }
struct SInsertTag { SInsertTag() {} };
template<class T> struct CArray {
    T* m_begin; T* m_end; T* m_capacity;
    void Insert(T* where, const T& value, const SInsertTag& tag, long count, long flag);   // 0x00ABEB40
    bool Empty() const { return m_begin == m_end; }
    T& Last() { return m_end[-1]; }
    void Add(const T& value)
    {
        if (m_end != m_capacity) {
            new (m_end) T(value);
            ++m_end;
        } else {
            Insert(m_end, value, SInsertTag(), 1, 1);
        }
    }
};
struct C3DMeshFileGroupChunk {
    unsigned char _base[0x30];     // C3DMeshFileChunk (0x2c) + BoneIndex (+0x2c)
    CArray<C3DVertexBlend2> VertexList;   // +0x30
    void AddVertex(const C3DVertexBlend2& vertex);
};
void C3DMeshFileGroupChunk::AddVertex(const C3DVertexBlend2& vertex)
{
    if (!VertexList.Empty() && VertexList.Last().BoneIndex == vertex.BoneIndex) {
        VertexList.Last().Weight += vertex.Weight;
    } else {
        VertexList.Add(vertex);
    }
}