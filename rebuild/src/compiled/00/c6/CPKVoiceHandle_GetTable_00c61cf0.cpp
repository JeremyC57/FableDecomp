// CPKVoiceHandle::GetTable(CPKEmitter* emitter) -- the voice table a voice handle talks to: resolve the emitter's voice context (0x00C61CE0) and return the table pointer of its device (context +4, device +0x38)
// Leaf: ?GetTable@CPKVoiceHandle@@QAEPAVCPKVoiceTable@@PAVCPKEmitter@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C61CF0, 19 bytes, thiscall `ret 4`: `mov eax,[esp+4] ; push eax ; call
//   0x00C61CE0 ; mov ecx,[eax+4] ; mov eax,[ecx+38h] ; ret 4`.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 442 "0xC61CF0 "CRegion::SetMiniMapRegionExitTextOffsetY" = voice-table lookup
//   (thiscall; CHandleOwner_00c61fb0 declares it __stdcall)"): agent442's landed CPKVoiceHandle::UpdateScaledParameter
//   0x00C61F60 calls it as GetTable(emitter) (bound ?GetTable@CPKVoiceHandle@@... to 0x00C61CF0) and then the table's slot 25
//   with the handle; the caller at 0x00C61F34 loads `mov ecx,ebp` (the handle) before the call, so it is a thiscall member
//   (`this` unused, ecx passed through to 0x00C61CE0). Its third caller 0x00C61FB0 releases the handle through slot 22.
//   0x00C61CE0 (`ret 4`, `this` unused) follows the emitter's owner chain four words deep. The CRegion name and the landed
//   FABLE_STDCALL stand-in are propagated; descriptive names (libpk: no PDB/RTTI).
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

class CPKVoiceDevice {
public:
    unsigned char  _pad00[0x38];
    CPKVoiceTable* m_pVoiceTable;                      // +0x38
};

class CPKVoiceContext {
public:
    void*           m_00;                              // +0x00
    CPKVoiceDevice* m_pDevice;                         // +0x04
};

class CPKEmitter;

class CPKVoiceHandle {
public:
    CPKVoiceContext* GetContext(CPKEmitter* emitter);  // 0x00C61CE0
    CPKVoiceTable* GetTable(CPKEmitter* emitter);
};

CPKVoiceTable* CPKVoiceHandle::GetTable(CPKEmitter* emitter)
{
    return GetContext(emitter)->m_pDevice->m_pVoiceTable;
}
