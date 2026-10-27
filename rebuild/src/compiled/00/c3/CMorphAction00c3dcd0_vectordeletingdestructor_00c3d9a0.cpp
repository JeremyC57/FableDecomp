// CMorphAction_00c3dcd0::`vector deleting destructor'(unsigned int flags) -- compiler-generated: for an array (bit 1) destroy every 0x4C-byte element through `eh vector destructor iterator` and free the block from its count word, else destroy this one; free through CPKObject::operator delete when bit 0 is set
// Leaf: ??_ECMorphAction_00c3dcd0@@UAEPAXI@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C3D9A0, 84 bytes, thiscall `ret 4`.
// Identity: slot 0 of vtable 0x012B5A80, the pooled character-morph action whose constructor 0x00C3CE50 is landed as
//   CMorphAction_00c3dcd0::CMorphAction_00c3dcd0 (base CBase_00c4cf20, +0x48 = 0x80000000; one retail address, one name)
//   and whose destructor is 0x00C3D7B0 (stores 0x012B5A80, resets +0x48, tail-jumps to the base destructor 0x00C4CF40).
//   The factory 0x00C3DFD0 builds arrays of it with `eh vector constructor iterator`, so the slot is `??_E`. Both frees
//   go to 0x00C3C370 (CPKObject::operator delete; the libpk root's operator delete[] forwards to it inline).
// Callees: ~CMorphAction_00c3dcd0 0x00C3D7B0, `eh vector destructor iterator` 0x00BFEED8, CPKObject::operator delete
//   0x00C3C370.
// Shape: the compiler-generated `??_E`, emitted by the array-new factory below (a model device, not graded).
class CPKObject {                                         // vtable 0x012B5868: the library's root class
public:
    virtual ~CPKObject() { }
    static void* __cdecl operator new(unsigned int size);     // 0x00C3C360
    static void __cdecl operator delete(void* p);             // 0x00C3C370
    static void __cdecl operator delete[](void* p) { operator delete(p); }   // inline: the array free reaches 0x00C3C370 too
};

class CBase_00c4cf20 : public CPKObject {                 // 0x48 bytes (vptr, prev +4, next +8, ...)
public:
    CBase_00c4cf20();                                     // 0x00C4CF20
    void* Field04;                                        // +0x04
    void* Field08;                                        // +0x08
    unsigned char _pad0C[0x3C];                           // +0x0C
};

class CMorphAction_00c3dcd0 : public CBase_00c4cf20 {     // vtable 0x012B5A80, 0x4C bytes
public:
    CMorphAction_00c3dcd0();                              // 0x00C3CE50
    virtual ~CMorphAction_00c3dcd0();                     // 0x00C3D7B0
    long m_48;                                            // +0x48
};

CMorphAction_00c3dcd0* CreateMorphActions_00c3d9a0(unsigned int count)
{
    return new CMorphAction_00c3dcd0[count];
}