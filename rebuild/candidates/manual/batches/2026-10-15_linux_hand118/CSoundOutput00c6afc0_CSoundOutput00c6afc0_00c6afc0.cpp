// CSoundOutput00c6afc0::CSoundOutput00c6afc0() -- builds the polymorphic format member at +0x234 (2 channels, 48000 Hz, four zero words), the 0x2c-byte record at +0x260 and the stream object at +0x28c, then clears the two trailing words
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C6AFC0, length 143 bytes.
// Leaf: ??0CSoundOutput00c6afc0
// Flags: /EHsc
// Label: the manifest's "C3DMeshMaterial::C3DMeshMaterial" is propagated. The body lives in a no-RTTI module
// (vtable 0x012B9584 has no complete-object locator, named CVtable_012b9584 here); its only caller is the
// constructor at 0x00C54090, which builds this object as its member at +0x2f8. Names are descriptive.
// Callees: 0x00C77770 the record constructor (`[this+28h] = 0`, propagated label CGameEventPackage::Clear),
// 0x00C6A650 the stream constructor (its own EH frame; vtable 0x012B9594, the same 2-channel / 48 kHz pair
// at +0x2c/+0x30).
// Shape: the words stored before the vtable store belong to a `__declspec(novtable)` root with an inline
// constructor (rate first, then channels, then the zero words -- body order); the EH state 0 after the vtable
// store is the format member's destructor; the record and the stream add no further state (the record has a
// trivial destructor and nothing can throw after the stream). The trailing words are plain zero initialisers.
struct __declspec(novtable) CSoundFormatBase {
    virtual void v00();
    long m_channels;                                // +0x04
    long m_sampleRate;                              // +0x08
    long m_0c, m_10, m_14, m_18;                    // +0x0c..+0x18
    CSoundFormatBase()
    {
        m_sampleRate = 48000;
        m_channels = 2;
        m_0c = 0; m_10 = 0; m_14 = 0; m_18 = 0;
    }
};
struct CVtable_012b9584 : CSoundFormatBase {        // 0x1c bytes
    virtual void v00();
    CVtable_012b9584() {}
    ~CVtable_012b9584() {}
};
struct CSoundRecord00c77770 {                       // 0x2c bytes
    CSoundRecord00c77770();                         // 0x00C77770
    unsigned char _body[0x2c];
};
struct CSoundStream00c6a650 {                       // 0x298 bytes
    CSoundStream00c6a650();                         // 0x00C6A650
    unsigned char _body[0x298];
};

struct CSoundOutput00c6afc0 {
    unsigned char        _pad000[0x234];
    CVtable_012b9584     m_format;                  // +0x234
    unsigned char        _pad250[0x10];
    CSoundRecord00c77770 m_record;                  // +0x260
    CSoundStream00c6a650 m_stream;                  // +0x28c
    long                 m_524;                     // +0x524
    long                 m_528;                     // +0x528
    CSoundOutput00c6afc0();
};

CSoundOutput00c6afc0::CSoundOutput00c6afc0()
    : m_524(0), m_528(0)
{
}
