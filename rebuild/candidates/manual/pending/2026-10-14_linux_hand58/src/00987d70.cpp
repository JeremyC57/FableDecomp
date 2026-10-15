// CPointerPair_00987d70::operator!=(const CPointerPair_00987d70& other) -- 1 unless both words at +4 and +8 are equal, else 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00987D70, length 33 bytes.
// No callees. The chunk label (CMessageEventFilter_IsOriginatedByThing::operator()) is propagated: the argument has this object's own layout (a word at +0 that is not compared -- a vptr -- then two words), so this is an inequality operator of a two-word record such as an intelligent pointer with its item at +4 and a second word at +8; named by address. Shape: no register saves, the `== && ==` chain falls through to `xor eax,eax`, `ret 4`.
struct CPointerPair_00987d70 {
    virtual void v00();
    void* m_pItem;                                 // +0x04
    long  m_tag;                                   // +0x08
    long operator!=(const CPointerPair_00987d70& other) const;
};

long CPointerPair_00987d70::operator!=(const CPointerPair_00987d70& other) const
{
    if (m_pItem == other.m_pItem && m_tag == other.m_tag)
        return 0;
    return 1;
}
