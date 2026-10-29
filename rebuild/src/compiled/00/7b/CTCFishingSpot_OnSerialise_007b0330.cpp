// CTCFishingSpot::OnSerialise(CPersistContext& context, bool transfer) -- when asked to, persist the "Enabled" flag (default true)
// Leaf: OnSerialise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007B0330, length 39 bytes (vtable 0x0126D51C slot 1, the CTCBase
// "OnSerialise" slot).
// Callees: CPersistContext::Transfer<bool>(const char*, bool&, const bool&) @ 0x004045C0 (manifest "Transfer<signed char>",
// a byte-identical one-byte instantiation; the landed 0x0075D0A0 calls it the same way); the key "Enabled" @ 0x0126D58C.
// Layout: PDB CTCFishingSpot (40 bytes): PParticle1 +0xc, PParticle2 +0x14, PParticleFish +0x1c (three
// CIntelligentPointer<CThing>), Enabled +0x24.
// Shape: `ret 8` for the two arguments; the bool gate `mov al,[esp+8] ; test al,al ; je`; the `true` default is a
// one-byte const-reference temporary homed in the now-dead bool slot (`lea eax,[esp+8]`, `mov byte ptr [esp+14h],1`
// stored right before the call); the member address is `add ecx,24h` on `this`.
#include "engine/CTCBase.h"

class CPersistContext { public:
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);   // CPersistContext::Transfer<T> (member template, PDB)
    //   instances here: bool 0x004045C0
};

class CTCFishingSpot : public CTCBase { public:
    unsigned char PParticle1[8];                 // +0x0c CIntelligentPointer<CThing>
    unsigned char PParticle2[8];                 // +0x14
    unsigned char PParticleFish[8];              // +0x1c
    bool          Enabled;                       // +0x24
    void OnSerialise(CPersistContext& context, bool transfer);
};

void CTCFishingSpot::OnSerialise(CPersistContext& context, bool transfer)
{
    if (transfer)
        context.Transfer("Enabled", Enabled, true);
}