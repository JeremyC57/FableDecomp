// CAIStateGroup_NymphFloatAround::GetName() -- static: the AI state group name "SG_NYMPH_FLOAT_AROUND" as a CCharString
// Leaf: ?GetName@CAIStateGroup_NymphFloatAround@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FDE78: push esi; push -1; push "SG_NYMPH_FLOAT_AROUND"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x122086c registers this GetName with Alloc 0x902686 (pushed before it); Alloc 0x902686 -> ctor 0x90059c stores (last) the vtable of CAIStateGroup_NymphFloatAround at this+0; literal "SG_NYMPH_FLOAT_AROUND"; PDB name set ?GetName@CAIStateGroup_NymphFloatAround@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_NymphFloatAround@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fde78::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_NymphFloatAround
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_NymphFloatAround::GetName()
{
    return CCharString((const char*)0x01289E4C, -1);   // "SG_NYMPH_FLOAT_AROUND"
}
