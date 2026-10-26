#pragma optimize("s", on)
extern void __fastcall CTCFlashOnHit_Destruct(void* hit);
void* __fastcall CTCFlashOnHit_ScalarDeletingDestructor(void* hit, int, unsigned int flags)
{
    CTCFlashOnHit_Destruct(hit);
    if (flags & 1)
        operator delete(hit);
    return hit;
}