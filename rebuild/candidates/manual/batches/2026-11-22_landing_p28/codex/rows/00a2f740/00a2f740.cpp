// C3DMeshStats::InitialiseFromFileData(const CCharString& name, const C3DMeshFile& file, bool animated, CMovableResourceMemoryPool* pool)
//   -- build a mesh's stats block from its file: name, animated flag and pool stored; from the root chunk's helpers, every helper
//   point (keyed by the CRC of its name, name collected) and every dummy object -- a dummy named "createmesh <mesh>" becomes a mesh
//   generator instead -- and every mesh volume (keyed by name CRC, the volume map sorted); the collected names packed, the buffers
//   allocated, the helper points and dummy objects copied into their pre-allocated maps and sorted, the packed names copied in; the
//   bounding sphere from the XBDS chunk and the bounding box (centre +- diagonal) from the XBDB chunk; returns true
// Leaf: ?InitialiseFromFileData@C3DMeshStats@@QAE_NABVCCharString@@ABVC3DMeshFile@@_NPAVCMovableResourceMemoryPool@@@Z
// Flags: /Gr
// Inline-budget: shared SetCenterDiag expansion; the imported plain-inline ablation is recorded in handback_HDR_README.md.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A2F740, 3051 bytes, thiscall, `ret 10h`, frame 0x170, no EH frame.
// Identity (proven): Ego_r.pdb / FableWin.pdb ?InitialiseFromFileData@C3DMeshStats@@QAE_NABVCCharString@@ABVC3DMeshFile@@_NPAVCMovableResourceMemoryPool@@@Z
//   (0x017C4CFF / 0x031E2E40 PDB entries); its callees are the landed C3DMeshStats::PackNames 0x00A2EEF0 / AllocateBuffers 0x00A2E760.
// Callees (all landed or CRT): CCharString::operator= 0x0099EFB0, C3DMeshFile::GetRoot 0x00A32620, malloc 0x00BFEA0E (the two set headers),
//   C3DMeshFileRootChunk::HasHelpers 0x00A9B2C0 / GetHelpers 0x00A9B3D0, the six C3DMeshFileHelpersChunk finders 0x00AAA270 / 0x00AAA300 /
//   0x00AAA550 / 0x00AAA5E0 / 0x00AAA830 / 0x00AAA8C0, vector<pair>::_M_insert_overflow 0x00A33D90 / 0x00A33E80 / 0x00A34F00,
//   _Rb_tree<CCharString>::insert_unique 0x004AC380 / _M_erase 0x004ABA70, CCharString(const char*, long) 0x0099EBF0, GetAsLowerCase
//   0x0099ED10, strncmp 0x00BFEAF8, CCharString copy 0x0099EC30 / dtor 0x0099EAE0, vector<C3DGeneratorObject>::push_back 0x00A345D0,
//   C3DMeshVolume(const chunk&, pool*) 0x00A2E9A0, std::sort instantiations 0x00A355F0 / 0x00A35150 / 0x00A351B0, PackNames 0x00A2EEF0,
//   AllocateBuffers 0x00A2E760, std::__copy 0x00A31150 / 0x00A311B0, memmove 0x00BFEAE6, CChunkedFileSuperChunk::FindFirstSubChunkOfType
//   0x00AA7F40 ("XBDS" 0x0129CEF0, "XBDB" 0x0129CEF8), __destroy_aux 0x00A33720, free 0x00BFEA14, operator delete 0x00BFE9BC.
#define FABLE_RETAIL_STLP_RB_INCREMENT_SIZE_OWNER_PASS27
#define FABLE_RETAIL_STLP_FILL_CHAR
#define FABLE_RETAIL_MESH_BOOTSTRAP_BOUNDS_PASS28
#define FABLE_RETAIL_CDATASTREAM
#define FABLE_RETAIL_STLP_SET
#define FABLE_RETAIL_STLP_SET_EMPTY
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM_WRITESTRING
#define FABLE_RETAIL_CMEMORYDATAOUTPUTSTREAM
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_GETASCHARARRAY_NULLRETURN   // EVIDENCED (15th agent): the three inline GetCRC loops index [ecx+esi] (counter base) as retail 0x00A2F8A0 / 0x00A2FB90 / 0x00A2FD20; the landed same-shape sibling 0x00AF4900 carries this opt-in; PackNames 0x00A2EEF0 still RELOCATION_MATCHes in the TU
#define FABLE_RETAIL_CCHARSTRING_STARTSWITH
#define FABLE_RETAIL_CCHARSTRING_GETASLOWERCASE
#define FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE
#define FABLE_RETAIL_CCRC_CALC_BODY
#define FABLE_RETAIL_STLP_COPY
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CCHUNKEDFILECHUNK
#define FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK
#define FABLE_RETAIL_C3DMESHFILEHELPERSCHUNK
#define FABLE_RETAIL_C3DMESHFILEHELPERSCHUNK_FINDERS
#define FABLE_RETAIL_C3DMESHFILEROOTCHUNK_HELPERS
#define FABLE_RETAIL_C3DMESHFILEHELPERPOINTCHUNK
#define FABLE_RETAIL_C3DMESHFILEHELPERPOINTCHUNK_GETTERS
#define FABLE_RETAIL_C3DMESHFILEDUMMYOBJECTCHUNK
#define FABLE_RETAIL_C3DMESHFILEDUMMYOBJECTCHUNK_GETTERS
#define FABLE_RETAIL_C3DMESHFILECONVEXVOLUMECHUNK_GETTERS
#define FABLE_RETAIL_C3DMESHFILEXBOUNDINGSPHERECHUNK
#define FABLE_RETAIL_C3DPRIMITIVE2_LOADER
#define FABLE_RETAIL_C3DMESHFILEXBOUNDINGBOXCHUNK
#define FABLE_RETAIL_C3DMESHFILEXBOUNDINGBOXCHUNK_GETTERS
#define FABLE_RETAIL_C3DBOUNDINGBOX
#define FABLE_RETAIL_C3DBOUNDINGBOX_SETCENTERDIAG
#define FABLE_RETAIL_CVECTORMAP_INSERT
#define FABLE_RETAIL_CVECTORMAP_CTOR
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CCHUNKEDFILE_GETCHUNKEDFILEROOT
#define FABLE_RETAIL_C3DMESHSTATS_GENERATORS
#define FABLE_RETAIL_C3DMESHSTATS_FILE_LOADER_PASS28
#define FABLE_RETAIL_C3DHELPERPOINT_LAYOUT_PASS27
#define FABLE_RETAIL_C3DMESHSTATS_INITIALISEFROMFILEDATA
#define FABLE_RETAIL_C3DDUMMYOBJECT_TRANSFORMCTOR
#define FABLE_RETAIL_C3DGENERATOROBJECT_CTOR
#define FABLE_RETAIL_C3DMESHVOLUME_CHUNKCTOR
#include "engine/C3DMeshStats.h"
#include "engine/C3DMeshFile.h"
#include "engine/C3DMeshFileRootChunk.h"
#include "engine/C3DMeshFileHelpersChunk.h"
#include "engine/C3DMeshFileHelperPointChunk.h"
#include "engine/C3DMeshFileDummyObjectChunk.h"
#include "engine/C3DMeshFileConvexVolumeChunk.h"
#include "engine/C3DMeshFileXBoundingSphereChunk.h"
#include "engine/C3DMeshFileXBoundingBoxChunk.h"

#include "engine/CMemoryDataOutputStream.h"
extern "C" void* __cdecl memcpy(void* dst, const void* src, unsigned int size);
#pragma intrinsic(memcpy)
// Same-TU callee (landed 0x00A2EEF0 C3DMeshStats::PackNames, its text on the shared headers), defined above the row as in retail's
// TU: c2 sees that the packed-names array's address is not captured (resize's _M_fill_insert body visible through
// FABLE_RETAIL_STLP_VECTOR), so the row keeps the array's begin / end / size in ebx / edi / esi across AllocateBuffers and the copies.

// STLport byte fills at 004110D0 use the character overloads of fill and fill_n.
template<> inline void std::vector<char>::_M_fill_insert(char* position, size_type n, const char& x)
{
    if (n) {
        if (size_type(this->_M_end_of_storage._M_data - this->_M_finish) >= n) {
            char x_copy = x;
            const size_type elems_after = this->_M_finish - position;
            char* old_finish = this->_M_finish;
            if (elems_after > n) {
                char* first = old_finish - n;
                if (old_finish != first) memmove(old_finish, first, old_finish - first);
                this->_M_finish += n;
                int remaining = old_finish - n - position;
                if (remaining > 0) memmove(old_finish - remaining, position, remaining);
                memset(position, (unsigned char)x_copy, n);
            } else {
                memset(this->_M_finish,(unsigned char)x_copy,n-elems_after);
                this->_M_finish += n-elems_after;
                if (old_finish != position) memmove(this->_M_finish, position, old_finish - position);
                this->_M_finish += elems_after;
                memset(position, (unsigned char)x_copy, old_finish - position);
            }
        } else {
            __true_type t;
            this->_M_insert_overflow(position,x,t,n,false);
        }
    }
}
#define FABLE_RETAIL_CHANDLEBASEDMEMORYPOOL
#define FABLE_RETAIL_CMOVABLERESOURCEMEMORYPOOL
#include "engine/CMovableResourceMemoryPool.h"

// Original FableWin member stores Elements and Size; native 00A2E760 expands these two stores.
template<> inline void CPreAllocVectorMap<unsigned long,C3DHelperPoint>::Initialise(value_type* elements,unsigned long size)
{
    Elements = elements;
    Size = size;
}

// Original FableWin member stores Elements and Size; native 00A2E760 expands these two stores.
template<> inline void CPreAllocVectorMap<unsigned long,C3DDummyObject>::Initialise(value_type* elements,unsigned long size)
{
    Elements = elements;
    Size = size;
}


CCountedPointer<const C3DMeshFileRootChunk> C3DMeshFile::GetRoot() const
{
    CCountedPointer<const C3DMeshFileRootChunk> root;
    root.SetStaticCast(CCountedPointer<const CChunkedFileSuperChunk>(GetChunkedFileRoot()));
    return root;
}
bool C3DMeshStats::AllocateBuffers(unsigned long helper_points, unsigned long dummy_objects, unsigned long packed_names_size)
{
    typedef std::pair<unsigned long, C3DHelperPoint> PointEntry;
    typedef std::pair<unsigned long, C3DDummyObject> DummyEntry;
#ifdef _MSC_VER
    typedef unsigned __int64 Size64;
#else
    typedef unsigned long long Size64;
#endif
    Size64 total = Size64(dummy_objects) * sizeof(DummyEntry) + Size64(helper_points) * sizeof(PointEntry) + packed_names_size;
    PackedNames = 0;
    PackedNamesSize = static_cast<unsigned short>(packed_names_size);
    if (total > 0xffffffffu)
        return false;
    unsigned long dummy_size = dummy_objects * sizeof(DummyEntry);
    unsigned long point_size = helper_points * sizeof(PointEntry);
    unsigned long size = point_size + dummy_size + packed_names_size;
    if (size) {
        if (!MemoryPool->AllocateResource(&StatsMemoryHandle, this, size, 0))
            return false;
        char* base = static_cast<char*>(MemoryPool->GetPointerFromHandle(StatsMemoryHandle));
        HelperPoints.Initialise(reinterpret_cast<PointEntry*>(base), helper_points);
        base += point_size;
        DummyObjects.Initialise(reinterpret_cast<DummyEntry*>(base), dummy_objects);
        if (PackedNamesSize)
            PackedNames = reinterpret_cast<char*>(reinterpret_cast<DummyEntry*>(base) + dummy_objects);
    }
    return true;
}
void C3DMeshStats::PackNames(CArray<char>& packed_names, const std::set<CCharString>& helper_point_names,
                             const std::set<CCharString>& dummy_object_names)
{
    if (helper_point_names.empty() && dummy_object_names.empty())
        return;
    CMemoryDataOutputStream stream;
    stream.WriteUWORD(0);
    for (std::set<CCharString>::iterator it = helper_point_names.begin(); it != helper_point_names.end(); ++it)
        if (it->GetLength() != 0)
            stream.WriteNullTerminatedString(*it);
    stream.WriteNullTerminatedString("");
    unsigned long dummy_offset = stream.GetPosition();
    for (std::set<CCharString>::iterator it2 = dummy_object_names.begin(); it2 != dummy_object_names.end(); ++it2)
        if (it2->GetLength() != 0)
            stream.WriteNullTerminatedString(*it2);
    stream.WriteNullTerminatedString("");
    stream.SetPosition(0);
    stream.WriteUWORD((unsigned short)dummy_offset);
    unsigned long length = stream.GetLength();
    packed_names.resize(length);
    memcpy(packed_names.GetAsCArray(), stream.PeekData(), length);
}

bool C3DMeshStats::InitialiseFromFileData(const CCharString& name, const C3DMeshFile& file_data, bool animated, CMovableResourceMemoryPool* pool)
{
    Name = name;
    AnimatedFlag = animated;
    MemoryPool = pool;
    CCountedPointer<const C3DMeshFileRootChunk> root = file_data.GetRoot();
    CVectorMap<unsigned long, C3DHelperPoint> helper_points;
    CVectorMap<unsigned long, C3DDummyObject> dummy_objects;
    CVectorMap<unsigned long, C3DMeshVolume> mesh_volumes;   // never used: retail destroys an empty pair<unsigned long, C3DMeshVolume> vector at 0x00A302C8 (__destroy_aux(0, 0)) between the name sets and dummy_objects
    std::set<CCharString> helper_point_names;
    std::set<CCharString> dummy_object_names;
    if (root->HasHelpers()) {
        CCountedPointer<const C3DMeshFileHelpersChunk> helpers = root->GetHelpers();
        CCountedPointer<const C3DMeshFileHelperPointChunk> point_data = helpers->FindFirstPoint();
        while (point_data.Get()) {
            C3DHelperPoint helper_point(point_data->GetPosition(), point_data->GetBoneIndex());
            helper_points.Insert(std::pair<unsigned long, C3DHelperPoint>(point_data->GetName().GetCRC(), helper_point));
            helper_point_names.insert(point_data->GetName());
            point_data = helpers->FindNextPoint(point_data);
        }
        CCountedPointer<const C3DMeshFileDummyObjectChunk> dummy_data = helpers->FindFirstDummy();
        while (dummy_data.Get()) {
            C3DDummyObject dummy(dummy_data->GetTransform(), dummy_data->GetBoneIndex());
            const CCharString& dummy_name = dummy_data->GetName();
            bool local_origin = dummy_data->GetUseLocalOrigin();
            if (dummy_name.GetAsLowerCase().StartsWith("createmesh ")) {
                CCharString objname(dummy_name.GetAsCharArray() + strlen("createmesh "));
                C3DGeneratorObject generator(dummy, local_origin, objname, 0);
                MeshGenerators.push_back(generator);
            } else {
                dummy_objects.Insert(std::pair<unsigned long, C3DDummyObject>(dummy_name.GetCRC(), dummy));
                dummy_object_names.insert(dummy_name);
            }
            dummy_data = helpers->FindNextDummy(dummy_data);
        }
        CCountedPointer<const C3DMeshFileConvexVolumeChunk> volume_data = helpers->FindFirstVolumeW();
        while (volume_data.Get()) {
            C3DMeshVolume mesh_volume(*volume_data, pool);
            MeshVolumes.Insert(std::pair<unsigned long, C3DMeshVolume>(volume_data->GetName().GetCRC(), mesh_volume));
            volume_data = helpers->FindNextVolumeW(volume_data);
        }
        MeshVolumes.Update();
    }
    CArray<char> packed_names;
    PackNames(packed_names, helper_point_names, dummy_object_names);
    AllocateBuffers(helper_points.size(), dummy_objects.size(), packed_names.size());
    std::copy(helper_points.begin(), helper_points.end(), HelperPoints.begin());
    HelperPoints.Sort();
    std::copy(dummy_objects.begin(), dummy_objects.end(), DummyObjects.begin());
    DummyObjects.Sort();
    std::copy(packed_names.begin(), packed_names.end(), PackedNames);
    CCountedPointer<const C3DMeshFileXBoundingSphereChunk> bounding_sphere_data;
    bounding_sphere_data.SetStaticCast(root->FindFirstSubChunkOfType(C3DMeshFileXBoundingSphereChunk::GetStaticChunkType()));
    if (bounding_sphere_data.Get()) {
        BoundingSphere.SetCenter(bounding_sphere_data->GetCenter());
        BoundingSphere.SetRadius(bounding_sphere_data->GetRadius());
    }
    CCountedPointer<const C3DMeshFileXBoundingBoxChunk> bounding_box_data;
    bounding_box_data.SetStaticCast(root->FindFirstSubChunkOfType(C3DMeshFileXBoundingBoxChunk::GetStaticChunkType()));
    if (bounding_box_data.Get()) {
        C3DVector center = bounding_box_data->GetCenter();
        C3DVector diag = bounding_box_data->GetDiagonal();
        BoundingBox.SetCenterDiag(center, diag);
    }
    return true;
}
