// CNetString_Retail::Substr(unsigned int pos, unsigned int n) const -- STLport basic_string<char>::substr: bounds-check pos, then return a new string over [start+pos, start+pos+min(n, size()-pos))
// Grade: DIFFER(ours v retail) -- 84v86: the lane CSEs `m_start+pos`; retail recomputes the end as `min + start + pos` (`add eax,esi ; add eax,edx`). 4 attempts used; next: `Min(...) + m_start + pos` as the second argument
// Leaf: Substr
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00569330, length 86 bytes.
// Callees: _String_base::_M_throw_out_of_range() const @ 0x00565360 (puts("basic_string") + abort with exceptions
// off; `this` still in ecx), CNetString_Retail::RangeInitialize @ 0x00567D00 (the out-of-line _M_range_initialize).
// Shape: the hidden result pointer is the first stack argument (returned in eax, `ret 0Ch`). STLport
//   if (pos > size()) _M_throw_out_of_range();
//   return basic_string(start + pos, start + pos + min(n, size() - pos));
// min is the const-reference template (`b < a ? b : a`): `size()-pos` is a temporary homed in the dead `pos`
// argument slot and the selected reference is dereferenced (`lea eax,[esp+0Ch] ; jb ; lea eax,[esp+10h] ; mov eax,[eax]`).
// The range constructor zeroes the three words of the result and passes the empty iterator tag homed in the
// dead `n` slot (`lea edx,[esp+10h]`). Landed callers 0x0083F508.. are the HTTP URL accessors. The manifest
// label "CTCElectrocutedEffect::CopyDataRange" is propagated.
struct SForwardIteratorTag { SForwardIteratorTag() { } };

template <class T> inline const T& Min(const T& a, const T& b) { return b < a ? b : a; }

struct CNetString_Retail {                         // 12 bytes: start, finish, end of storage
    char* m_start;
    char* m_finish;
    char* m_endOfStorage;
    __declspec(noreturn) void ThrowOutOfRange() const;   // 0x00565360
    void RangeInitialize(const char* first, const char* last, const SForwardIteratorTag& tag);   // 0x00567D00
    CNetString_Retail(const char* first, const char* last) : m_start(0), m_finish(0), m_endOfStorage(0)
    {
        RangeInitialize(first, last, SForwardIteratorTag());
    }
    unsigned int size() const { return m_finish - m_start; }
    CNetString_Retail Substr(unsigned int pos, unsigned int n) const;
};

CNetString_Retail CNetString_Retail::Substr(unsigned int pos, unsigned int n) const
{
    if (pos > size())
        ThrowOutOfRange();
    return CNetString_Retail(m_start + pos, m_start + pos + Min(n, size() - pos));
}
