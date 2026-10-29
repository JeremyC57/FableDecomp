// CTCTavernGameCardPairs::vfunc_1(CPersistContext* context, long arg) -- persist hook: card-base persist, then the game's "TargetTime" (+0x98, zero default)
// Retail: Fable.exe @ 0x007D6830, 56 bytes (vtable 0x0126F6FC slot 1, `ret 8`).
// Callees: CTCTavernGameCardBase::vfunc_1 0x008F0920 (slot 1 of the card base, called non-virtually),
//          CPersistContext::Transfer 0x00410620 (the 4-byte instance shared by long/float via ICF).
// +0x98 is the CTCTavernGame member TargetTime: the PDB puts it at +0xB4, but retail's UIComponents
// vector is 0x0C bytes (PDB 0x14) and the five handle vectors 0x0C (PDB 0x10), so every member
// after them sits 0x1C lower -- the engine header's layout does not match, hence the local model.
// Shape: the `const float&` zero default is homed in the dead `arg` slot; `this` advanced in place.
class CPersistContext { public:
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);   // CPersistContext::Transfer<T> (member template, PDB)
    //   instances here: float 0x00410620
};

struct CTCTavernGameCardBase {
    unsigned char _pad[0x98];
    float TargetTime;                                                           // +0x98 (CTCTavernGame)
    unsigned char _pad_0x9c[0x27c];
    void vfunc_1(CPersistContext* context, long arg);                          // 0x008F0920
};

struct CTCTavernGameCardPairs : CTCTavernGameCardBase {
    void vfunc_1(CPersistContext* context, long arg);
};

void CTCTavernGameCardPairs::vfunc_1(CPersistContext* context, long arg)
{
    CTCTavernGameCardBase::vfunc_1(context, arg);
    context->Transfer("TargetTime", TargetTime, 0.0f);
}