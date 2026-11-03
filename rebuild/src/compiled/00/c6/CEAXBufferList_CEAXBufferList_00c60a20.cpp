// CEAXBufferList::~CEAXBufferList() -- destructor of a node of the EAX context's source list: nothing to release, so only the CCodaObjectBase root vtable (0x012B5868) survives from the inlined root destructor
// Leaf: ??1CEAXBufferList@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C60A20, 7 bytes: `mov dword ptr [ecx],12B5868h ; ret`.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 442 "CLandscapeBackgroundPatch ??_G 0xC60AD0/0xC60A00 = context/node deleting
//   dtors"): its only caller is the node's deleting destructor 0x00C60A00, slot 0 of the CEAXBufferList vtable 0x012B9338
//   (stored by agent442's CEAXBufferList::CEAXBufferList 0x00C5FF50). The node's own vtable store is dead (overwritten by
//   the root's) and is dropped. The landed T::~T / "CBase" names are placeholders.
//   agent442 (FINDINGS chunk 442) names the software EAX 4.0 layer: CEAXListener (vtable 0x012B9340, destructor 0x00C60930),
//   its source records CEAXBuffer (vtable 0x012B9344: context +0x14, dirty mask +0x368; CEAXBuffer::Set 0x00C61B40) and the
//   context's source-list nodes CEAXBufferList (vtable 0x012B9338, ctor 0x00C5FF50, 0xc bytes), all on the library root
//   CCodaObjectBase (vtable 0x012B5868, operator new 0x00C3C360 / delete 0x00C3C370). Descriptive names (libpk: no PDB/RTTI).
extern "C" void* __cdecl CodaMalloc(unsigned int size);   // 0x00C3C360 (GameCODA Memory.obj)
extern "C" void __cdecl CodaFree(void* p);                // 0x00C3C370
class CCodaObjectBase {                                   // vtable 0x012B5868: the library's root class
public:
    virtual ~CCodaObjectBase() { }
    static void* __cdecl operator new(unsigned int size) { return CodaMalloc(size); }   // 0x00C3C360
    static void __cdecl operator delete(void* p) { CodaFree(p); }           // 0x00C3C370
};

class CEAXBuffer;

class CEAXBufferList : public CCodaObjectBase {           // vtable 0x012B9338, 0xc bytes
public:
    CEAXBufferList*   m_pNext;                      // +0x04
    CEAXBuffer*       m_pSource;                    // +0x08
    ~CEAXBufferList();
};

CEAXBufferList::~CEAXBufferList()
{
}
