#pragma optimize("s",on)
// operator delete forwarder @ 0x005949a5
extern "C" void __stdcall inner_free_00594706(void *block, void *heap, int flags);
extern "C" void __stdcall operator_delete(void *block)
{
    inner_free_00594706(block, 0, 0);
}