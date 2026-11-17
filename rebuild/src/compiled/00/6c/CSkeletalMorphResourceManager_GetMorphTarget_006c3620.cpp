// CSkeletalMorphResourceManager::GetMorphTarget(const CCharString& name) -- the cached morph target for a bone-config file: build its full path under Data\Bones\, add an empty cache entry on first use, and on first use of an empty entry create the target and load it from the file when it exists
// Leaf: ?GetMorphTarget@CSkeletalMorphResourceManager@@QAEAAV?$CCountedPointer@VC3DSkeletalMorphTarget@@@@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006C3620, `ret 4`, 0x3C-byte frame (the full path is built in the dead parameter slot).
// Callees: CCharString::ToWideString @ 0x0099E480, ?GetBoneConfigDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ @ 0x0041A290,
//   ??H@YI?AVCWideString@@ABV0@0@Z @ 0x0099BE70, ?ConvertPathToFullPath@CAFile@@SI?AVCWideString@@ABV2@@Z @ 0x00999110,
//   ?ToCharString@CWideString@@QBE?AVCCharString@@XZ @ 0x0099B2C0, ~CWideString @ 0x0099B510, the CachedMorphs map's find @ 0x006C31B0
//   and insert_unique(const value_type&) @ 0x006C3410, CCharString copy ctor @ 0x0099EC30 / dtor @ 0x0099EAE0, operator delete
//   @ 0x00BFE9BC / new @ 0x00BFEA1A, ??0C3DSkeletalMorphTarget@@QAE@XZ @ 0x00A44E60, CCountedPointer<C3DSkeletalMorphTarget>::Reset
//   @ 0x006C3AC0 (out of line), ?PathExists@CAFile@@SI_NABVCWideString@@@Z @ 0x00999230, CDiskFileWin32(const CWideString&,
//   FileAccessType, unsigned long) @ 0x0099AD80 and its inline destructor (vtable 0x0122D06C, Close 0x0099A920, ~CBaseClassNonCopyable
//   0x0099A300), ?GFLoadSkeletalMorphTargetFromFile@@YIXAAVCAFile@@AAVC3DSkeletalMorphTarget@@@Z @ 0x0088B0C0 (Ego_r.pdb spellings).
// Layout: engine/CSkeletalMorphResourceManager.h (Ego_r.pdb CachedMorphs map +0x04); C3DSkeletalMorphTarget sizeof 0x10.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDISKFILEWIN32
#define FABLE_RETAIL_CAFILE
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

CCountedPointer<C3DSkeletalMorphTarget>& CSkeletalMorphResourceManager::GetMorphTarget(const CCharString& name)
{
    CCharString filename = CAFile::ConvertPathToFullPath(NResourceDirectoryNames::GetBoneConfigDir() + name.ToWideString()).ToCharString();
    std::map<CCharString, CCountedPointer<C3DSkeletalMorphTarget> >::iterator it = CachedMorphs.find(filename);
    if (it == CachedMorphs.end())
    {
        CachedMorphs.insert(std::map<CCharString, CCountedPointer<C3DSkeletalMorphTarget> >::value_type(filename, CCountedPointer<C3DSkeletalMorphTarget>()));
        it = CachedMorphs.find(filename);
    }
    CCountedPointer<C3DSkeletalMorphTarget>& morph_target = it->second;
    if (morph_target.IsNull())
    {
        morph_target.Reset(new C3DSkeletalMorphTarget);
        if (CAFile::PathExists(filename.ToWideString()))
        {
            CDiskFileWin32 file(filename.ToWideString(), FILE_ACCESS_READ, 2);
            GFLoadSkeletalMorphTargetFromFile(file, *morph_target);
        }
    }
    return morph_target;
}
