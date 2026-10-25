// CTCInventoryItem::GetTattooCardDef() const -- the owner thing's definition's CTattooDef sub-definition as a counted pointer (null when the thing has no def or the sub-def is missing)
// Leaf: ?GetTattooCardDef@CTCInventoryItem@@QBE?AV?$CDefPointer@$$CBVCTattooDef@@@@XZ
// Name: Ego_r.pdb / FableWin.pdb have exactly two CTCInventoryItem::GetTattooCardDef overloads -- this instance getter
//   (`QBE`, CDefPointer<const CTattooDef>(void) const) and the static (long) one, which is the landed neighbour 0x005D9AA0
//   with the same shape for a manager-fetched def; neither PDB has the earlier descriptive name (FINDINGS chunk 379).
// Retail: Fable.exe (TLC, VC7.1) @ 0x005D93B0, 128 bytes, `ret 4` (hidden CDefPointer result). Callees:
// CDefClassBase::GetSubDef<CTattooDef>(CDefPointer&) 0x005B1EFC (landed; its type string is 'CTattooDef'),
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

class CTattooDef;
template <class T> struct CDefPointer {        // CDefPointer<const CTattooDef>; the def is reached through CDefBase
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
    bool GetSubDefTattoo(CDefPointer<const CTattooDef>& out);      // 0x005B1EFC (GetSubDef<CTattooDef>)
};

struct CThing {
    unsigned char  _pad00[0x70];
    CDefClassBase* m_pDef;                 // +0x70 (retail; CDefPointer<CThingDef>)
};

struct CTCInventoryItem {
    void*   __vftable;                     // +0x00
    CThing* m_pThing;                      // +0x04
    CDefPointer<const CTattooDef> GetTattooCardDef() const;
};

#pragma auto_inline(off)
bool CDefClassBase::GetSubDefTattoo(CDefPointer<const CTattooDef>& out)   // same-TU stand-in for 0x005B1EFC: fills only the out-pointer
{
    out.m_p = this;
    return true;
}
#pragma auto_inline(on)

CDefPointer<const CTattooDef> CTCInventoryItem::GetTattooCardDef() const
{
    CDefPointer<const CTattooDef> def(m_pThing->m_pDef);
    if (def) {
        CDefPointer<const CTattooDef> out;
        if (((CDefClassBase*)def.m_p)->GetSubDefTattoo(out))
            return out;
    }
    return CDefPointer<const CTattooDef>();
}