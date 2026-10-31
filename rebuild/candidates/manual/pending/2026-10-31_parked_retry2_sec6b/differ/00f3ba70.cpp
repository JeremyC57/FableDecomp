// CTriangleStripifier2::BuildCacheOptimizedVertexList(CArray<unsigned short>& unused, CArray<CArray<long> >& vertexFaces, CArray<CArray<unsigned short> >& faceVertices, CArray<unsigned short>& stripOfFace, CArray<CArray<unsigned short> >& strips, CArray<unsigned short>& vertexList) -- emit each strip's vertices in first-use order into vertexList, looking ahead into the next strip so a vertex is emitted while it would still be in a 16-entry cache
// Grade: DIFFER(816v853) 208 diff lines (parked 684v853, 281 lines): vertexList.clear() is STLport erase(begin(), end()) through the copy helper (retail's unfolded `cmp eax,eax`), the argument roles are re-decoded from retail (arg4 is the per-face strip table compared with s, arg1 is never read), s is signed (`test edi,edi ; jle`), `now` is a strip-level counter advanced per emitted vertex, the face scan is retail's at>=0 / f==vertex split with one shared `jge` to next_strip, growth uses (max)(count, one) by const&, and push_back is __forceinline; left: retail re-reads next.size() and faces.size() every iteration (ours hoists them into frame/registers), and register/slot assignment around the strip loop
// Leaf: BuildCacheOptimizedVertexList
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F3BA70, length 853 bytes (`ret 0x18`).
// Callees: the CArray<bool>(n, value) / CArray<long>(n, value) constructors 0x00451583 / 0x00512630, memmove 0x00BFEAE6,
//   malloc 0x00BFEA0E, free 0x00BFEA14 (CArray storage). First-pass decode: argument roles are provisional.
// Layout: CArray<T> = {T* begin, T* end, T* capacity} (12 bytes, malloc/free storage).
#include <string.h>
extern "C" void* __cdecl malloc(unsigned int size);
extern "C" void __cdecl free(void* block);
template <class T> inline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }
template <class T> struct CArray {
    T* m_begin;
    T* m_end;
    T* m_capacity;
    CArray(unsigned int count, const T& value);
    ~CArray() { if (m_begin) free(m_begin); }
    unsigned int size() const { return m_end - m_begin; }
    T& operator[](unsigned int i) const { return m_begin[i]; }
    static T* copy_ptrs(T* first, T* last, T* result)
    {
        if (first == last)
            return result;
        unsigned int bytes = (char*)last - (char*)first;
        return (T*)((char*)memmove(result, first, bytes) + bytes);
    }
    void erase(T* first, T* last) { m_end = copy_ptrs(last, m_end, first); }
    void clear() { erase(m_begin, m_end); }
    __forceinline void push_back(const T& x)
    {
        if (m_end != m_capacity) {
            if (m_end)
                *m_end = x;
            ++m_end;
            return;
        }
        unsigned int count = m_end - m_begin;
        unsigned int one = 1;
        unsigned int newCapacity = (max)(count, one) + count;
        T* data = newCapacity ? (T*)malloc(newCapacity * sizeof(T)) : 0;
        T* p;
        if (m_end == m_begin)
            p = data;
        else {
            unsigned int bytes = (char*)m_end - (char*)m_begin;
            p = (T*)((char*)memmove(data, m_begin, bytes) + bytes);
        }
        *p = x;
        if (m_begin)
            free(m_begin);
        m_begin = data;
        m_end = p + 1;
        m_capacity = data + newCapacity;
    }
};
struct CTriangleStripifier2 {
    void BuildCacheOptimizedVertexList(CArray<unsigned short>& unused, CArray<CArray<long> >& vertexFaces,
                                       CArray<CArray<unsigned short> >& faceVertices, CArray<unsigned short>& stripOfFace,
                                       CArray<CArray<unsigned short> >& strips, CArray<unsigned short>& vertexList);
};

void CTriangleStripifier2::BuildCacheOptimizedVertexList(CArray<unsigned short>& unused, CArray<CArray<long> >& vertexFaces,
                                                         CArray<CArray<unsigned short> >& faceVertices, CArray<unsigned short>& stripOfFace,
                                                         CArray<CArray<unsigned short> >& strips, CArray<unsigned short>& vertexList)
{
    unsigned int vertexCount = vertexFaces.size();
    vertexList.clear();
    CArray<bool> emitted(vertexCount, false);
    CArray<long> emittedAt(vertexCount, -1);
    long counter = 0;
    unsigned long vertex = 0;
    for (long s = 0; s < strips.size(); ++s) {
        CArray<unsigned short>& strip = strips[s];
        long now = counter + 1;
        for (unsigned long i = 0; i < strip.size(); ++i) {
            vertex = strip[i];
            long ahead = now;
            if (s > 0 && s < strips.size() - 1) {
                CArray<unsigned short>& next = strips[s + 1];
                long distance = now - counter - 1;
                for (unsigned long j = 0; j < next.size(); ++j) {
                    unsigned short w = next[j];
                    ++ahead;
                    bool needed = false;
                    CArray<unsigned short>& faces = faceVertices[w];
                    ++distance;
                    for (unsigned long k = 0; k < faces.size(); ++k) {
                        unsigned short f = faces[k];
                        if (stripOfFace[f] == s) {
                            long at = emittedAt[f];
                            if (at >= 0) {
                                if (ahead - at >= 16)
                                    goto next_strip;
                            } else {
                                if (f != vertex)
                                    continue;
                                if (distance >= 16)
                                    goto next_strip;
                            }
                            needed = true;
                        }
                    }
                    if (!needed)
                        break;
                }
            }
            emitted[vertex] = true;
            vertexList.push_back((unsigned short)vertex);
            emittedAt[vertex] = ++counter;
            ++now;
        }
    next_strip:;
    }
}
