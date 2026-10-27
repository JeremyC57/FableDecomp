// Behaviour test for 0x00AC4F30 CMirroredTextureHighlightsChunk::GetList() -- the index array, or the CArray's own address when empty
#include <cstdio>
template <class T> class CArray {
public:
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    T& operator[](unsigned int n) { return _M_start[n]; }
    T* GetAsCArray() { return size() > 0 ? &(*this)[0] : GetZeroSizeArray(); }
private:
    T* GetZeroSizeArray() { return reinterpret_cast<T*>(this); }
};
class CMirroredTextureHighlightsChunk { public:
    CArray<unsigned long> TriangleVertexIndexList;
    unsigned long* GetList() { return TriangleVertexIndexList.GetAsCArray(); }
};
int main()
{
    unsigned long data[3] = { 1, 2, 3 };
    CMirroredTextureHighlightsChunk c;
    c.TriangleVertexIndexList._M_start = data; c.TriangleVertexIndexList._M_finish = data;
    if (c.GetList() != reinterpret_cast<unsigned long*>(&c.TriangleVertexIndexList)) { std::printf("00ac4f30_TEST FAIL\n"); return 1; }
    c.TriangleVertexIndexList._M_finish = data + 3;
    if (c.GetList() != data) { std::printf("00ac4f30_TEST FAIL\n"); return 1; }
    std::printf("00ac4f30_TEST PASS\n");
    return 0;
}