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
class CAFile : public CBaseClassNonCopyable {
#ifdef FABLE_RETAIL_CAFILE_ISROOMFORFILE_B
public:
    // Opt-in (chunk 2052, row 0x0049F4C0): Ego_r.pdb ?IsRoomForFile@CAFile@@SI_NJABVCWideString@@@Z (static __fastcall: size in ecx,
    // path in edx), out of line at 0x00999AB0. Declaration only.
    static bool FABLE_FASTCALL IsRoomForFile(long size, const CWideString& path);   // 0x00999AB0
#endif
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
    static CCharString __fastcall GetRelativeFilePath(const CCharString&,const CCharString&);
    static void __fastcall SplitPath(const CCharString&,CCharString*,CCharString*,CCharString*,CCharString*);
    static bool __fastcall PathExists(const CWideString&);
    static bool __fastcall IsPathFile(const CWideString&);
#ifdef FABLE_RETAIL_CAFILE_ISROOMFORFILE
    // Nested opt-in (wf19, row 0x0055D180): Ego_r.pdb / FableWin.pdb ?IsRoomForFile@CAFile@@SI_NJABVCWideString@@@Z (retail 0x00999AB0; the
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
