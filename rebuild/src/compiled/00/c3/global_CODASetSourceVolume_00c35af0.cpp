// CODA_SetSourceVolume(CAudioObject* source, float volume) -- the libpk `__stdcall` API wrapper that sets a CODA source's linear volume through CAudioObject::SetGain (0x00C42F50)
// Leaf: _CODA_SetSourceVolume@8
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C35AF0, 17 bytes: mov eax,[esp+8] ; mov ecx,[esp+4] ; push eax ;
//   call 0x00C42F50 ; ret 8
// Naming (sweep1, 2026-10-25): was landed as an `extern "C"` __stdcall New_c35af0(C* a, void* b) stand-in (catalog
//   CEngineSubPrimitiveBase::operator_new, manifest operator_new: propagated); descriptive (no PDB covers the libpk /
//   GameCODA code). Its callee 0x00C42F50 is the landed CAudioObject::SetGain(float) (linear gain at +0x18, its
//   millibel volume at +0x1c), whose source names this wrapper as its only caller. Of its five callers, the landed
//   CCodaPlayback rows SetPosition 0x00C27550 / SetVolume 0x00C27620 / Update 0x00C276B0 declare it
//   `void __stdcall CODA_SetSourceVolume(CODASource* source, float volume)` and the landed 0x00C49420 declares it
//   `CodaSource_SetDecibels(void*, float)` (FINDINGS chunk 435: reconcile); the value is a linear gain, not decibels,
//   so the CODA_SetSourceVolume name is kept. It is part of the `extern "C"` CODA API: the agent435 callers declare it
//   so, and under their /EHsc the scoped lock around the call needs no EH frame only because an extern "C" callee is
//   assumed not to throw. The parameter is typed with the callee's landed class (agent435's CODASource is the same
//   object).
class CAudioObject {
public:
    void SetGain(float gain);                   // 0x00C42F50
};

extern "C" void __stdcall CODA_SetSourceVolume(CAudioObject* source, float volume)
{
    source->SetGain(volume);
}
