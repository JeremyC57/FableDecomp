// std::vector::_M_insert_overflow(CMeshMorphSetParts* position, const CMeshMorphSetParts& x, const __false_type&, unsigned int fill_len, bool atend) -- STLport non-POD growth of a mesh morph set's part list (vector<CTCAppearanceMorphBase::CMeshMorphSet::CMeshMorphSetParts>): allocate old + max(old, n), copy the head, place the n copies, copy the tail, destroy and free the old block
// Leaf: ?_M_insert_overflow@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00728D10, length 216 bytes (+ int3 padding), thiscall, `ret 14h`.
// Callers: the same vector's _M_fill_insert 0x00728DF0 (0x00728EFE, the no-room arm; reached from resize 0x00728F20).
// Callees: malloc @ 0x00BFEA0E (inline allocate: `n != 0 ? malloc(n * 16) : 0`), __uninitialized_copy @ 0x00728B10
//   (landed, twice), __uninitialized_fill_n @ 0x00728A40 (landed), CArray<CEngineTextureMorph>::CArray(const CArray&)
//   @ 0x005BBDF1 (landed; the fill_len == 1 arm's inline element copy), __destroy_aux @ 0x00722130 (landed; _M_clear's
//   _Destroy of the old range), free @ 0x00BFEA14. The three helpers are __fastcall (first/n in ecx, last in edx).
// Label: catalog "std::vector_InsertAndGrow_Pair" is descriptive.
// Element (16 bytes, `sar eax,4`): {unsigned long key; CArray of 20-byte records} with the record {long, long, vector of
//   16-byte PODs} (the landed destroy 0x00722130 and copy 0x005BBDF1). That is Ego_r.pdb's CTCAppearanceMorphBase::
//   CMeshMorphSet::CMeshMorphSetParts {ulong MeshIndex; CEngineTextureMorphSet TextureMorphs;} with CEngineTextureMorphSet
//   {CArray<CEngineTextureMorph> Morphs;} and CEngineTextureMorph {long OriginalTextureIndex; long MorphID;
//   vector<CCompositeBlendDesc> Blends;} at the retail 12-byte vector (the PDB's 20 / 16 / 24 bytes); the bodies sit in
//   the CTCRandomAppearanceMorph (a CTCAppearanceMorphBase) neighbourhood.
// Shape: STLport 4.6 verbatim, as 0x00721060 of this chunk (max by const reference into the `push ecx` local, the tag
//   temporaries homed in the atend slot through same-TU stand-in bodies of the out-of-line helpers, _M_clear / _M_set
//   inline), here with a non-trivial _Destroy: the out-of-line __destroy_aux(first, last, __false_type) before the free.
#define FABLE_STLP_OUTOFLINE_UNINIT_COPY
#define FABLE_STLP_OUTOFLINE_UNINIT_FILL_N
#define FABLE_RETAIL_STLP_DESTROY_AUX_NOINLINE_FASTCALL
#define FABLE_RETAIL_STLP_VECTOR_COPY_DECL_ONLY
#define FABLE_RETAIL_CENGINETEXTUREMORPHSET
#define FABLE_RETAIL_CTCAPPEARANCEMORPHBASE
#define FABLE_RETAIL_CTCAPPEARANCEMORPHBASE_MESHMORPHSET
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CArray.h"
#include "engine/CEngineTextureMorph.h"
#include "engine/CTCAppearanceMorphBase.h"     // CTCAppearanceMorphBase::CMeshMorphSet::CMeshMorphSetParts (Ego_r.pdb; retail 16 bytes)

typedef CTCAppearanceMorphBase::CMeshMorphSet::CMeshMorphSetParts TMeshMorphSetParts;
template void std::vector<TMeshMorphSetParts>::_M_insert_overflow(TMeshMorphSetParts*, const TMeshMorphSetParts&,
    const std::__false_type&, unsigned int, bool);
