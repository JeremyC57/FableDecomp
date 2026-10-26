// CAIStateGroup_WanderAroundInitialPos::GetName() -- static: the AI state group name "SG_WANDER_AROUND_INITIAL_POS" as a CCharString
// Leaf: ?GetName@CAIStateGroup_WanderAroundInitialPos@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD744: push esi; push -1; push "SG_WANDER_AROUND_INITIAL_POS"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1220024 registers this GetName with Alloc 0x8ff6e0 (pushed before it); Alloc 0x8ff6e0 stores the vtable of CAIStateGroup_WanderAroundInitialPos into the new object; literal "SG_WANDER_AROUND_INITIAL_POS"; PDB name set ?GetName@CAIStateGroup_WanderAroundInitialPos@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_WanderAroundInitialPos@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fd744::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_WanderAroundInitialPos
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_WanderAroundInitialPos::GetName()
{
    return CCharString((const char*)0x0128943C, -1);   // "SG_WANDER_AROUND_INITIAL_POS"
}
