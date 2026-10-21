// CTriangleStripifier2::GenerateTriangleListAndStrip(const CArray<C3DTriangle2>& triangles, CArray<C3DTriangle2>& triangleList, CArray<unsigned short>& strip) -- flatten the triangles to an index list, run the index-list overload and rebuild the reordered triangle list from its output
// Leaf: GenerateTriangleListAndStrip
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F3F3A0, 507 bytes + int3, `ret 0xc`.
// Signature from Ego_r.pdb: ?GenerateTriangleListAndStrip@CTriangleStripifier2@@UAEXABV?$CArray@VC3DTriangle2@@@@AAV2@AAV?$CArray@G@@@Z
// (virtual). Callees: the index-list overload GenerateTriangleListAndStrip(CArray<unsigned short>& x3) @ 0x00F3EA50
// (QAEXAAV?$CArray@G@@00@Z; catalogued as GenerateStrips), CArray<unsigned short>(size_type) @ 0x00A8D750,
// STLport __copy for 6-byte elements @ 0x00A744B0 (fastcall, the vector<C3DTriangle2> erase), vector<C3DTriangle2>
// _M_fill_insert @ 0x00A96F00 (the resize grow arm), memmove @ 0x00BFEAE6 (the inlined __copy_trivial of the
// unsigned short clear), free @ 0x00BFEA14 (the two local arrays, reverse order).
// Layout: CArray = retail STLport vector {first, last, end_of_storage} (12 bytes); C3DTriangle2 = ushort[3].
// Shape: both clears are STLport erase(begin(), end()); resize(n) is erase(begin()+n, end()) / insert(end(), n -
// size(), T()) with an empty user default constructor (no stores to the fill value).
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size);   // 0x00BFEAE6
extern "C" void __cdecl free(void* block);                                         // 0x00BFEA14

struct random_access_iterator_tag { };

struct C3DTriangle2 {
    unsigned short VertexIndex[3];
    C3DTriangle2() { }
};

C3DTriangle2* __fastcall CopyTriangles(const C3DTriangle2* first, const C3DTriangle2* last, C3DTriangle2* result,
                                       const random_access_iterator_tag& tag, int* distance);   // 0x00A744B0

inline void* CopyTrivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result
                           : (char*)memmove(result, first, (const char*)last - (const char*)first) +
                                 ((const char*)last - (const char*)first);
}

struct CTriangleArray {                                 // CArray<C3DTriangle2>
    C3DTriangle2* _M_start;
    C3DTriangle2* _M_finish;
    C3DTriangle2* _M_end_of_storage;
    C3DTriangle2* begin() const { return _M_start; }
    C3DTriangle2* end() const { return _M_finish; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    C3DTriangle2& operator[](unsigned long n) const { return *(begin() + n); }
    void _M_fill_insert(C3DTriangle2* position, unsigned long n, const C3DTriangle2& x);   // 0x00A96F00
    C3DTriangle2* erase(C3DTriangle2* first, C3DTriangle2* last)
    {
        C3DTriangle2* i = CopyTriangles(last, _M_finish, first, random_access_iterator_tag(), (int*)0);
        _M_finish = i;
        return first;
    }
    void insert(C3DTriangle2* position, unsigned long n, const C3DTriangle2& x) { _M_fill_insert(position, n, x); }
    void resize(unsigned long newSize, const C3DTriangle2& x)
    {
        if (newSize < size())
            erase(begin() + newSize, end());
        else
            insert(end(), newSize - size(), x);
    }
    void resize(unsigned long newSize) { resize(newSize, C3DTriangle2()); }
    void clear() { erase(begin(), end()); }
};

struct CIndexArray {                                    // CArray<unsigned short>
    unsigned short* _M_start;
    unsigned short* _M_finish;
    unsigned short* _M_end_of_storage;
    CIndexArray() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    explicit CIndexArray(unsigned long n);              // 0x00A8D750
    ~CIndexArray() { if (_M_start) free(_M_start); }
    unsigned short* begin() const { return _M_start; }
    unsigned short* end() const { return _M_finish; }
    unsigned short& operator[](unsigned long n) const { return *(begin() + n); }
    unsigned short* erase(unsigned short* first, unsigned short* last)
    {
        _M_finish = (unsigned short*)CopyTrivial(last, _M_finish, first);
        return first;
    }
    void clear() { erase(begin(), end()); }
};

void* AllocIndexStorage_00a8d750(unsigned long bytes);

#pragma auto_inline(off)
// Same-TU stand-ins for the out-of-line STLport instantiations (retail's TU instantiated them).
C3DTriangle2* __fastcall CopyTriangles(const C3DTriangle2* first, const C3DTriangle2* last, C3DTriangle2* result,
                                       const random_access_iterator_tag& tag, int* distance)
{
    for (; first != last; ++first, ++result)
        *result = *first;
    return result;
}
CIndexArray::CIndexArray(unsigned long n)
{
    _M_start = (unsigned short*)AllocIndexStorage_00a8d750(n * sizeof(unsigned short));
    _M_finish = _M_start + n;
    _M_end_of_storage = _M_finish;
}
void CTriangleArray::_M_fill_insert(C3DTriangle2* position, unsigned long n, const C3DTriangle2& x)
{
    for (unsigned long i = 0; i < n; ++i)
        *_M_finish++ = x;
}
#pragma auto_inline(on)

class CTriangleStripifier2 {
public:
    virtual void GenerateTriangleListAndStrip(const CTriangleArray& triangles, CTriangleArray& triangleList,
                                              CIndexArray& strip);
    void GenerateTriangleListAndStrip(CIndexArray& triangles, CIndexArray& triangleList, CIndexArray& strip);   // 0x00F3EA50
};

void CTriangleStripifier2::GenerateTriangleListAndStrip(const CTriangleArray& triangles, CTriangleArray& triangleList,
                                                        CIndexArray& strip)
{
    if (triangles.size() == 0) {
        triangleList.clear();
        strip.clear();
        return;
    }
    CIndexArray indices(triangles.size() * 3);
    CIndexArray listIndices;
    unsigned short* index = &indices[0];
    for (unsigned long i = 0; i < triangles.size(); ++i) {
        index[i * 3] = triangles[i].VertexIndex[0];
        index[i * 3 + 1] = triangles[i].VertexIndex[1];
        index[i * 3 + 2] = triangles[i].VertexIndex[2];
    }
    GenerateTriangleListAndStrip(indices, listIndices, strip);
    triangleList.resize(triangles.size());
    const unsigned short* listIndex = &listIndices[0];
    for (unsigned long i = 0; i < triangles.size(); ++i) {
        triangleList[i].VertexIndex[0] = listIndex[i * 3];
        triangleList[i].VertexIndex[1] = listIndex[i * 3 + 1];
        triangleList[i].VertexIndex[2] = listIndex[i * 3 + 2];
    }
}