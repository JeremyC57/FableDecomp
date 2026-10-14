// CEventChain_00c60410::DeleteAll() -- unlink every entry from the singly linked chain at +0x3c4 (next pointer at +0x94 of the entry) and hand it to the __cdecl destroyer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C60410, length 55 bytes.
// Callees: the destroyer @ 0x00C3C370 (`__cdecl`, one argument: the entry, `add esp,4` after it; manifest "CTCHeroStats::InformOfEvent" is propagated).
// Shape: the head is unlinked BEFORE the call (`mov [esi+3C4h],ecx` between the push and the call) and re-read for the loop test; the loop head is
// aligned with a `lea ecx,[ecx]` nop. The chunk label "CDrawGuildSeal::ClearMessages" is propagated; the class is named by address.
struct CChainEntry_00c60410 {
    unsigned char          _pad[0x94];
    CChainEntry_00c60410*  m_pNext;                // +0x94
};

void __cdecl DestroyChainEntry_00c3c370(CChainEntry_00c60410* entry);   // 0x00C3C370

struct CEventChain_00c60410 {
    unsigned char          _pad[0x3c4];
    CChainEntry_00c60410*  m_pFirst;               // +0x3c4
    void DeleteAll();
};

void CEventChain_00c60410::DeleteAll()
{
    while (m_pFirst != 0) {
        CChainEntry_00c60410* entry = m_pFirst;
        m_pFirst = entry->m_pNext;
        DestroyChainEntry_00c3c370(entry);
    }
}
