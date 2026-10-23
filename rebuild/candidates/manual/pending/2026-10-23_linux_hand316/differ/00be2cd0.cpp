// CEngineLandscapeMeshBuilder::BuildSharedIndexBufferGenerationTables(C2DCoordI* vertices, unsigned short* indices, unsigned short* triangles, long width, long height, long* indexCount) -- walk a width x height grid of alternating-diagonal quads, give every new corner a shared vertex number through a 17x17 lookup (-1 = unused), write each triangle's three vertex numbers and stitch them into one strip in indices (a local index array stands in when indices is null)
// Grade: DIFFER(1091v1088) -- register/slot allocation only: vector start zero via edx (retail ebx), the value temp keeps its own frame word (retail shares [esp+0x10] with triangleCount), constant 1 in eax (retail edi), triangle store base +2 (retail +4)
// Leaf: BuildSharedIndexBufferGenerationTables
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BE2CD0, length 1088 bytes, `ret 0x18`, /GS cookie (frame 0x520: the
//   17x17 long lookup plus the unsigned short v[3] "string buffer" that triggers the cookie).
// Callees: InsertN 0x005149F0 (STLport vector<unsigned short> fill-insert, via the forceinline resize), free 0x00BFEA14.
#include <string.h>
#pragma intrinsic(memset)
extern "C" void __cdecl free(void* block);         // 0x00BFEA14

struct C2DCoordI {
    long X, Y;
};
template <class T> struct allocator { allocator() { } };
template <class V, class A> struct _STLP_alloc_proxy : A {
    V _M_data;
    _STLP_alloc_proxy(const A& a, V v) : A(a), _M_data(v) { }
};
template <class T> struct _Vector_base {
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, allocator<T> > _M_end_of_storage;
    _Vector_base(const allocator<T>& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, (T*)0) { }
    ~_Vector_base() { if (_M_start) free(_M_start); }
};
struct CIndexArray : _Vector_base<unsigned short> {   // retail 12-byte STLport vector<unsigned short>
    CIndexArray(const allocator<unsigned short>& a = allocator<unsigned short>()) : _Vector_base<unsigned short>(a) { }
    unsigned long size() const { return _M_finish - _M_start; }
    void InsertN(unsigned short* where, unsigned long count, const unsigned short& value);   // 0x005149F0
    __forceinline void resize(unsigned long n, const unsigned short& value)
    {
        if (n < size())
            _M_finish = _M_start + n;
        else
            InsertN(_M_finish, n - size(), value);
    }
    __forceinline void resize(unsigned long n) { resize(n, unsigned short()); }
    unsigned short* GetData() { return size() > 0 ? _M_start : (unsigned short*)this; }
};
struct CEngineLandscapeMeshBuilder {
    void BuildSharedIndexBufferGenerationTables(C2DCoordI* vertices, unsigned short* indices, unsigned short* triangles,
                                                long width, long height, long* indexCount);
};

void CEngineLandscapeMeshBuilder::BuildSharedIndexBufferGenerationTables(C2DCoordI* vertices, unsigned short* indices,
    unsigned short* triangles, long width, long height, long* indexCount)
{
    __int64 total = (__int64)width * height * 6;
    if (total < 0 || total > 0x7fffffff)
        return;
    long lookup[17][17];
    memset(lookup, -1, sizeof(lookup));
    CIndexArray localIndices;
    if (!indices) {
        localIndices.resize(width * height * 6);
        indices = localIndices.GetData();
    }
    long offsetX[2][2][3] = { { { 0, 1, 1 }, { 0, 0, 1 } }, { { 0, 1, 0 }, { 1, 0, 1 } } };
    long offsetY[2][2][3] = { { { 0, 0, 1 }, { 0, 1, 1 } }, { { 0, 0, 1 }, { 0, 1, 1 } } };
    long triangleCount = 0;
    long vertexCount = 0;
    *indexCount = 0;
    for (long x = 0; x < width; ++x) {
        for (long y = 0; y < height; ++y) {
            long parity = (y ^ x) & 1;
            for (long k = 0; k < 2; ++k) {
                long vx = offsetX[parity][k][0] + x;
                long vy = offsetY[parity][k][0] + y;
                long* slot = &lookup[vx][vy];
                long index = *slot;
                if (index < 0) {
                    index = vertexCount++;
                    vertices[index].X = vx;
                    vertices[index].Y = vy;
                    *slot = index;
                }
                unsigned short v[3];
                v[0] = (unsigned short)index;
                vx = offsetX[parity][k][1] + x;
                vy = offsetY[parity][k][1] + y;
                slot = &lookup[vx][vy];
                index = *slot;
                if (index < 0) {
                    index = vertexCount++;
                    vertices[index].X = vx;
                    vertices[index].Y = vy;
                    *slot = index;
                }
                v[1] = (unsigned short)index;
                vx = offsetX[parity][k][2] + x;
                vy = offsetY[parity][k][2] + y;
                slot = &lookup[vx][vy];
                index = *slot;
                if (index < 0) {
                    index = vertexCount++;
                    vertices[index].X = vx;
                    vertices[index].Y = vy;
                    *slot = index;
                }
                v[2] = (unsigned short)index;
                if (triangles) {
                    triangles[triangleCount * 3 + 2] = v[2];
                    if (k == 1) {
                        triangles[triangleCount * 3] = v[0];
                        triangles[triangleCount * 3 + 1] = v[1];
                    } else {
                        triangles[triangleCount * 3 + 1] = v[0];
                        triangles[triangleCount * 3] = v[1];
                    }
                }
                ++triangleCount;
                if (k == (*indexCount & 1)) {
                    if (*indexCount > 3) {
                        indices[*indexCount] = indices[*indexCount - 1];
                        indices[*indexCount + 1] = v[0];
                        indices[*indexCount + 2] = v[0];
                        indices[*indexCount + 3] = v[0];
                        indices[*indexCount + 4] = v[1];
                        indices[*indexCount + 5] = v[2];
                        *indexCount += 6;
                    } else {
                        indices[*indexCount] = v[0];
                        indices[*indexCount + 1] = v[0];
                        indices[*indexCount + 2] = v[1];
                        indices[*indexCount + 3] = v[2];
                        *indexCount += 4;
                    }
                } else {
                    if (*indexCount > 2 && v[0] == indices[*indexCount - 2] && v[1] == indices[*indexCount - 1]) {
                        indices[*indexCount] = v[2];
                        *indexCount += 1;
                    } else if (*indexCount < 3) {
                        indices[*indexCount] = v[0];
                        indices[*indexCount + 1] = v[1];
                        indices[*indexCount + 2] = v[2];
                        *indexCount += 3;
                    } else {
                        indices[*indexCount] = indices[*indexCount - 1];
                        indices[*indexCount + 1] = v[0];
                        indices[*indexCount + 2] = v[0];
                        indices[*indexCount + 3] = v[1];
                        indices[*indexCount + 4] = v[2];
                        *indexCount += 5;
                    }
                }
            }
        }
    }
}
