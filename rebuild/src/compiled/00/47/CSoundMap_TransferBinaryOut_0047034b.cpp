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