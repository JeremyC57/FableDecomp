// CPersistContext_TransferableVector<CCrimeWaitingForBribe>::Resize(unsigned long count) -- resize the transferred vector<CCrimeWaitingForBribe> to `count` (STLport resize(n) -> resize(n, CCrimeWaitingForBribe()) inlined: erase the tail when shrinking, fill-insert default records when growing)
// Leaf: ?Resize@CPersistContext_TransferableVector@@UAEXK@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007B5310, 88 bytes (+8 int3 padding), `ret 4`.
// Callees: vector<CCrimeWaitingForBribe>::erase(first, last) @ 0x007B48E0 (manifest `StdVector_MoveElements`),
// vector<CCrimeWaitingForBribe>::_M_fill_insert(pos, n, x) @ 0x007B4E00 (manifest `StdVector_RawInsert`); both thiscall.
// Slot 1 of the transferable-vector vtable 0x0126D7F8 (slot 0 count 0x007B4010, slot 2 TransferOut 0x007B4020, slot 3
// TransferIn 0x007B4770), the wrapper the landed CPersistContext::TransferVector<CCrimeWaitingForBribe> 0x007B5470 builds.
// Shape: STLport's one-argument `resize(n) { resize(n, T()); }` with BOTH resizes __forceinline (a plain inline leaves
// the two-argument resize out of line; a named default local or a `resize(count, T())` call schedules the zero stores
// before the size computation). The default record (Ego_r.pdb CCrimeWaitingForBribe, 8 bytes: DeedHandle,
// EOpinionDeedType Type) is the `sub esp,8` temporary, zeroed through eax by its inline constructor; the size is `(finish - start) >> 3`; the shrink arm passes `start + n` and the cached finish, the grow arm
// reloads finish and passes (finish, n - size, &x); no destructor, both arms return separately (`ret 4`).
class CCrimeWaitingForBribe { public:                     // 8 bytes
    long DeedHandle;                               // +0x0
    long Type;                                     // +0x4 EOpinionDeedType
    CCrimeWaitingForBribe() : DeedHandle(0), Type(0) { }
};

struct CCrimeWaitingForBribeVector {               // STLport vector<CCrimeWaitingForBribe>
    CCrimeWaitingForBribe* _M_start;
    CCrimeWaitingForBribe* _M_finish;
    CCrimeWaitingForBribe* _M_end_of_storage;
    CCrimeWaitingForBribe* begin() { return _M_start; }
    CCrimeWaitingForBribe* end() { return _M_finish; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    CCrimeWaitingForBribe* erase(CCrimeWaitingForBribe* first, CCrimeWaitingForBribe* last);                         // 0x007B48E0
    void _M_fill_insert(CCrimeWaitingForBribe* position, unsigned long n, const CCrimeWaitingForBribe& x);           // 0x007B4E00
    void insert(CCrimeWaitingForBribe* position, unsigned long n, const CCrimeWaitingForBribe& x) { _M_fill_insert(position, n, x); }
    __forceinline void resize(unsigned long newSize, const CCrimeWaitingForBribe& x)
    {
        if (newSize < size())
            erase(begin() + newSize, end());
        else
            insert(end(), newSize - size(), x);
    }
    __forceinline void resize(unsigned long newSize) { resize(newSize, CCrimeWaitingForBribe()); }
};

struct CPersistContext_TransferableVector {
    virtual long GetCount();                       // slot 0
    virtual void Resize(unsigned long count);      // slot 1
    CCrimeWaitingForBribeVector* TransVector;      // +0x4
};

void CPersistContext_TransferableVector::Resize(unsigned long count)
{
    TransVector->resize(count);
}