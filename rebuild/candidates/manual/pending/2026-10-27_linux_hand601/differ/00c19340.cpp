// PKCTRRedblackTree::RemoveValue(CUnprovenArrayComparator* comparator, const void* value) -- remove an item from the libpk red-black tree: take the remembered node (+0x24) when the comparator calls it equal, else binary-search from the root with the comparator's value slots (0 Equal / 1 Less); a miss traces "Error: Key not found" (PKCTRRedblackTree.cpp line 368) and breaks; then unlink the node (textbook deleteNode: successor splice, key copy, DeleteFixup for a black splice), free the spliced node through the allocator and forget the remembered node
// Grade: DIFFER(419v419) -- frame-slot packing only: retail homes the trace string and the spliced node y in the dead `comparator` argument slot ([esp+24h]) and the CDebugBreak temporary in the `value` slot ([esp+28h]); VC7.1 here puts all three in the `value` slot. Every instruction and register matches.
// Leaf: ?RemoveValue@PKCTRRedblackTree@@UAEXPAVCUnprovenArrayComparator@@PBX@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C19340, 419 bytes, thiscall `ret 8`, EH-framed (the trace string, state 0).
// Identity (class proven, method name descriptive; libpk has no PDB or RTTI): slot 6 of the tree vtable 0x012B2F0C (its only
//   reference), between slot 5 0x00C18E20 and Find (slot 7, 0x00C19080, comparator slots 0/1) / FindByKey (slot 8, 0x00C19010);
//   slot 6 of the PKCTRArray vtable 0x012B4E80 is the landed PKCTRArray::RemoveValue 0x00C322E0 with the same (comparator, value)
//   arguments and trace macro, so the same name; the trace names ".\PKCTRRedblackTree.cpp" (0x012B2E98) with
//   "Error: Key not found" (0x012B2F48). Layout of the landed ctor 0x00C194F0 / DeleteFixup 0x00C18C00 (allocator +4, nil
//   sentinel +0x0C, root +0x20, remembered node +0x24, which Find / FindByKey set and the ctor clears). Manifest label
//   "RedBlackTree_RemoveByKey" (a guess: it removes by value, the by-key twin is PKCTRRedblackMap's 0x00C1B7D0).
// Callees: PKCTRRedblackTree::DeleteFixup 0x00C18C00 (landed leaf), the allocator's free-and-clear helper 0x00C19610 (thiscall on
//   the allocator: slot 0 Free(*pp), then *pp = 0; descriptive), the libpk trace stream (CDebugStream 0x00C0DEC0 / 0x00C0DF00 /
//   ~0x00C0BA40, g_pDebugBuffer 0x0139C900, g_pfnErrorReport 0x0143BB2C, CDebugBreak 0x00C16750: the spellings of the landed
//   PKCTRArray::RemoveValue).
// Shape: the spliced node `y` lives in memory (its address goes to the free helper) and shares the dead `comparator` argument
//   slot with the trace string; the trace does not return (retail goes on to unlink the sentinel).
// Layout: local model (no engine header; libpk is not in the PDB type set).
struct CDebugStream {
    char* m_pBuffer;
    CDebugStream(char* buffer) : m_pBuffer(buffer) { }
    CDebugStream& __fastcall operator<<(const char* text); // 0x00C0DEC0
    CDebugStream& __fastcall operator<<(long value);       // 0x00C0DF00
    ~CDebugStream();                                       // 0x00C0BA40
};
struct CDebugBreak { CDebugBreak(); };                     // 0x00C16750
extern char* g_pDebugBuffer;                               // 0x0139C900
extern void (__fastcall* g_pfnErrorReport)(const char*);   // 0x0143BB2C

class CUnprovenArrayComparator {          // the libpk container comparator (landed PKCTRArray::RemoveValue model)
public:
    virtual int EqualValue(const void*, const void*) = 0;
    virtual int LessValue(const void*, const void*) = 0;
    virtual int EqualKey(const void*, const void*) = 0;
    virtual int LessKey(const void*, const void*) = 0;
    virtual ~CUnprovenArrayComparator() {}
};

struct PKCTRRedblackNode {
    PKCTRRedblackNode* Left;               // +0x00
    PKCTRRedblackNode* Right;              // +0x04
    PKCTRRedblackNode* Parent;             // +0x08
    long               Colour;             // +0x0c (0 black, 1 red)
    void*              Key;                // +0x10
};

enum { PKCTR_BLACK = 0, PKCTR_RED = 1 };

class PKCTRAllocator {                     // node allocator, vtable 0x012B2F60 (slot 0 Free 0x00C19640)
public:
    virtual void Free(void* block);
    void FreeAndClear(PKCTRRedblackNode** pp);               // 0x00C19610 (descriptive)
};

class PKCTRRedblackTree {
public:
    virtual ~PKCTRRedblackTree();                              // vtable 0x012B2F0C
    PKCTRAllocator*    PAllocator;                             // +0x04
    PKCTRAllocator*    POwnedAllocator;                        // +0x08
    PKCTRRedblackNode  Sentinel;                               // +0x0c (0x14 bytes)
    PKCTRRedblackNode* Root;                                   // +0x20
    PKCTRRedblackNode* LastFound;                              // +0x24

    void DeleteFixup(PKCTRRedblackNode* x);                    // 0x00C18C00
    virtual void RemoveValue(CUnprovenArrayComparator* comparator, const void* value);   // slot 6 (0x00C19340)
};

// Same-TU body of the free-and-clear helper 0x00C19610 (retail compiles it into this TU, right after the ctor; context only):
// with it in the TU VC7.1 sees that &y does not escape and keeps y in a register.
#pragma auto_inline(off)
void PKCTRAllocator::FreeAndClear(PKCTRRedblackNode** pp)
{
    Free(*pp);
    *pp = 0;
}
#pragma auto_inline(on)

void PKCTRRedblackTree::RemoveValue(CUnprovenArrayComparator* comparator, const void* value)
{
    PKCTRRedblackNode* z;
    if (LastFound && comparator->EqualValue(value, LastFound->Key))
    {
        z = LastFound;
    }
    else
    {
        z = Root;
        while (z != &Sentinel)
        {
            if (comparator->EqualValue(value, z->Key))
                break;
            if (comparator->LessValue(value, z->Key))
                z = z->Left;
            else
                z = z->Right;
        }
        if (z == &Sentinel)
        {
            {
                CDebugStream s(g_pDebugBuffer);
                s << ".\\PKCTRRedblackTree.cpp" << "(" << 368 << ") : " << "Error: Key not found" << "\n";
                if (g_pfnErrorReport)
                    g_pfnErrorReport(s.m_pBuffer);
            }
            CDebugBreak();
        }
    }

    {
    PKCTRRedblackNode* x;
    PKCTRRedblackNode* y;
    if (z->Left == &Sentinel || z->Right == &Sentinel)
    {
        y = z;
    }
    else
    {
        y = z->Right;
        while (y->Left != &Sentinel)
            y = y->Left;
    }
    if (y->Left != &Sentinel)
        x = y->Left;
    else
        x = y->Right;
    x->Parent = y->Parent;
    if (y->Parent)
    {
        if (y == y->Parent->Left)
            y->Parent->Left = x;
        else
            y->Parent->Right = x;
    }
    else
    {
        Root = x;
    }
    if (y != z)
        z->Key = y->Key;
    if (y->Colour == PKCTR_BLACK)
        DeleteFixup(x);
    PAllocator->FreeAndClear(&y);
    }
    LastFound = 0;
}
