// CWideString::CWideString() -- the empty wide string: no data, one more live instance counted
// Leaf: ??0CWideString@@QAE@XZ
// Retail: Fable.exe @ 0x0099AED0, 15 bytes, thiscall plain `ret`, this in eax.
// Identity (proven): reloc_bindings already binds ??0CWideString@@QAE@XZ to 0x0099AED0 (twice; Ego_r.pdb / FableWin.pdb
// have ??0CWideString@@QAE@XZ). The instance counter it increments, 0x013BCA20, is the one the landed CWideString copy
// constructor 0x0099B720 increments (bound ?g_CWideStringInstanceCount@@3JA); the CCharString default constructor
// 0x0099E4B0 has the same shape but counts in 0x013BD800.
// Label correction: catalog "CCharString" (module CCharString).
// Callees: none. Globals: 0x013BCA20 ?g_CWideStringInstanceCount@@3JA (existing binding; descriptive name).
// Layout: Ego_r.pdb CWideString = { CWideStringData* PStringData } (4 bytes).
// Shape: `mov eax,ecx` first (the constructor returns this), then the zero store and the counter increment.
class CWideStringData;

extern long g_CWideStringInstanceCount;                     // 0x013BCA20

class CWideString {
public:
    CWideString();
private:
    CWideStringData* PStringData;                           // +0x00
};

CWideString::CWideString()
    : PStringData(0)
{
    ++g_CWideStringInstanceCount;
}