// CThingPropertyDialog::SetAsAttachingWithMode(unsigned long) -- one-argument vector resize: resize(n, T()) with a zero 12-byte element, plus a scoped array holder that stays null
// Retail: Fable.exe 0x006FD170, 54 bytes. The label is propagated: `this` passes untouched in ecx to
// 0x006FCF90, a 12-byte-element vector resize(n, const T&) (begin/end at this+0/+4, divide by 12), so the
// object IS the vector; the model is descriptive.
// Callees: the two-argument resize 0x006FCF90 (__thiscall, ret 8), operator delete[] (import
// ??_V@YAXPAX@Z through the thunk 0x00BFEB1C) with a constant null pointer.
// Shape: the default element is a 12-byte temporary whose inline constructor zeroes three FLOATS (retail
// stores immediates after the argument pushes; integer members take an `xor eax,eax` register); the
// `push 0 ; call delete[]` after the call is the destructor of a never-filled scoped array holder whose
// pointer the compiler folds to 0 (a pointer member of the element itself is re-read after the call,
// and the other resize caller 0x006FA0AE shows the same null delete[]); its argument is popped with the
// frame (`add esp,10h`). The holder is a modelling device: review at landing.
struct SElement {                                        // C3DVector-like: three floats
    float x, y, z;
    SElement() : x(0.0f), y(0.0f), z(0.0f) {}
};

struct SArrayHolder {
    char* m_pData;
    SArrayHolder() : m_pData(0) {}
    ~SArrayHolder() { delete[] m_pData; }
};

struct CThingPropertyDialog {                            // really a vector<SElement>
    SElement* m_pFirst;                                  // +0x00
    SElement* m_pLast;                                   // +0x04
    SElement* m_pEnd;                                    // +0x08
    void Resize(unsigned long count, const SElement& value);   // 0x006FCF90
    void SetAsAttachingWithMode(unsigned long count);
};

void CThingPropertyDialog::SetAsAttachingWithMode(unsigned long count)
{
    SArrayHolder holder;
    Resize(count, SElement());
}
