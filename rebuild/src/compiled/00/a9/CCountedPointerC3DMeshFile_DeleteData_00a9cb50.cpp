struct AutoTinyVirtualObject
// Leaf: DeleteData
{
    virtual ~AutoTinyVirtualObject();
};
void __fastcall DeleteData(AutoTinyVirtualObject* object)
{
    delete object;
}