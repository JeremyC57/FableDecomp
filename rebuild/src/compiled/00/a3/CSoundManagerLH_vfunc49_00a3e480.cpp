// CSoundManagerLH::vfunc_49(long id) -- true when the sound system at +0x40 exists and its player (slot 5) reports state 2 for `id` (slot 6)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A3E480, length 41 bytes (48 with padding).
// Vtable 0x0129D2C4 (CSoundManagerLH, rtti-vtables.tsv) slot 49.
// Shape: null test of +0x40 jumping to the shared `xor al,al ; ret 4` tail at the end, slot 5 (+0x14) on the
// object, slot 6 (+0x18) on its result with the argument, `cmp eax,2 ; jne` to the same tail, `mov al,1`:
// the nested `if (p) { if (... == 2) return true; } return false;` form with a bool result.
struct CSoundPlayer_00a3e480 {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual long GetState(long id);             // slot 6 (+0x18)
};

struct CSoundSystem_00a3e480 {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual CSoundPlayer_00a3e480* GetPlayer(); // slot 5 (+0x14)
};

struct CSoundManagerLH {                        // vtable 0x0129D2C4
    virtual void v00();
    unsigned char          _04[0x3c];           // +0x04..+0x3f
    CSoundSystem_00a3e480* m_pSystem;           // +0x40
    virtual bool vfunc_49(long id);
};

bool CSoundManagerLH::vfunc_49(long id)
{
    if (m_pSystem) {
        if (m_pSystem->GetPlayer()->GetState(id) == 2)
            return true;
    }
    return false;
}