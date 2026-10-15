// CThing::OnDie() -- (propagated label) shut down: if the active flag (+0x24) is set clear it and run virtual slot 16, then call slot 2 on the object held by every 36-byte entry of the vector at +0x30
// Retail: Fable.exe (TLC, VC7.1) @ 0x00991750, 53 bytes. Callees: virtual slot 16 (+0x40) on `this`, virtual slot 2
// (+0x08) on each entry's object. Called directly from 0x00417614 and 0x0042EC1A.
// Identity: the catalog's "CThing::OnDie" is a propagated name; the retail CThing keeps its components at +0x44
// (0x004C7D50), so this owner of a vector at +0x30 is another class, modelled locally.
// Shape: the flag is cleared between the vtable fetch and the call (`mov eax,[edi] ; mov byte ptr [edi+24h],0`);
// the vector loop re-reads the last pointer after every call (aligned loop head), stride 0x24.
struct CEntryObject {
    virtual void v00();
    virtual void v01();
    virtual void Shutdown();                         // slot 2 (+0x08)
};

struct SEntry {                                      // 36-byte vector element
    unsigned long m_key;                             // +0x00
    CEntryObject* m_pObject;                         // +0x04
    unsigned char _pad08[0x24 - 0x08];
};

struct CThing {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void Deactivate();                       // slot 16 (+0x40)
    unsigned char _pad04[0x24 - 0x04];
    bool          m_active;                          // +0x24
    unsigned char _pad25[0x30 - 0x25];
    SEntry*       m_entriesFirst;                    // +0x30
    SEntry*       m_entriesLast;                     // +0x34
    void OnDie();
};

void CThing::OnDie()
{
    if (m_active) {
        m_active = false;
        Deactivate();
    }
    for (SEntry* it = m_entriesFirst; it != m_entriesLast; ++it)
        it->m_pObject->Shutdown();
}
