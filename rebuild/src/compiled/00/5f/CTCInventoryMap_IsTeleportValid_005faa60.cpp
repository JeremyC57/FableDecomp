// CTCInventoryMap::IsTeleportValid() -- true when the display engine's teleport flag is set and the map screen's secondary NHeroInformationScreens::CBase slot 3 reports no blocking state
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005FAA60, 45 bytes (+3 int3 padding).
// Callees: CViewBase::GetDisplayEngine-shaped accessor on `this` @ 0x00686A80 (returns the display engine; manifest
//          label CViewBase::GetDisplayEngine), virtual slot 3 (+0x0C) of the secondary base at +0x130.
//
// Shape: `call 0x686a80 ; mov cl,[eax+0D5h] ; test cl,cl ; je F ; mov eax,[esi+130h] ; lea ecx,[esi+130h] ;
// call [eax+0Ch] ; test al,al ; jne F ; mov al,1 ; ret ; F: xor al,al ; ret`.
// The two separate exits are `if (a && !b) return true; return false;` (GOTCHAS wave 24p). The +0x130 sub-object is
// the NHeroInformationScreens::CBase secondary base every CTCInventory<Screen> carries (rtti-vtables: CTCInventoryMap's
// second vtable 0x01257A84; CTCInventoryStats' destructor stores the CBase vtable 0x0123A3E0 at +0x130); slot 3 of the
// map's copy (0x005FA590) returns the byte at map+0x240, the CBase default (0x004D2E41) returns true. The flag byte
// at +0xD5 of the display engine is named from use. Local model: no engine header for CTCInventoryMap / CBase.
struct CDisplayEngine {
    unsigned char _pad_0x00[0xd5];
    unsigned char m_teleportAllowed;          // +0xD5 (name from use)
};

struct CTCInventoryBase_Retail {              // primary base, vptr at +0; retail size 0x130
    virtual void v00();
    unsigned char _body[0x130 - 4];
};

namespace NHeroInformationScreens {
struct CBase {                                // secondary base at +0x130 (vtable 0x0123A3E0 for the plain base)
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual unsigned char IsBlocked();        // slot 3 (+0x0C)
};
}

struct CTCInventoryMap : CTCInventoryBase_Retail, NHeroInformationScreens::CBase {
    CDisplayEngine* GetDisplayEngine();       // 0x00686A80
    bool IsTeleportValid();
};

bool CTCInventoryMap::IsTeleportValid()
{
    if (GetDisplayEngine()->m_teleportAllowed && !IsBlocked())
        return true;
    return false;
}