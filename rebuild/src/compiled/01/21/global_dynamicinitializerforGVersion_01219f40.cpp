// `dynamic initializer for 'GVersion''() -- construct the global game version GVersion = CVersion(25, 19, 5, 125) through the out-of-line ctor (trivial dtor: no atexit)
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01219F40, 19 bytes, .CRT$XCU entry 0x01374220: `push 7Dh ; push 5 ; push 13h ;
//   push 19h ; mov ecx,0x013B85E4 ; call 0x009EA080 ; ret`.
// Identity (proven): Ego_r.pdb (0x0100C2B0 GGame, 0x0100C2D0 GDebugString, 0x0100C310 GVersionString, 0x0100C330 GVersion) and
//   FableWin.pdb (0x03FDDAA0 GGame .. 0x03FDDB30 GVersion) list `dynamic initializer for 'GVersion'' after GGame and the version
//   string; retail's table runs 0x01219F00 GGame (0x013B83D0, ESTABLISHED), 0x01219F20 a CCharString "Unknown" at 0x013B85E0, then
//   this row at 0x013B85E4. 0x009EA080 is CVersion's ctor (Ego_r.pdb ??0CVersion@@QAE@EEKK@Z: two unsigned chars, two unsigned longs;
//   `ret 10h`, +1 = arg1, +0 = arg2, +4, +8); the object's two readers pass it as `const CVersion&` to CInputManager::StreamInputsToFile
//   0x009F4CF0 / StreamInputsFromFile 0x009F5E10 (PDB ?StreamInputsToFile@CInputManager@@QAEXABVCVersion@@ABVCWideString@@@Z).
// Layout: Ego_r.pdb CVersion (12 bytes): Month +0, Day +1, Year +4, Build +8. Parameter names descriptive (the PDB has types only).
class CVersion {
public:
    CVersion(unsigned char day, unsigned char month, unsigned long year, unsigned long build);   // 0x009EA080 ??0CVersion@@QAE@EEKK@Z
    unsigned char Month;                                  // +0x00 (second argument)
    unsigned char Day;                                    // +0x01 (first argument)
    unsigned long Year;                                   // +0x04
    unsigned long Build;                                  // +0x08
};

CVersion GVersion(25, 19, 5, 125);                        // 0x013B85E4