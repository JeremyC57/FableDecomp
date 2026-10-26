// CSimVoice::TransferBinaryOut(CDataOutputStream& out) -- writes the Voices map: its entry count, then the raw entry block (the vector's data, or the map object itself when empty) of count * 8 bytes
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006808C0, length 73 bytes.
// Leaf: TransferBinaryOut
// Callees: CDataOutputStream::WriteSLONG @ 0x00993EB0; CDataOutputStream::Write(const void*, unsigned long), virtual slot 4 (+0x10).
// Layout as the landed CSimVoice::HasVoice 0x00680910: Voices (a sorted vector {first, last, end}, comparator, dirty
// flag) at +8. Same data/size idiom as the landed CDataOutputStream::Write(const CByteBlock&) 0x00995EB0.
// Shape: the map's address is kept in esi; the count is `(last - first) >> 3`, the data pointer an inline GetData()
// (`test ; ja` then `mov eax,esi` for the empty map) taken into a local FIRST, then the byte count re-derived as
// `((last - first) >> 3) << 3` from fresh loads: the second Size() goes through a may-alias pointer reference
// (`const CVoiceMap* const& voices = &Voices;`) so it is not CSE'd with GetData()'s count across the branch.
enum EReactionSpeechType { REACTION_SPEECH_NONE = 0 };

struct SVoiceEntry {                            // pair<EReactionSpeechType, long>
    EReactionSpeechType first;
    long                second;
};

class CDataOutputStream { public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void Write(const void* data, unsigned long size);   // slot 4 (+0x10)
public:
    void WriteSLONG(long);   // 0x00993EB0
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
        out.WriteSLONG(Size());
        out.Write(GetData(), Size() * sizeof(SVoiceEntry));
    }
};

class CInitBaseClass { public:                         // vtable 0x0129A7CC
    virtual ~CInitBaseClass();
    bool m_initialised;                         // +0x04
};

class CSimVoice : public CInitBaseClass { public:             // vtable 0x012337AC
    CVoiceMap Voices;                           // +0x08
    void TransferBinaryOut(CDataOutputStream& out) const;
};

void CSimVoice::TransferBinaryOut(CDataOutputStream& out) const
{
    out.WriteSLONG(Voices.Size());
    const SVoiceEntry* data = Voices.GetData();
    const CVoiceMap* const& voices = &Voices;
    out.Write(data, voices->Size() * sizeof(SVoiceEntry));
}