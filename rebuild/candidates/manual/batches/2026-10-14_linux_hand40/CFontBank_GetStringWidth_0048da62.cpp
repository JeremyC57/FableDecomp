// CFontBank::GetStringWidth(const CCharString& text) -- the X of the 2D size that virtual slot 1 measures for the string's characters
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0048DA62, length 39 bytes.
// Callees: the string's character accessor @ 0x0099AF10 (with 0; labelled "CCharString::operator char const*"), virtual slot 1 (+4) of `this` returning a C2DVector
// by value (the hidden slot's address comes back in eax, its X is loaded with `fld`). Shape: the frame-pointer module (`push ecx ; push ecx` for the 8-byte
// temporary, `leave`), the vtable fetched into edi before the accessor call, `ret 4`.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct C2DVector_0048da62 { float X, Y; };

struct CCharString {
    void* m_pData;
    const char* GetCharacters(long from) const;    // 0x0099AF10
};

struct CFontBank {
    virtual void v00();
    virtual C2DVector_0048da62 GetStringSize(const char* text);   // slot 1 (+0x04)
    float GetStringWidth(const CCharString& text);
};

float CFontBank::GetStringWidth(const CCharString& text)
{
    return GetStringSize(text.GetCharacters(0)).X;
}
