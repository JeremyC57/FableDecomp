// CLandscapeBackgroundTreeNode::~CLandscapeBackgroundTreeNode() -- EH-framed destructor: own vtable (0x012B8AE0), the task at +0x220 deleted through its virtual deleting destructor when present, the polymorphic member at +0x1f0 restores its vtable (0x012B5868), then the out-of-line base destructor (@ 0x00C3F490)
// Leaf: ??1CLandscapeBackgroundTreeNode@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C55130, oracle length 100 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; mov dword ptr [esi],offset vtable ; mov ecx,[esi+220h] ; test ecx,ecx ; mov dword ptr [esp+10h],1 ; je skip ; mov eax,[ecx] ; push 1 ; call [eax] ; skip: mov ecx,esi ; mov dword ptr [esi+1F0h],offset member vtable ; mov dword ptr [esp+10h],-1 ; call ~CBase ; mov ecx,[esp+8] ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// EH state 1 covers the body (the polymorphic member at +0x1f0, with an inline empty destructor,
// and the base are alive); the member's vtable store then precedes the base destructor. The
// manifest label (`ProcessGPUBuildTask`) names the inlined helper; the class is the one whose
// vtable is stored.
struct CBuildTask_00c55130 {
    virtual ~CBuildTask_00c55130();
};

struct CTreeNodeBase_00c55130 {
    virtual void v00();
    ~CTreeNodeBase_00c55130();             // 0x00C3F490
};

struct CTreeNodeSlot_00c55130 {            // +0x1f0: vtable 0x012B5868
    virtual void v00();
    ~CTreeNodeSlot_00c55130() { }
};

struct CLandscapeBackgroundTreeNode : CTreeNodeBase_00c55130 {   // vtable 0x012B8AE0
    unsigned char _pad[0x1ec];
    CTreeNodeSlot_00c55130 m_slot;         // +0x1f0
    unsigned char _pad2[0x2c];
    CBuildTask_00c55130* m_pTask;          // +0x220
    ~CLandscapeBackgroundTreeNode();
};

CLandscapeBackgroundTreeNode::~CLandscapeBackgroundTreeNode()
{
    if (m_pTask)
        delete m_pTask;
}