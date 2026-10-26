// CUserProfileManager::GetDebugSaveDir() const -- the debug save directory: L"Data\\" prefixed to NResourceDirectoryNames::GetSaveDir()
// Leaf: ?GetDebugSaveDir@CUserProfileManager@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00406780, 44 bytes (+4 int3 pad). Callees:
// NResourceDirectoryNames::GetSaveDir 0x0041A540 (__fastcall, hidden result in ecx: My Documents +
// L"My Games\\Fable\\" + L"Saves" + L"\\"), operator+(const wchar_t*, const CWideString&) 0x0099BFF0
// (__fastcall: hidden result ecx, the literal edx, the string on the stack), ~CWideString
// 0x0099B510; literal L"Data\\" 0x0122DEC8.
//
// Retail bytes:  push ecx ; push esi ; lea ecx,[tmp] ; call GetSaveDir ; mov esi,[result] ; push eax ;
//   mov edx,L"Data\\" ; mov ecx,esi ; call operator+ ; lea ecx,[tmp] ; call ~CWideString ;
//   mov eax,esi ; pop esi ; pop ecx ; ret 4
// A const __fastcall member that never reads `this` (ecx is overwritten at once; the hidden result
// is its one stack argument -> `ret 4`). Identity (manifest label ConvertPathToFullPath rejected:
// every CAFile::ConvertPathToFullPath overload takes a path argument): the only caller 0x0041A1A0
// is a __fastcall no-argument CWideString getter that returns `CUserProfileManager::Get()` (0x0040D2A0)
// `.this_function()`; FableWin.pdb has exactly one such pair, `?GetDebugSaveDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ`
// and `?GetDebugSaveDir@CUserProfileManager@@QBE?AVCWideString@@XZ`. The temporary save-dir string
// is the right operand, built first and destroyed after the concatenation.
typedef unsigned short wchar16;                   // VC7.1 default: wchar_t is not a native type

class CWideStringData;

class CWideString {
public:
    ~CWideString();                               // 0x0099B510
private:
    CWideStringData* PStringData;
};

CWideString __fastcall operator+(const wchar16* left, const CWideString& right);   // 0x0099BFF0

namespace NResourceDirectoryNames {
    CWideString __fastcall GetSaveDir();          // 0x0041A540
}

class CUserProfileManager {
public:
    CWideString GetDebugSaveDir() const;
};

CWideString CUserProfileManager::GetDebugSaveDir() const
{
    return L"Data\\" + NResourceDirectoryNames::GetSaveDir();
}