// CMapBrushOwner004fe510::ApplyScriptedMapBrushByName(const CCharString&, long) -- look the name up in the name-to-index map and forward the brush to the indexed entry's map
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004FE510, length 54 bytes (+ int3 padding).
// Callees: map<CCharString, long>::Find(const CCharString&) @ 0x0042B467 (the VC6 Dinkumware walk with less<CCharString>; returns the node or the head), CMap::ApplyScriptedMapBrush
// (CCharString const&, long) @ 0x00820B80 on the entry's map pointer. The chunk label "CDrawQuestInfo::OracleSymbolPushBack" is propagated over a
// different body (the real one is @ 0x0064A3A1); this object owns a name map at +0x14 and a pointer at +0x20 to an array of 72-byte entries
// whose word at +0x10 is the map, so it is named by address (its neighbour @ 0x004FE550 is CreatureMode_ApplyAnimatedBrush).
// Shape: the map is a named pointer (`lea edi,[esi+14h]`) whose head is compared with the result; the node's value (+0x14) indexes the entry
// array (`lea edx,[eax+eax*8]` then `[eax+edx*8+10h]`); the second argument is forwarded as a 4-byte value.
struct CCharString;

struct CRetailNameMap {                            // VC6 Dinkumware map<CCharString, long>: head node pointer (+ size)
    struct node { node* left; node* parent; node* right; long colour; void* key; long value; };
    node* head;
    node* Find(const CCharString& key);            // 0x0042B467
};

struct CMap {
    void ApplyScriptedMapBrush(const CCharString& name, long flags);   // 0x00820B80
};

struct SMapEntry {                                 // 72-byte entry
    unsigned char _pad[0x10];
    CMap*         m_pMap;                          // +0x10
    unsigned char _rest[72 - 0x14];
};

struct CMapBrushOwner004fe510 {
    unsigned char  _pad[0x14];
    CRetailNameMap m_nameToIndex;                  // +0x14
    unsigned char  _pad18[0x20 - 0x18];
    SMapEntry*     m_pEntries;                     // +0x20
    void ApplyScriptedMapBrushByName(const CCharString& name, long flags);
};

void CMapBrushOwner004fe510::ApplyScriptedMapBrushByName(const CCharString& name, long flags)
{
    CRetailNameMap* map = &m_nameToIndex;
    CRetailNameMap::node* found = map->Find(name);
    if (found != map->head)
        m_pEntries[found->value].m_pMap->ApplyScriptedMapBrush(name, flags);
}
