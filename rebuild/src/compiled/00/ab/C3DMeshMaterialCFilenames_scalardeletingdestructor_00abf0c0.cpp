// C3DMeshMaterial::CFilenames::`scalar deleting destructor'(unsigned int flags) -- destroy the four CCharStrings at +0x14 down to +8 (0x0099EAE0), the CIVCountedPointeeBase base's inline destructor stores its vtable (0x012354D0), then free for flag bit 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ABF0C0, length 64 bytes.
// Leaf: ??_GCNamedPointee_00abf0c0@@QAEPAXI@Z
// The chunk label is propagated; named by address. Shape: the derived class stores no vtable of its own (its vtable folds into the base's), the base's inline destructor stores the base vtable, `ret 4`.
void __cdecl operator delete(void* block);         // 0x00BFE9BC
class CCharString { public:
    void* m_pStringData;
    ~CCharString();                                // 0x0099EAE0
};
class CIVCountedPointeeBase { public:                     // vtable 0x012354D0
    virtual void Destroy(int flags);
    virtual void Delete();
    unsigned long IVRefCount;                      // +4
    ~CIVCountedPointeeBase() { }                   // inline: the vtable store
};
struct __declspec(novtable) CNamedPointee_00abf0c0 : CIVCountedPointeeBase {
    CCharString m_8;                               // +0x08
    CCharString m_c;                               // +0x0c
    CCharString m_10;                              // +0x10
    CCharString m_14;                              // +0x14
    ~CNamedPointee_00abf0c0() { }
};

void Kill_00abf0c0(CNamedPointee_00abf0c0* object)                      // forces the scalar deleting destructor into the object; not part of the retail body
{
    delete object;
}