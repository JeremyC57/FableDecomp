// CFileInstaller::IsCompressedFilename(const CWideString&, CWideString&) const -- replace a recognized compressed extension: split the name, look its lower-case extension up in CompressedFileTypes and output stem + mapped extension (true), else stem + extension (false)
// Leaf: ?IsCompressedFilename@CFileInstaller@@IBE_NABVCWideString@@AAV2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009C46B0, 240 bytes. Name: Ego_r.pdb 0x00B13080 ?IsCompressedFilename@CFileInstaller@@IBE_NABVCWideString@@AAV2@@Z
//   (protected const), as engine/CFileInstaller.h declares it.
// Calls: CAFile::SplitPath(const CWideString&, ...) 0x00997620, wide ctors 0x0099AED0 / dtors 0x0099B510, CWideString::ToCharString
//   0x0099B2C0, CCharString::GetAsLowerCase 0x0099ED10, the map<CCharString,CCharString> tree's _M_find 0x009AB5D0 (const find),
//   ~CCharString 0x0099EAE0, CCharString::ToWideString 0x0099E480, operator+(const CWideString&, const CWideString&) 0x0099BE70,
//   CWideString::operator= 0x0099B7D0.
// Layout: engine/CFileInstaller.h CompressedFileTypes (std::map<CCharString,CCharString>) at retail +0x94.
// NF41 (2026-11-16): was a renaming-macro row (CCharStringCore / CWideStringCore) with local CExtensionMap / CAFile / CFileInstaller
//   models (public non-const); now the shared engine/CFileInstaller.h + engine/CAFile.h (SPLITPATH_WIDE) + fable_string.h under the
//   include chain of the landed CFileInstaller::AddFile 0x009C47A0.
#define FABLE_RETAIL_CCHARSTRING_GETASLOWERCASE
#define FABLE_RETAIL_CAFILE_SPLITPATH_WIDE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_GETASUPPERCASE
#define FABLE_RETAIL_CCHARSTRING_SUBSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CWIDESTRING_TEXT_EQ
#define FABLE_RETAIL_CWIDESTRING_TEXT_EQ_STLP_INLINE
#define FABLE_RETAIL_CWIDESTRING_FORCEINLINE_EQ
#define FABLE_RETAIL_CWIDESTRING_FORCEINLINE_DATA_TEXT_EQ
#define FABLE_RETAIL_STLP_CHAR_TRAITS_LENGTH
#define FABLE_RETAIL_CCOMPRESSORZLIB
#define FABLE_RETAIL_CACOMPRESSOR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_SET
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_CONSTFIND
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_CAFILE_INSTALLERPATHS
#define FABLE_RETAIL_CDISKFILEWIN32
#define FABLE_RETAIL_CDISKFILEWIN32_DEFAULTCTOR
#define FABLE_RETAIL_CCRITICALSECTIONLOCK
#define FABLE_RETAIL_CFILEINSTALLER_CFILEINFO_CTOR
#include "engine/CFileInstaller.h"
#include "engine/CAFile.h"
#include "engine/CDiskFileWin32.h"
#include "engine/CCriticalSectionLock.h"

bool CFileInstaller::IsCompressedFilename(const CWideString& filename, CWideString& output) const
{
    bool result;
    {
        CWideString stem, extension;
        CAFile::SplitPath(filename, 0, 0, &stem, &extension);
        std::map<CCharString, CCharString>::const_iterator found = CompressedFileTypes.find(extension.ToCharString().GetAsLowerCase());
        if (found != CompressedFileTypes.end()) {
            output = stem + found->second.ToWideString();
            result = true;
        } else {
            output = stem + extension;
            result = false;
        }
    }
    return result;
}
