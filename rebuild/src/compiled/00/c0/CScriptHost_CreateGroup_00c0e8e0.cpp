// CScriptHost::CreateGroup(unsigned long group) -- EH-framed `new CDriverGroup(this, group)` (0x20 bytes through the global operator new @ 0x00BFEA1A, constructor @ 0x00C1E6D0); null when the allocation fails
// Leaf: ?CreateGroup@CScriptHost@@QAEPAUCDriverGroup@@K@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C0E8E0, oracle length 102 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; push 20h ; mov esi,ecx ; call operator new ; add esp,4 ; mov [esp+4],eax ; test eax,eax ; mov dword ptr [esp+10h],0 ; je null ; mov ecx,[argument] ; push ecx ; push esi ; mov ecx,eax ; call CDriverGroup::CDriverGroup ; pop esi ; mov ecx,[esp+4] ; mov fs:[0],ecx ; add esp,10h ; ret 4 ; null: mov ecx,[esp+8] ; xor eax,eax ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret 4
// Naming (sweep1, 2026-10-25): was landed as `CFactory_00c0e8e0::Create` (the manifest's `Alloc` is propagated).
//   FINDINGS chunk 436: its only caller is the landed CBankProcessor::AddDriverToGroup 0x00C2E5A0 (0x00C2E5F6), which
//   creates the group when FindGroup 0x00C0E950 (0x00C2E5E9) finds none, then adds the driver to it.
// Owner: `this` is the libpk object of vtable 0x012B25A8, landed under agent431's descriptive class CScriptHost (QueueEvent
//   0x00C0E9E0, ClearEvents 0x00C0EA10, DeleteAllScripts 0x00C0EAF0, ...; ctor 0x00C0EDF0, dtor 0x00C0ECE0): these factories
//   sit in its translation unit between its slot-5 TryActivate 0x00C0E600 and QueueEvent, and FindGroup reads the
//   registry its destructor tears down at +0x74. The bank processor holds it at +4 (agent436 called it the sound
//   manager / CSoundManager). No PDB covers libpk: descriptive names.
// Shape: a member factory passing the owner and the argument; two epilogues. EH state 0 covers the constructor (the
//   funclet frees the block).
class CScriptHost;

struct CDriverGroup {                           // 0x20 bytes
    unsigned char _body[0x20];
    CDriverGroup(CScriptHost* owner, unsigned long group);   // 0x00C1E6D0
};

class CScriptHost {
public:
    CDriverGroup* CreateGroup(unsigned long group);
};

CDriverGroup* CScriptHost::CreateGroup(unsigned long group)
{
    return new CDriverGroup(this, group);
}
