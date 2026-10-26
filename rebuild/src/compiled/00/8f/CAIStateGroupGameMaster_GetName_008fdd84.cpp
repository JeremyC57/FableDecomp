// CAIStateGroup_GameMaster::GetName() -- static: the AI state group name "SG_GAME_MASTER" as a CCharString
// Leaf: ?GetName@CAIStateGroup_GameMaster@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FDD84: push esi; push -1; push "SG_GAME_MASTER"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x12207a4 registers this GetName with Alloc 0x9025c7 (pushed before it); Alloc 0x9025c7 stores the vtable of CAIStateGroup_GameMaster into the new object; literal "SG_GAME_MASTER"; PDB name set ?GetName@CAIStateGroup_GameMaster@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_GameMaster@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fdd84::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_GameMaster
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_GameMaster::GetName()
{
    return CCharString((const char*)0x01289CD8, -1);   // "SG_GAME_MASTER"
}
