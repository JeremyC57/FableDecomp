// CTCLight::RequestAppearanceUpdate() -- while the light is active and its def asks for it (byte +0x42), forward request 4 to the owning thing's graphic appearance
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00640D00, length 28 bytes.
// Leaf: RequestAppearanceUpdate
// Callees: the CThing forwarder @ 0x004C7590 (`if (PTCGraphicAppearance at +0x64) jmp 0x004BF9E0`, one long argument; the
// manifest's CRenderTarget::DetachTarget is propagated).
// Vtable: slot 9 of CTCLight's primary vtable (0x0125A194); the real slot name is not recorded (catalog "vfunc_9"), so the
// method is named from what it does. Layout (local model, no engine header): the CTCBase owner at +4, PLightDef +0x18,
// Active +0x1c. Shape: two byte tests through al/dl, the owner loaded into ecx last; retail keeps `call ; ret`.
class CLightDef { public:
    unsigned char _pad_0x00[0x42];
    bool          m_bNeedsAppearanceUpdate;        // +0x42
};

class CThing { public:
    void ForwardToGraphicAppearance(long request);  // 0x004C7590
};

class CTCLight { public:
    void*            __vftable;                    // +0x00 (CTCBase)
    CThing*          m_pThing;                     // +0x04 (CTCBase owner)
    unsigned char    _pad_0x08[0x18 - 0x08];
    const CLightDef* PLightDef;                    // +0x18
    bool             Active;                       // +0x1c
    void RequestAppearanceUpdate();
};

void CTCLight::RequestAppearanceUpdate()
{
    if (Active && PLightDef->m_bNeedsAppearanceUpdate)
        m_pThing->ForwardToGraphicAppearance(4);
}