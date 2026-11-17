// NUISystem::CFrontEndManager::GetProfileNames(std::vector<CWideString>& names) -- list the profiles: every sub-directory entry of the save directory (skipping "." / ".." and only while the search pattern is not a file) that holds *.bin files contributes its name once per file found
// Leaf: ?GetProfileNames@CFrontEndManager@NUISystem@@IAEXAAV?$vector@VCWideString@@V?$allocator@VCWideString@@@std@@@std@@@Z
// Naming wave 27 (family pb, DECISIONS_W26 D7): std::vector takes STLport's allocator argument, so push_back names
//   0x004128A0's STLport spelling and erase the bound STLport erase 0x00412450; GetProfileNames is protected (Ego_r.pdb /
//   FableWin.pdb IAE). Was the allocator-less vector<CWideString> (?push_back@?$vector@VCWideString@@@std@@..) and public.
// Flags: /GS
// Retail: Fable.exe @ 0x005955AB, 556 bytes, `ret 4`, /GS cookie (0x0139C8A8, __security_check_cookie 0x00BFE9F9) in a
// biased frame (`lea ebp,[esp-3D0h]`).
// Body: names.erase(begin, end) (0x00412450); pattern = NResourceDirectoryNames::GetSaveDir() (0x0041A540, __fastcall hidden
// result) + L"*" (operator+ 0x0099BF30); CAFile::StartFileScan / ScanNextFile (0x00999760 / 0x00999850, __fastcall) over pattern:
// fileName(data name) (0x0099B6B0); skip when fileName.ConvertToCharString() (0x0099B2C0) ends with "." (inline length
// difference + inline strcmp) or IsPathFile(pattern) (0x00999A30); else fileName += L"\\" (0x0099B940), SplitPath(fileName,
// 0, &dir, 0, 0) (0x00997620), files = CWideString(fileName) (copy 0x0099B720) + L"*.bin" and an inner find loop whose
// every hit pushes the OUTER entry's name (SplitPath name part) with the out-of-line push_back (0x004128A0).
// Names: Ego_r.pdb CFrontEndManager member; GetSaveDir / IsPathFile / SplitPath / Find* as the landed
// CUserProfileManager::DeleteProfile (0x00407040) and CFrontendGameComponent::GetWorldNames (0x00495890) models.
// Layout: find handle 8 bytes, find data 0x20C; CCharString data block { char* Text; long Length; }.
// Shape: each loop keeps its own `found` flag assigned by ScanNextFile inside the body (the loop locals die after the
// call, `mov bl,al` before the destructor).
// NF73 (2026-11-17): ported onto the shared headers (fable_string.h CWideString / CCharString EndsWith, CAFile.h, fable_resource_directories.h,
//   stlport/fable_containers.h, engine/CFrontEndManager.h BUILDCOMPONENTS) with the Ego_d.pdb local names; binds CAFile::IsPathFile
//   (was a free IsPathFile) and the PDB ?ToCharString@CWideString@@QBE?AVCCharString@@XZ (Ego_r 0x00B02850 / FableWin 0x02F67B80; was the
//   made-up ConvertToCharString).
// Opt: size TU (`#pragma optimize("s", on)`: the biased /GS frame).
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_ENDSWITH
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_CAFILE_SCANNEXTFILE
#define FABLE_RETAIL_CAFILE_SPLITPATH_WIDE
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#define FABLE_RETAIL_CFRONTENDMANAGER_DESTRUCTOR
#define FABLE_RETAIL_CFRONTENDMANAGER_BUILDCOMPONENTS
#define FABLE_RETAIL_STLP_MAP
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/CAFile.h"
#include "fable_resource_directories.h"
#include "engine/CFrontEndManager.h"

namespace NUISystem {
void CFrontEndManager::GetProfileNames(std::vector<CWideString>& vProfileNames)
{
    vProfileNames.clear();
    CWideString dir_search_path = NResourceDirectoryNames::GetSaveDir() + L"*";
    CAFile::CFileScanInfo scan_info;
    wchar_t pathname[262];
    bool found_one = CAFile::StartFileScan(dir_search_path, scan_info, pathname);
    while (found_one) {
        CWideString search_path(pathname);
        bool dots;
        {
            CCharString dot(".");
            dots = search_path.ToCharString().EndsWith(dot);
        }
        if (!dots && !CAFile::IsPathFile(dir_search_path)) {
            search_path += L"\\";
            CWideString search_directory_name;
            CAFile::SplitPath(search_path, 0, &search_directory_name, 0, 0);
            CWideString profile_search_path = CWideString(search_path) + L"*.bin";
            CAFile::CFileScanInfo profile_scan_info;
            wchar_t profile_pathname[262];
            bool found_one_profile = CAFile::StartFileScan(profile_search_path, profile_scan_info, profile_pathname);
            while (found_one_profile) {
                CWideString profileName;
                {
                    CWideString entry(pathname);
                    CAFile::SplitPath(entry, 0, 0, &profileName, 0);
                }
                vProfileNames.push_back(profileName);
                found_one_profile = CAFile::ScanNextFile(profile_search_path, profile_scan_info, profile_pathname);
            }
        }
        found_one = CAFile::ScanNextFile(dir_search_path, scan_info, pathname);
    }
}
}
