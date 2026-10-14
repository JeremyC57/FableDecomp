// CShaderResourceArray::CShaderResourceArray(const CShaderResourceArray&) -- EH-framed copy constructor: the out-of-line base copy (@ 0x00C0DAB0, state 0 after it), the item array at +4 zeroed (inline constructor, state 1), resized to the source count (@ 0x00C31810), then every element copied through the element accessor (@ 0x00C2EBC0) on both arrays; the source array's address is parked in the dead argument slot across the loop
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3B710, oracle length 140 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push ebx ; push ebp ; mov ebp,[other] ; push esi ; push edi ; mov edi,ecx ; push ebp ; mov [esp+14h],edi ; call CBase::CBase ; xor esi,esi ; lea ebx,[edi+4] ; mov [esp+1Ch],esi ; mov [ebx],esi ; mov [ebx+4],esi ; mov eax,[ebp+8] ; push eax ; mov ecx,ebx ; mov byte ptr [esp+20h],1 ; call CItems::Resize ; cmp [edi+8],esi ; jbe done ; add ebp,4 ; mov [other],ebp ; loop: mov ecx,[other] ; push esi ; call CItems::At ; push esi ; mov ecx,ebx ; mov ebp,eax ; call CItems::At ; mov ecx,[ebp] ; mov [eax],ecx ; mov eax,[edi+8] ; inc esi ; cmp esi,eax ; jb loop ; done: mov ecx,[esp+14h] ; mov eax,edi ; ...
// The count is re-read from the destination each iteration. The manifest label (`Assign`) is
// propagated for the copy constructor (it returns `this`).
struct CShaderResourceBase_00c3b710 {      // 4 bytes
    unsigned char _body[4];
    CShaderResourceBase_00c3b710(const CShaderResourceBase_00c3b710& other);   // 0x00C0DAB0
    ~CShaderResourceBase_00c3b710();
};

struct CResourceItems_00c3b710 {           // +4: {items, count}
    void** m_pItems;
    unsigned long m_count;
    CResourceItems_00c3b710() : m_pItems(0), m_count(0) { }
    ~CResourceItems_00c3b710();
    void Resize(unsigned long count);      // 0x00C31810
    void** At(unsigned long index);        // 0x00C2EBC0
};

struct CShaderResourceArray : CShaderResourceBase_00c3b710 {
    CResourceItems_00c3b710 m_items;       // +4
    CShaderResourceArray(const CShaderResourceArray& other);
};

CShaderResourceArray::CShaderResourceArray(const CShaderResourceArray& other)
    : CShaderResourceBase_00c3b710(other), m_items()
{
    m_items.Resize(other.m_items.m_count);
    for (unsigned long i = 0; i < m_items.m_count; ++i)
        *m_items.At(i) = *const_cast<CResourceItems_00c3b710&>(other.m_items).At(i);
}
