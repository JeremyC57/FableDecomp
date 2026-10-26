class C3DPrimitive2 { public:
    void* vftable;
};

void __fastcall C3DPrimitive2_dtor(C3DPrimitive2* primitive2);
void operator delete(void*);

void* __fastcall C3DPrimitive2_scalar_deleting_destructor(C3DPrimitive2* primitive2, int edx_unused, unsigned int flags)
{
    C3DPrimitive2_dtor(primitive2);
    if (flags & 1)
        operator delete(primitive2);
    return primitive2;
}