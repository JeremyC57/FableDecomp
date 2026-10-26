// CAIStateGroup_SitForMeal::GetName() -- static: the AI state group name "SG_SIT_FOR_MEAL" as a CCharString
// Leaf: ?GetName@CAIStateGroup_SitForMeal@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FDB8F: push esi; push -1; push "SG_SIT_FOR_MEAL"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x12204d4 registers this GetName with Alloc 0x9024f0 (pushed before it); Alloc 0x9024f0 -> ctor 0x8fff74 stores (last) the vtable of CAIStateGroup_SitForMeal at this+0; literal "SG_SIT_FOR_MEAL"; PDB name set ?GetName@CAIStateGroup_SitForMeal@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_SitForMeal@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fdb8f::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_SitForMeal
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_SitForMeal::GetName()
{
    return CCharString((const char*)0x01289B28, -1);   // "SG_SIT_FOR_MEAL"
}
