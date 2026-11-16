// CSoundMap::TransferBinaryIn(CMemoryDataInputStream& stream) -- read the sound-key/range records and the criteria-key/def-string records back into both vector maps, re-sorting each one marked dirty
// Grade: RELOCATION_MATCH -- w24A: the one SIB operand order (`lea ecx,[eax+ecx+4]`, 0x004706C9: index * 8 first) comes from the release ASSERT of CArray<T>::operator[]: with its dead arm (fable_assert.h; FABLE_RETAIL_CARRAY_INDEX_ASSERT, engine/CArray.h) in the indexer the second `CriteriaMap[j]` access comes out index-first as retail, without it begin-first (graded both ways; cause not measured). No spelling of the element access itself moves it (Q9 and earlier: 11 inert probes).
// Leaf: ?TransferBinaryIn@CSoundMap@@QAEXAAVCMemoryDataInputStream@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0047042A, 716 bytes, thiscall `ret 4`, size-optimised def TU (`mov ebp,esp` frame).
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING_TRANSFER
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CVECTORMAP_SORT
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_CSOUNDMAP
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#define FABLE_RETAIL_CARRAY_INDEX_ASSERT
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_algorithm.h"
#include "engine/CMemoryDataInputStream.h"
#include "engine/CSoundMap.h"

void CSoundMap::TransferBinaryIn(CMemoryDataInputStream& stream)
{
    unsigned long count = stream.ReadULONG();
    SoundMap.resize(count, std::pair<unsigned long,CSoundPair>());
    for (unsigned long i = 0; i < SoundMap.size(); ++i) {
        SoundMap[i].first = stream.ReadULONG();
        SoundMap[i].second.FirstSound = stream.ReadSLONG();
        SoundMap[i].second.LastSound = stream.ReadSLONG();
    }
    SoundMap.Update();
    count = stream.ReadULONG();
    CriteriaMap.resize(count, std::pair<unsigned long,CDefString>());
    for (unsigned long j = 0; j < CriteriaMap.size(); ++j) {
        CriteriaMap[j].first = stream.ReadULONG();
        CriteriaMap[j].second.TransferBinaryIn(stream);
    }
    CriteriaMap.Update();
}