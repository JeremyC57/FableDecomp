// CLHFile::Read(unsigned long block, unsigned long blockCount, unsigned long reserved, void* buffer, void* userData, LHA::CFileReadCallback* callback) -- queue an asynchronous block read of the open threaded file, completing through a one-shot CLHFileCompletionCallback
// Leaf: ?Read@CLHFile@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ADD240, 0xED bytes (+ int3 padding), thiscall, `ret 18h`, returns true when
// the read was queued.
// Identity: CLHFile vtable 0x0129F094 slot 1 (RTTI .?AVCLHFile@@ over LHA::CFile; slots 0 / 2 / 3 / 4 are the landed
// GetSize 0x00ADD160, block-size query 0x00ADD190 (0x200 / 0x800 / 1), Close 0x00ADD330 and the deleting destructor
// 0x00ADD5F0). The slot name is inferred from the body (no symbol). Layout from the constructor 0x00ADD380:
// CCountedPointer<CThreadedFile> m_pFile +4/+8, +0xC, bool m_bIsWMA +0x10 (set when the file name ends ".WMA").
// FableWin.pdb: CThreadedFile.OpenFlag +0x19; CThreadedFileObjectDesc {PDst, FilePos, Size, PCallback,
// QueueWhenFinished +0x10, Priority +0x14} whose constructor gives {0, 0, 0, 0, true, 3}. The two priority words
// 0x0139945C (4) / 0x01399460 (3) are initialised data. Catalog "NHeroInformationScreens_CAnimationEntry_Construct"
// is propagated.
// Callees: scalar operator new @ 0x00BFEA1A (0xC-byte callback), CBaseClassNonCopyable::CBaseClassNonCopyable @
// 0x0099A2F0, this object's block-size virtual (slot 2, twice), CThreadedFile::AddObject(const desc&) @ 0x0098F350.
// Shape: `if (file && file->OpenFlag) { desc; desc.PCallback = new CLHFileCompletionCallback(callback, userData);
// desc.FilePos = GetBlockSize() * block; desc.PDst = buffer; desc.Size = GetBlockSize() * blockCount;
// desc.QueueWhenFinished = false; desc.Priority = m_bIsWMA ? 4-word : 3-word (if/else); m_pFile->AddObject(desc);
// return true; } return false;` -- VC7.1 duplicates the queue-and-return tail into both priority arms.
enum EThreadedFilePriority { THREADED_FILE_PRIORITY_DEFAULT = 3 };

class CThreadedFileCallback;

class CThreadedFileObjectDesc {                             // FableWin.pdb, 0x18 bytes
public:
    void*                  PDst;                            // +0x00
    unsigned long          FilePos;                         // +0x04
    unsigned long          Size;                            // +0x08
    CThreadedFileCallback* PCallback;                       // +0x0C
    bool                   QueueWhenFinished;               // +0x10
    EThreadedFilePriority  Priority;                        // +0x14
    CThreadedFileObjectDesc()
        : PDst(0), FilePos(0), Size(0), PCallback(0), QueueWhenFinished(true), Priority(THREADED_FILE_PRIORITY_DEFAULT) { }
};

class CThreadedFile {                                       // FableWin.pdb, 0x1C bytes
public:
    virtual ~CThreadedFile();
    long          DeviceID;                                 // +0x04
    void*         FileHandle;                               // +0x08
    void*         Filename;                                 // +0x0C CWideString
    unsigned long Length;                                   // +0x10
    unsigned long PhysicalSortKey;                          // +0x14
    bool          OpenedForWrite;                           // +0x18
    bool          OpenFlag;                                 // +0x19
    unsigned long AddObject(const CThreadedFileObjectDesc& desc);   // 0x0098F350
};

struct CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T*              Data;                                   // +0x0
    CCPPointerInfo* Info;                                   // +0x4
    T* operator->() const { return Data; }
};

namespace LHA {
class CFileReadCallback {                                   // the LHA client's completion interface (name descriptive)
public:
    virtual void OnReadComplete(bool success, void* userData) = 0;
};

class CFile {                                               // vtable 0x0129F06C
public:
    virtual bool GetSize(unsigned long* size) = 0;          // slot 0
    virtual bool Read(unsigned long block, unsigned long blockCount, unsigned long reserved, void* buffer,
                      void* userData, CFileReadCallback* callback) = 0;   // slot 1
    virtual long GetBlockSize() = 0;                        // slot 2
    virtual void Close() = 0;                               // slot 3
    virtual ~CFile() { }                                    // slot 4
};
}

class CBaseClass {
public:
    virtual ~CBaseClass() { }
};

class CBaseClassNonCopyable : public CBaseClass {
protected:
    CBaseClassNonCopyable();                                // 0x0099A2F0
private:
    CBaseClassNonCopyable(const CBaseClassNonCopyable&);
    CBaseClassNonCopyable& operator=(const CBaseClassNonCopyable&);
};

class CThreadedFileCallback : public CBaseClassNonCopyable {
public:
    CThreadedFileCallback() { }
    virtual ~CThreadedFileCallback();
    virtual bool OnFileReadBegin(void** buffer, EThreadedFilePriority priority);
    virtual void OnFileReadCompletion(bool success);
};

class CLHFileCompletionCallback : public CThreadedFileCallback {   // vtable 0x0129F084, 0xC bytes
public:
    LHA::CFileReadCallback* m_pClientCallback;              // +0x4
    void*                   m_pUserData;                    // +0x8
    CLHFileCompletionCallback(LHA::CFileReadCallback* callback, void* userData)
        : m_pClientCallback(callback), m_pUserData(userData) { }
    virtual void OnFileReadCompletion(bool success);        // 0x00ADD130
};

extern EThreadedFilePriority g_LHFileStreamedReadPriority;  // 0x0139945C (4)
extern EThreadedFilePriority g_LHFileReadPriority;          // 0x01399460 (3)

class CLHFile : public LHA::CFile {                         // vtable 0x0129F094
public:
    CCountedPointer<CThreadedFile> m_pFile;                 // +0x04
    unsigned long                  m_unknown0C;             // +0x0C
    bool                           m_bIsWMA;                // +0x10
    virtual bool GetSize(unsigned long* size);
    virtual bool Read(unsigned long block, unsigned long blockCount, unsigned long reserved, void* buffer,
                      void* userData, LHA::CFileReadCallback* callback);
    virtual long GetBlockSize();
    virtual void Close();
};

bool CLHFile::Read(unsigned long block, unsigned long blockCount, unsigned long, void* buffer, void* userData,
                   LHA::CFileReadCallback* callback)
{
    CThreadedFile* file = m_pFile.Data;
    if (file && file->OpenFlag) {
        CThreadedFileObjectDesc desc;
        desc.PCallback = new CLHFileCompletionCallback(callback, userData);
        desc.FilePos = GetBlockSize() * block;
        desc.PDst = buffer;
        desc.Size = GetBlockSize() * blockCount;
        desc.QueueWhenFinished = false;
        if (m_bIsWMA)
            desc.Priority = g_LHFileStreamedReadPriority;
        else
            desc.Priority = g_LHFileReadPriority;
        m_pFile->AddObject(desc);
        return true;
    }
    return false;
}