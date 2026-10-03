// MSVCR71 / MSVCP71 imports.
#include "format.hpp"
#include "host.hpp"

#include <io.h>
#include <direct.h>
#include <mbstring.h>
#include <process.h>
#include <shellapi.h>

#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cwchar>
#include <cwctype>
#include <mutex>
#include <vector>

namespace host {
namespace {

constexpr const char* R = "msvcr71.dll";
constexpr const char* P = "msvcp71.dll";

// ---- process state ------------------------------------------------------------------------
uint32_t g_fmode, g_commode, g_adjustFdiv, g_acmdln;  // guest addresses of CRT variables
std::vector<uint32_t> g_atexit;
std::mutex g_atexitLock;

struct CrtThread {
    uint32_t errnoAddr = 0;
    uint32_t holdrand = 1;
};
thread_local CrtThread t_crt;

// FILE* / find handles: guest sees small tokens.
std::vector<FILE*> g_files;
std::mutex g_fileLock;
uint32_t fileToGuest(FILE* f) {
    std::lock_guard<std::mutex> l(g_fileLock);
    for (size_t i = 0; i < g_files.size(); ++i)
        if (!g_files[i]) { g_files[i] = f; return 0xFFA00000u + 0x20u * static_cast<uint32_t>(i + 1); }
    g_files.push_back(f);
    return 0xFFA00000u + 0x20u * static_cast<uint32_t>(g_files.size());
}
FILE* fileFromGuest(uint32_t g) {
    std::lock_guard<std::mutex> l(g_fileLock);
    const uint32_t i = (g - 0xFFA00000u) / 0x20u;
    if (g < 0xFFA00000u || i == 0 || i > g_files.size()) return nullptr;
    return g_files[i - 1];
}
void fileForget(uint32_t g) {
    std::lock_guard<std::mutex> l(g_fileLock);
    const uint32_t i = (g - 0xFFA00000u) / 0x20u;
    if (g >= 0xFFA00000u && i > 0 && i <= g_files.size()) g_files[i - 1] = nullptr;
}

std::vector<intptr_t> g_finds;
std::mutex g_findLock;

}  // namespace

void crtInit(const std::string& cmdline) {
    g_fmode = gcalloc(1, 4);
    g_commode = gcalloc(1, 4);
    g_adjustFdiv = gcalloc(1, 4);
    g_acmdln = gcalloc(1, 4);
    wr32(g_acmdln, gstrdup(cmdline.c_str()));
    registerDataImport(R, "_acmdln", g_acmdln);
    registerDataImport(R, "_adjust_fdiv", g_adjustFdiv);
}

void runAtExit() {
    for (;;) {
        uint32_t fn;
        {
            std::lock_guard<std::mutex> l(g_atexitLock);
            if (g_atexit.empty()) return;
            fn = g_atexit.back();
            g_atexit.pop_back();
        }
        guestCall(fn, {});
    }
}

void guestExit(uint32_t code) {
    log("guest exit(%u)", code);
    runAtExit();
    ExitProcess(code);
}

namespace {

// ============================================================================
// startup / termination
// ============================================================================
IMPORT(R, __set_app_type) { retCdecl(c, 0); }
IMPORT(R, __p__fmode) { retCdecl(c, g_fmode); }
IMPORT(R, __p__commode) { retCdecl(c, g_commode); }
IMPORT(R, __setusermatherr) { retCdecl(c, 0); }
IMPORT(R, _initterm) {
    const uint32_t b = arg(c, 0), e = arg(c, 1);
    for (uint32_t p = b; p < e; p += 4) {
        const uint32_t fn = rd32(p);
        if (fn) guestCall(fn, {});
    }
    retCdecl(c, 0);
}
IMPORT(R, __getmainargs) {
    static uint32_t argv = 0, envp = 0;
    if (!argv) {
        int n = 0;
        LPWSTR* w = CommandLineToArgvW(GetCommandLineW(), &n);
        argv = gcalloc(n + 1, 4);
        wr32(argv, gstrdup(narrow(g_exePath.c_str()).c_str()));
        int out = 1;
        for (int i = 1; i < n; ++i) {
            if (_wcsicmp(w[i], L"--game") == 0 && i + 1 < n) { ++i; continue; }
            wr32(argv + 4 * out++, gstrdup(narrow(w[i]).c_str()));
        }
        LocalFree(w);
        envp = gcalloc(1, 4);
        static int argc = 0;
        argc = out;
        wr32(arg(c, 0), argc);
    } else {
        uint32_t n = 0;
        while (rd32(argv + 4 * n)) ++n;
        wr32(arg(c, 0), n);
    }
    wr32(arg(c, 1), argv);
    wr32(arg(c, 2), envp);
    retCdecl(c, 0);
}
IMPORT(R, _amsg_exit) { die("CRT runtime error R60%02u", arg(c, 0)); }
IMPORT(R, exit) { guestExit(arg(c, 0)); }
IMPORT(R, _exit) { log("guest _exit(%u)", arg(c, 0)); ExitProcess(arg(c, 0)); }
IMPORT(R, _cexit) { runAtExit(); retCdecl(c, 0); }
IMPORT(R, _c_exit) { retCdecl(c, 0); }
IMPORT(R, abort) { die("guest called abort() from 0x%08X", rd32(c->esp)); }
IMPORT(R, _purecall) { die("pure virtual function call from 0x%08X", rd32(c->esp)); }
IMPORTN(R, "?terminate@@YAXXZ", mangled_1) { die("std::terminate() from 0x%08X", rd32(c->esp)); }
IMPORT(R, __security_error_handler) { die("buffer overrun detected (code %u)", arg(c, 0)); }
IMPORT(R, _XcptFilter) { retCdecl(c, 0); /* EXCEPTION_CONTINUE_SEARCH */ }
IMPORT(R, _except_handler3) { die("SEH exception dispatch reached _except_handler3 (unsupported)"); }
// __CxxFrameHandler and _CxxThrowException: see eh.cpp.
IMPORT(R, _onexit) {
    std::lock_guard<std::mutex> l(g_atexitLock);
    g_atexit.push_back(arg(c, 0));
    retCdecl(c, arg(c, 0));
}
IMPORT(R, __dllonexit) {
    std::lock_guard<std::mutex> l(g_atexitLock);
    g_atexit.push_back(arg(c, 0));
    retCdecl(c, arg(c, 0));
}

// _controlfp: abstract CRT bits <-> x87 control word (and MXCSR rounding).
uint32_t cwToAbstract(uint16_t cw) {
    uint32_t a = 0;
    if (cw & 0x01) a |= 0x10;       // IM -> _EM_INVALID
    if (cw & 0x02) a |= 0x80000;    // DM -> _EM_DENORMAL
    if (cw & 0x04) a |= 0x08;       // ZM
    if (cw & 0x08) a |= 0x04;       // OM
    if (cw & 0x10) a |= 0x02;       // UM
    if (cw & 0x20) a |= 0x01;       // PM
    switch ((cw >> 10) & 3) { case 1: a |= 0x100; break; case 2: a |= 0x200; break; case 3: a |= 0x300; break; }
    switch ((cw >> 8) & 3) { case 0: a |= 0x20000; break; case 2: a |= 0x10000; break; default: break; }
    if (cw & 0x1000) a |= 0x40000;  // IC affine
    return a;
}
uint16_t abstractToCw(uint32_t a, uint16_t old) {
    uint16_t cw = old & 0xE0C0;
    if (a & 0x10) cw |= 0x01;
    if (a & 0x80000) cw |= 0x02;
    if (a & 0x08) cw |= 0x04;
    if (a & 0x04) cw |= 0x08;
    if (a & 0x02) cw |= 0x10;
    if (a & 0x01) cw |= 0x20;
    switch (a & 0x300) { case 0x100: cw |= 0x400; break; case 0x200: cw |= 0x800; break; case 0x300: cw |= 0xC00; break; }
    switch (a & 0x30000) { case 0x20000: break; case 0x10000: cw |= 0x200; break; default: cw |= 0x300; break; }
    if (a & 0x40000) cw |= 0x1000;
    return cw;
}
IMPORT(R, _controlfp) {
    const uint32_t nw = arg(c, 0), mask = arg(c, 1) & 0xFFFFF7FFu;
    if (mask) {
        const uint32_t cur = cwToAbstract(c->fpu.cw);
        const uint32_t v = (nw & mask) | (cur & ~mask);
        c->fpu.cw = abstractToCw(v, c->fpu.cw);
        const uint32_t rc = (c->fpu.cw >> 10) & 3;
        c->mxcsr = (c->mxcsr & ~0x6000u) | (rc << 13);
    }
    retCdecl(c, cwToAbstract(c->fpu.cw));
}

// ============================================================================
// memory
// ============================================================================
IMPORT(R, malloc) { retCdecl(c, gmalloc(arg(c, 0))); }
IMPORT(R, calloc) { retCdecl(c, gcalloc(arg(c, 0), arg(c, 1))); }
IMPORT(R, realloc) { retCdecl(c, grealloc(arg(c, 0), arg(c, 1))); }
IMPORT(R, free) { gfree(arg(c, 0)); retCdecl(c, 0); }
IMPORTN(R, "??2@YAPAXI@Z", mangled_2) { retCdecl(c, gmalloc(arg(c, 0))); }
IMPORTN(R, "??_U@YAPAXI@Z", mangled_3) { retCdecl(c, gmalloc(arg(c, 0))); }
IMPORTN(R, "??3@YAXPAX@Z", mangled_4) { gfree(arg(c, 0)); retCdecl(c, 0); }
IMPORTN(R, "??_V@YAXPAX@Z", mangled_5) { gfree(arg(c, 0)); retCdecl(c, 0); }
IMPORT(R, _errno) {
    if (!t_crt.errnoAddr) t_crt.errnoAddr = gcalloc(1, 4);
    retCdecl(c, t_crt.errnoAddr);
}
void setErrno(int e) {
    if (!t_crt.errnoAddr) t_crt.errnoAddr = gcalloc(1, 4);
    wr32(t_crt.errnoAddr, static_cast<uint32_t>(e));
}

// ============================================================================
// strings: pointers are identity-mapped, so host functions work in place
// ============================================================================
size_t h_strlen(const char* s) { return std::strlen(s); }
const char* h_strchr(const char* s, int ch) { return std::strchr(s, ch); }
const char* h_strstr(const char* a, const char* b) { return std::strstr(a, b); }
const void* h_memchr(const void* p, int ch, uint32_t n) { return std::memchr(p, ch, n); }
const wchar_t* h_wcschr(const wchar_t* s, wchar_t ch) { return std::wcschr(s, ch); }
const wchar_t* h_wcsrchr(const wchar_t* s, wchar_t ch) { return std::wcsrchr(s, ch); }
const wchar_t* h_wcsstr(const wchar_t* a, const wchar_t* b) { return std::wcsstr(a, b); }
char* h_strncat(char* d, const char* s, uint32_t n) { return std::strncat(d, s, n); }
char* h_strncpy(char* d, const char* s, uint32_t n) { return std::strncpy(d, s, n); }
int h_strncmp(const char* a, const char* b, uint32_t n) { return std::strncmp(a, b, n); }
void* h_memmove(void* d, const void* s, uint32_t n) { return std::memmove(d, s, n); }
int h_strnicmp(const char* a, const char* b, uint32_t n) { return _strnicmp(a, b, n); }
wchar_t* h_wcsncpy(wchar_t* d, const wchar_t* s, uint32_t n) { return std::wcsncpy(d, s, n); }
int h_wcsncmp(const wchar_t* a, const wchar_t* b, uint32_t n) { return std::wcsncmp(a, b, n); }
uint32_t h_wcslen(const wchar_t* s) { return static_cast<uint32_t>(std::wcslen(s)); }
int h_toupper(int ch) { return (ch >= 'a' && ch <= 'z') ? ch - 32 : ch; }
int h_tolower(int ch) { return (ch >= 'A' && ch <= 'Z') ? ch + 32 : ch; }
int h_isspace(int ch) { return ch >= 0 && ch < 256 ? std::isspace(ch) : 0; }
int h_isalpha(int ch) { return ch >= 0 && ch < 256 ? std::isalpha(ch) : 0; }
int h_isdigit(int ch) { return ch >= '0' && ch <= '9' ? 4 : 0; }
int h_iswalpha(uint32_t ch) { return std::iswalpha(static_cast<wint_t>(ch & 0xFFFF)); }
int h_iswdigit(uint32_t ch) { return std::iswdigit(static_cast<wint_t>(ch & 0xFFFF)); }
int h_iswspace(uint32_t ch) { return std::iswspace(static_cast<wint_t>(ch & 0xFFFF)); }
int h_atoi(const char* s) { return std::atoi(s); }
int h_atol(const char* s) { return static_cast<int>(std::atol(s)); }
char* h_itoa(int v, char* b, int r) { return _itoa(v, b, r); }
wchar_t* h_itow(int v, wchar_t* b, int r) { return _itow(v, b, r); }
const unsigned char* h_mbspbrk(const unsigned char* a, const unsigned char* b) { return _mbspbrk(a, b); }
const unsigned char* h_mbsrchr(const unsigned char* a, uint32_t ch) { return _mbsrchr(a, ch); }
const unsigned char* h_mbschr(const unsigned char* a, uint32_t ch) { return _mbschr(a, ch); }
const unsigned char* h_mbsstr(const unsigned char* a, const unsigned char* b) { return _mbsstr(a, b); }
uint32_t h_mbsspn(const unsigned char* a, const unsigned char* b) { return static_cast<uint32_t>(_mbsspn(a, b)); }
uint32_t h_mbscspn(const unsigned char* a, const unsigned char* b) { return static_cast<uint32_t>(_mbscspn(a, b)); }
int h_mbsnbcmp(const unsigned char* a, const unsigned char* b, uint32_t n) { return _mbsnbcmp(a, b, n); }
const unsigned char* h_mbsinc(const unsigned char* a) { return _mbsinc(a); }
uint32_t h_mbclen(const unsigned char* a) { return static_cast<uint32_t>(_mbclen(a)); }
int h_ismbcdigit(uint32_t ch) { return _ismbcdigit(ch); }
int h_ismbcspace(uint32_t ch) { return _ismbcspace(ch); }
int h_ismbblead(uint32_t ch) { return _ismbblead(ch); }

#define CRT_AS(name, fn) static ::host::AutoReg HOST_CAT(reg_, __COUNTER__)(R, name, &::host::cdeclThunk<&fn>)
CRT_AS("strchr", h_strchr);
CRT_AS("strstr", h_strstr);
CRT_AS("memchr", h_memchr);
CRT_AS("wcschr", h_wcschr);
CRT_AS("wcsrchr", h_wcsrchr);
CRT_AS("wcsstr", h_wcsstr);
CRT_AS("strncat", h_strncat);
CRT_AS("strncpy", h_strncpy);
CRT_AS("strncmp", h_strncmp);
CRT_AS("memmove", h_memmove);
CRT_AS("_strnicmp", h_strnicmp);
CRT_AS("wcsncpy", h_wcsncpy);
CRT_AS("wcsncmp", h_wcsncmp);
CRT_AS("wcslen", h_wcslen);
CRT_AS("toupper", h_toupper);
CRT_AS("tolower", h_tolower);
CRT_AS("isspace", h_isspace);
CRT_AS("isalpha", h_isalpha);
CRT_AS("isdigit", h_isdigit);
CRT_AS("iswalpha", h_iswalpha);
CRT_AS("iswdigit", h_iswdigit);
CRT_AS("iswspace", h_iswspace);
CRT_AS("atoi", h_atoi);
CRT_AS("atol", h_atol);
CRT_AS("_itoa", h_itoa);
CRT_AS("_itow", h_itow);
CRT_AS("_mbspbrk", h_mbspbrk);
CRT_AS("_mbsrchr", h_mbsrchr);
CRT_AS("_mbschr", h_mbschr);
CRT_AS("_mbsstr", h_mbsstr);
CRT_AS("_mbsspn", h_mbsspn);
CRT_AS("_mbscspn", h_mbscspn);
CRT_AS("_mbsnbcmp", h_mbsnbcmp);
CRT_AS("_mbsinc", h_mbsinc);
CRT_AS("_mbclen", h_mbclen);
CRT_AS("_ismbcdigit", h_ismbcdigit);
CRT_AS("_ismbcspace", h_ismbcspace);
CRT_AS("_ismbblead", h_ismbblead);

int h_stricmp(const char* a, const char* b) { return _stricmp(a, b); }
char* h_strupr(char* s) { for (char* p = s; *p; ++p) *p = static_cast<char>(h_toupper(static_cast<unsigned char>(*p))); return s; }
char* h_strlwr(char* s) { for (char* p = s; *p; ++p) *p = static_cast<char>(h_tolower(static_cast<unsigned char>(*p))); return s; }
char* h_strrev(char* s) { return _strrev(s); }
wchar_t* h_wcsupr(wchar_t* s) { return _wcsupr(s); }
int h_wcsicmp(const wchar_t* a, const wchar_t* b) { return _wcsicmp(a, b); }
wchar_t* h_wcscpy(wchar_t* d, const wchar_t* s) { return std::wcscpy(d, s); }
wchar_t* h_wcscat(wchar_t* d, const wchar_t* s) { return std::wcscat(d, s); }
int h_wcscmp(const wchar_t* a, const wchar_t* b) { return std::wcscmp(a, b); }
CRT_AS("_stricmp", h_stricmp);
CRT_AS("_strupr", h_strupr);
CRT_AS("_strlwr", h_strlwr);
CRT_AS("_strrev", h_strrev);
CRT_AS("_wcsupr", h_wcsupr);
CRT_AS("_wcsicmp", h_wcsicmp);
CRT_AS("wcscpy", h_wcscpy);
CRT_AS("wcscat", h_wcscat);
CRT_AS("wcscmp", h_wcscmp);

// ============================================================================
// formatted I/O
// ============================================================================
IMPORT(R, sprintf) {
    uint32_t ap = c->esp + 12;
    const std::string s = formatA(argp(c, 1), ap);
    std::memcpy(argp(c, 0), s.c_str(), s.size() + 1);
    retCdecl(c, static_cast<uint32_t>(s.size()));
}
IMPORT(R, vsprintf) {
    uint32_t ap = arg(c, 2);
    const std::string s = formatA(argp(c, 1), ap);
    std::memcpy(argp(c, 0), s.c_str(), s.size() + 1);
    retCdecl(c, static_cast<uint32_t>(s.size()));
}
IMPORT(R, _snprintf) {
    uint32_t ap = c->esp + 16;
    const std::string s = formatA(argp(c, 2), ap);
    retCdecl(c, static_cast<uint32_t>(copyTruncA(argp(c, 0), arg(c, 1), s)));
}
IMPORT(R, _vsnprintf) {
    uint32_t ap = arg(c, 3);
    const std::string s = formatA(argp(c, 2), ap);
    retCdecl(c, static_cast<uint32_t>(copyTruncA(argp(c, 0), arg(c, 1), s)));
}
IMPORT(R, _snwprintf) {
    uint32_t ap = c->esp + 16;
    const wstring s = formatW(argp<wchar_t>(c, 2), ap);
    retCdecl(c, static_cast<uint32_t>(copyTruncW(argp<wchar_t>(c, 0), arg(c, 1), s)));
}
IMPORT(R, _vsnwprintf) {
    uint32_t ap = arg(c, 3);
    const wstring s = formatW(argp<wchar_t>(c, 2), ap);
    retCdecl(c, static_cast<uint32_t>(copyTruncW(argp<wchar_t>(c, 0), arg(c, 1), s)));
}
IMPORT(R, printf) {
    uint32_t ap = c->esp + 8;
    const std::string s = formatA(argp(c, 0), ap);
    log("[guest printf] %s", s.c_str());
    retCdecl(c, static_cast<uint32_t>(s.size()));
}
IMPORT(R, puts) {
    log("[guest puts] %s", argp(c, 0));
    retCdecl(c, 0);
}
IMPORT(R, perror) {
    log("[guest perror] %s", arg(c, 0) ? argp(c, 0) : "");
    retCdecl(c, 0);
}
IMPORT(R, sscanf) { retCdecl(c, static_cast<uint32_t>(scanA(argp(c, 0), argp(c, 1), c->esp + 12))); }

IMPORT(R, fopen) {
    FILE* f = std::fopen(argp(c, 0), argp(c, 1));
    HLOG(1, "fopen(%s, %s) -> %s", argp(c, 0), argp(c, 1), f ? "ok" : "fail");
    retCdecl(c, f ? fileToGuest(f) : 0);
}
IMPORT(R, _wfopen) {
    FILE* f = _wfopen(argp<wchar_t>(c, 0), argp<wchar_t>(c, 1));
    HLOG(1, "_wfopen(%s) -> %s", narrow(argp<wchar_t>(c, 0)).c_str(), f ? "ok" : "fail");
    retCdecl(c, f ? fileToGuest(f) : 0);
}
IMPORT(R, fclose) {
    FILE* f = fileFromGuest(arg(c, 0));
    int r = f ? std::fclose(f) : -1;
    fileForget(arg(c, 0));
    retCdecl(c, static_cast<uint32_t>(r));
}
IMPORT(R, fprintf) {
    uint32_t ap = c->esp + 12;
    const std::string s = formatA(argp(c, 1), ap);
    FILE* f = fileFromGuest(arg(c, 0));
    if (f) std::fwrite(s.data(), 1, s.size(), f);
    else log("[guest fprintf] %s", s.c_str());
    retCdecl(c, static_cast<uint32_t>(s.size()));
}

// ============================================================================
// math (x87 results in ST0)
// ============================================================================
IMPORT(R, ldexp) { retCdeclF(c, std::ldexp(rdf64(c->esp + 4), static_cast<int32_t>(rd32(c->esp + 12)))); }
IMPORT(R, frexp) {
    int e = 0;
    const double m = std::frexp(rdf64(c->esp + 4), &e);
    wr32(rd32(c->esp + 12), static_cast<uint32_t>(e));
    retCdeclF(c, m);
}
IMPORT(R, ceil) { retCdeclF(c, std::ceil(rdf64(c->esp + 4))); }
IMPORT(R, floor) { retCdeclF(c, std::floor(rdf64(c->esp + 4))); }
IMPORT(R, _isnan) { retCdecl(c, std::isnan(rdf64(c->esp + 4)) ? 1 : 0); }
// _CI* helpers take their operands on the x87 stack.
IMPORT(R, _CIasin) { const double x = fpop(c); fpush(c, std::asin(x)); c->esp += 4; }
IMPORT(R, _CIacos) { const double x = fpop(c); fpush(c, std::acos(x)); c->esp += 4; }
IMPORT(R, _CItanh) { const double x = fpop(c); fpush(c, std::tanh(x)); c->esp += 4; }
IMPORT(R, _CIpow) { const double y = fpop(c); const double x = fpop(c); fpush(c, std::pow(x, y)); c->esp += 4; }
IMPORT(R, _CIfmod) { const double y = fpop(c); const double x = fpop(c); fpush(c, std::fmod(x, y)); c->esp += 4; }
IMPORT(R, rand) {
    t_crt.holdrand = t_crt.holdrand * 214013u + 2531011u;
    retCdecl(c, (t_crt.holdrand >> 16) & 0x7FFF);
}
IMPORT(R, time) {
    const uint32_t t = static_cast<uint32_t>(std::time(nullptr));
    if (arg(c, 0)) wr32(arg(c, 0), t);
    retCdecl(c, t);
}

// qsort: the VC7.1 algorithm, so equal elements end up in the same order as retail.
void gswap(uint32_t a, uint32_t b, uint32_t w) {
    if (a == b) return;
    uint8_t* p = gp<uint8_t>(a);
    uint8_t* q = gp<uint8_t>(b);
    while (w--) { const uint8_t t = *p; *p++ = *q; *q++ = t; }
}
IMPORT(R, qsort) {
    const uint32_t base = arg(c, 0), num = arg(c, 1), width = arg(c, 2), cmp = arg(c, 3);
    auto comp = [&](uint32_t a, uint32_t b) { return static_cast<int32_t>(guestCall(cmp, {a, b})); };
    if (num >= 2 && width) {
        uint32_t lostk[30], histk[30];
        int stkptr = 0;
        uint32_t lo = base, hi = base + width * (num - 1);
        for (;;) {
            const uint32_t size = (hi - lo) / width + 1;
            if (size <= 8) {
                uint32_t h = hi;
                while (h > lo) {
                    uint32_t mx = lo;
                    for (uint32_t p = lo + width; p <= h; p += width)
                        if (comp(p, mx) > 0) mx = p;
                    gswap(mx, h, width);
                    h -= width;
                }
            } else {
                const uint32_t mid = lo + (size / 2) * width;
                gswap(mid, lo, width);
                uint32_t loguy = lo, higuy = hi + width;
                for (;;) {
                    do { loguy += width; } while (loguy <= hi && comp(loguy, lo) <= 0);
                    do { higuy -= width; } while (higuy > lo && comp(higuy, lo) >= 0);
                    if (higuy < loguy) break;
                    gswap(loguy, higuy, width);
                }
                gswap(lo, higuy, width);
                if (static_cast<int64_t>(higuy) - 1 - lo >= static_cast<int64_t>(hi) - loguy) {
                    if (lo + width < higuy) { lostk[stkptr] = lo; histk[stkptr] = higuy - width; ++stkptr; }
                    if (loguy < hi) { lo = loguy; continue; }
                } else {
                    if (loguy < hi) { lostk[stkptr] = loguy; histk[stkptr] = hi; ++stkptr; }
                    if (lo + width < higuy) { hi = higuy - width; continue; }
                }
            }
            if (--stkptr < 0) break;
            lo = lostk[stkptr];
            hi = histk[stkptr];
        }
    }
    retCdecl(c, 0);
}

// ============================================================================
// threads, files, directories
// ============================================================================
IMPORT(R, _beginthread) {
    DWORD tid = 0;
    HANDLE h = startGuestThread(arg(c, 0), arg(c, 2), arg(c, 1), 0, &tid);
    HLOG(1, "_beginthread(0x%08X) -> tid %lu", arg(c, 0), tid);
    retCdecl(c, h ? gh(h) : 0xFFFFFFFFu);
}

IMPORT(R, _wfindfirst) {
    _wfinddata32_t fd;
    const intptr_t h = _wfindfirst32(argp<wchar_t>(c, 0), &fd);
    if (h == -1) { retCdecl(c, 0xFFFFFFFFu); return; }
    std::memcpy(argp(c, 1), &fd, sizeof fd);
    std::lock_guard<std::mutex> l(g_findLock);
    g_finds.push_back(h);
    retCdecl(c, static_cast<uint32_t>(g_finds.size()));  // 1-based token
}
IMPORT(R, _wfindnext) {
    intptr_t h;
    {
        std::lock_guard<std::mutex> l(g_findLock);
        const uint32_t i = arg(c, 0);
        h = (i >= 1 && i <= g_finds.size()) ? g_finds[i - 1] : -1;
    }
    _wfinddata32_t fd;
    const int r = h == -1 ? -1 : _wfindnext32(h, &fd);
    if (r == 0) std::memcpy(argp(c, 1), &fd, sizeof fd);
    retCdecl(c, static_cast<uint32_t>(r));
}
IMPORT(R, _findclose) {
    std::lock_guard<std::mutex> l(g_findLock);
    const uint32_t i = arg(c, 0);
    int r = -1;
    if (i >= 1 && i <= g_finds.size() && g_finds[i - 1] != -1) { r = _findclose(g_finds[i - 1]); g_finds[i - 1] = -1; }
    retCdecl(c, static_cast<uint32_t>(r));
}
int h_getdrive() { return _getdrive(); }
int h_chdrive(int d) { return _chdrive(d); }
int h_wmkdir(const wchar_t* p) { return _wmkdir(p); }
int h_wchdir(const wchar_t* p) { return _wchdir(p); }
void h_wmakepath(wchar_t* p, const wchar_t* d, const wchar_t* dir, const wchar_t* f, const wchar_t* e) { _wmakepath(p, d, dir, f, e); }
void h_splitpath(const char* p, char* d, char* dir, char* f, char* e) { _splitpath(p, d, dir, f, e); }
void h_wsplitpath(const wchar_t* p, wchar_t* d, wchar_t* dir, wchar_t* f, wchar_t* e) { _wsplitpath(p, d, dir, f, e); }
CRT_AS("_getdrive", h_getdrive);
CRT_AS("_chdrive", h_chdrive);
CRT_AS("_wmkdir", h_wmkdir);
CRT_AS("_wchdir", h_wchdir);
CRT_AS("_wmakepath", h_wmakepath);
CRT_AS("_splitpath", h_splitpath);
CRT_AS("_wsplitpath", h_wsplitpath);
IMPORT(R, _wgetcwd) {
    wchar_t buf[MAX_PATH * 2];
    if (!_wgetcwd(buf, MAX_PATH * 2)) { retCdecl(c, 0); return; }
    uint32_t dst = arg(c, 0);
    const uint32_t n = arg(c, 1);
    const uint32_t len = static_cast<uint32_t>(wcslen(buf)) + 1;
    if (!dst) dst = gmalloc(2 * (n > len ? n : len));
    else if (len > n) { setErrno(ERANGE); retCdecl(c, 0); return; }
    std::memcpy(gp(dst), buf, len * 2);
    retCdecl(c, dst);
}

// ============================================================================
// C++ runtime classes (VC7.1 layouts)
// ============================================================================
// std::exception: { vftable, const char* _m_what, int _m_doFree }
uint32_t g_excVtbl;
void exceptionDtorBody(uint32_t self) {
    if (rd32(self + 8)) gfree(rd32(self + 4));
    wr32(self + 4, 0);
    wr32(self + 8, 0);
}
void excScalarDtor(Ctx* c) {  // thiscall (ecx), stack: flags
    const uint32_t self = c->ecx;
    exceptionDtorBody(self);
    if (arg(c, 0) & 1) gfree(self);
    retStd(c, self, 1);
}
void excWhat(Ctx* c) {
    const uint32_t w = rd32(c->ecx + 4);
    retStd(c, w ? w : gstrdup("Unknown exception"), 0);
}
uint32_t excVtbl() {
    if (!g_excVtbl) {
        g_excVtbl = gcalloc(3, 4);
        wr32(g_excVtbl, addTrap("std::exception::`scalar deleting destructor'", excScalarDtor));
        wr32(g_excVtbl + 4, addTrap("std::exception::what", excWhat));
    }
    return g_excVtbl;
}
IMPORTN(R, "??0exception@@QAE@XZ", mangled_6) {
    const uint32_t self = c->ecx;
    wr32(self, excVtbl());
    wr32(self + 4, 0);
    wr32(self + 8, 0);
    retStd(c, self, 0);
}
IMPORTN(R, "??0exception@@QAE@ABV0@@Z", mangled_7) {
    const uint32_t self = c->ecx, other = arg(c, 0);
    wr32(self, excVtbl());
    if (rd32(other + 8)) {
        wr32(self + 4, gstrdup(gp<char>(rd32(other + 4))));
        wr32(self + 8, 1);
    } else {
        wr32(self + 4, rd32(other + 4));
        wr32(self + 8, 0);
    }
    retStd(c, self, 1);
}
IMPORTN(R, "??1exception@@UAE@XZ", mangled_8) {
    exceptionDtorBody(c->ecx);
    retStd(c, 0, 0);
}
IMPORTN(R, "??1type_info@@UAE@XZ", mangled_9) { retStd(c, 0, 0); }

// std::basic_string<char> (VC7.1): { _Alval(4), union{char _Buf[16]; char* _Ptr;}, _Mysize, _Myres }
void strInit(uint32_t self, const char* s, uint32_t n) {
    wr32(self + 0, 0);
    uint32_t res = 15;
    if (n > 15) {
        res = n | 15;
        const uint32_t p = gmalloc(res + 1);
        std::memcpy(gp(p), s, n);
        wr8(p + n, 0);
        wr32(self + 4, p);
    } else {
        std::memcpy(gp(self + 4), s, n);
        wr8(self + 4 + n, 0);
    }
    wr32(self + 20, n);
    wr32(self + 24, res);
}
IMPORTN(P, "??0?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAE@PBD@Z", mangled_10) {
    const char* s = argp(c, 0);
    strInit(c->ecx, s, static_cast<uint32_t>(std::strlen(s)));
    retStd(c, c->ecx, 1);
}
IMPORTN(P, "??0?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAE@ABV01@@Z", mangled_11) {
    const uint32_t o = arg(c, 0);
    const uint32_t n = rd32(o + 20);
    const uint32_t data = rd32(o + 24) >= 16 ? rd32(o + 4) : o + 4;
    strInit(c->ecx, gp<char>(data), n);
    retStd(c, c->ecx, 1);
}
IMPORTN(P, "??1?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAE@XZ", mangled_12) {
    const uint32_t self = c->ecx;
    if (rd32(self + 24) >= 16) gfree(rd32(self + 4));
    wr32(self + 24, 15);
    wr32(self + 20, 0);
    wr8(self + 4, 0);
    retStd(c, 0, 0);
}

}  // namespace
}  // namespace host
