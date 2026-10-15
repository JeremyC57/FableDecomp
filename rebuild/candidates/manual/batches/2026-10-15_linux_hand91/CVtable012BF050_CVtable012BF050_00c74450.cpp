// CVtable_012BF050::~CVtable_012BF050() -- base destructor: drops one reference on the class-wide shared table and frees the table with the last reference
// Leaf: ??1CVtable_012BF050
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C74450, length 39 bytes (+ int3 padding).
// Callee: FreeSharedTable 0x00C763D0 (__cdecl; a `jmp` to the module's free hook 0x00C3C370).
// Catalog label "CTCHeroStats::~CTCHeroStats" is propagated (byte-identical bodies). The class is a
// no-RTTI base (vtable 0x012BF050) whose constructor 0x00C74420 bumps the reference count
// 0x0143CCE0 or builds the shared table 0x0143CCDC (0x00C74360) for the first instance; 23 derived
// destructors in 0x00C69xxx/0x00C6Axxx tail-jump here. 0x00C743F0 is the same release as a static
// function, 0x00C74480 the deleting destructor.
// Shape: polymorphic class (vptr store first) with a NON-virtual destructor so the TU's largest
// symbol is `??1` (a virtual destructor adds `??_G`); `if (--s_refCount == 0)` folds into
// `dec dword ptr [g] ; jne`; the pointer is cleared after the free.
void __cdecl FreeSharedTable(void* table);     // 0x00C763D0

struct CVtable_012BF050 {
    virtual void v00();
    ~CVtable_012BF050();
    static void* s_pSharedTable;                // 0x0143CCDC
    static long  s_refCount;                    // 0x0143CCE0
};

CVtable_012BF050::~CVtable_012BF050()
{
    if (--s_refCount == 0) {
        FreeSharedTable(s_pSharedTable);
        s_pSharedTable = 0;
    }
}
