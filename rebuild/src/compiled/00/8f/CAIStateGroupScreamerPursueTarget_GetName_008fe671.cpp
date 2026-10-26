// CAIStateGroup_ScreamerPursueTarget::GetName() -- static: the AI state group name "SG_SCREAMER_PURSUE_TARGET" as a CCharString
// Leaf: ?GetName@CAIStateGroup_ScreamerPursueTarget@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE671: push esi; push -1; push "SG_SCREAMER_PURSUE_TARGET"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x12214c4 registers this GetName with Alloc 0x902ced (pushed before it); Alloc 0x902ced -> ctor 0x901a89 stores (last) the vtable of CAIStateGroup_ScreamerPursueTarget at this+0; literal "SG_SCREAMER_PURSUE_TARGET"; PDB name set ?GetName@CAIStateGroup_ScreamerPursueTarget@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_ScreamerPursueTarget@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe671::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_ScreamerPursueTarget
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_ScreamerPursueTarget::GetName()
{
    return CCharString((const char*)0x0128AAC0, -1);   // "SG_SCREAMER_PURSUE_TARGET"
}
