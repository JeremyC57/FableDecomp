// Win32 on POSIX: the subset of the Windows API the host (and through it the game) uses,
// implemented on POSIX, SDL2 and the C++ runtime. Declarations come from the MinGW-w64
// headers (see include/windows.h), so every function here is checked against the real
// prototype. Internal helpers shared by the w32/*.cpp files live in namespace w32.
#pragma once

#include <windows.h>

#include <atomic>
#include <condition_variable>
#include <functional>
#include <cstdint>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include "wstring16.h"

namespace w32 {

// ---- last error ---------------------------------------------------------------------------
void setError(DWORD e);
DWORD errnoToWin(int e);
inline BOOL fail(DWORD e) { setError(e); return FALSE; }

// ---- UTF-16 strings (wchar_t is 16-bit here) ------------------------------------------------
using wstr = host::wstring;  // UTF-16; never std::wstring (see wstring16.h)
size_t wlen(const wchar_t* s);
std::string toUtf8(const wchar_t* s, size_t n = size_t(-1));
wstr fromUtf8(const char* s, size_t n = size_t(-1));
std::string acpToUtf8(const char* s, size_t n = size_t(-1));  // Windows-1252 -> UTF-8
std::string utf8ToAcp(const char* s);
wchar_t upper(wchar_t c);
wchar_t lower(wchar_t c);

// ---- kernel objects -------------------------------------------------------------------------
// Every handle the API hands out is a small integer (so the 32-bit guest can hold it)
// naming a reference-counted Object. Waitable objects share one lock and condition variable:
// simple, and correct for WaitForMultipleObjects.
struct Object {
    virtual ~Object() = default;
    virtual bool waitable() const { return false; }
    virtual bool signaled(DWORD tid) const { (void)tid; return false; }
    virtual void acquire(DWORD tid) { (void)tid; }  // called under the wait lock when a wait succeeds
    virtual const char* kind() const = 0;
};
HANDLE newHandle(std::shared_ptr<Object> o);
std::shared_ptr<Object> object(HANDLE h);
template <class T> std::shared_ptr<T> objectAs(HANDLE h) { return std::dynamic_pointer_cast<T>(object(h)); }
bool closeHandle(HANDLE h);

std::mutex& waitLock();
void notifyAll();  // call with waitLock held after changing any object's state
DWORD currentTid();

// Alertable waits: queued APCs/completion routines run when the thread waits alertably.
void queueApc(DWORD tid, std::function<void()> fn);
bool runApcs();  // returns true if any ran

// ---- paths ----------------------------------------------------------------------------------
// Windows paths ("Z:\dir\file", "dir\file", "C:\...") map to POSIX paths: drive Z: is the
// root as in Wine, other drives map to the game's virtual C: (FABLE_C_DRIVE). Lookup is
// case-insensitive, component by component, like NTFS.
std::string toPosixPath(const wchar_t* winPath, bool forCreate = false);
std::string toPosixPathA(const char* winPath, bool forCreate = false);
wstr toWindowsPath(const std::string& posix);
void setCDrive(const std::string& dir);

// ---- SDL / windows (user.cpp) ---------------------------------------------------------------
void* sdlWindow(HWND h);  // SDL_Window* for a top-level window, nullptr otherwise

// ---- time -----------------------------------------------------------------------------------
uint64_t nowFileTime();  // 100 ns since 1601 (UTC)
uint64_t monotonicNs();

}  // namespace w32
