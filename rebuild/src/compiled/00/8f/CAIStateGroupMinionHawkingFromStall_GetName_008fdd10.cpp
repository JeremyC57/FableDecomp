// CAIStateGroup_MinionHawkingFromStall::GetName() -- static: the AI state group name "SG_MINION_HAWKING_FROM_STALL" as a CCharString
// Leaf: ?GetName@CAIStateGroup_MinionHawkingFromStall@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FDD10: push esi; push -1; push 0x01289C44 ("SG_MINION_HAWKING_FROM_STALL"); mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall returning CCharString
//   by value (hidden result in ecx).
// Name fix (agentNF36 2026-11-16): was the stand-in free function GetActionName / CAIStateGroupNameTag. FableWin.pdb name set holds
//   ?GetName@CAIStateGroup_MinionHawkingFromStall@@SI?AVCCharString@@XZ; the StateGroupInfo initializer 0x0121F86F (landed) already binds it; literal "SG_MINION_HAWKING_FROM_STALL" at 0x01289C44.
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_MinionHawkingFromStall                     // no shared header; only the static name accessor is modelled
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_MinionHawkingFromStall::GetName()
{
    return CCharString("SG_MINION_HAWKING_FROM_STALL", -1);   // "SG_MINION_HAWKING_FROM_STALL"
}
