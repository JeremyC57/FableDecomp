// CSoundManagerLH::vfunc_40(long key, long name, long a, long b) -- find/insert the entry for (key, name) (0x00A3EE50); false when none, else call its slot 1 with (a, b) and return true
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A3EED0, length 46 bytes (48 with padding).
// Vtable 0x0129D2C4 (CSoundManagerLH, rtti-vtables.tsv) slot 40.
// Callees: 0x00A3EE50 (a thiscall on `this` with the first two arguments; manifest label
// Map_Insert_Sorted_CCharStringPair), slot 1 (+4) of the object it returns.
// Shape: the helper with `this` still in ecx, the early `xor al,al ; ret 10h`, the two remaining arguments
// pushed in reverse order for the virtual call, then `mov al,1`. Argument names are descriptive.
struct CSoundEntry_00a3eed0 {
    virtual void v00();
    virtual void Set(long a, long b);           // slot 1 (+0x04)
};

struct CSoundManagerLH {                        // vtable 0x0129D2C4
    virtual void v00();
    CSoundEntry_00a3eed0* Lookup(long key, long name);   // 0x00A3EE50
    virtual bool vfunc_40(long key, long name, long a, long b);
};

bool CSoundManagerLH::vfunc_40(long key, long name, long a, long b)
{
    CSoundEntry_00a3eed0* entry = Lookup(key, name);
    if (!entry)
        return false;
    entry->Set(a, b);
    return true;
}