// CTCDPhysicalObstruction::StatGetClassTypeName() -- the class's type name as a CCharString, "CTCDPhysicalObstruction" (static)
// Leaf: StatGetClassTypeName@CTCDPhysicalObstruction
// Retail: Fable.exe @ 0x004D5FDB, 19 bytes. Catalog label NTCScriptedControl::CActionDoCreatureAction::GetActionName is wrong.
// Evidence: builds CCharString("CTCDPhysicalObstruction", -1) (string @ 0x0123DD14) into the result; called by 0x004DF1B0 and Init_RegisterGameComponents 0x004F7F4F;
// Ego_r.pdb/FableWin.pdb carry ?StatGetClassTypeName@CTCDPhysicalObstruction@@SI?AVCCharString@@XZ (public static __fastcall), so the hidden result
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
class CTCDPhysicalObstruction {
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCDPhysicalObstruction::StatGetClassTypeName()
{
    return CCharString("CTCDPhysicalObstruction");
}