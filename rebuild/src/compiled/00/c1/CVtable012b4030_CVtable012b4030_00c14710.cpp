// CVtable_012b4030::~CVtable_012b4030() -- implicit destructor of the sound-event manager's +0x7B4 worker part: destroys its +0xC worker queue
// Leaf: ??1CVtable_012b4030@@QAE@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C14710, 8 bytes: `add ecx,0Ch ; jmp 0x00C14560` (thiscall tail call, no vptr store).
// Identity: the class is the landed CVtable_012b4030 (vtable 0x012B4030, one slot PostRequest 0x00C26EC0; constructor 0x00C26E90
//   ??0CVtable_012b4030@@QAE@PAX@Z, bound by the landed CSoundEventManager::CSoundEventManager 0x00C11D00, which stores it at
//   +0x7B4). CSoundEventManager::~CSoundEventManager 0x00C13470 deletes that +0x7B4 part with this destructor inlined (`lea ecx,[edi+0Ch] ;
//   call 0x00C14560`); the queue destructor is the landed PKCWorkerQueue_00c14560::~PKCWorkerQueue_00c14560. The class keeps its landed
//   address-derived spelling (one class, one name; libpk has no PDB or RTTI); its only other caller is the unreferenced scalar deleting
//   destructor 0x00C14740, landed under the propagated CLandscapeBackgroundPatch label (NAME_FIXES).
// Shape: a compiler-generated destructor: a user-written `~CVtable_012b4030() {}` stores the vptr first (`mov [ecx],12B4030h`), retail
//   does not. EmitCVtable012b4030Destructor below only makes the translation unit emit the implicit ??1; it is not a retail function.
struct PKCRequest;

struct PKCWorkerQueue_00c14560 {                    // 0x30-byte libpk worker queue
    unsigned char _body[0x30];
    ~PKCWorkerQueue_00c14560();                     // 0x00C14560
};

class CVtable_012b4030 {                        // vtable 0x012B4030
public:
    virtual void PostRequest(unsigned char flag, PKCRequest* request);   // slot 0 (0x00C26EC0)
    void*                   m_link[2];          // +0x04 the inner link {vptr, owner}
    PKCWorkerQueue_00c14560 m_worker;           // +0x0c
    void*                   m_pOwner;           // +0x3c
};

void EmitCVtable012b4030Destructor(CVtable_012b4030* part)   // emits the implicit ??1; not a retail function
{
    delete part;
}