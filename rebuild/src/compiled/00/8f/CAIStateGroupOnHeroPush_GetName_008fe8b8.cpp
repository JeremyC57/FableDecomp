// CAIStateGroup_OnHeroPush::GetName() -- static: the AI state group name "SG_ON_HERO_PUSH" as a CCharString
// Leaf: ?GetName@CAIStateGroup_OnHeroPush@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE8B8: push esi; push -1; push "SG_ON_HERO_PUSH"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1221744 registers this GetName with Alloc 0x901e05 (pushed before it); Alloc 0x901e05 stores the vtable of CAIStateGroup_OnHeroPush into the new object; literal "SG_ON_HERO_PUSH"; PDB name set ?GetName@CAIStateGroup_OnHeroPush@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_OnHeroPush@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe8b8::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_OnHeroPush
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_OnHeroPush::GetName()
{
    return CCharString((const char*)0x0128AF58, -1);   // "SG_ON_HERO_PUSH"
}
