// CAIStateGroup_DeedReactionRoot::GetName() -- static: the AI state group name "SG_DEED_REACTION_ROOT" as a CCharString
// Leaf: ?GetName@CAIStateGroup_DeedReactionRoot@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE381: push esi; push -1; push "SG_DEED_REACTION_ROOT"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x122112c registers this GetName with Alloc 0x902ab6 (pushed before it); Alloc 0x902ab6 -> ctor 0x901178 stores (last) the vtable of CAIStateGroup_DeedReactionRoot at this+0; literal "SG_DEED_REACTION_ROOT"; PDB name set ?GetName@CAIStateGroup_DeedReactionRoot@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_DeedReactionRoot@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe381::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_DeedReactionRoot
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_DeedReactionRoot::GetName()
{
    return CCharString((const char*)0x01275988, -1);   // "SG_DEED_REACTION_ROOT"
}
