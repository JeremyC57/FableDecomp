// Windows x64 host for the statically recompiled Fable.exe.
//
// Guest memory is identity-mapped: guest address == host address, everything
// guest-visible (PE image at 0x00400000, heap, stacks, TEBs) lives below 2 GiB,
// so guest pointers can be handed to 64-bit Win32 APIs unchanged. Imports are
// "traps": IAT slots hold addresses in an unmapped range (kTrapBase+) and
// recomp_dispatch routes calls there to host handlers that read the guest's
// x86 stack and call the real 64-bit API.
#pragma once

#ifndef RECOMP_IDENTITY_MEMORY
#define RECOMP_IDENTITY_MEMORY 1
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <shellapi.h>

#include <cstdint>
#include <cstring>
#include <initializer_list>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

extern "C" {
#include "recomp.h"
GuestFn recomp_lookup(uint32_t target);
extern void (*recomp_on_fatal)(Ctx* c, uint32_t eip, const char* what);
extern int (*recomp_on_unknown_target)(Ctx* c, uint32_t target);
}

namespace host {

// ---- constants --------------------------------------------------------------------
constexpr uint32_t kImageBase = 0x00400000;
constexpr uint32_t kImageEnd = 0x0146C000;
constexpr uint32_t kEntryPoint = 0x00401067;
constexpr uint32_t kTrapBase = 0xFFC00000;  // never mapped: import / COM method traps
constexpr uint32_t kTrapEnd = 0xFFFF0000;
constexpr uint32_t kGuestLimit = 0x7FFF0000; // guest allocations stay below 2 GiB
constexpr uint32_t kReturnSentinel = 0xFFFF1000; // return address for host->guest calls

// ---- logging ----------------------------------------------------------------------
void logInit(const std::wstring& path);
void log(const char* fmt, ...);
[[noreturn]] void die(const char* fmt, ...);
extern int g_logLevel;  // 0 = errors, 1 = info, 2 = every import call
#define HLOG(level, ...) do { if (::host::g_logLevel >= (level)) ::host::log(__VA_ARGS__); } while (0)

// ---- memory -----------------------------------------------------------------------
template <class T = void> inline T* gp(uint32_t a) { return reinterpret_cast<T*>(static_cast<uintptr_t>(a)); }
inline bool isGuest(const void* p) { return reinterpret_cast<uintptr_t>(p) < kGuestLimit; }
uint32_t ga(const void* p);  // host pointer -> guest address (dies if not guest-visible)

bool memInit();
uint32_t findFreeLow(uint32_t size, uint32_t align = 0x10000);
uint32_t lowVirtualAlloc(uint32_t addr, uint32_t size, DWORD type, DWORD protect);
uint32_t gmalloc(uint32_t size);
uint32_t gcalloc(uint32_t n, uint32_t size);
uint32_t grealloc(uint32_t p, uint32_t size);
void gfree(uint32_t p);
uint32_t gmsize(uint32_t p);
uint32_t gstrdup(const char* s);
uint32_t gwcsdup(const wchar_t* s);
uint32_t gmemdup(const void* p, uint32_t n);

// ---- guest stack access -------------------------------------------------------------
inline uint32_t arg(Ctx* c, int i) { return rd32(c->esp + 4 + 4 * i); }
template <class T = char> inline T* argp(Ctx* c, int i) { return gp<T>(arg(c, i)); }
inline void retStd(Ctx* c, uint32_t v, int nargs) { c->eax = v; c->esp += 4 + 4 * nargs; }
inline void retCdecl(Ctx* c, uint32_t v) { c->eax = v; c->esp += 4; }
inline void retStd64(Ctx* c, uint64_t v, int nargs) { c->eax = (uint32_t)v; c->edx = (uint32_t)(v >> 32); c->esp += 4 + 4 * nargs; }
inline void retCdeclF(Ctx* c, double v) { fpush(c, v); c->esp += 4; }
inline void retStdF(Ctx* c, double v, int nargs) { fpush(c, v); c->esp += 4 + 4 * nargs; }
// Guest handle/pointer value -> host (sign-extended, so INVALID_HANDLE_VALUE, HKEY_* work).
inline HANDLE hh(uint32_t v) { return reinterpret_cast<HANDLE>(static_cast<intptr_t>(static_cast<int32_t>(v))); }
inline uint32_t gh(const void* h) { return static_cast<uint32_t>(reinterpret_cast<uintptr_t>(h)); }

// ---- traps / import registry ----------------------------------------------------------
using Handler = void (*)(Ctx*);
using DataHandler = void (*)(Ctx*, uintptr_t data);
// Registers a host function; returns its guest-callable address.
uint32_t addTrap(const char* name, Handler h);
uint32_t addTrap(const char* name, DataHandler h, uintptr_t data);
const char* trapName(uint32_t addr);
// Import table: dll (case-insensitive, without path) + name -> handler.
struct ImportDef {
    const char* dll;
    const char* name;
    Handler fn;
    uint32_t dataAddr;  // non-zero: data import (IAT slot receives this address)
};
void registerImport(const char* dll, const char* name, Handler fn);
void registerDataImport(const char* dll, const char* name, uint32_t addr);
uint32_t resolveImport(const char* dll, const char* name);  // trap/data address, 0 if unknown
uint32_t resolveImportOrStub(const char* dll, const char* name);  // unknown -> logging stub
bool isKnownDll(const char* dll);
struct AutoReg {
    AutoReg(const char* dll, const char* name, Handler fn) { registerImport(dll, name, fn); }
};
void initImports();  // runs every module's registration function

// ---- automatic thunks: guest x86 stack -> host call -------------------------------------
namespace detail {
template <class T> T fetch(Ctx* c, uint32_t& off) {
    const uint32_t a = c->esp + 4 + off;
    if constexpr (std::is_same_v<T, double>) { off += 8; return rdf64(a); }
    else if constexpr (std::is_same_v<T, float>) { off += 4; return rdf32(a); }
    // 8-byte integers here are pointer-sized Win32 types (SIZE_T, ULONG_PTR, WPARAM, LPARAM,
    // LONG_PTR): 4 bytes on the x86 stack. No forwarded API takes a true 64-bit integer by value.
    else if constexpr (std::is_integral_v<T> && sizeof(T) == 8 && std::is_signed_v<T>) { off += 4; return static_cast<T>(static_cast<int32_t>(rd32(a))); }
    else if constexpr (std::is_integral_v<T> && sizeof(T) == 8) { off += 4; return static_cast<T>(rd32(a)); }
    else if constexpr (std::is_pointer_v<T>) { off += 4; return reinterpret_cast<T>(static_cast<intptr_t>(static_cast<int32_t>(rd32(a)))); }
    else if constexpr (std::is_enum_v<T>) { off += 4; return static_cast<T>(rd32(a)); }
    else if constexpr (std::is_integral_v<T> && std::is_signed_v<T>) { off += 4; return static_cast<T>(static_cast<int32_t>(rd32(a))); }
    else { off += 4; return static_cast<T>(rd32(a)); }
}
template <class R> void put(Ctx* c, R r) {
    if constexpr (std::is_floating_point_v<R>) fpush(c, (double)r);
    else if constexpr (std::is_pointer_v<R>) c->eax = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(r));
    else if constexpr (sizeof(R) == 8) { c->eax = (uint32_t)(uint64_t)r; c->edx = (uint32_t)((uint64_t)r >> 32); }
    else c->eax = static_cast<uint32_t>(r);
}
}  // namespace detail

// Argument converter: reads one guest stack argument, yields the host value,
// and writes results back after the call. Specialised for COM interfaces
// (com.hpp) and structures whose layout differs between x86 and x64.
template <class T, class = void> struct Arg {
    T v{};
    void in(Ctx* c, uint32_t& off) { v = detail::fetch<T>(c, off); }
    T host() { return v; }
    void post() {}
};
template <class T> struct Arg<T&, void> {  // references (REFGUID, REFIID): guest passes a pointer
    T* p = nullptr;
    void in(Ctx* c, uint32_t& off) { p = gp<T>(rd32(c->esp + 4 + off)); off += 4; }
    T& host() { return *p; }
    void post() {}
};

// void** out-parameters (VerQueryValue-style): host writes a pointer, the guest slot is 4 bytes.
template <> struct Arg<void**, void> {
    uint32_t g = 0;
    void* v = nullptr;
    void in(Ctx* c, uint32_t& off) { g = rd32(c->esp + 4 + off); off += 4; }
    void** host() { return g ? &v : nullptr; }
    void post() { if (g) wr32(g, static_cast<uint32_t>(reinterpret_cast<uintptr_t>(v))); }
};

namespace detail {
template <class R, class F, class Tuple, size_t... I> R callWith(F&& f, Tuple& t, std::index_sequence<I...>) {
    return f(std::get<I>(t).host()...);
}
template <class Tuple, size_t... I> void inAll(Ctx* c, uint32_t& off, Tuple& t, std::index_sequence<I...>) {
    (std::get<I>(t).in(c, off), ...);
}
template <class Tuple, size_t... I> void postAll(Tuple& t, std::index_sequence<I...>) { (std::get<I>(t).post(), ...); }

// Calls `f` with converted guest arguments starting at stack offset `off` (bytes after the
// return address); returns bytes consumed.
template <class R, class... A, class F> uint32_t invoke(Ctx* c, uint32_t off, F&& f) {
    const uint32_t start = off;
    std::tuple<Arg<A>...> args;
    using Seq = std::index_sequence_for<A...>;
    inAll(c, off, args, Seq{});
    if constexpr (std::is_void_v<R>) callWith<R>(f, args, Seq{});
    else {
        R r = callWith<R>(f, args, Seq{});
        put<R>(c, r);
    }
    postAll(args, Seq{});
    return off - start;
}

template <auto F, bool Std, class R, class... A> void thunkImpl(Ctx* c, R (*)(A...)) {
    const uint32_t used = invoke<R, A...>(c, 0, F);
    c->esp += 4 + (Std ? used : 0);
}
}  // namespace detail

template <auto F> void stdThunk(Ctx* c) { detail::thunkImpl<F, true>(c, F); }
template <auto F> void cdeclThunk(Ctx* c) { detail::thunkImpl<F, false>(c, F); }

#define HOST_CAT2(a, b) a##b
#define HOST_CAT(a, b) HOST_CAT2(a, b)
// Forward a stdcall Win32 function whose arguments need no conversion.
#define FWD_STD(dll, fn) static ::host::AutoReg HOST_CAT(reg_, __COUNTER__)(dll, #fn, &::host::stdThunk<&::fn>)
// Custom handler: IMPORT(dll, name) { ... }  (body gets Ctx* c)
#define IMPORT(dll, fn)                                                            \
    static void HOST_CAT(imp_, fn)(Ctx * c);                                       \
    static ::host::AutoReg HOST_CAT(reg_, fn)(dll, #fn, &HOST_CAT(imp_, fn));      \
    static void HOST_CAT(imp_, fn)(Ctx * c)
// Same, for names that are not C identifiers (decorated C++ names).
#define IMPORTN(dll, name, id)                                                     \
    static void HOST_CAT(imp_, id)(Ctx * c);                                       \
    static ::host::AutoReg HOST_CAT(reg_, id)(dll, name, &HOST_CAT(imp_, id));     \
    static void HOST_CAT(imp_, id)(Ctx * c)

// ---- threads / host->guest calls -----------------------------------------------------------
struct GuestThread {
    Ctx ctx{};
    uint32_t teb = 0, stackLo = 0, stackHi = 0;
    DWORD tid = 0;
};
GuestThread* currentThread();  // creates the guest context lazily for foreign host threads
inline Ctx* cur() { return &currentThread()->ctx; }
void threadsInit();
GuestThread* newGuestThread(uint32_t stackSize);
void bindThread(GuestThread* t);
// Calls guest code at `fn` with stdcall/cdecl args on the current thread's guest stack.
// All guest registers except EAX/EDX are preserved for the caller.
uint32_t guestCall(uint32_t fn, std::initializer_list<uint32_t> args);
uint32_t guestCallThis(uint32_t fn, uint32_t thisPtr, std::initializer_list<uint32_t> args);
HANDLE startGuestThread(uint32_t fn, uint32_t param, uint32_t stackSize, DWORD flags, DWORD* tid);
uint32_t pebAddr();

// ---- process-wide state ----------------------------------------------------------------------
extern std::wstring g_gameDir;   // with trailing backslash
extern std::wstring g_exePath;   // <game>\Fable.exe
extern HMODULE g_fableRes;       // Fable.exe loaded as a data file (resources)
extern HINSTANCE g_hinst;        // this executable
void runAtExit();                // guest atexit/_onexit list
[[noreturn]] void guestExit(uint32_t code);

// Guest PE images (loader.cpp)
void reserveGuestDllRanges();
void mapPeImage(const std::vector<uint8_t>& file, uint32_t base, const char* what);
uint32_t loadGuestDll(const std::string& path);  // recompiled DLL from the game folder, 0 if none
bool isGuestDll(uint32_t handle);
uint32_t guestDllExport(uint32_t base, const std::string& name);

// Module handles visible to the guest (small fake values; 0x00400000 = Fable.exe).
uint32_t moduleToGuest(HMODULE m, const char* name);
void registerModule(uint32_t guestHandle, HMODULE resources, const char* name);
HMODULE moduleFromGuest(uint32_t g);
const char* moduleName(uint32_t g);
HINSTANCE instFromGuest(uint32_t g);  // 0x400000 -> this exe's HINSTANCE (window classes)

// Strings
std::string narrow(const wchar_t* w);
std::wstring widen(const char* s);

}  // namespace host
