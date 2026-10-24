// std::vector::_Xlen() const -- VC7.1 Dinkumware vector's length-overflow thrower: `throw length_error("vector<T> too long")` (the message becomes a std::string temporary, the logic_error base copies it into its _Str)
// Bounds: int3-after-noreturn row, landed against the N+1 span: int3-after-noreturn: every instruction matches (relocations masked); VC7.1 emits the 0xCC after the final `call _CxxThrowException` that the chunk cutter stripped, land against the N+1 span
// Leaf: ?_Xlen@?$vector@
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C138F0, 112 bytes + the int3 after the noreturn call, __fastcall
//   (`this` unused), EH-framed (handler 0x012118B2).
// Identity: the libpk audio library uses VC7.1's own Dinkumware STL (MSVCP71 basic_string imports): the string
//   constructors are `??0?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAE@PBD@Z` (import 0x0143FF98) and
//   `...QAE@ABV01@@Z` (0x0143FFA0), the exception base constructor `??0exception@@QAE@XZ` (MSVCR71, 0x0143FFF8); the
//   logic_error / length_error vtables 0x012B263C / 0x012B2644 and the ThrowInfo 0x0136AA24 are this TU's. Callers: the
//   request vector's _Insert_n @ 0x00C14270 (the worker queue's request vector; `mov ecx,esi ; call` when the grown
//   size would pass max_size 0x3FFFFFFF) and 0x00C26AE5; the second element type's twin is @ 0x00C13840.
// Shape: EH state 0 = the message string, 1 = the exception base built (the logic_error vtable store), back to 0 with
//   the length_error vtable store before the throw; the string temporary lives at esp+0, the 0x28-byte length_error at
//   esp+0x1c.
// Label correction: the catalog's `StdVector_ThrowTooLongExceptionVariant` is descriptive; the PDB-style name is vector<T>::_Xlen.
class exception {
public:
    __declspec(dllimport) exception();                       // MSVCR71 import 0x0143FFF8
    __declspec(dllimport) exception(const exception&);
    __declspec(dllimport) virtual ~exception();
    __declspec(dllimport) virtual const char* what() const;
private:
    const char* _m_what;
    int _m_doFree;
};

namespace std {
template<class _Elem> struct char_traits { };
template<class _Ty> class allocator { };

template<class _Elem, class _Traits, class _Ax> class basic_string {     // 0x1c bytes
public:
    __declspec(dllimport) basic_string(const _Elem* _Ptr);              // MSVCP71 import 0x0143FF98
    __declspec(dllimport) basic_string(const basic_string& _Right);     // MSVCP71 import 0x0143FFA0
    __declspec(dllimport) ~basic_string();
    __declspec(dllimport) const _Elem* c_str() const;
private:
    _Ax _Alval;
    union { _Elem _Buf[16]; _Elem* _Ptr; } _Bx;
    unsigned int _Mysize;
    unsigned int _Myres;
};
typedef basic_string<char, char_traits<char>, allocator<char> > string;

class logic_error : public exception {                                   // vtable 0x012B263C
public:
    explicit logic_error(const string& _Message) : _Str(_Message) { }
    virtual ~logic_error() throw() { }
    virtual const char* what() const throw() { return (_Str.c_str()); }
private:
    string _Str;
};

class length_error : public logic_error {                                // vtable 0x012B2644
public:
    explicit length_error(const string& _Message) : logic_error(_Message) { }
    virtual ~length_error() throw() { }
};

template<class _Ty, class _Ax = allocator<_Ty> > class vector {
public:
    void _Xlen() const;
    _Ax _Alval;                                                          // +0x00
    _Ty* _Myfirst;                                                       // +0x04
    _Ty* _Mylast;                                                        // +0x08
    _Ty* _Myend;                                                         // +0x0c
};

template<class _Ty, class _Ax> void vector<_Ty, _Ax>::_Xlen() const
{
    throw length_error("vector<T> too long");
}
}

struct PKCRequest_00c14630;
template class std::vector<PKCRequest_00c14630*>;