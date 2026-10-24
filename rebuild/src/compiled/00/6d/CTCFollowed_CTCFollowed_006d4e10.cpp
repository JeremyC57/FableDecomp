// CTCFollowed::CTCFollowed(CThing* parent) -- CTCBase(parent), own vtable, two empty STLport follower lists, the idle/moved frame counters zeroed
// Leaf: ??0CTCFollowed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D4E10, length 74 bytes (+ int3 padding), `ret 4`. Single caller
// 0x004D5257 (the component factory). The manifest label "CEngineSubPrimitive2DClockSprite_Constructor" is
// propagated from the base constructor's wrong label: the vtable stored is 0x0125F644 = CTCFollowed (rtti-vtables.tsv,
// slots 9/21/22 = the landed CTCFollowed FrameUpdate/GetClassTypeName/scalar deleting destructor).
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800 (catalog "CEngineSubPrimitive2DClockSprite" is propagated);
// malloc @ 0x00BFEA0E (STLport node allocator, one 0x10-byte sentinel per list).
// Layout: Ego_r.pdb CTCFollowed with the retail 4-byte STLport lists (landed CTCFollowed::AddFollower 0x006D50C0):
// Followers +0x0C, AIFollowers +0x10, FrameLastIdleReaction +0x14, FramesSincePlayerMoved +0x18, LastPlayerPosition
// +0x1C (C3DVector, left unset by its empty default constructor).
// Shape: the list constructor (landed std::list 0x006C28A0 model): `head(0)` before the allocation, the sentinel
// self-linked through a local before it is stored; the two counters share the zero register with the head inits.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
struct CThing;

struct CTCBase_Calls {
    virtual void v00();
    unsigned char _body[0x8];                               // ParentThing +4, ToBeRemoved +8, IsInGlobalUpdateTCs +9
    CTCBase_Calls(CThing* parent);                          // 0x00686800
};

struct CRetailList_node0x10 {                               // STLport list<CIntelligentPointer<T>>: one sentinel pointer
    struct node { node* next; node* prev; unsigned char value[0x10 - 8]; };
    node* head;
    CRetailList_node0x10()
        : head(0)
    {
        node* sentinel = (node*)malloc(sizeof(node));
        sentinel->next = sentinel;
        sentinel->prev = sentinel;
        head = sentinel;
    }
};

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
};

struct CTCFollowed : CTCBase_Calls {
    CRetailList_node0x10 Followers;                         // +0x0C list<CIntelligentPointer<CThing>>
    CRetailList_node0x10 AIFollowers;                       // +0x10 list<CIntelligentPointer<CThingAICreature>>
    long                 FrameLastIdleReaction;             // +0x14
    long                 FramesSincePlayerMoved;            // +0x18
    C3DVector            LastPlayerPosition;                // +0x1C
    virtual void v00();                                     // own vtable 0x0125F644
    CTCFollowed(CThing* parent);
};

CTCFollowed::CTCFollowed(CThing* parent)
    : CTCBase_Calls(parent)
    , FrameLastIdleReaction(0)
    , FramesSincePlayerMoved(0)
{
}