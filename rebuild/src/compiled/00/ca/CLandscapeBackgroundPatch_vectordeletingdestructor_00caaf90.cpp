// __fastcall this=ecx, flags=stack. base /O2 /Oy (no pragma). family len30 x22 (template 00412b00)
struct T { void Dtor_00ca7a20(); void* VecDel(unsigned flags); };
void __cdecl operator delete(void* p);
void* T::VecDel(unsigned flags) {
    this->Dtor_00ca7a20();
    if (flags & 1) ::operator delete(this);
    return this;
}