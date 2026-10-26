// CEngineResourceManager::GetIndexBufferUsage() -- 24 while the index-buffer usage cost of the global resource owner is below 0x44c, else 8 (copy a of two)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B3B030, length 33 bytes.
// Callees: GetIndexBufferUsageCost @ 0x009BEDE0 (on the global @ 0x01436E1C, no arguments). Shape: the alternative constant 8 is hoisted into esi before the call,
// the unsigned `cmp eax,44Ch ; mov eax,18h ; jb` selects; no `this` use (a static).
struct CResourceOwner_01436e1c {
    unsigned long GetIndexBufferUsageCost();       // 0x009BEDE0
};
extern CResourceOwner_01436e1c* g_pResourceOwner_01436e1c;   // 0x01436E1C

class CEngineResourceManager { public:
    static long GetIndexBufferUsage();
};

long CEngineResourceManager::GetIndexBufferUsage()
{
    long usage = 8;                                // the alternative is held in esi across the call
    if (g_pResourceOwner_01436e1c->GetIndexBufferUsageCost() < 0x44c)
        usage = 24;
    return usage;
}