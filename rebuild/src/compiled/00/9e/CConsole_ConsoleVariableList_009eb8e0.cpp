// CConsole::ConsoleVariableList(CConsole&, const vector&) -- console command: print the name of every registered input whose type is "variable"
// Retail: Fable.exe (TLC, VC7.1) @ 0x009EB8E0, 196 bytes (208 with padding). PDB: ?ConsoleVariableList@CConsole@@SIXAAV1@
//   ABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z (static, __fastcall; the parameters are unused).
//   Catalog label "CConsole_ListVariablesCommand" was close; the PDB name is ConsoleVariableList.
// Callees: the input's virtual slot 3 (+0x0c, a type string by value; "variable" 0x0122E5DC, 9 bytes with the NUL),
//   CBasicString<char>::operator==(const char*) 0x004115A0, ~CCharString 0x0099EAE0, CConsole::SendMessageToUser 0x009EB260.
// Shape: byte-for-byte the twin of CConsole::ConsoleCommandList 0x009EB690 with the "variable" literal: in-order walk of
//   RegisteredConsoleInputs, inline CCharString::operator==(const char*) (null-block arm `strcmp(text, "") == 0`),
//   the type temporary destroyed before the branch, the input's Name (+4) sent to the user.
// Layout: retail CConsole RegisteredConsoleInputs header +0x40 (as the landed 0x009EB590 / 0x009EB430).
// Leaf: ?ConsoleVariableList@CConsole_Retail@@SIXAAU1@ABU?$vector@UCGenericVar@@@_STL@@@Z
extern "C" int __cdecl strcmp(const char* a, const char* b);
#pragma intrinsic(strcmp)

#pragma pack(push, 1)
struct CBasicStringChar {
    char*         PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
    bool operator==(const char* text) const;        // 0x004115A0
};
struct CCharStringData {
    CBasicStringChar Data;
    long             NoRefs;
};
#pragma pack(pop)
struct CCharString {
    CCharStringData* PStringData;
    ~CCharString();                                 // 0x0099EAE0
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

struct CGenericVar {                                // PDB 0x1c bytes
    void*         _vtable;
    long          Type;
    CCharString   ValString;
    long          ValSLONG;
    unsigned long ValULONG;
    bool          ValBool;
    float         ValFloat;
};
namespace _STL { template <class T> struct vector { T* _M_start; T* _M_finish; T* _M_end_of_storage; }; }

struct CConsoleInputBase {
    virtual ~CConsoleInputBase();
    virtual void v01();
    virtual void v02();
    virtual CCharString GetTypeAsString() const;    // slot 3 (+0x0c): "variable" for console variables
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
struct CConsole_Retail {
    unsigned char  _pad00[0x40];                    // vptr, maps, command lines, HelpBarString
    SInputNode*    RegisteredConsoleInputsHeader;   // +0x40 map<CCharString, CCountedPointer<CConsoleInputBase>> header
    void SendMessageToUser(const CCharString& message);     // 0x009EB260
    static void __fastcall ConsoleVariableList(CConsole_Retail& console, const _STL::vector<CGenericVar>& params);
};

void __fastcall CConsole_Retail::ConsoleVariableList(CConsole_Retail& console, const _STL::vector<CGenericVar>& params)
{
    CInputIterator it;
    it._M_node = console.RegisteredConsoleInputsHeader->_M_left;
    for (; it._M_node != console.RegisteredConsoleInputsHeader; ++it)
    {
        if (it->second->GetTypeAsString() == "variable")
            console.SendMessageToUser(it->second->GetName());
    }
}