// CSoundManagerLH::vfunc_38(long key, long sound, long priority, long position, long flags) -- look the bank up by key (0x00A3ED30); false when absent, else its slot-6 play call with (sound, position, flags, 0, 4.0f, priority) succeeding (>= 0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A3ED90, length 64 bytes (80 with padding).
// Vtable 0x0129D2C4 (CSoundManagerLH, rtti-vtables.tsv) slot 38.
// Callees: 0x00A3ED30 (a thiscall lookup on `this` taking the key; manifest label VectorMap_LookupByKey),
// slot 6 (+0x18) of the object it returns.
// Shape: the lookup with `this` still in ecx, the early `xor al,al ; ret 14h`, then six pushes in reverse
// argument order (the third argument goes last, a literal 4.0f and 0 before it), and the `>= 0` result
// through a full register (`xor edx,edx ; test ; setge dl ; mov al,dl`): a plain `return r >= 0;` bool.
// The argument names are descriptive; only their positions are recovered from the pushes.
struct CSoundBankEntry_00a3ed90 {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual long Play(long sound, long position, long flags, long loop, float fade, long priority);   // slot 6 (+0x18)
};

struct CSoundManagerLH {                        // vtable 0x0129D2C4
    virtual void v00();
    CSoundBankEntry_00a3ed90* Find(long key);   // 0x00A3ED30
    virtual bool vfunc_38(long key, long sound, long priority, long position, long flags);
};

bool CSoundManagerLH::vfunc_38(long key, long sound, long priority, long position, long flags)
{
    CSoundBankEntry_00a3ed90* entry = Find(key);
    if (!entry)
        return false;
    return entry->Play(sound, position, flags, 0, 4.0f, priority) >= 0;
}