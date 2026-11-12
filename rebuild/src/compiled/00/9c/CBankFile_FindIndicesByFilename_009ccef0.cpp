// CBankFile::FindIndicesByFilename(path, type, excludeUnassigned) const -- every entry index whose lower-cased base filename
//   equals path's (the FilenameIndices multimap equal range), optionally restricted to one entry data type (-1 = any); with
//   excludeUnassigned set, entries whose symbol has no '[' and does not start with "UNASSIGNED" are left out.
// Retail: Fable.exe (TLC, VC7.1) @ 0x009CCEF0, 592 bytes, ret 0x10 (hidden CArray<unsigned long> result); Ego_r.pdb / FableWin.pdb
//   ?FindIndicesByFilename@CBankFile@@QBE?AV?$CArray@K@@ABVCCharString@@J_N@Z.
// Callees: CBankFile::ConvertPathToFilename 0x009CBE10, CCharString::GetAsLowerCase 0x0099ED10 / FindCharPos 0x0099E5A0 /
//   copy ctor 0x0099EC30 / (const char*, long) 0x0099EBF0 / dtor 0x0099EAE0, multimap tree _M_upper_bound 0x009D2B50 and
//   _M_lower_bound 0x0042AD34 (STLport equal_range = pair(lower_bound, upper_bound), args evaluated right to left),
//   CPackedUIntArray::operator[] 0x00A629F0 (inline GetEntryDataType 0x009CC210), inline GetEntrySymbol 0x009CC1D0 ("(Unknown)"),
//   inline StartsWith -> strncmp 0x00BFEAF8 ("UNASSIGNED", a conditional temporary inside the &&), vector<unsigned long>
//   _M_insert_overflow 0x00464ACE (inline push_back), malloc 0x00BFEA0E / memmove 0x00BFEAE6 / free 0x00BFEA14 (inline copy
//   of the local array into the result, no NRVO).
// Leaf: ?FindIndicesByFilename@CBankFile@@QBE?AV?$CArray@K@@ABVCCharString@@J_N@Z
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_SUBSTRING
#define FABLE_RETAIL_CCHARSTRING_STARTSWITH
#define FABLE_RETAIL_CCHARSTRING_GETASLOWERCASE
#define FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE
#define FABLE_RETAIL_STLP_MULTIMAP_EQUAL_RANGE
#define FABLE_RETAIL_CBANKFILE_FILENAMEINDICES
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "fable_string.h"
#include "engine/CArray.h"
#include "engine/CPackedUIntArray.h"
#include "engine/CBankFile.h"

CArray<unsigned long> CBankFile::FindIndicesByFilename(const CCharString& path, long type, bool excludeUnassigned) const
{
    CArray<unsigned long> indices;
    CCharString name = ConvertPathToFilename(path).GetAsLowerCase();
    std::pair<std::multimap<CCharString, unsigned long>::const_iterator, std::multimap<CCharString, unsigned long>::const_iterator> range =
        FilenameIndices.equal_range(name);
    for (std::multimap<CCharString, unsigned long>::const_iterator it = range.first; it != range.second; ++it) {
        unsigned long index = (*it).second;
        if (type == -1 || GetEntryDataType(index) == (unsigned long)type) {
            CCharString symbol = GetEntrySymbol(index);
            bool skip = excludeUnassigned && symbol.FindCharPos('[') == -1 && !symbol.StartsWith("UNASSIGNED");
            if (!skip)
                indices.push_back(index);
        }
    }
    return indices;
}