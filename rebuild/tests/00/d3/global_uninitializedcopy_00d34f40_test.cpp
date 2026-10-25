// Behaviour test for the trivial __uninitialized_copy (0x00D34F40): memmove [first, last) to result and return
// result + count; an empty range returns result untouched.
#include <cstdio>
#include <cstring>

void* CopyTrivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result :
        ((char*)std::memmove(result, first, ((const char*)last - (const char*)first))) +
        ((const char*)last - (const char*)first);
}
bool* UninitializedCopy(const bool* first, const bool* last, bool* result)
{
    return (bool*)CopyTrivial(first, last, result);
}

int main()
{
    bool src[4] = { true, false, false, true };
    bool dst[6] = { false, true, true, false, true, true };
    bool* end = UninitializedCopy(src, src + 4, dst + 1);
    bool* same = UninitializedCopy(src, src, dst);
    bool overlap[5] = { true, false, true, false, false };
    bool* end2 = UninitializedCopy(overlap, overlap + 3, overlap + 2);   // overlapping move
    if (end != dst + 5 || same != dst || dst[0] != false || dst[1] != true || dst[2] != false || dst[3] != false
        || dst[4] != true || dst[5] != true || end2 != overlap + 5 || overlap[2] != true || overlap[3] != false || overlap[4] != true) {
        std::printf("00d34f40_TEST FAIL\n");
        return 1;
    }
    std::printf("00d34f40_TEST PASS\n");
    return 0;
}