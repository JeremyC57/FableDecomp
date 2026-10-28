// CTCActivationReceptorCreatureGenerator::Validate(CCharString& error) const -- valid when exactly one of ActivateOnActivate / TriggerOnActivate is set; otherwise the error text is assigned and false returned
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F75C0, length 49 bytes.
// Leaf: Validate
// Callees: CCharString::operator=(const char*) @ 0x0099EFE0. Literal 0x01271F18 = "must set one and only one creature generator
// flag, not both or none!!!". Ego_r.pdb: `?Validate@CTCActivationReceptorCreatureGenerator@@MBE_NAAVCCharString@@@Z` (the chunk's
// "vfunc_17"); TriggerOnActivate +0x2c, ActivateOnActivate +0x2d in retail (PDB +0x30/+0x31 behind a 16-byte vector).
// Shape: the exclusive-or written as `(A && !T) || (!A && T)`: A is tested first, the redundant `test al,al ; jne` re-tests A
// on the A-and-T path and T is re-read at the second disjunct; the two `bool` returns are `mov al,1` / `xor al,al`.
class CCharString { public:
    void* m_pData;
    const CCharString& operator=(const char* text); // 0x0099EFE0
};

class CTCActivationReceptorBase { public:             // retail 0x2c bytes (see 0x007F7710)
    virtual void v00();
    unsigned char _body[0x28];
};

struct CTCActivationReceptorCreatureGenerator : CTCActivationReceptorBase {
    bool TriggerOnActivate;                    // +0x2c
    bool ActivateOnActivate;                   // +0x2d
    bool Validate(CCharString& error) const;
};

bool CTCActivationReceptorCreatureGenerator::Validate(CCharString& error) const
{
    if ((ActivateOnActivate && !TriggerOnActivate) || (!ActivateOnActivate && TriggerOnActivate))
        return true;
    error = "must set one and only one creature generator flag, not both or none!!!";
    return false;
}