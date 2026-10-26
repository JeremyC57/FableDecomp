// CShaderRenderManager::SetPixelShaderConstant(const CCharString& name, const C4DVector& value) -- look the named pixel-shader constant up (adding a new name + table slot when it is unknown or unassigned), store the value in its slot and mark the pixel-shader constants dirty
// Leaf: SetPixelShaderConstant
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0098B390, length 144 bytes (140 + int3 padding).
// Callees: the PSConstNames index map's find @ 0x0042B467 (returns the node pointer in eax; the header
// node is end()), CIndexedArray<CCharString>::Add @ 0x0098DB20 (manifest "CVectorMap_Insert_Morphs"),
// the PSConstTable push_back @ 0x0098D820 (manifest "std_vector_PushBack_Fixed").
// Shape: PSConstNames in a reference local (ebp, the map at ebp+0xc); `if (it == end || (index = it->second) == -1) { index = size(); Add(name); push_back(value); }`
// (size from the signed pointer difference `sar esi,4`), then `table[index] = value` as an implicit
// 16-byte copy and `UpdateFlags |= 2` in memory form.
// Layout: PDB PSConstNames (CIndexedArray<CCharString>) +0x3c8, PSConstTable +0x3e8, UpdateFlags +0x3f8;
// retail +0x3b4 (its name array, then the name -> index map at +0xc), +0x3cc, +0x3d8.
class CCharString { public:
    void* PStringData;
};

class C4DVector { public:
    float X, Y, Z, W;
};

struct CNameIndexNode {                          // STLport _Rb_tree_node<pair<const CCharString, long>>
    long            color;
    CNameIndexNode* parent;
    CNameIndexNode* left;
    CNameIndexNode* right;
    CCharString     key;                         // +0x10
    long            index;                       // +0x14
};

struct CNameIndexMap {                           // map<CCharString, long>
    CNameIndexNode* header;                      // +0x00 end()
    unsigned long   count;                       // +0x04
    unsigned long   compare;                     // +0x08
    CNameIndexNode* find(const CCharString& name);   // 0x0042B467
};

struct CIndexedCharStringArray {                 // CIndexedArray<CCharString>
    CCharString*  first;                         // +0x00
    CCharString*  last;                          // +0x04
    CCharString*  end_of_storage;                // +0x08
    CNameIndexMap Map;                           // +0x0c
    void Add(const CCharString& name);           // 0x0098DB20
};

struct CC4DVectorArray {                         // CArray<C4DVector>
    C4DVector* first;                            // +0x00
    C4DVector* last;                             // +0x04
    C4DVector* end_of_storage;                   // +0x08
    long size() const { return last - first; }
    void push_back(const C4DVector& value);      // 0x0098D820
    C4DVector& operator[](long i) { return *(first + i); }
};

class CShaderRenderManager { public:
    unsigned char           _pad000[0x3b4];
    CIndexedCharStringArray PSConstNames;        // +0x3b4
    CC4DVectorArray         PSConstTable;        // +0x3cc
    unsigned int            UpdateFlags;         // +0x3d8
    void SetPixelShaderConstant(const CCharString& name, const C4DVector& value);
};

void CShaderRenderManager::SetPixelShaderConstant(const CCharString& name, const C4DVector& value)
{
    long index;
    CIndexedCharStringArray& names = PSConstNames;
    CNameIndexNode* it = names.Map.find(name);
    if (it == names.Map.header || (index = it->index) == -1) {
        index = PSConstTable.size();
        names.Add(name);
        PSConstTable.push_back(value);
    }
    PSConstTable[index] = value;
    unsigned int* flags = &UpdateFlags;          // a pointer local gives the memory-form `or`
    *flags |= 2u;
}