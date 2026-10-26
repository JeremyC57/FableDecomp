// CTCTavernGameCardPairs::vfunc_53() -- show the tavern game def's OK message (title text +0xA0, body text +0xF8) and move on to state 0x17 once it is acknowledged
// Leaf: vfunc_53
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007D6260, length 108 bytes (padded row 112); CTCTavernGameCardPairs vtable 0x0126F6FC slot 53.
// Callees: CTCTavernGame::GetTavernGameDef(CDefPointer<const CTavernGameDef>*) @ 0x005EA000 (manifest label "_Cons_val<...>" is propagated),
// CTCTavernGame::GetText(unsigned long) @ 0x005E9E70 (x2, each built straight into its by-value argument slot), CTCTavernGame::SetStateAfterOK(
// CWideString, CWideString, ETavernGameStates) @ 0x005EB080, the def's virtual slot 1 (destroy).
// Layout: tavern game def text ids at +0xA0 and +0xF8.
// Shape: as agent 171's CTCTavernGameShoveHaPenny::EnterGame model: a null-tested CDefPointer dtor with the def's inline ReleaseTemporary()
// (DecRef/GetRefCount on the ref-count base at +4); the def object is a named pointer local (edi) after the first read; CWideString declares
// its copy ctor so the GetText results are built in the argument area (right to left: state, body text, title text).
class CWideString { public:
    void* m_pData;
    CWideString(const CWideString& other);
    ~CWideString();
};
struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
class CTavernGameDef : public CDefVirtual, public CRefCounted { public:
    unsigned char _pad08[0xa0 - 0x08];
    unsigned long OKTitleText;                  // +0xa0
    unsigned char _padA4[0xf8 - 0xa4];
    unsigned long OKBodyText;                   // +0xf8
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
struct CDefPointer {
    CTavernGameDef* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) Object->ReleaseTemporary(); }
};
class CTCTavernGame { public:
    virtual void v00();
    void GetTavernGameDef(CDefPointer* out);    // 0x005EA000
    CWideString GetText(unsigned long textID);  // 0x005E9E70
    void SetStateAfterOK(CWideString title, CWideString text, long state);   // 0x005EB080
};
struct CTCTavernGameCardPairs : CTCTavernGame {
    void vfunc_53();
};

void CTCTavernGameCardPairs::vfunc_53()
{
    CDefPointer def;
    GetTavernGameDef(&def);
    CTavernGameDef* d = def.Object;
    SetStateAfterOK(GetText(d->OKTitleText), GetText(d->OKBodyText), 0x17);
}