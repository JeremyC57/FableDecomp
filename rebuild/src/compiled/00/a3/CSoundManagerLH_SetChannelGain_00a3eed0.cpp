// CSoundManagerLH::SetChannelGain() -- set the gain of an atmos channel with a fade
// Name (agentSND, 2026-11-10): vtable slot 40 (+0xA0); spelling from the Fable Anniversary 360 PDB publics (CSoundManagerUN / CASoundManager at the same vtable position) with the TLC slot evidence in SLOTS.tsv; was vfunc_40.
// Leaf: SetChannelGain
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A3EED0, length 46 bytes (48 with padding).
// Vtable 0x0129D2C4 (CSoundManagerLH, rtti-vtables.tsv) slot 40.
// Callees: 0x00A3EE50 CSoundManagerLH::FindByNamePair(const CCharString&, const CCharString&) (descriptive; FINDINGS chunk 418:
// it copies both names into a pair<CCharString, CCharString> key and runs the inline CVectorMap find over the pair map at +0x54;
// nothing is inserted, so the manifest label Map_Insert_Sorted_CCharStringPair is wrong, and the keys are names, not longs;
// this function forwards its first two argument words unchanged), slot 1 (+4) of the object it returns.
// Shape: the helper with `this` still in ecx, the early `xor al,al ; ret 10h`, the two remaining arguments
// pushed in reverse order for the virtual call, then `mov al,1`. Argument names are descriptive.
class CSoundBankLH {                            // the bank FindByNamePair returns (as 0x00A3F250)
public:
    virtual void v00();
    virtual void Set(long a, long b);           // slot 1 (+0x04)
};

class CCharString {
public:
    void* PStringData;
};

struct CSoundManagerLH {                        // vtable 0x0129D2C4
    virtual void v00();
    CSoundBankLH* FindByNamePair(const CCharString& first, const CCharString& second);   // 0x00A3EE50 (descriptive)
    virtual bool SetChannelGain(const CCharString& first, const CCharString& second, long a, long b);
};

bool CSoundManagerLH::SetChannelGain(const CCharString& first, const CCharString& second, long a, long b)
{
    CSoundBankLH* entry = FindByNamePair(first, second);
    if (!entry)
        return false;
    entry->Set(a, b);
    return true;
}