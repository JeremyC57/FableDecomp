// CLipSyncSampleData::ReadFromFile @0x00A495F0.
// Grade: MASKED_MATCH_PENDING_MAIN_REVIEW --1261 target bytes;original STLport library TU selects the native global helper.
// Retail: Fable.exe TLC @0x00A495F0..0x00A49ADD exclusive,1261 bytes,thiscall ret4.
// Leaf: ?ReadFromFile@CLipSyncSampleData@@QAEXAAVCDataInputStream@@@Z
// Flags: /Gr
// Inline-budget: real recursive tree erase stays called after CPP placement/accessor,8 budget,and scoped-control trials.
// Library: original STLport4.6.2 _Rb_global<bool>::_M_decrement is raw57 bytes at /Os;ordinary speed-TU64-byte copy retained.
#define FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_HINT_BODY
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_STLP_RANDOM_ACCESS_COPY_BACKWARD
#include "fable_string.h"
#include "engine/CDataInputStream.h"
#include "engine/CLipSyncSampleData.h"
#include "stlport/fable_map.h"

void CLipSyncSampleData::ReadFromFile(CDataInputStream& is)
{
    std::map<unsigned long,CCharString> phoneme_map;
    unsigned long count = is.ReadULONG();
    for (unsigned long i = 0; i < count; ++i) {
        unsigned long id = is.ReadUBYTE();
        CCharString symbol = is.ReadNullTerminatedString();
        phoneme_map[id] = symbol;
    }
    FramesPerSecond = (float)is.ReadULONG();
    FrameData.resize(is.ReadULONG());
    for (unsigned long c_frame = 0; c_frame < FrameData.size(); ++c_frame) {
        CLipSyncFrameData& frame = FrameData[c_frame];
        unsigned long n = is.ReadUBYTE();
        for (unsigned long k = 0; k < n; ++k) {
            CLipSyncPhoneme phoneme;
            phoneme.ID = is.ReadUBYTE();
            phoneme.Weight = (float)is.ReadUBYTE() * (1.0f / 255.0f);
            if (phoneme.Weight < 0.0f)
                phoneme.Weight = 0.0f;
            else if (phoneme.Weight > 1.0f)
                phoneme.Weight = 1.0f;
            phoneme.Symbol = phoneme_map[phoneme.ID];
            frame.Phonemes.push_back(phoneme);
        }
    }
}