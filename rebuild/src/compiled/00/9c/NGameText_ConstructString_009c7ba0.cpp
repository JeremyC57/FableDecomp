// NGameText::ConstructString(CWideString& output, long a, long b, long c) -- replace the %1..%3 placeholders of a game-text string with three numbers
// Leaf: ?ConstructString@NGameText@@YI_NAAVCWideString@@JJJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C7BA0, 25 bytes, __fastcall (output in ecx, a in edx, b and c on
// the stack, `ret 8`).
// Name: FableWin.pdb `?ConstructString@NGameText@@YI_NAAVCWideString@@JJJ@Z` (one of six overloads: CWideString,
// vector<CWideString>, and one to four longs). Caller: TerminateMinigame 0x006DE280.
// Callee: NGameText::ConstructStringInternal(CWideString&, long, ...) @ 0x009C7A60 (landed, __cdecl varargs; the
// -1 terminates the number list).
// Shape: a plain forward: the arguments are pushed right to left (-1, c, b, a, output) and the bool result is
// returned, so the cdecl cleanup is `add esp,14h`.

class CWideString;

namespace NGameText {

bool __cdecl ConstructStringInternal(CWideString& output, long first, ...);     // 0x009C7A60

bool __fastcall ConstructString(CWideString& output, long a, long b, long c)
{
    return ConstructStringInternal(output, a, b, c, -1);
}

}