// CTCInventoryExperience::GetXString(long count) -- "" for zero, else the localised "x" prefix + the count as text
// Retail 0x0056EA4E, 76 bytes, ret 8 (hidden CWideString result + count; `this` is passed by every
// caller -- e.g. 0x00574A4E with NumItems at +0x218 -- but never read).
// Shape: count == 0 builds the result straight from the empty literal L"" (0x0122D70C) through
// CWideString(const wchar_t*) 0x0099B6B0. Otherwise `GetXString() + IntToWideString(count)`: VC
// evaluates the right operand first (the __fastcall IntToWideString 0x00405F00 -- hidden result in
// ecx = the one-word frame slot, count in edx -- whose address is pushed as operator+'s stack
// argument), then the global GetXString 0x005CC772 (localised TEXT_GUI_MENU_XSTRING, hidden result
// homed in the dead `count` slot [ebp+0Ch]), then the __fastcall operator+(const CWideString&,
// const CWideString&) 0x0099BE70 into the hidden result; both temporaries die through
// ~CWideString 0x0099B510 (last built first).
// Frame-pointer body that ends in `leave`: `#pragma optimize("y", off)` + `("s", on)`.
#pragma optimize("y", off)
#pragma optimize("s", on)
typedef unsigned short wchar_t;                    // VC7.1 without /Zc:wchar_t

struct CWideString {
    void* m_pStringData;
    CWideString(const wchar_t* text);              // 0x0099B6B0
    CWideString(const CWideString& other);         // 0x0099B720
    ~CWideString();                                // 0x0099B510
};
CWideString __fastcall IntToWideString(long value);                                 // 0x00405F00
CWideString __fastcall GetXString();                                                // 0x005CC772
CWideString __fastcall operator+(const CWideString& left, const CWideString& right); // 0x0099BE70

struct CTCInventoryExperience {
    CWideString GetXString(long count);
};

CWideString CTCInventoryExperience::GetXString(long count)
{
    if (count == 0)
        return CWideString(L"");
    return ::GetXString() + IntToWideString(count);
}