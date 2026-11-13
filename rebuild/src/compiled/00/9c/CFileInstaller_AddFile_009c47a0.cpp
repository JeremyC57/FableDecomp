// CFileInstaller::AddFile(const CCharString& source, const CCharString& destination, const CArray<CCharString>* filters, bool create_marker_file) -- queue a file (or every match of a wildcard source) for installation, skipping the filtered names
// Leaf: ?AddFile@CFileInstaller@@QAEXABVCCharString@@0PBV?$CArray@VCCharString@@@@_N@Z
// Inline-budget: header opt-ins FABLE_RETAIL_CWIDESTRING_FORCEINLINE_EQ + FABLE_RETAIL_CWIDESTRING_FORCEINLINE_DATA_TEXT_EQ (FABLE_FORCEINLINE on CWideString::operator==(const unsigned short*) and CWideStringData::operator==(const unsigned short*)) -- the `filename == L""` site (0x009C4B98..0x009C4CBF) at c2 static frequency 1.0: plain inline expands the outer body (score 27) but calls the inner CWideStringData one (score 78 > T 74) where retail expands all three levels (STLport text == 68 included); no /d2inlT<N> fits because the same frequencies must keep push_back (77) and the CFileInfo ctor (76) called and ~CCountedPointer (67) expanded at the two single-file sites (c2 inline trace, tools/inl). Same opt-ins as rows 0x00491AD2 / 0x0041E6D3.
// Retail: Fable.exe (TLC, VC7.1) @ 0x009C47A0, 1915 bytes, thiscall `ret 10h`, `and esp,-8` frame. Name: Ego_r.pdb (0x00B13370) and
//   FableWin.pdb ?AddFile@CFileInstaller@@QAEXABVCCharString@@0PBV?$CArray@VCCharString@@@@_N@Z (public); catalog label
//   "AddFilteredFiles" is descriptive. The 0x10 argument bytes are the four PDB parameters; FilesToCopy +0x38, TotalSizeOfAllFiles
//   +0x70, Completed +0x80, Event +0x84, CriticalSection +0x04 are the shared engine/CFileInstaller.h members.
// Shape: under the installer's lock the source and destination become full wide paths. Each filter is upper-cased; a wildcard
//   filter is expanded by a file scan, the others made full paths, and all of them go into a local set<CCharString>. A wildcard
//   source scans the matches (destination = a directory) and queues every match not in the set; a plain source goes to the
//   destination's directory (empty file name) or to the destination itself. Each queued file is opened to add its length to
//   TotalSizeOfAllFiles and pushed as CCountedPointer<const CFileInfo>(new CFileInfo(...)). A non-empty queue clears Completed;
//   the worker is woken with SetEvent(Event).
// Inline split (measured with the c2 inline trace): inside the scan loop (frequency 2.5) retail expands the CFileInfo ctor, the
//   CCountedPointer(T*) ctor and list::push_back; at the two single-file sites (0.25) it calls their COMDATs 0x009C3130 / 0x009C6A70
//   / 0x009C6BF0. That needs the filter loop entered at 0.25 (filters non-null AND non-empty); the emptiness test reads the
//   iterator it then walks, so the iterator is taken first (testing filters->empty() first keeps a zero in esi across the guard
//   and colours the rest of the function differently).
// Callees: CCriticalSectionLock(_RTL_CRITICAL_SECTION&, bool) 0x00A62920 / ~ 0x00A629A0; CWideString() 0x0099AED0 / (const
//   CWideString&) 0x0099B720 / ~ 0x0099B510; CCharString::ToWideString 0x0099E480, GetAsUpperCase 0x0099EE20, FindCharPos(char)
//   0x0099E5A0, ~CCharString 0x0099EAE0; CWideString::ToCharString 0x0099B2C0, FindCharPos(unsigned short) 0x0099B070; operator+
//   (const CWideString&, const CWideString&) 0x0099BE70; CAFile::ConvertPathToFullPath 0x00999140 (wide, bool) / 0x009990B0
//   (narrow), GetFilenameFromPathname 0x00997930, StartFileScan 0x00999C50, ScanNextFile 0x00999CB0; set<CCharString>
//   insert_unique 0x004AC380, _M_find 0x004B9370, _M_erase 0x004ABA70; CBaseClassNonCopyable() 0x0099A2F0 / ~ 0x0099A300,
//   CDiskFileWin32::Open 0x0099A6A0 / Close 0x0099A920 (vtable 0x0122D06C); IsCompressedFilename 0x009C46B0; CFileInfo ctor
//   0x009C3130; CCountedPointer<const CFileInfo>(const CFileInfo*) 0x009C6A70 (deleter DeleteData 0x009C6640); list::push_back
//   0x009C6BF0; __char_traits_base<unsigned short,unsigned short>::compare 0x00403C60; wcscmp 0x00BFE94C (L"" 0x0122D70C);
//   malloc 0x00BFEA0E, free 0x00BFEA14, operator new 0x00BFEA1A, operator delete 0x00BFE9BC; SetEvent (IAT 0x0143FDE0).
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
extern "C" __declspec(dllimport) int __stdcall SetEvent(void* event);   // IAT 0x0143FDE0

void CFileInstaller::AddFile(const CCharString& source, const CCharString& destination, const CArray<CCharString>* filters,
                             bool create_marker_file)
{
    CCriticalSectionLock lock(CriticalSection, false);
    CWideString source_path;
    CAFile::ConvertPathToFullPath(source.ToWideString(), source_path);
    CWideString destination_path;
    CAFile::ConvertPathToFullPath(destination.ToWideString(), destination_path);

    std::set<CCharString> filtered_files;
    CArray<CCharString>::const_iterator it;
    if (filters != NULL && (it = filters->begin()) != filters->end()) {
        for (; it != filters->end(); ++it) {
            CCharString filter = it->GetAsUpperCase();
            if (filter.FindCharPos('*') >= 0) {
                CWideString filename;
                CAFile::CFileScanInfo scan_info;
                if (CAFile::StartFileScan(filter.ToWideString(), scan_info, filename)) {
                    do {
                        filtered_files.insert(filename.ToCharString().GetAsUpperCase());
                    } while (CAFile::ScanNextFile(filter.ToWideString(), scan_info, filename));
                }
            } else {
                filtered_files.insert(CAFile::ConvertPathToFullPath(filter).GetAsUpperCase());
            }
        }
    }

    if (source_path.FindCharPos('*') >= 0) {
        // Wildcard source: the destination is a directory.
        CWideString filename;
        CWideString destination_directory(destination_path);
        CAFile::CFileScanInfo scan_info;
        if (CAFile::StartFileScan(source_path, scan_info, filename)) {
            do {
                if (filtered_files.find(filename.ToCharString().GetAsUpperCase()) == filtered_files.end()) {
                    CWideString output_name;
                    bool decompress = IsCompressedFilename(filename, output_name);
                    CWideString output_path = destination_directory + output_name;
                    CDiskFileWin32 file;
                    file.Open(filename, FILE_ACCESS_READ, 0);
                    long size = file.GetLength();
                    TotalSizeOfAllFiles += size;
                    FilesToCopy.push_back(CCountedPointer<const CFileInfo>(new CFileInfo(filename, output_path, size, decompress, create_marker_file)));
                }
            } while (CAFile::ScanNextFile(source_path, scan_info, filename));
        }
    } else {
        CWideString filename = CAFile::GetFilenameFromPathname(destination_path);
        if (filename == L"") {
            // The destination names a directory: keep the source's (decompressed) file name.
            CWideString output_name;
            bool decompress = IsCompressedFilename(source_path, output_name);
            CWideString output_path = destination_path + output_name;
            CDiskFileWin32 file;
            file.Open(source_path, FILE_ACCESS_READ, 0);
            long size = file.GetLength();
            TotalSizeOfAllFiles += size;
            if (filtered_files.find(source_path.ToCharString().GetAsUpperCase()) == filtered_files.end())
                FilesToCopy.push_back(CCountedPointer<const CFileInfo>(new CFileInfo(source_path, output_path, size, decompress, create_marker_file)));
        } else {
            // The destination is the full target path.
            CDiskFileWin32 file;
            file.Open(source_path, FILE_ACCESS_READ, 0);
            long size = file.GetLength();
            TotalSizeOfAllFiles += size;
            CWideString output_name;
            bool decompress = IsCompressedFilename(source_path, output_name);
            if (filtered_files.find(source_path.ToCharString().GetAsUpperCase()) == filtered_files.end())
                FilesToCopy.push_back(CCountedPointer<const CFileInfo>(new CFileInfo(source_path, destination_path, size, decompress, create_marker_file)));
        }
    }

    if (FilesToCopy.size() != 0)
        Completed = false;
    SetEvent(Event);
}