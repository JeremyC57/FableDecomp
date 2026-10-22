#pragma optimize("s", on)
extern void __fastcall CTCFlashOnHit_Destruct(void* hit);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCFlashOnHit_ScalarDeletingDestructor(void* hit, int, unsigned int flags)
{
    CTCFlashOnHit_Destruct(hit);
    if (flags & 1)
        OperatorDelete(hit);
    return hit;
}