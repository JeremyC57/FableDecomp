// CFileInstaller::CFileInfo::CFileInfo(const CWideString&, const CWideString&, long, bool, bool) -- copy both paths, store InputSize,
// Started = false, Decompress and CreateMarkerFile
// Leaf: ??0CFileInfo@CFileInstaller@@QAE@ABVCWideString@@0J_N1@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009C3130, `ret 14h`. Callees: CWideString copy constructor 0x0099B720 (twice).
// NF39 (2026-11-16): moved off its address-suffixed stand-in (CFileInstallerEntry_009c3130) onto the nested class of the shared
//   engine/CFileInstaller.h (whose _CFILEINFO_CTOR comment names this COMDAT 0x009C3130; without that opt-in the header declares the ctor and
//   this row defines it with the header's body): FableWin.pdb 0x03046BC0 ??0CFileInfo@CFileInstaller@@QAE@ABVCWideString@@0J_N1@Z; AddFile
//   0x009C47A0 calls it after operator new(0x10) at its two single-file sites and binds this symbol.
#include "engine/CFileInstaller.h"

CFileInstaller::CFileInfo::CFileInfo(const CWideString& src_path, const CWideString& dst_path, long input_size, bool decompress, bool create_marker_file)
    : SrcPath(src_path), DstPath(dst_path), InputSize(input_size), Started(false), Decompress(decompress), CreateMarkerFile(create_marker_file)
{
}
