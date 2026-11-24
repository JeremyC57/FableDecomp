#ifndef FABLE_RETAIL_HEADER_CAFILE
#define FABLE_RETAIL_HEADER_CAFILE
#include "engine/CBaseClassNonCopyable.h"
#include "fable_string.h"
#ifdef FABLE_RETAIL_CAFILE_MODIFIEDTIME
#include "engine/CDateAndTime.h"
#endif
#ifdef FABLE_RETAIL_CAFILE
#include "engine/CArray.h"
#endif
class CCharString;
class CASuspendableProcess;
class CFileInformation;
enum FileAccessType { FILE_ACCESS_NULL=0, FILE_ACCESS_READ=1, FILE_ACCESS_READ_WRITE=2, FILE_ACCESS_READ_WRITE_CREATE=3, FILE_ACCESS_CREATE=4 };
// Original CAFile virtual order from Ego_r TPI and native CDiskFileWin32 vtable.
#ifdef FABLE_RETAIL_CAFILE_SETCALLBACKS
// Storage of the hard-disk error callback, 0x013BCA00: Ego_d.pdb ?m_harddiskErrorCallback@CAFile@@0P6IXPBD@ZA (a private static);
// spelled here as the landed rows spell the address (?DebugMessage@@3P6IXPBD@ZA, CDiskFileWin32::Write 0x0099AA90; NAME_FIXES).
extern void (__fastcall* DebugMessage)(const char*);                // 0x013BCA00
#endif
class CAFile : public CBaseClassNonCopyable {
public:
#ifdef FABLE_RETAIL_CAFILE_ERRORCALLBACK
    // Retail opt-in (Codex pass17): original static callback chain, donor02dd7c60..a0.
#ifdef _MSC_VER
    typedef void (__fastcall *ErrorCallbackType)();
    static void __fastcall DoErrorCallback() {if(GetErrorCallback())CallErrorCallback();}
protected:
    static ErrorCallbackType __fastcall GetErrorCallback() {return m_errorCallback;}
    static void __fastcall CallErrorCallback() {m_errorCallback();}
#else
    typedef void (*ErrorCallbackType)();
    static void DoErrorCallback() {if(GetErrorCallback())CallErrorCallback();}
protected:
    static ErrorCallbackType GetErrorCallback() {return m_errorCallback;}
    static void CallErrorCallback() {m_errorCallback();}
#endif
private:
    static ErrorCallbackType m_errorCallback;
public:
#endif
#ifdef FABLE_RETAIL_CAFILE_SETCALLBACKS
    // b24c4 opt-in (row 0x00402510 GFMain): FableWin.pdb ?SetErrorCallback@CAFile@@SIXP6IXXZ@Z /
    // ?SetHardDiskErrorCallback@CAFile@@SIXP6IXPBD@Z@Z, inline in retail (GFMain stores 0x013BC9FC / 0x013BCA00 in place at
    // 0x00402566 / 0x00402579). Needs FABLE_RETAIL_CAFILE_ERRORCALLBACK.
    static void __fastcall SetErrorCallback(ErrorCallbackType callback) { m_errorCallback = callback; }
    static void __fastcall SetHardDiskErrorCallback(void (__fastcall* callback)(const char*)) { DebugMessage = callback; }
#endif
#ifdef FABLE_RETAIL_CAFILE_CURRENTPATH
    // b24c4 opt-in (row 0x00402510 GFMain): Ego_r.pdb + FableWin.pdb ?SetCurrentPath@CAFile@@SIXABVCWideString@@@Z and
    // ?GetProjectPath@CAFile@@SI?AVCWideString@@XZ, out of line in retail (0x009974F0 / 0x00997510). Declarations only.
    static void __fastcall SetCurrentPath(const CWideString& path);   // 0x009974F0
    static CWideString __fastcall GetProjectPath();                   // 0x00997510
#endif
#ifdef FABLE_RETAIL_CAFILE_DTOR_DECL_ONLY
    // b24c4 opt-in (row 0x00402510 GFMain): FableWin.pdb ??1CAFile@@UAE@XZ declared, so a TU that destroys a
    // CDiskFileWin32 emits no header-implicit ~CAFile copy (a 5-byte `jmp ~CBaseClassNonCopyable`, NEQ to the folded retail body
    // 0x0099A300 `mov [ecx],vtable ; ret`; pick-any check, EXTRA_RULES 2026-11-20). Declaration only.
    virtual ~CAFile();
#endif
    static CCharString __fastcall GetRelativeFilePath(const CCharString&,const CCharString&);
    static void __fastcall SplitPath(const CCharString&,CCharString*,CCharString*,CCharString*,CCharString*);
    static bool __fastcall PathExists(const CWideString&);
    static bool __fastcall IsPathFile(const CWideString&);
#ifdef FABLE_RETAIL_CAFILE_CONVERTDIRECTORYANDFILENAMETOPATH
    // Retail opt-in (wfE7, row 0x00B3CB30): Ego_r.pdb / FableWin.pdb ?ConvertDirectoryAndFilenameToPath@CAFile@@SI?AVCWideString@@ABV2@0@Z
    // (retail 0x009979E0, landed; static __fastcall, hidden result). Declaration only.
    static CWideString __fastcall ConvertDirectoryAndFilenameToPath(const CWideString& directory, const CWideString& filename);   // 0x009979E0
#endif
#ifdef FABLE_RETAIL_CAFILE_COPYFILE
    // NF73 opt-in (row 0x00416953 CMainGameComponent::PostInit): Ego_r.pdb 0x00B009F0 / FableWin.pdb ?CopyFileW@CAFile@@SIXAAV1@0@Z (static
    // __fastcall, source in ecx, destination in edx), out of line in retail at 0x00998E70. Declaration only.
    static void __fastcall CopyFileW(CAFile& source, CAFile& destination);   // 0x00998E70
#endif
#ifdef FABLE_RETAIL_CAFILE_ISROOMFORFILE
    // Opt-in (wf19, row 0x0055D180; chunk 2052, row 0x0049F4C0): Ego_r.pdb / FableWin.pdb ?IsRoomForFile@CAFile@@SI_NJABVCWideString@@@Z (retail 0x00999AB0; the
    // byte count in ecx, the path in edx). Declaration only.
    static bool __fastcall IsRoomForFile(long size, const CWideString& path);   // 0x00999AB0
#endif
#ifdef FABLE_RETAIL_CAFILE_MODIFIEDTIME_WACKY
    // Nested opt-in (chunk 1013, row 0x00502620 CWorldMap::LoadLevel): Ego_r.pdb / FableWin.pdb
    // ?GetFileModifiedTimeAsSomeWackyWin32BigNumber@CAFile@@SI_KABVCWideString@@@Z (retail 0x00999320; the path in ecx, the stamp in edx:eax).
    static unsigned __int64 __fastcall GetFileModifiedTimeAsSomeWackyWin32BigNumber(const CWideString& filename);   // 0x00999320
#endif
#ifdef FABLE_RETAIL_CAFILE_MODIFIEDTIME
    // Nested opt-in (chunk 2013, row 0x00597DF7 CFrontEndManager::GetSavedGameNames): Ego_r.pdb / FableWin.pdb
    // ?GetFileModifiedTime@CAFile@@SI?AVCDateAndTime@@ABVCWideString@@@Z (retail 0x009992A0: the hidden result in ecx, the path in edx,
    // 0x00597EF5..0x00597EFE). Needs engine/CDateAndTime.h.
    static CDateAndTime __fastcall GetFileModifiedTime(const CWideString& filename);   // 0x009992A0
#endif
#ifdef FABLE_RETAIL_CAFILE_SPLITPATH_WIDE
    // Nested opt-in (chunk 775, row 0x009A8170 OpenIniFile): Ego_r.pdb ?SplitPath@CAFile@@SIXABVCWideString@@PAV2@111@Z, the
    // CWideString overload (0x00997620; path in ecx, drive in edx, directory / name / extension pushed).
    static void __fastcall SplitPath(const CWideString&,CWideString*,CWideString*,CWideString*,CWideString*);
#endif
#if defined(FABLE_RETAIL_CAFILE_MAKEPATH) && !(defined(FABLE_RETAIL_CAFILE) && defined(FABLE_RETAIL_CAFILE_CREATEDIRECTORY))
    // NF294 add-only opt-in (row 0x00997800 ReplacePathExtension): Ego_r.pdb / FableWin.pdb ?MakePath@CAFile@@SI?AVCWideString@@ABV2@000@Z
    // (0x00997780; hidden result in ecx), static __fastcall, outside the FABLE_RETAIL_CAFILE branch (that branch pulls engine/CArray.h and
    // with it a /DEFAULTLIB:"libcp" directive). Declaration only; when CREATEDIRECTORY's copy is also on, that one is used.
    static CWideString __fastcall MakePath(const CWideString& drive,const CWideString& dir,const CWideString& name,const CWideString& ext);   // 0x00997780
#endif
#ifdef FABLE_RETAIL_CAFILE
    // Movie row 0x006286F0: Ego_r.pdb + FableWin.pdb ?ConvertPathToFullPath@CAFile@@SI?AVCWideString@@ABV2@@Z (0x00999110; the
    // hidden return slot in ecx, the path in edx). The PDBs' CCharString overload (SI?AVCCharString@@ABV2@@Z) and the
    // bool (SI_NABVCWideString@@AAV2@@Z) overload are not used by a retail row of this branch.
    static CWideString __fastcall ConvertPathToFullPath(const CWideString&);
    // 0x00AE8D50: Ego_r / FableWin ?GetAbsoluteFilePath@CAFile@@SI?AVCCharString@@ABV2@0@Z (the CCharString overload).
    static CCharString __fastcall GetAbsoluteFilePath(const CCharString&,const CCharString&);
    // Chunk 734 (row 0x004A5E90): Ego_r.pdb CAFile::CFileScanInfo (8 bytes: Stopped +0, SearchHandle +4; no destructor) and
    // ?StartFileScan@CAFile@@SI_NABVCWideString@@AAVCFileScanInfo@1@PA_W@Z (0x00999760: pattern in ecx, info in edx, buffer pushed).
    class CFileScanInfo {
    public:
        bool Stopped;                                        // +0x00
        long SearchHandle;                                   // +0x04
    };
    static bool __fastcall StartFileScan(const CWideString& pattern,CFileScanInfo& info,wchar_t* firstFile);   // 0x00999760
#ifdef FABLE_RETAIL_CAFILE_SCANNEXTFILE
    // Nested opt-in (chunk 2010, row 0x004959E0 CFrontendGameComponent::GetProfileNames): Ego_r.pdb / FableWin.pdb
    // ?ScanNextFile@CAFile@@SI_NABVCWideString@@AAVCFileScanInfo@1@PA_W@Z (retail 0x00999850; pattern in ecx, info in edx, the name buffer
    // pushed, as StartFileScan; bound as ...PAG@Z by the landed 0x005955AB: VC7.1 without /Zc:wchar_t spells the buffer unsigned short).
    static bool __fastcall ScanNextFile(const CWideString& pattern,CFileScanInfo& info,unsigned short* nextFile);   // 0x00999850
#endif
#ifdef FABLE_RETAIL_CAFILE_EXPANDPATH
    // NF41 opt-in (2026-11-16, row 0x00999850 ScanNextFile): Ego_r.pdb 0x00AFFC70 ?GetCurrentPath@CAFile@@SI?AVCWideString@@XZ (retail
    // 0x00997F80) and 0x00B00820 ?ExpandPath@CAFile@@SI?AVCWideString@@ABV2@0@Z (retail 0x00998C90), static __fastcall with the hidden
    // result in ecx. Declarations only.
    static CWideString __fastcall GetCurrentPath();                                                     // 0x00997F80
    static CWideString __fastcall ExpandPath(const CWideString& path,const CWideString& base);          // 0x00998C90
#endif
#ifdef FABLE_RETAIL_CAFILE_CREATEDIRECTORY
    // NF93 opt-in (q11296, rows 0x009994A0 / 0x00999620): Ego_r.pdb / FableWin.pdb ?CreateDirectoryW@CAFile@@SIXABVCWideString@@@Z (0x009994A0;
    // the source wrote CreateDirectory, <windows.h> made it CreateDirectoryW), ?CreateDirectoryForFile@CAFile@@SIXABVCWideString@@@Z (0x00999620)
    // and ?MakePath@CAFile@@SI?AVCWideString@@ABV2@000@Z (0x00997780; hidden result in ecx), static __fastcall. Declarations only.
    static void __fastcall CreateDirectoryW(const CWideString& path);                                    // 0x009994A0
    static void __fastcall CreateDirectoryForFile(const CWideString& path);                              // 0x00999620
    static CWideString __fastcall MakePath(const CWideString& drive,const CWideString& dir,const CWideString& name,const CWideString& ext);   // 0x00997780
#endif
#ifdef FABLE_RETAIL_CAFILE_INSTALLERPATHS
    // Nested opt-in (chunk 2032, row 0x009C47A0 CFileInstaller::AddFile): Ego_r.pdb / FableWin.pdb static __fastcall members, each
    // also the self-symbol of a landed row (0x009990B0, 0x00999140, 0x00997930, 0x00999C50, 0x00999CB0). Wide text is unsigned short.
    static CCharString __fastcall ConvertPathToFullPath(const CCharString&);                                // 0x009990B0 SI?AVCCharString@@ABV2@@Z
    static bool __fastcall ConvertPathToFullPath(const CWideString&,CWideString&);                          // 0x00999140 SI_NABVCWideString@@AAV2@@Z
    static CWideString __fastcall GetFilenameFromPathname(const CWideString&);                              // 0x00997930 SI?AVCWideString@@ABV2@@Z
    static bool __fastcall StartFileScan(const CWideString& pattern,CFileScanInfo& info,CWideString& firstFile);   // 0x00999C50
    static bool __fastcall ScanNextFile(const CWideString& pattern,CFileScanInfo& info,CWideString& nextFile);     // 0x00999CB0
#endif
    // Header wave 2026-11-03 (row 0x00821010, the length/version prefix of each file record): FableWin.pdb
    // ??$ReadVar@K@CAFile@@QAEXAAKPAVCASuspendableProcess@@@Z / ??$ReadVar@E@.. (no out-of-line copy in Fable.exe): an aligned
    // file reads through a rounded CArray<unsigned char> buffer.
#ifdef FABLE_RETAIL_CAFILE_READVAR_DECL
    // Per-TU opt-in (NF62, row 0x004FE630 CWorldMap::GetNavigationSectionOffsetInFile): the same member template, declaration only --
    // that TU calls the out-of-line instance ??$ReadVar@H@CAFile@@QAEXAAHPAVCASuspendableProcess@@@Z 0x00516180 (`push 0 ; push &var ;
    // mov ecx,file ; call`) at all three reads. (An explicit-specialization declaration after the class makes VC7.1 fail with C1001.)
    template<class T> void ReadVar(T& var,CASuspendableProcess* psuspendable);
#else
    template<class T> void ReadVar(T& var,CASuspendableProcess* psuspendable) {
        if(GetAlignment()<=1) Read(&var,sizeof(T),psuspendable);
        else {
            unsigned long buffer_length=GetAlignment();
            buffer_length=((sizeof(T)+buffer_length-1)/buffer_length)*buffer_length;
            CArray<unsigned char> buffer(buffer_length);
            Read(&buffer[0],sizeof(T),psuspendable);
            var=*(T*)&buffer[0];
        }
    }
#endif
#endif
#ifdef FABLE_RETAIL_CAFILE_WRITEVAR
    // Nested opt-in (chunk Q22, row 0x00A78B20 CNavQuadTree::SaveToFile): FableWin.pdb ??$WriteVar@J@CAFile@@QAEXABJ@Z /
    // ??$WriteVar@M@.. / ??$WriteVar@I@.. (CNavQuadTree::SaveToFile calls them; no out-of-line copy in Fable.exe): retail expands
    // each to `push 0 ; push sizeof(T) ; push &var ; call [vtbl+10h]` (Write, no alignment branch; as the landed
    // CNavBlockedQuadTreeNode::SaveToFile 0x00A76950 model).
    template<class T> void WriteVar(const T& var) { Write(&var,sizeof(T),0); }
#endif
    virtual void Open(const CWideString&,FileAccessType,unsigned long)=0;
    virtual void Close()=0;
    virtual void Read(void*,long,CASuspendableProcess*)=0;
    virtual void Write(const void*,long,CASuspendableProcess*)=0;
    virtual void SetPosition(long)=0;
    virtual void MovePosition(long)=0;
    virtual long GetPosition() const=0;
    virtual void SetLength(long)=0;
    virtual long GetLength() const=0;
    virtual bool IsOpen() const=0;
    virtual CWideString GetPathname() const=0;
    virtual bool IsIOFinished() const;
    virtual long GetAlignment() const;
    virtual CAFile* Clone() const=0;
    virtual CFileInformation GetFileInformation();
};
#endif
