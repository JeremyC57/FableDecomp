// Grade: DIFFER(222v239) -- LTCG custom conventions: retail passes the copy/destroy helpers' range in edx (0x4719CE, 0x454404), builds `push 0x14 ; pop edi` divisors and an ebp frame; plain C++ calls cannot reproduce them | tried: Dinkumware vector::operator= under optimize("s") | next: lane gap: LTCG custom convention
// std::vector<std::pair<unsigned long, CAnimationEntry>>::operator=(const vector&) -- (catalog "CAnimationSet::AssignAnimations") Dinkumware vector assignment of 20-byte (key, CAnimationEntry) pairs: reallocate when the source is larger than the capacity, otherwise copy over the live elements and destroy or uninitialised-copy the remainder
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00471865, length 239 bytes, ret 4, returns `this` (ebp frame, /Os-style LTCG code).
// Callees: _Ucopy/allocate-copy @ 0x0047197A (catalog "AnimationVector_AllocCopy"), _Tidy @ 0x00471A4F (catalog
// "CActiveFile::OnReadFinished", propagated), copy @ 0x004719CE (catalog "VectorInsert_CopyAnimations"; range end in edx),
// _Destroy_range @ 0x00454404 (__fastcall, iterator tag by reference), CAnimationEntry::CAnimationEntry(const CAnimationEntry&) @ 0x00471954.
#pragma optimize("s", on)
struct CAnimationEntry {
    long BankIndex; long AnimName; long GroupName; void* Components;
    CAnimationEntry(const CAnimationEntry& other);              // 0x00471954
    CAnimationEntry& operator=(const CAnimationEntry& other);
};
struct SPair { unsigned long first; CAnimationEntry second; };
struct STag { STag() {} };
inline void* operator new(unsigned int, void* p) { return p; }
SPair* AllocCopy(void* self, unsigned long count, SPair* first, SPair* last);   // 0x0047197A
SPair* CopyRange(SPair* first, SPair* last, SPair* dest);                        // 0x004719CE
void __fastcall DestroyRange(SPair* first, SPair* last, const STag& tag);        // 0x00454404
struct CAnimationVector {
    SPair* _Myfirst; SPair* _Mylast; SPair* _Myend;
    void _Tidy();                                                // 0x00471A4F
    unsigned long size() const { return _Mylast - _Myfirst; }
    unsigned long capacity() const { return _Myend - _Myfirst; }
    CAnimationVector& operator=(const CAnimationVector& right);
};
CAnimationVector& CAnimationVector::operator=(const CAnimationVector& right)
{
    if (this != &right) {
        unsigned long count = right.size();
        if (count > capacity()) {
            SPair* p = AllocCopy(this, count, right._Myfirst, right._Mylast);
            _Tidy();
            _Myfirst = p;
            _Myend = p + count;
        } else if (size() >= count) {
            SPair* p = CopyRange(right._Myfirst, right._Mylast, _Myfirst);
            DestroyRange(p, _Mylast, STag());
        } else {
            SPair* mid = right._Myfirst + size();
            CopyRange(right._Myfirst, mid, _Myfirst);
            SPair* d = _Mylast;
            for (SPair* s = mid; s != right._Mylast; ++s, ++d)
                if (d) { d->first = s->first; new (&d->second) CAnimationEntry(s->second); }
        }
        _Mylast = _Myfirst + count;
    }
    return *this;
}
