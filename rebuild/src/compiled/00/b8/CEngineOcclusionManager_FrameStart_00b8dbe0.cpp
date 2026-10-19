// CEngineOcclusionManager::FrameStart(const CEngineComponentParams&) -- swaps current/previous occlusion vectors, clears current queues, and advances frame; retail 0x00B8DBE0 length 0xE1. Helpers: vector<28>::resize 0x00B8F780 and vector<pair<unsigned long,unsigned long>>::resize 0x009B8350. Shared manager header is included; retail treats the opaque collections as adjacent 12-byte begin/end/capacity triples, matching helper expectations despite the generated 16-byte CArray labels.
#include <algorithm>
#if defined(_MSC_VER)
#include "engine/CEngineOcclusionManager.h"
typedef unsigned long RetailWord;
#else
struct COccluderEntry;
typedef unsigned int RetailWord;
struct CEngineOcclusionManager {
    COccluderEntry *Entries; COccluderEntry *PrevFrameEntries;
    RetailWord EntriesSize, PrevFrameEntriesSize, CurrentFrame;
    unsigned char _pad[0x24024 - 2 * sizeof(void *) - 3 * sizeof(RetailWord)];
    unsigned char _collections[0x80];
};
#endif
struct CEngineComponentParams;
struct SOccluderBehaviour { unsigned char bytes[0x1c]; };
struct SHandlePair { unsigned char bytes[8]; };
struct COccluderVector {
    RetailWord begin;
    RetailWord end;
    RetailWord capacity;
    void resize(RetailWord, const SOccluderBehaviour&); // vector<28>::resize, 0x00B8F780
};
struct CHandleMapVector {
    RetailWord begin;
    RetailWord end;
    RetailWord capacity;
    void resize(RetailWord); // vector<pair<unsigned long,unsigned long>>::resize, 0x009B8350
};
struct CEngineOcclusionManager_FrameStart : CEngineOcclusionManager {
    void FrameStart(const CEngineComponentParams&);
};
static void swap3(RetailWord *a, RetailWord *b)
{
    RetailWord t = a[0]; a[0] = b[0]; b[0] = t;
    t = a[1]; a[1] = b[1]; b[1] = t;
    t = a[2]; a[2] = b[2]; b[2] = t;
}
void CEngineOcclusionManager_FrameStart::FrameStart(const CEngineComponentParams&)
{
    std::swap(Entries, PrevFrameEntries);
    PrevFrameEntriesSize = EntriesSize;
    EntriesSize = 0;
    unsigned char *base = reinterpret_cast<unsigned char *>(this);
    RetailWord *a = reinterpret_cast<RetailWord *>(base + 0x24024);
    RetailWord *b = reinterpret_cast<RetailWord *>(base + 0x24030);
    std::swap(a[0], b[0]); std::swap(a[1], b[1]); std::swap(a[2], b[2]);
    a = reinterpret_cast<RetailWord *>(base + 0x2403c);
    b = reinterpret_cast<RetailWord *>(base + 0x24048);
    std::swap(a[0], b[0]); std::swap(a[1], b[1]); std::swap(a[2], b[2]);
    ++CurrentFrame;
    SOccluderBehaviour unusedFillValue;
    reinterpret_cast<COccluderVector *>(base + 0x24024)->resize(0, unusedFillValue);
    reinterpret_cast<CHandleMapVector *>(base + 0x2403c)->resize(0);
}