// NGameText::ConstructString(CWideString& output, long a) -- replace the %1 placeholder of a game-text string with one number
// Leaf: ?ConstructString@NGameText@@YI_NAAVCWideString@@J@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C7B70, 16 bytes, __fastcall (output in ecx, a in edx, plain `ret`).
// Name: Ego_r.pdb 0x00B15970 + FableWin.pdb 0x02F6EB80 `?ConstructString@NGameText@@YI_NAAVCWideString@@J@Z` (one of the six overloads;
//   the one-long overload: retail pushes -1, a, output); the landed 0x009C7BA0 is the three-long overload in the same form.
// Callee: NGameText::ConstructStringInternal(CWideString&, long, ...) @ 0x009C7A60 (__cdecl varargs; the -1 terminates the number list).
// Naming (NF146, 2026-11-20, q4285): was the made-up `void __fastcall ConstructString(void*, void*)` calling a ConstructStringHelper
//   stand-in for 0x009C7A60; now the PDB overload calling the PDB ConstructStringInternal (the 0x009C7BA0 form), so 0x009C7A60 ends on one
//   spelling. Own leaf was ?ConstructString@@YIXPAX0@Z.

class CWideString;

namespace NGameText {

bool __cdecl ConstructStringInternal(CWideString& output, long first, ...);     // 0x009C7A60

bool __fastcall ConstructString(CWideString& output, long a)
{
    return ConstructStringInternal(output, a, -1);
}

}
