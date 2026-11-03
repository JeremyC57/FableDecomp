// CEAXListener::`scalar deleting destructor'(unsigned int flags) -- run ~CEAXListener (0x00C60930, out of line) and, when bit 0 of the flags is set, return the block to the library allocator (CCodaObjectBase::operator delete 0x00C3C370); returns this
// Leaf: ??_GCEAXListener@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C60AD0, 28 bytes, thiscall `ret 4`: `push esi ; mov esi,ecx ; call ~CEAXListener ; test byte ptr
//   [esp+8],1 ; je ; push esi ; call operator delete ; add esp,4 ; mov eax,esi ; pop esi ; ret 4`.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 442): slot 0 of the CEAXListener vtable 0x012B9340; the destructor it calls is 0x00C60930.
//   The landed row was a `__fastcall` free-function stand-in named CLandscapeBackgroundPatch_vector_deleting_destructor.
//   agent442 (FINDINGS chunk 442) names the software EAX 4.0 layer: CEAXListener (vtable 0x012B9340, destructor 0x00C60930),
//   its source records CEAXBuffer (vtable 0x012B9344: context +0x14, dirty mask +0x368; CEAXBuffer::Set 0x00C61B40) and the
//   context's source-list nodes CEAXBufferList (vtable 0x012B9338, ctor 0x00C5FF50, 0xc bytes), all on the library root
//   CCodaObjectBase (vtable 0x012B5868, operator new 0x00C3C360 / delete 0x00C3C370). Descriptive names (libpk: no PDB/RTTI).
// Shape: the compiler-generated `??_G`; the vtable (and so `??_G`) is emitted by the out-of-line factory below, the
//   destructor itself stays external (retail calls it, VC7.1 does not inline into `??_G`).
extern "C" void* __cdecl CodaMalloc(unsigned int size);   // 0x00C3C360 (GameCODA Memory.obj)
extern "C" void __cdecl CodaFree(void* p);                // 0x00C3C370
class CCodaObjectBase {                                   // vtable 0x012B5868: the library's root class
public:
    virtual ~CCodaObjectBase() { }
    static void* __cdecl operator new(unsigned int size) { return CodaMalloc(size); }   // 0x00C3C360
    static void __cdecl operator delete(void* p) { CodaFree(p); }           // 0x00C3C370
};

class CEAXListener : public CCodaObjectBase {
public:
    CEAXListener() { }
    ~CEAXListener();                                          // 0x00C60930
    long          m_primarySlot;                    // +0x004
    void*         m_pSources;                       // +0x008 (CEAXBufferList*)
};

CEAXListener* Create_CEAXListener()
{
    return new CEAXListener;
}
