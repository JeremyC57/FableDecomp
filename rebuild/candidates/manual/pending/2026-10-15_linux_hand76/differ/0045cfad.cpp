// Grade: DIFFER(ours v retail) -- 49v49, register/scheduling only: the second Dirty byte is copied through cl with `mov eax,esi` and two pops scheduled between the load and the store, retail copies it through al before the pops; explicit, implicit and void-returning inner operator= give the same, `optimize("s")` out-lines the inner operator= (33 bytes)
// CSoundMap::operator=(const CSoundMap& other) -- assign the SoundMap and CriteriaMap vector-maps member by member: each one's vector (out of line) and its Dirty flag
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045CFAD, length 49 bytes.
// Leaf: ??4CSoundMap
// The chunk label (CSoundMap::CSoundMap) is the right class but the wrong member: the body assigns (each
// callee starts with the `&other == this` test of a vector operator=) and returns `this`.
// Callees: 0x0045CFDE (vector<pair<unsigned long, CSoundPair>>::operator=, 12-byte elements) and 0x0045D183
// (vector<pair<unsigned long, CDefString>>::operator=, 8-byte elements); their manifest labels
// CActiveFile_AssignUndefinedVector12/8 are placeholders.
// Layout from the PDB (Ego_r): CSoundMap = { SoundMap, CriteriaMap }, each a CVectorMap<K, V, CKeyPairCompareLess>
// = { vector Data, empty Compare, bool Dirty } (PDB 20 bytes with the 16-byte vector; retail 0x10: the vector
// +0x00, Compare +0x0c, Dirty +0x0d), so CriteriaMap sits at +0x10.
// Shape: the first map's vector assigned on `this`, its Dirty byte copied, the second map's address formed
// once (`lea ebx,[esi+10h]`, `add edi,10h`) for its vector call and Dirty copy: an inline memberwise
// CVectorMap::operator= (the empty comparator copies nothing) called twice from a memberwise outer one.
struct CSoundPair {
    long FirstSound;                            // +0x00
    long LastSound;                             // +0x04
};

class CDefString { long m_index; };

struct SSoundEntry   { unsigned long key; CSoundPair value; };     // 12 bytes
struct SCriteriaEntry { unsigned long key; CDefString value; };    // 8 bytes

struct CSoundEntryVector {                      // retail vector<SSoundEntry>: first / last / end
    SSoundEntry* first;
    SSoundEntry* last;
    SSoundEntry* end;
    CSoundEntryVector& operator=(const CSoundEntryVector& other);      // 0x0045CFDE
};

struct CCriteriaEntryVector {                   // retail vector<SCriteriaEntry>: first / last / end
    SCriteriaEntry* first;
    SCriteriaEntry* last;
    SCriteriaEntry* end;
    CCriteriaEntryVector& operator=(const CCriteriaEntryVector& other);   // 0x0045D183
};

struct CKeyPairCompareLess { };                 // empty comparator (1 byte)

template <class TVector>
struct CVectorMap {
    TVector             Data;                   // +0x00
    CKeyPairCompareLess Compare;                // +0x0c
    bool                Dirty;                  // +0x0d
    CVectorMap& operator=(const CVectorMap& other)
    {
        Data = other.Data;
        Dirty = other.Dirty;
        return *this;
    }
};

struct CSoundMap {
    CVectorMap<CSoundEntryVector>    SoundMap;      // +0x00
    CVectorMap<CCriteriaEntryVector> CriteriaMap;   // +0x10
    CSoundMap& operator=(const CSoundMap& other);
};

CSoundMap& CSoundMap::operator=(const CSoundMap& other)
{
    SoundMap = other.SoundMap;
    CriteriaMap = other.CriteriaMap;
    return *this;
}
