// CAIStateGroup_ButterflyWander::GetName() -- static: the AI state group name "SG_BUTTERFLY_WANDER" as a CCharString
// Leaf: ?GetName@CAIStateGroup_ButterflyWander@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD7D9: push esi; push -1; push 0x0128954C ("SG_BUTTERFLY_WANDER"); mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall returning CCharString
//   by value (hidden result in ecx).
// Name fix (agentNF36 2026-11-16): was the stand-in free function GetActionName / CAIStateGroupNameTag. FableWin.pdb name set holds
//   ?GetName@CAIStateGroup_ButterflyWander@@SI?AVCCharString@@XZ; the StateGroupInfo initializer 0x0121F86F (landed) already binds it; literal "SG_BUTTERFLY_WANDER" at 0x0128954C.
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_ButterflyWander                     // no shared header; only the static name accessor is modelled
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_ButterflyWander::GetName()
{
    return CCharString("SG_BUTTERFLY_WANDER", -1);   // "SG_BUTTERFLY_WANDER"
}
