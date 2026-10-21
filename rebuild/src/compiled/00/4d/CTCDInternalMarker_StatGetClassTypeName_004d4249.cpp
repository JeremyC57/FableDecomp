// CTCDInternalMarker::StatGetClassTypeName() -- the class's type name as a CCharString, "CTCDInternalMarker" (static)
// Leaf: StatGetClassTypeName@CTCDInternalMarker
// Retail: Fable.exe @ 0x004D4249, 19 bytes. Catalog label NTCScriptedControl::CActionDoCreatureAction::GetActionName is wrong.
// Evidence: builds CCharString("CTCDInternalMarker", -1) (string @ 0x0123B9CC) into the result; called by CTCDInternalMarker::GetClassTypeName 0x004D427E (`return StatGetClassTypeName();`) and Init_RegisterGameComponents 0x004F285C;
// Ego_r.pdb/FableWin.pdb carry ?StatGetClassTypeName@CTCDInternalMarker@@SI?AVCCharString@@XZ (public static __fastcall), so the hidden result
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
class CTCDInternalMarker {
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCDInternalMarker::StatGetClassTypeName()
{
    return CCharString("CTCDInternalMarker");
}