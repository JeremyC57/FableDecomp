// CIndexMap_00c3b160::Get(unsigned long index) -- look a value up by its numeric index: the map's slot-17 lookup returns the {value, index} node or null; the node's value (or 0) is returned
// Leaf: ?Get@CIndexMap_00c3b160@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00c3b160, 20 bytes (+ int3 padding), __fastcall `ret 4`.
// Library: libpk audio bank loader (no PDB/RTTI); descriptive names. Same map as 0x00C3B0E0's Add (the twin
//   instantiation is 0x00C3ABD0). Callers: 0x00C2E2C3. The manifest's "Container_GetFirstItem" is propagated.
// Shape: the index is looked up through its argument slot; `if (!node) return 0;` before the dereference returns the
//   tested zero eax (the ternary form adds `xor eax,eax` at the end).
struct SIndexNode_00c3b160 {                           // 8 bytes
    void* m_pValue;                                         // +0x00
    unsigned long m_index;                                  // +0x04
};

class CIndexMap_00c3b160 {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Insert(SIndexNode_00c3b160* node);                      // slot 9 (+0x24)
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16();
    virtual SIndexNode_00c3b160* Lookup(const unsigned long* index);     // slot 17 (+0x44)
    void* Get(unsigned long index);
};

void* CIndexMap_00c3b160::Get(unsigned long index)
{
    SIndexNode_00c3b160* node = Lookup(&index);
    if (!node)
        return 0;
    return node->m_pValue;
}