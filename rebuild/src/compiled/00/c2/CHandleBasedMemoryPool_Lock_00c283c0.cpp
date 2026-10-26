// CHandleBasedMemoryPool::Lock(long handle) -- lock the area behind a handle: set its header's lock flag (+0xf) and count it (+0x18) unless it is already locked
// Leaf: ?Lock@CHandleBasedMemoryPool@@QAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C283C0, 32 bytes: mov edx,[esp+4] ; mov eax,[ecx+440h] ;
//   lea eax,[eax+edx*4] ; mov eax,[eax] ; mov dl,[eax+0Fh] ; test dl,dl ; jne done ; mov byte [eax+0Fh],1 ;
//   inc dword [ecx+18h] ; done: ret 4
// Fix (sweep1, 2026-10-25): was landed as a fake `void __fastcall Lock(CHandleBasedMemoryPool* self, int /*edx*/, long
//   index)` global stand-in (FINDINGS chunk 435: dummy edx); retail is the plain thiscall member (ecx = this, the handle
//   on the stack, `ret 4`), the mirror of the landed CHandleBasedMemoryPool::Unlock 0x00C283E0. This libpk build of the
//   pool keeps the handle table as a Dinkumware vector<CHeader*> at +0x43C (_Myfirst +0x440) and the lock count at +0x18
//   (landed MoveArea 0x00C28580 layout notes), so it is modelled locally: engine/CHandleBasedMemoryPool.h carries the
//   engine (Ego_r) layout (header debt, see NOTES.md).
// Shape: the slot address is formed first (`lea eax,[eax+edx*4]`) and dereferenced once; a plain `m_handles[handle]`
// folds the index into the load.
struct CHeader {                                // CHandleBasedMemoryPool::CHeader (32 bytes in front of every area)
    unsigned char _pad00[0xf];
    unsigned char Locked;                       // +0x0f
};

class CHandleBasedMemoryPool { public:
    unsigned char _pad00[0x18];
    long          LockCount;                    // +0x18
    unsigned char _pad1c[0x440 - 0x1c];
    CHeader**     Handles;                      // +0x440 (the handle vector's _Myfirst)
    void Lock(long handle);
};

void CHandleBasedMemoryPool::Lock(long handle)
{
    CHeader** slot = &Handles[handle];
    if ((*slot)->Locked == 0) {
        (*slot)->Locked = 1;
        LockCount++;
    }
}
