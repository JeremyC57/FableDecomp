// NThingManager::CEntityRuntimePersistence::ClearPersistenceDataForSectionInEntityScript(const CCharString& section, CEntityScriptInfo& info) -- cut the named section out of an entity's persistent script text (XXXSectionStart <name>; ... XXXSectionEnd;) and out of its zlib-compressed binary save data, then recompress
// Leaf: ?ClearPersistenceDataForSectionInEntityScript@CEntityRuntimePersistence@NThingManager@@AAEXABVCCharString@@AAVCEntityScriptInfo@12@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0051F480, 1270 bytes, thiscall (this unused), `ret 8`. Literals "" 0x0122D70E, "XXXSectionStart"
//   0x012459A4, ";" 0x0123542C, "XXXSectionEnd;" 0x01245994; CCompressorZlib vtable 0x01245974.
// Identity (two sources): Ego_r.pdb ?ClearPersistenceDataForSectionInEntityScript@CEntityRuntimePersistence@NThingManager@@AAEXABVCCharString@@
//   AAVCEntityScriptInfo@12@@Z (private; donor order AddPersistedDeadEntity, this, ClearPersistenceDataForSection); its only callers are
//   ClearPersistenceDataForSection 0x00520390 (0x005203C6 / 0x005203E6: `push &record ; push section ; mov ecx,this` for every Valid
//   record of the level and game script arrays, 24-byte CEntityScriptInfo records).
// Layout: CEntityScriptInfo (engine header) Script +0x00, SaveData (CArray<unsigned char>) +0x04, UncompressedSize +0x10.
// Callees: CStringParser(const CCharString&, error callback, const CCharString& filename, long) 0x00404720 (defaults 0, "", 0),
//   SkipPastString 0x009B9BA0, SkipWhitespace 0x009B9C60, ReadAsStringUntilString 0x009BA330, ReadAsStringUntilPastString 0x009BA3E0,
//   ~CBaseClass 0x0099A2E0 (the parser's implicit destructor is expanded: comment list nodes freed with free 0x00BFEA14);
//   CCharString(const char*, long) 0x0099EBF0 / ~ 0x0099EAE0, Compare 0x00411570 (inline operator==), GetSubString 0x0099EC70,
//   operator+(const CCharString&, const CCharString&) 0x0099F570, operator= 0x0099EFB0; vector<unsigned char>(n) 0x00514E20 (out of
//   line in this TU), CCompressorZlib: GetRequiredWorkspaceSize 0x00A3A7D0, CACompressor(long) 0x00A3AEA0, Initialise 0x00A3AA10,
//   InternalDecompress 0x00A3AB00, InternalCompress(const void*, unsigned long, CArray<unsigned char>&) 0x00A3ADD0, ~ 0x00A3ACC0;
//   CMemoryDataInputStream(const void*, unsigned long) 0x00994930, ReadWithSrcChunkOverflow 0x00993CA0 (inline ReadULONG / ReadSLONG),
//   ReadNullTerminatedString 0x00995D30, GetPosition (vtable +8, inside the inline ReadConstData), SetPositionAssumeWithinSourceChunk
//   0x00993C10, ~CDataStream 0x00993B10; memmove 0x00BFEAE6 (the inline vector::erase).
// Shape (measured, chunk 2003): shared headers only; vector<unsigned char>(n) out of line (FABLE_RETAIL_STLP_VECTOR_N_OUTOFLINE, as
//   retail calls 0x00514E20). The new entry count is a named local (`new_count = count - 1`): retail computes it into a fresh
//   register before the GetAsCArray select (0x0051F87E `lea ecx,[ebp-1]`); stored in one expression VC7.1 reuses count's register
//   (`dec ebp`). The unused per-entry CMemoryDataInputStream over the entry's bytes is retail's (0x0051F80A..0x0051F810).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_SUBSTRING
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CACOMPRESSOR
#define FABLE_RETAIL_CCOMPRESSORZLIB
#define FABLE_RETAIL_CCOMPRESSORZLIB_COMPRESS_ARRAY
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_VECTOR_N_OUTOFLINE
#define FABLE_RETAIL_CENTITYRUNTIMEPERSISTENCE_CLEARINSCRIPT
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CStringParser.h"
#include "engine/CArray.h"
#include "engine/CMemoryDataInputStream.h"
#include "engine/CCompressorZlib.h"
#include "engine/NThingManager_CEntityRuntimePersistence.h"

void NThingManager::CEntityRuntimePersistence::ClearPersistenceDataForSectionInEntityScript(const CCharString& section, CEntityScriptInfo& info)
{
    CStringParser parser(info.Script);
    while (parser.SkipPastString("XXXSectionStart")) {
        long section_start = parser.GetStringPos() - CCharString("XXXSectionStart").GetLength();
        parser.SkipWhitespace();
        CCharString name = parser.ReadAsStringUntilString(";");
        if (name == section) {
            parser.ReadAsStringUntilPastString("XXXSectionEnd;");
            info.Script = info.Script.GetSubString(0, section_start) + info.Script.GetSubString(parser.GetStringPos(), info.Script.GetLength());
            break;
        }
    }
    if (info.UncompressedSize > 0) {
        CArray<unsigned char> decompressed(info.UncompressedSize);
        CCompressorZlib compressor;
        compressor.InternalDecompress(info.SaveData.GetAsCArray(), info.SaveData.size(), decompressed.GetAsCArray(), decompressed.size());
        CMemoryDataInputStream stream(decompressed.GetAsCArray(), decompressed.size());
        unsigned long count = stream.ReadULONG();
        for (unsigned long i = 0; i < count; ++i) {
            unsigned long entry_start = stream.GetPosition();
            CCharString name = stream.ReadNullTerminatedString();
            long size = stream.ReadSLONG();
            CMemoryDataInputStream entry_stream(stream.ReadConstData(size), size);
            unsigned long entry_end = stream.GetPosition();
            if (name == section) {
                unsigned long new_count = count - 1;
                *(unsigned long*)decompressed.GetAsCArray() = new_count;
                decompressed.erase(decompressed.begin() + entry_start, decompressed.begin() + entry_end);
                info.UncompressedSize = decompressed.size();
                compressor.InternalCompress(decompressed.GetAsCArray(), decompressed.size(), info.SaveData);
                break;
            }
        }
    }
}