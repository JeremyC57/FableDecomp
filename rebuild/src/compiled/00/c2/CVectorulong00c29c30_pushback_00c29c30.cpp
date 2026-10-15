// CVector_ulong_00c29c30::push_back(const unsigned long&) -- Dinkumware vector<unsigned long>::push_back: fill in place below capacity (through _Uninitialized_fill_n), else _Insert_n(end(), 1, value)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C29C30, length 98 bytes (padded to 112).
// Leaf: push_back
// Label correction: the catalog "CParticleProgram::AddTextureCoord" is propagated and wrong: `this` is the vector itself
// (allocator word +0, first +4, last +8, end +0xc -- VC7 Dinkumware, as the landed sibling 0x00C29B30), so the body is
// vector<unsigned long>::push_back; named by address like the sibling.
// Callees: _Uninitialized_fill_n<unsigned long*, unsigned int, unsigned long, allocator> @ 0x00C294B0 (`__fastcall(first, count,
// const T&, allocator&, _Scalar_ptr_iterator_tag)`, `ret 0Ch`; manifest label "_Fill_n"), _Insert_n(iterator, count, const T&)
// @ 0x00C298D0.
// Shape: Dinkumware size()/capacity() with their null-first tests (size in ecx, capacity in eax, unsigned compare); the inline
// _Ufill passes the empty tag BY VALUE -- VC places the temporary in the dead `value` argument slot and pushes that dword
// (`mov eax,[esp+8] ... push eax`); _Mylast advanced from the cached register.
struct allocator_ulong { };
struct _Scalar_ptr_iterator_tag { _Scalar_ptr_iterator_tag() {} };
void __fastcall _Uninitialized_fill_n(unsigned long* first, unsigned int count, const unsigned long& value,
                                      allocator_ulong& al, _Scalar_ptr_iterator_tag);   // 0x00C294B0
struct CVector_ulong_00c29c30 {
    allocator_ulong _Alval;                 // +0x00 (padded to a word)
    unsigned long*  _Myfirst;               // +0x04
    unsigned long*  _Mylast;                // +0x08
    unsigned long*  _Myend;                 // +0x0c
    unsigned int size() const { return _Myfirst == 0 ? 0 : (unsigned int)(_Mylast - _Myfirst); }
    unsigned int capacity() const { return _Myfirst == 0 ? 0 : (unsigned int)(_Myend - _Myfirst); }
    unsigned long* _Ufill(unsigned long* ptr, unsigned int count, const unsigned long& value)
    {
        _Uninitialized_fill_n(ptr, count, value, _Alval, _Scalar_ptr_iterator_tag());
        return ptr + count;
    }
    struct iterator {
        unsigned long* _Myptr;
        iterator(unsigned long* p) : _Myptr(p) { }
    };
    iterator end() { return iterator(_Mylast); }
    void _Insert_n(iterator where, unsigned int count, const unsigned long& value);   // 0x00C298D0
    void push_back(const unsigned long& value);
};
void CVector_ulong_00c29c30::push_back(const unsigned long& value)
{
    if (size() < capacity())
        _Mylast = _Ufill(_Mylast, 1, value);
    else
        _Insert_n(end(), 1, value);
}