// CSoundManagerLH::vfunc_40(const CCharString& first, const CCharString& second, long a, long b) -- find the entry for the (first, second) name pair (0x00A3EE50); false when none, else call its slot 1 with (a, b) and return true
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A3EED0, length 46 bytes (48 with padding).
// Vtable 0x0129D2C4 (CSoundManagerLH, rtti-vtables.tsv) slot 40.
// Callees: 0x00A3EE50 CSoundManagerLH::FindByNamePair(const CCharString&, const CCharString&) (descriptive; FINDINGS chunk 418:
// it copies both names into a pair<CCharString, CCharString> key and runs the inline CVectorMap find over the pair map at +0x54;
// nothing is inserted, so the manifest label Map_Insert_Sorted_CCharStringPair is wrong, and the keys are names, not longs;
// this function forwards its first two argument words unchanged), slot 1 (+4) of the object it returns.
// Shape: the helper with `this` still in ecx, the early `xor al,al ; ret 10h`, the two remaining arguments
// pushed in reverse order for the virtual call, then `mov al,1`. Argument names are descriptive.
struct CSoundEntry_00a3eed0 {
    virtual void v00();
    virtual void Set(long a, long b);           // slot 1 (+0x04)
};

class CCharString {
public:
    void* PStringData;
};

struct CSoundManagerLH {                        // vtable 0x0129D2C4
    virtual void v00();
    CSoundEntry_00a3eed0* FindByNamePair(const CCharString& first, const CCharString& second);   // 0x00A3EE50 (descriptive)
    virtual bool vfunc_40(const CCharString& first, const CCharString& second, long a, long b);
};

bool CSoundManagerLH::vfunc_40(const CCharString& first, const CCharString& second, long a, long b)
{
    CSoundEntry_00a3eed0* entry = FindByNamePair(first, second);
    if (!entry)
        return false;
    entry->Set(a, b);
    return true;
}