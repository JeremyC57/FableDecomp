// __fastcall this=ecx, flags=stack. base /O2 /Oy (no pragma). family len30 x22 (template 00412b00)
struct CCombatSequenceWhisperBoast { void Dtor(); void* ScalarDeletingDestructor(unsigned flags); };
void __cdecl operator delete(void* p);
void* CCombatSequenceWhisperBoast::ScalarDeletingDestructor(unsigned flags) {
    this->Dtor();
    if (flags & 1) ::operator delete(this);
    return this;
}