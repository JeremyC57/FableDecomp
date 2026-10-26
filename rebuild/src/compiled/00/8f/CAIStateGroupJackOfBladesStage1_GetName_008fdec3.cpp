// CAIStateGroup_JackOfBladesStage1::GetName() -- static: the AI state group name "SG_JACK_OF_BLADES_STAGE1" as a CCharString
// Leaf: ?GetName@CAIStateGroup_JackOfBladesStage1@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FDEC3: push esi; push -1; push "SG_JACK_OF_BLADES_STAGE1"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1220b14 registers this GetName with Alloc 0x9034d9 (pushed before it); Alloc 0x9034d9 -> ctor 0x903216 stores (last) the vtable of CAIStateGroup_JackOfBladesStage1 at this+0; literal "SG_JACK_OF_BLADES_STAGE1"; PDB name set ?GetName@CAIStateGroup_JackOfBladesStage1@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_JackOfBladesStage1@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fdec3::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_JackOfBladesStage1
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_JackOfBladesStage1::GetName()
{
    return CCharString((const char*)0x01289EC0, -1);   // "SG_JACK_OF_BLADES_STAGE1"
}
