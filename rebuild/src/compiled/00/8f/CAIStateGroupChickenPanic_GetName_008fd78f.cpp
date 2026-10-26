// CAIStateGroup_ChickenPanic::GetName() -- static: the AI state group name "SG_CHICKEN_PANIC" as a CCharString
// Leaf: ?GetName@CAIStateGroup_ChickenPanic@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD78F: push esi; push -1; push "SG_CHICKEN_PANIC"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x121fe6c registers this GetName with Alloc 0x8ff760 (pushed before it); Alloc 0x8ff760 stores the vtable of CAIStateGroup_ChickenPanic into the new object; literal "SG_CHICKEN_PANIC"; PDB name set ?GetName@CAIStateGroup_ChickenPanic@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_ChickenPanic@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fd78f::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_ChickenPanic
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_ChickenPanic::GetName()
{
    return CCharString((const char*)0x012894B0, -1);   // "SG_CHICKEN_PANIC"
}
