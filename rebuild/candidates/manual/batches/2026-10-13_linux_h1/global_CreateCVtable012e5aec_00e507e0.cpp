// CCountedPointer<CVtable_012e5aec> Create_CVtable_012e5aec(owner, arg, const CScriptThing&) -- new CVtable_012e5aec(owner, thing, arg) handed to a counted pointer (count block + deleter 0x00CDEE00)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E507E0, oracle length 145 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ebx ; push esi ; push edi ; push 0x24 ; mov ebx,edx ; mov edi,ecx ; call operator new ; mov esi,eax ; add esp,4 ; test esi,esi ; je Z ; mov eax,[ebx+0x40] ; mov [esi+4],eax ; mov eax,[esp+14h] ; push eax ; lea ecx,[esi+8] ; mov dword ptr [esi],offset CVtable_012c3224::`vftable' ; call CScriptThing::CScriptThing(const&) ; mov ecx,[esp+10h] ; mov dword ptr [esi],offset CVtable_012e5aec::`vftable' ; mov [esi+0x14],ebx ; mov [esi+0x18],ecx ; jmp P ; Z: xor esi,esi ; P: test esi,esi ; mov [edi],esi ; je N ; push 0Ch ; call operator new ; add esp,4 ; test eax,eax ; je Q ; mov ecx,[edi] ; mov dword ptr [eax],1 ; mov dword ptr [eax+4],offset Destroy ; mov [eax+8],ecx ; mov [edi+4],eax ; mov eax,edi ; (pops) ; ret 8 ; Q: xor eax,eax ; mov [edi+4],eax ; ... ; N: mov dword ptr [edi+4],0 ; ...
// Script-side factory (no-RTTI module): a __fastcall function returning a CCountedPointer by
// value (hidden result pointer in ecx, the owning script in edx, then on the stack one argument
// and the CScriptThing by reference). It allocates sizeof(CVtable_012e5aec) = 0x24, runs the inlined two-level
// constructor (base CVtable_012c3224: context pointer copied from owner+0x40 and the CScriptThing
// copy-constructed @ 0x004ABE90; derived CVtable_012e5aec: owner and argument), then builds the
// 12-byte count block {1, deleter 0x00CDEE00, object} exactly as the counted-pointer
// constructor family does. Vtables are named by address (the module carries no RTTI).
// One of 557 bodies of this shape.
struct CScriptThing {
    unsigned char _body[12];
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
    void* m_pContext;                      // +4
    CVtable_012c3224_Root(void* context) : m_pContext(context) { }
};

struct CVtable_012c3224 : CVtable_012c3224_Root {
    virtual void v00();
    CScriptThing m_thing;                  // +8
    CVtable_012c3224(CScript* owner, const CScriptThing& thing) : CVtable_012c3224_Root(owner->m_pContext), m_thing(thing) { }
};

struct CVtable_012e5aec : CVtable_012c3224 {
    virtual void v00();
    CScript* m_pOwner;                     // +0x14
    void* m_pArgument;                     // +0x18
    unsigned char _tail[0x8];
    CVtable_012e5aec(CScript* owner, const CScriptThing& thing, void* argument) : CVtable_012c3224(owner, thing), m_pOwner(owner), m_pArgument(argument) { }
};

void __fastcall Destroy_00cdee00(void* object);   // 0x00CDEE00

struct SCountBlock {
    long m_references;
    void (__fastcall* m_pfnDestroy)(void*);
    void* m_pObject;
    SCountBlock(void* object) : m_references(1), m_pfnDestroy(Destroy_00cdee00), m_pObject(object) { }
};

struct CCountedPointer {
    void* m_pObject;
    SCountBlock* m_pCount;
    CCountedPointer(void* object) : m_pObject(object)
    {
        if (object)
            m_pCount = new SCountBlock(m_pObject);
        else
            m_pCount = 0;
    }
};

CCountedPointer __fastcall Create_CVtable_012e5aec(CScript* owner, void* argument, const CScriptThing& thing)
{
    return CCountedPointer(new CVtable_012e5aec(owner, thing, argument));
}
