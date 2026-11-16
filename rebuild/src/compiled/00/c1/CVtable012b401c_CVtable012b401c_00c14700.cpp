// CVtable_012b401c::~CVtable_012b401c() -- implicit destructor of the sound-event manager's +0x7B8 worker part: destroys its +0xC worker queue
// Leaf: ??1CVtable_012b401c@@QAE@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C14700, 8 bytes: `add ecx,0Ch ; jmp 0x00C14630` (thiscall tail call, no vptr store).
// Identity: the class is the landed CVtable_012b401c (vtable 0x012B401C, one slot PostRequest 0x00C26A60; constructor 0x00C26A30
//   ??0CVtable_012b401c@@QAE@PAX@Z, bound by the landed CSoundEventManager::CSoundEventManager 0x00C11D00, which stores it at
//   +0x7B8). CSoundEventManager::~CSoundEventManager 0x00C13470 deletes that +0x7B8 part with this destructor inlined (`lea ecx,[edi+0Ch] ;
//   call 0x00C14630`); the queue destructor is the landed PKCWorkerQueue_00c14630::~PKCWorkerQueue_00c14630. The class keeps its landed
//   address-derived spelling (one class, one name; libpk has no PDB or RTTI); its only other caller is the unreferenced scalar deleting
//   destructor 0x00C14720, landed under the propagated CLandscapeBackgroundPatch label (NAME_FIXES).
// Shape: a compiler-generated destructor: a user-written `~CVtable_012b401c() {}` stores the vptr first (`mov [ecx],12B401Ch`), retail
//   does not. EmitCVtable012b401cDestructor below only makes the translation unit emit the implicit ??1; it is not a retail function.
struct PKCRequest;

struct PKCWorkerQueue_00c14630 {                    // 0x30-byte libpk worker queue
    unsigned char _body[0x30];
    ~PKCWorkerQueue_00c14630();                     // 0x00C14630
};

class CVtable_012b401c {                        // vtable 0x012B401C
public:
    virtual void PostRequest(unsigned char flag, PKCRequest* request);   // slot 0 (0x00C26A60)
    void*                   m_link[2];          // +0x04 the inner link {vptr, owner}
    PKCWorkerQueue_00c14630 m_worker;           // +0x0c
    void*                   m_pOwner;           // +0x3c
};

void EmitCVtable012b401cDestructor(CVtable_012b401c* part)   // emits the implicit ??1; not a retail function
{
    delete part;
}