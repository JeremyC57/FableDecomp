// GFSerialiseVectorBinaryOut(CDataOutputStream& out, std::vector<CTCHeroOnlineScoreboard::EOnlineScoreboardActivity>& v) -- write the element count, then every element as a long
// Leaf: ??$GFSerialiseVectorBinaryOut@W4EOnlineScoreboardActivity@CTCHeroOnlineScoreboard@@@@YIXAAVCDataOutputStream@@AAV?$vector@W4EOnlineScoreboardActivity@CTCHeroOnlineScoreboard@@V?$allocator@W4EOnlineScoreboardActivity@CTCHeroOnlineScoreboard@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005665B0, 55 bytes, `__fastcall` (stream in ecx, vector in edx), plain `ret`.
// Identity (proven): its only caller is 0x00569F20, bound as CPersistContext::Transfer<CTCHeroOnlineScoreboard::EOnlineScoreboardActivity>(const char*, vector<...>&) (the manifest's CQuestActivityRecord label is propagated); the element is that enum vector's (4 bytes, written as a long).
// Callees: CDataOutputStream::WriteSLONG(long) 0x00993EB0 (count, then each element).
// Shape (family M013, representative 0x0040FD40): `long count = v.size(); out.WriteSLONG(count); for (long i = 0; i < count;
//   ++i) out.WriteSLONG(v[i]);` -- STLport operator[] as `*(begin() + n)` re-reads _M_start every pass (`mov eax,[ebx]`);
//   the enum traits' TransferBinaryOut is the inline WriteSLONG of the value.
class CDataOutputStream {
public:
    void WriteSLONG(long value);                          // 0x00993EB0
};
class CTCHeroOnlineScoreboard {
public:
    enum EOnlineScoreboardActivity { ONLINE_SCOREBOARD_ACTIVITY_NONE = 0 };
};
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* begin() { return _M_start; }
    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
    _Tp& operator[](unsigned int __n) { return *(begin() + __n); }
    _Tp* _M_start;                                        // +0x00
    _Tp* _M_finish;                                       // +0x04
    _Tp* _M_end_of_storage;                               // +0x08
};
}
template <class T> void __fastcall GFSerialiseVectorBinaryOut(CDataOutputStream& out, std::vector<T>& v)
{
    long count = v.size();
    out.WriteSLONG(count);
    for (long i = 0; i < count; ++i)
        out.WriteSLONG(v[i]);
}
template void __fastcall GFSerialiseVectorBinaryOut<CTCHeroOnlineScoreboard::EOnlineScoreboardActivity>(CDataOutputStream& out, std::vector<CTCHeroOnlineScoreboard::EOnlineScoreboardActivity>& v);