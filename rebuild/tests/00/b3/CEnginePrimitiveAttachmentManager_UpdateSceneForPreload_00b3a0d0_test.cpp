// Behaviour test for CEnginePrimitiveAttachmentManager::UpdateSceneForPreload (0x00B3A0D0): attached primitives referenced outside the manager get their output updated at time 0 and enabled.
#include <cstdio>
struct Output { bool Enable; };
struct Attached { unsigned int IVRefCount; Output* OutputPrimitive; float updatedAt; int updates;
    void UpdateOutputPrimitive(float t) { updatedAt = t; ++updates; } };
struct Node { Node* next; Attached* object; };
static unsigned int GetReferenceCount(const Attached* a) { return a != 0 ? a->IVRefCount : 0; }
static void UpdateSceneForPreload(Node* head)
{
    for (Node* n = head->next; n != head; n = n->next)
        if (GetReferenceCount(n->object) > 1) { n->object->UpdateOutputPrimitive(0.0f); n->object->OutputPrimitive->Enable = true; }
}
int main()
{
    Output o1, o2; o1.Enable = o2.Enable = false;
    Attached a1 = { 2, &o1, -1.0f, 0 }, a2 = { 1, &o2, -1.0f, 0 };
    Node head, n1, n2, n3; head.next = &n1; n1.next = &n2; n2.next = &n3; n3.next = &head;
    n1.object = &a1; n2.object = &a2; n3.object = 0;
    UpdateSceneForPreload(&head);
    if (a1.updates != 1 || a1.updatedAt != 0.0f || !o1.Enable || a2.updates != 0 || o2.Enable) { std::printf("00b3a0d0_TEST FAIL\n"); return 1; }
    std::printf("00b3a0d0_TEST PASS\n");
    return 0;
}