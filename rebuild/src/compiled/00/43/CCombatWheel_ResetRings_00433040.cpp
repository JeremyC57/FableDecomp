struct Ring { };
extern "C" void __fastcall Ring_reset(Ring* r);
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

struct CCombatWheel {
    Ring* begin;
    Ring* end;
};

void __fastcall CCombatWheel_ResetRings(CCombatWheel* self)
{
    Ring* s = self->begin;
    Ring* e = self->end;
    if (s != e) {
        do {
            Ring_reset(s);
            s = (Ring*)((char*)s + 4);
        } while (s != e);
    }
    void* base = self->begin;
    if (base) {
        free(base);
    }
}