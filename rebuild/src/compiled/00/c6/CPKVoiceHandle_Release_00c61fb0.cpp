// CPKVoiceHandle::Release(CPKEmitter* emitter) -- when the voice handle at +0xc is live (not negative), release it through the voice table (GetTable 0x00C61CF0, virtual slot 22) and mark the handle invalid (-1)
// Leaf: ?Release@CPKVoiceHandle@@QAEXPAVCPKEmitter@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C61FB0, 41 bytes, thiscall `ret 4`.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 442, 0xC61CF0 item): agent442 identifies `this` as the voice handle of
//   CPKVoiceHandle::UpdateScaledParameter 0x00C61F60 (handle +0xc, -1 = none) and the callee 0x00C61CF0 as its thiscall
//   GetTable(emitter) (bound ?GetTable@CPKVoiceHandle@@... from 0x00C61F60). The landed model declared the getter as a
//   `__stdcall` free function taking a `long` "context": the bytes are the same (ecx still holds `this` from entry, so no
//   `mov ecx` is needed), but the declaration was wrong. The CHandleOwner_00c61fb0 / "CItem::GetItemThings" names are
//   propagated; descriptive names (libpk: no PDB/RTTI).
// Shape: the handle is read once into edi (`test edi,edi ; jl`), then `push emitter ; call GetTable ; push handle ;
//   call [edx+58h]`.
class CPKVoiceTable {                               // the voice table reached from the emitter
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

class CPKEmitter;

class CPKVoiceHandle {
public:
    unsigned char _pad000[8];
    bool          m_bUnscaled;                         // +8
    long          m_handle;                            // +0xc (-1 = none)
    CPKVoiceTable* GetTable(CPKEmitter* emitter);      // 0x00C61CF0
    void Release(CPKEmitter* emitter);
};

void CPKVoiceHandle::Release(CPKEmitter* emitter)
{
    long handle = m_handle;
    if (handle >= 0) {
        GetTable(emitter)->Release(handle);
        m_handle = -1;
    }
}
