
struct Prim {
    virtual void slot0();
    virtual void slot1();
};

struct ListNode {
    ListNode* next;
    ListNode* prev;
    Prim* obj;
};

struct MemberAt38 {
    int dummy;
    void Clear();
};

class CWorld {
public:
    static long __fastcall GetFrame();   // 0x0049D870
};   // 0x0049D870

struct CTCGraphicAppearance {
    char pad[0x38];
    MemberAt38 m38;
    ListNode* head;
    char pad2[0x54 - 0x40];
    int counter;
};

void __fastcall CTCGraphicAppearance_ClearAllPrimitives(CTCGraphicAppearance* appearance)
{
    ListNode* node = appearance->head->next;
    while (node != appearance->head) {
        node->obj->slot1();
        node = node->next;
    }
    appearance->m38.Clear();
    appearance->counter = CWorld::GetFrame() + 1;
}