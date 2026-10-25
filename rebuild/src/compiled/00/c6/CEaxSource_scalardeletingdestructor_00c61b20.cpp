// CEaxSource::`scalar deleting destructor'(unsigned int flags) -- run ~CEaxSource (0x00C60B90, out of line) and, when bit 0 of the flags is set, return the block to the library allocator (CPKObject::operator delete 0x00C3C370); returns this
// Leaf: ??_GCEaxSource@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C61B20, 28 bytes, thiscall `ret 4`: `push esi ; mov esi,ecx ; call ~CEaxSource ; test byte ptr
//   [esp+8],1 ; je ; push esi ; call operator delete ; add esp,4 ; mov eax,esi ; pop esi ; ret 4`.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 442): slot 0 of the CEaxSource vtable 0x012B9344; the destructor it calls is 0x00C60B90.
//   The landed row was a `__fastcall` free-function stand-in named CConsoleVariable_scalar_deleting_destructor (CConsoleVariable).
//   agent442 (FINDINGS chunk 442) names the software EAX 4.0 layer: CEaxContext (vtable 0x012B9340, destructor 0x00C60930),
//   its source records CEaxSource (vtable 0x012B9344: context +0x14, dirty mask +0x368; CEaxSource::Set 0x00C61B40) and the
//   context's source-list nodes CEaxSourceNode (vtable 0x012B9338, ctor 0x00C5FF50, 0xc bytes), all on the library root
//   CPKObject (vtable 0x012B5868, operator new 0x00C3C360 / delete 0x00C3C370). Descriptive names (libpk: no PDB/RTTI).
// Shape: the compiler-generated `??_G`; the vtable (and so `??_G`) is emitted by the out-of-line factory below, the
//   destructor itself stays external (retail calls it, VC7.1 does not inline into `??_G`).
class CPKObject {                                   // vtable 0x012B5868: the library's root class
public:
    virtual ~CPKObject() { }
    static void* __cdecl operator new(unsigned int size);   // 0x00C3C360
    static void __cdecl operator delete(void* p);           // 0x00C3C370
};

class CEaxSource : public CPKObject {
public:
    CEaxSource() { }
    ~CEaxSource();                                          // 0x00C60B90
    unsigned char _pad04[0x10];                     // +0x04
    void*         m_pContext;                       // +0x14 (CEaxContext*)
};

CEaxSource* Create_CEaxSource()
{
    return new CEaxSource;
}
