// CAIStateGroup_SellToBuyer::GetName() -- static: the AI state group name "SG_SELL_TO_BUYER" as a CCharString
// Leaf: ?GetName@CAIStateGroup_SellToBuyer@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FDD4C: push esi; push -1; push "SG_SELL_TO_BUYER"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x122072c registers this GetName with Alloc 0x9025b1 (pushed before it); Alloc 0x9025b1 -> ctor 0x9002a5 stores (last) the vtable of CAIStateGroup_SellToBuyer at this+0; literal "SG_SELL_TO_BUYER"; PDB name set ?GetName@CAIStateGroup_SellToBuyer@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_SellToBuyer@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fdd4c::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_SellToBuyer
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_SellToBuyer::GetName()
{
    return CCharString((const char*)0x01289C78, -1);   // "SG_SELL_TO_BUYER"
}
