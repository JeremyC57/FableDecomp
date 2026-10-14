// CMovableResourceMemoryPool::MoveResource(CMovableResource* resource, long reason, void* source, void* destination, unsigned long size) -- move a resource's bytes (memmove @ 0x00BFEAE6) and, when the resource object exists, tell it through its virtual slot 1 (reason, source, destination, size)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A62F30, length 64 bytes.
// Leaf: ?MoveResource@CMovableResourceMemoryPool@@MAEXPAXJ00K@Z
// Shape: the three memmove arguments in ebx/esi/edi, the resource read after the call, `ret 14h`.
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int size);

struct CMovableResource {
    virtual void v000();
    virtual void OnMoved(long reason, void* source, void* destination, unsigned long size);   // slot 1 (+0x04)
};

struct CMovableResourceMemoryPool {
    virtual void v000();
    virtual void MoveResource(void* resource, long reason, void* source, void* destination, unsigned long size);
};

void CMovableResourceMemoryPool::MoveResource(void* resource, long reason, void* source, void* destination, unsigned long size)
{
    memmove(destination, source, size);
    if (resource)
        ((CMovableResource*)resource)->OnMoved(reason, source, destination, size);
}