// CSBAgentsFactory_00c1abb0::CreateFromFile(void*) -- EH-framed: when the file name resolves out of line (@ 0x00C1A510, the argument also passed by address so it can be rewritten), a 12-byte CSBAgents is allocated through the global operator new (@ 0x00BFEA1A) and built from the owner and the resolved argument (@ 0x00C32FC0); null otherwise or when the allocation fails
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1ABB0, oracle length 123 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov ecx,[name] ; lea eax,[name] ; push eax ; push ecx ; mov ecx,esi ; call Resolve ; test al,al ; je null ; push 0Ch ; call operator new ; add esp,4 ; mov [esp+4],eax ; test eax,eax ; mov dword ptr [esp+10h],0 ; je null ; mov edx,[name] ; push edx ; push esi ; mov ecx,eax ; call CSBAgents::CSBAgents ; pop esi ; mov ecx,[esp+4] ; mov fs:[0],ecx ; add esp,10h ; ret 4 ; null: mov ecx,[esp+8] ; xor eax,eax ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret 4
// The two failure paths share one epilogue. The argument is re-read after the resolve call
// because its address escaped. The class is named by address.
struct CSBAgentsFactory_00c1abb0;

struct CSBAgents {                         // 12 bytes
    unsigned char _body[12];
    CSBAgents(CSBAgentsFactory_00c1abb0* owner, void* name);   // 0x00C32FC0
};

struct CSBAgentsFactory_00c1abb0 {
    bool Resolve(void* name, void** resolved);   // 0x00C1A510
    CSBAgents* CreateFromFile(void* name);
};

CSBAgents* CSBAgentsFactory_00c1abb0::CreateFromFile(void* name)
{
    if (!Resolve(name, &name))
        return 0;
    return new CSBAgents(this, name);
}
