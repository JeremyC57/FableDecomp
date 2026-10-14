// CLipSyncDataBank::Close() -- drop every entry of the entry vector at +0x164 (the VC7.1 Dinkumware `erase(begin(), end())` with its empty-range test folded away: `_Move` of the empty tail onto the first element @ 0x00A438E0, `_Destroy` of the rest @ 0x00A43A00, last pointer rewritten), then the CBankFile base closed (0x009CE360)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A42DA0, length 80 bytes.
// Callees: 0x00A438E0 (`__fastcall _Move(first, last, dest, tag, bool)`), 0x00A43A00 (`__fastcall _Destroy(first, last, allocator tag)`), CBankFile::Close @ 0x009CE360. Shape: one 4-byte frame slot whose high byte homes both empty tag temporaries, the moved-to pointer in edi, `ret`. The two iterators are read into locals in the order last, first (that order puts `first` in eax and the tag's `lea` in edx as retail has them).
struct SEntry_00a42da0 { unsigned char m_body[4]; };
struct SMoveTag_00a42da0 { SMoveTag_00a42da0() { } };
struct SAllocatorTag_00a42da0 { SAllocatorTag_00a42da0() { } };
SEntry_00a42da0* __fastcall Move_00a438e0(SEntry_00a42da0* first, SEntry_00a42da0* last, SEntry_00a42da0* dest, const SMoveTag_00a42da0& tag, bool overlap);   // 0x00A438E0
void __fastcall Destroy_00a43a00(SEntry_00a42da0* first, SEntry_00a42da0* last, const SAllocatorTag_00a42da0& allocator);   // 0x00A43A00

struct CBankFile {
    unsigned char _body[0x164];
    void Close();                                  // 0x009CE360
};

struct CLipSyncDataBank : CBankFile {
    SEntry_00a42da0* m_first;                      // +0x164
    SEntry_00a42da0* m_last;                       // +0x168
    SEntry_00a42da0* m_end;                        // +0x16c
    void Close();
};

void CLipSyncDataBank::Close()
{
    SEntry_00a42da0* last = m_last;
    SEntry_00a42da0* first = m_first;
    SEntry_00a42da0* kept = Move_00a438e0(last, last, first, SMoveTag_00a42da0(), false);
    Destroy_00a43a00(kept, m_last, SAllocatorTag_00a42da0());
    m_last = kept;
    CBankFile::Close();
}
