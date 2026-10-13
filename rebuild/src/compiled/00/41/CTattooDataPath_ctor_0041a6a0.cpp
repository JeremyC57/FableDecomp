// CTattooDataPath constructor (CWideString L"Data\\Tattoos\\")
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0041A6A0, oracle length 17 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  push esi ; push offset L"Data\Tattoos\" ; mov esi,ecx ; call CWideString::CWideString(wchar_t const*) ; mov eax,esi ; pop esi ; ret
// Manifest label `CUserProfileManager::GetAutoSaveFileName` is propagated across 20 bodies and
// is NOT trusted; the bytes construct a CWideString at `this` from the wide literal at
// 0x0122F578 ("Data\Tattoos\", the tattoo asset directory) and return `this`: a constructor of
// a wide-string path tag. CWideString::CWideString(wchar_t const*) @ 0x0099B6B0 is a trusted
// (landed, PASS) identity. Shape template: CUserProfileManager_GetAutoSaveFileName_0041a060.cpp.
//
// Lane feedback: The local CWideString stub could not take the L"" literal (unsigned short[] under VC7.1);
// using the shared fable_string.h declaration gives RELOCATION_MATCH.
#include "fable_string.h"      // CWideString with the landed CWideString(const wchar_t*) @ 0x0099B6B0

struct CTattooDataPath {
    CWideString Path;                               // +0x00
    CTattooDataPath();
};

CTattooDataPath::CTattooDataPath()
    : Path(L"Data\\Tattoos\\")
{
}