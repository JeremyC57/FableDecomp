#pragma optimize("s",on)
struct CSpecialAbilitiesEnflameDef {
    void* vftable;
    void Dtor();
    void* scalar_deleting_destructor(unsigned int flags);
};
extern void __cdecl op_delete(void* p);

void* CSpecialAbilitiesEnflameDef::scalar_deleting_destructor(unsigned int flags)
{
    this->Dtor();
    if (flags & 1)
        op_delete(this);
    return this;
}