#pragma optimize("s",on)
// CTCCarryable::OnKill. family len41 x6 (template 0042abe1). Repeated this->m0 reloads are faithful.
struct Inner { char pad4[4]; void* m4; void* m8; void* mc; };
struct T { Inner* m0; long m4; void OnKill(); void BuildTreeArray_00593795(void* n); };
void T::OnKill() {
    if (this->m4) {
        this->BuildTreeArray_00593795(this->m0->m4);
        this->m0->m8 = this->m0;
        this->m0->m4 = 0;
        this->m0->mc = this->m0;
        this->m4 = 0;
    }
}