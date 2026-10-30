// CTCInventoryItem::OnDie @ 00a5e650
// push esi; mov esi,ecx; mov eax,[esi+4]; test eax,eax; je +;
// push eax; call 0xbfea14; add esp,4; mov ecx,esi; pop esi; jmp 0x99a2e0

extern "C" void __cdecl free(void* p);                 // 0x00BFEA14 MSVCR71 free (import thunk)

class CTCInventoryItem
{ public:
    void* m_field0;                           // +0
    void* m_thing;                            // +4
    void OnDie();
    void BaseOnDie();                         // thiscall base OnDie @ 0x99a2e0
};

void CTCInventoryItem::OnDie()
{
    if (this->m_thing)
        free(this->m_thing);
    this->BaseOnDie();
}