// CNetString_Retail::Append(const char* first, const char* last, const forward_iterator_tag&) -- STLport basic_string<char>::append(first, last, forward_iterator_tag): grow into a fresh malloc'd block (old text, new text, terminator) when the capacity is short, else copy in place behind the old terminator
// Leaf: Append
// Retail: Fable.exe (TLC, VC7.1) @ 0x00567D60, 281 bytes (the int3 VC7.1 emits after the final noreturn call included), `ret 0Ch` (returns *this). The catalog/manifest label
// "CTCElectrocutedEffect::ReplaceString" is propagated across byte-identical bodies (15 callers, all string appends: the HTTP URL
// code 0x0083FC4B.. and this module's 0x005687F8..).
// Callees: malloc @ 0x00BFEA0E (allocator<char>::allocate, zero length -> null), memmove @ 0x00BFEAE6 (__copy_trivial behind
// uninitialized_copy), free @ 0x00BFEA14 (_M_deallocate_block), _String_base::_M_throw_length_error() const @ 0x00565A30 (puts +
// abort with exceptions off; landed, one TU copy of three).
// Layout: the retail STLport string as the landed CNetString_Retail::Substr 0x00569330 (start, finish, end of storage; 12 bytes).
// Shape: STLport 4.x `append(_ForwardIter, _ForwardIter, const forward_iterator_tag&)` verbatim: max_size() is size_t(-1) - 1, the
// old size lives in the function's `push ecx` word and `n` / the new length in the dead `last` slot (max is the const-reference
// template); __copy_trivial tests its count before memmove (the second copy's test folds away: first != last is known).
extern "C" void* __cdecl malloc(unsigned int size);                               // 0x00BFEA0E
extern "C" void  __cdecl free(void* block);                                       // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6

struct SForwardIteratorTag { SForwardIteratorTag() { } };

template <class T> inline const T& Max(const T& a, const T& b) { return a < b ? b : a; }

__forceinline void* CopyTrivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result
        : ((char*)memmove(result, first, ((const char*)last - (const char*)first))) + ((const char*)last - (const char*)first);
}
__forceinline char* UninitializedCopy(const char* first, const char* last, char* result)
{
    return (char*)CopyTrivial(first, last, result);
}
inline void Distance(const char* first, const char* last, int& n) { n += last - first; }

struct CNetString_Retail {                         // 12 bytes: start, finish, end of storage
    char* m_start;
    char* m_finish;
    char* m_endOfStorage;
    __declspec(noreturn) void ThrowLengthError() const;   // 0x00565A30
    unsigned int size() const { return m_finish - m_start; }
    unsigned int max_size() const { return (unsigned int)-1 - 1; }
    unsigned int capacity() const { return (m_endOfStorage - m_start) - 1; }
    char* Allocate(unsigned int n) { return n != 0 ? (char*)malloc(n) : 0; }
    void DeallocateBlock() { if (m_start) free(m_start); }
    CNetString_Retail& Append(const char* first, const char* last, const SForwardIteratorTag& tag);
};

CNetString_Retail& CNetString_Retail::Append(const char* first, const char* last, const SForwardIteratorTag&)
{
    if (first != last) {
        const unsigned int oldSize = size();
        int n = 0;
        Distance(first, last, n);
        if ((unsigned int)n > max_size() || oldSize > max_size() - (unsigned int)n)
            ThrowLengthError();
        if (oldSize + n > capacity()) {
            const unsigned int len = oldSize + Max(oldSize, (unsigned int)n) + 1;
            char* newStart = Allocate(len);
            char* newFinish = newStart;
            newFinish = UninitializedCopy(m_start, m_finish, newStart);
            newFinish = UninitializedCopy(first, last, newFinish);
            *newFinish = 0;
            DeallocateBlock();
            m_start = newStart;
            m_finish = newFinish;
            m_endOfStorage = newStart + len;
        } else {
            const char* f1 = first;
            ++f1;
            UninitializedCopy(f1, last, m_finish + 1);
            m_finish[n] = 0;
            *m_finish = *first;
            m_finish += n;
        }
    }
    return *this;
}