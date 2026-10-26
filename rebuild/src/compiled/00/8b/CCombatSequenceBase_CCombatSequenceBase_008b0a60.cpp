// CCombatSequenceBase::~CCombatSequenceBase() -- own vtable, then the CombatActions vector's inline destructor: destroy the elements and free the block
// Leaf: ??1CCombatSequenceBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B0A60, length 45 bytes.
// Callees: the element range destroy @ 0x008B3010 (landed `_Destroy_range` over 8-byte counted pointers,
// `__fastcall` first/last + a stack tag), free @ 0x00BFEA14.
// Identity: vtable 0x01278FEC = CCombatSequenceBase (rtti-vtables.tsv); the manifest's "CTCCarryable::OnKill"
// label is propagated and rejected. Layout: Ego_r.pdb CCombatSequenceBase (Running +4, NoAvailableActions +8,
// CurrentActionIndex +0xc, CombatActions +0x10, Creature, Target, CreateCalled); the retail vector is 12 bytes.
// Shape: the polymorphic root with an empty destructor body; the member vector's inline destructor passes
// its empty tag by reference from the high byte of the `push ecx` frame slot (`lea eax,[esp+7]`, the landed
// CChunkedFileSuperChunk 0x00AA8490 form) and frees `first` after a null test.
extern "C" void __cdecl free(void* block);                                  // 0x00BFEA14

class CThingCreatureBase;

class CCombatAction { public: unsigned char _body[8]; };                           // a counted pointer (8 bytes)

struct SNonscalarPtrIteratorTag { SNonscalarPtrIteratorTag() { } };

void __fastcall DestroyRange(CCombatAction* first, CCombatAction* last, const SNonscalarPtrIteratorTag& tag);   // 0x008B3010

template <class T> struct CRetailVector {                                    // retail std::vector: {first, last, end}
    T* first;
    T* last;
    T* end_of_storage;
    ~CRetailVector()
    {
        DestroyRange(first, last, SNonscalarPtrIteratorTag());
        if (first != 0)
            free(first);
    }
};

class CCombatSequenceBase { public:                                                 // vtable 0x01278FEC
    bool                          Running;              // +0x04
    long                          NoAvailableActions;   // +0x08
    long                          CurrentActionIndex;   // +0x0c
    CRetailVector<CCombatAction>  CombatActions;        // +0x10
    CThingCreatureBase*           Creature;             // +0x1c
    CThingCreatureBase*           Target;               // +0x20
    bool                          CreateCalled;         // +0x24
    virtual void v00();
    ~CCombatSequenceBase();
};

CCombatSequenceBase::~CCombatSequenceBase()
{
}