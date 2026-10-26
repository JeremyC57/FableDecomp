// CAIStateGroup_FireflyWander::GetName() -- static: the AI state group name "SG_FIREFLY_WANDER" as a CCharString
// Leaf: ?GetName@CAIStateGroup_FireflyWander@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD7F1: push esi; push -1; push "SG_FIREFLY_WANDER"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x121febc registers this GetName with Alloc 0x8ff80e (pushed before it); Alloc 0x8ff80e stores the vtable of CAIStateGroup_FireflyWander into the new object; literal "SG_FIREFLY_WANDER"; PDB name set ?GetName@CAIStateGroup_FireflyWander@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_FireflyWander@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fd7f1::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_FireflyWander
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_FireflyWander::GetName()
{
    return CCharString((const char*)0x01289560, -1);   // "SG_FIREFLY_WANDER"
}
