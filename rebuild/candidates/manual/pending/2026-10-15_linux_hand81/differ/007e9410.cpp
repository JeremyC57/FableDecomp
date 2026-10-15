// CTCAvoidRegionExit::OnKill() -- run the base OnKill, remove the parent thing's entry from the global exit-weight map and recompute the maximum weight
// Grade: DIFFER(ours v retail) -- 78v78: one load out of place: retail reads range.first (`mov edx,[esp+8]`) before reserving the second iterator's argument slot, the lane reads it after copying the second (same instructions). Tried: plain call, inline map forwarders (both loads hoisted, `mov ecx,esp` slot addressing), STLport base-class iterator copy, a named first-iterator local.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E9410, length 78 bytes (+ int3 padding).
// Identity: slot 14 of vtable 0x0123EFAC = CTCAvoidRegionExit (rtti-vtables.tsv; slot 14 of
// CTCBoastingArea is also labelled OnKill). The base call is CTCBase's empty slot-14 implementation
// @ 0x006868F0 (a direct, qualified call).
// Callees: the map's equal_range @ 0x007E9AE0 (thiscall, hidden pair result + the key by const
// reference; manifest "std::map_Tree_Find_Node"), erase(first, last) @ 0x007E99F0 (landed as the
// CRetailTree erase-range; two 4-byte iterators by value), UpdateMaxExitWeight @ 0x007E9250 (walks
// the map for the largest weight into the global float @ 0x013BAE28; manifest
// "TreeWalk_FindMaxValue"). Global 0x013BAF00 = a map<CThing*, float> (key +0x10, weight +0x14 of
// the node, unsigned key compare) of every avoid-region exit by its parent thing.
// Shape: the parent pointer reaches equal_range through an inline accessor returning it BY VALUE, so
// the const-reference key is a temporary (retail copies [esi+4] into a stack slot); the pair is
// returned through the hidden pointer into a frame local; each iterator is built in its argument
// slot (`push ecx ; mov eax,esp ; mov [eax],..`, second first).
// The key temporary gets its own frame word here (no argument slot to reuse).
struct CThing;

struct SExitWeightNode;                                     // map node: colour, parent, left, right, key +0x10, weight +0x14

struct CExitWeightIterator {                                // 4-byte iterator with a user copy constructor:
    SExitWeightNode* m_pNode;                               // by value it is built in its argument slot
    CExitWeightIterator(const CExitWeightIterator& other) : m_pNode(other.m_pNode) { }
};

struct SExitWeightRange {                                   // pair<iterator, iterator>: non-POD, returned by hidden pointer
    CExitWeightIterator first;
    CExitWeightIterator second;
};

struct CExitWeightMap {                                     // map<CThing*, float>, header + size
    SExitWeightNode* m_pHeader;
    unsigned long    m_size;
    SExitWeightRange EqualRange(CThing* const& key);        // 0x007E9AE0
    void Erase(CExitWeightIterator first, CExitWeightIterator last);   // 0x007E99F0
};

extern CExitWeightMap g_AvoidRegionExitWeights;             // 0x013BAF00

struct CTCBase_Calls {                                      // engine/CTCBase.h layout (0xc bytes)
    void*   __vftable;                                      // +0x00
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    CThing* GetParentThing() const { return ParentThing; }  // by value: the key is an rvalue temporary
    void vfunc_2();                                         // 0x006867F0 (empty base implementation, virtual slot 2)
    void OnKill();                                          // 0x006868F0 (empty base implementation, virtual slot 14)
    void vfunc_16(bool flag);                               // 0x00686910 (empty base implementation, virtual slot 16)
};

struct CTCAvoidRegionExit : CTCBase_Calls {
    unsigned char _body[0x20 - 0x0c];                       // EntranceMarker, ExitMarker, NextFrameCheck
    void UpdateMaxExitWeight();                             // 0x007E9250
    __forceinline void RemoveFromExitWeights()
    {
        SExitWeightRange range = g_AvoidRegionExitWeights.EqualRange(GetParentThing());
        g_AvoidRegionExitWeights.Erase(range.first, range.second);
        UpdateMaxExitWeight();
    }
    void OnKill();
};

void CTCAvoidRegionExit::OnKill()
{
    CTCBase_Calls::OnKill();
    RemoveFromExitWeights();
}
