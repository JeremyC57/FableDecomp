#pragma optimize("t", on)
extern void __fastcall DestructMember_0099eae0(void* member);
extern void __fastcall CActiveSound_Destruct(void* sound);
void* __fastcall CActiveSound_CompositeScalarDeletingDestructor(void* sound, int, unsigned int flags)
{
    DestructMember_0099eae0(static_cast<unsigned char*>(sound) + 28);
    CActiveSound_Destruct(sound);
    if (flags & 1)
        operator delete(sound);
    return sound;
}