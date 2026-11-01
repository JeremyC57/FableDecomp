#pragma optimize("t", on)
class CCharString { public:
    ~CCharString();   // 0x0099EAE0 (Ego_r.pdb ??1CCharString@@QAE@XZ)
};
extern void __fastcall CActiveSound_Destruct(void* sound);
void* __fastcall CActiveSound_CompositeScalarDeletingDestructor(void* sound, int, unsigned int flags)
{
    ((CCharString*)(static_cast<unsigned char*>(sound) + 28))->~CCharString();
    CActiveSound_Destruct(sound);
    if (flags & 1)
        operator delete(sound);
    return sound;
}