// CIsThingAlive::operator()(T) -- Checker(+0x74).vslot17(&argument)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C0E950, oracle length 17 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  mov eax,[ecx+74h] ; add ecx,74h ; lea edx,[esp+4] ; push edx ; call [eax+44h] ; ret 4
// The predicate forwards its by-value argument (taken by reference by the callee: `lea edx,[esp+4]`)
// to virtual slot 17 of the polymorphic member at +0x74 and returns that result.
struct CAliveCheckerVirtuals {
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40();
    virtual bool IsAlive(const void* const& thing);   // slot 17 (+0x44)
};

struct CIsThingAlive_Retail {
    unsigned char         _pad_0x00[0x74];
    CAliveCheckerVirtuals Checker;          // +0x74 (embedded polymorphic member)
    bool operator()(void* thing);
};

bool CIsThingAlive_Retail::operator()(void* thing)
{
    return this->Checker.IsAlive(thing);
}
