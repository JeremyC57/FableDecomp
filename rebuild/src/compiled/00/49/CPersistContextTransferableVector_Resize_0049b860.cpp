// CPersistContext_TransferableVector::Resize(unsigned long count) -- resize the transferred vector<CCharString> to `count` (STLport resize(n, CCharString()) inlined: erase the tail when shrinking, fill-insert default strings when growing)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0049B860, length 99 bytes (+ int3 padding).
// Leaf: ?Resize@CPersistContext_TransferableVector@@UAEXK@Z
// Catalogued as CPersistContext_TransferableVector::vfunc_1 (slot 1 of the transferable-vector vtable: the count getter is
// slot 0, resize slot 1, as the landed CPersistContext_TransferableVector::Resize 0x00F349B0).
// Callees: CCharString::CCharString() @ 0x0099E4B0, vector<CCharString>::erase(first, last) @ 0x0043336A,
// vector<CCharString>::_M_fill_insert(pos, n, x) @ 0x004333B3, CCharString::~CCharString @ 0x0099EAE0.
// Shape: the default string is a named local built BEFORE TransVector is read (so the vector stays in ecx for the calls;
// `TransVector->resize(count)` with the one-argument resize reads the member first and keeps it in esi); it lives in the
// `push ecx` frame word; the size is `(finish - start) >> 2`; the
// shrink arm passes `start + n` and the cached finish, the grow arm reloads finish and passes (finish, n - size, &x);
// both arms destroy the temporary and return separately (`ret 4`).
class CCharString { public:
    void* PStringData;
    CCharString();                                 // 0x0099E4B0
    ~CCharString();                                // 0x0099EAE0
};

struct CCharStringVector {                         // STLport vector<CCharString>
    CCharString* _M_start;
    CCharString* _M_finish;
    CCharString* _M_end_of_storage;
    CCharString* begin() { return _M_start; }
    CCharString* end() { return _M_finish; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    CCharString* erase(CCharString* first, CCharString* last);                         // 0x0043336A
    void _M_fill_insert(CCharString* position, unsigned long n, const CCharString& x);   // 0x004333B3
    void insert(CCharString* position, unsigned long n, const CCharString& x) { _M_fill_insert(position, n, x); }
    void resize(unsigned long newSize, const CCharString& x)
    {
        if (newSize < size())
            erase(begin() + newSize, end());
        else
            insert(end(), newSize - size(), x);
    }
};

struct CPersistContext_TransferableVector {
    virtual long GetCount();                       // slot 0
    virtual void Resize(unsigned long count);      // slot 1
    CCharStringVector* TransVector;                // +0x4
};

void CPersistContext_TransferableVector::Resize(unsigned long count)
{
    CCharString defaultValue;
    TransVector->resize(count, defaultValue);
}