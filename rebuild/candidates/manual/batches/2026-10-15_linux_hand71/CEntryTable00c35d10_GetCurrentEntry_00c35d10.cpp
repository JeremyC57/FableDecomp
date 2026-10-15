// CEntryTable_00c35d10::GetCurrentEntry() -- return the entry at the current index of the table when the table is live, else 0
// Leaf: GetCurrentEntry
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C35D10, length 19 bytes.
// Retail bytes:  mov eax,[ecx+8] ; test eax,eax ; je Z ; mov eax,[ecx+10h] ; mov ecx,[ecx] ; mov eax,[ecx+eax*4] ; ret ; Z: xor eax,eax ; ret
// Manifest label "CResource::GetUnusedFrames" is propagated and wrong (the real CResource::GetUnusedFrames is the landed
// 0x009FBF70; this body has no vptr at +0 -- the word at +0 is indexed as an array). It is a `__thiscall` getter of the
// 0x00C3xxxx module with no recoverable class name, named by address: +0 the entry array, +8 the word that marks the
// table live, +0x10 the current index.
struct CEntry_00c35d10;

struct CEntryTable_00c35d10 {
    CEntry_00c35d10** m_pEntries;            // +0x00
    unsigned long     _unused04;             // +0x04
    void*             m_pLive;               // +0x08
    unsigned long     _unused0c;             // +0x0C
    long              m_current;             // +0x10
    CEntry_00c35d10* GetCurrentEntry() const;
};

CEntry_00c35d10* CEntryTable_00c35d10::GetCurrentEntry() const
{
    if (m_pLive)
        return m_pEntries[m_current];
    return 0;
}
