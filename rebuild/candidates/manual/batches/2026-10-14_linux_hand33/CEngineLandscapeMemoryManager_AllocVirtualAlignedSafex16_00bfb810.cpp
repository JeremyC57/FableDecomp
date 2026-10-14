// CEngineLandscapeMemoryManager::AllocVirtualAlignedSafe_x16(long count, unsigned long alignment, long flags) -- AllocVirtualAligned(count * 16, alignment, flags) when the byte count, computed in 64 bits, lies in [0, 0x7FFFFFFF]; null otherwise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BFB810, length 78 bytes (80 with padding).
// Callees: __allmul @ 0x00BFEB50 (the 64-bit product count * 16), CEngineLandscapeMemoryManager::AllocVirtualAligned(long, unsigned long, long) @ 0x00BD9C40
// (this, count << 4, alignment, flags; its result passes straight through). Shape: the count is sign-extended (`cdq`) and multiplied as an __int64 by the
// 16-byte element size, range-checked through an inline predicate whose two tests share one `xor eax,eax` failure tail placed after the call path (the call is the `if` body, the zero the fall-through return); the 32-bit byte count is recomputed
// (`shl esi,4`) for the call. One of three element-size instances (4 @ 0x00BFB860, 0x50 @ 0x00BFB8C0).
struct CEngineLandscapeMemoryManager {
    void* AllocVirtualAligned(long size, unsigned long alignment, long flags);   // 0x00BD9C40
    void* AllocVirtualAlignedSafe_x16(long count, unsigned long alignment, long flags);
};

static inline bool IsSafeAllocationSize(__int64 size)
{
    return size >= 0 && size <= 0x7FFFFFFF;
}

void* CEngineLandscapeMemoryManager::AllocVirtualAlignedSafe_x16(long count, unsigned long alignment, long flags)
{
    if (IsSafeAllocationSize((__int64)count * 16))
        return AllocVirtualAligned(count * 16, alignment, flags);
    return 0;
}
