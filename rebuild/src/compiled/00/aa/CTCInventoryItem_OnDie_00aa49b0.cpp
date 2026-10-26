// CTCInventoryItem::OnDie  @ 0x00aa49b0  __fastcall void(CTCInventoryItem*)
// Leaf: OnDie
// Frees a heap pointer held at +0x10 (operator delete), then tail-calls base OnDie.

extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

class CTCInventoryItem
{ public:
    char pad0[0x10];
    void* m_ptr;          // +0x10
    void BaseOnDie_00aa75c0();     // 0xaa75c0  __fastcall base method
    void OnDie_impl();
};

void CTCInventoryItem::OnDie_impl()
{
    void* p = this->m_ptr;
    if (p)
        free(p);
    this->BaseOnDie_00aa75c0();
}