// CConsole::ConsoleVariableListContainingString(CConsole& console, const vector<CGenericVar>& params) -- console command "VarListContaining": print the name of every registered console variable whose name contains the first parameter (case-insensitive)
// Leaf: ?ConsoleVariableListContainingString@CConsole@@SIXAAV1@ABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009EB9B0, 376 bytes, static __fastcall (console in ecx, params in edx), `ret`.
// Identity: Ego_r.pdb `?ConsoleVariableListContainingString@CConsole@@SIXAAV1@ABV?$vector@VCGenericVar@@V?$allocator@
//   VCGenericVar@@@std@@@std@@@Z`. Donor order ConsoleConsoleListContainingString, ConsoleCommandList,
//   ConsoleCommandListContainingString, ConsoleVariableList, ConsoleVariableListContainingString matches retail
//   0x009EB590 / 0x009EB690 (both landed) / 0x009EB760 / 0x009EB8E0 (landed) / this row; CConsole's registration code
//   stores this address in the console command named "VarListContaining" (0x009ED646, string 0x0129C6D0), and the
//   body compares the input's type with "variable" (0x0122E5DC).
// Callees: the input's virtual slot 3 GetTypeAsString (a CCharString by value), CBasicString<char>::operator==(const
//   char*) 0x004115A0, CCharString::CCharString(const CCharString&) 0x0099EC30, CCharString::GetAsLowerCase 0x0099ED10,
//   strstr 0x00BFEB3A, ~CCharString 0x0099EAE0, CConsole::SendMessageToUser 0x009EB260.
// Shape: ConsoleCommandListContainingString (0x009EB760) with "variable"; the landed ConsoleCommandList (0x009EB690) walk with ConsoleConsoleListContainingString's (0x009EB590) test
//   folded into one `&&`: `type == "variable" && name.GetAsLowerCase().Contains(params[0].GetString().GetAsLowerCase())`
//   (the inline CGenericVar::GetString by value builds the copy in its frame slot; a CCharString(...) temporary is
//   addressed through the constructor's eax instead)
//   in a bool local, the four conditional temporaries tracked in ebx (bits 1, 2, 4, 8) and destroyed before the
//   SendMessageToUser branch.
// Layout: retail CConsole RegisteredConsoleInputs (map<CCharString, CCountedPointer<CConsoleInputBase> >) header at
//   +0x40 (as the landed 0x009EB590 / 0x009EB690); CGenericVar ValString +0x08 (Ego_r.pdb, 0x1c bytes).
extern "C" char* __cdecl strstr(const char* s, const char* sub);
extern "C" int __cdecl strcmp(const char* a, const char* b);
#pragma intrinsic(strcmp)

#pragma pack(push, 1)
template <class T> class CBasicString { public:
    char*         PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
    bool operator==(const char* text) const;        // 0x004115A0
};
class CCharStringData { public:
    CBasicString<char> Data;
    long             NoRefs;
};
#pragma pack(pop)
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const CCharString& other);          // 0x0099EC30
    ~CCharString();                                 // 0x0099EAE0
    CCharString GetAsLowerCase() const;             // 0x0099ED10
    __forceinline const char* GetAsCharArray() const
    {
        return PStringData == 0 ? "" : PStringData->Data.PData;
    }
    __forceinline bool Contains(const CCharString& text) const
    {
        return strstr(GetAsCharArray(), text.GetAsCharArray()) != 0;
    }
    __forceinline bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};

class CGenericVar { public:                                // Ego_r.pdb, 0x1c bytes
    void*         _vtable;
    long          Type;
    CCharString   ValString;                           // +0x08
    long          ValSLONG;
    unsigned long ValULONG;
    bool          ValBool;
    float         ValFloat;
    CCharString GetString() const { return ValString; }   // Ego_r.pdb ?GetString@CGenericVar@@QBE?AVCCharString@@XZ
};
namespace std {
template <class _Tp> class allocator { public: };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector { public:
    _Tp* _M_start; _Tp* _M_finish; _Tp* _M_end_of_storage;
    const _Tp& operator[](unsigned int __n) const { return *(_M_start + __n); }
};
}

class CConsole;
class CConsoleInputBase { public:
    virtual ~CConsoleInputBase();
    virtual bool ProcessCommandLine(const CCharString& line, CConsole& console);
    virtual CCharString GetHelp();
    virtual CCharString GetTypeAsString() const;    // slot 3 (+0x0c): "command" / "variable"
    CCharString Name;                               // +0x04
    const CCharString& GetName() const { return Name; }
};
struct CInputPointer {                              // CCountedPointer<CConsoleInputBase>
    CConsoleInputBase* Data;
    void*              Info;
    CConsoleInputBase* operator->() const { return Data; }
};
struct SInputNode {                                 // STLport _Rb_tree_node<pair<const CCharString, CCountedPointer<..>>>
    unsigned char  _M_color;
    SInputNode*    _M_parent;
    SInputNode*    _M_left;
    SInputNode*    _M_right;
    CCharString    first;                           // +0x10
    CInputPointer  second;                          // +0x14
};
struct CInputIterator {
    SInputNode* _M_node;
    SInputNode* operator->() const { return _M_node; }
    __forceinline CInputIterator& operator++()
    {
        SInputNode* x = _M_node;
        if (x->_M_right) {
            x = x->_M_right;
            while (x->_M_left) x = x->_M_left;
        } else {
            SInputNode* y = x->_M_parent;
            while (x == y->_M_right) { x = y; y = y->_M_parent; }
            if (x->_M_right != y) x = y;
        }
        _M_node = x;
        return *this;
    }
};
class CConsole { public:
    unsigned char  _pad00[0x40];                    // vptr, maps, command lines, HelpBarString
    SInputNode*    RegisteredConsoleInputsHeader;   // +0x40
    void SendMessageToUser(const CCharString& message);   // 0x009EB260
    static void __fastcall ConsoleVariableListContainingString(CConsole& console, const std::vector<CGenericVar>& params);
};

void __fastcall CConsole::ConsoleVariableListContainingString(CConsole& console, const std::vector<CGenericVar>& params)
{
    CInputIterator it;
    it._M_node = console.RegisteredConsoleInputsHeader->_M_left;
    for (; it._M_node != console.RegisteredConsoleInputsHeader; ++it)
    {
        bool found = it->second->GetTypeAsString() == "variable"
            && it->second->GetName().GetAsLowerCase().Contains(params[0].GetString().GetAsLowerCase());
        if (found)
            console.SendMessageToUser(it->second->GetName());
    }
}