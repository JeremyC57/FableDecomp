// CAIStateGroup_SkillPractice::GetName() -- static: the AI state group name "SG_SKILL_PRACTICE" as a CCharString
// Leaf: ?GetName@CAIStateGroup_SkillPractice@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE893: push esi; push -1; push "SG_SKILL_PRACTICE"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x122171c registers this GetName with Alloc 0x901dc5 (pushed before it); Alloc 0x901dc5 stores the vtable of CAIStateGroup_SkillPractice into the new object; literal "SG_SKILL_PRACTICE"; PDB name set ?GetName@CAIStateGroup_SkillPractice@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_SkillPractice@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe893::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_SkillPractice
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_SkillPractice::GetName()
{
    return CCharString((const char*)0x0128AF00, -1);   // "SG_SKILL_PRACTICE"
}
