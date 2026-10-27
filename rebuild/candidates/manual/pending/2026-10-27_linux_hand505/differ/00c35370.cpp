// CEnvelope_012b37dc::GetPosition(void* context, SEnvelopePosition_012b37dc* out) -- bring the envelope up to date (Update(1.0f) through slot 0), then report the position over the whole envelope (ticks done / attack+loop+release) and the loops left (0 once releasing; done reports 1.0)
// Grade: DIFFER(198v198) -- operand order only: the phase-length sum and the case-2 sum add the loop length before the attack length (`add edx,ecx ; add edx,eax`, loads ecx before edx) where retail adds attack first (`mov edx,[esi+1Ch] ; mov ecx,[esi+18h] ; add edx,eax ; add edx,ecx`); both source orders of the sum gave the same canonical order.
// Leaf: ?GetPosition@CEnvelope_012b37dc@@UAEXPAXPAUSEnvelopePosition_012b37dc@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C35370, 198 bytes, thiscall `ret 8`.
// Identity (descriptive; libpk has no PDB or RTTI): slot 2 of vtable 0x012B37DC (its only reference, 0x012B37E4), the
//   envelope member at +0x78 of the landed CScriptModuleBase 0x00C21BA0; slot 0 is Update 0x00C35290. The first argument
//   is never read. Catalog gapscan name is a placeholder.
// Callees: slot 0 (virtual); constant 2^32 (0x0122DCB4) for the unsigned-to-float fix-ups.
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

void CEnvelope_012b37dc::GetPosition(void* context, SEnvelopePosition_012b37dc* out)
{
    Update(1.0f);
    unsigned long attack = m_attack;
    unsigned long loop = m_loop;
    float total = (float)(m_release + attack + loop);
    unsigned long done;
    switch (m_state) {
    case 0:
        done = m_pos;
        out->m_loopsLeft = m_loopsLeft;
        out->m_fraction = (float)done / total;
        break;
    case 1:
        done = m_pos + attack;
        out->m_loopsLeft = m_loopsLeft;
        out->m_fraction = (float)done / total;
        break;
    case 2:
        done = m_pos + attack + loop;
        out->m_loopsLeft = 0;
        out->m_fraction = (float)done / total;
        break;
    default:
        out->m_loopsLeft = 0;
        out->m_fraction = 1.0f;
        break;
    }
}
