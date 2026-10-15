// CSharedObjectHandle_009d8960::operator=(const CSharedObjectHandle_009d8960& other) -- release the held object, share the other's and add-ref it
// Retail 0x009D8960, 40 bytes, ret 4, returns `this`. No direct callers or vtable references were
// found (xref.py); the manifest label "CTCInventoryItem::GetEngineGraphic" is propagated from another
// byte-identical body, so the class is named by address.
// Shape: a polymorphic handle (vptr at +0, object at +4) over an intrusively counted object (count at
// +0x10, scalar deleting destructor in vtable slot 0). The old object is released under a null test
// (`dec [ecx+10h] ; jne ; push 1 ; call [eax]`), then the other's object is stored and add-ref'd with
// no null test (the store precedes the increment). The release lives on the object (inline
// `Release()`): written through the member it re-reads m_pObject after the decrement.

// Leaf: operator=

struct CSharedObject {
    virtual void DeleteThis(unsigned int flags);   // slot 0 (scalar deleting destructor)
    unsigned char _pad_04[0x0C];
    long m_refCount;                               // +0x10
    void AddRef() { ++m_refCount; }
    void Release() { if (--m_refCount == 0) DeleteThis(1); }
};

struct CSharedObjectHandle_009d8960 {
    virtual void v00();
    CSharedObject* m_pObject;                      // +0x04
    CSharedObjectHandle_009d8960& operator=(const CSharedObjectHandle_009d8960& other);
};

CSharedObjectHandle_009d8960& CSharedObjectHandle_009d8960::operator=(const CSharedObjectHandle_009d8960& other)
{
    if (m_pObject)
        m_pObject->Release();
    m_pObject = other.m_pObject;
    m_pObject->AddRef();
    return *this;
}
