// CAmbientEmitter::Update() -- per-frame update of an ambient emitter: push the current volume to its channels, finish once the definition says so, then fire its due events
// Leaf: ?Update@CAmbientEmitter@@QAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C44E80, 35 bytes, thiscall, tail `jmp 0x00C44BD0` (ProcessEvents).
// Library: libpk ambient sound (no PDB/RTTI): descriptive names. Identity: the three callees are the landed CAmbientEmitter members
//   (Finish 0x00C44E30, whose header names this row as its only caller, and ProcessEvents 0x00C44BD0, whose only caller is this tail
//   `jmp`), so this is the emitter's per-frame update; named Update by analogy with the landed CAmbientSource::Update 0x00C47300
//   (same "definition IsFinished -> Finish" shape, same IsFinished 0x00C384E0 on the def at +4). Only caller: the `__stdcall` API
//   forwarder 0x00C36230 (bound as the placeholder ?Method_00c44e80@CObject_00c44e80@@QAEXXZ; the catalog's
//   "CTCShotDefinitionFireballSpell::~CTCShotDefinitionFireballSpell" is propagated).
// Callees: CAmbientEmitter::UpdateVolume 0x00C44DC0 (descriptive; landed under the placeholder
//   CFireballSpell_00c44dc0::DispatchScaledDamage: owner cube level times the emitter's value, visited over the +0x50 channel list),
//   ?IsFinished@CAmbientSourceDef@@QBE_NXZ 0x00C384E0 (landed spelling), ?Finish@CAmbientEmitter@@QAEXXZ 0x00C44E30,
//   ?ProcessEvents@CAmbientEmitter@@QAEXXZ 0x00C44BD0.
// Shape: plain calls in order; the last call is a sibling `jmp`.
struct CAmbientSourceDef {
    bool IsFinished() const;                // 0x00C384E0 (compares the floats at +0x20 / +0x24)
};

class CAmbientEmitter {
public:
    void*              m_pOwner;            // +0x00
    CAmbientSourceDef* m_pDef;              // +0x04
    void UpdateVolume();                    // 0x00C44DC0
    void Finish();                          // 0x00C44E30
    void ProcessEvents();                   // 0x00C44BD0
    void Update();
};

void CAmbientEmitter::Update()
{
    UpdateVolume();
    if (m_pDef->IsFinished())
        Finish();
    ProcessEvents();
}
