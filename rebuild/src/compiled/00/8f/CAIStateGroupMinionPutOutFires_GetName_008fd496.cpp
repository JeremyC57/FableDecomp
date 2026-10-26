// CAIStateGroup_MinionPutOutFires::GetName() -- static: the AI state group name "SG_MINION_PUT_OUT_FIRES" as a CCharString
// Leaf: ?GetName@CAIStateGroup_MinionPutOutFires@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD496: push esi; push -1; push "SG_MINION_PUT_OUT_FIRES"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x121fad4 registers this GetName with Alloc 0x9020b4 (pushed before it); Alloc 0x9020b4 -> ctor 0x8ff003 stores (last) the vtable of CAIStateGroup_MinionPutOutFires at this+0; literal "SG_MINION_PUT_OUT_FIRES"; PDB name set ?GetName@CAIStateGroup_MinionPutOutFires@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_MinionPutOutFires@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fd496::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_MinionPutOutFires
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_MinionPutOutFires::GetName()
{
    return CCharString((const char*)0x01288FC0, -1);   // "SG_MINION_PUT_OUT_FIRES"
}
