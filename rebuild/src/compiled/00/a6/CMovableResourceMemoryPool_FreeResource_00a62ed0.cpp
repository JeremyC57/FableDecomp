// CMovableResourceMemoryPool::FreeResource(long) (Ego_r.pdb `?FreeResource@CMovableResourceMemoryPool@@QAEXJ@Z`):
// forwards to the base CHandleBasedMemoryPool::Free (0xA02B30, landed); VC7.1 tail-calls it (`jmp rel32`).
// The pool derives from CHandleBasedMemoryPool at offset 0 (rebuild/include/engine/CMovableResourceMemoryPool.h:
// flattened base at +0x00..+0x467); the shared headers are data-only, so the methods are modelled locally.
// Leaf: ?FreeResource@CMovableResourceMemoryPool@@QAEXJ@Z
class CHandleBasedMemoryPool {
public:
    void Free(long handle);
};

class CMovableResourceMemoryPool : public CHandleBasedMemoryPool {
public:
    void FreeResource(long handle);
};

void CMovableResourceMemoryPool::FreeResource(long handle)
{
    Free(handle);
}
