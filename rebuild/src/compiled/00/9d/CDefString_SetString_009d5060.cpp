// CDefString::SetString(const CCharString& string) -- store the table position of the string added to the global def string table (CDefStringTable::AddString 0x009D4F90 on GDefStringTable 0x013CA828)
// Leaf: ?SetString@CDefString@@QAEXABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009D5060, 24 bytes, thiscall `ret 4`: `mov eax,[esp+4] ; push esi ; mov esi,ecx ; push eax ; mov ecx,offset GDefStringTable ; call AddString ; mov [esi],eax ; pop esi ; ret 4`.
// Naming (naming wave 21): the catalog's CDefString::SetString (no `mov eax,esi` epilogue: not the CCharString constructor ??0CDefString@@QAE@ABVCCharString@@@Z, which
//   returns `this`); the callee is the PDB ?AddString@CDefStringTable@@QAEJABVCCharString@@@Z (returns the position) on the established ?GDefStringTable@@3VCDefStringTable@@A.
//   Was a `Singleton::method` stand-in over g_singleton_13ca828 (the CRegion::SetMiniMapRegionExitTextOffsetX label is propagated).
class CCharString;
class CDefStringTable { public:
    long AddString(const CCharString& string);           // 0x009D4F90
};
extern CDefStringTable GDefStringTable;                 // 0x013CA828

class CDefString { public:
    long TablePos;                                      // +0x00
    void SetString(const CCharString& string);
};

void CDefString::SetString(const CCharString& string)
{
    TablePos = GDefStringTable.AddString(string);
}
