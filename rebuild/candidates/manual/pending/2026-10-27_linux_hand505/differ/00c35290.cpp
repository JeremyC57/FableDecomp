// CEnvelope_012b37dc::Update(float speed) -- advance the three-phase envelope by the milliseconds since the last update (GetTickCount delta * rate / 1000, truncated): attack, then the loop phase (repeated while loops remain, forever when negative), then release, then done; restamps the tick and stores the new rate speed * m_speed
// Grade: DIFFER(251v220) -- the phase bodies cache m_pos in a register (`mov edi,[esi+0Ch] ; add edi,ecx ; cmp edi,edx ; mov [esi+0Ch],edi`, zero in ebx) where retail works on memory (`sub ecx,[esi+0Ch] ; add [esi+0Ch],ecx ; cmp [esi+0Ch],edx`, zero in edi, no ebx push, no loop-head pad). /Os gives the memory forms but adds `and [m],0` stores and a GetTickCount pointer in ebx (231 bytes).
// Leaf: ?Update@CEnvelope_012b37dc@@UAEXM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C35290, 220 bytes, thiscall `ret 4`.
// Identity (descriptive; libpk has no PDB or RTTI): slot 0 of vtable 0x012B37DC (its only reference, 0x012B37DC), the
//   0x28-byte polymorphic member at +0x78 of the landed CScriptModuleBase constructor 0x00C21BA0 (inline ctor stores the
//   vtable at 0x00C21BE3); slot 2 GetPosition 0x00C35370 calls this slot with 1.0f. Catalog gapscan name is a placeholder.
// Callees: GetTickCount (IAT 0x0143FDFC, twice), __ftol2 0x00BFEA70; constants 0.001f (0x01266300) and the 2^32 unsigned
//   fix-up (0x0122DCB4).
// Shape: each phase open-codes `n = length - pos; clamp; pos += n; ticks -= n` on the members (retail `add [esi+0Ch],ecx`);
//   an inline Advance(length, ticks&) helper re-reads pos into a register (the reference may alias it). The default
//   case `ticks = 0` threads straight to the loop exit.
extern "C" __declspec(dllimport) unsigned long __stdcall GetTickCount();   // IAT 0x0143FDFC

struct SEnvelopePosition_012b37dc {           // what GetPosition reports
    float m_fraction;                         // +0x00: position over the whole envelope (0..1)
    long  m_loopsLeft;                        // +0x04
};

class CEnvelope_012b37dc {                    // 0x28 bytes; vtable 0x012B37DC (the member at +0x78 of the landed CScriptModuleBase)
public:
    virtual void Update(float speed);                                          // slot 0 (0x00C35290)
    virtual void v01();                                                        // slot 1 (0x00C35470)
    virtual void GetPosition(void* context, SEnvelopePosition_012b37dc* out);  // slot 2 (0x00C35370)
    float         m_speed;                    // +0x04
    long          m_loopsLeft;                // +0x08: <0 loops forever
    unsigned long m_pos;                      // +0x0c: ticks into the current phase
    unsigned long m_state;                    // +0x10: 0 attack, 1 loop, 2 release, 3 done
    unsigned long m_attack;                   // +0x14
    unsigned long m_loop;                     // +0x18
    unsigned long m_release;                  // +0x1c
    unsigned long m_lastTick;                 // +0x20
    float         m_rate;                     // +0x24: phase ticks per millisecond
};

void CEnvelope_012b37dc::Update(float speed)
{
    unsigned long ticks = (unsigned long)((float)(GetTickCount() - m_lastTick) * m_rate * 0.001f);
    while (ticks > 0) {
        unsigned long n;
        switch (m_state) {
        case 0:
            n = m_attack - m_pos;
            if (n > ticks)
                n = ticks;
            m_pos += n;
            ticks -= n;
            if (m_pos == m_attack) {
                m_state = 1;
                m_pos = 0;
            }
            break;
        case 1:
            n = m_loop - m_pos;
            if (n > ticks)
                n = ticks;
            m_pos += n;
            ticks -= n;
            if (m_pos == m_loop) {
                m_pos = 0;
                if (m_loopsLeft == 0)
                    m_state = 2;
                else if (m_loopsLeft > 0)
                    --m_loopsLeft;
            }
            break;
        case 2:
            n = m_release - m_pos;
            if (n > ticks)
                n = ticks;
            m_pos += n;
            ticks -= n;
            if (m_pos == m_release) {
                m_state = 3;
                m_pos = 0;
            }
            break;
        default:
            ticks = 0;
            break;
        }
    }
    m_lastTick = GetTickCount();
    m_rate = speed * m_speed;
}
