// CAIStateGroup_CreatureGoUnderground::GetName() -- static: the AI state group name "SG_CREATURE_GO_UNDERGROUND" as a CCharString
// Leaf: ?GetName@CAIStateGroup_CreatureGoUnderground@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE2F9: push esi; push -1; push "SG_CREATURE_GO_UNDERGROUND"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1220e5c registers this GetName with Alloc 0x900ffa (pushed before it); Alloc 0x900ffa stores the vtable of CAIStateGroup_CreatureGoUnderground into the new object; literal "SG_CREATURE_GO_UNDERGROUND"; PDB name set ?GetName@CAIStateGroup_CreatureGoUnderground@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_CreatureGoUnderground@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe2f9::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_CreatureGoUnderground
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_CreatureGoUnderground::GetName()
{
    return CCharString((const char*)0x0128A5B8, -1);   // "SG_CREATURE_GO_UNDERGROUND"
}
