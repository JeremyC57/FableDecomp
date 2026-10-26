#pragma optimize("s",on)
// Leaf: OnKill
struct Node { void* p0; void* f4; Node* f8; Node* fc; };
class CTCCarryable { public: Node* head; long count; };
extern void __stdcall FreeChunk(void* p);
void __fastcall CTCCarryable_OnKill(CTCCarryable* self)
{
    if (self->count != 0) {
        FreeChunk(self->head->f4);
        self->head->f8 = self->head;
        self->head->f4 = 0;
        self->head->fc = self->head;
        self->count = 0;
    }
}