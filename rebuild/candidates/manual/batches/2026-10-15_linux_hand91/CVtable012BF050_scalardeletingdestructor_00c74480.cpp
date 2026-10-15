// CVtable_012BF050::`scalar deleting destructor'(unsigned int flags) -- runs the inlined base destructor (shared-table release) and frees the object when bit 0 of the flags is set
// Leaf: ??_GCVtable_012BF050
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C74480, length 63 bytes.
// Callees: FreeSharedTable 0x00C763D0 (__cdecl), operator delete 0x00BFE9BC.
// Catalog label "CTCHeroStats::~CTCHeroStats" is propagated. Slot 0 of the no-RTTI vtable
// 0x012BF050 (the only reference to the body is that vtable slot); the destructor 0x00C74450 is
// inlined: vptr store, `dec [0x0143CCE0] ; jne`, free + clear of 0x0143CCDC.
// Shape: a `virtual` destructor plus a `new T` factory makes VC7.1 emit `??_G` in this TU; the
// `test byte ptr [esp+8],1` guard and the `mov eax,esi` return are the standard scalar deleting
// destructor.
void __cdecl FreeSharedTable(void* table);     // 0x00C763D0

struct CVtable_012BF050 {
    virtual ~CVtable_012BF050();
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

CVtable_012BF050* NewCVtable_012BF050() { return new CVtable_012BF050; }
