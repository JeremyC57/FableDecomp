// Vendor naming wave V1 (one address, one name): 0x00C1BC50 ?SetPositional@CSoundPlayParameters@@QAEX_N@Z (was ?SetupIdentityFlag@C3DAnimationSequenceData@@QAEX_N@Z)
class CSoundPlayParameters { public:
    char pad0;
    char pad1;
    unsigned char flags; // +2
    void SetPositional(bool enable);
};

void CSoundPlayParameters::SetPositional(bool enable)
{
    if (enable)
        this->flags |= 4;
    else
        this->flags &= 0xFB;
}