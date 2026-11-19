// CDefString::operator CCharString() const -- the def string's text: forwards to GetString, whose inline body (the global def-string
//   table's string at TablePos) is expanded here
// Leaf: ??BCDefString@@QBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00415DD0, 25 bytes, thiscall `ret 4` (hidden CCharString result): the same bytes as GetString 0x00415D70
//   (`mov eax,[ecx] ; push esi ; mov esi,[esp+8] ; push eax ; push esi ; mov ecx,offset GDefStringTable ; call 0x009D49B0 ; mov eax,esi ;
//   pop esi ; ret 4`).
// Identity (owner decision 2026-11-19, "Swap and re-land"; hand1 lane, Fable advisor fable3): the conversion operator is the later of the two
//   identical bodies because VC7.1 /O2 emits the inline callee GetString first (full evidence in CDefString_GetString_00415d70.cpp);
//   Ego_r.pdb / FableWin.pdb ??BCDefString@@QBE?AVCCharString@@XZ. 49 retail call sites in the game TUs call it out of line for an implicit
//   conversion temporary. Was ?GetString@CDefString@@QBE?AVCCharString@@XZ (naming wave 13), and before that the conversion (wave 1).
// Shape: the forwarder's body is engine/CDefString.h's (`return GetString();`), with GetString expanded; the result is constructed straight
//   into the hidden result (passed through to CDefStringTable::GetString), `this` is not kept.
class CCharString {
public:
    void* PStringData;                             // +0
};

class CDefStringTable {
public:
    CCharString GetString(long index) const;       // 0x009D49B0
};
extern CDefStringTable GDefStringTable;            // 0x013CA828 ?GDefStringTable@@3VCDefStringTable@@A (ESTABLISHED; naming wave 21)

class CDefString {
public:
    long TablePos;                                 // +0x00 (Ego_r.pdb CDefString, 4 bytes)
    operator CCharString() const;
    CCharString GetString() const { return GDefStringTable.GetString(TablePos); }
};

CDefString::operator CCharString() const
{
    return GetString();
}
