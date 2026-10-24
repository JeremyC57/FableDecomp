// std::operator!=(const basic_string& s, const char* text) -- STLport string-vs-C-string inequality: lengths first, then the characters
// Leaf: ??9std
// Retail: Fable.exe (TLC, VC7.1) @ 0x00566EA0, 79 bytes, `__cdecl` (`ret`, 10 callers in the scoreboard /
//   HTTP code: 0x0056156C.., 0x0083FA1C, 0x008412BA). Label correction: the catalog row "String_Compare" is
//   descriptive and the landed caller 0x005615C0 declares this address as operator==, but the body returns
//   FALSE when the sizes and bytes agree (`mov eax,1 ; test al,al ; sete cl`): it is STLport's
//   `operator!=(const basic_string&, const _CharT*)` = `!(s == text)` with operator== inlined.
// Callees: none (strlen and memcmp are the /Oi intrinsics: the `inc eax ; test cl,cl` scan and `repe cmpsb`).
// Shape: STLport 4.x verbatim: `size_t n = _Traits::length(s2); return s.size() == n &&
//   _Traits::compare(s.data(), s2, n) == 0;` inlined under `!`, which leaves the materialised 1/0 then the
//   `sete` inversion in both tails.
extern "C" unsigned int __cdecl strlen(const char* text);
extern "C" int __cdecl memcmp(const void* a, const void* b, unsigned int size);
#pragma intrinsic(strlen, memcmp)

namespace std {

struct char_traits_char {
    static unsigned int length(const char* __s) { return strlen(__s); }
    static int compare(const char* __s1, const char* __s2, unsigned int __n) { return memcmp(__s1, __s2, __n); }
};

class basic_string {
public:
    char* _M_start;
    char* _M_finish;
    char* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    const char* data() const { return _M_start; }
};

inline bool operator==(const basic_string& __s, const char* __s2)
{
    unsigned int __n = char_traits_char::length(__s2);
    return __s.size() == __n && char_traits_char::compare(__s.data(), __s2, __n) == 0;
}

bool operator!=(const basic_string& __s, const char* __s2)
{
    return !(__s == __s2);
}

}