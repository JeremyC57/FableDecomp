// CTCInventoryItem::OnDie  @ 0x00aa49b0  __fastcall void(CTCInventoryItem*)
// Leaf: OnDie
// Frees a heap pointer held at +0x10 (operator delete), then tail-calls base OnDie.

extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

class CTCInventoryItem
{ public:
    char pad0[0x10];
    void* m_ptr;          // +0x10
    void OnDie_impl();
};

// Naming (NF449 gen, 2026-11-25): callee-only respell of this row's destructor stand-in calls to the PDB destructor (T1 tracker;
//   target self rows' Leaf), as a qualified destructor call on a bodiless declaration (no layout; rulings r2/r3):
//   0x00AA75C0 ??1CChunkedFileChunk@@UAE@XZ (was ?BaseOnDie_00aa75c0@CTCInventoryItem@@QAEXXZ)
class CChunkedFileChunk { public: virtual ~CChunkedFileChunk(); };
void CTCInventoryItem::OnDie_impl()
{
    void* p = this->m_ptr;
    if (p)
        free(p);
    reinterpret_cast<CChunkedFileChunk*>(this)->CChunkedFileChunk::~CChunkedFileChunk();
}