// CThreadedFile::FinishAndGetFileHandle  @ 0x0098ec70
// __fastcall, returns void* (the file handle stored at this+8)

class CThreadedFile;

class CThreadedFileManager
{ public:
    void CloseAllStreamsForFile(CThreadedFile* f);   // NF296: PDB name, was RemovePending
};

// Global manager instance at 0x13bc870
extern CThreadedFileManager g_ThreadedFileManager;

class CThreadedFile
{ public:
    int   field_0;   // +0
    int   DeviceID;   // +4
    void* handle;    // +8

    void* FinishAndGetFileHandle();
};

void* CThreadedFile::FinishAndGetFileHandle()
{
    if (this->handle != (void*)-1)
    {
        g_ThreadedFileManager.CloseAllStreamsForFile(this);
    }
    return this->handle;
}