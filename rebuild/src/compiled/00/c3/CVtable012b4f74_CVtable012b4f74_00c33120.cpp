// CVtable_012b4f74::~CVtable_012b4f74() -- EH-framed destructor: stores the class's vtable (0x012B4F74), closes out of
// line (slot 1 @ 0x00C3C450), then the base destructor ~CVtable_012b5860 (out of line @ 0x00C3C5C0)
// Leaf: ??1CVtable_012b4f74@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C33120, oracle length 79 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; mov dword ptr [esi],offset vtable ; mov dword ptr [esp+10h],0 ; call Close ; mov ecx,esi ; mov dword ptr [esp+10h],-1 ; call ~base ; mov ecx,[esp+8] ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// Identity: the vtable it stores, 0x012B4F74, has no RTTI; its slot 0 is the scalar deleting destructor 0x00C33170
// (`call 0x00C33120`), and the landed constructor 0x00C3C3B0 stores the same vtable, so this is that class's destructor
// under the class's established descriptive name CVtable_012b4f74 (catalog: ctor 0x00C3C3B0, slot0 0x00C33170). It is not
// CScriptDefinitionManager: that class's constructor 0x00CD3F00 stores 0x012C2648 (manifest label propagated here).
// Base: ~CVtable_012b5860 @ 0x00C3C5C0 stores vtable 0x012B5860 (the base the constructor 0x00C3C3B0 builds through
// 0x00C3C590) and is virtual (0x012B5860 slot 0 = its scalar deleting destructor 0x00C3C600, `call 0x00C3C5C0`).
// Close is slot 1 of 0x012B4F74 (0x00C3C450), called directly as in any destructor.

struct CVtable_012b5860 {
    virtual ~CVtable_012b5860();                       // 0x00C3C5C0
};

class CVtable_012b4f74 : public CVtable_012b5860 {
public:
    virtual ~CVtable_012b4f74();
    virtual void Close();                              // 0x00C3C450
};

CVtable_012b4f74::~CVtable_012b4f74()
{
    CVtable_012b4f74::Close();
}
