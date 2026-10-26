#pragma optimize("s",on)
// Leaf: OnKill
struct Node { void* p0; void* p4; void* p8; void* pc; };
class CTCCarryable { public: Node* node; int flag; };
extern "C" void __stdcall FreeThing(void* p);
void __fastcall CTCCarryable_OnKill(CTCCarryable* self)
{
    if (self->flag != 0)
    {
        FreeThing(self->node->p4);
        self->node->p8 = self->node;
        self->node->p4 = 0;
        self->node->pc = self->node;
        self->flag = 0;
    }
}