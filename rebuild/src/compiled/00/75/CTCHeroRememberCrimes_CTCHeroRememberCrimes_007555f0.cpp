// CTCHeroRememberCrimes::CTCHeroRememberCrimes(CThing* parent) -- CTCBase(parent), empty region -> (crime duration, fine) map, HasPaidTheFine = false
// Leaf: ??0CTCHeroRememberCrimes
// Retail: Fable.exe (TLC, VC7.1) @ 0x007555F0, 71 bytes.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, malloc @ 0x00BFEA0E (the STLport node allocator).
// Label: the manifest's "CEngineSubPrimitive2DClockSprite_Constructor" is a propagation -- the vtable stored, 0x0126980C,
// is CTCHeroRememberCrimes (its landed OnSerialise 0x007556B0 is slot 1 of that vtable).
// Layout (retail): RegionNamesToCrimeDurationAndFine = STLport map<CCharString, pair<long,long>> +0x0C (header +0x0C,
// node count +0x10, empty compare +0x14), HasPaidTheFine +0x18 (PDB +0x1C: Dinkumware map is 16 bytes).
// Shape: the STLport _Rb_tree constructor inlined: the alloc proxy nulls the header, allocate(1) mallocs one 0x1C-byte
// node (colour/parent/left/right + an 0xC-byte pair<const CCharString, pair<long,long>>), the count is zeroed, then
// _M_empty_initialize() writes colour red (0), root 0, leftmost/rightmost = header. Every header store re-reads the
// member (the node pointer stores may alias it); the zero register (ebx) is shared with the bool.
struct CThing;
extern "C" void* __cdecl malloc(unsigned int size);    // 0x00BFEA0E

struct CRetailLess { };

struct CCrimeMap {                                      // map<CCharString, pair<long,long>>
    struct node {
        char colour;                                    // +0x00
        node* parent;                                   // +0x04
        node* left;                                     // +0x08
        node* right;                                    // +0x0C
        unsigned char value[0xC];                       // +0x10 pair<const CCharString, pair<long,long>>
    };
    node*         m_header;                             // +0x00
    unsigned long m_nodeCount;                          // +0x04
    CRetailLess   m_compare;                            // +0x08
    CCrimeMap() : m_header(0)
    {
        m_header = (node*)malloc(sizeof(node));
        m_nodeCount = 0;
        m_header->colour = 0;
        m_header->parent = 0;
        m_header->left = m_header;
        m_header->right = m_header;
    }
};

struct CTCBase_Calls {                                  // CTCBase, 0xc bytes
    virtual void v00();
    unsigned char _body[0x8];                           // ParentThing +4, ToBeRemoved +8, IsInGlobalUpdateTCs +9
    CTCBase_Calls(CThing* parent);                      // 0x00686800
};

struct CTCHeroRememberCrimes : CTCBase_Calls {          // vtable 0x0126980C
    CCrimeMap RegionNamesToCrimeDurationAndFine;        // +0x0C
    bool      HasPaidTheFine;                           // +0x18
    virtual void v00();
    CTCHeroRememberCrimes(CThing* parent);
};

CTCHeroRememberCrimes::CTCHeroRememberCrimes(CThing* parent)
    : CTCBase_Calls(parent)
    , HasPaidTheFine(false)
{
}