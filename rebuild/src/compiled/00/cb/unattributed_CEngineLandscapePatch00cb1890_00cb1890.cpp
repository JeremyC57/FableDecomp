// CEngineLandscapePatch_00cb1890::~CEngineLandscapePatch_00cb1890() -- EH-framed destructor: an out-of-line release (@ 0x00CB1340), then the node list at +0x14 freed inline node by node through operator delete
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB1890, oracle length 98 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; mov dword ptr [esp+10h],0 ; call Release ; mov esi,[esi+14h] ; test esi,esi ; mov dword ptr [esp+10h],-1 ; je done ; loop: mov eax,esi ; mov esi,[esi+4] ; push eax ; call operator delete ; add esp,4 ; test esi,esi ; jne loop ; done: mov ecx,[esp+8] ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// The body runs under EH state 0 (the list member is alive); the member's inline destructor then
// walks and frees the nodes (state -1). The class stores no vtable. The manifest label names the
// inlined helper; the class is named by address.
struct SPatchNode_00cb1890 {
    void* m_pData;
    SPatchNode_00cb1890* m_pNext;          // +4
};

struct CPatchNodeList_00cb1890 {           // +0x14
    SPatchNode_00cb1890* m_pHead;
    ~CPatchNodeList_00cb1890()
    {
        for (SPatchNode_00cb1890* node = m_pHead; node; ) {
            SPatchNode_00cb1890* current = node;
            node = node->m_pNext;
            ::operator delete(current);
        }
    }
};

struct CEngineLandscapePatch_00cb1890 {
    unsigned char _pad[0x14];
    CPatchNodeList_00cb1890 m_nodes;       // +0x14
    void Release();                        // 0x00CB1340
    ~CEngineLandscapePatch_00cb1890();
};

CEngineLandscapePatch_00cb1890::~CEngineLandscapePatch_00cb1890()
{
    Release();
}