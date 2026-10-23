// operator new: forward the size to a fixed global allocator object (0x013B8760).
// __fastcall size=ecx; the pool method cleans its own stack arg.
struct Pool { void* Alloc(unsigned size); };
extern Pool g_pool_013b8760;
extern "C" void* __fastcall OperatorNew(unsigned size) { return g_pool_013b8760.Alloc(size); }