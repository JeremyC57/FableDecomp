// std::basic_string::assign(const char* f, const char* l) -- STLport range assign: overwrite in place and erase the tail when the range fits, else overwrite size() bytes and append the rest
// Leaf: ?assign@
// Retail: Fable.exe (TLC, VC7.1) @ 0x005693B0, 151 bytes, thiscall `ret 8`, returns *this. Callers include the
//   transaction constructor 0x00569C50 (three `= ""` assignments: push lit ; push lit). The catalog label
//   "Vector_Insert" is descriptive.
// Callees: memmove @ 0x00BFEAE6 (char_traits::move inside erase), basic_string::append(const char*, const char*,
//   const forward_iterator_tag&) @ 0x00567D60 (landed as CNetString_Retail::Append; the second TU copy of the
//   0x00567400 instantiation).
// Layout: STLport basic_string<char> {_M_start, _M_finish, _M_end_of_storage} (12 bytes).
// Shape: STLport 4.x `assign(const _CharT*, const _CharT*)` verbatim: `n = l - f; if ((size_type)n <= size())
//   { _Traits::copy(_M_start, f, n); erase(begin() + n, end()); } else { _Traits::copy(_M_start, f, size());
//   append(f + size(), l); }`. The traits copy/move are `n == 0 ? s1 : memcpy/memmove(..)` (the memcpy is the
//   /Oi `rep movsd ; rep movsb` intrinsic, the zero tests stay); erase moves the terminator with
//   `(finish - last) + 1` (not folded); append dispatches to the forward-iterator overload, whose tag
//   temporary takes the dead `l` argument slot.
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size); // 0x00BFEAE6
extern "C" void* __cdecl memcpy(void* dst, const void* src, unsigned int size);
#pragma intrinsic(memcpy)

namespace std {

struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };

struct char_traits_char {
    static char* move(char* __s1, const char* __s2, unsigned int __n)
    {
        return (__n == 0 ? __s1 : (char*)memmove(__s1, __s2, __n * sizeof(char)));
    }
    static char* copy(char* __s1, const char* __s2, unsigned int __n)
    {
        return (__n == 0 ? __s1 : (char*)memcpy(__s1, __s2, __n * sizeof(char)));
    }
};

class basic_string {
public:
    typedef unsigned int size_type;
    typedef char* iterator;
    char* _M_start;
    char* _M_finish;
    char* _M_end_of_storage;

    size_type size() const { return _M_finish - _M_start; }
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }

    iterator erase(iterator __first, iterator __last)
    {
        if (__first != __last) {
            // The move includes the terminating _CharT().
            char_traits_char::move(__first, __last, (_M_finish - __last) + 1);
            char* __new_finish = _M_finish - (__last - __first);
            _M_finish = __new_finish;
        }
        return __first;
    }

    basic_string& append(const char* __first, const char* __last, const forward_iterator_tag&);   // 0x00567D60
    basic_string& append(const char* __first, const char* __last)
    {
        return append(__first, __last, forward_iterator_tag());
    }

    basic_string& assign(const char* __f, const char* __l);
};

basic_string& basic_string::assign(const char* __f, const char* __l)
{
    int __n = __l - __f;
    if ((size_type)__n <= size()) {
        char_traits_char::copy(_M_start, __f, __n);
        erase(begin() + __n, end());
    }
    else {
        char_traits_char::copy(_M_start, __f, size());
        append(__f + size(), __l);
    }
    return *this;
}

}