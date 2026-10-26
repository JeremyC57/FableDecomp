// CLipSyncDataBank::Close() -- drop every entry of the entry vector at +0x164 (the STLport `erase(begin(), end())` with its empty-range test folded away: `__copy` of the empty tail onto the first element @ 0x00A438E0, `_Destroy` of the rest @ 0x00A43A00, last pointer rewritten), then the CBankFile base closed (0x009CE360)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A42DA0, length 80 bytes.
// Callees: 0x00A438E0 (`__fastcall __copy(first, last, result, const random_access_iterator_tag&, int*)`), 0x00A43A00 (`__fastcall _Destroy(first, last, allocator tag)`), CBankFile::Close @ 0x009CE360. Shape: one 4-byte frame slot whose high byte homes both empty tag temporaries, the moved-to pointer in edi, `ret`. The two iterators are read into locals in the order last, first (that order puts `first` in eax and the tag's `lea` in edx as retail has them).
// Fixed (rename1, 2026-10-24): the out-of-line copy 0x00A438E0 was declared as `_Move(first, last, dest, tag&, bool)`
//   (the Dinkumware spelling); it is STLport's counted `__copy(first, last, result, const random_access_iterator_tag&,
//   int*)` (the landed callee body has that signature; the trailing word is the null distance pointer, pushed as 0
//   either way), and the erase(begin(), end()) around it is STLport's, not VC6/VC7.1 Dinkumware. Bytes unchanged.
struct SEntry_00a42da0 { unsigned char m_body[4]; };
struct random_access_iterator_tag { random_access_iterator_tag() { } };
struct SAllocatorTag_00a42da0 { SAllocatorTag_00a42da0() { } };
SEntry_00a42da0* __fastcall __copy(const SEntry_00a42da0* first, const SEntry_00a42da0* last, SEntry_00a42da0* result, const random_access_iterator_tag&, int*);   // 0x00A438E0
void __fastcall Destroy_00a43a00(SEntry_00a42da0* first, SEntry_00a42da0* last, const SAllocatorTag_00a42da0& allocator);   // 0x00A43A00

class CBankFile { public:
    unsigned char _body[0x164];
    void Close_009ce360();                                  // 0x009CE360
};

class CLipSyncDataBank : public CBankFile { public:
    SEntry_00a42da0* m_first;                      // +0x164
    SEntry_00a42da0* m_last;                       // +0x168
    SEntry_00a42da0* m_end;                        // +0x16c
    void Close();
};

void CLipSyncDataBank::Close()
{
    SEntry_00a42da0* last = m_last;
    SEntry_00a42da0* first = m_first;
    SEntry_00a42da0* kept = __copy(last, last, first, random_access_iterator_tag(), (int*)0);
    Destroy_00a43a00(kept, m_last, SAllocatorTag_00a42da0());
    m_last = kept;
    CBankFile::Close_009ce360();
}