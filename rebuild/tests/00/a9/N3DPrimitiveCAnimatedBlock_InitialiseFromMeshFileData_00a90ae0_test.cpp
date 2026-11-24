// Behavior fixture for animated primitive mesh-file data, VC7.1 /Od /W3 /GR-.
// Leaf: ?InitialiseFromMeshFileData@CAnimatedBlock@N3DPrimitive@@QAEXABVC3DMeshFileXAnimatedPrimitiveBlockChunk@@_N@Z
// Flags:
#define FABLE_RETAIL_ANIMATEDPRIMITIVEBLOCK_ACCESSORS_PASS27
#define FABLE_RETAIL_C3DPRIMITIVE2_LOADER
#define FABLE_RETAIL_C3DPRIMITIVE2_ANIMATEDBLOCKS
#define FABLE_RETAIL_CANIMATEDBLOCK_DATA
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ORIGINAL_CASTTOBASE_PASS22
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_CTOR
#define FABLE_RETAIL_CVERTEXGROUPBLOCK_MEMBERS
#define FABLE_RETAIL_CCHUNKEDFILECHUNK
#define FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK
#define FABLE_RETAIL_C3DINDEXEDBONEWEIGHT_CTOR_PASS27
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_STLP_MAP_COPY_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_COPY_DECL_ONLY
#define FABLE_RETAIL_CARRAY_ORIGINAL_INDEX_BASE_PASS22
#define FABLE_RETAIL_STLP_VECTOR_ULONG_ERASE_COPY_PASS27
#include <string.h>
#include "fable_assert.h"
#include "engine/EVertexType.h"
#include "engine/C3DPrimitive2.h"
#include "engine/C3DMeshFileXAnimatedPrimitiveBlockChunk.h"

// 00A96DA0 is the emitted copy of the debug donor's inline group-block destructor.

// The fixture owns only the I/O and container dependencies that are external to the leaf.
#include <assert.h>
#include <stdio.h>

namespace std {
template<class K,class V,class Compare,class A>
map<K,V,Compare,A>::map(const map& source) : _M_t(Compare(),A())
{
    assert(source.begin() == source._M_t.end());
}

template<class T,class A>
vector<T,A>::vector(const vector& source) : _Vector_base<T,A>(source.size(),source.get_allocator())
{
    this->_M_finish = __uninitialized_copy(source.begin(), source.end(),
        this->_M_start, typename __type_traits<T>::is_POD_type());
}

template<class T,class A>
void vector<T,A>::_M_fill_insert(iterator position,size_type count,const T& value)
{
    assert(position == this->_M_finish);
    const size_type old_size = size();
    if (!count) return;
    const size_type new_size = old_size + count;
    T* storage = this->_M_end_of_storage.allocate(new_size);
    for (size_type i = 0; i != old_size; ++i)
        _Construct(storage+i,this->_M_start[i]);
    for (size_type i = old_size; i != new_size; ++i)
        _Construct(storage+i,value);
    if (this->_M_start) {
        _Destroy(this->_M_start,this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start,
            this->_M_end_of_storage._M_data-this->_M_start);
    }
    this->_M_start = storage;
    this->_M_finish = storage + new_size;
    this->_M_end_of_storage._M_data = this->_M_finish;
}
}

static unsigned long initial_groups[18];
static unsigned long initial_group_count;
static unsigned int deletes;

CVertexGroupBlock::CVertexGroupBlock()
{
    Groups.resize(initial_group_count);
    for (unsigned long i=0; i<initial_group_count; ++i)
        Groups[i] = initial_groups[i];
}

inline CVertexGroupBlock::~CVertexGroupBlock() {}

void CIVCountedPointeeBase::Delete() const
{
    ++deletes;
    delete this;
}

CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}

CChunkedFileChunk::CChunkedFileChunk(const char* code) : Parent(0) { strcpy(ChunkType,code); }
CChunkedFileChunk::~CChunkedFileChunk() {}
const CCharString CChunkedFileChunk::GetChunkDescription() const { abort(); }
const CCharString CChunkedFileChunk::GetChunkInfo() const { abort(); }
int CChunkedFileChunk::IsSuperChunk() const { return 0; }
int CChunkedFileChunk::IsUnknownType() const { return 0; }
CChunkedFileSuperChunk* CChunkedFileChunk::GetAsSuperChunk() { return 0; }
const CChunkedFileSuperChunk* CChunkedFileChunk::GetAsSuperChunk() const { return 0; }
CChunkedFileChunk* CChunkedFileChunk::InitNewInstance(CChunkedFileChunk*) const { abort(); }

CChunkedFileSuperChunk::CChunkedFileSuperChunk(const char* code) : CChunkedFileChunk(code), CurrentChunkIndex(0) {}
CChunkedFileSuperChunk::~CChunkedFileSuperChunk() {}
int CChunkedFileSuperChunk::IsSuperChunk() const { return 1; }
const CChunkedFileSuperChunk* CChunkedFileSuperChunk::GetAsSuperChunk() const { return this; }
CChunkedFileSuperChunk* CChunkedFileSuperChunk::GetAsSuperChunk() { return this; }
void CChunkedFileSuperChunk::ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&) { abort(); }
void CChunkedFileSuperChunk::WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const { abort(); }
CChunkedFileChunk* CChunkedFileSuperChunk::InitNewInstance(CChunkedFileChunk*) const { abort(); }

C3DMeshFileXAnimatedPrimitiveBlockChunk::~C3DMeshFileXAnimatedPrimitiveBlockChunk() {}
const CCharString C3DMeshFileXAnimatedPrimitiveBlockChunk::GetChunkDescription() const { abort(); }
void C3DMeshFileXAnimatedPrimitiveBlockChunk::ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&) { abort(); }
void C3DMeshFileXAnimatedPrimitiveBlockChunk::WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const { abort(); }
CChunkedFileChunk* C3DMeshFileXAnimatedPrimitiveBlockChunk::CreateNewInstance() const { abort(); }

void N3DPrimitive::CAnimatedBlock::InitialiseFromMeshFileData(const C3DMeshFileXAnimatedPrimitiveBlockChunk& file_data, bool vb_built)
{
    CVertexGroupBlock group_block = file_data.GetGroupBlock();
    GroupCount = (unsigned char)group_block.GetGroupCount();
    FABLE_RETAIL_ASSERT(GroupCount <= 18);
    memset(Groups, 0, sizeof(Groups));
    for (unsigned long c_group = 0; c_group < GroupCount; ++c_group)
        Groups[c_group] = (unsigned char)group_block.GetGroup(c_group);

    VertexCount = file_data.GetVertexCount();
    BonesPerVertex = (unsigned short)file_data.GetBonesPerVertex();
    PalettedFlag = true;

    if (!vb_built)
    {
        Data.Reset(new CDeviceIndependent);
        Data->VertexList.resize(VertexCount);
        for (unsigned long c_vertex = 0; c_vertex < VertexCount; ++c_vertex)
            Data->VertexList[c_vertex] = file_data.GetVertex(c_vertex);

        Data->BoneWeights.resize(VertexCount * BonesPerVertex);
        for (unsigned long c_vertex = 0; c_vertex < VertexCount; ++c_vertex)
            for (unsigned long c_bone = 0; c_bone < BonesPerVertex; ++c_bone)
                Data->BoneWeights[BonesPerVertex * c_vertex + c_bone] =
                    file_data.GetBoneWeight(c_vertex, c_bone);
    }
}

static void check_groups(const N3DPrimitive::CAnimatedBlock& block)
{
    assert(block.GroupCount == initial_group_count);
    for (unsigned long i=0; i<sizeof(block.Groups); ++i)
        assert(block.Groups[i] == (i<initial_group_count ? static_cast<unsigned char>(initial_groups[i]) : 0));
}

static void populate(C3DMeshFileXAnimatedPrimitiveBlockChunk& input, unsigned long vertices, unsigned long per_vertex)
{
    input.BonesPerVertex = per_vertex;
    input.VertexList.resize(vertices);
    input.BoneWeights.resize(vertices * per_vertex);
    for (unsigned long vertex=0; vertex<vertices; ++vertex) {
        input.VertexList[vertex] = 400 + 17*vertex;
        for (unsigned long bone=0; bone<per_vertex; ++bone) {
            unsigned long i = vertex * per_vertex + bone;
            input.BoneWeights[i].BoneIndex = 1000 + vertex*100 + bone;
            input.BoneWeights[i].Weight = 0x76540000 + i;
        }
    }
}

int main()
{
    typedef N3DPrimitive::CAnimatedBlock Block;
    {
        initial_group_count = 18;
        for (unsigned long i=0; i<initial_group_count; ++i)
            initial_groups[i] = 0x120 + i*9;
        C3DMeshFileXAnimatedPrimitiveBlockChunk mesh;
        populate(mesh,3,4);
        Block out;
        out.PrimitiveCount=77;
        out.BoneConstantsFixupOffset=0xaabbccdd;
        out.InitialiseFromMeshFileData(mesh,false);
        check_groups(out);
        assert(out.PalettedFlag && out.VertexCount == 3 && out.BonesPerVertex == 4);
        assert(out.PrimitiveCount == 77 && out.BoneConstantsFixupOffset == 0xaabbccdd);
        assert(out.Data->VertexList.size() == 3 && out.Data->BoneWeights.size() == 12);
        for (unsigned long i=0; i<3; ++i) {
            assert(out.Data->VertexList[i] == mesh.VertexList[i]);
            for (unsigned long j=0; j<4; ++j) {
                unsigned long index = 4*i+j;
                assert(out.Data->BoneWeights[index].BoneIndex == mesh.BoneWeights[index].BoneIndex);
                assert(out.Data->BoneWeights[index].Weight == mesh.BoneWeights[index].Weight);
            }
        }
        Block::CDeviceIndependent* saved = out.Data.operator->();
        initial_group_count = 2;
        initial_groups[0] = 257;
        initial_groups[1] = 255;
        C3DMeshFileXAnimatedPrimitiveBlockChunk built;
        built.BonesPerVertex = 0x12345;
        out.InitialiseFromMeshFileData(built,true);
        check_groups(out);
        assert(out.PalettedFlag && out.VertexCount == 0 && out.BonesPerVertex == 0x2345);
        assert(out.Data.operator->() == saved && deletes == 0);
        assert(out.Data->VertexList.size() == 3 && out.Data->BoneWeights.size() == 12);

        initial_group_count = 0;
        C3DMeshFileXAnimatedPrimitiveBlockChunk empty;
        populate(empty,0,3);
        out.InitialiseFromMeshFileData(empty,false);
        check_groups(out);
        assert(out.PalettedFlag && out.VertexCount == 0 && out.BonesPerVertex == 3);
        assert(out.Data->VertexList.size() == 0 && out.Data->BoneWeights.size() == 0);
        assert(out.Data.operator->() != saved && deletes == 1);

        C3DMeshFileXAnimatedPrimitiveBlockChunk no_bones;
        populate(no_bones,2,0);
        out.InitialiseFromMeshFileData(no_bones,false);
        assert(out.VertexCount == 2 && out.BonesPerVertex == 0 && out.PalettedFlag);
        assert(out.Data->VertexList[1] == 417 && out.Data->BoneWeights.size() == 0);
        assert(deletes == 2);
    }
    assert(deletes == 3);
    {
        C3DMeshFileXAnimatedPrimitiveBlockChunk mesh;
        populate(mesh,1,2);
        Block out;
        out.InitialiseFromMeshFileData(mesh,true);
        assert(out.PalettedFlag && out.VertexCount == 1 && out.BonesPerVertex == 2);
        assert(out.Data.operator->() == 0 && deletes == 3);
    }
    puts("00a90ae0_TEST PASS");
    return 0;
}