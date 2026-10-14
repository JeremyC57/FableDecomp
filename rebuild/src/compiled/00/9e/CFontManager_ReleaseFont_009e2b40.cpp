// CFontManager::ReleaseFont(long key) -- find the font entry for `key` in the static bank (symbol map pointer at +0x14, bank at +4) or else the streaming bank (map pointer at +0x1c, bank at +0xc); when the entry says it can be released (virtual slot 2) release it (virtual slot 3)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E2B40, length 96 bytes.
// Callees: CSymbolMap::FindSymbol @ 0x009E51C0 (a bool in al) and Lookup @ 0x009E5120 on the two maps, the banks' entry accessors @ 0x00A635D0 / 0x00A5F0A0
// (`[bank+120h][index]`; labelled "CTCInventoryAbilities::GetAbilityLevel", propagated), virtual slots 2 (+8, a bool) and 3 (+0xc) of the entry. Shape: the two maps are POINTER members (re-read for each call), the key
// kept in edi and its push hoisted above the branch (both arms call with it first), the entry pointer in esi, `ret 4`.
struct CFontEntry_009e2b40 {
    virtual void v00(); virtual void v01();
    virtual bool CanRelease();                     // slot 2 (+0x08)
    virtual void Release();                        // slot 3 (+0x0c)
};
struct CFontBank_009e2b40 {
    CFontEntry_009e2b40* GetEntry(long index);     // 0x00A635D0
};
struct CFontBankB_009e2b40 {
    CFontEntry_009e2b40* GetEntry(long index);     // 0x00A5F0A0
};
struct CSymbolMap {
    unsigned char _body[0x8];
    bool FindSymbol(long key);                     // 0x009E51C0
    long Lookup(long key);                         // 0x009E5120
};

struct CFontManager {
    long                 m_0;
    CFontBank_009e2b40*  m_pStaticBank;            // +0x04
    long                 m_8;
    CFontBankB_009e2b40* m_pStreamingBank;         // +0x0c
    long                 m_10;
    CSymbolMap*          m_pStaticMap;             // +0x14
    long                 m_18;
    CSymbolMap*          m_pStreamingMap;          // +0x1c
    void ReleaseFont(long key);
};

void CFontManager::ReleaseFont(long key)
{
    CFontEntry_009e2b40* entry;
    if (m_pStaticMap->FindSymbol(key))
        entry = m_pStaticBank->GetEntry(m_pStaticMap->Lookup(key));
    else if (m_pStreamingMap->FindSymbol(key))
        entry = m_pStreamingBank->GetEntry(m_pStreamingMap->Lookup(key));
    else
        return;
    if (entry->CanRelease())
        entry->Release();
}