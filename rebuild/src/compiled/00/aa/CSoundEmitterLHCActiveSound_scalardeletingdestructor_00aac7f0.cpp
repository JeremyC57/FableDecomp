#pragma optimize("t", on)
extern void __fastcall DestructMember(void* member);
extern void __fastcall CActiveSound_Destruct(void* sound);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CActiveSound_CompositeScalarDeletingDestructor(void* sound, int, unsigned int flags)
{
    DestructMember(static_cast<unsigned char*>(sound) + 28);
    CActiveSound_Destruct(sound);
    if (flags & 1)
        OperatorDelete(sound);
    return sound;
}