// CAIStateGroup_KrakenTentacleIdle::GetName() -- static: the AI state group name "SG_KRAKEN_TENTACLE_IDLE" as a CCharString
// Leaf: ?GetName@CAIStateGroup_KrakenTentacleIdle@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE6A9: push esi; push -1; push "SG_KRAKEN_TENTACLE_IDLE"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1221514 registers this GetName with Alloc 0x901b13 (pushed before it); Alloc 0x901b13 stores the vtable of CAIStateGroup_KrakenTentacleIdle into the new object; literal "SG_KRAKEN_TENTACLE_IDLE"; PDB name set ?GetName@CAIStateGroup_KrakenTentacleIdle@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_KrakenTentacleIdle@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe6a9::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_KrakenTentacleIdle
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_KrakenTentacleIdle::GetName()
{
    return CCharString((const char*)0x0128AB2C, -1);   // "SG_KRAKEN_TENTACLE_IDLE"
}
