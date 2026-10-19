#include <cstdio>
#include <cstdlib>
#include <cstring>
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

static unsigned long g_raw[32];
static SHandlePair* const g_storage = (SHandlePair*)g_raw;
SHandlePair* CHandleMapper::insert(SHandlePair* position, const SHandlePair& value)
{
    long at = position - _M_start;
    long n = _M_finish - _M_start;
    for (long i = n; i > at; --i) _M_start[i] = _M_start[i - 1];
    _M_start[at] = value;
    ++_M_finish;
    return _M_start + at;
}
int main()
{
    CEngineOcclusionManager* m = (CEngineOcclusionManager*)std::calloc(1, sizeof(CEngineOcclusionManager));
    static COccluderEntry entries[0x2000];
    m->Entries = entries;
    m->HandleMapper._M_start = g_storage; m->HandleMapper._M_finish = g_storage; m->HandleMapper._M_end_of_storage = g_storage + 16;
    entries[0].PixelCount = 77; entries[0].Bits = 0xffffffff;
    bool ok = true;
    if (m->AllocateOccluderHandleWithRetrivalID(50) != 0) ok = false;
    if (entries[0].PixelCount != 0 || entries[0].Bits != 0 || m->EntriesSize != 1) ok = false;
    if (m->AllocateOccluderHandleWithRetrivalID(10) != 1) ok = false;
    if (m->AllocateOccluderHandleWithRetrivalID(30) != 2) ok = false;
    // sorted map: 10->1, 30->2, 50->0
    if (m->HandleMapper._M_finish - m->HandleMapper._M_start != 3) ok = false;
    if (g_storage[0].first != 10 || g_storage[0].second != 1 || g_storage[1].first != 30 || g_storage[1].second != 2
        || g_storage[2].first != 50 || g_storage[2].second != 0) ok = false;
    // existing id: remapped in place, no insert
    if (m->AllocateOccluderHandleWithRetrivalID(30) != 3 || g_storage[1].second != 3 || m->HandleMapper._M_finish - m->HandleMapper._M_start != 3) ok = false;
    // full table: handle -1
    m->EntriesSize = 0x2000;
    if (m->AllocateOccluderHandleWithRetrivalID(10) != 0xffffffffUL || g_storage[0].second != 0xffffffffUL || m->EntriesSize != 0x2000) ok = false;
    std::free(m);
    if (!ok) { std::printf("00b8d6c0_TEST FAIL\n"); return 1; }
    std::printf("00b8d6c0_TEST PASS\n");
    return 0;
}
