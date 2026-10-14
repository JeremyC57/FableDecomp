// CNamedOwner_00a0b6e0::SetName(const CCharString& name) -- when the owned object at +0x10 exists, pass the string's characters to its virtual slot 5
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0B6E0, length 32 bytes.
// Callees: CCharString::operator const char* @ 0x0099AF10 (on the argument), the object's virtual slot 5 (+0x14). Shape: the object's vtable is loaded into
// edi before the string call (the object pointer is re-read from +0x10 for `this` afterwards). The chunk label "CEnginePrimitiveRenderer::ClearAllChildPrimitives"
// is propagated (the 0x00A0xxxx module is game code); the class is named by address.
struct CCharString {
    void* m_pData;
    operator const char*() const;                  // 0x0099AF10
};

struct CNamedObject_00a0b6e0 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void SetName(const char* name);        // slot 5 (+0x14)
};

struct CNamedOwner_00a0b6e0 {
    unsigned char          _pad[0x10];
    CNamedObject_00a0b6e0* m_pObject;              // +0x10
    void SetName(const CCharString& name);
};

void CNamedOwner_00a0b6e0::SetName(const CCharString& name)
{
    if (m_pObject)
        m_pObject->SetName(name);
}
