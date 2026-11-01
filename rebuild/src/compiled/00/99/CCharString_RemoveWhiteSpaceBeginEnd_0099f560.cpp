// CCharString::RemoveWhiteSpaceBeginEnd() -- strip the leading, then the trailing white space.
// Leaf: ?RemoveWhiteSpaceBeginEnd@CCharString@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0099F560. Ego_r / FableWin.pdb name (chunk 685); the catalog's CCharString::Trim / Dest_val
//   were guesses (naming wave 19). Callees: ?RemoveWhiteSpaceBegin@CCharString@@QAEXXZ 0x0099F450, ?RemoveWhiteSpaceEnd@CCharString@@QAEXXZ
//   0x0099F4D0 (FableWin.pdb; the chunk 314 landed-names label of this row already named the second).
class CCharString { public:
    void RemoveWhiteSpaceBegin();      // 0x0099F450
    void RemoveWhiteSpaceEnd();        // 0x0099F4D0
    void RemoveWhiteSpaceBeginEnd();   // 0x0099F560
};
void CCharString::RemoveWhiteSpaceBeginEnd()
{
    RemoveWhiteSpaceBegin();
    RemoveWhiteSpaceEnd();
}
