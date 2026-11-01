// CPKVoiceParams::SetMaxDistance(float value) -- the sound's maximum distance (range maximum minus the listener distance): store the value through the record's +8 slot (0x00C1BBA0) and set flag bit 8 of the record's first word
// Leaf: ?SetMaxDistance@CPKVoiceParams@@QAEXM@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C204A0 (thiscall, `ret 4`).
// Naming (naming wave 21, DECISIONS_W20_RESOLVED / chunk 687): a setter of the libpk voice play-parameter record CPKVoiceParams (ctor 0x00C10A30);
//   CAmbientTrigger::Play 0x00C62530 passes the sound's maximum distance (range maximum minus the listener distance) (`fstp [esp]`: a float). Was landed as a CBrainOutput::SetAsPumpCombatActions stand-in
//   (propagated). Descriptive (libpk, no PDB): the slot callee 0x00C1BBA0 is named by its address (the landed callers bind it under several
//   stand-in spellings).
struct CPKParamSlot_00c1bb80 {               // the record's +8 word
    void Set_00c1bba0(float value);              // 0x00C1BBA0 (thiscall: the slot in ecx, the value on the stack)
};

struct CPKVoiceParams {
    unsigned long         m_flags;           // +0x00
    long                  m_4;               // +0x04
    CPKParamSlot_00c1bb80 m_slot8;           // +0x08
    void SetMaxDistance(float value);
};

void CPKVoiceParams::SetMaxDistance(float value)
{
    m_slot8.Set_00c1bba0(value);
    m_flags |= 8;
}
