// _Dest_val<allocator<pair<K, CCountedPointer>>> -- release the pair's counted pointer (block count at +0, destroy routine at +4 over the object at +8, block deleted @ 0x00BFE9BC) and null the two fields
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB96C0, oracle length 49 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi+0Ch] ; push edi ; xor edi,edi ; cmp eax,edi ; je N ; dec dword ptr [eax] ; mov eax,[esi+0Ch] ; cmp dword ptr [eax],edi ; jne N ; mov ecx,[eax+8] ; call [eax+4] ; mov eax,[esi+0Ch] ; push eax ; call operator delete ; add esp,4 ; N: mov [esi+8],edi ; mov [esi+0Ch],edi ; pop edi ; pop esi ; ret
// Dinkumware `_Dest_val` over a map value whose second member is a counted pointer (object at +8,
// count block at +0xC): the block's count is decremented and re-read through the field, at zero
// the type-erased destroy routine runs on the object and the block is deleted; both fields are
// nulled (the zero kept in edi). Fastcall: the value in ecx. The manifest label is propagated;
// the key is opaque padding. One of 7 bodies of this shape.
void __cdecl operator delete(void* block);

struct SCountBlock {
    long m_references;                     // +0
    void (__thiscall* m_pfnDestroy)(void*);// +4
    void* m_pObject;                       // +8
};

struct SValue {
    unsigned char _key[8];                 // +0 key
    void* m_pObject;                       // +8
    SCountBlock* m_pCount;                 // +0xC
};

void __fastcall DestroyValue_00cb96c0(SValue* value)
{
    if (value->m_pCount) {
        --value->m_pCount->m_references;
        if (value->m_pCount->m_references == 0) {
            value->m_pCount->m_pfnDestroy(value->m_pCount->m_pObject);
            operator delete(value->m_pCount);
        }
    }
    value->m_pObject = 0;
    value->m_pCount = 0;
}
