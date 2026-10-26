// CAIStateGroup_KrakenAttack::GetName() -- static: the AI state group name "SG_KRAKEN_ATTACK" as a CCharString
// Leaf: ?GetName@CAIStateGroup_KrakenAttack@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE684: push esi; push -1; push "SG_KRAKEN_ATTACK"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x12214ec registers this GetName with Alloc 0x901ad3 (pushed before it); Alloc 0x901ad3 stores the vtable of CAIStateGroup_KrakenAttack into the new object; literal "SG_KRAKEN_ATTACK"; PDB name set ?GetName@CAIStateGroup_KrakenAttack@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_KrakenAttack@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe684::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_KrakenAttack
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_KrakenAttack::GetName()
{
    return CCharString((const char*)0x0128AADC, -1);   // "SG_KRAKEN_ATTACK"
}
