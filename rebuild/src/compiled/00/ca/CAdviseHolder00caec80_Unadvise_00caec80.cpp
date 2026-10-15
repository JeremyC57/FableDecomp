// CAdviseHolder_00caec80::Unadvise() -- COM-style method of the secondary interface: with no advise sink at +0x10 return 0x80040234; otherwise ask the sink to drop this object (sink slot 3, __fastcall) and on success forget the sink and return S_OK, else the sink's failure code
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CAEC80, length 47 bytes (48 with padding).
// Leaf: Unadvise
// The chunk label "CTCGraphicAppearance::GetAnims" is propagated and wrong: third-party media code reached only through the
// vtable cell 0x012BF8C4. Named by address; Unadvise is descriptive.
// Shape: a `__stdcall` virtual that overrides a slot of the SECOND base (at +0x0c): its `this` arrives on the stack as the
// interface pointer (`mov esi,[esp+8]`, `ret 4`), the member at full-object +0x10 is read as [esi+4], and passing `this`
// (the full object) to the sink is the adjusted `lea edx,[esi-0xc]` without a null test.
struct CAdviseHolder_00caec80;

struct IAdviseSink_00caec80 {                     // plain C++ virtuals (`this` in ecx)
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual long Unadvise(CAdviseHolder_00caec80* holder);   // slot 3 (+0x0c)
};

struct CPrimaryBase_00caec80 {                    // 0x0c bytes: vptr + two words
    virtual long __stdcall v00();
    long m_word04;                                // +0x04
    long m_word08;                                // +0x08
};

struct IAdviseSource_00caec80 {                   // the secondary interface (+0x0c)
    virtual long __stdcall Unadvise() = 0;
};

struct CAdviseHolder_00caec80 : CPrimaryBase_00caec80, IAdviseSource_00caec80 {
    IAdviseSink_00caec80* m_pSink;                // +0x10
    long __stdcall Unadvise();
};

long __stdcall CAdviseHolder_00caec80::Unadvise()
{
    if (!m_pSink)
        return 0x80040234;
    long hr = m_pSink->Unadvise(this);
    if (hr < 0)
        return hr;
    m_pSink = 0;
    return 0;
}