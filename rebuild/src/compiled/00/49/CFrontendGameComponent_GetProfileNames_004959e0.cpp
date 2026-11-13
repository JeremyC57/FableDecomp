// CFrontendGameComponent::GetProfileNames(std::vector<CWideString>& names) -- list the profile names for the front end: for every sub-directory of the save directory, open each *.bin profile file, read its text (a "FablePro" header: version, end offset, then the text; or a bare legacy text file), transfer "ProfileName" out of it and collect the names, then sort them
// Leaf: ?GetProfileNames@CFrontendGameComponent@@SIXAAV?$vector@VCWideString@@V?$allocator@VCWideString@@@std@@@std@@@Z
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x004959E0, 1522 bytes, static __fastcall (names in ecx), plain `ret`, /GS cookie in an 8-aligned
//   ebp frame (`and esp,0FFFFFFF8h`: the inline 64-bit read checks). Starts 16-aligned after int3 padding, ends `ret` at 0x00495FD1.
// Identity (two sources): Ego_r.pdb / FableWin.pdb ?GetProfileNames@CFrontendGameComponent@@SIXAAV?$vector@VCWideString@@V?$allocator@
//   VCWideString@@@std@@@std@@@Z (public static __fastcall: the vector arrives in ecx, nothing on the stack); callers 0x00496056 /
//   0x004961C3 in the same CFrontendGameComponent TU; the "ProfileName" transfer and the "FablePro" header are the profile file
//   format of CUserProfileManager::LoadGameState 0x0040BCA0.
// Callees: vector<CWideString>::erase 0x00412450 (clear, out of line), NResourceDirectoryNames::GetSaveDir 0x0041A540,
//   operator+(const CWideString&, const unsigned short*) 0x0099BF30, CAFile::StartFileScan 0x00999760 / ScanNextFile 0x00999850 /
//   IsPathFile 0x00999A30 / SplitPath (wide) 0x00997620, CWideString (const unsigned short*) 0x0099B6B0 / () 0x0099AED0 / copy
//   0x0099B720 / ~ 0x0099B510 / += 0x0099B940 / ToCharString 0x0099B2C0, CCharString::EndsWith (inline, strcmp expanded),
//   CDiskFileWin32(const CWideString&, FileAccessType, unsigned long) 0x0099AD80, its Read 0x0099A960 / SetPosition 0x0099ABF0 /
//   inline IsOpen and inline destructor (vtable 0x0122D06C, Close 0x0099A920, ~CBaseClassNonCopyable 0x0099A300),
//   CFileDataInputStream(CAFile&, 0x4000) 0x00994700 / ~ 0x00994780 with the inline ReadULONG (ReadWithSrcChunkOverflow 0x00993CA0),
//   CDataInputStream::ReadString 0x00996BE0, CCharString::LoadFromFile 0x0099F2E0, the inline CStringParser(const CCharString&, ...)
//   (Init 0x009B9530) and its implicit destructor, CPersistContext(CStringParser&, ...) 0x009BADD0, Transfer<CCharString> 0x004109A0
//   (through the inline two-argument Transfer + GetDefaultVal<CCharString>()), CCharString::ToWideString 0x0099E480, the expanded
//   push_back (copy 0x0099B720, _M_insert_overflow 0x00412330), sort<CWideString*, less<CWideString> > 0x004977D0 (__fastcall).
// Shape (chunk 2010): shared headers only (hdr/ overlay opt-ins CAFILE_SCANNEXTFILE, CDISKFILEWIN32_ISOPEN_INLINE; INLINE_CTOR from
//   chunk 2001). The "." test is a named bool of its own statement (its two string temporaries die before the IsPathFile test,
//   0x00495AFB..0x00495B0F); one `!a && !b` condition keeps conditional-destruction flags instead. The profile file is opened with
//   an implicit unsigned short* -> CWideString conversion (temporary built before the 1 / 2 pushes, its address re-taken with
//   `lea`); an explicit CWideString(...) argument pushes the constants first.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_ENDSWITH
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_CAFILE_SPLITPATH_WIDE
#define FABLE_RETAIL_CAFILE_SCANNEXTFILE
#define FABLE_RETAIL_CDISKFILEWIN32
#define FABLE_RETAIL_CDISKFILEWIN32_ISOPEN_INLINE
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM_READSTRING
#define FABLE_RETAIL_CFILEDATAINPUTSTREAM
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CSTRINGPARSER_INLINE_CTOR
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_NAMED_DEFAULTS
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "fable_resource_directories.h"
#include "engine/CAFile.h"
#include "engine/CDiskFileWin32.h"
#include "engine/CFileDataInputStream.h"
#include "engine/CStringParser.h"
#include "engine/CPersistContext.h"
#include <string.h>

class CFrontendGameComponent {
public:
    static void __fastcall GetProfileNames(std::vector<CWideString>& names);
};

template<class T> inline T FABLE_FASTCALL CPersistContext::GetDefaultVal() { T value = T(); return value; }

void CFrontendGameComponent::GetProfileNames(std::vector<CWideString>& names)
{
    names.clear();
    CWideString pattern = NResourceDirectoryNames::GetSaveDir() + (const unsigned short*)L"*";
    CAFile::CFileScanInfo handle;
    unsigned short fileName[262];
    bool found = CAFile::StartFileScan(pattern, handle, fileName);
    while (found) {
        CWideString name(fileName);
        bool dots = name.ToCharString().EndsWith(".");
        if (!dots && !CAFile::IsPathFile(pattern)) {
            name += (const unsigned short*)L"\\";
            CWideString dir;
            CAFile::SplitPath(name, 0, &dir, 0, 0);
            CWideString files = CWideString(name) + (const unsigned short*)L"*.bin";
            CAFile::CFileScanInfo fileHandle;
            unsigned short profileFileName[262];
            bool fileFound = CAFile::StartFileScan(files, fileHandle, profileFileName);
            while (fileFound) {
                CDiskFileWin32 file(profileFileName, FILE_ACCESS_READ, 2);
                if (file.IsOpen()) {
                    CCharString text;
                    char signature[8];
                    file.Read(signature, 8);
                    if (memcmp(signature, "FablePro", 8) == 0) {
                        CFileDataInputStream stream(file, 0x4000);
                        stream.ReadULONG();
                        unsigned long length = stream.ReadULONG();
                        text = stream.ReadString(length - stream.GetPosition());
                    } else {
                        file.SetPosition(0);
                        text.LoadFromFile(file, 0, -1);
                    }
                    CStringParser parser(text);
                    CPersistContext context(parser, 0, true, 0, 0, 0);
                    CCharString profileName;
                    context.Transfer("ProfileName", profileName);
                    names.push_back(profileName.ToWideString());
                }
                fileFound = CAFile::ScanNextFile(files, fileHandle, profileFileName);
            }
        }
        found = CAFile::ScanNextFile(pattern, handle, fileName);
    }
    std::sort(names.begin(), names.end(), std::less<CWideString>());
}