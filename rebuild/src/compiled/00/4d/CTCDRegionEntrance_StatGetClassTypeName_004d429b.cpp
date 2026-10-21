// CTCDRegionEntrance::StatGetClassTypeName() -- the class's type name as a CCharString, "CTCDRegionEntrance" (static)
// Leaf: StatGetClassTypeName@CTCDRegionEntrance
// Retail: Fable.exe @ 0x004D429B, 19 bytes. Catalog label NTCScriptedControl::CActionDoCreatureAction::GetActionName is wrong.
// Evidence: builds CCharString("CTCDRegionEntrance", -1) (string @ 0x0123BA5C) into the result; called by its GetClassTypeName 0x004D42D6 and Init_RegisterGameComponents 0x004F28C3;
// Ego_r.pdb/FableWin.pdb carry ?StatGetClassTypeName@CTCDRegionEntrance@@SI?AVCCharString@@XZ (public static __fastcall), so the hidden result
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
class CTCDRegionEntrance {
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCDRegionEntrance::StatGetClassTypeName()
{
    return CCharString("CTCDRegionEntrance");
}