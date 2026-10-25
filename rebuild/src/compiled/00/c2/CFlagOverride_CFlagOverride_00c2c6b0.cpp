// CFlagOverride::CFlagOverride() -- the 12-byte libpk flag override: its vtable (0x012B4630) and the two (mask, value) word pairs zeroed (+0x04/+0x06 group B, +0x08/+0x0A group A)
// Leaf: ??0CFlagOverride@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2C6B0, 27 bytes: mov eax,ecx ; xor ecx,ecx ;
//   mov dword ptr [eax],12B4630h ; mov word ptr [eax+4],cx ; mov word ptr [eax+6],cx ; mov word ptr [eax+8],cx ;
//   mov word ptr [eax+0Ah],cx ; ret
// Naming (sweep1, 2026-10-25): was landed as NScript::CQ_GuildTrainingPreMeleeScript::Init, retyped onto that game
//   class's engine header by label alone (a propagated manifest label; the stand-in stored an extern word as the
//   vtable). FINDINGS chunk 436: it is the libpk CFlagOverride constructor -- the class whose landed ApplyTo
//   0x00C2C670 merges the same four words into a flag state (agent455's PKCPlaybackTemplate::ApplyTo 0x00C14A80 calls
//   ApplyTo on three of these at +0x78/+0x84/+0x90 and names the class PKCFlagPatch). Descriptive (no PDB or RTTI covers
//   libpk). The base vtable 0x012B2828 (slots 0, 1 and 3 `_purecall`) is stored first by the inline base constructor
//   and dropped as a dead store.
class CFlagOverrideBase {                       // vtable 0x012B2828
public:
    virtual void v00();
    CFlagOverrideBase() { }
};

class CFlagOverride : public CFlagOverrideBase {   // vtable 0x012B4630, 12 bytes
public:
    CFlagOverride();
    virtual void v00();
    unsigned short m_maskB;                     // +0x04
    unsigned short m_valueB;                    // +0x06
    unsigned short m_maskA;                     // +0x08
    unsigned short m_valueA;                    // +0x0a
};

CFlagOverride::CFlagOverride()
    : m_maskB(0), m_valueB(0), m_maskA(0), m_valueA(0)
{
}
