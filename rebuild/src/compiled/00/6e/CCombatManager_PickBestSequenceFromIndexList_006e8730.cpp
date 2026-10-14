// CCombatManager::PickBestSequenceFromIndexList(const std::vector<long>&) -- a world-random pick: index = (rand % (count * 100)) / 100
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E8730, length 85 bytes (+ int3 padding).
// Callees: CWorld::GetPRandomSeed @ 0x0049D810 (`[world+8] + 1613Ch`; the manifest's "CVertexBufferWin32::DoSizeof"
// is a propagated label) on the world at +0x20 (Ego_r.pdb has World at +0x28 in its build; local model), GFROR13
// @ 0x00497890 (__fastcall; one inlined step of the world random generator, as in the landed 0x0092C440).
// Shape: the world is loaded first (`mov ecx,[ecx+20h]`), the count (`(last-first) >> 2`) is formed in esi and
// scaled by 100 after the seed call; the range helper is `range ? seed % range : 0` (`xor edx,edx ; test ;
// je ; div`), then the signed /100 (`imul 51EB851Fh ; sar 5 ; shr 1Fh ; add`) indexes the vector.
extern unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890

inline unsigned long WorldRandomRange(unsigned long& seed, unsigned long range)
{
    seed = seed * 0x24A1 + 0x24DF;
    seed = GFROR13(seed);
    if (range == 0)
        return 0;
    return seed % range;
}

struct CLongVector {                            // std::vector<long>
    long* m_pFirst;                             // +0x00
    long* m_pLast;                              // +0x04
    long* m_pEnd;                               // +0x08
    long size() const { return m_pLast - m_pFirst; }
    long operator[](long index) const { return m_pFirst[index]; }
};

struct CWorld_Calls {              // engine/CWorld.h exists but is not used: only its GetPRandomSeed call is used
    unsigned long* GetPRandomSeed();            // 0x0049D810
};

struct CCombatManager {
    unsigned char _pad[0x20];
    CWorld_Calls* World;                              // +0x20
    long PickBestSequenceFromIndexList(const CLongVector& indices);
};

long CCombatManager::PickBestSequenceFromIndexList(const CLongVector& indices)
{
    long count = indices.size();
    unsigned long* seed = World->GetPRandomSeed();      // named: the seed call precedes `imul esi,100`
    long pick = WorldRandomRange(*seed, count * 100);
    return indices[pick / 100];
}