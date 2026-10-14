// CTimedEntryArray_00594df4::ResetEntry(unsigned long index) -- store a freshly constructed 0x1c-byte entry at the given index of the array that heads this object
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00594DF4, length 33 bytes. A frame-pointer size-optimised module (the 0x0059xx family).
// Callees: SEntry_00594df4::SEntry_00594df4 @ 0x00436560 (the out-of-line default constructor of the 0x1c-byte element), CArray_00594df4::Set(unsigned long index, const SEntry_00594df4&)
// @ 0x00594CEE (bounds-checks the index against (last - first) / 0x1c; the manifest label "CArray<pair<unsigned long,CSoundPair>>::push_back" is propagated).
// The chunk label "CInputProcessGameBase::IsOtherQuickAccessItemButtonHeld" is propagated (the body never reads a game-base member).
// Shape: `push ebp ; mov ebp,esp ; sub esp,1Ch` for the temporary, which is built in place and whose address (the constructor's eax)
// is pushed straight after `push [ebp+8]`; `leave ; ret 4`. The frame pointer with free esi use is optimize("y",off) + ("s",on).
#pragma optimize("y", off)
#pragma optimize("s", on)
struct SEntry_00594df4 {                              // 0x1c bytes
    unsigned char _body[0x1c];
    SEntry_00594df4();                                // 0x00436560
};
struct CArray_00594df4 {                              // {first, last, end} of 0x1c-byte entries
    SEntry_00594df4* first;
    SEntry_00594df4* last;
    SEntry_00594df4* end;
    void Set(unsigned long index, const SEntry_00594df4& entry);   // 0x00594CEE
};
struct CTimedEntryArray_00594df4 {
    CArray_00594df4 m_entries;                        // +0x00
    void ResetEntry(unsigned long index);
};

void CTimedEntryArray_00594df4::ResetEntry(unsigned long index)
{
    m_entries.Set(index, SEntry_00594df4());
}
