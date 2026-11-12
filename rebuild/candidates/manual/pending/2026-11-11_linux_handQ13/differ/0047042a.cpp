// CSoundMap::TransferBinaryIn(CMemoryDataInputStream& stream) -- read the sound-key/range records and the criteria-key/def-string records back into both vector maps, re-sorting each one marked dirty
// Grade: DIFFER(ours v retail) -- DIFFER(716v716) measured 2026-11-11 with FABLE_HDR_OVERLAY=agentQ13/hdr (no overlay header used): one SIB operand order only: CriteriaMap[j].second.TransferBinaryIn `lea ecx,[eax+ecx+4]` (0x004706C9) v ours `[ecx+eax+4]`; the first element store `[eax+ecx]` already matches. Not re-probed this pass (the prior draft lists six inert spellings).
// Q13 (3 grades): named key local inert; begin() pointer local for the first store worse (726v716).
// Earlier grade (prior draft): DIFFER(716v716) -- on shared headers (2 grades): one SIB operand order only, the CDefString element address `lea ecx,[eax+ecx+4]` in retail v `[ecx+eax+4]` here (CriteriaMap[j].second.TransferBinaryIn). Inert probes: CArray-style begin()+j, (begin()+j)->, unsigned int / long j, a reused index, a reference local, a named CDefString&.
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
