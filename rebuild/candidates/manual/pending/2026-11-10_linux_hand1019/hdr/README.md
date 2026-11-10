# Chunk 1019 header overlay (`FABLE_HDR_OVERLAY=$A/hdr`, exported for every grade, probe and c2prio run)

Every file is the committed `rebuild/include/<same path>` (HEAD, 2026-11-10) plus additions that sit entirely behind
`#ifdef FABLE_RETAIL_*` opt-ins; with none of them defined each header preprocesses to the committed text. The opt-ins are the
chunk-1017 ones (pending/2026-11-10_linux_hand1017/hdr, documented in its README), re-merged onto HEAD: HEAD has since gained
an `inline` FABLE_RETAIL_STLP_FOR_EACH_BODY (chunk 1018) and FABLE_RETAIL_STLP_VECTOR_RANGE_CTOR, so `stlport/fable_algorithm.h`
is NOT overlaid any more (0x008365F0 instantiates the committed inline body explicitly: `template ... std::for_each(...)` emits
the /Gr COMDAT) and `stlport/fable_containers.h` is HEAD + the INTROSORT block. No opt-in was added by chunk 1019.

| Header | Opt-in | What it adds | Row |
|---|---|---|---|
| `fable_definition_table.h` | `FABLE_RETAIL_CDEFSTRINGTABLE_ASCHARARRAY` | `const char* CDefStringTable::GetStringAsCharArray(long) const;` 0x009D49F0 (Ego_r / FableWin const spelling; NAME_FIXES) | 0x008365F0 |
| `engine/CDefString.h` | `FABLE_RETAIL_CDEFSTRING_GETASCHARARRAY` | inline `GetAsCharArray() const` = `GDefStringTable.GetStringAsCharArray(TablePos)` (Ego_r / FableWin ?GetAsCharArray@CDefString@@QBEPBDXZ) | 0x008365F0 |
| `engine/CMeshDataBank.h` | `FABLE_RETAIL_CMESHDATABANK_PEEKANIMATION` | fwd `class C3DAnimationInfo;` + `const C3DAnimationInfo* PeekAnimation(unsigned long) const;` 0x00A242C0 | 0x008365F0 |
| `stlport/fable_containers.h` | `FABLE_RETAIL_STLP_INTROSORT_LOOP` | STLport 4.6 `__stl_threshold`, `__median`, `__unguarded_partition` / `__partial_sort` declarations, `partial_sort` front end, `__introsort_loop` body (`FABLE_STLP_FASTCALL`); the one edited line is the `FABLE_STLP_NOINLINE` / `FABLE_STLP_FASTCALL` `#if`, which gains `|| defined(FABLE_RETAIL_STLP_INTROSORT_LOOP)` (default path unchanged) | 0x006A3E00 |
| `engine/C3DGroup2.h` (new, all under the opt-in) | `FABLE_RETAIL_C3DGROUP2` | `C3DGroup2 { long BoneIndex; CArray<C3DVertexBlend2> VertexBlends; }` (16 bytes), inline `GetBoneIndex() const`, `SetBoneIndex(long)` 0x00AC4070 | 0x00AE3920 |
| `engine/CArray.h` | `FABLE_RETAIL_CARRAY_PUSHBACK_NOINLINE` | visible but `__declspec(noinline)` (MSC only) `push_back(const T&)` (retail out of line 0x00AE83D0) | 0x00AE3920 |
| `engine/CParticleProgram.h` | `FABLE_RETAIL_CPARTICLEPROGRAM_REMAPBONEINDICES` | `#include "stlport/fable_map.h"` (under the ifdef) + private `static void __fastcall RemapBoneIndices(std::map<unsigned long,unsigned long>&, CArray<C3DGroup2>&);` 0x00AE3920 | 0x00AE3920 |

Committed opt-ins the matched rows also use (no overlay change): `FABLE_RETAIL_STLP_FOR_EACH_BODY` (0x008365F0),
`FABLE_RETAIL_STLP_MAP` (0x00AE3920: the visible map::operator[] body), `FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN`
with `FABLE_RETAIL_CTHING` and friends (0x006A3E00). The differ/ rows build on HEAD plus this overlay (none needs it except
through the headers above).
