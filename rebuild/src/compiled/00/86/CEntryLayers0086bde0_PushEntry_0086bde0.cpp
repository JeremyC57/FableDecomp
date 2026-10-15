// CEntryLayers_0086bde0::PushEntry(long layer, long a, long b) -- appends a fresh 24-byte entry {a, b, empty list, flag false} to the entry vector of layer `layer`
// Leaf: PushEntry
// Retail: Fable.exe @ 0x0086BDE0, 83 bytes (`ret 0Ch`).
// Callees: vector<SEntry>::push_back(const SEntry&) 0x0086E880 (copies a/b, copy-constructs the list
//          with 0x00673E10, copies the +0x15 flag), free 0x00BFEA14 (the temporary's list storage).
// Label: the manifest's "CTCTavernGameCoinGolf::DrawPreparePrimitivesForRendering" is propagated over a
// byte-identical body; this address has no direct callers (an inlined-elsewhere or table-reached
// helper), so the owner is named by address. `this` starts with a 12-byte vector of 12-byte layer
// vectors (`lea edx,[eax+eax*2] ; lea ecx,[first+edx*4]`).
// Shape: the entry is a by-value temporary with an inline constructor (a, b, the list's three null
//        words from one `xor ebx,ebx`, the flag) built in the 0x18-byte frame. The flag lives INSIDE
//        the 16-byte list record at +0x0D (a separate entry member would sit at +0x18 and grow the
//        frame to 0x1C); push_back's `mov al,[esi+0Dh]` copies it relative to the list. Its inline
//        destructor frees the list storage when non-null (`cmp eax,ebx` reuses the zero register).
#include <stdlib.h>

struct SEntryList {                     // malloc'd vector of 12-byte records
    void* m_first;
    void* m_last;
    void* m_endOfStorage;
    unsigned char m_alloc;              // +0x0c (empty allocator byte, never written)
    bool m_flag;                        // +0x0d (entry +0x15; push_back copies it after the list)
    SEntryList() : m_first(0), m_last(0), m_endOfStorage(0), m_flag(false) {}
    ~SEntryList() { if (m_first) free(m_first); }
};

struct SEntry {                         // 24 bytes
    long m_a;                           // +0x00
    long m_b;                           // +0x04
    SEntryList m_list;                  // +0x08 (16 bytes, flag at +0x15)
    SEntry(long a, long b) : m_a(a), m_b(b) {}
};

struct SEntryVector {                   // 12 bytes
    SEntry* m_first;
    SEntry* m_last;
    SEntry* m_endOfStorage;
    void push_back(const SEntry& entry);    // 0x0086E880
};

struct CEntryLayers_0086bde0 {
    SEntryVector* m_layersFirst;        // +0x00 vector<SEntryVector> begin
    SEntryVector* m_layersLast;
    SEntryVector* m_layersEnd;
    void PushEntry(long layer, long a, long b);
};

void CEntryLayers_0086bde0::PushEntry(long layer, long a, long b)
{
    m_layersFirst[layer].push_back(SEntry(a, b));
}