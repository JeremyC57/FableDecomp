// CTimedEntryArray_0048623f::ResetEntry(unsigned long index) -- store a freshly constructed 0x1c-byte entry at the given index of the array that heads this object
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0048623F, length 33 bytes. A frame-pointer size-optimised module (the 0x0048xx family).
// Callees: SEntry_0048623f::SEntry_0048623f @ 0x009E9AB0 (the out-of-line default constructor of the 0x1c-byte element), CArray_0048623f::Set(unsigned long index, const SEntry_0048623f&)
// @ 0x004861F7 (bounds-checks the index against (last - first) / 0x1c; the manifest label "CArray<pair<unsigned long,CSoundPair>>::push_back" is propagated).
// The chunk label "CInputProcessGameBase::IsOtherQuickAccessItemButtonHeld" is propagated (the body never reads a game-base member).
// Shape: `push ebp ; mov ebp,esp ; sub esp,1Ch` for the temporary, which is built in place and whose address (the constructor's eax)
// is pushed straight after `push [ebp+8]`; `leave ; ret 4`. The frame pointer with free esi use is optimize("y",off) + ("s",on).
#pragma optimize("y", off)
#pragma optimize("s", on)
struct SEntry_0048623f {                              // 0x1c bytes
    unsigned char _body[0x1c];
    SEntry_0048623f();                                // 0x009E9AB0
};
struct CArray_0048623f {                              // {first, last, end} of 0x1c-byte entries
    SEntry_0048623f* first;
    SEntry_0048623f* last;
    SEntry_0048623f* end;
    void Set(unsigned long index, const SEntry_0048623f& entry);   // 0x004861F7
};
struct CTimedEntryArray_0048623f {
    CArray_0048623f m_entries;                        // +0x00
    void ResetEntry(unsigned long index);
};

void CTimedEntryArray_0048623f::ResetEntry(unsigned long index)
{
    m_entries.Set(index, SEntry_0048623f());
}