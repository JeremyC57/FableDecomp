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