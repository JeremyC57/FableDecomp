// CAmbientEmitter::GetVolume() const -- the emitter's ramped volume parameter (+4) times its owner's (+0) +0x160 scale
// Leaf: ?GetVolume@CAmbientEmitter@@QBEMXZ
// Vendor naming wave V1 (one address, one name): 0x00C38510 ?GetValue@CRampedParamOwner_00c38510@@QBEMXZ (was ?GetValue@CRampedParam_00c38510@@QBEMXZ)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C44A90, 25 bytes (thiscall, float in st0).
// Name (naming wave 20, chunk 687): descriptive (libpk, no PDB) -- CAmbientEvent::Trigger 0x00C61D10 calls it on the emitter for the volume it hands
//   to the voice parameters (compared with 0.0f); the landed CPKVoiceHandle::UpdateScaledParameter 0x00C61F60 and the emitter's own 0x00C44AB0
//   call it too. Was the propagated CMouseCursor::TurnToPointer (a __fastcall free stub). The ramped parameter's getter is the landed
//   ?GetValue@CRampedParamOwner_00c38510@@QBEMXZ 0x00C38510.
class CAmbientEmitterOwner { public: unsigned char _pad[0x160]; float m_scale; };   // +0x160 (descriptive)
class CRampedParamOwner_00c38510 { public: float GetValue() const; };                    // 0x00C38510 (the landed row's spelling)
class CAmbientEmitter { public:
    CAmbientEmitterOwner*  m_pOwner;              // +0x00
    CRampedParamOwner_00c38510* m_pVolume;             // +0x04
    float GetVolume() const;
};
float CAmbientEmitter::GetVolume() const
{
    float v = m_pVolume->GetValue();
    return v * m_pOwner->m_scale;
}
