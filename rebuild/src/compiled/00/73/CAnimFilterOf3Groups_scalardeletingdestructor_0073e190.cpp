// CAnimFilterOf3Groups::`scalar deleting destructor'(unsigned int flags) -- destroy the three CCharStrings at +0xc, +8, +4 (0x0099EAE0), the CAnimFilterBase base's inline destructor stores its vtable (0x0122FFCC), then free for flag bit 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0073E190, length 64 bytes.
// Leaf: ??_GCNamedAnimFilter_0073e190@@QAEPAXI@Z
// The chunk label is propagated; named by address. Shape: the derived class stores no vtable of its own (its vtable folds into the base's), the base's inline destructor stores the base vtable, `ret 4`.
void __cdecl operator delete(void* block);         // 0x00BFE9BC
class CCharString { public:
    void* m_pStringData;
    ~CCharString();                                // 0x0099EAE0
};
class CAnimFilterBase { public:                           // vtable 0x0122FFCC
    virtual void v00();
    ~CAnimFilterBase() { }                         // inline: the vtable store
};
struct __declspec(novtable) CNamedAnimFilter_0073e190 : CAnimFilterBase {
    CCharString m_4;                               // +0x04
    CCharString m_8;                               // +0x08
    CCharString m_c;                               // +0x0c
    ~CNamedAnimFilter_0073e190() { }
};

void Kill_0073e190(CNamedAnimFilter_0073e190* object)                      // forces the scalar deleting destructor into the object; not part of the retail body
{
    delete object;
}