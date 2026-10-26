class CTCHitOnCollision { public:
    void* vftable;
};

void __fastcall CTCHitOnCollision_dtor(CTCHitOnCollision* collision);
void operator delete(void*);

void* __fastcall CTCHitOnCollision_scalar_deleting_destructor(CTCHitOnCollision* collision, int edx_unused, unsigned int flags)
{
    CTCHitOnCollision_dtor(collision);
    if (flags & 1)
        operator delete(collision);
    return collision;
}