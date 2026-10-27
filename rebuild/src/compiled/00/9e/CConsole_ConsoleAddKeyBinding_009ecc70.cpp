// CConsole::ConsoleAddKeyBinding(CConsole&, const std::vector<CGenericVar>&) -- console command "BindKey": binds a key character to a command string
// Retail: Fable.exe 0x009ECC70, 111 bytes: copies args[1].ValString then args[0].ValString into temporaries (CCharString
// copy ctor 0x0099EC30), takes the first character of the key string (GetAsCharArray: data or ""), assigns the binding
// to KeyBindingMap[key] (map<char, CCharString>::operator[] 0x009EFCF0, then CCharString::operator= 0x0099EFB0) and
// destroys the temporaries (0x0099EAE0) in reverse order.
// Identity: CConsole::Initialise 0x009ED190 registers this handler (`mov [edi+18h],9ECC70h`) under the command name
// "BindKey" (0x0129C734); Ego_r.pdb ?ConsoleAddKeyBinding@CConsole@@SIXAAV1@ABV?$vector@VCGenericVar@@V?$allocator@
// VCGenericVar@@@std@@@std@@@Z (public static __fastcall). The body is the inlined private
// AddKeyBinding(char, const CCharString&) over the by-value CGenericVar::GetString(); the call arguments evaluate
// right to left, which is why args[1]'s copy is built first.
// Layout: KeyBindingMap (map<char, CCharString>, STLport 0xC bytes) is CConsole's first member after the vptr (+4).
// Leaf: ?ConsoleAddKeyBinding@CConsole@@SIXAAV1@ABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z
class CCharStringData {
public:
    char* Data;                                    // +0x00
};

class CCharString {
public:
    CCharString(const CCharString& other);         // 0x0099EC30
    ~CCharString();                                // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    const char* GetAsCharArray() const { return PStringData == 0 ? "" : PStringData->Data; }
    CCharStringData* PStringData;
};

class CGenericVar {                                // Ego_r.pdb, 0x1C bytes, polymorphic
public:
    virtual ~CGenericVar();
    CCharString GetString() const { return ValString; }
    long          Type;                            // +0x04 EGenericVarType
    CCharString   ValString;                       // +0x08
    long          ValSLONG;                        // +0x0C
    unsigned long ValULONG;                        // +0x10
    bool          ValBool;                         // +0x14
    float         ValFloat;                        // +0x18
};

namespace std {
    template <class _Tp> class allocator { public: };
    template <class _Tp> struct less { };
    template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
    template <class _Value, class _Tp, class _MaybeReboundAlloc>
    class _STLP_alloc_proxy : public _MaybeReboundAlloc { public: _Value _M_data; };
    template <class _Tp, class _Alloc> class _Vector_base {
    public:
        _Tp* _M_start;
        _Tp* _M_finish;
        _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    };
    template <class _Tp, class _Alloc = allocator<_Tp> >
    class vector : protected _Vector_base<_Tp, _Alloc> {
    public:
        typedef const _Tp& const_reference;
        typedef unsigned int size_type;
        const _Tp* begin() const { return this->_M_start; }
        const_reference operator[](size_type __n) const { return *(begin() + __n); }
    };
    template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
    class map {
    public:
        typedef _Key key_type;
        _Tp& operator[](const key_type& __k);      // 0x009EFCF0 (out of line in this TU's build)
    private:
        unsigned char _M_t[0xC];                   // _Rb_tree: header, node count, compare
    };
}

class CConsole {
public:
    virtual ~CConsole();
    static void __fastcall ConsoleAddKeyBinding(CConsole& console, const std::vector<CGenericVar>& args);
private:
    void AddKeyBinding(char key, const CCharString& binding) { KeyBindingMap[key] = binding; }
    std::map<char, CCharString> KeyBindingMap;     // +0x04
};

void __fastcall CConsole::ConsoleAddKeyBinding(CConsole& console, const std::vector<CGenericVar>& args)
{
    console.AddKeyBinding(args[0].GetString().GetAsCharArray()[0], args[1].GetString());
}