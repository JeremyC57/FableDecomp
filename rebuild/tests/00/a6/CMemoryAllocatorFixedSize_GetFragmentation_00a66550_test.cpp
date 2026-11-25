// Behaviour test for 0x00A66550 CMemoryAllocatorFixedSize::GetFragmentation() (NF415: linked): links the row object (the method is
// declared in the shared header, not defined here) and checks that it returns exactly 0.0f (retail's pooled constant at 0x0122DEDC).
#include <stdio.h>
#include <string.h>
#include "candidates/fixed_size_allocator_accessors_compiled.h"

int main()
{
    CMemoryAllocatorFixedSize* allocator = 0;          // the method reads no member
    float f = allocator->GetFragmentation();
    unsigned int bits; memcpy(&bits, &f, sizeof(bits));
    if (f != 0.0f || bits != 0u) { printf("FIXED_ALLOCATOR_FRAGMENTATION_TEST FAIL\n"); return 1; }
    printf("FIXED_ALLOCATOR_FRAGMENTATION_TEST PASS\n");
    return 0;
}
