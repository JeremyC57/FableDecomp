#ifndef FABLE_RETAIL_HEADER_CAFILE
#define FABLE_RETAIL_HEADER_CAFILE
#include "engine/CBaseClassNonCopyable.h"
#include "fable_string.h"
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
