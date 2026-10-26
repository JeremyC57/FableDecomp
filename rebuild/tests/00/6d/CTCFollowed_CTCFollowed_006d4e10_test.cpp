// Behaviour test for 0x006D4E10 CTCFollowed::CTCFollowed: base gets the parent, both follower lists are empty
// self-linked sentinels, the two frame counters start at zero.
#include <cstdio>
#include <cstdlib>

class CThing { public: int m_id; };

struct CTCBase_Calls {
    CThing* ParentThing;
    bool ToBeRemoved;
    virtual ~CTCBase_Calls() {}
    CTCBase_Calls(CThing* parent) : ParentThing(parent), ToBeRemoved(false) {}
};

struct CRetailList_node0x10 {
    struct node { node* next; node* prev; unsigned char value[8]; };
    node* head;
    CRetailList_node0x10() : head(0)
    {
        node* sentinel = (node*)std::malloc(sizeof(node));
        sentinel->next = sentinel;
        sentinel->prev = sentinel;
        head = sentinel;
    }
    ~CRetailList_node0x10() { std::free(head); }
};

class C3DVector { public: float X, Y, Z; C3DVector() {} };

class CTCFollowed : public CTCBase_Calls { public:
    CRetailList_node0x10 Followers;
    CRetailList_node0x10 AIFollowers;
    long FrameLastIdleReaction;
    long FramesSincePlayerMoved;
    C3DVector LastPlayerPosition;
    CTCFollowed(CThing* parent) : CTCBase_Calls(parent), FrameLastIdleReaction(0), FramesSincePlayerMoved(0) {}
};

int main()
{
    CThing hero;
    CTCFollowed f(&hero);
    if (f.ParentThing != &hero || f.FrameLastIdleReaction != 0 || f.FramesSincePlayerMoved != 0) { std::printf("006d4e10_TEST FAIL\n"); return 1; }
    if (f.Followers.head == 0 || f.Followers.head->next != f.Followers.head || f.Followers.head->prev != f.Followers.head) { std::printf("006d4e10_TEST FAIL\n"); return 1; }
    if (f.AIFollowers.head == 0 || f.AIFollowers.head == f.Followers.head || f.AIFollowers.head->next != f.AIFollowers.head) { std::printf("006d4e10_TEST FAIL\n"); return 1; }
    std::printf("006d4e10_TEST PASS\n");
    return 0;
}