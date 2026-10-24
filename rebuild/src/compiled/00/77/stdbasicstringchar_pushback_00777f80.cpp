// std::basic_string<char>::push_back(char c) -- STLport string push_back: grow to size + max(size, 1) through reserve when the block is full, then store the character and move the terminator
// Leaf: ?push_back@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00777F80, 84 bytes (+ int3 padding), thiscall `ret 4`.
// Callee: basic_string<char>::reserve(size_type) @ 0x00777DB0 (this chunk).
// Callers: 0x007749F4, 0x00774AAB, 0x00774AB5, 0x00841AD8.
// Label correction: catalogued as `CCharString_PushBack_Char`; the body is STLport `basic_string<char>::push_back`.
// Layout: STLport _String_base<char> {_M_start, _M_finish, _M_end_of_storage} (12 bytes).
// Shape: STLport 4.x verbatim: `if (_M_finish + 1 == _M_end_of_storage._M_data) reserve(size() + (max)(size(),
//   size_type(1))); _M_construct_null(_M_finish + 1); _Traits::assign(*_M_finish, c); ++_M_finish;` -- `max` takes both
//   arguments by reference, so the constant 1 and the size get the two frame words (`mov [esp+4],1`).
namespace std {

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

struct char_traits {
    static void assign(char& __c1, const char& __c2) { __c1 = __c2; }
};

class basic_string {
public:
    typedef unsigned int size_type;
    char* _M_start;
    char* _M_finish;
    char* _M_end_of_storage;

    size_type size() const { return _M_finish - _M_start; }
    void _M_construct_null(char* __p) { *__p = 0; }
    void reserve(size_type __res_arg);                                           // 0x00777DB0
    void push_back(char __c);
};

void basic_string::push_back(char __c)
{
    if (_M_finish + 1 == _M_end_of_storage)
        reserve(size() + (max)(size(), size_type(1)));
    _M_construct_null(_M_finish + 1);
    char_traits::assign(*_M_finish, __c);
    ++_M_finish;
}

}