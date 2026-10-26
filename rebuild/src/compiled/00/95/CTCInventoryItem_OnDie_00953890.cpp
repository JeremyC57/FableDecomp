// Byte-exact reconstruction of CTCInventoryItem::OnDie @ 0x00953890
// Leaf: OnDie
// __fastcall void OnDie(CTCInventoryItem* self)

class CCombatActionBase {
public:
    virtual ~CCombatActionBase();   // 0x0062E900
};
class CTCInventoryItem { public:
    char pad[0x120];
    void* m120;          // +0x120
};

// cdecl helper freeing the member (call 0xbfea14, add esp,4 -> cdecl)
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

// base class OnDie, tail-called (mov ecx,esi; jmp 0x62e900) -> __fastcall
struct CBase { };

void __fastcall OnDie(CTCInventoryItem* self)
{
    void* p = self->m120;
    if (p != 0)
        free(p);
    ((CCombatActionBase*)(self))->CCombatActionBase::~CCombatActionBase();
}