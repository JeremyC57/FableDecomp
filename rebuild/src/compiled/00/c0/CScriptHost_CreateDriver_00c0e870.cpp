// CScriptHost::CreateDriver(CSample* sample, const CSoundPlayParameters& params) -- EH-framed `new CDriver(sample, params)` (0x1c bytes through the global operator new @ 0x00BFEA1A, constructor @ 0x00C1E5A0); null when the allocation fails
// Leaf: ?CreateDriver@CScriptHost@@QAEPAUCDriver@@PAUCSample@@ABUCSoundPlayParameters@@@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C0E870, oracle length 100 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push 1Ch ; call operator new ; add esp,4 ; mov [esp],eax ; test eax,eax ; mov dword ptr [esp+0Ch],0 ; je null ; mov ecx,[b] ; mov edx,[a] ; push ecx ; push edx ; mov ecx,eax ; call CDriver::CDriver ; mov ecx,[esp+4] ; mov fs:[0],ecx ; add esp,10h ; ret 8 ; null: mov ecx,[esp+4] ; xor eax,eax ; mov fs:[0],ecx ; add esp,10h ; ret 8
// Naming (sweep1, 2026-10-25): was landed as `CFactory_00c0e870::Create` (the manifest's `Alloc` is propagated).
//   Its caller CBankProcessor::AddDriver 0x00C2E2B0 (0x00C2E459) passes the sample found in the bank's sample map and
//   the CSoundPlayParameters block it filled, then adds the result to the driver map; the other callers are
//   0x00C1647A and 0x00C269C5.
// Owner: `this` is the libpk object of vtable 0x012B25A8, landed under agent431's descriptive class CScriptHost (QueueEvent
//   0x00C0E9E0, ClearEvents 0x00C0EA10, DeleteAllScripts 0x00C0EAF0, ...; ctor 0x00C0EDF0, dtor 0x00C0ECE0): these factories
//   sit in its translation unit between its slot-5 TryActivate 0x00C0E600 and QueueEvent, and FindGroup reads the
//   registry its destructor tears down at +0x74. The bank processor holds it at +4 (agent436 called it the sound
//   manager / CSoundManager). No PDB covers libpk: descriptive names.
// Shape: `this` is not read; the block is kept in the frame because the constructor may throw (EH state 0; the
//   funclet frees it through operator delete); the null path has its own epilogue.
struct CSample;
struct CSoundPlayParameters;

struct CDriver {                                // 0x1c bytes
    unsigned char _body[0x1c];
    CDriver(CSample* sample, const CSoundPlayParameters& params);   // 0x00C1E5A0
};

class CScriptHost {
public:
    CDriver* CreateDriver(CSample* sample, const CSoundPlayParameters& params);
};

CDriver* CScriptHost::CreateDriver(CSample* sample, const CSoundPlayParameters& params)
{
    return new CDriver(sample, params);
}
