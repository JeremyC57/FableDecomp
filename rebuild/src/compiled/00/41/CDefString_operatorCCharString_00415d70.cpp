// CDefString::operator CCharString() const -- the def string's text: CDefStringTable::GetString(TablePos) (0x009D49B0) on GDefStringTable 0x013CA828, returned through the hidden result pointer
// Leaf: ??BCDefString@@QBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00415D70, 25 bytes, thiscall `ret 4` (hidden result): `mov eax,[ecx] ; push esi ; mov esi,[esp+8] ; push eax ; push esi ; mov ecx,offset GDefStringTable ; call GetString ; mov eax,esi ; pop esi ; ret 4`.
// Naming (naming wave 21): the PDB ??BCDefString@@QBE?AVCCharString@@XZ (Ego_r / FableWin), the callee the PDB ?GetString@CDefStringTable@@QBE?AVCCharString@@J@Z on the
//   established ?GDefStringTable@@3VCDefStringTable@@A. Was a `CCharString::AssignTo` / `Pool::Assign` stand-in over g_pool.
class CCharString { public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30 (not called here: the callee's result lands in the caller's slot)
    ~CCharString();                                     // 0x0099EAE0
};
class CDefStringTable { public:
    CCharString GetString(long tablePos) const;        // 0x009D49B0
};
extern CDefStringTable GDefStringTable;                 // 0x013CA828

class CDefString { public:
    long TablePos;                                      // +0x00
    operator CCharString() const;
};

CDefString::operator CCharString() const
{
    return GDefStringTable.GetString(TablePos);
}
