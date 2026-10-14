// C3DMeshVolume_00c2d2f0::C3DMeshVolume_00c2d2f0(void*, const C3DVector*, void*, void*, void*, void*) -- EH-framed constructor: the origin copied memberwise from the second argument, three words at +0xc..+0x14 from the third to fifth, an array of two 0x180-byte parts at +0x18 built through `__ehvec_ctor` (state 0), the word at +0x31c from the first argument and +0x318 zeroed, the member at +0x320 built out of line (@ 0x00C2D1A0, state 1), the byte at +0x33c cleared, then every part bound to the sixth argument (@ 0x00C38850)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2D2F0, oracle length 192 bytes. Built under /EHsc.
// Retail bytes:  ... mov eax,[esp+18h] ; mov edx,[eax] ; push ebx ; push ebp ; push esi ; mov esi,ecx ; push edi ; mov [ecx],edx ; mov edx,[eax+4] ; push offset ~CMeshPart ; mov [ecx+4],edx ; mov eax,[eax+8] ; mov edx,[esp+34h] ; push offset CMeshPart::CMeshPart ; push 2 ; mov [ecx+8],eax ; mov ecx,[esp+38h] ; mov eax,[esp+40h] ; push 180h ; lea edi,[esi+18h] ; push edi ; mov [esp+24h],esi ; mov [esi+0Ch],ecx ; mov [esi+10h],edx ; mov [esi+14h],eax ; call __ehvec_ctor ; mov ecx,[esp+24h] ; xor ebx,ebx ; mov [esi+31Ch],ecx ; lea ecx,[esi+320h] ; mov [esp+1Ch],ebx ; mov [esi+318h],ebx ; call CVolumeData::CVolumeData ; mov ebp,[esp+38h] ; mov [esi+33Ch],bl ; mov byte ptr [esp+1Ch],1 ; mov ebx,2 ; loop: push ebp ; mov ecx,edi ; call CMeshPart::Bind ; add edi,180h ; dec ebx ; jne loop ; ...
// The vector copy is a user copy constructor (dword moves); the bind loop is a counted pointer
// walk. The manifest label is a description; the classes are named by address.
struct C3DVector_00c2d2f0 {              // POD: the copy is a plain dword block move
    float x, y, z;
};

struct CMeshPart_00c2d2f0 {                // 0x180 bytes
    unsigned char _body[0x180];
    CMeshPart_00c2d2f0();                  // 0x00C39130
    ~CMeshPart_00c2d2f0();                 // 0x00C38B20
    void Bind(void* owner);                // 0x00C38850
};

struct CVolumeData_00c2d2f0 {              // +0x320
    unsigned char _body[0x1c];
    CVolumeData_00c2d2f0();                // 0x00C2D1A0
    ~CVolumeData_00c2d2f0();
};

struct C3DMeshVolume_00c2d2f0 {
    C3DVector_00c2d2f0 m_origin;           // +0
    void* m_c;
    void* m_10;
    void* m_14;
    CMeshPart_00c2d2f0 m_parts[2];         // +0x18
    long m_318;
    void* m_pOwner;                        // +0x31c
    CVolumeData_00c2d2f0 m_data;           // +0x320
    bool m_33c;
    C3DMeshVolume_00c2d2f0(void* owner, const C3DVector_00c2d2f0* origin, void* a, void* b, void* c, void* binder);
};

C3DMeshVolume_00c2d2f0::C3DMeshVolume_00c2d2f0(void* owner, const C3DVector_00c2d2f0* origin, void* a, void* b, void* c, void* binder)
    : m_origin(*origin), m_c(a), m_10(b), m_14(c), m_318(0), m_pOwner(owner), m_data(), m_33c(false)
{
    for (long i = 0; i < 2; ++i)
        m_parts[i].Bind(binder);
}
