// PKCDefaultFileManager::CreateFromFile(void*) -- EH-framed: when the file name resolves out of line (@ 0x00C1A510, the argument also passed by address so it can be rewritten), a 12-byte CSBAgents is allocated through the global operator new (@ 0x00BFEA1A) and built from the owner and the resolved argument (@ 0x00C32FC0); null otherwise or when the allocation fails
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1ABB0, oracle length 123 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov ecx,[name] ; lea eax,[name] ; push eax ; push ecx ; mov ecx,esi ; call Resolve ; test al,al ; je null ; push 0Ch ; call operator new ; add esp,4 ; mov [esp+4],eax ; test eax,eax ; mov dword ptr [esp+10h],0 ; je null ; mov edx,[name] ; push edx ; push esi ; mov ecx,eax ; call CSBAgents::CSBAgents ; pop esi ; mov ecx,[esp+4] ; mov fs:[0],ecx ; add esp,10h ; ret 4 ; null: mov ecx,[esp+8] ; xor eax,eax ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret 4
// The two failure paths share one epilogue. The argument is re-read after the resolve call
// because its address escaped.
// Leaf: ?CreateFromFile@PKCDefaultFileManager@@QAEPAUCSBAgents@@PAX@Z
// NF188 (2026-11-20, q5108): the owner is the shared libpk/PKCDefaultFileManager.h class (the header's own note names this factory
//   00C1ABB0; `this` is the manager whose OpenFile 0x00C1A510 it calls: ?OpenFile@PKCDefaultFileManager@@QAE_NPBGPAK@Z, the landed
//   self row's spelling; was the stand-in ?Resolve@CSBAgentsFactory_00c1abb0@@QAE_NPAXPAPAX@Z). The name word is handed to OpenFile as
//   the file name and receives the opened file key. CreateFromFile stays the catalogued descriptive name (libpk has no PDB).
#define FABLE_RETAIL_PKCDEFAULTFILEMANAGER_OPENFILE
#include "libpk/PKCDefaultFileManager.h"

struct CSBAgents {                         // 12 bytes
    unsigned char _body[12];
    CSBAgents(PKCDefaultFileManager* owner, void* name);   // 0x00C32FC0
};

CSBAgents* PKCDefaultFileManager::CreateFromFile(void* name)
{
    if (!OpenFile(static_cast<const unsigned short*>(name), reinterpret_cast<unsigned long*>(&name)))
        return 0;
    return new CSBAgents(this, name);
}