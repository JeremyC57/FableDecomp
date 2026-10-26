// CAIStateGroup_KrakenTentacleRise::GetName() -- static: the AI state group name "SG_KRAKEN_TENTACLE_RISE" as a CCharString
// Leaf: ?GetName@CAIStateGroup_KrakenTentacleRise@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE6CE: push esi; push -1; push "SG_KRAKEN_TENTACLE_RISE"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x122153c registers this GetName with Alloc 0x901b53 (pushed before it); Alloc 0x901b53 stores the vtable of CAIStateGroup_KrakenTentacleRise into the new object; literal "SG_KRAKEN_TENTACLE_RISE"; PDB name set ?GetName@CAIStateGroup_KrakenTentacleRise@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_KrakenTentacleRise@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe6ce::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_KrakenTentacleRise
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_KrakenTentacleRise::GetName()
{
    return CCharString((const char*)0x0128AB80, -1);   // "SG_KRAKEN_TENTACLE_RISE"
}
