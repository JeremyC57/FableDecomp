// CAIStateGroup_TellStory::GetName() -- static: the AI state group name "SG_TELL_STORY" as a CCharString
// Leaf: ?GetName@CAIStateGroup_TellStory@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FDA4A: push esi; push -1; push "SG_TELL_STORY"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1220394 registers this GetName with Alloc 0x90248a (pushed before it); Alloc 0x90248a stores the vtable of CAIStateGroup_TellStory into the new object; literal "SG_TELL_STORY"; PDB name set ?GetName@CAIStateGroup_TellStory@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_TellStory@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fda4a::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_TellStory
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_TellStory::GetName()
{
    return CCharString((const char*)0x012898C8, -1);   // "SG_TELL_STORY"
}
