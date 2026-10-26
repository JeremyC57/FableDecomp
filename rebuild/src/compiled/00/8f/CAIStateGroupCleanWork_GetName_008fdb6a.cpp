// CAIStateGroup_CleanWork::GetName() -- static: the AI state group name "SG_CLEAN_WORK" as a CCharString
// Leaf: ?GetName@CAIStateGroup_CleanWork@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FDB6A: push esi; push -1; push "SG_CLEAN_WORK"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x12204ac registers this GetName with Alloc 0x8fff34 (pushed before it); Alloc 0x8fff34 stores the vtable of CAIStateGroup_CleanWork into the new object; literal "SG_CLEAN_WORK"; PDB name set ?GetName@CAIStateGroup_CleanWork@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_CleanWork@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fdb6a::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_CleanWork
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_CleanWork::GetName()
{
    return CCharString((const char*)0x01289AD8, -1);   // "SG_CLEAN_WORK"
}
