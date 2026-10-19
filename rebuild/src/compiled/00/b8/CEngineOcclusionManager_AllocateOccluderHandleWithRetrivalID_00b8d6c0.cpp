// CEngineOcclusionManager::AllocateOccluderHandleWithRetrivalID(unsigned long retrievalID) -- take the next occluder entry (cleared) while fewer than 8192 are in use, else handle -1, and record retrievalID -> handle in the sorted handle map (overwriting an existing mapping)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B8D6C0, length 192 bytes (ret 4).
// Callees: the handle map's insert(iterator, const pair<unsigned long, unsigned long>&) 0x00B8F650 (STLport vector insert;
// manifest "Vector_InsertElement_WithReallocation").
// Layout (retail = PDB less 8 after the two narrower CArrays): Entries +0x4014 (8-byte COccluderEntry: PixelCount, the
// SlotID/Testing/Completed bit word), EntriesSize +0x401C; HandleMapper +0x2403C (sorted vector<pair<ulong, ulong> >).
// Shape: the handle is a register variable spilled across the search; the lower_bound is the inlined STLport loop over a
// signed distance (`sar`), `count -= half + 1` as `or ebp,-1 ; sub ebp,edx ; add eax,ebp`; the map's address is computed
// once (ecx) for the insert; the pair temporary is built in the frame. The new entry is cleared by copying a zeroed
// local COccluderEntry (its two fields get separate zero registers, edx and esi) at the size read once into ecx.
struct COccluderEntry {
    unsigned long PixelCount;                   // +0x00
    unsigned long Bits;                         // +0x04 SlotID:30, Testing:1, Completed:1
};

struct SHandlePair {                            // std::pair<unsigned long, unsigned long>
    unsigned long first;
    unsigned long second;
    SHandlePair(unsigned long a, unsigned long b) : first(a), second(b) { }
};

struct CHandleMapper {                          // STLport vector<pair<unsigned long, unsigned long> >
    SHandlePair* _M_start;
    SHandlePair* _M_finish;
    SHandlePair* _M_end_of_storage;
    SHandlePair* insert(SHandlePair* position, const SHandlePair& value);   // 0x00B8F650
};

static inline SHandlePair* LowerBound(SHandlePair* first, SHandlePair* last, unsigned long key)
{
    long count = last - first;
    while (count > 0) {
        long half = count >> 1;
        SHandlePair* middle = first + half;
        if (middle->first < key) {
            first = middle + 1;
            count -= half + 1;
        }
        else
            count = half;
    }
    return first;
}

class CEngineOcclusionManager {
public:
    unsigned char   _pad0000[0x4014];
    COccluderEntry* Entries;                    // +0x4014
    COccluderEntry* PrevFrameEntries;           // +0x4018
    unsigned long   EntriesSize;                // +0x401c
    unsigned char   _pad4020[0x2403c - 0x4020];
    CHandleMapper   HandleMapper;               // +0x2403c
    unsigned long AllocateOccluderHandleWithRetrivalID(unsigned long retrievalID);
};

unsigned long CEngineOcclusionManager::AllocateOccluderHandleWithRetrivalID(unsigned long retrievalID)
{
    unsigned long handle;
    if (EntriesSize < 0x2000) {
        unsigned long index = EntriesSize;
        COccluderEntry empty;
        empty.PixelCount = 0;
        empty.Bits = 0;
        Entries[index] = empty;
        ++EntriesSize;
        handle = index;
    }
    else
        handle = 0xffffffff;
    SHandlePair* it = LowerBound(HandleMapper._M_start, HandleMapper._M_finish, retrievalID);
    if (it != HandleMapper._M_finish && it->first == retrievalID) {
        it->second = handle;
        return handle;
    }
    HandleMapper.insert(it, SHandlePair(retrievalID, handle));
    return handle;
}