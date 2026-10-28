// CTCSoundPlayer::PlayAttachedSample(CASoundBank* bank, const CSoundPair* pair, CSoundInit* init, bool a, bool b, bool c) -- play a random sample index from the pair's [min, max] range attached to the owner
// Leaf: PlayAttachedSample
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0066B300, length 112 bytes (`ret 18h`).
// Callees: CTCBase accessor @ 0x00686D60 (`[[0x13B8A1C]+0x1C]`, the game object that owns a random generator at +0x16140; manifest label
// "CViewBase::GetDisplayEngine" is a byte twin), GFROR13 @ 0x00497890 (`__fastcall`), CTCSoundPlayer::PlayAttachedSampleIndex @ 0x0066B080
// (bank, index, init and the three flags; manifest "Sound_PlayAttached").
// Layout: CSoundPair {Min +0, Max +4} (sample index range); the generator's seed is its only word.
// Shape: the generator step `seed = GFROR13(seed * 0x24A1 + 0x24DF)` and `n ? r % n : 0` inlined (as the landed 0x008EE720); the generator
// is a named reference fetched first; the index is a named local computed before the argument pushes (min re-read after the divide).
class CASoundBank;
class CSoundInit;
class CSoundPair { public:
    long Min;                                   // +0x00
    long Max;                                   // +0x04
};
long __fastcall GFROR13(unsigned long value);            // 0x00497890 ?GFROR13@@YIJK@Z
struct CRandomGenerator {
    unsigned long Seed;
    __forceinline unsigned long Next() { Seed = Seed * 0x24a1 + 0x24df; Seed = GFROR13(Seed); return Seed; }
    __forceinline unsigned long Random(unsigned long n) { unsigned long r = Next(); return n ? r % n : 0; }
};
struct CRandomOwner {
    unsigned char _pad00[0x16140];
    CRandomGenerator Random;                    // +0x16140
};
class CTCBase { public:
    virtual void v00();
    void* ParentThing;                          // +0x04
    CRandomOwner* GetRandomOwner();             // 0x00686D60
};
class CTCSoundPlayer : public CTCBase { public:
    unsigned long PlayAttachedSampleIndex(CASoundBank* bank, long index, CSoundInit* init, bool a, bool b, bool c);   // 0x0066B080
    unsigned long PlayAttachedSample(CASoundBank* bank, const CSoundPair* pair, CSoundInit* init, bool a, bool b, bool c);
};

unsigned long CTCSoundPlayer::PlayAttachedSample(CASoundBank* bank, const CSoundPair* pair, CSoundInit* init, bool a, bool b, bool c)
{
    CRandomGenerator& rng = GetRandomOwner()->Random;
    long index = pair->Min + rng.Random(pair->Max - pair->Min + 1);
    return PlayAttachedSampleIndex(bank, index, init, a, b, c);
}