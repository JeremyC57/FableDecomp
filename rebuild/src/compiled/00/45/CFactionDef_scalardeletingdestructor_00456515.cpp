#pragma optimize("s",on)
class CFactionDef { public:
    int f0;
    int Parent;
    void* f8;
    void CFactionDef_dtor();
    void* CFactionDef_scalar_deleting_destructor(unsigned int flags);
};
void* CFactionDef::CFactionDef_scalar_deleting_destructor(unsigned int flags)
{
    this->CFactionDef_dtor();
    if (flags & 1)
        operator delete(this);
    return this;
}