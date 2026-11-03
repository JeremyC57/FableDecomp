// CEAXBuffer::~CEAXBuffer() -- EH-framed destructor of a software EAX source: its vtable (0x012B9344), unregister from the owning context (+0x14, CEAXListener::RemoveSource 0x00C5FF60), then the CCodaObjectBase root vtable (0x012B5868) from its inlined empty destructor
// Leaf: ??1CEAXBuffer@@UAE@XZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C60B90, 74 bytes, thiscall, plain `ret`.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 442 "CConsoleVariable 0xC60B90/0xC61B20 = EAX source (vtable 0x12B9344)"):
//   the vtable it stores is the one whose slot 0 is the deleting destructor 0x00C61B20 (`call 0x00C60B90` then
//   CCodaObjectBase::operator delete 0x00C3C370), and the call is agent442's landed CEAXListener::RemoveSource 0x00C5FF60 (walks the
//   context's CEAXBufferList list for this source and deletes its node) on the +0x14 context pointer that CEAXBuffer::Set
//   0x00C61B40 also uses. The CConsoleVariable / CConsoleVariableRegistry names are propagated game labels.
//   agent442 (FINDINGS chunk 442) names the software EAX 4.0 layer: CEAXListener (vtable 0x012B9340, destructor 0x00C60930),
//   its source records CEAXBuffer (vtable 0x012B9344: context +0x14, dirty mask +0x368; CEAXBuffer::Set 0x00C61B40) and the
//   context's source-list nodes CEAXBufferList (vtable 0x012B9338, ctor 0x00C5FF50, 0xc bytes), all on the library root
//   CCodaObjectBase (vtable 0x012B5868, operator new 0x00C3C360 / delete 0x00C3C370). Descriptive names (libpk: no PDB/RTTI).
// Shape: EH state 0 covers the body (the root is alive and has a non-trivial inline destructor whose only effect is
//   restoring its vtable); `this` is kept in the frame for the unwind funclet.
extern "C" void* __cdecl CodaMalloc(unsigned int size);   // 0x00C3C360 (GameCODA Memory.obj)
extern "C" void __cdecl CodaFree(void* p);                // 0x00C3C370
class CCodaObjectBase {                                   // vtable 0x012B5868: the library's root class
public:
    virtual ~CCodaObjectBase() { }
    static void* __cdecl operator new(unsigned int size) { return CodaMalloc(size); }   // 0x00C3C360
    static void __cdecl operator delete(void* p) { CodaFree(p); }           // 0x00C3C370
};

class CEAXBuffer;

class CEAXListener : public CCodaObjectBase {              // vtable 0x012B9340
public:
    long RemoveSource(CEAXBuffer* source);          // 0x00C5FF60
};

class CEAXBuffer : public CCodaObjectBase {               // vtable 0x012B9344
public:
    unsigned char _pad04[0x10];                     // +0x04
    CEAXListener*  m_pContext;                       // +0x14
    ~CEAXBuffer();
};

CEAXBuffer::~CEAXBuffer()
{
    m_pContext->RemoveSource(this);
}
