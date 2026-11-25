// CTCInventoryItem::OnDie @ 00a5e650
// push esi; mov esi,ecx; mov eax,[esi+4]; test eax,eax; je +;
// push eax; call 0xbfea14; add esp,4; mov ecx,esi; pop esi; jmp 0x99a2e0

extern "C" void __cdecl free(void* p);                 // 0x00BFEA14 MSVCR71 free (import thunk)

class CTCInventoryItem
{ public:
    void* m_field0;                           // +0
    void* m_thing;                            // +4
    void OnDie();
};

// Naming (NF449 gen, 2026-11-25): callee-only respell of this row's destructor stand-in calls to the PDB destructor (T1 tracker;
//   target self rows' Leaf), as a qualified destructor call on a bodiless declaration (no layout; rulings r2/r3):
//   0x0099A2E0 ??1CBaseClass@@UAE@XZ (was ?BaseOnDie@CTCInventoryItem@@QAEXXZ)
class CBaseClass { public: virtual ~CBaseClass(); };
void CTCInventoryItem::OnDie()
{
    if (this->m_thing)
        free(this->m_thing);
    reinterpret_cast<CBaseClass*>(this)->CBaseClass::~CBaseClass();
}