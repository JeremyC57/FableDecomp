// CStringKeyedTable_006fd040::~CStringKeyedTable_006fd040() -- erase and free the lookup tree at +0x14, destroy and free the CCharString-keyed vector at +4, then the CBase destructor
// Leaf: ??1CStringKeyedTable_006fd040
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006FD040, length 112 bytes.
// Label correction: catalogued as CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip (propagated). The class has no
// vtable store of its own and is used as a base/member by several records: CManager::Save/Load stack objects
// (0x006F9E9D, 0x006FA95E), the record destructors @ 0x006FD1B0 / 0x006FD330 / 0x006FD400 (its object at +8), the
// record destructor @ 0x00AECB80 (at +0) and the class of vtable 0x0129F38C (two members at +0x1ec / +0x210, 0x24 apart);
// no PDB name could be pinned, so the class is named by address.
// Callees: the tree erase @ 0x004ABA70, free @ 0x00BFEA14 (x2), vector<pair<CCharString,...>>::_Destroy @ 0x006FBCC0
// (`__fastcall` first/last, the iterator tag by reference at `lea [esp+0Bh]`), CBase::~CBase @ 0x0099A300.
// Shape: members destroyed in reverse order -- the red-black tree's inline destructor (as the landed ~CRetailTree in
// 0x00755520), then the vector map's vector (destroy range, free) -- the class `__declspec(novtable)` over CBase, then the
// base destructor as a plain call (the frame word for the tag is popped after it); `ret`.
extern "C" void __cdecl free(void* block);         // 0x00BFEA14

struct CBase {
    virtual void v00();
    ~CBase();                                      // 0x0099A300
};

struct SNonscalarPtrIteratorTag { SNonscalarPtrIteratorTag() { } };

struct SCharStringPair;                            // pair<CCharString, ...>
void __fastcall DestroyRange(SCharStringPair* first, SCharStringPair* last, const SNonscalarPtrIteratorTag& tag);   // 0x006FBCC0

struct CCharStringPairVector {
    SCharStringPair* _M_start;
    SCharStringPair* _M_finish;
    SCharStringPair* _M_end_of_storage;
    ~CCharStringPairVector()
    {
        DestroyRange(_M_start, _M_finish, SNonscalarPtrIteratorTag());
        if (_M_start)
            free(_M_start);
    }
};
struct CCharStringVectorMap {                      // CVectorMap: the vector and its comparator
    CCharStringPairVector m_vector;                // +0x00
    long                  m_compare;               // +0x0c
};
struct CRetailTree_004aba70 {                      // the retail red-black tree {head, size, compare}
    struct node { unsigned char colour; unsigned char _pad[3]; node* parent; node* left; node* right; };
    node*         head;
    unsigned long size;
    long          compare;
    void Erase(node* root);                        // 0x004ABA70
    ~CRetailTree_004aba70()
    {
        if (size) {
            Erase(head->parent);
            head->left = head;
            head->parent = 0;
            head->right = head;
            size = 0;
        }
        if (head)
            free(head);
    }
};

struct __declspec(novtable) CStringKeyedTable_006fd040 : CBase {
    CCharStringVectorMap m_entries;                // +0x04
    CRetailTree_004aba70 m_index;                  // +0x14
    virtual void v00();
    ~CStringKeyedTable_006fd040();
};

CStringKeyedTable_006fd040::~CStringKeyedTable_006fd040()
{
}
