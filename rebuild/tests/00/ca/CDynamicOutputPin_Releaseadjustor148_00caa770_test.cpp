#include <cstdio>
// [thunk] CDynamicOutputPin::Release adjustor: a call through the second interface base reaches the override with
// `this` moved back to the first interface base (the compiler-generated adjustment).
struct IPinLike { virtual unsigned Release() = 0; int pad; };
struct IFlowLike { virtual unsigned Release() = 0; virtual int Block(unsigned f) = 0; };
struct DynPin : IPinLike, IFlowLike {
    IPinLike* seen; int refs;
    unsigned Release() { seen = this; return --refs; }
    int Block(unsigned f) { return (int)f; }
};
int main()
{
    DynPin d; d.seen = 0; d.refs = 2;
    IFlowLike* flow = &d;
    if ((void*)flow == (void*)(IPinLike*)&d) { std::printf("00caa770_TEST FAIL\n"); return 1; }
    if (flow->Release() != 1 || d.seen != (IPinLike*)&d || d.refs != 1) { std::printf("00caa770_TEST FAIL\n"); return 1; }
    std::printf("00caa770_TEST PASS\n");
    return 0;
}