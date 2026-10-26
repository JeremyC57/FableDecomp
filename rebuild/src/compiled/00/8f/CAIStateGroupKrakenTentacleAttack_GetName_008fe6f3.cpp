// CAIStateGroup_KrakenTentacleAttack::GetName() -- static: the AI state group name "SG_KRAKEN_TENTACLE_ATTACK" as a CCharString
// Leaf: ?GetName@CAIStateGroup_KrakenTentacleAttack@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE6F3: push esi; push -1; push "SG_KRAKEN_TENTACLE_ATTACK"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1221564 registers this GetName with Alloc 0x901b93 (pushed before it); Alloc 0x901b93 stores the vtable of CAIStateGroup_KrakenTentacleAttack into the new object; literal "SG_KRAKEN_TENTACLE_ATTACK"; PDB name set ?GetName@CAIStateGroup_KrakenTentacleAttack@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_KrakenTentacleAttack@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe6f3::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_KrakenTentacleAttack
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_KrakenTentacleAttack::GetName()
{
    return CCharString((const char*)0x0128ABD4, -1);   // "SG_KRAKEN_TENTACLE_ATTACK"
}
