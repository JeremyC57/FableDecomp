// StdEncryptString(const std::string& text, const std::string& key) -- (anonymous namespace) encrypt a narrow STL string: fill a zeroed char vector of size(), run EncryptBuffer (CryptoMatic mode 1) from the text into it with the key, and return the first `length` encrypted bytes as a new string
// Leaf: ?StdEncryptString@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0055F140, 170 bytes, `ret 4` (/Gr __fastcall: the hidden std::string result in
//   ecx, &text in edx, &key on the stack). Byte twin of StdDecryptString 0x0055F090 (no +32, mode 1).
// Callers: 0x0055F35D (the CCharString EncryptString front end 0x0055F2D0; not in the manifest), 0x00561204
//   (CTCHeroOnlineScoreboard::ProcessLogin 0x00560D00).
// Callees: std::vector<char>(n, const char&, const allocator&) 0x00567560, CryptoMatic 0x0055E2C0 (EncryptBuffer
//   0x0055E510 inlined), basic_string::_M_range_initialize<char*>(f, l, const forward_iterator_tag&) 0x00567D00 (twice),
//   free 0x00BFEA14.
// Label correction: catalogued as "CryptoEncrypt_ToStruct"; Ego_r.pdb
//   `?StdEncryptString@?A0xf010c0d0@@YI?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@ABV23@0@Z`.
// Instance: STLport 4.6's range constructor and `_M_range_initialize` are member templates and `char_traits` is a class:
//   both strings are built from `char*` ranges (`&buffer[0]`, then the copy constructor's `__s._M_start`), so both calls
//   are the `<char*>` instance 0x00567D00 (`??$_M_range_initialize@PAD@...`); `<const char*>` is 0x005672D0.
// Layout: STLport 4.x 12-byte string / vector { _M_start, _M_finish, _STLP_alloc_proxy _M_end_of_storage }.
// Same-TU stand-ins (as retail's whole-program view): the vector fill constructor (RELOCATION_MATCH against
//   0x00567560), `_M_range_initialize` (the landed 0x00567D00 body) and `_M_throw_length_error` (landed 0x00565A30);
//   they let the local string reuse the vector's frame slot and keep both start pointers in registers.
// Shape: as StdDecryptString: `vector<char> buffer(size, 0)`, a named local string, `return result;`.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
extern "C" void* __cdecl memset(void* dest, int value, unsigned int count);
#pragma intrinsic(memset)
extern "C" int __cdecl puts(const char* text);                // 0x00BFEAF2
extern "C" __declspec(noreturn) void __cdecl abort();        // 0x00BFEAEC
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6

namespace std {
struct input_iterator_tag { input_iterator_tag() { } };
struct forward_iterator_tag : input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : forward_iterator_tag { bidirectional_iterator_tag() { } };
struct random_access_iterator_tag : bidirectional_iterator_tag { random_access_iterator_tag() { } };

template <class T> class char_traits { };
template <class T> class allocator {
public:
    allocator() { }
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
    unsigned int max_size() const { return (unsigned int)(-1) / sizeof(T); }
    void deallocate(T* p, unsigned int) { if (p != 0) free(p); }
};

template <class T, class A> struct _STLP_alloc_proxy : A {
    T _M_data;
    _STLP_alloc_proxy(const A& a, T p) : A(a), _M_data(p) { }
};

template <class C, class Tr, class A> class basic_string {   // STLport 4.x: start, finish, end of storage
public:
    C* _M_start;                                                // +0x00
    C* _M_finish;                                               // +0x04
    _STLP_alloc_proxy<C*, A> _M_end_of_storage;                 // +0x08

    template <class It> basic_string(It f, It l, const A& a = A()) : _M_start(0), _M_finish(0), _M_end_of_storage(a, (C*)0)
    {
        _M_range_initialize(f, l);
    }
    basic_string(const basic_string& s) : _M_start(0), _M_finish(0), _M_end_of_storage(A(), (C*)0)
    {
        _M_range_initialize(s._M_start, s._M_finish);
    }
    ~basic_string() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    template <class It> void _M_range_initialize(It f, It l) { _M_range_initialize(f, l, random_access_iterator_tag()); }
    __declspec(noinline) __declspec(noreturn) void _M_throw_length_error() const   // 0x00565A30 (same-TU body)
    {
        puts("basic_string");
        abort();
    }
    unsigned int max_size() const { return _M_end_of_storage.max_size() - 1; }
    void _M_allocate_block(unsigned int n)
    {
        if (n <= max_size() + 1 && n > 0) {
            _M_start = _M_end_of_storage.allocate(n);
            _M_finish = _M_start;
            _M_end_of_storage._M_data = _M_start + n;
        }
        else
            _M_throw_length_error();
    }
    static C* __copy_trivial(const C* first, const C* last, C* result)
    {
        return last == first ? result : (C*)memmove(result, first, (last - first) * sizeof(C)) + (last - first);
    }
    void _M_terminate_string() { *_M_finish = 0; }
    template <class It> __declspec(noinline) void _M_range_initialize(It f, It l, const forward_iterator_tag&)   // <char*> 0x00567D00 (same-TU body)
    {
        int n = l - f;
        _M_allocate_block(n + 1);
        _M_finish = __copy_trivial(f, l, _M_start);
        _M_terminate_string();
    }
    const C* c_str() const { return _M_start; }
    unsigned int size() const { return _M_finish - _M_start; }
};
typedef basic_string<char, char_traits<char>, allocator<char> > string;

inline char* uninitialized_fill_n(char* first, unsigned int n, const char& val)
{
    memset(first, (unsigned char)val, n);
    return first + n;
}

template <class T, class A = allocator<T> > class vector {      // STLport 4.x: start, finish, end of storage
public:
    T* _M_start;                                                // +0x00
    T* _M_finish;                                               // +0x04
    _STLP_alloc_proxy<T*, A> _M_end_of_storage;                 // +0x08

    __declspec(noinline) vector(unsigned int n, const T& val, const A& a = A())   // 0x00567560 (same-TU body)
        : _M_start(0), _M_finish(0), _M_end_of_storage(a, (T*)0)
    {
        _M_start = _M_end_of_storage.allocate(n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + n;
        _M_finish = uninitialized_fill_n(_M_start, n, val);
    }
    ~vector() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    T& operator[](unsigned int n) { return *(_M_start + n); }
};
}

namespace {
int __fastcall CryptoMatic(const char* source, char* destination, int length, const char* key, char encrypt);   // 0x0055E2C0
int __fastcall DecryptBuffer(const char* source, char* destination, int length, const char* key)   // 0x0055E4F0
{
    return CryptoMatic(source, destination, length, key, 0);
}
int __fastcall EncryptBuffer(const char* source, char* destination, int length, const char* key)   // 0x0055E510
{
    return CryptoMatic(source, destination, length, key, 1);
}

std::string __fastcall StdEncryptString(const std::string& text, const std::string& key)
{
    std::vector<char> buffer(text.size(), 0);
    int length = EncryptBuffer(text.c_str(), &buffer[0], text.size(), key.c_str());
    std::string result(&buffer[0], &buffer[0] + length);
    return result;
}
}