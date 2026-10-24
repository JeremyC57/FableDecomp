struct CObservable { char pad[0xA0]; void* exclusive; };
// Leaf: AddExclusiveObserver

void __fastcall CObservable_AddExclusiveObserver(CObservable* self, int edx, void* observer)
{
    self->exclusive = observer;
}