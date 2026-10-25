// CDspObject::CDspObject() -- base constructor of every reverb DSP part: vptr, then share the global DSP-ops table (build it for the first user, else bump its reference count)
// Leaf: ??0CDspObject
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C74420, length 37 bytes. Callee: CreateSharedOps 0x00C74360
//   (allocates the x87 or SIMD ops object into 0x0143CCDC and sets the count 0x0143CCE0 to 1).
// Layout (descriptive; libpk reverb module, no PDB/RTTI): vtable 0x012BF050 (slot 0 = the deleting destructor
//   0x00C74480); the base destructor 0x00C74450 drops the reference and frees the table with the last one.
//   Fifteen part constructors (0x00C69663.., the reverb ctor 0x00C6A6AB..) call this body.
// Shape: the ops pointer test reads the static, the `inc dword ptr [count]` arm is the else; both arms return
//   `this`, so VC7.1 keeps two `mov eax,esi ; pop esi ; ret` epilogues.
bool CreateSharedOps();                    // 0x00C74360

struct CDspOps;

struct CDspObject {
    virtual ~CDspObject();                 // 0x00C74480 (deleting) / 0x00C74450
    CDspObject();
    static CDspOps* s_pOps;                // 0x0143CCDC
    static long s_refCount;                // 0x0143CCE0
};

CDspObject::CDspObject()
{
    if (!s_pOps)
        CreateSharedOps();
    else
        ++s_refCount;
}