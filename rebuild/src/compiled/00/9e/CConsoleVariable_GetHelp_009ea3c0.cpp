// CConsoleVariable::GetHelp() -- "variable of type <bool|float|slong|string>; value is currently <value>"
// Leaf: GetHelp
// Retail: Fable.exe (TLC, VC7.1) @ 0x009EA3C0, 206 bytes (208 with padding), `?GetHelp@CConsoleVariable@@EAE?AVCCharString@@XZ`.
// Callees: the variable's virtual slot 3 (+0xc) GetTypeAsString() const (0x00413700, returns "variable" by value; vtable
//   0x0122E5C8: 0 deleting dtor, 1 ProcessCommandLine, 2 GetFormattedName, 3 GetTypeAsString, 4 GetHelp), operator+(const CCharString&, const char*) 0x0099F600
//   and operator+(const char*, const CCharString&) 0x0099F690 (__fastcall, hidden result in ecx; landed 0x00CB7A00),
//   CCharString::operator+=(const char*) 0x0099F100 / operator+=(const CCharString&) 0x0099F0A0, CCharString() 0x0099E4B0,
//   CGenericVarRef::PrintToString(CCharString&) 0x00A63C00, CCharString copy ctor 0x0099EC30, ~CCharString 0x0099EAE0.
// Literals: " of type " 0x0129C43C, "bool" 0x0129C434, "float" 0x0129C42C, "slong" 0x0129C424, "string" 0x0129C41C,
//   "; value is currently " 0x0129C404.
// Layout: the variable reference (CGenericVarRef: type +0, data +4) at +8.
// Shape: the help string is copy-initialised from `GetTypeAsString() + " of type "` (built in place); the value text is a
//   if/else chain on the type (a switch becomes a jump table); the value text is a default-constructed local filled by PrintToString; `help += "..." + value`; the return copies (no NRVO).
struct CCharStringData;

struct CCharString {
    CCharStringData* PStringData;
    CCharString();                                               // 0x0099E4B0
    CCharString(const CCharString& other);                       // 0x0099EC30
    ~CCharString();                                              // 0x0099EAE0
    CCharString& operator+=(const CCharString& other);           // 0x0099F0A0
    CCharString& operator+=(const char* text);                   // 0x0099F100
};
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690
CCharString __fastcall operator+(const CCharString& left, const char* right);   // 0x0099F600

struct CGenericVarRef {
    long  Type;                                                  // +0x00 (1 slong, 3 float, 4 bool, 5 string)
    void* PData;                                                 // +0x04
    void PrintToString(CCharString& out) const;                  // 0x00A63C00
};

struct CConsoleVariable {
    virtual void v00();                                          // slot 0 scalar deleting destructor
    virtual void v01();                                          // slot 1 ProcessCommandLine
    virtual void v02();                                          // slot 2 GetFormattedName
    virtual CCharString GetTypeAsString() const;                 // slot 3 (+0xc) 0x00413700
    unsigned char _pad04[4];
    CGenericVarRef Var;                                          // +0x08
    CCharString GetHelp();
};

CCharString CConsoleVariable::GetHelp()
{
    CCharString help = GetTypeAsString() + " of type ";
    if (Var.Type == 4)
        help += "bool";
    else if (Var.Type == 3)
        help += "float";
    else if (Var.Type == 1)
        help += "slong";
    else if (Var.Type == 5)
        help += "string";
    CCharString value;
    Var.PrintToString(value);
    help += "; value is currently " + value;
    return help;
}