// CAIStateGroup_JackOfBladesStage3::GetName() -- static: the AI state group name "SG_JACK_OF_BLADES_STAGE3" as a CCharString
// Leaf: ?GetName@CAIStateGroup_JackOfBladesStage3@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FDEE9: push esi; push -1; push "SG_JACK_OF_BLADES_STAGE3"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1220b64 registers this GetName with Alloc 0x9032bc (pushed before it); Alloc 0x9032bc stores the vtable of CAIStateGroup_JackOfBladesStage3 into the new object; literal "SG_JACK_OF_BLADES_STAGE3"; PDB name set ?GetName@CAIStateGroup_JackOfBladesStage3@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_JackOfBladesStage3@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fdee9::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_JackOfBladesStage3
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_JackOfBladesStage3::GetName()
{
    return CCharString((const char*)0x01289EF8, -1);   // "SG_JACK_OF_BLADES_STAGE3"
}
