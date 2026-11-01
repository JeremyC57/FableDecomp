// CLHFile::CLHFile(LHA::CFileManager* manager, const char* fileName) -- open a CLHFile over a new CThreadedFile
#include "engine/CCountedPointer.h"
#include <string.h>

template <class T> class CBasicString { public:
    T*            PData;
    unsigned long StringLength;
    bool operator==(const T* text) const;                         // 0x004115A0
};
class CCharStringData { public:
    CBasicString<char> Data;
};
class CWideString;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString();                                                 // 0x0099E4B0
    CCharString(const char* text, long length = -1);               // 0x0099EBF0
    ~CCharString();                                                // 0x0099EAE0
    CCharString ToUpper() const;                                   // 0x0099EE20
    CWideString ToWideString() const;                              // 0x0099E480
    __forceinline bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
class CWideString {
public:
    void* PStringData;
    CWideString();                                                 // 0x0099AED0
    CWideString(const unsigned short* text);                       // 0x0099B6B0
    ~CWideString();                                                // 0x0099B510
    CCharString ToCharString() const;                              // 0x0099B2C0
};
enum EThreadedFileDeviceID { };
class CAFile {
public:
    static void __fastcall SplitPath(const CCharString& path, CCharString* drive, CCharString* dir, CCharString* name, CCharString* ext);   // 0x009976D0
    static void __fastcall SplitPath(const CWideString& path, CWideString* drive, CWideString* dir, CWideString* name, CWideString* ext);   // 0x00997620
};
class CThreadedFile {
public:
    CThreadedFile();                                               // 0x0098DFD0
    virtual ~CThreadedFile();
    bool Open(const CWideString& fileName, bool readOnly);         // 0x0098E1E0
    EThreadedFileDeviceID DeviceID;                                // +0x04
    void*                 FileHandle;                              // +0x08
    CWideString           Filename;                                // +0x0C
    unsigned long         Length;                                  // +0x10
    unsigned long         PhysicalSortKey;                         // +0x14
    bool                  OpenedForWrite;                          // +0x18
    bool                  OpenFlag;                                // +0x19
};
namespace LHA {
class CFileManager;
class CFileReadCallback;
class CFile {                                                      // vtable 0x0129F06C
public:
    virtual bool GetSize(unsigned long* size) = 0;
    virtual bool Read(unsigned long block, unsigned long blockCount, unsigned long reserved, void* buffer,
                      void* userData, CFileReadCallback* callback) = 0;
    virtual long GetBlockSize() = 0;
    virtual void Close() = 0;
    virtual ~CFile() { }
};
}
class CLHFile : public LHA::CFile {                                // vtable 0x0129F094
public:
    CCountedPointer<CThreadedFile> m_pFile;                        // +0x04
    unsigned long                  m_unknown0C;                    // +0x0C
    bool                           m_bIsWMA;                       // +0x10
    CLHFile(LHA::CFileManager* manager, const char* fileName);
    CLHFile(LHA::CFileManager* manager, const unsigned short* fileName);
    virtual bool GetSize(unsigned long* size);
    virtual bool Read(unsigned long block, unsigned long blockCount, unsigned long reserved, void* buffer,
                      void* userData, LHA::CFileReadCallback* callback);
    virtual long GetBlockSize();
    virtual void Close();
};

CLHFile::CLHFile(LHA::CFileManager*, const char* fileName)
    : m_unknown0C(0)
{
    m_pFile.Reset(new CThreadedFile);
    m_bIsWMA = false;
    CCharString ext;
    CAFile::SplitPath(CCharString(fileName), 0, 0, 0, &ext);
    if (ext.ToUpper() == ".WMA")
        m_bIsWMA = true;
    m_pFile->Open(CCharString(fileName).ToWideString(), true);
}

CLHFile::CLHFile(LHA::CFileManager*, const unsigned short* fileName)
    : m_unknown0C(0)
{
    m_pFile.Reset(new CThreadedFile);
    m_bIsWMA = false;
    CWideString ext;
    CAFile::SplitPath(CWideString(fileName), 0, 0, 0, &ext);
    if (ext.ToCharString().ToUpper() == ".WMA")
        m_bIsWMA = true;
    m_pFile->Open(fileName, true);
}
