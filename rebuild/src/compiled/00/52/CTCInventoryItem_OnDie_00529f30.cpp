// CTCInventoryItem::OnDie  @ 0x00529f30
// void __fastcall (CTCInventoryItem*)

extern "C" void __cdecl free(void* p);                 // 0x00BFEA14 MSVCR71 free (import thunk)

struct CTCInventoryItemBase
{
    void* m_vptr;                  // +0
    void OnDie();                  // 0x0099eae0 (base OnDie, thiscall)
};

class CTCInventoryItem : public CTCInventoryItemBase
{ public:
    void* m_pField4;               // +4
    void OnDie();
};

void CTCInventoryItem::OnDie()
{
    if (this->m_pField4 != 0)
        free(this->m_pField4);
    this->CTCInventoryItemBase::OnDie();
}