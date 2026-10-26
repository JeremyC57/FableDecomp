class CThing { public:
    void* vftable;
};

void __fastcall CThing_dtor(CThing* thing);
void operator delete[](void*);

void* __fastcall CThing_scalar_deleting_destructor(CThing* thing, int edx_unused, unsigned int flags)
{
    CThing_dtor(thing);
    if (flags & 1)
        operator delete[](thing);
    return thing;
}