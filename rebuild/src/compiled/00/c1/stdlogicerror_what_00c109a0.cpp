// std::logic_error::what() const -- Dinkumware (VC7.1) logic_error::what: the message string's characters (_Str.c_str())
// Leaf: ?what@logic_error@std@@UBEPBDXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C109A0, 14 bytes, thiscall: `cmp [ecx+24h],10h ; jb buf ; mov eax,[ecx+10h] ; ret`
//   / `lea eax,[ecx+10h] ; ret` -- the small-string test of basic_string::_Myptr().
// Identity (proven): slot 1 of the std::logic_error vtable 0x012B263C (RTTI; the length_error vtable 0x012B2644 shares
//   the slot), the throw type chain of ThrowInfo 0x0136AA24 being length_error : logic_error : exception. The VC7.1
//   <stdexcept> body is `return (_Str.c_str());`, inlined in the libpk TU that throws length_error.
// Layout: exception {vptr, _m_what, _m_doFree} (12 bytes, global namespace in VC7.1), then the Dinkumware string _Str
//   at +0x0c: _Alval +0x0c, _Bx (16-byte buffer / pointer) +0x10, _Mysize +0x20, _Myres +0x24. Hand model of the
//   VC7.1 headers (the real <stdexcept> drags MSVCP71 imports into the object).
class exception {                               // VC7.1 <exception>: global ::exception
public:
    virtual ~exception();
    virtual const char* what() const;
private:
    const char* _m_what;                        // +0x04
    int         _m_doFree;                      // +0x08
};

namespace std {

class allocator_char { };                       // the empty _Alval

class string {                                  // basic_string<char> data words (VC7.1 Dinkumware)
public:
    enum { _BUF_SIZE = 16 };
    const char* c_str() const { return _Myptr(); }
private:
    const char* _Myptr() const { return (_BUF_SIZE <= _Myres ? _Bx._Ptr : _Bx._Buf); }
    allocator_char _Alval;                      // +0x00
    union _Bxty {
        char  _Buf[_BUF_SIZE];
        char* _Ptr;
    } _Bx;                                      // +0x04
    unsigned int _Mysize;                       // +0x14
    unsigned int _Myres;                        // +0x18
};

class logic_error : public exception {
public:
    virtual const char* what() const;
private:
    string _Str;                                // +0x0c
};

const char* logic_error::what() const
{
    return (_Str.c_str());
}

}   // namespace std