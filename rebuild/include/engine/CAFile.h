#ifndef FABLE_RETAIL_HEADER_CAFILE
#define FABLE_RETAIL_HEADER_CAFILE
#include "engine/CBaseClassNonCopyable.h"
#include "fable_string.h"
#ifdef FABLE_RETAIL_CAFILE
#include "engine/CArray.h"
#endif
class CCharString;
class CASuspendableProcess;
class CFileInformation;
enum FileAccessType { FILE_ACCESS_NULL=0, FILE_ACCESS_READ=1, FILE_ACCESS_READ_WRITE=2, FILE_ACCESS_READ_WRITE_CREATE=3, FILE_ACCESS_CREATE=4 };
// Original CAFile virtual order from Ego_r TPI and native CDiskFileWin32 vtable.
class CAFile : public CBaseClassNonCopyable {
public:
    static CCharString __fastcall GetRelativeFilePath(const CCharString&,const CCharString&);
    static void __fastcall SplitPath(const CCharString&,CCharString*,CCharString*,CCharString*,CCharString*);
    static bool __fastcall PathExists(const CWideString&);
    static bool __fastcall IsPathFile(const CWideString&);
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
