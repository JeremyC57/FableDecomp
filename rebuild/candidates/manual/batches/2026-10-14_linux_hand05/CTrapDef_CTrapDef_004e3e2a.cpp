// CTrapDef::CTrapDef() -- base definition constructor, own vtable, the member at +0x28 built out of line, the index at +0x40 set to -1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E3E2A, length 30 bytes.
// Callees: the definition base constructor @ 0x0044C0C0 (the common def base behind the
// propagated "CShoveHaPennyRegionDef" label), the member constructor @ 0x00430345 (a frame-pointer
// body in the 0x43 module; the manifest's "CDisplayManager::CopyBackBufferToTexture" label is
// propagated and rejected). This class's vtable is 0x01243054 (rtti-vtables.tsv: CTrapDef).
// Shape: base, vtable, member constructor, then `or dword ptr [esi+40h],-1` -- the member at
// +0x40 is assigned -1 in the body after the member is built; the `or ..,-1` form marks the
// size-optimised def module (#pragma optimize("s", on)).
#pragma optimize("s", on)

struct CDefBase {
    virtual void v00();
    unsigned char _body[0x24];              // +0x04..+0x28
    CDefBase();                             // 0x0044C0C0
};

struct CTrapTrigger {                       // +0x28..+0x40, built out of line
    unsigned char _body[0x18];
    CTrapTrigger();                         // 0x00430345
};

struct CTrapDef : CDefBase {                // vtable 0x01243054
    CTrapTrigger m_trigger;                 // +0x28
    long         m_trapIndex;               // +0x40
    virtual void v00();
    CTrapDef();
};

CTrapDef::CTrapDef()
    : CDefBase()
    , m_trigger()
{
    m_trapIndex = -1;
}
