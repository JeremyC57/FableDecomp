// CScriptDefinitionManager::InitAndCompile() -- build the script-def symbol path list (bank headers, PC bank headers, Data\Defs\ScriptDefs headers, thing_def.h, ScriptDefs defs) and compile path list (ScriptDefs .tpl and .def), hand both to the definition manager, compile, drop the symbols, save the def string table when binary definitions are saved, and set the global script def
// Leaf: ?InitAndCompile@CScriptDefinitionManager@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CD3F50, 867 bytes, plain `ret` (slot 2 of the CScriptDefinitionManager vtable 0x012C2648, whose slot 0 is
//   the ??_G at 0x00CD4330); name in Ego_r.pdb and FableWin.pdb (?InitAndCompile@CScriptDefinitionManager@@UAEXXZ). No EH frame.
// Callees: CWideString(const unsigned short*) 0x0099B6B0 / copy 0x0099B720 / dtor 0x0099B510; operator+ 0x0099BF30 / 0x0099BE70 (__fastcall,
//   hidden result in ecx); NBankFileManager::GetHeaderDir 0x009A76A0 (Ego_r ?GetHeaderDir@NBankFileManager@@YI?AVCWideString@@XZ, returns
//   SymbolHeaderDir); vector<CWideString>::_M_insert_overflow 0x00412330 and operator= 0x00433DF0; CDefinitionManager::Compile 0x009B08C0 and
//   ClearSymbols 0x009ACB10; CDefStringTable::SaveTable(bool) 0x009D4CE0 on GDefStringTable 0x013CA828; SetGlobalScriptDef 0x00CBF647; free 0x00BFEA14.
// Shape: the "Data\Defs\" directory is a CWideString temporary built right where it is used: retail pushes the right operand of operator+ first
//   and builds the directory after it (a call to the inline NResourceDirectoryNames::GetDefDir builds it before the push, probes 5-10).
//   STLport 4.6 push_back is inline (null-checked placement copy, else _M_insert_overflow with the __false_type tag); the first push to each empty
//   vector folds to position 0. VC7.1 keeps the vectors' finish pointers in esi/edi across calls only when the two members that receive a
//   vector's address (_M_insert_overflow, operator=) have same-TU auto_inline(off) bodies (GOTCHAS retry 10f/11b); the bodies below are
//   stand-ins, graded only through the caller. __false_type needs a user ctor (no zero byte store).
// Globals: 0x0138E188 is the descriptive g_bSaveBinaryDefinitions (as in the landed CDefinitionManager::Compile); GDefStringTable is PDB.
extern "C" void free(void*);
extern "C" void* __cdecl malloc(unsigned int);
class CWideString {
public:
    unsigned short* PString;
    CWideString(const unsigned short* text);           // 0x0099B6B0
    CWideString(const CWideString& other);             // 0x0099B720
    ~CWideString();                                    // 0x0099B510
};
CWideString __fastcall operator+(const CWideString& left, const CWideString& right);   // 0x0099BE70
CWideString __fastcall operator+(const CWideString& left, const unsigned short* right); // 0x0099BF30
namespace NBankFileManager { CWideString __fastcall GetHeaderDir(); }                  // 0x009A76A0
namespace NResourceDirectoryNames {  }
inline void* operator new(unsigned int, void* p) { return p; }
namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __p) { __p->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{ for (; __first != __last; ++__first) _Destroy(&*__first); }
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base() { if (_M_start) free(_M_start); }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    explicit vector(const _Alloc& __a = _Alloc()) : _Vector_base<_Tp, _Alloc>(__a) { }
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
    vector<_Tp, _Alloc>& operator=(const vector<_Tp, _Alloc>& __x);    // 0x00433DF0
protected:
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend);  // 0x00412330
public:
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        } else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
};
}
namespace std {
#pragma auto_inline(off)
template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend)
{
    unsigned int __old_size = this->_M_finish - this->_M_start;
    unsigned int __len = __old_size + (__old_size > __fill_len ? __old_size : __fill_len);
    _Tp* __new_start = (_Tp*)malloc(__len * sizeof(_Tp));
    _Tp* __new_finish = __new_start;
    for (_Tp* p = this->_M_start; p != __position; ++p, ++__new_finish) _Construct(__new_finish, *p);
    for (; __fill_len > 0; --__fill_len, ++__new_finish) _Construct(__new_finish, __x);
    if (!__atend)
        for (_Tp* p = __position; p != this->_M_finish; ++p, ++__new_finish) _Construct(__new_finish, *p);
    _Destroy(this->_M_start, this->_M_finish);
    if (this->_M_start) free(this->_M_start);
    this->_M_start = __new_start;
    this->_M_finish = __new_finish;
    this->_M_end_of_storage._M_data = __new_start + __len;
}
template <class _Tp, class _Alloc>
vector<_Tp, _Alloc>& vector<_Tp, _Alloc>::operator=(const vector<_Tp, _Alloc>& __x)
{
    if (&__x != this) {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_finish = this->_M_start;
        for (const _Tp* p = __x._M_start; p != __x._M_finish; ++p) { _Construct(this->_M_finish, *p); ++this->_M_finish; }
    }
    return *this;
}
#pragma auto_inline(on)
}
class CDefStringTable { public: void SaveTable(bool lock); };   // 0x009D4CE0
extern CDefStringTable GDefStringTable;                       // 0x013CA828
extern bool g_bSaveBinaryDefinitions;                         // 0x0138E188
void __fastcall SetGlobalScriptDef();                         // 0x00CBF647
class CDefinitionManager {
public:
    virtual ~CDefinitionManager();
    virtual void v1();
    virtual void InitAndCompile();
    unsigned char _pad04[0x40 - 0x04];
    std::vector<CWideString> SymbolPathList;           // +0x40
    std::vector<CWideString> CompilePathList;          // +0x4c
    unsigned char _pad58[0xd8 - 0x58];
    void Compile();                                    // 0x009B08C0
    void ClearSymbols();                               // 0x009ACB10
};
class CScriptDefinitionManager : public CDefinitionManager {
public:
    virtual void InitAndCompile();
};
void CScriptDefinitionManager::InitAndCompile()
{
    std::vector<CWideString> symbolPaths;
    std::vector<CWideString> compilePaths;
    CWideString platform((const unsigned short*)L"pc\\");
    symbolPaths.push_back(NBankFileManager::GetHeaderDir() + (const unsigned short*)L"*.h");
    symbolPaths.push_back(NBankFileManager::GetHeaderDir() + platform + (const unsigned short*)L"*.h");
    symbolPaths.push_back(CWideString((const unsigned short*)L"Data\\Defs\\") + (const unsigned short*)L"ScriptDefs\\*.h");
    symbolPaths.push_back(CWideString((const unsigned short*)L"Data\\Defs\\") + (const unsigned short*)L"thing_def.h");
    symbolPaths.push_back(CWideString((const unsigned short*)L"Data\\Defs\\") + (const unsigned short*)L"ScriptDefs\\*.def");
    compilePaths.push_back(CWideString((const unsigned short*)L"Data\\Defs\\") + (const unsigned short*)L"ScriptDefs\\*.tpl");
    compilePaths.push_back(CWideString((const unsigned short*)L"Data\\Defs\\") + (const unsigned short*)L"ScriptDefs\\*.def");
    SymbolPathList = symbolPaths;
    CompilePathList = compilePaths;
    Compile();
    ClearSymbols();
    if (g_bSaveBinaryDefinitions)
        GDefStringTable.SaveTable(true);
    SetGlobalScriptDef();
}