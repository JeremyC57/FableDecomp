#pragma optimize("s", on)
struct Node {
    Node* next;            // +0x00
    long  pad;             // +0x04
    long  pad2;            // +0x08
    long  value;           // +0x0c
};

struct NodeList {          // the list sentinel
    Node* head;
    bool empty() const { return head->next == head; }
    long& front() const { return head->next->value; }
};

struct CMultiStatBar {
    char     pad0[0x28];
    long     mode;         // +0x28
    char     pad1[0x68 - 0x2c];
    NodeList list;         // +0x68
};

long __fastcall GetCurrentChange(CMultiStatBar* self)
{
    if (self->mode != 1 && !self->list.empty())
        return self->list.front();
    return 0;
}
