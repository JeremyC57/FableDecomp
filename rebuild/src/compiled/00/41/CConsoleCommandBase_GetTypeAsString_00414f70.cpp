// CConsoleCommandBase::GetTypeAsString() const -- the console entry's type name, "command"
// Leaf: GetTypeAsString@CConsoleCommandBase
// Retail: Fable.exe @ 0x00414F70, 25 bytes, `ret 4` (hidden result pointer). Ego_r.pdb ?GetTypeAsString@CConsoleCommandBase@@MBE?AVCCharString@@XZ
// (protected virtual); the catalog's NTCScriptedControl::CActionDoCreatureAction prefix is wrong. Referenced from the vtables of
// CConsoleCommandBase (0x0122E644 slot 0), CConsoleClasslessCommand (0x0122E668), CConsoleCommand<CGameTimeManager>
// (0x0125D674) and CConsoleCommand<CConsole> (0x0129C4EC). Builds CCharString("command", -1) (string @ 0x0122E650) via
// CCharString::CCharString(const char*, long) 0x0099EBF0.
class CCharStringData;
class CCharString {                                     // 4-byte handle (Ego_r.pdb); user copy ctor/dtor: returned through a hidden pointer
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();
};
class CConsoleCommandBase {
protected:
    virtual CCharString GetTypeAsString() const;
};

CCharString CConsoleCommandBase::GetTypeAsString() const
{
    return CCharString("command");
}