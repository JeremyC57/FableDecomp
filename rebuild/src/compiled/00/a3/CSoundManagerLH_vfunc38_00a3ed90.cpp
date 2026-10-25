// CSoundManagerLH::vfunc_38(const CCharString& name, long sound, long priority, long position, long flags) -- look the object up by name (0x00A3ED30); false when absent, else its slot-6 play call with (sound, position, flags, 0, 4.0f, priority) succeeding (>= 0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A3ED90, length 64 bytes (80 with padding).
// Vtable 0x0129D2C4 (CSoundManagerLH, rtti-vtables.tsv) slot 38.
// Callees: 0x00A3ED30 CSoundManagerLH::FindByName(const CCharString&) (descriptive; FINDINGS chunk 418: the inline CVectorMap
// find in the name map at +0x64 through CBasicString<char>::operator< 0x00429950 -- the key is a CCharString passed by reference,
// not a long; this function forwards its first argument word unchanged), // slot 6 (+0x18) of the object it returns.
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

class CCharString {
public:
    void* PStringData;
};

struct CSoundManagerLH {                        // vtable 0x0129D2C4
    virtual void v00();
    CSoundBankEntry_00a3ed90* FindByName(const CCharString& name);   // 0x00A3ED30 (descriptive)
    virtual bool vfunc_38(const CCharString& name, long sound, long priority, long position, long flags);
};

bool CSoundManagerLH::vfunc_38(const CCharString& name, long sound, long priority, long position, long flags)
{
    CSoundBankEntry_00a3ed90* entry = FindByName(name);
    if (!entry)
        return false;
    return entry->Play(sound, position, flags, 0, 4.0f, priority) >= 0;
}