// CScriptHost::CreateSample(const CAudioFormat* format, unsigned long dataOffset) -- EH-framed `new CSample(this, dataOffset, format)` (0x24 bytes through the global operator new @ 0x00BFEA1A, constructor @ 0x00C1D880); null when the allocation fails
// Leaf: ?CreateSample@CScriptHost@@QAEPAUCSample@@PBUCAudioFormat@@K@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C0E700, oracle length 107 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; push 24h ; mov esi,ecx ; call operator new ; add esp,4 ; mov [esp+4],eax ; test eax,eax ; mov dword ptr [esp+10h],0 ; je null ; mov ecx,[a] ; mov edx,[b] ; push ecx ; push edx ; push esi ; mov ecx,eax ; call CSample::CSample ; pop esi ; mov ecx,[esp+4] ; mov fs:[0],ecx ; add esp,10h ; ret 8 ; null: mov ecx,[esp+8] ; xor eax,eax ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret 8
// Naming (sweep1, 2026-10-25): was landed as `CFactory_00c0e700::Create` (the manifest's `Alloc` is propagated).
//   FINDINGS chunk 436: its only caller is the landed CBankProcessor::AddSample 0x00C2E190 (0x00C2E24D), which passes
//   the sample's CAudioFormat and data offset and adds the result to the bank's sample map.
// Owner: `this` is the libpk object of vtable 0x012B25A8, landed under agent431's descriptive class CScriptHost (QueueEvent
//   0x00C0E9E0, ClearEvents 0x00C0EA10, DeleteAllScripts 0x00C0EAF0, ...; ctor 0x00C0EDF0, dtor 0x00C0ECE0): these factories
//   sit in its translation unit between its slot-5 TryActivate 0x00C0E600 and QueueEvent, and FindGroup reads the
//   registry its destructor tears down at +0x74. The bank processor holds it at +4 (agent436 called it the sound
//   manager / CSoundManager). No PDB covers libpk: descriptive names.
// Shape: a member factory passing the owner and the two arguments (the second before the first); two epilogues. EH
//   state 0 covers the constructor.
struct CAudioFormat;
class CScriptHost;

struct CSample {                                // 0x24 bytes
    unsigned char _body[0x24];
    CSample(CScriptHost* owner, unsigned long dataOffset, const CAudioFormat* format);   // 0x00C1D880
};

class CScriptHost {
public:
    CSample* CreateSample(const CAudioFormat* format, unsigned long dataOffset);
};

CSample* CScriptHost::CreateSample(const CAudioFormat* format, unsigned long dataOffset)
{
    return new CSample(this, dataOffset, format);
}
