// CPKVoiceParams::SetMinDistance(float value) -- the sound's minimum distance (range minimum minus the listener distance): store the value through the record's +8 slot (0x00C1BB80) and set flag bit 4 of the record's first word
// Leaf: ?SetMinDistance@CPKVoiceParams@@QAEXM@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C20480 (thiscall, `ret 4`).
// Naming (naming wave 21, DECISIONS_W20_RESOLVED / chunk 687): a setter of the libpk voice play-parameter record CPKVoiceParams (ctor 0x00C10A30);
//   CAmbientTrigger::Play 0x00C62530 passes the sound's minimum distance (range minimum minus the listener distance) (`fstp [esp]`: a float). Was landed as a CBrainOutput::SetAsPumpCombatActions stand-in
//   (propagated). Descriptive (libpk, no PDB): the slot callee 0x00C1BB80 is named by its address (the landed callers bind it under several
//   stand-in spellings).
struct CPKParamSlot_00c1bb80 {               // the record's +8 word
    void Set_00c1bb80(float value);              // 0x00C1BB80 (thiscall: the slot in ecx, the value on the stack)
};

struct CPKVoiceParams {
    unsigned long         m_flags;           // +0x00
    long                  m_4;               // +0x04
    CPKParamSlot_00c1bb80 m_slot8;           // +0x08
    void SetMinDistance(float value);
};

void CPKVoiceParams::SetMinDistance(float value)
{
    m_slot8.Set_00c1bb80(value);
    m_flags |= 4;
}
