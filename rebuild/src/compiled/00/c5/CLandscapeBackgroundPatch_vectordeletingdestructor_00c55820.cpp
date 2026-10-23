// __fastcall this=ecx, flags=stack. base /O2 /Oy (no pragma). family len30 x22 (template 00412b00)
struct T { void Dtor_00c557f0(); void* VecDel(unsigned flags); };
void __cdecl operator delete(void* p);
void __cdecl AudioFree(void* block);            // 0x00C3C370 (audio free; retail calls it here, not operator delete)
void* T::VecDel(unsigned flags) {
    this->Dtor_00c557f0();
    if (flags & 1) AudioFree(this);
    return this;
}