// Vendor naming wave V1 (one address, one name): 0x00C1BC10 ?SetLooped@CSoundPlayParameters@@QAEX_N@Z (was ?SetupIdentityFlag@C3DAnimationSequenceData@@QAEX_N@Z)
class CSoundPlayParameters { public:
    char pad0;
    char pad1;
    unsigned char flags;
    void SetLooped(bool set);
};

void CSoundPlayParameters::SetLooped(bool set)
{
    if (set)
        flags = (unsigned char)(flags | 1);
    else
        flags = (unsigned char)(flags & 0xFE);
}