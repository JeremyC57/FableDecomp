class CTCFlammable { public:
    void* vftable;
};

void __fastcall CTCFlammable_dtor(CTCFlammable* flammable);
void operator delete(void*);

void* __fastcall CTCFlammable_scalar_deleting_destructor(CTCFlammable* flammable, int edx_unused, unsigned int flags)
{
    CTCFlammable_dtor(flammable);
    if (flags & 1)
        operator delete(flammable);
    return flammable;
}