// CLHFileCompletionCallback::CLHFileCompletionCallback(void* callback, void* userData) -- the inline CThreadedFileCallback base (CBaseClassNonCopyable constructor call), the callback at +4, the own vtable, the user data at +8
// Leaf: ??0CLHFileCompletionCallback@@QAE@PAX0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ADD1C0, 34 bytes (+ int3 padding to 48), thiscall, `ret 8`, returns this.
// Identity (proven class; parameter types descriptive): CLHFileCompletionCallback::CLHFileCompletionCallback(void*, void*); evidence: it stores vtable 0x0129F084 (RTTI .?AVCLHFileCompletionCallback@@, bases CThreadedFileCallback <- CBaseClassNonCopyable <- CBaseClass); the landed CLHFile::Read 0x00ADD240 inlines this constructor as `new CLHFileCompletionCallback(callback, userData)`; neither PDB holds the class, so the two pointer parameters are typed void* (no proven class for the callback object)
// Label: the manifest's "CVertexShaderData" is propagated (family J024: six two-argument constructors that call one base
//   constructor and store the arguments around the own vtable store; retail has no identical-code folding).
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable() 0x0099A2F0 (??0CBaseClassNonCopyable@@IAE@XZ, catalog-bound, PDB-confirmed).
// Shape: the CThreadedFileCallback constructor is inline and its vtable store dead; VC7.1 schedules the +4 store before the own vtable store (the landed CLHFile::Read inline copy has the same order).
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseClassNonCopyable : public CBaseClass {
protected:
    CBaseClassNonCopyable();                                    // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();                           // 0x0099A300
};
enum EThreadedFilePriority { EThreadedFilePriority_Force32 = 0x7fffffff };
class CThreadedFileCallback : public CBaseClassNonCopyable {   // RTTI .?AVCThreadedFileCallback@@
public:
    CThreadedFileCallback() { }                                 // ??0CThreadedFileCallback@@QAE@XZ (inline here)
    virtual ~CThreadedFileCallback();
    virtual bool OnFileReadBegin(void** buffer, EThreadedFilePriority priority);
    virtual void OnFileReadCompletion(bool success);
};
class CLHFileCompletionCallback : public CThreadedFileCallback {   // vtable 0x0129F084 (RTTI), 0xC bytes
public:
    CLHFileCompletionCallback(void* callback, void* userData);
    virtual void OnFileReadCompletion(bool success);            // 0x00ADD130
    void* Callback;                                             // +0x04 (descriptive name: the client callback object)
    void* UserData;                                             // +0x08 (descriptive name: passed back to it)
};

CLHFileCompletionCallback::CLHFileCompletionCallback(void* callback, void* userData)
    : Callback(callback), UserData(userData)
{
}