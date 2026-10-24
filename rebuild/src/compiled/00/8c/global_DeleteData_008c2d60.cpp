struct IData {
// Leaf: DeleteData
    virtual void Release(int flag) = 0;
};

void __fastcall DeleteData(IData *self)
{
    if (self)
        self->Release(1);
}