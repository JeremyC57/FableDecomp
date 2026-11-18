// CUserProfileManager::GetSortedSaveNamesAndTimes(std::vector<CWideString>& names, std::vector<CDateAndTime>& times) -- list the extensionless files in the profile directory (other than the profile file itself) into names, with each file's modified time into times
// Leaf: ?GetSortedSaveNamesAndTimes@CUserProfileManager@@QAEXAAV?$vector@VCWideString@@V?$allocator@VCWideString@@@std@@@std@@AAV?$vector@VCDateAndTime@@V?$allocator@VCDateAndTime@@@std@@@3@@Z
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004091C0, length 534 bytes, /GS cookie frame (0x250), `ret 8`.
// Name: Ego_r.pdb 0x00408280 / FableWin.pdb 0x0187D7D0 ?GetSortedSaveNamesAndTimes@CUserProfileManager@@QAEX..@Z (the header's SAVELIST
//   declaration; the landed callers 0x00495830 / 0x00597DF7 already bind it); Ego_d.pdb names the parameters names / times and the
//   locals search_path_name, path_name, scan_info, found_one and, in the loop block, file_name / extension_name. Was LoadFileList.
// Callees: vector<CWideString>::erase(first, last) 0x00412450, the CDateAndTime erase's __copy 0x0040E350 (__fastcall),
//   GetPathNameFromFileName 0x00408120, operator+(const CWideString&, const wchar_t*) 0x0099BF30, CAFile::StartFileScan / ScanNextFile
//   0x00999760 / 0x00999850, CAFile::SplitPath 0x00997620, CWideString::operator!= 0x0099B0F0, CAFile::GetFileModifiedTime 0x009992A0,
//   the vectors' _M_insert_overflow 0x00412330 (CWideString) / 0x004125A0 (CDateAndTime). Strings: L"" 0x0122D70C, L"*" 0x0122DE6C.
// Layout: CUserProfileManager ProfileName +0x08 (Ego_r.pdb); path_name 262 wide characters (0x20C bytes, retail [esp+4Ch]..[esp+258h]).
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CWIDESTRING_TEXT_EQ
#define FABLE_RETAIL_CWIDESTRING_TEXT_EQ_STLP_INLINE
#define FABLE_RETAIL_CWIDESTRING_NE
#define FABLE_RETAIL_STLP_CHAR_TRAITS_LENGTH
#define FABLE_RETAIL_STLP_CHAR_TRAITS_COMPARE_INLINE
#define FABLE_RETAIL_STLP_COPY_NOINLINE
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_CAFILE_SCANNEXTFILE
#define FABLE_RETAIL_CAFILE_SPLITPATH_WIDE
#define FABLE_RETAIL_CAFILE_MODIFIEDTIME
#define FABLE_RETAIL_CUSERPROFILEMANAGER_SAVENAMES
#define FABLE_RETAIL_CUSERPROFILEMANAGER_SAVELIST
#include "stlport/fable_string.h"   // first: the overlay copy carries FABLE_RETAIL_STLP_CHAR_TRAITS_COMPARE_INLINE (guarded, so fable_string.h reuses it)
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CDateAndTime.h"
#include "engine/CAFile.h"
#include "engine/CUserProfileManager.h"

void CUserProfileManager::GetSortedSaveNamesAndTimes(std::vector<CWideString>& names, std::vector<CDateAndTime>& times)
{
    names.clear();
    times.clear();
    CWideString search_path_name = GetPathNameFromFileName(L"", false) + L"*";
    wchar_t path_name[262];
    CAFile::CFileScanInfo scan_info;
    bool found_one = CAFile::StartFileScan(search_path_name, scan_info, path_name);
    while (found_one) {
        CWideString file_name;
        CWideString extension_name;
        CAFile::SplitPath(path_name, 0, 0, &file_name, &extension_name);
        if (extension_name == L"" && file_name != ProfileName) {
            names.push_back(file_name);
            times.push_back(CAFile::GetFileModifiedTime(path_name));
        }
        found_one = CAFile::ScanNextFile(search_path_name, scan_info, path_name);
    }
}
