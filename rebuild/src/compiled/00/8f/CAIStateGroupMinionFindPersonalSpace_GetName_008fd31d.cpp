// CAIStateGroup_MinionFindPersonalSpace::GetName() -- static: the AI state group name "SG_MINION_FIND_PERSONAL_SPACE" as a CCharString
// Leaf: ?GetName@CAIStateGroup_MinionFindPersonalSpace@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD31D: push esi; push -1; push "SG_MINION_FIND_PERSONAL_SPACE"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x121f91c registers this GetName with Alloc 0x901f31 (pushed before it); Alloc 0x901f31 -> ctor 0x8fec21 stores (last) the vtable of CAIStateGroup_MinionFindPersonalSpace at this+0; literal "SG_MINION_FIND_PERSONAL_SPACE"; PDB name set ?GetName@CAIStateGroup_MinionFindPersonalSpace@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_MinionFindPersonalSpace@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fd31d::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_MinionFindPersonalSpace
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_MinionFindPersonalSpace::GetName()
{
    return CCharString((const char*)0x01288D00, -1);   // "SG_MINION_FIND_PERSONAL_SPACE"
}
