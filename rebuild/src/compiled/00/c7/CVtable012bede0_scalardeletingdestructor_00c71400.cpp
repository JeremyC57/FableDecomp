// CVtable_012bede0::`scalar deleting destructor'(unsigned int flags) -- compiler-generated: the destructor (0x00C70BB0), then the library's CPKObject::operator delete (0x00C3C370) when bit 0 of the flags is set; returns `this`
// Leaf: ??_GCVtable_012bede0@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C71400, 28 bytes (+ int3 padding), thiscall `ret 4`.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 444 "engine_api CLandscapeBackgroundPatch 0xC70A30/0xC71400/0xC70120/0xC70B00
//   are the voice effects' dtors/ctors"):
//   - slot 0 of the mixer voice-effect vtable 0x012BEDE0 (agent444's CVtable_012bede0: the landed 0x00C71120 UpdateVoices is
//     slot 1 of the same table), stored by the landed constructor 0x00C70B00 (called once, from 0x00C55393);
//   - it calls the destructor 0x00C70BB0 (the vtable store, then a tail jump to the CVtable_012beda4 destructor
//     0x00C6E630) and frees through 0x00C3C370, the libpk root's class operator delete: CVtable_012beda4 derives from
//     the CPKObject root (vtable 0x012B5868).
//   The CLandscapeBackgroundPatch vector deleting destructor (catalog engine_api.tsv) is a propagated label over a
//   `T::VecDel` placeholder; the class is named after its vtable (no-RTTI module), as the landed siblings are.
// Shape: the compiler-generated `??_G`; the vtable (and so `??_G`) is emitted by the out-of-line factory below, the
//   destructor stays an out-of-line call.
class CPKObject {                                       // vtable 0x012B5868: the library's root class
public:
    virtual ~CPKObject() { }
    static void* __cdecl operator new(unsigned int size);   // 0x00C3C360
    static void __cdecl operator delete(void* p);           // 0x00C3C370
};

class CVtable_012beda4 : public CPKObject {             // vtable 0x012BEDA4: the mixer voice-effect base
public:
    CVtable_012beda4();                                 // 0x00C6E6A0
    ~CVtable_012beda4();                                // 0x00C6E630
    unsigned char _pad04[0x24];
};

class CVtable_012bede0 : public CVtable_012beda4 {     // vtable 0x012BEDE0
public:
    CVtable_012bede0() { }                             // out of line at 0x00C70B00
    ~CVtable_012bede0();                               // 0x00C70BB0
};

CVtable_012bede0* Create_CVtable_012bede0()
{
    return new CVtable_012bede0;
}
