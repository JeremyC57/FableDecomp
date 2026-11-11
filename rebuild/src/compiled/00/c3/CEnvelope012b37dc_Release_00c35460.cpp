// CEnvelope_012b37dc::Release() -- vtable slot 4 of the libpk envelope: releasing the envelope ends it at once (forwards to the virtual Stop, slot 5)
// Leaf: ?Release@CEnvelope_012b37dc@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C35460, 5 bytes (`mov eax,[ecx]; jmp [eax+0x14]`), thiscall; slot 4 of the vtable
//   0x012B37DC (cell 0x012B37EC).
// Identity (descriptive; libpk has no PDB or RTTI): the envelope class of the landed GetPosition 0x00C35370 (slot 2), named by
//   its vtable. Slot 5 (+0x14) is 0x00C35450 (landed as the shape row ResetPair_00c35450: m_state = 3 "done", m_loopsLeft = 0),
//   called Stop here. Both names are descriptive (NOTES).
// Extent: the catalog row ran through int3 padding into three more one-instruction slots (0x00C35470 `ret 8` slot 1,
//   0x00C35480 `ret 4` slot 6, 0x00C35490 `ret 4` slot 7); see SPLITS.tsv.
struct SEnvelopePosition_012b37dc;
class CEnvelope_012b37dc {                    // 0x28 bytes; vtable 0x012B37DC
public:
    virtual void Update(float speed);                                          // slot 0 (0x00C35290)
    virtual void v01(long, long);                                              // slot 1 (0x00C35470, empty `ret 8`)
    virtual void GetPosition(void* context, SEnvelopePosition_012b37dc* out);  // slot 2 (0x00C35370)
    virtual void v03();                                                        // slot 3 (0x00C35440)
    virtual void Release();                                                    // slot 4 (0x00C35460)
    virtual void Stop();                                                       // slot 5 (0x00C35450)
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

void CEnvelope_012b37dc::Release()
{
    Stop();
}