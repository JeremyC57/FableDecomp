// CGuiControlListBox::SelectEntryFromName(const CCharString& name, long mode) -- forward to the virtual `const char*` overload (slot 1) with the string's characters; the class result comes back through the hidden pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0048DAA2, length 38 bytes.
// Callees: CCharString::operator const char* @ 0x0099AF10, this object's virtual slot 1 (+4: the hidden result pointer, the characters, the mode; `ret 0Ch`).
// Shape: the vtable fetched before the conversion call, the mode pushed straight from its slot, the hidden pointer handed through and returned, `ret 0Ch`. The result type is
// only known to be a class (hidden pointer); it is modelled as an 8-byte handle with a constructor. Size-optimised TU (the mode pushed straight from its slot).
#pragma optimize("s", on)
namespace N_0099af10 { struct CCharString {
    void* m_pData;
    operator const char*() const;                  // 0x0099AF10
}; } using N_0099af10::CCharString;
struct SEntryHandle_0048daa2 {                     // the class result (returned through the hidden pointer)
    long m_index;
    long m_tag;
    SEntryHandle_0048daa2() {}
};

class CGuiControlListBox { public:
    virtual void v00();
    virtual SEntryHandle_0048daa2 SelectEntryFromName(const char* name, long mode);   // slot 1 (+0x04)
    SEntryHandle_0048daa2 SelectEntryFromName(const CCharString& name, long mode);
};

SEntryHandle_0048daa2 CGuiControlListBox::SelectEntryFromName(const CCharString& name, long mode)
{
    return SelectEntryFromName((const char*)name, mode);
}