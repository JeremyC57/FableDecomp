// Behaviour test for 0x0047042A CSoundMap::TransferBinaryIn: the row's defines, includes and body verbatim over the shared
// headers, run on zeroed images of the map and of a CMemoryDataInputStream whose chunk holds the whole record stream (the
// inline bounds-checked Read never overflows the chunk). Doubles: CDataInputStream::ReadWithSrcChunkOverflow (must not be
// reached) and CDefString::TransferBinaryIn (reads one table position).
#include <stdio.h>
#include <string.h>
// ---- row body ----
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING_TRANSFER
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CVECTORMAP_SORT
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_CSOUNDMAP
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#define FABLE_RETAIL_CARRAY_INDEX_ASSERT
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_algorithm.h"
#include "engine/CMemoryDataInputStream.h"
#include "engine/CSoundMap.h"

void CSoundMap::TransferBinaryIn(CMemoryDataInputStream& stream)
{
    unsigned long count = stream.ReadULONG();
    SoundMap.resize(count, std::pair<unsigned long,CSoundPair>());
    for (unsigned long i = 0; i < SoundMap.size(); ++i) {
        SoundMap[i].first = stream.ReadULONG();
        SoundMap[i].second.FirstSound = stream.ReadSLONG();
        SoundMap[i].second.LastSound = stream.ReadSLONG();
    }
    SoundMap.Update();
    count = stream.ReadULONG();
    CriteriaMap.resize(count, std::pair<unsigned long,CDefString>());
    for (unsigned long j = 0; j < CriteriaMap.size(); ++j) {
        CriteriaMap[j].first = stream.ReadULONG();
        CriteriaMap[j].second.TransferBinaryIn(stream);
    }
    CriteriaMap.Update();
}


// ---- doubles ----
static int g_overflowReads = 0;
void CDataInputStream::ReadWithSrcChunkOverflow(unsigned char*, long) { ++g_overflowReads; }
static int g_defStringReads = 0;
void CDefString::TransferBinaryIn(CDataInputStream& stream) { ++g_defStringReads; *(long*)this = stream.ReadSLONG(); }
// the release ASSERT's dead arm is still referenced at /Od (never reached: the arm sits behind `if (0)`)
static int g_assertCalls = 0;
bool __fastcall GFAssertFailed(const char*, const char*, const char*, unsigned long) { ++g_assertCalls; return false; }
// the two STLport bodies the row's headers only declare (retail calls them out of line): a plain growth-and-fill
// _M_fill_insert and an insertion sort behind the __fastcall std::sort
#include <stdlib.h>
namespace std {
template<class _Tp,class _Alloc> void vector<_Tp,_Alloc>::_M_fill_insert(iterator position,size_type n,const _Tp& x)
{
    size_type before = size_type(position - this->_M_start), after = size_type(this->_M_finish - position);
    _Tp* fresh = (_Tp*)malloc((before + n + after) * sizeof(_Tp));
    memcpy(fresh, this->_M_start, before * sizeof(_Tp));
    for (size_type i = 0; i < n; ++i)
        memcpy(fresh + before + i, &x, sizeof(_Tp));
    memcpy(fresh + before + n, position, after * sizeof(_Tp));
    free(this->_M_start);
    this->_M_start = fresh; this->_M_finish = fresh + before + n + after; this->_M_end_of_storage._M_data = this->_M_finish;
}
template<class _RandomAccessIter,class _Compare> void __fastcall sort(_RandomAccessIter first,_RandomAccessIter last,_Compare comp)
{
    for (_RandomAccessIter i = first; i != last; ++i)
        for (_RandomAccessIter j = i; j != first && comp(*j, *(j - 1)); --j) {
            unsigned char tmp[64];
            memcpy(tmp, &*j, sizeof(*j)); memcpy(&*j, &*(j - 1), sizeof(*j)); memcpy(&*(j - 1), tmp, sizeof(*j));
        }
}
}

// ---- harness ----
static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
struct SSoundRecord { unsigned long key; long first; long last; };       // std::pair<unsigned long,CSoundPair>
struct SCriteriaRecord { unsigned long key; long tablePos; };            // std::pair<unsigned long,CDefString>
int main()
{
    // the stream: 3 sound records (keys out of order), 2 criteria records (keys out of order)
    static const unsigned long data[] = {
        3, 30, 300, 301, 10, 100, 101, 20, 200, 201,
        2, 9, 77, 5, 55,
        0xdeadbeef };
    static unsigned char streamImage[sizeof(CMemoryDataInputStream)];
    static unsigned char mapImage[sizeof(CSoundMap)];
    memset(streamImage, 0, sizeof(streamImage)); memset(mapImage, 0, sizeof(mapImage));
    *(const unsigned char**)(streamImage + 0x0c) = (const unsigned char*)data;      // CurrentSourcePtr
    *(long*)(streamImage + 0x14) = 15 * 4;                                          // SourceChunkBytesRemaining
    CMemoryDataInputStream& stream = *(CMemoryDataInputStream*)streamImage;
    CSoundMap& map = *(CSoundMap*)mapImage;
    check(sizeof(CSoundMap) == 0x20 && sizeof(SSoundRecord) == sizeof(std::pair<unsigned long,CSoundPair>) && sizeof(SCriteriaRecord) == sizeof(std::pair<unsigned long,CDefString>), "layouts");
    mapImage[0x0d] = 1;            // SoundMap.Dirty: the loaded records are re-sorted
    mapImage[0x1d] = 0;            // CriteriaMap.Dirty clear: left in stream order

    map.TransferBinaryIn(stream);

    const SSoundRecord* sounds = *(const SSoundRecord**)(mapImage + 0x00);
    const SSoundRecord* soundsEnd = *(const SSoundRecord**)(mapImage + 0x04);
    check(soundsEnd - sounds == 3, "three sound records");
    check(sounds[0].key == 10 && sounds[0].first == 100 && sounds[0].last == 101, "sound record sorted first");
    check(sounds[1].key == 20 && sounds[1].first == 200 && sounds[1].last == 201, "sound record sorted second");
    check(sounds[2].key == 30 && sounds[2].first == 300 && sounds[2].last == 301, "sound record sorted third");
    check(mapImage[0x0d] == 0, "SoundMap dirty flag cleared by the sort");
    const SCriteriaRecord* criteria = *(const SCriteriaRecord**)(mapImage + 0x10);
    const SCriteriaRecord* criteriaEnd = *(const SCriteriaRecord**)(mapImage + 0x14);
    check(criteriaEnd - criteria == 2 && g_defStringReads == 2, "two criteria records, one def-string read each");
    check(criteria[0].key == 9 && criteria[0].tablePos == 77 && criteria[1].key == 5 && criteria[1].tablePos == 55, "criteria records in stream order (map not dirty)");
    check(*(long*)(streamImage + 0x04) == 15 * 4 && *(long*)(streamImage + 0x14) == 0, "whole stream consumed");
    check(*(const unsigned char**)(streamImage + 0x0c) == (const unsigned char*)(data + 15) && g_overflowReads == 0, "source pointer advanced inside the chunk");

    // a second load over the same map replaces the records (resize down) and sorts the dirty criteria map
    static const unsigned long data2[] = { 1, 7, 70, 71, 2, 8, 80, 4, 40 };
    memset(streamImage, 0, sizeof(streamImage));
    *(const unsigned char**)(streamImage + 0x0c) = (const unsigned char*)data2;
    *(long*)(streamImage + 0x14) = 9 * 4;
    mapImage[0x1d] = 1;
    map.TransferBinaryIn(stream);
    sounds = *(const SSoundRecord**)(mapImage + 0x00); soundsEnd = *(const SSoundRecord**)(mapImage + 0x04);
    criteria = *(const SCriteriaRecord**)(mapImage + 0x10); criteriaEnd = *(const SCriteriaRecord**)(mapImage + 0x14);
    check(soundsEnd - sounds == 1 && sounds[0].key == 7 && sounds[0].first == 70 && sounds[0].last == 71, "second load: one sound record");
    check(criteriaEnd - criteria == 2 && criteria[0].key == 4 && criteria[0].tablePos == 40 && criteria[1].key == 8 && criteria[1].tablePos == 80, "second load: criteria sorted");
    check(mapImage[0x1d] == 0 && g_overflowReads == 0 && g_assertCalls == 0, "CriteriaMap dirty flag cleared");
    if (!g_fail) printf("0047042a_TEST PASS\n");
    return g_fail;
}