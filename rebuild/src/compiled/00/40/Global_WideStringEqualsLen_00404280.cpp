namespace std {
template <class _CharT, class _IntT> class __char_traits_base { public:   // STLport 4.6 char_traits.h; wint_t == unsigned short
    static int __cdecl compare(const _CharT* __s1, const _CharT* __s2, unsigned int __n);   // 0x00403C60
};
}

struct WideStringRange
{
    unsigned short* begin;
    unsigned short* end;
};

int __cdecl WideString_EqualsLen(
    const WideStringRange* wideString, const unsigned short* text)
{
    int length = 0;
    unsigned short ch = *text;
    while (ch != 0)
    {
        ++length;
        ch = text[length];
    }
    const long byteSpan = reinterpret_cast<const char*>(wideString->end) -
                          reinterpret_cast<const char*>(wideString->begin);
    if ((byteSpan >> 1) == length &&
        (length = std::__char_traits_base<unsigned short, unsigned short>::compare(wideString->begin, text, length)) == 0)
        return 1;
    return 0;
}
