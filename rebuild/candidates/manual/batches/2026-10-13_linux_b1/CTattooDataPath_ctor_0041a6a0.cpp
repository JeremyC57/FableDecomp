// CTattooDataPath constructor (CWideString L"Data\\Tattoos\\")
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0041A6A0, oracle length 17 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  push esi ; push offset L"Data\Tattoos\" ; mov esi,ecx ; call CWideString::CWideString(wchar_t const*) ; mov eax,esi ; pop esi ; ret
// Manifest label `CUserProfileManager::GetAutoSaveFileName` is propagated across 20 bodies and
// is NOT trusted; the bytes construct a CWideString at `this` from the wide literal at
// 0x0122F578 ("Data\Tattoos\", the tattoo asset directory) and return `this`: a constructor of
// a wide-string path tag. CWideString::CWideString(wchar_t const*) @ 0x0099B6B0 is a trusted
// (landed, PASS) identity. Shape template: CUserProfileManager_GetAutoSaveFileName_0041a060.cpp.

struct CWideString {
    void* PStringData;                              // +0x00 CWideString::PStringData
    CWideString(const wchar_t* text);               // 0x0099B6B0
};

struct CTattooDataPath {
    CWideString Path;                               // +0x00
    CTattooDataPath();
};

CTattooDataPath::CTattooDataPath()
    : Path(L"Data\\Tattoos\\")
{
}
