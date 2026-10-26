// CEngineLandscapeMemoryManager::AllocVirtualSafetyChecked(long size, long flags) -- AllocVirtual(size, flags) when the size, widened to 64 bits, lies in [0, 0x7FFFFFFF]; null otherwise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BFB780, length 52 bytes.
// Callees: CEngineLandscapeMemoryManager::AllocVirtual(long, long) @ 0x00BD9AF0 (this, size, flags; its result passes straight through). Shape: the size is
// sign-extended (`cdq`) and range-checked as an __int64 through an inline predicate, whose two tests share one `xor eax,eax` failure tail placed after the call path (the call is the `if` body, the zero the fall-through return); the size rides in esi.
class CEngineLandscapeMemoryManager { public:
    void* AllocVirtual(long size, long flags);     // 0x00BD9AF0
    void* AllocVirtualSafetyChecked(long size, long flags);
};

static inline bool IsSafeAllocationSize(__int64 size)
{
    return size >= 0 && size <= 0x7FFFFFFF;
}

void* CEngineLandscapeMemoryManager::AllocVirtualSafetyChecked(long size, long flags)
{
    if (IsSafeAllocationSize((__int64)size))
        return AllocVirtual(size, flags);
    return 0;
}