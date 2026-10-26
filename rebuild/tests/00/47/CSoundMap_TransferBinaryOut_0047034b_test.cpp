#include <cstdio>
#ifndef _MSC_VER
#define FABLE_ENGINE_CDATAOUTPUTSTREAM_H
class CDataOutputStream { public:void* __vftable;void* NewLine_PStringData;};
#endif
// CSoundMap::TransferBinaryOut(CDataOutputStreamMethods&) -- write sound-key/range records then criteria-key/string-table records
// Leaf: TransferBinaryOut
// Retail0047034B,195 bytes ending ret4 at0047040B, no trailing padding.
// Shared CDataOutputStream retained; named method-only derived adapter adds writer calls.
// No CSoundMap/CDefString headers. PDB two CVectorMap members
// donor20 bytes each become retail16 bytes (12-byte vector plus comparator/dirty flags).
// Stream writes993EA0 ULONG/993EB0 SLONG, both4 bytes through virtual Write slot4.
#pragma optimize("s",on)
#include "engine/CDataOutputStream.h"
struct CDataOutputStreamMethods : CDataOutputStream { void WriteULONG(unsigned long); void WriteSLONG(long); };
class CSoundPair { public:long FirstSound,LastSound;};
class CDefString { public:long TablePos;};
template<class Value> struct CMapEntry {unsigned long Key;Value Data;};
template<class Value> struct CSoundVectorMap {
    CMapEntry<Value>* Start;CMapEntry<Value>* Finish;CMapEntry<Value>* End;
    unsigned char Comparator;bool Dirty;
    unsigned long size() const {return Finish-Start;}
    CMapEntry<Value>& operator[](unsigned long index) {return Start[index];}
};
class CSoundMap { public:
    CSoundVectorMap<CSoundPair> SoundMap;
    CSoundVectorMap<CDefString> CriteriaMap;
    void TransferBinaryOut(CDataOutputStreamMethods&);
};
void CSoundMap::TransferBinaryOut(CDataOutputStreamMethods& output) {
    output.WriteULONG(SoundMap.size());
    for(unsigned long i=0;i<SoundMap.size();++i) {
        output.WriteULONG(SoundMap[i].Key);
        output.WriteULONG(SoundMap[i].Data.FirstSound);
        output.WriteULONG(SoundMap[i].Data.LastSound);
    }
    output.WriteULONG(CriteriaMap.size());
    for(unsigned long j=0;j<CriteriaMap.size();++j) {
        output.WriteULONG(CriteriaMap[j].Key);
        output.WriteSLONG(CriteriaMap[j].Data.TablePos);
    }
}

static unsigned long values[32];static char types[32];
static int calls,bad,change;
static CDataOutputStreamMethods* stream;
static CSoundMap* active;
static void record(CDataOutputStreamMethods* self,unsigned long value,char type) {
    if(self!=stream || calls>=32) {bad=1;return;}
    values[calls]=value;types[calls]=type;++calls;
    if(change) {
        if(calls==2) active->SoundMap.Start[0].Data.FirstSound=77;
        if(calls==3) active->SoundMap.Start[0].Data.LastSound=-88;
        if(calls==9) active->CriteriaMap.Start[0].Data.TablePos=-99;
    }
}
void CDataOutputStreamMethods::WriteULONG(unsigned long value) {record(this,value,'U');}
void CDataOutputStreamMethods::WriteSLONG(long value) {record(this,(unsigned long)value,'S');}
static int run(int empty,int mutate) {
    CDataOutputStreamMethods output;stream=&output;
    CSoundMap map;active=&map;
    CMapEntry<CSoundPair> sounds[2];sounds[0].Key=11;sounds[0].Data.FirstSound=-3;sounds[0].Data.LastSound=5;
    sounds[1].Key=17;sounds[1].Data.FirstSound=19;sounds[1].Data.LastSound=-23;
    CMapEntry<CDefString> criteria[2];criteria[0].Key=29;criteria[0].Data.TablePos=-31;
    criteria[1].Key=37;criteria[1].Data.TablePos=41;
    map.SoundMap.Start=sounds;map.SoundMap.Finish=sounds+(empty?0:2);map.SoundMap.End=sounds+2;map.SoundMap.Comparator=47;map.SoundMap.Dirty=true;
    map.CriteriaMap.Start=criteria;map.CriteriaMap.Finish=criteria+(empty?0:2);map.CriteriaMap.End=criteria+2;map.CriteriaMap.Comparator=53;map.CriteriaMap.Dirty=false;
    calls=bad=0;change=mutate;map.TransferBinaryOut(output);
    unsigned long expected[12]={2,11,(unsigned long)(mutate?77:-3),(unsigned long)(mutate?-88:5),17,19,(unsigned long)-23,2,29,(unsigned long)(mutate?-99:-31),37,41};
    if(bad || calls!=(empty?2:12)) return 0;
    for(int i=0;i<calls;++i) if(values[i]!=(empty?0:expected[i]) || types[i]!=(!empty && (i==9 || i==11)?'S':'U')) return 0;
    return map.SoundMap.Start==sounds && map.SoundMap.Finish==sounds+(empty?0:2) && map.SoundMap.End==sounds+2 &&
      map.SoundMap.Comparator==47 && map.SoundMap.Dirty && map.CriteriaMap.Start==criteria && map.CriteriaMap.Finish==criteria+(empty?0:2) &&
      map.CriteriaMap.End==criteria+2 && map.CriteriaMap.Comparator==53 && !map.CriteriaMap.Dirty;
}
int main() {
    if(!run(1,0) || !run(0,0) || !run(0,1)) {printf("0047034b_TEST FAIL\n");return 1;}
    printf("0047034b_TEST PASS\n");return 0;
}