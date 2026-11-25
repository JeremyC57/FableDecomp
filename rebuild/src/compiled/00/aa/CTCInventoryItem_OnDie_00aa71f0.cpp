// CTCInventoryItem::OnDie @ 0x00aa71f0
// push esi; mov esi,ecx; eax=[esi+0x40]; if(eax){push eax; call free; add esp,4}
// mov ecx,esi; jmp base::OnDie
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

struct CTCInventoryItemBase {
    char pad[0x40];
    void* m_ptr;   // +0x40
};

class CTCInventoryItem : public CTCInventoryItemBase { public:
    void OnDie();
};

// Naming (NF449 gen, 2026-11-25): callee-only respell of this row's destructor stand-in calls to the PDB destructor (T1 tracker;
//   target self rows' Leaf), as a qualified destructor call on a bodiless declaration (no layout; rulings r2/r3):
//   0x00AA75C0 ??1CChunkedFileChunk@@UAE@XZ (was ?BaseOnDie@CTCInventoryItemBase@@QAEXXZ)
class CChunkedFileChunk { public: virtual ~CChunkedFileChunk(); };
void CTCInventoryItem::OnDie()
{
    if (this->m_ptr)
        free(this->m_ptr);
    reinterpret_cast<CChunkedFileChunk*>(this)->CChunkedFileChunk::~CChunkedFileChunk();
}