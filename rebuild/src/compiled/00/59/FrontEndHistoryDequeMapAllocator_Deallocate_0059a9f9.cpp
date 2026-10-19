// Front-end history deque map-slot deallocator @ 0x0059A9F9.
//
// This is the ownership companion to Allocate at 0x0059A9E0.  It frees the
// map allocation itself; the number of slots is part of the allocator
// interface but is not needed by the retail heap.

extern "C" void __cdecl free(void* block);   // MSVCR71 import thunk 0x00BFEA14

class FrontEndHistoryDequeMapAllocator_0059a9f9
{
public:
    void Deallocate(void** allocation, unsigned long count);
};

#pragma optimize("s", on)
void FrontEndHistoryDequeMapAllocator_0059a9f9::Deallocate(
    void** allocation,
    unsigned long)
{
    if (allocation != 0)
    {
        free(allocation);
    }
}
#pragma optimize("s", off)
