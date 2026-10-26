class CTCCameraCollision { public:
    void* vftable;
};

void __fastcall CTCCameraCollision_dtor(CTCCameraCollision* collision);
void operator delete(void*);

void* __fastcall CTCCameraCollision_scalar_deleting_destructor(CTCCameraCollision* collision, int edx_unused, unsigned int flags)
{
    CTCCameraCollision_dtor(collision);
    if (flags & 1)
        operator delete(collision);
    return collision;
}