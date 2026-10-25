// CEaxSource::~CEaxSource() -- EH-framed destructor of a software EAX source: its vtable (0x012B9344), unregister from the owning context (+0x14, CEaxContext::RemoveSource 0x00C5FF60), then the CPKObject root vtable (0x012B5868) from its inlined empty destructor
// Leaf: ??1CEaxSource@@UAE@XZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C60B90, 74 bytes, thiscall, plain `ret`.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 442 "CConsoleVariable 0xC60B90/0xC61B20 = EAX source (vtable 0x12B9344)"):
//   the vtable it stores is the one whose slot 0 is the deleting destructor 0x00C61B20 (`call 0x00C60B90` then
//   CPKObject::operator delete 0x00C3C370), and the call is agent442's landed CEaxContext::RemoveSource 0x00C5FF60 (walks the
//   context's CEaxSourceNode list for this source and deletes its node) on the +0x14 context pointer that CEaxSource::Set
//   0x00C61B40 also uses. The CConsoleVariable / CConsoleVariableRegistry names are propagated game labels.
//   agent442 (FINDINGS chunk 442) names the software EAX 4.0 layer: CEaxContext (vtable 0x012B9340, destructor 0x00C60930),
//   its source records CEaxSource (vtable 0x012B9344: context +0x14, dirty mask +0x368; CEaxSource::Set 0x00C61B40) and the
//   context's source-list nodes CEaxSourceNode (vtable 0x012B9338, ctor 0x00C5FF50, 0xc bytes), all on the library root
//   CPKObject (vtable 0x012B5868, operator new 0x00C3C360 / delete 0x00C3C370). Descriptive names (libpk: no PDB/RTTI).
// Shape: EH state 0 covers the body (the root is alive and has a non-trivial inline destructor whose only effect is
//   restoring its vtable); `this` is kept in the frame for the unwind funclet.
class CPKObject {                                   // vtable 0x012B5868: the library's root class
public:
    virtual ~CPKObject() { }
    static void* __cdecl operator new(unsigned int size);   // 0x00C3C360
    static void __cdecl operator delete(void* p);           // 0x00C3C370
};

class CEaxSource;

class CEaxContext : public CPKObject {              // vtable 0x012B9340
public:
    long RemoveSource(CEaxSource* source);          // 0x00C5FF60
};

class CEaxSource : public CPKObject {               // vtable 0x012B9344
public:
    unsigned char _pad04[0x10];                     // +0x04
    CEaxContext*  m_pContext;                       // +0x14
    ~CEaxSource();
};

CEaxSource::~CEaxSource()
{
    m_pContext->RemoveSource(this);
}
