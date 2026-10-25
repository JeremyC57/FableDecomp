// CPKVoiceHandle::UpdateScaledParameter(CPKEmitter* emitter) -- push the emitter's ramped value (times the owner's scale unless unscaled) to the live voice
// Leaf: ?UpdateScaledParameter@CPKVoiceHandle@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C61F60, 80 bytes (thiscall, `ret 4`).
// Callees: the emitter getter 0x00C44A90 (a ramped value 0x00C38510 times the owner's +0x160 float), the owner's +0x4c
//   ramped parameter 0x00C45940, the voice-table lookup 0x00C61CF0 (a thiscall member: `mov ecx,esi` before it), then
//   the table's vtable slot 25 (+0x64) with (handle, value).
// Identity: the manifest's "CRegion_UpdateMouseCursorPosition" is a propagated label. `this` is the same voice-handle
//   object as 0x00C61FB0 (handle at +0xc, -1 = none; that row releases it through slot 22). Descriptive names: the
//   parameter is most likely a gain/volume but nothing in the image names it.
// Shape: the float lives in the dead `emitter` argument slot; any of `v *= s`, `v = v * s`, `v = s * v` matches.
class CPKVoiceTable {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21();
    virtual void Release(long handle);                 // slot 22 (+0x58)
    virtual void v23(); virtual void v24();
    virtual void SetValue(long handle, float value);   // slot 25 (+0x64)
};
class CPKScaleSource {
public:
    float GetScale();                                  // 0x00C45940
};
class CPKEmitterOwner {
public:
    unsigned char  _pad000[0x4c];
    CPKScaleSource m_scale;                            // +0x4c
};
class CPKEmitter {
public:
    CPKEmitterOwner* m_pOwner;                         // +0
    float GetValue();                                  // 0x00C44A90
};
class CPKVoiceHandle {
public:
    unsigned char _pad000[8];
    bool          m_bUnscaled;                         // +8
    long          m_handle;                            // +0xc
    CPKVoiceTable* GetTable(CPKEmitter* emitter);      // 0x00C61CF0
    void UpdateScaledParameter(CPKEmitter* emitter);
};

void CPKVoiceHandle::UpdateScaledParameter(CPKEmitter* emitter)
{
    if (m_handle >= 0)
    {
        float value = emitter->GetValue();
        if (!m_bUnscaled)
            value *= emitter->m_pOwner->m_scale.GetScale();
        GetTable(emitter)->SetValue(m_handle, value);
    }
}