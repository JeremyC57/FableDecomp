// CAIStateGroup_MinionReactToCorpse::GetName() -- static: the AI state group name "SG_MINION_REACT_TO_CORPSE" as a CCharString
// Leaf: ?GetName@CAIStateGroup_MinionReactToCorpse@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD4BC: push esi; push -1; push 0x01288FF0 ("SG_MINION_REACT_TO_CORPSE"); mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall returning CCharString
//   by value (hidden result in ecx).
// Name fix (agentNF36 2026-11-16): was the stand-in free function GetActionName / CAIStateGroupNameTag. FableWin.pdb name set holds
//   ?GetName@CAIStateGroup_MinionReactToCorpse@@SI?AVCCharString@@XZ; the StateGroupInfo initializer 0x0121F86F (landed) already binds it; literal "SG_MINION_REACT_TO_CORPSE" at 0x01288FF0.
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_MinionReactToCorpse                     // no shared header; only the static name accessor is modelled
{
public:
    static CCharString __fastcall GetName();
};

// Literal (NF413, 2026-11-25): HEAD cast the retail address (const char*)0x01288FF0; it is now the literal spelled from retail's bytes
//   at 0x01288FF0 (narrow "SG_MINION_REACT_TO_CORPSE", NUL-terminated).
CCharString __fastcall CAIStateGroup_MinionReactToCorpse::GetName()
{
    return CCharString("SG_MINION_REACT_TO_CORPSE", -1);
}
