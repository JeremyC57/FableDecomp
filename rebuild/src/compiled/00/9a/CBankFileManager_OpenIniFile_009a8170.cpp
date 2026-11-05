// CBankFileManager::OpenIniFile(const CWideString& pathname) -- read the bank ini file: `name = "dev path" "header"` lines register a CBankInfo, the BeginRetailFile / EndRetailFile / DevHeaderDirectory / RetailHeaderDirectory keywords set the running paths; the ini file's directory becomes the base path
// Leaf: ?OpenIniFile@CBankFileManager@@QAEXABVCWideString@@@Z
// Flags: none
// Retail: Fable.exe @ 0x009A8170, 1732 bytes, thiscall `ret 4`; Ego_r.pdb (0x00AF5C20) / FableWin.pdb
// ?OpenIniFile@CBankFileManager@@QAEXABVCWideString@@@Z. Replaces the byte-baked oracle.
// Body: Banks (map<CCharString,CBankInfo> +0x04) is cleared (STLport clear: _M_erase 0x009AA7C0 out of line), the ini file is
// loaded into a local CMemoryFile (inline ctor/dtor) and read line by line through a 16 KB CFileDataInputStream until IsEOF.
// Each line has its white space trimmed and is parsed through a CMemoryDataInputStream over it. A line with '=' is a bank:
// the '=' byte is skipped (inline 1-byte Read), then ReadQuotedString (0x009A77D0) reads the dev path and, if present and
// non-empty, a header name that is joined to the dev / retail header directories; Banks[name] becomes a CBankInfo of the dev
// path, the current retail file and the two header paths. Any other line starts with a keyword compared case-insensitively
// (CCharString::EqualsCaseInsensitive, the inline _stricmp against a CCharString temporary of each literal).
// Shape: the dev path is converted into a named CWideString before the CBankInfo temporary. Built inline as the first ctor
// argument, VC7.1 passes the ToWideString result in eax and lays out every later loop temporary differently (DIFFER
// 1719v1732, 248 lines; the 2026-10-23 local-model attempt stopped at 1708v1732 on the same slot residue).
// IsEOF must compare the fields in place: through the virtual GetPosition() it stays a vtable call.
// Layout: fable_retail_banks.h retail branch (FABLE_RETAIL_CBANKFILEMANAGER), engine/CMemoryFile.h retail branch
// (FABLE_RETAIL_CMEMORYFILE) and the nested opt-ins listed below.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_WHITESPACE
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_CAFILE_SPLITPATH_WIDE
#define FABLE_RETAIL_CMEMORYFILE
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM_TEXT
#define FABLE_RETAIL_CFILEDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM_CHARSTRING
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CBANKFILEMANAGER
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CAFile.h"
#include "engine/CMemoryFile.h"
#include "engine/CFileDataInputStream.h"
#include "engine/CMemoryDataInputStream.h"
#include "fable_retail_banks.h"

void CBankFileManager::OpenIniFile(const CWideString& pathname)
{
    Banks.clear();
    CMemoryFile file;
    file.Open(pathname, FILE_ACCESS_READ, 2);
    CWideString retail_file;
    CWideString dev_header_directory;
    CWideString retail_header_directory;
    CFileDataInputStream stream(file, 0x4000);
    while (!stream.IsEOF()) {
        CCharString line = stream.ReadLine();
        line.RemoveWhiteSpaceBeginEnd();
        CMemoryDataInputStream line_stream(line);
        bool found;
        CCharString bank_name = line_stream.ReadUntilCharacter('=', found);
        if (found) {
            char equals;
            line_stream.Read(&equals, 1);
            CCharString dev_path;
            if (ReadQuotedString(dev_path, line_stream)) {
                CCharString header_name;
                CWideString dev_header;
                CWideString retail_header;
                if (ReadQuotedString(header_name, line_stream) && header_name.GetLength() > 0) {
                    dev_header = dev_header_directory + header_name.ToWideString();
                    retail_header = retail_header_directory + header_name.ToWideString();
                }
                CWideString dev_bank_file_path = dev_path.ToWideString();
                Banks[bank_name] = CBankInfo(dev_bank_file_path, retail_file, dev_header, retail_header);
            }
        } else {
            line_stream.SetPosition(0);
            CCharString keyword = line_stream.ReadUntilWhiteSpace();
            if (keyword.EqualsCaseInsensitive("BeginRetailFile")) {
                CCharString value;
                ReadQuotedString(value, line_stream);
                retail_file = value.ToWideString();
            } else if (keyword.EqualsCaseInsensitive("EndRetailFile")) {
                retail_file = L"";
            } else if (keyword.EqualsCaseInsensitive("DevHeaderDirectory")) {
                CCharString value;
                ReadQuotedString(value, line_stream);
                dev_header_directory = value.ToWideString();
            } else if (keyword.EqualsCaseInsensitive("RetailHeaderDirectory")) {
                CCharString value;
                ReadQuotedString(value, line_stream);
                retail_header_directory = value.ToWideString();
            }
        }
    }
    CWideString ini_path = file.GetPathname();
    CWideString directory;
    CAFile::SplitPath(ini_path, 0, &directory, 0, 0);
    BasePath = directory;
}