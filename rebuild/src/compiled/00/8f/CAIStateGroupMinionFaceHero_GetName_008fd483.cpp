// CAIStateGroup_MinionFaceHero::GetName() -- static: the AI state group name "SG_MINION_FACE_HERO_READY_FOR_INTERACTION" as a CCharString
// Leaf: ?GetName@CAIStateGroup_MinionFaceHero@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD483: push esi; push -1; push "SG_MINION_FACE_HERO_READY_FOR_INTERACTION"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x121faac registers this GetName with Alloc 0x90209e (pushed before it); Alloc 0x90209e -> ctor 0x8fefb9 stores (last) the vtable of CAIStateGroup_MinionFaceHero at this+0; literal "SG_MINION_FACE_HERO_READY_FOR_INTERACTION"; PDB name set ?GetName@CAIStateGroup_MinionFaceHero@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_MinionFaceHero@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fd483::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_MinionFaceHero
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_MinionFaceHero::GetName()
{
    return CCharString((const char*)0x01288F94, -1);   // "SG_MINION_FACE_HERO_READY_FOR_INTERACTION"
}
