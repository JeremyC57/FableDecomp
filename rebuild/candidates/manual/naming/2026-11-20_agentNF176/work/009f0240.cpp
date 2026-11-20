// CCodeSectionManager::GetSectionDesc(unsigned long) -- returns the description registered for a section handle, or a shared empty description when the handle is unknown
// Leaf: ?GetSectionDesc@CCodeSectionManager@@AAEAAUCSectionDesc@1@K@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009F0240, 121 bytes of code (`ret 4`).
// Callees: the SectionDesc map's find @ 0x009F0D90 (STLport _Rb_tree::find: iterator returned through the hidden
//          pointer, the key by reference -- the address of this function's own argument slot; manifest label
//          "RBTree_FindBestFit" is propagated), CCharString::CCharString() @ 0x0099E4B0 (twice), atexit @ 0x004012BC.
// Globals: the function-local static empty description @ 0x013CAA54 (two CCharStrings, a byte at +8, a long at +0xc)
//          and its guard word @ 0x013CAA64; the atexit destructor thunk @ 0x0122A940.
// Shape: `it == end()` compares with the map's header pointer (STLport: end() is the header node); the found path
//          returns &node->value.second (node +0x14 = +0x10 key + 4). The not-found path constructs the static once
//          (guard bit 0) and then clears its byte and long on EVERY call before returning it (`xor eax,eax` shared).
// Retail layout: the maps are 12-byte STLport trees (SectionDesc at +0x1c, PDB +0x24), so `this` is modelled locally.
// NF176 (2026-11-20, q3968): respelled in place to the Ego_r.pdb 0x00B424D0 / FableWin.pdb 0x03166E60 spelling
//   ?GetSectionDesc@CCodeSectionManager@@AAEAAUCSectionDesc@1@K@Z (private, non-const reference, CSectionDesc nested in
//   CCodeSectionManager): the row's own CSectionDesc model moved into the class, and the static renamed to the PDB `desc` (both PDBs
//   name its atexit thunk "dynamic atexit destructor for 'desc'"); the static now carries the spelling the landed atexit-destructor row
//   0x0122A940 binds (?desc@?4??GetSectionDesc@CCodeSectionManager@@AAEAAUCSectionDesc@2@K@Z@4U32@A). Was public `const CSectionDesc&`
//   with a global CSectionDesc and `s_empty`.
class CCharStringData;

class CCharString { public:
    CCharStringData* PStringData;
    CCharString();                                  // 0x0099E4B0
    ~CCharString();                                 // 0x0099EAE0
};

struct SSectionNode;

struct CSectionMap {                                // STLport map<unsigned long, CSectionDesc>, 12 bytes in retail
    struct iterator {
        SSectionNode* m_pNode;
        iterator() { }
    };
    SSectionNode* m_pHeader;                        // +0x00 end()
    unsigned long m_count;
    unsigned long m_compare;
    iterator find(const unsigned long& key);        // 0x009F0D90
};

class CCodeSectionManager { public:
    struct CSectionDesc {                           // Ego_r.pdb CCodeSectionManager::CSectionDesc, 16 bytes
        CCharString ShortName;                      // +0x00
        CCharString LongName;                       // +0x04
        bool        Loaded;                         // +0x08
        long        Handle;                         // +0x0c
    };
    unsigned char _pad00[0x1c];
    CSectionMap SectionDesc;                        // +0x1c
    bool SectionLoadingEnabled;                     // +0x28
private:
    CSectionDesc& GetSectionDesc(unsigned long handle);
};

struct SSectionNode {                               // STLport _Rb_tree_node<pair<const unsigned long, CSectionDesc>>
    long          Color;                            // +0x00
    SSectionNode* Parent;                           // +0x04
    SSectionNode* Left;                             // +0x08
    SSectionNode* Right;                            // +0x0c
    unsigned long Key;                              // +0x10
    CCodeSectionManager::CSectionDesc Value;        // +0x14
};

CCodeSectionManager::CSectionDesc& CCodeSectionManager::GetSectionDesc(unsigned long handle)
{
    CSectionMap::iterator it = SectionDesc.find(handle);
    if (it.m_pNode == SectionDesc.m_pHeader) {
        static CSectionDesc desc;
        desc.Loaded = false;
        desc.Handle = 0;
        return desc;
    }
    return it.m_pNode->Value;
}
