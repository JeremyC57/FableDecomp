// CAIStateGroup_BoastingCrowdMember::GetName() -- static: the AI state group name "SG_BOASTING_CROWD_MEMBER" as a CCharString
// Leaf: ?GetName@CAIStateGroup_BoastingCrowdMember@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE35B: push esi; push -1; push "SG_BOASTING_CROWD_MEMBER"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x12210dc registers this GetName with Alloc 0x902aa0 (pushed before it); Alloc 0x902aa0 -> ctor 0x9010c4 stores (last) the vtable of CAIStateGroup_BoastingCrowdMember at this+0; literal "SG_BOASTING_CROWD_MEMBER"; PDB name set ?GetName@CAIStateGroup_BoastingCrowdMember@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_BoastingCrowdMember@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe35b::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_BoastingCrowdMember
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_BoastingCrowdMember::GetName()
{
    return CCharString((const char*)0x0128A690, -1);   // "SG_BOASTING_CROWD_MEMBER"
}
