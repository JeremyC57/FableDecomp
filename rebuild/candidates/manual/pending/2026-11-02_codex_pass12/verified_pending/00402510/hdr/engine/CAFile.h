#ifdef FABLE_RETAIL_GFMAIN
#ifndef FABLE_RETAIL_HEADER_CAFILE
#define FABLE_RETAIL_HEADER_CAFILE
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CWideString.h"
class CCharString;
class CASuspendableProcess;
class CFileInformation;
enum FileAccessType { FILE_ACCESS_NULL=0, FILE_ACCESS_READ=1, FILE_ACCESS_READ_WRITE=2, FILE_ACCESS_READ_WRITE_CREATE=3, FILE_ACCESS_CREATE=4 };
// Original CAFile virtual order from Ego_r TPI and native CDiskFileWin32 vtable.
class CAFile : public CBaseClassNonCopyable {
public:
#ifdef FABLE_RETAIL_GFMAIN
 static CWideString __fastcall GetProjectPath();
 static void __fastcall SetCurrentPath(const CWideString&);
 static void __fastcall SetErrorCallback(void(__fastcall*f)()){m_errorCallback=f;}
 static void __fastcall SetHardDiskErrorCallback(void(__fastcall*f)(const char*)){m_harddiskErrorCallback=f;}
private:
 static void(__fastcall*m_errorCallback)();
 static void(__fastcall*m_harddiskErrorCallback)(const char*);
public:
#endif
#ifdef FABLE_RETAIL_RELATIVE_PATH
    static CCharString __fastcall GetRelativeFilePath(const CCharString&,const CCharString&);
    static void __fastcall SplitPath(const CCharString&,CCharString*,CCharString*,CCharString*,CCharString*);
#endif
#ifdef FABLE_RETAIL_EMITTER_DATABASE
    static CCharString __fastcall GetAbsoluteFilePath(const CCharString&,const CCharString&);
#endif
#ifdef FABLE_RETAIL_SOUND_INIT
    static void __fastcall SplitPath(const CWideString&,CWideString*,CWideString*,CWideString*,CWideString*);
    static CWideString __fastcall ConvertPathToFullPath(const CWideString&);
#endif
    static bool __fastcall PathExists(const CWideString&);
    static bool __fastcall IsPathFile(const CWideString&);
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

#else
#ifndef FABLE_RETAIL_HEADER_CAFILE
#define FABLE_RETAIL_HEADER_CAFILE
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CWideString.h"
#ifdef FABLE_RETAIL_MAP_BRUSHES
#include "engine/CArray.h"
#endif
class CCharString;
class CASuspendableProcess;
class CFileInformation;
enum FileAccessType { FILE_ACCESS_NULL=0, FILE_ACCESS_READ=1, FILE_ACCESS_READ_WRITE=2, FILE_ACCESS_READ_WRITE_CREATE=3, FILE_ACCESS_CREATE=4 };
// Original CAFile virtual order from Ego_r TPI and native CDiskFileWin32 vtable.
class CAFile : public CBaseClassNonCopyable {
public:
#ifdef FABLE_RETAIL_RELATIVE_PATH
    static CCharString __fastcall GetRelativeFilePath(const CCharString&,const CCharString&);
    static void __fastcall SplitPath(const CCharString&,CCharString*,CCharString*,CCharString*,CCharString*);
#endif
#ifdef FABLE_RETAIL_EMITTER_DATABASE
    static CCharString __fastcall GetAbsoluteFilePath(const CCharString&,const CCharString&);
#endif
#ifdef FABLE_RETAIL_MAP_BRUSHES
    template<class T>void ReadVar(T& var,CASuspendableProcess* psuspendable) {
        if(GetAlignment()<=1)Read(&var,sizeof(T),psuspendable);
        else {
            unsigned long buffer_length=GetAlignment();
            buffer_length=((sizeof(T)+buffer_length-1)/buffer_length)*buffer_length;
            CArray<unsigned char> buffer(buffer_length);
            Read(&buffer[0],sizeof(T),psuspendable);
            var=*(T*)&buffer[0];
        }
    }
#endif
    static bool __fastcall PathExists(const CWideString&);
    static bool __fastcall IsPathFile(const CWideString&);
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

#endif
