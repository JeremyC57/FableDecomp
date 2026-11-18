// CSkeletalMorphResourceManager::PreloadBoneConfigs() -- load every Data\Bones\*.bncfg morph target that is not already in the resource map
// Leaf: ?PreloadBoneConfigs@CSkeletalMorphResourceManager@@QAEXXZ
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C37D0, length 459 bytes (/GS cookie, no EH frame, `and esp,-8`).
// Callees: the L"Data\\Bones\\" fastcall getter 0x0041A290 (manifest "GetAutoSaveFileName", propagated),
// operator+(CWideString, const wchar16*) 0x0099BF30, CWideString(const wchar16*) 0x0099B6B0, ~CWideString 0x0099B510,
// CWideString::ToCharString 0x0099B2C0, ~CCharString 0x0099EAE0, CAFile::StartFileScan 0x00999760 / ScanNextFile
// 0x00999850 (__fastcall pattern, finder, name buffer), the map's find 0x006C31B0 / insert 0x006C3410,
// CDiskFileWin32(path, 1, 2) 0x0099AD80 (inline dtor: Close 0x0099A920, ~CBaseClassNonCopyable 0x0099A300), operator new
// 0x00BFEA1A, C3DSkeletalMorphTarget() 0x00A44E60, CCountedPointer::Reset 0x006C3AC0,
// GFLoadSkeletalMorphTargetFromFile 0x0088B0C0 (__fastcall).
// Layout: same map model as the landed GetMorphTarget 0x006C3620 (map at +0x04, node key +0x10, value +0x14).
// Shape: the /GS cookie sits above a 264-wchar16 name buffer; each found file is wrapped in a CWideString, its
// CCharString key is rebuilt for every find/insert/find; a missing key is inserted, re-found, and loaded through
// a scoped CDiskFileWin32 into a freshly reset target.
// NF82 (q11008): moved off the row-local CWideString (with the made-up ConvertToCharString) / CCharString / CAFile / CDiskFileWin32 /
//   C3DSkeletalMorphTarget / counted-pointer / map / manager models onto the shared headers, with the preamble of the landed sibling
//   GetMorphTarget 0x006C3620 (same TU): fable_string.h (CWideString::ToCharString, Ego_r.pdb ?ToCharString@CWideString@@QBE?AVCCharString@@XZ),
//   fable_resource_directories.h (NResourceDirectoryNames::GetBoneConfigDir 0x0041A290), engine/CAFile.h (CFileScanInfo, StartFileScan,
//   ScanNextFile), engine/CDiskFileWin32.h, engine/C3DSkeletalMorph.h, engine/CCountedPointer.h (Reset out of line, 0x006C3AC0) and
//   engine/CSkeletalMorphResourceManager.h (CachedMorphs map<CCharString, CCountedPointer<C3DSkeletalMorphTarget> > +0x04).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDISKFILEWIN32
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_CAFILE_SCANNEXTFILE
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER_RESET_NOINLINE
#define FABLE_RETAIL_CSKELETALMORPHRESOURCEMANAGER_GETMORPHTARGET
#define FABLE_RETAIL_C3DSKELETALMORPHTARGET_LAYOUT
#define FABLE_RETAIL_NRESOURCEDIRECTORYNAMES_BONECONFIGDIR
#include "fable_string.h"
#include "fable_resource_directories.h"
#include "engine/CCountedPointer.h"
#include "engine/CAFile.h"
#include "engine/CDiskFileWin32.h"
#include "engine/C3DSkeletalMorph.h"
#include "engine/CSkeletalMorphResourceManager.h"

void __fastcall GFLoadSkeletalMorphTargetFromFile(CAFile& file, C3DSkeletalMorphTarget& target);   // 0x0088B0C0

void CSkeletalMorphResourceManager::PreloadBoneConfigs()
{
    CWideString pattern = NResourceDirectoryNames::GetBoneConfigDir() + L"*.bncfg";
    CAFile::CFileScanInfo finder;
    wchar_t fileName[264];
    if (CAFile::StartFileScan(pattern, finder, fileName)) {
        do {
            CWideString name(fileName);
            std::map<CCharString, CCountedPointer<C3DSkeletalMorphTarget> >::iterator it = CachedMorphs.find(name.ToCharString());
            if (it == CachedMorphs.end()) {
                CachedMorphs.insert(std::map<CCharString, CCountedPointer<C3DSkeletalMorphTarget> >::value_type(name.ToCharString(), CCountedPointer<C3DSkeletalMorphTarget>()));
                it = CachedMorphs.find(name.ToCharString());
                CDiskFileWin32 file(name, FILE_ACCESS_READ, 2);
                it->second.Reset(new C3DSkeletalMorphTarget);
                GFLoadSkeletalMorphTargetFromFile(file, *it->second);
            }
        } while (CAFile::ScanNextFile(pattern, finder, fileName));
    }
}
