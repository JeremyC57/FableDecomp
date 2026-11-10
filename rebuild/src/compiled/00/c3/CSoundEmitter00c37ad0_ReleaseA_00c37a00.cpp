// CSoundEmitter_00c37ad0::ReleaseA(long handle) -- slot 1 of the sound emitter: hand the emitter back to the registry that owns it (+4), which destroys it (CInstanceRegistry::DestroyInstance: ~CInstance 0x00C37DA0, then operator delete)
// Leaf: ?ReleaseA@CSoundEmitter_00c37ad0@@UAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C37A00, 12 bytes, thiscall `ret 4` through the tail call: `mov [esp+4],ecx ;
//   mov ecx,[ecx+4] ; jmp 0x00C2BAF0` (the unused handle's stack slot is overwritten with `this`, the registry's argument).
// Identity: slot 1 of the emitter vtable 0x012B5370 (the only reference, cell 0x012B5374): the vtable the emitter's destructor
//   0x00C37DA0 stores and the landed slots 2 (SetScalar 0x00C37A10) and 5 (SetPosition 0x00C37A40) belong to; slots 1 and 4
//   (0x00C37A00 / 0x00C37A30) are the two byte-identical self-destroy entries. Every slot takes the voice handle first and
//   these two never read it. The callee is the landed CInstanceRegistry::DestroyInstance 0x00C2BAF0, whose CInstance is this
//   emitter (its destructor is 0x00C37DA0), hence the cast. libpk has no PDB/RTTI: the class keeps the landed sibling
//   spelling, the method names are descriptive: ReleaseA / ReleaseB for the two byte-identical release entries (slots 1 / 4; which
//   engine call reaches which is unproven), as CCodaSystem::CreateHardwareVoiceA / B.
struct CInstance;                                   // the landed DestroyInstance's spelling of this emitter

class CInstanceRegistry {
public:
    void DestroyInstance(CInstance* instance);      // 0x00C2BAF0
};

class CSoundEmitter_00c37ad0 {                      // vtable 0x012B5370
public:
    virtual void* v00();                            // slot 0 (0x00C38320)
    virtual void ReleaseA(long handle);              // slot 1 (0x00C37A00)
    virtual void SetScalar(long handle, float value);                     // slot 2 (0x00C37A10)
    virtual void v03(long handle, long value);      // slot 3 (0x00C37A20, empty)
    virtual void ReleaseB(long handle);                 // slot 4 (0x00C37A30)
    CInstanceRegistry* m_pOwner;                    // +0x04
};

void CSoundEmitter_00c37ad0::ReleaseA(long handle)
{
    m_pOwner->DestroyInstance((CInstance*)this);
}