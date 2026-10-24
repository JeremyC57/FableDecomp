// CLHFileCompletionCallback::OnFileReadCompletion(bool success) -- forward a finished threaded read to the LHA file client's callback with its user data, then delete this one-shot callback
// Leaf: ?OnFileReadCompletion@CLHFileCompletionCallback@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ADD130, 0x24 bytes (+ int3 padding), thiscall, `ret 4`.
// Identity: CLHFileCompletionCallback vtable 0x0129F084 slot 2 (RTTI .?AVCLHFileCompletionCallback@@ over
// CThreadedFileCallback / CBaseClassNonCopyable / CBaseClass; slot 0 its deleting destructor 0x00ADD5D0); FableWin.pdb
// names the base slot ?OnFileReadCompletion@CThreadedFileCallback@@UAEX_N@Z. The object (0xC bytes) is created by
// CLHFile's read 0x00ADD240 as {vtable, client callback (+4), user data (+8)}. Catalog "Callback_InvokePair_add130" is
// descriptive.
// Callees: the client callback's first virtual (`call [eax]`, (bool, user data)), this class's virtual deleting
// destructor (`push 1 ; call [eax]`).
// Shape: `m_pClientCallback->OnReadComplete(success != false, m_pUserData); delete this;` -- the bool is re-normalised
// (`test dl,dl ; setne dl`) in the byte register over the loaded user-data word.
namespace LHA {
class CFileReadCallback {                                   // the LHA client's completion interface (name descriptive)
public:
    virtual void OnReadComplete(bool success, void* userData) = 0;   // slot 0
};
}

enum EThreadedFilePriority { THREADED_FILE_PRIORITY_DEFAULT };

class CBaseClass {
public:
    virtual ~CBaseClass() { }
};

class CBaseClassNonCopyable : public CBaseClass {
public:
    CBaseClassNonCopyable() { }
private:
    CBaseClassNonCopyable(const CBaseClassNonCopyable&);
    CBaseClassNonCopyable& operator=(const CBaseClassNonCopyable&);
};

class CThreadedFileCallback : public CBaseClassNonCopyable { // FableWin.pdb, 4 bytes (the vtable)
public:
    CThreadedFileCallback();
    virtual ~CThreadedFileCallback();
    virtual bool OnFileReadBegin(void** buffer, EThreadedFilePriority priority);
    virtual void OnFileReadCompletion(bool success);
};

class CLHFileCompletionCallback : public CThreadedFileCallback {   // 0xC bytes
public:
    LHA::CFileReadCallback* m_pClientCallback;              // +0x4
    void*                   m_pUserData;                    // +0x8
    virtual void OnFileReadCompletion(bool success);
};

void CLHFileCompletionCallback::OnFileReadCompletion(bool success)
{
    m_pClientCallback->OnReadComplete(success != false, m_pUserData);
    delete this;
}