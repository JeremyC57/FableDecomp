// Vendor naming wave V1 (one address, one name): 0x00C1BC30 ?SetStreamed@CSoundPlayParameters@@QAEX_N@Z (was ?SetupIdentityFlag@C3DAnimationSequenceData@@QAEX_N@Z)
class CSoundPlayParameters
{ public:
    unsigned char pad0;
    unsigned char pad1;
    unsigned char flags;
    void SetStreamed(bool on);
};

void CSoundPlayParameters::SetStreamed(bool on)
{
    if (on)
        flags |= 0x02;
    else
        flags &= 0xFD;
}