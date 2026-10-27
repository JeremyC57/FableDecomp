// CConsoleClasslessCommand::CallCommandFunction(const std::vector<CGenericVar>& params) const -- run the console command: call the stored free command function with the parsed parameters
// Leaf: ?CallCommandFunction@CConsoleClasslessCommand@@UBEXABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z
// Retail: Fable.exe @ 0x00415040, 12 bytes, virtual __fastcall `ret 4`: `mov eax,ecx ; mov ecx,[esp+4] ; call [eax+14h]`
// (the __fastcall command function takes the vector in ecx).
// Identity: vtable 0x0122E65C (RTTI CConsoleClasslessCommand) slot 5 = 0x00415040 (slot 0 = its ??_E 0x00415050);
// Ego_r.pdb / FableWin.pdb ?CallCommandFunction@CConsoleClasslessCommand@@UBEXABV?$vector@VCGenericVar@@...@Z, and the
// PDB ctor ??0CConsoleClasslessCommand@@QAE@ABVCCharString@@P6IXABV?$vector@VCGenericVar@@...@Z@Z types CommandFunc as
// a __fastcall `void (*)(const vector<CGenericVar>&)`.
// Layout: retail CConsoleCommandBase is 0x14 bytes (engine/CConsoleCommandBase.h: vptr +0x00, byte +0x04, ParamTypes
// CConsoleCommandParameters +0x08, 12 bytes with STLport); CommandFunc sits at +0x14 (Ego_r says +0x18 with its 16-byte
// vector). The generated header models the vptr as a data member, so a virtual override cannot sit on it: the base is
// modelled locally with a real vptr (NOTES.md "Header debt").
class CGenericVar;
namespace std {
template <class _Tp> class allocator;
template <class _Tp, class _Alloc> class vector;
}

class CConsoleCommandBase {
public:
    virtual ~CConsoleCommandBase();
    unsigned char Field4;                       // +0x04 (header m_field4)
    unsigned char ParamTypes[0xC];              // +0x08 CConsoleCommandParameters
};

class CConsoleClasslessCommand : public CConsoleCommandBase {
public:
    typedef std::vector<CGenericVar, std::allocator<CGenericVar> > CGenericVarVector;
    void (__fastcall* CommandFunc)(const CGenericVarVector& params);   // +0x14
    virtual void CallCommandFunction(const CGenericVarVector& params) const;
};

void CConsoleClasslessCommand::CallCommandFunction(const CGenericVarVector& params) const
{
    CommandFunc(params);
}