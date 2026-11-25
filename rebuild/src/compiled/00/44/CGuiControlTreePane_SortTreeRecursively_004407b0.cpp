// CGuiControlTreePane::SortTreeRecursively @ 004407b0 (__fastcall, void)
// Clears a circular doubly-linked list: destroys each node's subobject and frees it.
struct Node {
    Node* next; // +0x0
    Node* prev; // +0x4
    char pad[0x10]; // +0x8 .. +0x17
    int sub; // +0x18 : subobject destroyed via thiscall
};
struct CGuiControlTreePane {
    Node* head; // +0x0 pointer to sentinel head node
};
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

// Naming (NF449 gen, 2026-11-25): callee-only respell of this row's destructor stand-in calls to the PDB destructor (T1 tracker;
//   target self rows' Leaf), as a qualified destructor call on a bodiless declaration (no layout; rulings r2/r3):
//   0x0099B510 ??1CWideString@@QAE@XZ (was @NodeSub_Dtor@4)
class CWideString { public: ~CWideString(); };
void __fastcall CGuiControlTreePane_SortTreeRecursively(CGuiControlTreePane* self)
{
    Node* head = self->head;
    Node* cur = head->next;
    if (cur != head) {
        do {
            Node* node = cur;
            cur = cur->next;
            reinterpret_cast<CWideString*>(&node->sub)->CWideString::~CWideString();
            free(node);
        } while (cur != self->head);
    }
    head = self->head;
    head->next = head;
    head = self->head;
    head->prev = head;
}