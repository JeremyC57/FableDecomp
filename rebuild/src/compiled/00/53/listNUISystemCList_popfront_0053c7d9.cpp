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
    ListIt erase_0053c7b9(ListIt _Where);
};
void __fastcall pop_front(MyList* self)
{
    self->erase_0053c7b9(ListIt(self->head->next));
}