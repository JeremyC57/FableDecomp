// C3DMeshStats::InitialiseFromFileData(const CCharString& name, const C3DMeshFile& file, bool animated, CMovableResourceMemoryPool* pool)
// Grade: DIFFER(3059v3051) -- md=720: chunk 868 shared-header rebuild (r1 md 1511 -> r10; 14 grades, probes r7/r9/r13): every callee, every inline decision (vector<C3DGeneratorObject>::push_back and both std::__copy out of line, the pair push_backs / GetCRC = CCRC::Calc / ~C3DMeshVolume inline), `this` in ebp and the bone/transform/bounding blocks now match retail; residue is allocation: retail homes the zero constant in ebx (`mov byte [..],bl`, `cmp R,ebx`) where ours spells 0 / `test R,R` with the piece in esi, retail CSEs the dummy name address once (`lea esi,[eax+40h] ; mov [esp+38h],esi`) and reloads it where ours re-forms it, frame 0x174 v 0x170 and the slot order shift that follows, the bone load scheduled before the Transform `rep movsd`, dl/cl expression-temp rotation; probe quantity: the const-0 piece takes the first allowed callee-saved register in table order (esi in ours; retail's ebx means its esi was already coloured by the name-address temp), `this` prio -35..-60 v a named name reference -14/-18 (a named `const CCharString&` wins ebp over `this`: r7/r9/r11 md 830)
// Next idea: give c2 a reason to keep the dummy name address live across the whole loop body (one candidate with >= 4 references that is not a reference binding: e.g. a `const CCharString* nameP` or the chunk pointer itself) so it colours esi before the zero piece; then the frame 4 bytes (a spilled temp retail keeps on the stack); r13/r14 named root pointer: md 645/661 but frame +8 / wrong register, not the quantity
// Chunk 868 rebuild on the shared headers (coordinator round 2, priority 5): new overlay headers engine/C3DMeshStats.h, C3DMeshFile.h, C3DHelperPoint.h, C3DDummyObject.h, C3DGeneratorObject.h, C3DMeshVolume.h, CPreAllocVectorMap.h, CCRC.h and the FindFirst*/FindNext*/GetRoot/GetHelpers/getter opt-ins listed in hdr/README.md; best plain candidate r10 (r12 = identical bytes).
//   -- build a mesh's stats block from its file: name, animated flag and pool stored; from the root chunk's helpers, every helper
//   point (keyed by the CRC of its name, name collected) and every dummy object -- a dummy named "createmesh <mesh>" becomes a mesh
//   generator instead -- and every mesh volume (keyed by name CRC, the volume map sorted); the collected names packed, the buffers
//   allocated, the helper points and dummy objects copied into their pre-allocated maps and sorted, the packed names copied in; the
//   bounding sphere from the XBDS chunk and the bounding box (centre +- diagonal) from the XBDB chunk; returns true
// Leaf: ?InitialiseFromFileData@C3DMeshStats@@QAE_NABVCCharString@@ABVC3DMeshFile@@_NPAVCMovableResourceMemoryPool@@@Z
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
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_STARTSWITH
#define FABLE_RETAIL_CCHARSTRING_GETASLOWERCASE
#define FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE
#define FABLE_RETAIL_CCRC_CALC_INLINE
#define FABLE_RETAIL_STLP_COPY
#define FABLE_RETAIL_STLP_SORT_FASTCALL
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
#define FABLE_RETAIL_C3DMESHFILEXBOUNDINGSPHERECHUNK_GETTERS
#define FABLE_RETAIL_C3DMESHFILEXBOUNDINGBOXCHUNK
#define FABLE_RETAIL_C3DMESHFILEXBOUNDINGBOXCHUNK_GETTERS
#define FABLE_RETAIL_C3DBOUNDINGSPHERE_SET
#define FABLE_RETAIL_C3DBOUNDINGBOX
#define FABLE_RETAIL_C3DBOUNDINGBOX_SETCENTERDIAG
#define FABLE_RETAIL_CVECTORMAP_INSERT
#define FABLE_RETAIL_CVECTORMAP_CTOR
#define FABLE_RETAIL_CARRAY
#include "engine/C3DMeshStats.h"
#include "engine/C3DMeshFile.h"
#include "engine/C3DMeshFileRootChunk.h"
#include "engine/C3DMeshFileHelpersChunk.h"
#include "engine/C3DMeshFileHelperPointChunk.h"
#include "engine/C3DMeshFileDummyObjectChunk.h"
#include "engine/C3DMeshFileConvexVolumeChunk.h"
#include "engine/C3DMeshFileXBoundingSphereChunk.h"
#include "engine/C3DMeshFileXBoundingBoxChunk.h"

bool C3DMeshStats::InitialiseFromFileData(const CCharString& name, const C3DMeshFile& file, bool animated, CMovableResourceMemoryPool* pool)
{
    Name = name;
    AnimatedFlag = animated;
    MemoryPool = pool;
    CCountedPointer<const C3DMeshFileRootChunk> root = file.GetRoot();
    CVectorMap<unsigned long, C3DHelperPoint> helperPoints;
    CVectorMap<unsigned long, C3DDummyObject> dummyObjects;
    CVectorMap<unsigned long, C3DMeshVolume> meshVolumes;   // never used: retail destroys an empty pair<unsigned long, C3DMeshVolume> vector at 0x00A302C8 (__destroy_aux(0, 0)) between the name sets and dummyObjects
    std::set<CCharString> helperPointNames;
    std::set<CCharString> dummyObjectNames;
    if (root->HasHelpers()) {
        CCountedPointer<const C3DMeshFileHelpersChunk> helpers = root->GetHelpers();
        CCountedPointer<const C3DMeshFileHelperPointChunk> point = helpers->FindFirstPoint();
        while (!point.IsNull()) {
            C3DHelperPoint helperPoint(point->GetPosition(), point->GetBoneIndex());
            helperPoints.Insert(std::pair<unsigned long, C3DHelperPoint>(point->GetName().GetCRC(), helperPoint));
            helperPointNames.insert(point->GetName());
            point = helpers->FindNextPoint(point);
        }
        CCountedPointer<const C3DMeshFileDummyObjectChunk> dummy = helpers->FindFirstDummy();
        while (!dummy.IsNull()) {
            bool useLocalOrigin = dummy->GetUseLocalOrigin();
            C3DDummyObject object(dummy->GetTransform(), dummy->GetBoneIndex());
            CCharString createMeshPrefix("createmesh ");
            if (dummy->GetName().GetAsLowerCase().StartsWith(createMeshPrefix)) {
                MeshGenerators.push_back(C3DGeneratorObject(object, useLocalOrigin, CCharString(dummy->GetName().GetAsCharArray() + 11), 0));
            } else {
                dummyObjects.Insert(std::pair<unsigned long, C3DDummyObject>(dummy->GetName().GetCRC(), object));
                dummyObjectNames.insert(dummy->GetName());
            }
            dummy = helpers->FindNextDummy(dummy);
        }
        CCountedPointer<const C3DMeshFileConvexVolumeChunk> volume = helpers->FindFirstVolumeW();
        while (!volume.IsNull()) {
            C3DMeshVolume meshVolume(*volume, pool);
            MeshVolumes.Insert(std::pair<unsigned long, C3DMeshVolume>(volume->GetName().GetCRC(), meshVolume));
            volume = helpers->FindNextVolumeW(volume);
        }
        MeshVolumes.Update();
    }
    CArray<char> packedNames;
    PackNames(packedNames, helperPointNames, dummyObjectNames);
    AllocateBuffers(helperPoints.size(), dummyObjects.size(), packedNames.size());
    std::copy(helperPoints.begin(), helperPoints.end(), HelperPoints.begin());
    HelperPoints.Sort();
    std::copy(dummyObjects.begin(), dummyObjects.end(), DummyObjects.begin());
    DummyObjects.Sort();
    std::copy(packedNames.begin(), packedNames.end(), PackedNames);
    CCountedPointer<const C3DMeshFileXBoundingSphereChunk> sphere;
    sphere.SetStaticCast(root->FindFirstSubChunkOfType("XBDS"));
    if (!sphere.IsNull()) {
        BoundingSphere.Center = sphere->GetCenter();
        BoundingSphere.Radius = sphere->GetRadius();
    }
    CCountedPointer<const C3DMeshFileXBoundingBoxChunk> box;
    box.SetStaticCast(root->FindFirstSubChunkOfType("XBDB"));
    if (!box.IsNull()) {
        C3DVector center = box->GetCenter();
        C3DVector diagonal = box->GetDiagonal();
        BoundingBox.SetCenterDiag(center, diagonal);
    }
    return true;
}
