#include <stdio.h>

struct Node
{
    char pad0[0x20];
    void* GridCell;
    char pad1[0x14];
    Node* NextPrimitive;
    void* RefPrimitive;
    void* NextLayerMask;
    void* RefLayerMask;
};

void __fastcall ClearPrimitiveList(Node* self)
{
    Node* p = self;
    while (p != 0)
    {
        Node* next = p->NextPrimitive;
        p->GridCell = 0;
        p->NextPrimitive = 0;
        p->RefPrimitive = 0;
        p->NextLayerMask = 0;
        p->RefLayerMask = 0;
        p = next;
    }
}

int main()
{
    Node a, b, c;
    a.GridCell = (void*)1; a.RefPrimitive = (void*)1; a.NextLayerMask = (void*)1; a.RefLayerMask = (void*)1;
    b.GridCell = (void*)1; b.RefPrimitive = (void*)1; b.NextLayerMask = (void*)1; b.RefLayerMask = (void*)1;
    c.GridCell = (void*)1; c.RefPrimitive = (void*)1; c.NextLayerMask = (void*)1; c.RefLayerMask = (void*)1;
    a.NextPrimitive = &b; b.NextPrimitive = &c; c.NextPrimitive = 0;

    ClearPrimitiveList(&a);

    int ok = 1;
    Node* arr[3]; arr[0]=&a; arr[1]=&b; arr[2]=&c;
    for (int i = 0; i < 3; i++)
    {
        if (arr[i]->GridCell || arr[i]->NextPrimitive || arr[i]->RefPrimitive || arr[i]->NextLayerMask || arr[i]->RefLayerMask)
            ok = 0;
    }

    ClearPrimitiveList(0);

    if (ok) printf("PARITY_OK\n");
    else printf("FAIL\n");
    return 0;
}