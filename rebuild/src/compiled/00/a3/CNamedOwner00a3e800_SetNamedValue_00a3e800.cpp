// CNamedOwner_00a3e800::SetNamedValue(const CCharString& name, long value) -- when the owned object at +0x40 exists, pass the string's characters and the value to its virtual slot 1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A3E800, length 37 bytes.
// Callees: CCharString::operator const char* @ 0x0099AF10 (on the first argument), the object's virtual slot 1 (+4). Shape: the object's vtable is loaded into
// edi, the value pushed, then the string converted (its address re-read from the stack) and pushed; the object pointer is re-read from +0x40 for `this`.
// The chunk label "CEnginePrimitiveRenderer::ClearAllChildPrimitives" is propagated (the 0x00A3xxxx module is game code); the class is named by address.
namespace N_0099af10 { struct CCharString {
    void* m_pData;
    operator const char*() const;                  // 0x0099AF10
}; } using N_0099af10::CCharString;

struct CNamedObject_00a3e800 {
    virtual void v00();
    virtual void SetNamedValue(const char* name, long value);   // slot 1 (+0x04)
};

struct CNamedOwner_00a3e800 {
    unsigned char          _pad[0x40];
    CNamedObject_00a3e800* m_pObject;              // +0x40
    void SetNamedValue(const CCharString& name, long value);
};

void CNamedOwner_00a3e800::SetNamedValue(const CCharString& name, long value)
{
    if (m_pObject)
        m_pObject->SetNamedValue(name, value);
}