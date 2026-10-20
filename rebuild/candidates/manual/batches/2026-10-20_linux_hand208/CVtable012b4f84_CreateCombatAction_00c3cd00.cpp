// CVtable_012b4f84::CreateCombatAction(void*) -- takes a pooled object (or news a 0x40-byte one), initialises it, reports a non-zero result, and on success files it in the pool's active list
// Leaf: ?CreateCombatAction@CVtable_012b4f84
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3CD00, 222 bytes (+ int3 padding), plain `ret` (a static).
// Label correction: the catalog files this row under CTCHeroStats (propagated); it lives in the no-RTTI 0x00C3xxxx
// library module and creates the object whose ctor 0x00C3C660 stores vtable 0x012B4F84 over the base 0x012B4F74
// (see SetupEventNotification 0x00C3C4E0), so the classes are named after their vtables; the catalog's member name
// is kept, the real one is unknown.
// Callees: the pool's GetHead() @ 0x00C3C9A0, RemoveHead() @ 0x00C3CA30 and Add(object) @ 0x00C3C9B0 (object
//          @ 0x0143BC00, its signed mode word at +0x0C read directly), the class operator new @ 0x00C3C360 (0x40 bytes;
//          operator delete @ 0x00C3C370 in the unwind funclet), the ctor @ 0x00C3C660, Initialise(void*) @ 0x00C3C6B0,
//          the result log's Report(long) @ 0x00C3D090 (object @ 0x0143BB58), the virtual deleting destructor (slot 0).
// Shape: /EHsc frame for the new-expression (state 0 around the ctor, the raw block spilled for the funclet); a
// non-zero result is reported, a negative one deletes a freshly newed object (pooled ones are left alone) and
// returns 0; success files the object (popping it from the free head first when pooled). All failures share one
// `return 0` tail at the end: the two null tests jump to a label (separate early returns each get their own EH
// epilogue, a merged `if (object)` test loses the per-branch tests).
struct CVtable_012b4f74 {                            // the base (vtable 0x012B4F74)
    virtual ~CVtable_012b4f74();
    static void* __cdecl operator new(unsigned int size);   // 0x00C3C360
    static void __cdecl operator delete(void* block);       // 0x00C3C370
    unsigned char _body_0x04[0x2C];
};

class CVtable_012b4f84 : public CVtable_012b4f74 {  // 0x40 bytes
public:
    unsigned char _body_0x30[0x10];
    CVtable_012b4f84();                              // 0x00C3C660
    virtual ~CVtable_012b4f84();
    long Initialise(void* params);                   // 0x00C3C6B0
    static CVtable_012b4f84* CreateCombatAction(void* params);
};

struct CObjectPool_0143bc00 {
    unsigned char _pad00[0x0C];
    long          Mode;                              // +0x0C (>= 0: pooled)
    CVtable_012b4f84* GetHead();                     // 0x00C3C9A0
    void RemoveHead();                               // 0x00C3CA30
    void Add(CVtable_012b4f84* object);              // 0x00C3C9B0
};
extern CObjectPool_0143bc00 g_objectPool_0143bc00;   // 0x0143BC00

struct CResultLog_0143bb58 {
    void Report(long result);                        // 0x00C3D090
};
extern CResultLog_0143bb58 g_resultLog_0143bb58;     // 0x0143BB58

CVtable_012b4f84* CVtable_012b4f84::CreateCombatAction(void* params)
{
    CVtable_012b4f84* object;
    long result;
    if (g_objectPool_0143bc00.Mode >= 0) {
        CVtable_012b4f84* pooled = g_objectPool_0143bc00.GetHead();
        if (pooled == 0)
            goto failed;
        object = pooled;
    } else {
        object = new CVtable_012b4f84;
        if (object == 0)
            goto failed;
    }
    result = object->Initialise(params);
    if (result)
        g_resultLog_0143bb58.Report(result);
    if (result >= 0) {
        if (g_objectPool_0143bc00.Mode >= 0)
            g_objectPool_0143bc00.RemoveHead();
        g_objectPool_0143bc00.Add(object);
        return object;
    }
    if (g_objectPool_0143bc00.Mode < 0)
        delete object;
failed:
    return 0;
}
