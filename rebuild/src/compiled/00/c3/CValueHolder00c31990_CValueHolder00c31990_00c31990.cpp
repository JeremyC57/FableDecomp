// CValueHolder_00c31990::CValueHolder_00c31990(void* value) -- one-argument constructor of a two-word record: clears the first word and keeps the argument at +4
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C31990, length 18 bytes.
// Leaf: ??0CValueHolder_00c31990
// Retail bytes: mov eax,ecx ; mov ecx,[esp+4] ; mov dword ptr [eax],0 ; mov [eax+4],ecx ; ret 4
// The manifest label "CSBAgents::CSBAgents" is propagated over byte-identical constructor bodies of many
// classes. The first store is an immediate zero (the retail image is fixed-base without a .reloc table,
// but no vtable can sit at address 0), so this is a plain two-word record: the first word zeroed and the
// argument at +4, both through the initialiser list in declaration order, `this` returned in eax.
struct CValueHolder_00c31990 {
    void* m_pFirst;                        // +0x00 cleared
    void* m_pValue;                        // +0x04
    CValueHolder_00c31990(void* value);
};

CValueHolder_00c31990::CValueHolder_00c31990(void* value)
    : m_pFirst(0), m_pValue(value)
{
}