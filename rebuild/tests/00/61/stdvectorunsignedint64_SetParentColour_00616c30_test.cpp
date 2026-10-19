#include <cstdio>
#ifndef _MSC_VER
#define __int64 long long
#endif
// CRetailVector_Unsigned64_00616c30::clear() -- erase(begin(), end()) on a native vector<unsigned __int64>
// Retail 0x00616C30, 41 bytes. The propagated CComponent::SetParentColour label is false. Caller evidence places this vector in the CPersistContext transfer dispatch at 0x006192F0, which
// dispatches context mode +0x18. The binary branches serialize vectors<unsigned __int64> at
// 0x00612D30/0x00619130 (eight-byte elements). 0x00606492/0x006068BA transfer ActiveTeleportUID. The native
// clear uses STLport's trivial byte-copy helper on [last,last), so the element view is uint64 but CopyTrivial
// computes byte distance through char pointers. The vector header is the native {first,last,capacity} triple.
extern "C" void* memmove(void* dest, const void* src, unsigned int count); // 0x00BFEAE6
static inline void* CopyTrivial(const void* first, const void* last, void* result) {
    return (last == first) ? result
        : ((char*)memmove(result, first, (const char*)last - (const char*)first))
            + ((const char*)last - (const char*)first);
}
typedef unsigned __int64 NativeUInt64;
struct CRetailVector_Unsigned64_00616c30 {
    NativeUInt64* first;
    NativeUInt64* last;
    NativeUInt64* end_of_storage;
    void clear();
};
void CRetailVector_Unsigned64_00616c30::clear() {
    NativeUInt64* newLast = (NativeUInt64*)CopyTrivial(last, last, first);
    last = newLast;
}

typedef char NativeUInt64IsEight[sizeof(NativeUInt64)==8?1:-1];
 #ifdef _MSC_VER
 typedef char NativeVectorIsTwelve[sizeof(CRetailVector_Unsigned64_00616c30)==12?1:-1];
 #endif
 static int g_memmoveCalls;
extern "C" void* memmove(void* destination, const void* source, unsigned int count)
{
    unsigned char* dst = (unsigned char*)destination;
    const unsigned char* src = (const unsigned char*)source;
    unsigned int i;
    ++g_memmoveCalls;
    for (i = 0; i < count; ++i)
        dst[i] = src[i];
    return destination;
}
int main()
{
    NativeUInt64 values[4] = { 11, 22, 33, 44 };
    NativeUInt64 copied[4] = { 0, 0, 0, 0 };
    CRetailVector_Unsigned64_00616c30 nonempty;
    nonempty.first = values;
    nonempty.last = values + 3;
    nonempty.end_of_storage = values + 4;
    nonempty.clear();
    if (nonempty.last != nonempty.first || nonempty.end_of_storage != values + 4 ||
        g_memmoveCalls != 0 || values[0] != 11 || values[3] != 44) {
        std::printf("00616c30_TEST FAIL\n");
        return 1;
    }
    CRetailVector_Unsigned64_00616c30 empty;
    empty.first = values;
    empty.last = values;
    empty.end_of_storage = values + 4;
    empty.clear();
    if (empty.first != values || empty.last != values || empty.end_of_storage != values + 4 || g_memmoveCalls != 0) {
        std::printf("00616c30_TEST FAIL\n");
        return 1;
    }
    CRetailVector_Unsigned64_00616c30 nullVector;
    nullVector.first = 0;
    nullVector.last = 0;
    nullVector.end_of_storage = 0;
    nullVector.clear();
    if (nullVector.first || nullVector.last || nullVector.end_of_storage || g_memmoveCalls != 0) {
        std::printf("00616c30_TEST FAIL\n");
        return 1;
    }
    if (CopyTrivial(values, values + 3, copied) != (char*)copied + sizeof(NativeUInt64) * 3 ||
        g_memmoveCalls != 1 || copied[0] != 11 || copied[1] != 22 || copied[2] != 33) {
        std::printf("00616c30_TEST FAIL\n");
        return 1;
    }
    std::printf("00616c30_TEST PASS\n");
    return 0;
}