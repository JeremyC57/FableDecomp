// PKCTRRedblackTree::AddValue(CUnprovenArrayComparator* comparator, void* value) -- insert a value into the libpk red-black tree: walk from the root with the comparator's value slots (0 Equal / 1 Less), tracing "Error: Duplicate key" (PKCTRRedblackTree.cpp line 298) and breaking when the value is already there; take a node from the allocator (tracing "Error: memory exhausted", line 309, and breaking when it has none), fill it red with the value and both children on the sentinel, hang it under the last node visited (left when the value compares less) or make it the root, rebalance with InsertFixup and forget the remembered node
// Leaf: ?AddValue@PKCTRRedblackTree@@UAEXPAVCUnprovenArrayComparator@@PAX@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C18E20, 486 bytes, thiscall `ret 8`, EH-framed (the two trace strings, states 0 / 1).
// Identity (class proven, method name descriptive; libpk has no PDB or RTTI): slot 5 of the tree vtable 0x012B2F0C (its only
//   reference, 0x012B2F20), the slot the landed PKCTRMorphable::MorphToTree / MorphToArray 0x00C17BE0 / 0x00C18870 call "Add"
//   when they move items between the containers, next to slot 6 RemoveValue 0x00C19340 and slot 7 Find 0x00C19080 (comparator
//   slots 0 / 1); slot 5 of the PKCTRArray vtable 0x012B4E80 is 0x00C32CD0 (this chunk, the same name). The traces name
//   ".\PKCTRRedblackTree.cpp" (0x012B2E98) with "Error: Duplicate key" (0x012B2E80) and "Error: memory exhausted" (0x012B2E68).
//   Layout of the landed ctor 0x00C194F0 / Find 0x00C19080 / DeleteFixup 0x00C18C00 (allocator +4, nil sentinel +0x0C, root
//   +0x20, remembered node +0x24, node {Left, Right, Parent, Colour, Key}); the catalog's gap-scan name is a placeholder.
// Callees: the allocator's virtual slot 1 Allocate() (vtable 0x012B2F60: slot 0 Free 0x00C19640, slot 1 0x00C19650),
//   PKCTRRedblackTree::InsertFixup 0x00C18B20 (thiscall `ret 4`; landed under the address-suffixed class, NAME_FIXES), the libpk
//   trace stream (CString operator<< 0x00C0DEC0 text / 0x00C0DF00 long, ~CString 0x00C0BA40, g_pchNil 0x0139C900,
//   g_pfnAssertHook 0x0143BB2C) and CDebugBreak 0x00C16750.
// Shape: the comparator is re-read from its argument slot at every virtual call; the two CDebugBreak temporaries take the
//   dead bytes of the 8-byte frame below the trace string; the node's fields are stored Parent, Left, Right, Colour, Key.
extern char* g_pchNil;                          // 0x0139C900 (the shared empty string data)
typedef void (__fastcall* PFNASSERTHOOK)(const char* message);
extern PFNASSERTHOOK g_pfnAssertHook;           // 0x0143BB2C

class CString {                                 // the libpk MFC-style string: one word
public:
    char* m_pchData;
    CString() : m_pchData(g_pchNil) { }
    ~CString();                                         // 0x00C0BA40
    CString& __fastcall operator<<(const char* text);   // 0x00C0DEC0
    CString& __fastcall operator<<(long value);         // 0x00C0DF00
};
struct CDebugBreak {                            // empty temporary whose constructor breaks into the debugger
    CDebugBreak();                              // 0x00C16750
};

class CUnprovenArrayComparator {          // the libpk container comparator (landed PKCTRArray::RemoveValue / FindValue model)
public:
    virtual int EqualValue(const void*, const void*) = 0;
    virtual int LessValue(const void*, const void*) = 0;
    virtual int EqualKey(const void*, const void*) = 0;
    virtual int LessKey(const void*, const void*) = 0;
    virtual ~CUnprovenArrayComparator() {}
};

struct PKCTRRedblackNode {                      // 0x14 bytes
    PKCTRRedblackNode* Left;               // +0x00
    PKCTRRedblackNode* Right;              // +0x04
    PKCTRRedblackNode* Parent;             // +0x08
    long               Colour;             // +0x0c (0 black, 1 red)
    void*              Key;                // +0x10
};

enum { PKCTR_BLACK = 0, PKCTR_RED = 1 };

class PKCTRAllocator {                     // node allocator, vtable 0x012B2F60
public:
    virtual void Free(void* block);        // slot 0 (0x00C19640)
    virtual void* Allocate();              // slot 1 (0x00C19650)
};

class PKCTRRedblackTree {
public:
    virtual ~PKCTRRedblackTree();                              // vtable 0x012B2F0C
    PKCTRAllocator*    PAllocator;                             // +0x04
    PKCTRAllocator*    POwnedAllocator;                        // +0x08
    PKCTRRedblackNode  Sentinel;                               // +0x0c (0x14 bytes)
    PKCTRRedblackNode* Root;                                   // +0x20
    PKCTRRedblackNode* LastFound;                              // +0x24

    void InsertFixup(PKCTRRedblackNode* x);                    // 0x00C18B20
    virtual void AddValue(CUnprovenArrayComparator* comparator, void* value);   // slot 5 (0x00C18E20)
};

void PKCTRRedblackTree::AddValue(CUnprovenArrayComparator* comparator, void* value)
{
    PKCTRRedblackNode* parent = 0;
    PKCTRRedblackNode* node = Root;
    while (node != &Sentinel) {
        if (comparator->EqualValue(value, node->Key)) {
            {
                CString line;
                line << ".\\PKCTRRedblackTree.cpp" << "(" << 298L << ") : " << "Error: Duplicate key" << "\n";
                if (g_pfnAssertHook)
                    g_pfnAssertHook(line.m_pchData);
            }
            CDebugBreak();
        }
        parent = node;
        if (comparator->LessValue(value, node->Key))
            node = node->Left;
        else
            node = node->Right;
    }
    node = (PKCTRRedblackNode*)PAllocator->Allocate();
    if (node == 0) {
        {
            CString line;
            line << ".\\PKCTRRedblackTree.cpp" << "(" << 309L << ") : " << "Error: memory exhausted" << "\n";
            if (g_pfnAssertHook)
                g_pfnAssertHook(line.m_pchData);
        }
        CDebugBreak();
    }
    node->Parent = parent;
    node->Left = &Sentinel;
    node->Right = &Sentinel;
    node->Colour = PKCTR_RED;
    node->Key = value;
    if (parent) {
        if (comparator->LessValue(value, parent->Key))
            parent->Left = node;
        else
            parent->Right = node;
    } else {
        Root = node;
    }
    InsertFixup(node);
    LastFound = 0;
}