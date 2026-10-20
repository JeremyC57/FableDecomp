// CConsoleCommandBase::GetHelp() -- "<name> with no parameters" / "<name> with N parameter[s]: <type> <type> ..."
// Leaf: ?GetHelp@CConsoleCommandBase
// Retail: Fable.exe (TLC, VC7.1) @ 0x009EA7D0, 313 bytes, `?GetHelp@CConsoleCommandBase@@MAE?AVCCharString@@XZ`.
// Callees: virtual slot 3 (+0xc, the command name by value), operator+(const CCharString&, const char*) 0x0099F600
//   (__fastcall, hidden result in ecx), GFIntToCharString(long) 0x0099F830 (__fastcall), CCharString::operator+=
//   (const char*) 0x0099F100 / (const CCharString&) 0x0099F0A0, CConsoleCommandParameters::GetParameterTypeAsString
//   (long) 0x009EA740 (landed), CCharString copy ctor 0x0099EC30, ~CCharString 0x0099EAE0.
// Literals: " with " 0x0129C480, "no parameters" 0x0129C470, " parameter" 0x0129C464, "s" 0x0129C460,
//   ": " 0x012780A8, " " 0x01258C1C.
// Layout: the engine header CConsoleCommandBase.h flattens the parameter list (+0x08, PDB 0x10 bytes) into a byte
//   array, so `this` is modelled locally: the parameter types are an STLport vector<long> at +0x08 whose count
//   is read through an inline `long GetNumParameters()` (signed compares: `jle`/`jl`, `and ~3 ; cmp 4`).
// Shape: same family as the landed CConsoleVariable::GetHelp 0x009EA3C0: copy-initialised from
//   `GetName() + " with "`, `help += a + b` temporaries destroyed in reverse order, and the return copies
//   the named local (no NRVO).
struct CCharStringData;

struct CCharString {
    CCharStringData* PStringData;
    CCharString(const CCharString& other);                       // 0x0099EC30
    ~CCharString();                                              // 0x0099EAE0
    CCharString& operator+=(const CCharString& other);           // 0x0099F0A0
    CCharString& operator+=(const char* text);                   // 0x0099F100
};
CCharString __fastcall operator+(const CCharString& left, const char* right);   // 0x0099F600
CCharString __fastcall GFIntToCharString(long value);                            // 0x0099F830

struct CConsoleCommandParameters {
    long* m_pTypesStart;                                         // +0x00 STLport vector<long>
    long* m_pTypesFinish;                                        // +0x04
    long* m_pTypesEnd;                                           // +0x08
    long GetNumParameters() const { return m_pTypesFinish - m_pTypesStart; }
    CCharString GetParameterTypeAsString(long index) const;      // 0x009EA740
};

struct CConsoleCommandBase {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual CCharString GetName();                               // slot 3 (+0xc)
    unsigned char m_field4;                                      // +0x04
    unsigned char _pad05[3];
    CConsoleCommandParameters ParamTypes;                        // +0x08
    CCharString GetHelp();
};

CCharString CConsoleCommandBase::GetHelp()
{
    CCharString help = GetName() + " with ";
    if (ParamTypes.GetNumParameters() == 0) {
        help += "no parameters";
    } else {
        help += GFIntToCharString(ParamTypes.GetNumParameters()) + " parameter";
        if (ParamTypes.GetNumParameters() > 1)
            help += "s";
        help += ": ";
        for (long i = 0; i < ParamTypes.GetNumParameters(); ++i)
            help += ParamTypes.GetParameterTypeAsString(i) + " ";
    }
    return help;
}