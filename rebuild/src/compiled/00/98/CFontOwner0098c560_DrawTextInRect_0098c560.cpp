// CFontOwner_0098c560::DrawTextInRect(long text, long left, long top, long right, long bottom) -- hand the text and a RECT built from the four edges to the font object at +0x1d4 through its COM-style virtual slot 94 (`__stdcall`, the object pushed as the first argument) with a trailing 1; returns the call's result
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0098C560, length 68 bytes.
// The chunk label "CIndexBuffer::GetSize" is propagated; named by its effect. Shape: a 16-byte RECT frame, the vtable fetched through the object, `ret 14h`.
struct SRect_0098c560 { long left, top, right, bottom; };
struct IFont_0098c560 {                            // a COM-style interface: __stdcall methods with `this` on the stack
    virtual void __stdcall v000(); virtual void __stdcall v001(); virtual void __stdcall v002(); virtual void __stdcall v003(); virtual void __stdcall v004(); virtual void __stdcall v005(); virtual void __stdcall v006(); virtual void __stdcall v007(); virtual void __stdcall v008(); virtual void __stdcall v009();
    virtual void __stdcall v010(); virtual void __stdcall v011(); virtual void __stdcall v012(); virtual void __stdcall v013(); virtual void __stdcall v014(); virtual void __stdcall v015(); virtual void __stdcall v016(); virtual void __stdcall v017(); virtual void __stdcall v018(); virtual void __stdcall v019();
    virtual void __stdcall v020(); virtual void __stdcall v021(); virtual void __stdcall v022(); virtual void __stdcall v023(); virtual void __stdcall v024(); virtual void __stdcall v025(); virtual void __stdcall v026(); virtual void __stdcall v027(); virtual void __stdcall v028(); virtual void __stdcall v029();
    virtual void __stdcall v030(); virtual void __stdcall v031(); virtual void __stdcall v032(); virtual void __stdcall v033(); virtual void __stdcall v034(); virtual void __stdcall v035(); virtual void __stdcall v036(); virtual void __stdcall v037(); virtual void __stdcall v038(); virtual void __stdcall v039();
    virtual void __stdcall v040(); virtual void __stdcall v041(); virtual void __stdcall v042(); virtual void __stdcall v043(); virtual void __stdcall v044(); virtual void __stdcall v045(); virtual void __stdcall v046(); virtual void __stdcall v047(); virtual void __stdcall v048(); virtual void __stdcall v049();
    virtual void __stdcall v050(); virtual void __stdcall v051(); virtual void __stdcall v052(); virtual void __stdcall v053(); virtual void __stdcall v054(); virtual void __stdcall v055(); virtual void __stdcall v056(); virtual void __stdcall v057(); virtual void __stdcall v058(); virtual void __stdcall v059();
    virtual void __stdcall v060(); virtual void __stdcall v061(); virtual void __stdcall v062(); virtual void __stdcall v063(); virtual void __stdcall v064(); virtual void __stdcall v065(); virtual void __stdcall v066(); virtual void __stdcall v067(); virtual void __stdcall v068(); virtual void __stdcall v069();
    virtual void __stdcall v070(); virtual void __stdcall v071(); virtual void __stdcall v072(); virtual void __stdcall v073(); virtual void __stdcall v074(); virtual void __stdcall v075(); virtual void __stdcall v076(); virtual void __stdcall v077(); virtual void __stdcall v078(); virtual void __stdcall v079();
    virtual void __stdcall v080(); virtual void __stdcall v081(); virtual void __stdcall v082(); virtual void __stdcall v083(); virtual void __stdcall v084(); virtual void __stdcall v085(); virtual void __stdcall v086(); virtual void __stdcall v087(); virtual void __stdcall v088(); virtual void __stdcall v089();
    virtual void __stdcall v090(); virtual void __stdcall v091(); virtual void __stdcall v092(); virtual void __stdcall v093();
    virtual long __stdcall DrawText(long text, const SRect_0098c560* rect, long flags);   // slot 94 (+0x178)
};
struct CFontOwner_0098c560 {
    unsigned char    _pad[0x1d4];
    IFont_0098c560*  m_pFont;                      // +0x1d4
    long DrawTextInRect(long text, long left, long top, long right, long bottom);
};

long CFontOwner_0098c560::DrawTextInRect(long text, long left, long top, long right, long bottom)
{
    SRect_0098c560 rect;
    rect.left = left;
    rect.top = top;
    rect.right = right;
    rect.bottom = bottom;
    return m_pFont->DrawText(text, &rect, 1);
}