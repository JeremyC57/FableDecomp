// __fastcall this=ecx, flags=stack. base /O2 /Oy (no pragma). family len30 x22 (template 00412b00)
// Leaf: `scalar_deleting_destructor'
struct CCreatureAction_SummonerFlameSlice { void Dtor(); void* ScalarDeletingDestructor(unsigned flags); };
void __cdecl operator delete(void* p);
void __cdecl AudioFree(void* block);            // 0x00C3C370 (audio free; retail calls it here, not operator delete)
void* CCreatureAction_SummonerFlameSlice::ScalarDeletingDestructor(unsigned flags) {
    this->Dtor();
    if (flags & 1) AudioFree(this);
    return this;
}