struct Node { Node* next; };
struct MyList;
struct ListIt {
    Node* _Ptr;
    ListIt() {}
    ListIt(Node* p) : _Ptr(p) {}
    ListIt(const ListIt& o) : _Ptr(o._Ptr) {}
};
struct MyList {
    Node* head;
    ListIt erase_006553d0(ListIt _Where);
};
void __fastcall pop_front(MyList* self)
{
    self->erase_006553d0(ListIt(self->head->next));
}