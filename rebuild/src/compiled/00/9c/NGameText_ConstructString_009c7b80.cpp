// NGameText::ConstructString(CWideString& output, long a, long b) -- replace the %1 / %2 placeholders of a game-text string with two numbers
// Leaf: ?ConstructString@NGameText@@YI_NAAVCWideString@@JJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C7B80, __fastcall (output in ecx, a in edx, b on the stack, `ret 4`):
//   `mov eax,[esp+4] ; push -1 ; push eax ; push edx ; push ecx ; call 0x009C7A60 ; add esp,10h ; ret 4`.
// Name: Ego_r.pdb 0x00B15980 + FableWin.pdb 0x02F6EBB0 `?ConstructString@NGameText@@YI_NAAVCWideString@@JJ@Z` (the two-long overload;
//   the landed 0x009C7BA0 is the three-long overload in the same form).
// Callee: NGameText::ConstructStringInternal(CWideString&, long, ...) @ 0x009C7A60 (__cdecl varargs; the -1 terminates the number list).
// Naming (NF146, 2026-11-20, q4285): was the made-up extern "C" FableConstructString_9c7b80 calling the FableStringCtor_9c7b80 stand-in for
//   0x009C7A60; now the PDB overload calling the PDB ConstructStringInternal, so 0x009C7A60 ends on one spelling. Own leaf was
//   @FableConstructString_9c7b80@12.

class CWideString;

namespace NGameText {

bool __cdecl ConstructStringInternal(CWideString& output, long first, ...);     // 0x009C7A60

bool __fastcall ConstructString(CWideString& output, long a, long b)
{
    return ConstructStringInternal(output, a, b, -1);
}

}
