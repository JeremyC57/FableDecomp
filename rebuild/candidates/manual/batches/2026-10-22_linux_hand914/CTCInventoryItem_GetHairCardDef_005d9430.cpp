// CTCInventoryItem::GetHairCardDef() -- the owner thing's definition's CHairCardDef sub-definition as a counted pointer (null when the thing has no def or the sub-def is missing)
// Leaf: GetHairCardDef
// Retail: Fable.exe (TLC, VC7.1) @ 0x005D9430, 128 bytes, `ret 4` (hidden CDefPointer result). Callees:
// CDefClassBase::GetSubDef<CHairCardDef>(CDefPointer&) 0x005DA180 (landed; its type string is 'CHairCardDef'),
// the def's virtual slot 1 (+4, the destroy-on-zero release). Unreferenced in the image (recovered from a
// mis-cut catalog row); placed among the CTCInventoryItem def getters (0x005D9A10 is the same shape for
// a manager-fetched def), so `this` is a thing component with its owner CThing at +4 and the thing's def
// pointer at +0x70 (retail offset, see CThing::ConstructFromParams 0x004CA0F7).
// Shape: a CDefPointer copy of the thing's def (the copy constructor's null test is shared with `if (def)`),
// a null out-pointer local filled by GetSubDef, `return out` (the result copy's addref and the local's
// release fold into `inc edx ; mov eax,edx ; dec eax`), else `return CDefPointer()`; the def copy is released last.
struct CDefBase {
    virtual void v00();
    virtual void Destroy();                // slot 1
    long m_count;                          // +4
    void Release()
    {
        if (--m_count == 0)
            Destroy();
    }
};

struct CDefPointer {
    CDefBase* m_p;
    CDefPointer() : m_p(0) { }
    CDefPointer(CDefBase* p) : m_p(p) { if (m_p) ++m_p->m_count; }
    CDefPointer(const CDefPointer& other) : m_p(other.m_p) { if (m_p) ++m_p->m_count; }
    ~CDefPointer()
    {
        if (m_p)
            m_p->Release();
    }
    operator bool() const { return m_p != 0; }
};

struct CDefClassBase : CDefBase {
    bool GetSubDefHairCard(CDefPointer& out);      // 0x005DA180 (GetSubDef<CHairCardDef>)
};

struct CThing {
    unsigned char  _pad00[0x70];
    CDefClassBase* m_pDef;                 // +0x70 (retail; CDefPointer<CThingDef>)
};

struct CTCInventoryItem {
    void*   __vftable;                     // +0x00
    CThing* m_pThing;                      // +0x04
    CDefPointer GetHairCardDef();
};

#pragma auto_inline(off)
bool CDefClassBase::GetSubDefHairCard(CDefPointer& out)   // same-TU stand-in for 0x005DA180: fills only the out-pointer
{
    out.m_p = this;
    return true;
}
#pragma auto_inline(on)

CDefPointer CTCInventoryItem::GetHairCardDef()
{
    CDefPointer def(m_pThing->m_pDef);
    if (def) {
        CDefPointer out;
        if (((CDefClassBase*)def.m_p)->GetSubDefHairCard(out))
            return out;
    }
    return CDefPointer();
}
