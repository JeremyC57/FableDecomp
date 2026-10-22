// N3DPrimitive::CAnimatedBlock::InitialiseFromMeshFileData(const CMeshFileData&, bool) -- copy the mesh's bone ids into the block's bone map and its vertex/weight counts; unless the data is shared, build a fresh ref-counted skin data object and copy the per-vertex bone indices and the per-vertex weight pairs into it
// Grade: DIFFER(620v629) -- structure right (sub-struct bone copy, 12-byte STLport set, skin-data release through a local); left: ebx/ebp role swap (retail keeps the mesh in ebp and zero in ebx, giving cmp dl,bl / cmp byte [flag],bl) and the nested weight-copy loop register allocation
// Leaf: InitialiseFromMeshFileData
#include <stddef.h>
extern "C" void __cdecl free(void* p);                                 // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dest, const void* src, size_t count);   // 0x00BFEAE6
extern "C" void* __cdecl memset(void* dest, int c, size_t count);
#pragma intrinsic(memset)
inline void* CopyTrivial(const void* first, const void* last, void* result)
{
    if (last == first)
        return result;
    size_t n = (const char*)last - (const char*)first;
    return (char*)memmove(result, first, n) + n;
}
struct CLongVector {                            // STLport vector<long>
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    CLongVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    CLongVector(const CLongVector& x);          // 0x00535470
    ~CLongVector() { if (_M_start) free(_M_start); }
    size_t size() const { return _M_finish - _M_start; }
    long& operator[](size_t n) { return *(_M_start + n); }
    const long& operator[](size_t n) const { return *(_M_start + n); }
    void _M_fill_insert(long* pos, size_t n, const long& x);   // 0x004649A3
    long* erase(long* first, long* last)
    {
        long* i = (long*)CopyTrivial(last, _M_finish, first);
        _M_finish = i;
        return first;
    }
    void resize(size_t new_size, const long& x = long())
    {
        if (new_size < size())
            erase(_M_start + new_size, _M_finish);
        else
            _M_fill_insert(_M_finish, new_size - size(), x);
    }
};
struct CWeightPair {                            // 8-byte bone index / weight pair
    long Bone;
    float Weight;
    CWeightPair() {}
};
struct CPairVector {                            // STLport vector<CWeightPair>
    CWeightPair* _M_start;
    CWeightPair* _M_finish;
    CWeightPair* _M_end_of_storage;
    CPairVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    size_t size() const { return _M_finish - _M_start; }
    CWeightPair& operator[](size_t n) { return *(_M_start + n); }
    void _M_fill_insert(CWeightPair* pos, size_t n, const CWeightPair& x);   // 0x00A97180
    CWeightPair* erase(CWeightPair* first, CWeightPair* last)
    {
        CWeightPair* result = first;
        for (long n = last - first; n > 0; --n) {
            *result = *last;
            ++last;
            ++result;
        }
        _M_finish = result;
        return first;
    }
    void resize(size_t new_size, const CWeightPair& x = CWeightPair())
    {
        if (new_size < size())
            erase(_M_start + new_size, _M_finish);
        else
            _M_fill_insert(_M_finish, new_size - size(), x);
    }
};
struct CTreeNode {
    long Color;
    CTreeNode* Parent;                          // +0x04
    CTreeNode* Left;                            // +0x08
    CTreeNode* Right;                           // +0x0c
    long Value;
};
struct CLongSet {                               // STLport set<long>: header node pointer + count
    CTreeNode* _M_header;
    size_t _M_node_count;
    struct less { } _M_key_compare;
    CLongSet(const CLongSet& x);                // 0x00A96E40
    void _M_erase(CTreeNode* x)
    {
        while (x != 0) {
            _M_erase(x->Right);
            CTreeNode* y = x->Left;
            free(x);
            x = y;
        }
    }
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_header->Parent);
            _M_header->Left = _M_header;
            _M_header->Parent = 0;
            _M_header->Right = _M_header;
            _M_node_count = 0;
        }
    }
    ~CLongSet() { clear(); if (_M_header) free(_M_header); }
};
struct CMeshBones {
    CLongVector Ids;                            // +0x00
    CLongSet Set;                               // +0x0c
};
struct CMeshFileData {
    unsigned char _pad00[0x2c];
    CMeshBones Bones;                           // +0x2c
    long NumWeightsPerVertex;                   // +0x44 (retail: read as a long in the copy loop)
    CLongVector VertexBones;                    // +0x48
    CWeightPair* Weights;                       // +0x54 (vector start)
};
struct __declspec(novtable) CRefCounted {
    virtual void v00();
    virtual void DeleteThis();                  // slot 1
    long RefCount;                              // +0x04
    CRefCounted() : RefCount(1) {}
};
struct CSkinData : CRefCounted {                // vtable 0x0129DF88, 0x20 bytes
    CLongVector Bones;                          // +0x08
    CPairVector Weights;                        // +0x14
    virtual void v00();
};
namespace N3DPrimitive {
struct CAnimatedBlock {
    unsigned char _pad00[0x10];
    unsigned long NumVertices;                  // +0x10
    unsigned short NumWeightsPerVertex;         // +0x14
    unsigned char Initialised;                  // +0x16
    unsigned char NumBones;                     // +0x17
    unsigned char BoneMap[18];                  // +0x18
    unsigned char _pad2a[2];
    CSkinData* PSkinData;                       // +0x2c CCountedPointer
    void InitialiseFromMeshFileData(const CMeshFileData& mesh, bool shared);
};
}
using namespace N3DPrimitive;

void CAnimatedBlock::InitialiseFromMeshFileData(const CMeshFileData& mesh, bool shared)
{
    CMeshBones bones(mesh.Bones);
    NumBones = (unsigned char)bones.Ids.size();
    memset(BoneMap, 0, sizeof(BoneMap));
    for (unsigned long i = 0; i < NumBones; ++i)
        BoneMap[i] = (unsigned char)bones.Ids[i];
    NumVertices = mesh.VertexBones.size();
    NumWeightsPerVertex = (unsigned short)mesh.NumWeightsPerVertex;
    Initialised = 1;
    if (!shared) {
        CSkinData* data = new CSkinData;
        CSkinData* old = PSkinData;
        if (old) {
            if (--old->RefCount == 0)
                old->DeleteThis();
            PSkinData = 0;
        }
        PSkinData = data;
        data->Bones.resize(NumVertices);
        for (unsigned long v = 0; v < NumVertices; ++v)
            PSkinData->Bones[v] = mesh.VertexBones[v];
        PSkinData->Weights.resize(NumWeightsPerVertex * NumVertices);
        for (unsigned long j = 0; j < NumVertices; ++j)
            for (unsigned long k = 0; k < NumWeightsPerVertex; ++k)
                PSkinData->Weights[NumWeightsPerVertex * j + k] = mesh.Weights[mesh.NumWeightsPerVertex * j + k];
    }
}
