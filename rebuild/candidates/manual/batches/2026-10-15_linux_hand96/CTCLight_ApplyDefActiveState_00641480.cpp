// CTCLight::ApplyDefActiveState() -- once a light def is attached and the component is not yet initialised, switch the light on or off from the def's default-active byte (+0x40) through SetActive
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00641480, length 25 bytes.
// Leaf: ApplyDefActiveState
// Callees: CTCLight::SetActive(bool) @ 0x006412B0 (the byte widened with `movzx` and pushed).
// Vtable: slot 6 of CTCLight's primary vtable (0x0125A194); the slot's real name is not recorded (catalog "vfunc_6"),
// so the method is named from what it does. Layout (local model, no engine header): PLightDef +0x18 (the def pointer),
// Initialised +0x28 (PDB offsets; nothing before the first retail vector moves).
// Shape: two early-outs then the call; retail keeps `call ; ret` for the last call.
struct CLightDef {
    unsigned char _pad_0x00[0x40];
    bool          m_bStartActive;                  // +0x40
};

struct CTCLight {
    unsigned char    _pad_0x00[0x18];
    const CLightDef* PLightDef;                    // +0x18
    unsigned char    _pad_0x1c[0x28 - 0x1c];
    bool             Initialised;                  // +0x28
    void SetActive(bool active);                   // 0x006412B0
    void ApplyDefActiveState();
};

void CTCLight::ApplyDefActiveState()
{
    if (PLightDef && !Initialised)
        SetActive(PLightDef->m_bStartActive);
}
