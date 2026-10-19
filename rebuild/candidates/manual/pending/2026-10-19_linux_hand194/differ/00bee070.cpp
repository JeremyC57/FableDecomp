// Grade: DIFFER(ours v retail) -- 206v206: a pure callee-saved register swap (retail esi = result, edi = first; ours the reverse) plus the eax/ecx/edx rotation that follows from it; same instructions otherwise (known lane gap shape). Tried: implicit pair assignment, explicit inline pair operator=.
// _global::__pop_heap(SSurfacePair* first, SSurfacePair* last, SSurfacePair* result, SSurfacePair value, CKeyPairCompareLess comp, long*) -- SGI pop_heap core for a heap of (long, CCompositeSurfaceInfo) pairs: *result = *first, then sift `value` down from the root
// Leaf: __pop_heap
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BEE070, 206 bytes, `ret 1Ch` (result, the 0x10-byte value, the comparator and the
//   distance pointer on the stack; first/last in ecx/edx -- the SGI algorithm TUs are /Gr). The catalog label
//   "CItem::operator=" is propagated: the copied value stores vtable 0x012A8154 = CCompositeSurfaceInfo (rtti-vtables.tsv)
//   and the callee 0x00BEDD70 is labelled `_Adjust_heap<pair<long,CCountedPointer<...>>*,...>`; callers 0x00BEE1FF,
//   0x00BEE40F, 0x00BEE9DA, 0x00BEED7E.
// Callees: the old count block's destroy (+4, object at +8 in ecx), operator delete @ 0x00BFE9BC, __adjust_heap @
//   0x00BEDD70, CBaseClass::~CBaseClass @ 0x0099A2E0 (the by-value value's surface info).
// Layout: Ego_r.pdb CCompositeSurfaceInfo (0xc bytes: CBaseClass vptr, Cache CCountedPointer<CCompositeSurfaceCache> +4;
//   its default ctor 0x00BED440 is landed); the pair is {long first; CCompositeSurfaceInfo second} (16 bytes).
// Shape: the implicit pair assignment copies the key and runs the counted pointer's inline operator= (source Data and
//   Info read first, the old block released with the count re-read); the value copy for __adjust_heap is the implicit
//   copy ctor (key, vtable, Data, Info, count up); the by-value parameter dies here: Cache released and cleared, then
//   the base destructor.
void __cdecl operator delete(void* block);         // 0x00BFE9BC

struct SCountBlock {
    long  m_references;                            // +0
    void (__fastcall* m_pfnDestroy)(void*);        // +4 per-type deleter, object in ecx
    void* m_pObject;                               // +8
};
struct CCompositeSurfaceCache;
struct CCountedPointer {                           // CCountedPointer<CCompositeSurfaceCache>
    CCompositeSurfaceCache* Data;
    SCountBlock*            Info;
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->m_references;
    }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->m_references;
            if (Info->m_references == 0) {
                Info->m_pfnDestroy(Info->m_pObject);
                operator delete(Info);
            }
        }
        Data = 0;
        Info = 0;
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCompositeSurfaceCache* data = other.Data;
        SCountBlock* info = other.Info;
        if (Info != info) {
            if (Info) {
                --Info->m_references;
                if (Info->m_references == 0) {
                    Info->m_pfnDestroy(Info->m_pObject);
                    operator delete(Info);
                }
            }
            Data = data;
            Info = info;
            if (info)
                ++info->m_references;
        }
        return *this;
    }
};
struct CBaseClass {
    virtual void v00();
    ~CBaseClass();                                 // 0x0099A2E0
};
struct CCompositeSurfaceInfo : CBaseClass {        // vtable 0x012A8154, 0xc bytes
    virtual void v00();
    CCountedPointer Cache;                         // +0x04
};
struct SSurfacePair {                              // std::pair<long, CCompositeSurfaceInfo>, 16 bytes
    long                  first;                   // +0x00
    CCompositeSurfaceInfo second;                  // +0x04
};
struct CKeyPairCompareLess { };                    // CKeyPairCompareLess<long, CCompositeSurfaceInfo>, empty, by value

void __fastcall __adjust_heap(SSurfacePair* first, long holeIndex, long len, SSurfacePair value,
                              CKeyPairCompareLess comp);   // 0x00BEDD70

void __fastcall __pop_heap(SSurfacePair* first, SSurfacePair* last, SSurfacePair* result, SSurfacePair value,
                           CKeyPairCompareLess comp, long*)
{
    *result = *first;
    __adjust_heap(first, 0, long(last - first), value, comp);
}
