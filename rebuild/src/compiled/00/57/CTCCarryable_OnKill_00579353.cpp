#pragma optimize("s", on)
struct AutoTinyListNode
{
    void* reserved;
    void* allocation;
    AutoTinyListNode* next;
    AutoTinyListNode* previous;
};
struct AutoTinyOwnedList
{
    AutoTinyListNode* head;
    long count;
};
extern void __stdcall AutoTinyFreeListAllocation_00578fa1(void* allocation);
void __fastcall OnKill(AutoTinyOwnedList* self)
{
    if (self->count != 0)
    {
        AutoTinyFreeListAllocation_00578fa1(self->head->allocation);
        self->head->next = self->head;
        self->head->allocation = 0;
        self->head->previous = self->head;
        self->count = 0;
    }
}