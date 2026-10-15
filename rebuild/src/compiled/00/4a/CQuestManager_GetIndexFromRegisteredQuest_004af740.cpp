// CQuestManager::GetIndexFromRegisteredQuest(const CCharString& name) -- index of `name` in the registered quest names (the vector<CCharString> at retail +0x2C), or -1
// Leaf: GetIndexFromRegisteredQuest
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AF740, length 89 bytes (+ int3 padding).
// Identity: the catalog label "CPhysicsMesh::GetNoDummyObjects" is propagated (CPhysicsMesh's dummy
// objects are 56-byte records at +0x20, see 0x00A6D090). This body sits among the CQuestManager
// functions (0x004AF290..0x004AF9C0) and indexes the same vector as GetRegisteredQuestFromIndex
// (0x004AF3C0: RegisteredQuestNames at retail +0x2C, PDB +0x34 with 16-byte vectors); Ego_r.pdb has
// `?GetIndexFromRegisteredQuest@CQuestManager@@QBEJABVCCharString@@@Z` right after
// GetRegisteredQuestFromIndex.
// Callees: Compare(const char*, const char*) @ 0x00411570 (`__fastcall`), reached through the
// inlined CCharString equality (same data block, both blocks present with equal lengths, then
// Compare == 0 as `neg ; sbb al,al ; inc al`).
// Shape: the running index starts at -1 (`or ebp,-1`) and is bumped at the top of every
// iteration, so the found exit returns it directly while the fall-through returns a fresh -1.
// The vector's end pointer is cached in ebx; the argument's data block is read ONCE, after the
// entry test, into esi: the source is the rotated loop (entry test, then a do/while) with the key
// copied into a local inside the non-empty arm (a plain `for` re-reads `name` every iteration, and
// a local declared before the loop is loaded ahead of the entry test). CCharString is modelled as
// its single data-block pointer, so the local copy is that pointer (no add-ref in this body).
// Retail-offset view of CQuestManager: engine/CQuestManager.h carries the PDB layout (RegisteredQuestNames
// at +0x34, 16-byte vectors), which disagrees with the retail bytes (+0x2C), so the model is local.
struct CCharStringData {
    const char*   m_pChars;                        // +0x00
    unsigned long m_length;                        // +0x04
};
long __fastcall Compare_00411570(const char* a, const char* b);   // 0x00411570
struct CCharString {
    CCharStringData* m_pData;
};
static __forceinline bool operator==(const CCharString& sa, const CCharString& sb)
{
    CCharStringData* a = sa.m_pData;
    CCharStringData* b = sb.m_pData;
    if (a == b)
        return true;
    if (!a || !b)
        return false;
    if (a->m_length != b->m_length)
        return false;
    return Compare_00411570(a->m_pChars, b->m_pChars) == 0;
}
struct CCharStringVector {                         // retail std::vector: first/last/end
    CCharString* first;
    CCharString* last;
    CCharString* end_of_storage;
};

struct CQuestManager {
    unsigned char     _pad00[0x2c];
    CCharStringVector RegisteredQuestNames;        // +0x2c
    long GetIndexFromRegisteredQuest(const CCharString& name) const;
};

long CQuestManager::GetIndexFromRegisteredQuest(const CCharString& name) const
{
    CCharString* last = RegisteredQuestNames.last;
    long index = -1;
    CCharString* it = RegisteredQuestNames.first;
    if (it != last) {
        CCharString key = name;
        do {
            ++index;
            if (*it == key)
                return index;
            ++it;
        } while (it != last);
    }
    return -1;
}