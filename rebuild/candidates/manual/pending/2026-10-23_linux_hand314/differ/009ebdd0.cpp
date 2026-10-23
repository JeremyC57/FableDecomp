// CConsoleCommandBase::ProcessCommandLine(const CCharString& line, CConsole& console) -- parse "name(arg, arg, ...)": the leading bracket, one CGenericVar per declared parameter type (separated by commas), the closing bracket; on success execute the command with the parsed arguments and return true, else report the error plus a usage line ("<name> takes N parameter(s): types") and return false
// Grade: DIFFER(1584v1625) -- 83 blocks: register/frame allocation -- retail keeps the error flag in memory (its slot shared with the push_back overflow tag) and ok in bl, ours the reverse; retail passes the bracket char through a dword stack copy (ours movsx), frame 0x8c v 0x78; message building, loop and usage tail follow retail call order
// Leaf: ProcessCommandLine
// Retail 0x009EBDD0, 1632 bytes, `ret 8`.
// Callees: CCharString(const char*, long) 0x0099EBF0 / () 0x0099E4B0 / copy 0x0099EC30 / ~ 0x0099EAE0 / != char 0x0099E520 /
//   != CCharString 0x0099E900 / += const char* 0x0099F100 / += CCharString 0x0099F0A0 / ToLower 0x0099ED10;
//   GFIntToCharString 0x0099F830, operator+ 0x0099F570 / 0x0099F600 / 0x0099F690 (all `__fastcall`, hidden result);
//   CStringParser(text, callback, filename, base line) 0x00404720, ReadAsStringUntilString 0x009BA110, ReadNextItem
//   0x009BA080, PeekNextItem 0x009BA8C0, comment-list dtor 0x004046B0, CBaseClass dtor 0x0099A2E0; CParsedItem::GetString
//   0x009B9870 (catalog "CModeBlock::GetAnimFilter"), ConvertToString 0x009B98B0; CConsoleCommandParameters create-var
//   0x009EA6C0, GetParameterTypeAsString 0x009EA740; CGenericVar::ReadFromParser 0x00A64360 (vtable 0x0129C3E4);
//   vector<CGenericVar>::_M_insert_overflow 0x009EFA50; CConsole::SendMessageToUser 0x009EB260; free 0x00BFEA14.
// Globals: the bracket / comma strings 0x013CAA48 "(" / 0x013CAA4C ")" / 0x013CAA50 ",".
// Layout: Ego_r.pdb CStringParser (48 bytes; retail 40 with a 4-byte STLport list), CParsedItem (12), CGenericVar (28),
//   CConsoleCommandParameters (vector of EGenericVarType, retail 12 bytes); CConsoleCommandBase {vptr, name +4,
//   ParamTypes +8} (rebuild/include/engine/CConsoleCommandBase.h types +4 as a byte; this function uses it as a string).
extern "C" void __cdecl free(void* p);                // 0x00BFEA14
struct SStringData { char* Text; };
struct CCharString {
    SStringData* PStringData;
    CCharString();                                     // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
    const char* c_str() const
    {
        if (PStringData == 0)
            return "";
        return PStringData->Text;
    }
    bool operator!=(char c) const;                     // 0x0099E520
    bool operator!=(const CCharString& other) const;   // 0x0099E900
    CCharString& operator+=(const char* text);         // 0x0099F100
    CCharString& operator+=(const CCharString& text);  // 0x0099F0A0
    CCharString ToLower() const;                       // 0x0099ED10
};
CCharString __fastcall GFIntToCharString(long value);                                  // 0x0099F830
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
CCharString __fastcall operator+(const CCharString& left, const char* right);          // 0x0099F600
CCharString __fastcall operator+(const char* left, const CCharString& right);          // 0x0099F690
struct CBaseClass {
    virtual ~CBaseClass();                             // 0x0099A2E0 (inline body: vtable store)
};
struct CStringParserCommentList {
    void* _M_node;
    ~CStringParserCommentList();                       // 0x004046B0
};
struct CParsedItem {
    long        Type;                                  // +0x00
    long        Value;                                 // +0x04
    CCharString String;                                // +0x08
    const CCharString& GetString() const;              // 0x009B9870
    CCharString ConvertToString() const;               // 0x009B98B0
};
struct CStringParser : CBaseClass {
    char*                    String;                   // +0x04
    unsigned long            BaseLineOffset;           // +0x08
    CCharString              Filename;                 // +0x0c
    CCharString              TempCharString;           // +0x10
    CStringParserCommentList Comments;                 // +0x14
    void (__cdecl* ErrorCallback)(CCharString*);       // +0x18
    long                     CursorPos[3];             // +0x1c
    CStringParser(const CCharString& text, void (__cdecl* callback)(CCharString*), const CCharString& filename,
                  long baseLine);                      // 0x00404720
    bool ReadAsStringUntilString(const CCharString& terminator, CParsedItem* item);   // 0x009BA110
    bool ReadNextItem(CParsedItem* item);              // 0x009BA080
    bool PeekNextItem(CParsedItem* item);              // 0x009BA8C0
};
struct CGenericVar {                                   // vtable 0x0129C3E4
    long        Type;                                  // +0x04
    CCharString ValString;                             // +0x08
    long        ValSLONG;                              // +0x0c
    unsigned long ValULONG;                            // +0x10
    bool        ValBool;                               // +0x14
    float       ValFloat;                              // +0x18
    virtual ~CGenericVar() {}
    bool ReadFromParser(CStringParser* parser);        // 0x00A64360
};
struct __false_type {};
template <class T> struct CArray {                     // retail STLport vector, 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CArray() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CArray()
    {
        for (T* p = _M_start; p != _M_finish; ++p)
            p->~T();
        if (_M_start)
            free(_M_start);
    }
    long size() const { return _M_finish - _M_start; }
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned long fill_len, bool atend);   // 0x009EFA50
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            if (_M_finish)
                new (_M_finish) T(x);
            ++_M_finish;
        } else {
            __false_type tag;
            _M_insert_overflow(_M_finish, x, tag, 1UL, true);
        }
    }
};
inline void* __cdecl operator new(unsigned int, void* p) { return p; }
struct CConsoleCommandParameters {
    CArray<long> ParamTypes;
    long size() const { return ParamTypes.size(); }
    CGenericVar CreateVar(long index) const;           // 0x009EA6C0
    CCharString GetParameterTypeAsString(long index) const;   // 0x009EA740
};
struct CConsole {
    void SendMessageToUser(const CCharString& message);   // 0x009EB260
};
extern CCharString g_OpenBracket;                      // 0x013CAA48
extern CCharString g_CloseBracket;                     // 0x013CAA4C
extern CCharString g_Comma;                            // 0x013CAA50
class CConsoleCommandBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual bool Execute(const CArray<CGenericVar>& args);   // slot 5 (+0x14)
    CCharString               Name;                    // +0x04
    CConsoleCommandParameters ParamTypes;              // +0x08
    bool ProcessCommandLine(const CCharString& line, CConsole& console);
};

bool CConsoleCommandBase::ProcessCommandLine(const CCharString& line, CConsole& console)
{
    CStringParser parser(line, 0, CCharString(""), 0);
    CParsedItem item;
    CArray<CGenericVar> args;
    bool error = false;
    parser.ReadAsStringUntilString(g_OpenBracket, &item);
    bool ok = parser.ReadNextItem(&item);
    if (!ok || item.Type != 1 || item.GetString() != g_OpenBracket.c_str()[0]) {
        console.SendMessageToUser(CCharString("command requires leading bracket"));
        error = true;
    }
    parser.PeekNextItem(&item);
    long i = 0;
    while (i < ParamTypes.size() && !error) {
        CGenericVar var = ParamTypes.CreateVar(i);
        if (var.ReadFromParser(&parser)) {
            args.push_back(var);
            ok = parser.ReadNextItem(&item);
            ++i;
            if (i < ParamTypes.size() && (!ok || item.Type != 1 || item.GetString() != g_Comma)) {
                console.SendMessageToUser(CCharString("expected ") + g_Comma + " after parameter " + GFIntToCharString(i));
                error = true;
            }
        } else {
            console.SendMessageToUser("parameter " + GFIntToCharString(i) + " is of incorrect type");
            error = true;
        }
    }
    if (i == 0)
        ok = parser.ReadNextItem(&item);
    if (i == ParamTypes.size() && !error && ok) {
        if (item.Type != 1) {
            console.SendMessageToUser("cannot interpret " + item.ConvertToString());
            error = true;
        } else if (item.GetString() != g_CloseBracket.c_str()[0]) {
            console.SendMessageToUser(CCharString("expected ") + g_CloseBracket + " instead of " + item.GetString());
            error = true;
        } else if (parser.ReadNextItem(&item)) {
            console.SendMessageToUser(CCharString("ignoring stuff after closing bracket"));
        }
    }
    if (!error) {
        Execute(args);
        return true;
    }
    CCharString usage = Name.ToLower() + " takes ";
    if (ParamTypes.size() == 0) {
        usage += "no parameters";
    } else {
        usage += GFIntToCharString(ParamTypes.size()) + " parameter";
        usage += (ParamTypes.size() == 1) ? ": " : "s: ";
        for (long j = 0; j < ParamTypes.size(); ++j)
            usage += ParamTypes.GetParameterTypeAsString(j) + " ";
    }
    console.SendMessageToUser(usage);
    return false;
}
