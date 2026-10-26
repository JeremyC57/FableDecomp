// CTCAvoidRegionExit::OnSetInLimbo(bool flag) -- run the base slot-16 hook; when the flag is set, remove the parent thing's entry from the global exit-weight map and recompute the maximum weight
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E9360, length 91 bytes (+ int3 padding).
// Identity: slot 16 of vtable 0x0123EFAC = CTCAvoidRegionExit (rtti-vtables.tsv). The base call is
// CTCBase's empty slot-16 implementation @ 0x00686910 (`ret 4`, a direct, qualified call).
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
// The flag rides in ebx; once it is tested its argument slot is dead and VC homes the key temporary
// there (`mov [esp+1Ch],eax`, `lea ecx,[esp+14h]`), so the frame holds only the 8-byte pair.
// Fix (retry 10): equal_range has its same-TU body (lower/upper bound walks; with only the declaration the second
// iterator was loaded late).
class CThing;

struct SExitWeightNode;                                     // map node: colour, parent, left, right, key +0x10, weight +0x14

struct CExitWeightIterator {                                // 4-byte iterator with a user copy constructor:
    SExitWeightNode* m_pNode;                               // by value it is built in its argument slot
    CExitWeightIterator() { }
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
long TmplSink_s7_0(const void* exit, long seen);   // stand-in: the work Erase does (outside the bytes graded here)
#pragma auto_inline(off)
// Same-TU stand-in for the Erase instantiation (template bodies are compiled in the caller's TU): reads the const-reference
// arguments (not the empty tags), stores the out pointers, lets no argument escape.
__declspec(noinline) void CExitWeightMap::Erase(CExitWeightIterator first, CExitWeightIterator last)
{
    long r = TmplSink_s7_0(first.m_pNode, (long)last.m_pNode);   // the range's node pointers read by value
    (void)r;
}
#pragma auto_inline(on)

#pragma auto_inline(off)
// Same-TU body of the map's equal_range instantiation (0x007E9AE0): STLport lower_bound / upper_bound walks over the raw
// node links (parent +4, left +8, right +0xc, key +0x10), the key read through its const reference; the iterator pair
// returned through the hidden pointer.
__declspec(noinline) SExitWeightRange CExitWeightMap::EqualRange(CThing* const& key)
{
    char* head = (char*)m_pHeader;
    char* lower = head;
    char* x = *(char**)(head + 4);
    while (x != 0) {
        if (!(*(CThing**)(x + 0x10) < key)) {
            lower = x;
            x = *(char**)(x + 8);
        } else
            x = *(char**)(x + 0xc);
    }
    char* upper = head;
    x = *(char**)(head + 4);
    while (x != 0) {
        if (key < *(CThing**)(x + 0x10)) {
            upper = x;
            x = *(char**)(x + 8);
        } else
            x = *(char**)(x + 0xc);
    }
    SExitWeightRange range;
    range.first.m_pNode = (SExitWeightNode*)lower;
    range.second.m_pNode = (SExitWeightNode*)upper;
    return range;
}
#pragma auto_inline(on)
extern CExitWeightMap g_AvoidRegionExitWeights;             // 0x013BAF00

struct CTCBase_Calls {                                      // engine/CTCBase.h layout (0xc bytes)
    void*   __vftable;                                      // +0x00
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    CThing* GetParentThing() const { return ParentThing; }  // by value: the key is an rvalue temporary
    void vfunc_2();                                         // 0x006867F0 (empty base implementation, virtual slot 2)
    void OnKill();                                          // 0x006868F0 (empty base implementation, virtual slot 14)
    void OnSetInLimbo(bool flag);                               // 0x00686910 (empty base implementation, virtual slot 16)
};

class CTCAvoidRegionExit : public CTCBase_Calls { public:
    unsigned char _body[0x20 - 0x0c];                       // EntranceMarker, ExitMarker, NextFrameCheck
    void UpdateMaxExitWeight();                             // 0x007E9250
    __forceinline void RemoveFromExitWeights()
    {
        SExitWeightRange range = g_AvoidRegionExitWeights.EqualRange(GetParentThing());
        g_AvoidRegionExitWeights.Erase(range.first, range.second);
        UpdateMaxExitWeight();
    }
    void OnSetInLimbo(bool flag);
};
long NearSink_s7_0(const void* exit, long seen);   // stand-in: the work UpdateMaxExitWeight does outside this TU
#pragma auto_inline(off)
// Same-TU stand-in for CTCAvoidRegionExit::UpdateMaxExitWeight @ 0x007E9250 (compiled in retail's TU): reads its const reference / pointer-to-const
// arguments, updates its out-parameters, and lets no argument escape.
__declspec(noinline) void CTCAvoidRegionExit::UpdateMaxExitWeight()
{
    NearSink_s7_0(this, 0);
}
#pragma auto_inline(on)

void CTCAvoidRegionExit::OnSetInLimbo(bool flag)
{
    CTCBase_Calls::OnSetInLimbo(flag);
    if (flag)
        RemoveFromExitWeights();
}