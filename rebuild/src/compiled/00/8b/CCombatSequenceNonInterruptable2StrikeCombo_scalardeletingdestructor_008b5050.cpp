// __fastcall this=ecx, flags=stack. base /O2 /Oy (no pragma). family len30 x22 (template 00412b00)
struct CCombatSequenceNonInterruptable2StrikeCombo { void Dtor(); void* ScalarDeletingDestructor(unsigned flags); };
void __cdecl operator delete(void* p);
void* CCombatSequenceNonInterruptable2StrikeCombo::ScalarDeletingDestructor(unsigned flags) {
    this->Dtor();
    if (flags & 1) ::operator delete(this);
    return this;
}