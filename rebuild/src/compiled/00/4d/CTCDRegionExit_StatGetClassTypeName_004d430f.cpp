// CTCDRegionExit::StatGetClassTypeName() -- the class's type name as a CCharString, "CTCDRegionExit" (static)
// Leaf: StatGetClassTypeName@CTCDRegionExit
// Retail: Fable.exe @ 0x004D430F, 19 bytes. Catalog label NTCScriptedControl::CActionDoCreatureAction::GetActionName is wrong.
// Evidence: builds CCharString("CTCDRegionExit", -1) (string @ 0x0123BAEC) into the result; called by Init_RegisterGameComponents 0x004F292A;
// Ego_r.pdb/FableWin.pdb carry ?StatGetClassTypeName@CTCDRegionExit@@SI?AVCCharString@@XZ (public static __fastcall), so the hidden result
// pointer arrives in ecx and is returned in eax.
// Callee: CCharString::CCharString(const char*, long) @ 0x0099EBF0.
class CCharStringData;
class CCharString {                                     // 4-byte handle (Ego_r.pdb); user copy ctor/dtor: returned through a hidden pointer
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();
};
class CTCDRegionExit {
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCDRegionExit::StatGetClassTypeName()
{
    return CCharString("CTCDRegionExit");
}