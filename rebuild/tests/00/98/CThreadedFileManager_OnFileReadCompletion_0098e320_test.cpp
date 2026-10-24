// Behaviour test for 0x0098E320 CThreadedFileManager::OnFileReadCompletion: success just finishes the object (status 3),
// signals the manager event and unpauses an existing installer; an error on the DVD device sets the dirty-disc flag,
// on another device calls the hard-disk error callback with "" (when one is set).
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#endif
static void* g_signalled = 0; static int g_signals = 0;
static int SignalEvent(void* e) { g_signalled = e; ++g_signals; return 1; }
static int g_unpauses = 0;
struct CFileInstaller { void Unpause() { ++g_unpauses; } };
static CFileInstaller* g_installer = 0;
struct _OVERLAPPED { unsigned long Internal, InternalHigh, Offset, OffsetHigh; void* hEvent; };
struct CThreadedFile { int DeviceID; };
struct CThreadedFileManager;
struct CFileObject { int LoadingStatus; CThreadedFileManager* PManager; CThreadedFile* PFile; };
static bool g_dirtyDisc = false;
static int g_callbackCalls = 0; static const char* g_callbackArg = 0;
static void __fastcall Callback(const char* s) { ++g_callbackCalls; g_callbackArg = s; }
static void (__fastcall* g_callback)(const char*) = 0;
struct CThreadedFileManager {
    void* Event;
    static void __stdcall OnFileReadCompletion(unsigned long errorCode, unsigned long, _OVERLAPPED* overlapped)
    {
        CFileObject* object = (CFileObject*)overlapped->hEvent;
        if (errorCode != 0) {
            if (object->PFile->DeviceID == 1)
                g_dirtyDisc = true;
            else if (g_callback)
                g_callback("");
        }
        object->LoadingStatus = 3;
        SignalEvent(object->PManager->Event);
        if (g_installer)
            g_installer->Unpause();
    }
};
int main()
{
    int fails = 0, ev = 0;
    CThreadedFileManager manager; manager.Event = &ev;
    CThreadedFile dvd = { 1 }, hdd = { 0 };
    CFileObject object = { 1, &manager, &dvd };
    _OVERLAPPED ov; std::memset(&ov, 0, sizeof(ov)); ov.hEvent = &object;
    CThreadedFileManager::OnFileReadCompletion(0, 100, &ov);
    if (object.LoadingStatus != 3 || g_signalled != &ev || g_signals != 1 || g_dirtyDisc || g_unpauses != 0) ++fails;
    CFileInstaller installer; g_installer = &installer;
    CThreadedFileManager::OnFileReadCompletion(23, 0, &ov);
    if (!g_dirtyDisc || g_callbackCalls != 0 || g_unpauses != 1 || g_signals != 2) ++fails;
    g_dirtyDisc = false; object.PFile = &hdd;
    CThreadedFileManager::OnFileReadCompletion(23, 0, &ov);
    if (g_dirtyDisc || g_callbackCalls != 0) ++fails;
    g_callback = Callback;
    CThreadedFileManager::OnFileReadCompletion(23, 0, &ov);
    if (g_dirtyDisc || g_callbackCalls != 1 || !g_callbackArg || g_callbackArg[0] != 0 || g_signals != 4) ++fails;
    std::printf(fails ? "0098e320_TEST FAIL\n" : "0098e320_TEST PASS\n");
    return fails ? 1 : 0;
}