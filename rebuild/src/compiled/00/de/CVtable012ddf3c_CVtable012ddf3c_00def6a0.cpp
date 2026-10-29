// CVtable_012ddf3c::CVtable_012ddf3c(CScript* owner, void* argument, const CScriptThing& thing) -- script-module object constructor: context from the owner, a copied CScriptThing, the owner, three cleared words, a default CScriptThing, then the argument
// Grade: RELOCATION_MATCH (retry 10, RTM 3077) -- the three zero words at +0x1c modelled as a 12-byte STLport vector (start, finish, alloc-proxy end with its own constructor): `mov ecx,edi` (the +0x28 CScriptThing's this) schedules between the second and third zero store as in retail
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DEF6A0, length 91 bytes (+5 int3 padding).
// Leaf: ??0CVtable_012ddf3c
// Retail bytes:  push ebx ; push esi ; push edi ; mov edi,[esp+10h] ; mov eax,[edi+40h] ; mov esi,ecx ; mov [esi+4],eax ;
//                mov eax,[esp+18h] ; push eax ; lea ecx,[esi+8] ; mov dword ptr [esi],012C3224h ; call CScriptThing(const&) ;
//                mov [esi+14h],edi ; mov dword ptr [esi],012DDF3Ch ; xor ebx,ebx ; lea edi,[esi+28h] ; mov [esi+1Ch],ebx ;
//                mov [esi+20h],ebx ; mov ecx,edi ; mov [esi+24h],ebx ; call CBaseClass::CBaseClass ; mov ecx,[esp+14h] ;
//                mov dword ptr [edi],01238C8Ch ; mov [edi+4],ebx ; mov [edi+8],ebx ; pop edi ; mov [esi+18h],ecx ;
//                mov eax,esi ; pop esi ; pop ebx ; ret 0Ch
// Manifest label "CRenderManagerCore::CRenderManagerCore" is propagated and wrong: the vtables are those of the script
// module (no RTTI: 0x012DDF3C derived, 0x012C3224 its base, the same base as the landed script factories such as
// 0x00D62360) and 0x01238C8C is CScriptThing (rtti-vtables.tsv). Callees: CScriptThing copy constructor @ 0x004ABE90,
// CBaseClass::CBaseClass @ 0x0099A2D0 (the root of the default-constructed CScriptThing at +0x28).
// Shape: the context word precedes the base vtable store (root class, its own vtable store dead); the owner store
// precedes the derived vtable (plain initialiser of the derived class); the three zero words follow it; the
// default CScriptThing at +0x28 is an inline constructor over the out-of-line CBase; the argument is stored last
// (a body assignment after the member constructors).
class CBaseClass {
public:
    CBaseClass();                               // 0x0099A2D0
    virtual void v00();
};

// The two words of a CScriptThing are zeroed AFTER its vtable store: they are a member object (a counted
// reference: object + count block) with its own inline constructor.
struct CScriptThingRef {
    void* m_pObject;                       // +0x00
    void* m_pCount;                        // +0x04
    CScriptThingRef() : m_pObject(0), m_pCount(0) { }
};

class CScriptThing : public CBaseClass { public:              // vtable 0x01238C8C, 0x0C bytes
    CScriptThingRef m_ref;                 // +0x04
    virtual void v00();
    CScriptThing() { }
    CScriptThing(const CScriptThing& other);   // 0x004ABE90
};

struct CScript {
    unsigned char _pad[0x40];
    void* m_pContext;                      // +0x40
};

// The context store precedes the base vtable store, so it belongs to a root class whose inline
// constructor is inlined here (its own vtable store is dead and dropped).
struct CVtable_012c3224_Root {
    virtual void v00();
    void* m_pContext;                      // +0x04
    CVtable_012c3224_Root(void* context) : m_pContext(context) { }
};

struct CVtable_012c3224 : CVtable_012c3224_Root {
    virtual void v00();
    CScriptThing m_thing;                  // +0x08
    CVtable_012c3224(CScript* owner, const CScriptThing& thing) : CVtable_012c3224_Root(owner->m_pContext), m_thing(thing) { }
};

// Three words cleared after the derived vtable store: a member object with an inline constructor.
template<class T> struct CArrayAllocProxy {   // STLport _STLP_alloc_proxy: the end-of-storage pointer with its own constructor
    T* p;
    CArrayAllocProxy(T* v) : p(v) { }
};
struct CStateWords {                       // a 12-byte STLport vector: start, finish, alloc-proxy end
    long*                  _M_start;       // +0x00
    long*                  _M_finish;      // +0x04
    CArrayAllocProxy<long> _M_end_of_storage;   // +0x08
    CStateWords() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
};

struct CVtable_012ddf3c : CVtable_012c3224 {
    virtual void v00();
    CScript* m_pOwner;                     // +0x14
    void* m_pArgument;                     // +0x18
    CStateWords m_state;                   // +0x1C
    CScriptThing m_target;                 // +0x28
    CVtable_012ddf3c(CScript* owner, void* argument, const CScriptThing& thing);
};

CVtable_012ddf3c::CVtable_012ddf3c(CScript* owner, void* argument, const CScriptThing& thing)
    : CVtable_012c3224(owner, thing), m_pOwner(owner)
{
    m_pArgument = argument;
}