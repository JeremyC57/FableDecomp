// CAmbientTrigger::UpdateVolume(CAmbientSource* source) -- when the trigger's sound is playing (handle not negative), set its volume to the source's current volume through the handle table (slot 25)
// Leaf: ?UpdateVolume@CAmbientTrigger@@QAEXPAVCAmbientSource@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C624C0, 54 bytes, thiscall, `ret 4`.
// Identity: libpk ambient sound (no PDB/RTTI: descriptive names). `this` is the 0x18-byte CAmbientTrigger of the landed
//   CAmbientSource::ProcessTriggers 0x00C46F90 (handle +0x08, offset vector +0x0C); its siblings PlaceAt 0x00C62460 and
//   RemoveHandle 0x00C62500 (landed on per-row placeholder classes) test the same handle and fetch the same table.
//   Only caller: the per-trigger forwarder 0x00C46F80 (`trigger->X(m_pSource)`, the sibling forwarders 0x00C46F60 /
//   0x00C46F70 call RemoveHandle-like 0x00C621B0 and PlaceAt). Label correction: the manifest row
//   "CConsoleCommandLine::DecrementCursorPosition" is propagated.
// Callees: CAmbientTrigger::GetHandleTable 0x00C62440 (thiscall, ignores `this`: source -> 0x00C623F0 -> +4 -> +0x38;
//   landed siblings' method name), CAmbientSource::GetVolume 0x00C46F40 (definition +4 GetValue 0x00C38510 times the
//   owner set's +0x10 scale), the table's slot 25 (+0x64, (handle, float)).
// Shape: the table is fetched into a named local first (its vptr hoisted into ebp), then the float argument is computed
//   (`push ecx ; fstp [esp]`) and the handle re-read and pushed.
class CAmbientSource {
public:
    float GetVolume();                                  // 0x00C46F40
};
class CHandleTable {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void SetVolume(long handle, float volume);  // slot 25 (+0x64)
};
class CAmbientTrigger {                                 // 0x18 bytes
public:
    unsigned long m_soundId;                            // +0x00
    unsigned long m_time;                               // +0x04 due time
    long          m_handle;                             // +0x08 (-1: not playing)
    float         m_offset[3];                          // +0x0c
    CHandleTable* GetHandleTable(CAmbientSource* source);   // 0x00C62440
    void UpdateVolume(CAmbientSource* source);
};

void CAmbientTrigger::UpdateVolume(CAmbientSource* source)
{
    if (m_handle >= 0) {
        CHandleTable* table = GetHandleTable(source);
        table->SetVolume(m_handle, source->GetVolume());
    }
}