// CSoundManagerLH::vfunc_1(long bank) -- allocate a CSoundBankLH owned by this manager (0x20 bytes, constructor 0x00A0BAC0), call its slot 1 with the argument and return it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A3E430, length 64 bytes (80 with padding).
// Vtable 0x0129D2C4 (CSoundManagerLH, rtti-vtables.tsv) slot 1.
// Callees: 0x00BFEA1A (operator new, 0x20 bytes), 0x00A0BAC0 (CSoundBankLH::CSoundBankLH, the manager as its
// one argument; landed, its stored word at +0x0c), slot 1 (+4) of the new bank (0x00A0B630).
// Shape: the `new` expression's null test; VC7.1 duplicates the tail (the slot-1 call and `return`) into
// both the constructed and the null arm (`xor esi,esi` then the same call through a null pointer).
struct CSoundManagerLH;

struct CSoundBankLH {                           // vtable 0x0129C9DC, 0x20 bytes
    virtual void v00();
    virtual void Load(long bank);               // slot 1 (+0x04): 0x00A0B630
    unsigned char _04[0x1c];                    // +0x04..+0x1f
    CSoundBankLH(CSoundManagerLH* owner);       // 0x00A0BAC0
};

struct CSoundManagerLH {                        // vtable 0x0129D2C4
    virtual void v00();
    virtual CSoundBankLH* vfunc_1(long bank);
};

CSoundBankLH* CSoundManagerLH::vfunc_1(long bank)
{
    CSoundBankLH* result = new CSoundBankLH(this);
    result->Load(bank);
    return result;
}
