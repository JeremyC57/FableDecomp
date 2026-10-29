struct Unsigned16ArrayRange
{
    unsigned long begin;
    unsigned long end;
};

namespace std {
template <class _CharT, class _IntT> class __char_traits_base { public:   // STLport 4.6 char_traits.h; wint_t == unsigned short
    static int __cdecl compare(const _CharT* __s1, const _CharT* __s2, unsigned int __n);   // 0x00403C60
};
}

unsigned long __cdecl Compare_Unsigned16Array(
    const Unsigned16ArrayRange* lhs,
    const Unsigned16ArrayRange* rhs)
{
    int count = ((int)lhs->end - (int)lhs->begin) >> 1;
    if (count == (((int)rhs->end - (int)rhs->begin) >> 1)) {
        count = std::__char_traits_base<unsigned short, unsigned short>::compare((const unsigned short*)lhs->begin, (const unsigned short*)rhs->begin, count);
        if (count == 0) {
            return 1;
        }
    }
    return 0;
}