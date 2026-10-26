#pragma optimize("s",on)
extern void __cdecl operator_delete(void* p);

class CCameraManagerDef { public:
    int f0;
    void __fastcall scalar_dtor();
    void* scalar_deleting_destructor(unsigned int flags);
};

void* CCameraManagerDef::scalar_deleting_destructor(unsigned int flags)
{
    this->scalar_dtor();
    if (flags & 1)
        operator_delete(this);
    return this;
}