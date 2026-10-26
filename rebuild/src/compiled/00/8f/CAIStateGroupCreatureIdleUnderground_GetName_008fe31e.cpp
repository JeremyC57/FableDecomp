// CAIStateGroup_CreatureIdleUnderground::GetName() -- static: the AI state group name "SG_CREATURE_IDLE_UNDERGROUND" as a CCharString
// Leaf: ?GetName@CAIStateGroup_CreatureIdleUnderground@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE31E: push esi; push -1; push "SG_CREATURE_IDLE_UNDERGROUND"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1220e84 registers this GetName with Alloc 0x90103a (pushed before it); Alloc 0x90103a stores the vtable of CAIStateGroup_CreatureIdleUnderground into the new object; literal "SG_CREATURE_IDLE_UNDERGROUND"; PDB name set ?GetName@CAIStateGroup_CreatureIdleUnderground@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_CreatureIdleUnderground@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe31e::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_CreatureIdleUnderground
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_CreatureIdleUnderground::GetName()
{
    return CCharString((const char*)0x0128A610, -1);   // "SG_CREATURE_IDLE_UNDERGROUND"
}
