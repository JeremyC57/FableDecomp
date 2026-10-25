// CDefString::operator CCharString() const -- the def string's text: the global def-string table's string at TablePos
// Leaf: ??BCDefString@@QBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00415DD0, 25 bytes, thiscall `ret 4` (hidden CCharString result).
// Identity: Ego_r.pdb `??BCDefString@@QBE?AVCCharString@@XZ` (name set; the catalog name is already this operator) and
//   struct_layouts_egor CDefString = { long TablePos; } (4 bytes): the body loads [this] = TablePos and returns
//   CDefStringTable::GetString(TablePos) const (Ego_r.pdb `?GetString@CDefStringTable@@QBE?AVCCharString@@J@Z`, landed
//   0x009D49B0) on the global table @ 0x013CA828 (Ego_r.pdb `GDefStringTable`; the landed TUs call it g_DefStringTable,
//   kept here so every TU binds one symbol). 49 callers.
// Fix (FINDINGS chunk 384): the landed model was a fake `CCharString::AssignTo(void*)` handing (dst, data) to a
//   placeholder `Pool::Assign`.
// Shape: the result is constructed straight into the hidden result (passed through to GetString), `this` is not kept.
class CCharString {
public:
    void* PStringData;                             // +0
};

class CDefStringTable {
public:
    CCharString GetString(long index) const;       // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;           // 0x013CA828 (Ego_r.pdb GDefStringTable)

class CDefString {
public:
    long TablePos;                                 // +0x00
    operator CCharString() const;
};

CDefString::operator CCharString() const
{
    return g_DefStringTable.GetString(TablePos);
}
