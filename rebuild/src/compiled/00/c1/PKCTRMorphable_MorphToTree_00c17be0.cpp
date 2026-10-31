// PKCTRMorphable::MorphToTree(void* comparator) -- switch the morphable container's implementation to a red-black tree: trace "Error: NULL implimentation" (PKCTRMorphable.cpp line 62) when there is none; unless it already is a tree (implementation slot 1), build a PKCTRRedblackTree with no shared allocator (trace "Error: EMEM", line 66, when the allocation fails), move every item across with the comparator, delete the old implementation and keep the new one
// Leaf: ?MorphToTree@PKCTRMorphable@@UAEXPAX@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C17BE0, 425 bytes, thiscall `ret 4`, EH-framed (two trace strings, states 0 / 2; the
//   new-expression's unwind, state 1).
// Identity (class from its trace file, method name descriptive; libpk has no PDB or RTTI): slot 3 of the morphable vtable
//   0x012B2DEC (its only reference, 0x012B2DF8), the vtable the landed constructor 0x00C185B0 stores; its neighbours 0x00C186D0
//   / 0x00C187A0 (slots 0 / 1) forward the implementation's slots 0 / 1 and slot 2 is the twin 0x00C18870 (MorphToArray). The
//   traces name ".\PKCTRMorphable.cpp" (0x012B2E50); libpk names its classes after their files (PKCTRArray.cpp,
//   PKCTRRedblackTree.cpp). The new implementation is the 0x28-byte PKCTRRedblackTree (vtable 0x012B2F0C; ctor 0x00C194F0 takes a
//   shared allocator, 0 here: the tree then owns its own); the tree's slot 0 returns 0 and slot 1 returns 1 (0x00C18A40 /
//   0x00C18A50), so slot 1 asks "is a tree". The argument is handed to
//   MoveAllItems 0x00C17B70 as the first (ecx) argument, which it passes to the containers' Remove / Add slots (6 / 5): the
//   item comparator, kept `void*` as in the landed MoveAllItems spelling.
// Callees: MoveAllItems 0x00C17B70 (landed leaf, its PKCContainer_00c17b70 interface spelling), operator new 0x00BFEA1A,
//   PKCTRRedblackTree::PKCTRRedblackTree 0x00C194F0, the implementation's virtual destructor (slot 14, `push 1 ; call [edx+38h]`), the libpk
//   trace stream (CDebugStream 0x00C0DEC0 / 0x00C0DF00 / ~0x00C0BA40, g_pDebugBuffer 0x0139C900, g_pfnErrorReport 0x0143BB2C,
//   CDebugBreak 0x00C16750; the PKCTRArray::RemoveValue spellings).
// Layout: local model (no engine header; libpk is not in the PDB type set).
void* __cdecl operator new(unsigned int size);             // 0x00BFEA1A
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

class PKCContainer_00c17b70 {              // libpk container interface (PKCTRArray 0x012B4E80, PKCTRRedblackTree 0x012B2F0C, morphable 0x012B2DEC)
public:
    virtual int IsArray();                                     // slot 0
    virtual int IsTree();                                      // slot 1
    virtual void MorphToArray(void* comparator);               // slot 2
    virtual void MorphToTree(void* comparator);                // slot 3
    virtual void Reserve(long count);                          // slot 4
    virtual void Add(void* owner, void* item);                 // slot 5
    virtual void Remove(void* owner, void* item);              // slot 6
    virtual void* Find(void* comparator, const void* item);    // slot 7
    virtual void* FindByKey(void* comparator, const void* key);// slot 8
    virtual void* GetItemByIndex(long index);                  // slot 9
    virtual long GetCount();                                   // slot 10
    virtual void* GetFirst();                                  // slot 11
    virtual void* GetLast();                                   // slot 12
    virtual void ForEach(void* visitor);                       // slot 13
    virtual ~PKCContainer_00c17b70();                          // slot 14
};

struct PKCTRRedblackNode {
    PKCTRRedblackNode* Left;               // +0x00
    PKCTRRedblackNode* Right;              // +0x04
    PKCTRRedblackNode* Parent;             // +0x08
    long               Colour;             // +0x0c (0 black, 1 red)
    void*              Key;                // +0x10
};
class PKCTRAllocator;                      // node allocator, vtable 0x012B2F60
class PKCTRRedblackTree : public PKCContainer_00c17b70 {        // 0x28 bytes, vtable 0x012B2F0C
public:
    PKCTRAllocator*    PAllocator;                             // +0x04
    PKCTRAllocator*    POwnedAllocator;                        // +0x08
    PKCTRRedblackNode  Sentinel;                               // +0x0c (0x14 bytes)
    PKCTRRedblackNode* Root;                                   // +0x20
    PKCTRRedblackNode* LastFound;                              // +0x24
    PKCTRRedblackTree(PKCTRAllocator* allocator);              // 0x00C194F0
};

void __fastcall MoveAllItems(void* owner, PKCContainer_00c17b70* source, PKCContainer_00c17b70* dest);   // 0x00C17B70

class PKCTRMorphable : public PKCContainer_00c17b70 {           // 8 bytes, vtable 0x012B2DEC
public:
    PKCContainer_00c17b70* m_pImplementation;                  // +0x04
    virtual void MorphToTree(void* comparator);                // slot 3 (0x00C17BE0)
};

void PKCTRMorphable::MorphToTree(void* comparator)
{
    if (m_pImplementation == 0)
    {
        {
            CDebugStream s(g_pDebugBuffer);
            s << ".\\PKCTRMorphable.cpp" << "(" << 62 << ") : " << "Error: NULL implimentation" << "\n";
            if (g_pfnErrorReport)
                g_pfnErrorReport(s.m_pBuffer);
        }
        CDebugBreak();
    }
    if (!m_pImplementation->IsTree())
    {
        PKCContainer_00c17b70* implementation = new PKCTRRedblackTree(0);
        if (implementation == 0)
        {
            {
                CDebugStream s(g_pDebugBuffer);
                s << ".\\PKCTRMorphable.cpp" << "(" << 66 << ") : " << "Error: EMEM" << "\n";
                if (g_pfnErrorReport)
                    g_pfnErrorReport(s.m_pBuffer);
            }
            CDebugBreak();
        }
        MoveAllItems(comparator, m_pImplementation, implementation);
        delete m_pImplementation;
        m_pImplementation = implementation;
    }
}