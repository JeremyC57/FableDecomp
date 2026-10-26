// CAIStateGroup_NymphAttack::GetName() -- static: the AI state group name "SG_NYMPH_ATTACK" as a CCharString
// Leaf: ?GetName@CAIStateGroup_NymphAttack@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FDE8B: push esi; push -1; push "SG_NYMPH_ATTACK"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1220894 registers this GetName with Alloc 0x900607 (pushed before it); Alloc 0x900607 stores the vtable of CAIStateGroup_NymphAttack into the new object; literal "SG_NYMPH_ATTACK"; PDB name set ?GetName@CAIStateGroup_NymphAttack@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_NymphAttack@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fde8b::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_NymphAttack
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_NymphAttack::GetName()
{
    return CCharString((const char*)0x01289E64, -1);   // "SG_NYMPH_ATTACK"
}
