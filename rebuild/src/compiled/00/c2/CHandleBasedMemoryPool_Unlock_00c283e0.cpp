// CHandleBasedMemoryPool_012b4098::Unlock  (0x00c283e0)
typedef unsigned char uchar;

struct PoolEntry {
    char pad[0xf];
    uchar locked;   // +0xf
};

class CHandleBasedMemoryPool_012b4098 { public:
    char pad0[0x18];
    int lockCount;          // +0x18
    char pad1[0x440 - 0x1c];
    PoolEntry** entries;    // +0x440

    void Unlock(long handle);
};

void CHandleBasedMemoryPool_012b4098::Unlock(long handle)
{
    PoolEntry** slot = &this->entries[handle];
    if ((*slot)->locked != 0) {
        (*slot)->locked = 0;
        this->lockCount--;
    }
}