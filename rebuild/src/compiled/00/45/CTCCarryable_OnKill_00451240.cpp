#pragma optimize("s",on)
// Leaf: OnKill
struct Node {
    void* f0;
    void* f4;
    Node* f8;
    Node* fc;
};
class CTCCarryable { public:
    Node* node;
    int flag;
};
extern void __stdcall ReleaseThing_004508d6(void* p);
void __fastcall CTCCarryable_OnKill(CTCCarryable* self)
{
    if (self->flag != 0) {
        Node* n = self->node;
        ReleaseThing_004508d6(n->f4);
        n = self->node;
        n->f8 = n;
        n = self->node;
        n->f4 = 0;
        n = self->node;
        n->fc = n;
        self->flag = 0;
    }
}