#include <cstdio>
#include <cstring>
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

struct COccluderEntry { unsigned Tag; };
struct CEngineComponentParams { unsigned Unused; };
static CEngineOcclusionManager_FrameStart* active;
static RetailWord swapped[12];
static RetailWord expectedFrame;
static int calls;
static bool bad;
static RetailWord* words() { return reinterpret_cast<RetailWord*>(reinterpret_cast<unsigned char*>(active)+0x24024); }
void COccluderVector::resize(RetailWord n,const SOccluderBehaviour&) {
 RetailWord* w=words();
 if(n || this!=reinterpret_cast<COccluderVector*>(w) || calls!=0 || active->CurrentFrame!=expectedFrame || active->EntriesSize!=0) bad=true;
 for(int i=0;i<12;++i) if(w[i]!=swapped[i]) bad=true;
 ++calls; end=begin;
}
void CHandleMapVector::resize(RetailWord n) {
 RetailWord* w=words();
 if(n || this!=reinterpret_cast<CHandleMapVector*>(w+6) || calls!=1 || w[1]!=w[0]) bad=true;
 for(int i=0;i<12;++i) if(i!=1 && w[i]!=swapped[i]) bad=true;
 ++calls; end=begin;
}
static bool run(RetailWord frame,bool empty) {
 CEngineOcclusionManager_FrameStart object;
 // The shared layout is a storage-only view; it has no C++ virtual functions.
 std::memset(&object,0,sizeof(object)); active=&object;
 COccluderEntry current,previous;
 object.Entries=&current;object.PrevFrameEntries=&previous;
 object.EntriesSize=empty?0:13; object.PrevFrameEntriesSize=4; object.CurrentFrame=frame;
 RetailWord* w=words();
 for(int i=0;i<12;++i) w[i]=empty?0:RetailWord(0x1000+i*0x100);
 for(int j=0;j<3;++j) {swapped[j]=w[j+3];swapped[j+3]=w[j];swapped[j+6]=w[j+9];swapped[j+9]=w[j+6];}
 expectedFrame=frame+RetailWord(1);calls=0;bad=false;
 CEngineComponentParams params;params.Unused=123;
 object.FrameStart(params);
 if(bad || calls!=2 || object.Entries!=&previous || object.PrevFrameEntries!=&current || object.EntriesSize || object.PrevFrameEntriesSize!=(empty?0:13) || object.CurrentFrame!=expectedFrame || params.Unused!=123) return false;
 for(int k=0;k<12;++k) {
  RetailWord expected=k==1?swapped[0]:k==7?swapped[6]:swapped[k];
  if(w[k]!=expected) return false;
 }
 return true;
}
int main() {
 if(!run(77,false) || !run(0,true) || !run(RetailWord(-1),false)) { std::printf("00b8dbe0_TEST FAIL\n");return 1; }
 std::printf("00b8dbe0_TEST PASS\n");return 0;
}