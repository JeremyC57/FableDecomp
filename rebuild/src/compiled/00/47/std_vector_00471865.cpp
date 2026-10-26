// std::vector<std::pair<unsigned long, CAnimationEntry>>::operator=(const vector&) -- (catalog "CAnimationSet::AssignAnimations") Dinkumware vector assignment of 20-byte (key, CAnimationEntry) pairs: reallocate when the source is larger than the capacity, otherwise copy over the live elements and destroy or uninitialised-copy the remainder
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00471865, length 239 bytes, ret 4, returns `this` (ebp frame, /Os code; not LTCG).
// Callees: _Ucopy/allocate-copy @ 0x0047197A (catalog "AnimationVector_AllocCopy"), _Tidy @ 0x00471A4F (catalog
// "CActiveFile::OnReadFinished", propagated), copy @ 0x004719CE (catalog "VectorInsert_CopyAnimations"; range end in edx),
// _Destroy_range @ 0x00454404 (__fastcall, iterator tag by reference), CAnimationEntry::CAnimationEntry(const CAnimationEntry&) @ 0x00471954.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CAnimationEntry { public:
    long BankIndex; long AnimName; long GroupName; void* Components;
    CAnimationEntry(const CAnimationEntry& other);              // 0x00471954
    CAnimationEntry& operator=(const CAnimationEntry& other);
};
struct SPair {
    unsigned long first; CAnimationEntry second;
    __forceinline SPair(const SPair& other) : first(other.first), second(other.second) {}
};
struct STag { };
inline void* operator new(unsigned int, void* p) throw() { return p; }
SPair* __fastcall CopyRange(SPair* first, SPair* last, SPair* dest, const STag& tag);   // 0x004719CE
struct SAlloc { SAlloc() {} };
void __fastcall DestroyRange(SPair* first, SPair* last, const SAlloc& alloc);        // 0x00454404
__forceinline void Construct(SPair* p, const SPair& value) { new ((void*)p) SPair(value); }
__forceinline SPair* UninitCopy(SPair* first, SPair* last, SPair* dest)
{
    SPair* ptr = dest;
    for (; first != last; ++first, ++ptr)
        Construct(ptr, *first);
    return ptr;
}
struct CAnimationVector {
    SPair* _Myfirst; SPair* _Mylast; SPair* _Myend;
    SPair* AllocCopy(unsigned long count, SPair* first, SPair* last);   // 0x0047197A
    void _Tidy();                                                // 0x00471A4F
    unsigned long size() const { return _Mylast - _Myfirst; }
    unsigned long capacity() const { return _Myend - _Myfirst; }
    CAnimationVector& operator=(const CAnimationVector& right);
};
CAnimationVector& CAnimationVector::operator=(const CAnimationVector& right)
{
    if (&right != this) {
        unsigned long count = right.size();
        if (count > capacity()) {
            SPair* p = AllocCopy(count, right._Myfirst, right._Mylast);
            _Tidy();
            _Myfirst = p;
            _Myend = p + count;
        } else {
            if (size() >= count) {
                SPair* p = CopyRange(right._Myfirst, right._Mylast, _Myfirst, STag());
                DestroyRange(p, _Mylast, SAlloc());
            } else {
                CopyRange(right._Myfirst, right._Myfirst + size(), _Myfirst, STag());
                UninitCopy(right._Myfirst + size(), right._Mylast, _Mylast);
            }
        }
        _Mylast = _Myfirst + count;
    }
    return *this;
}