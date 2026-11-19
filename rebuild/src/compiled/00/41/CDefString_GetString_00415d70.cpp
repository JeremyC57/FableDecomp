// CDefString::GetString() const -- the def string's text: CDefStringTable::GetString(TablePos) (0x009D49B0) on GDefStringTable 0x013CA828, returned through the hidden result pointer
// Leaf: ?GetString@CDefString@@QBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00415D70, 25 bytes, thiscall `ret 4` (hidden result): `mov eax,[ecx] ; push esi ; mov esi,[esp+8] ; push eax ; push esi ; mov ecx,offset GDefStringTable ; call GetString ; mov eax,esi ; pop esi ; ret 4`.
// Identity (owner decision 2026-11-19, "Swap and re-land"; hand1 lane, Fable advisor fable3): this is GetString and the identical body at
//   0x00415DD0 is the conversion operator ??BCDefString@@QBE?AVCCharString@@XZ (the reverse of the naming wave 13 binding). Evidence:
//   - VC7.1 /O1 /O2 emit a callee before its inline caller: 112 of 112 small retail caller->callee pairs put the callee at the lower
//     address, while /Od (FableWin.pdb, source order: ??B 0x018C20E0 before GetString 0x018C2120) keeps declaration order. The
//     conversion forwards to GetString (engine/CDefString.h), so GetString is emitted first: GetString 0x00415D70, ToWideString
//     0x00415D90, ??B 0x00415DD0, as engine/CDefString.h compiles at /O2.
//   - 0x008E7040 CMeleeCombatAbilityDef::IsAbilityValid on the shared header in its natural spelling (`.GetString().GetLength()` and an
//     implicit conversion argument) matches only under this binding; 0x0066A9E0 CTCSoundPlayer::PlayCollisionSound (FableWin shape, plain
//     VC7.1 inline heuristics: the conversion expanded at four sites and called out of line at the fifth) likewise.
//   - Retail's callers split cleanly: the NScript TUs call 0x00415D70 (an expanded conversion forwarding to an out-of-line GetString),
//     the game TUs call 0x00415DD0 (the conversion itself out of line).
//   Was ??BCDefString@@QBE?AVCCharString@@XZ (naming wave 21; before that a `CCharString::AssignTo` / `Pool::Assign` stand-in over g_pool).
class CCharString { public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30 (not called here: the callee's result lands in the caller's slot)
    ~CCharString();                                     // 0x0099EAE0
};
class CDefStringTable { public:
    CCharString GetString(long tablePos) const;        // 0x009D49B0
};
extern CDefStringTable GDefStringTable;                 // 0x013CA828 ?GDefStringTable@@3VCDefStringTable@@A (ESTABLISHED; naming wave 21)

class CDefString { public:
    long TablePos;                                      // +0x00 (Ego_r.pdb CDefString, 4 bytes)
    CCharString GetString() const;
};

CCharString CDefString::GetString() const
{
    return GDefStringTable.GetString(TablePos);
}
