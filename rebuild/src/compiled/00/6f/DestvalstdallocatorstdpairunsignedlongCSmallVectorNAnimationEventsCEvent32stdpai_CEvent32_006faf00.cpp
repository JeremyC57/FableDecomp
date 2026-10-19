// _Dest_val for a small-vector: free the heap buffer, null the pointer+size.
void operator delete[](void*);

struct SmallVec {
    int   f0;
    void* f4;   // heap buffer
    int   f8;   // count/size
};

void __fastcall _Dest_val_pair_ulong_CSmallVector(SmallVec* self)
{
    operator delete[](self->f4);
    self->f4 = 0;
    self->f8 = 0;
}