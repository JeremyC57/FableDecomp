// Grade: DIFFER(61v73) -- after the WriteLong call retail re-reads first/last for the byte count behind the GetData() branch (`mov ecx,[esi+4] ; mov ebx,[esi] ; sub ; sar 3 ; shl 3`, ebx saved), the lane reuses the count computed for GetData's test across the join (`shl eax,3`); ternary / if-local / void* GetData, signed and unsigned Size, separate byte-count accessor a data local, an iterator-object vector and an STLport vector base all CSE the same way
// CSimVoice::TransferBinaryOut(CDataOutputStream& out) -- writes the Voices map: its entry count, then the raw entry block (the vector's data, or the map object itself when empty) of count * 8 bytes
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006808C0, length 73 bytes.
// Leaf: TransferBinaryOut
// Callees: CDataOutputStream::WriteLong @ 0x00993EB0; CDataOutputStream::Write(const void*, unsigned long), virtual slot 4 (+0x10).
// Layout as the landed CSimVoice::HasVoice 0x00680910: Voices (a sorted vector {first, last, end}, comparator, dirty
// flag) at +8. Same data/size idiom as the landed CDataOutputStream::Write(const CByteBlock&) 0x00995EB0.
// Shape: the map is handled by its own inline TransferBinaryOut (its address kept in esi); the count is
// `(last - first) >> 3`, the data pointer an inline GetData() (`test ; ja` then `mov eax,esi` for the empty map),
// the byte count re-derived as `((last - first) >> 3) << 3`.
enum EReactionSpeechType { REACTION_SPEECH_NONE = 0 };

struct SVoiceEntry {                            // pair<EReactionSpeechType, long>
    EReactionSpeechType first;
    long                second;
};

struct CDataOutputStream {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void Write(const void* data, unsigned long size);   // slot 4 (+0x10)
    void WriteLong(long value);                                 // 0x00993EB0
};

struct CKeyPairCompareLess { };                 // empty comparator (one byte)

struct CVoiceMap {                              // CVectorMap<EReactionSpeechType,long,CKeyPairCompareLess<..>>
    SVoiceEntry*        m_pFirst;               // +0x00
    SVoiceEntry*        m_pLast;                // +0x04
    SVoiceEntry*        m_pEnd;                 // +0x08
    CKeyPairCompareLess m_compare;              // +0x0c
    bool                m_dirty;                // +0x0d
    unsigned long Size() const { return m_pLast - m_pFirst; }
    const SVoiceEntry* GetData() const { return Size() > 0 ? m_pFirst : (const SVoiceEntry*)this; }
    void TransferBinaryOut(CDataOutputStream& out) const
    {
        out.WriteLong(Size());
        out.Write(GetData(), Size() * sizeof(SVoiceEntry));
    }
};

struct CInitBaseClass {                         // vtable 0x0129A7CC
    virtual ~CInitBaseClass();
    bool m_initialised;                         // +0x04
};

struct CSimVoice : CInitBaseClass {             // vtable 0x012337AC
    CVoiceMap Voices;                           // +0x08
    void TransferBinaryOut(CDataOutputStream& out) const;
};

void CSimVoice::TransferBinaryOut(CDataOutputStream& out) const
{
    Voices.TransferBinaryOut(out);
}
