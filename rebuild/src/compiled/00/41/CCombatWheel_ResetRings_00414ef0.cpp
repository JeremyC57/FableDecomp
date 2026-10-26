struct Ring { int dummy; };
extern void __fastcall Ring_dtor(Ring* r);
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)
struct RingVec { Ring* first; Ring* last; };

void __fastcall CCombatWheel_ResetRings(RingVec* self)
{
    Ring* p = self->first;
    Ring* e = self->last;
    while (p != e) {
        Ring_dtor(p);
        p++;
    }
    if (self->first)
        free(self->first);
}