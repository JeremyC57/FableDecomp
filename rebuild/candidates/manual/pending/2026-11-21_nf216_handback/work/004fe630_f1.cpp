// CWorldMap::GetNavigationSectionOffsetInFile(CAFile& file, const set<CCharString>& sections) const -- read a level file's navigation section table (count, then name / offset records) and return the first section named in `sections`, else the last "NULL" / unnamed section, else ("", -1)
// Leaf: ?GetNavigationSectionOffsetInFile@CWorldMap@@IBE?AU?$pair@VCCharString@@J@std@@AAVCAFile@@ABV?$set@VCCharString@@U?$less@VCCharString@@@std@@V?$allocator@VCCharString@@@3@@3@@Z
// Flags: /O2 /Oy /W3
// Retail: Fable.exe (TLC, VC7.1) @ 0x004FE630, 440 bytes; Ego_r.pdb 0x004D24E0 / FableWin.pdb 0x01C8D060 (the Leaf above, protected
//   const); the one caller is CWorldMap::CreateNavMap 0x00500230 (landed, binds the same spelling).
// Callees: CAFile::ReadVar<int> 0x00516180 (out of line, engine/CAFile.h FABLE_RETAIL_CAFILE_READVAR_DECL), CCharString() 0x0099E4B0,
//   CCharString(const char*, long) 0x0099EBF0, CCharString(const CCharString&) 0x0099EC30, CCharString::operator= 0x0099EFB0,
//   ~CCharString 0x0099EAE0, CCharString::LoadFromFile 0x0099F2E0, CBasicString<char>::operator==(const char*) 0x004115A0,
//   set<CCharString>::_M_find 0x004B9370. Strings: "" 0x0122D70E, "NULL" 0x012393E4.
// Name fix (NF216, 2026-11-21, q12378): ported onto fable_string.h / engine/CAFile.h / engine/CWorldMap.h / stlport set (the NF62
//   attempt of 2026-11-17, DIFFER(461v440) because the header == reloaded PStringData after each call, now with the visible, called
//   CBasicString<char>::operator==(const char*) body: FABLE_RETAIL_CBASICSTRING_TEXT_EQ_NOINLINE_COMPARE_ZERO + COMPARE_BODY). The
//   X_004fe630 stand-ins (CBasicString / CCharStringData / CAFile / set / pair / CWorldMapMethods, the global-scope CCharString model)
//   are gone: 0x004115A0 binds ??8?$CBasicString@D@@QBE_NPBD@Z (was ??8?$CBasicString@D@X_004fe630@@QBE_NPBD@Z).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CBASICSTRING_COMPARE_BODY
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_NOINLINE_COMPARE_ZERO
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_STLP_TREE_MFIND_NOINLINE
#define FABLE_RETAIL_STLP_TREE_CONSTFIND_TEMPLATE_BODY
#define FABLE_RETAIL_STLP_SET
#define FABLE_RETAIL_STLP_SET_FIND
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_CAFILE_READVAR_DECL
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_NAVIGATIONSECTION
#include "fable_compiler.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/C2DBoxI.h"
#include "engine/CAFile.h"
#include "engine/CWorldMap.h"

std::pair<CCharString,long> CWorldMap::GetNavigationSectionOffsetInFile(CAFile& file, const std::set<CCharString, std::less<CCharString>, std::allocator<CCharString> >& sections) const
{
    std::pair<CCharString,long> fallback("", -1);
    std::pair<CCharString,long> found("", -1);
    int count;
    file.ReadVar(count, 0);
    for (int i = 0; i < count; ++i) {
        int length;
        file.ReadVar(length, 0);
        CCharString name;
        name.LoadFromFile(file, 0, length);
        int index;
        file.ReadVar(index, 0);
        if (!(name == "NULL" || name == "")) {
            if (sections.find(name) != sections.end()) {
                found.first = name;
                found.second = index;
                break;
            }
        } else {
            fallback.first = name;
            fallback.second = index;
        }
    }
    if (found.second == -1) {
        found.first = fallback.first;
        found.second = fallback.second;
    }
    return std::pair<CCharString,long>(found.first, found.second);
}
