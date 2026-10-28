// CFontDataBank::CFontDataBank() -- CBankFile base, this vtable, the embedded CFontBankStateBlock's vtable, an empty Fonts array, then an empty Filename
// Leaf: ??0CFontDataBank@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A63630, 75 bytes. Name: Ego_r.pdb ??0CFontDataBank@@QAE@XZ; the catalog label agrees.
// Layout: engine/CFontDataBank.h (retail 0x130: CBankFile base 0x110, StateBlock +0x110, Fonts +0x120, Filename +0x12C);
//   Ego_r.pdb CFontBankStateBlock {vptr, CStateBlock State} (State has no constructor code), Fonts = CArray<CFontDataBankEntry*>.
// Vtables: 0x0129DA84 CFontDataBank, 0x0129DA5C CFontBankStateBlock. Callees: CBankFile::CBankFile 0x009CD480 (landed),
//   CCharString::CCharString() 0x0099E4B0, CCharString::operator=(const char*) 0x0099EFE0 with "" (0x0122D70E, the pooled
//   tail of a longer literal).
// Shape: members built in declaration order with inline constructors (the STLport vector's three zero stores through one
//   zero register); `Filename = "";` in the body.
class CBankFile {
public:
    virtual ~CBankFile();
    unsigned char _body[0x10c];                         // +0x004..+0x10F (engine/CBankFile.h)
    CBankFile();                                        // 0x009CD480
};
class CCharString {
public:
    void* PStringData;                                  // +0x00
    CCharString();                                      // 0x0099E4B0
    ~CCharString();
    CCharString& operator=(const char* text);           // 0x0099EFE0
};
class CStateBlock { public: unsigned char _data[0xc]; };
class CFontBankStateBlock {                             // vtable 0x0129DA5C
public:
    virtual ~CFontBankStateBlock() { }
    CStateBlock State;                                  // +0x04
    CFontBankStateBlock() { }
};
class CFontDataBankEntry;
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    vector() : _Vector_base<_Tp, _Alloc>(_Alloc()) { }
};
}
template <class T> class CArray : public std::vector<T> { };

class CFontDataBank : public CBankFile {               // vtable 0x0129DA84
public:
    CFontBankStateBlock        StateBlock;              // +0x110
    CArray<CFontDataBankEntry*> Fonts;                  // +0x120
    CCharString                Filename;                // +0x12C
    CFontDataBank();
    virtual ~CFontDataBank();
};

CFontDataBank::CFontDataBank()
{
    Filename = "";
}