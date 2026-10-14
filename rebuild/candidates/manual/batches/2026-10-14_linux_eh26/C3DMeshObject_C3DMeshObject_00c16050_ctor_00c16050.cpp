// C3DMeshObject_00c16050::C3DMeshObject(owner, desc, unused) -- EH-framed constructor over a polymorphic base (dtor @ 0x00C15A40, its vtable store dead and dropped; state 0): vtable 0x012B2A74, owner at +4, volume at +8 cleared, the descriptor's two words copied to +0xc / +0xe, both scales at +0x10 / +0x14 set to 1.0f; the descriptor's origin vector is copied to the frame, the object's scale fetched (@ 0x00C15A70, result homed in the dead first argument slot) and a 0x340-byte C3DMeshVolume allocated (null-checked `new`, state 1 frees it if the constructor @ 0x00C2D2F0 throws) from the owner's +0x5c binder, the origin, the descriptor's three handles at +4..+0xc and the scale; the pointer (or 0) lands at +8
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C16050, oracle length 212 bytes. Built under /EHsc.
// Retail bytes:  ... mov edi,[esp+2Ch] ; mov eax,[esp+28h] ; mov [esi+4],eax ; mov dword ptr [esi],12B2A74h ; mov dword ptr [esi+8],0 ; mov cx,[edi] ; mov [esi+0Ch],cx ; mov dx,[edi+2] ; mov eax,3F800000h ; mov [esi+0Eh],dx ; mov [esi+10h],eax ; mov [esi+14h],eax ; mov ecx,[edi+18h] ; mov eax,[edi+10h] ; mov edx,[edi+14h] ; mov [esp+10h],ecx ; mov ecx,esi ; mov dword ptr [esp+20h],0 ; mov [esp+0Ch],eax ; mov [esp+14h],edx ; call GetScale ; fstp dword ptr [esp+28h] ; push 340h ; call operator new ; add esp,4 ; mov [esp+2Ch],eax ; test eax,eax ; mov byte ptr [esp+20h],1 ; je null ; ... call C3DMeshVolume::C3DMeshVolume ; jmp store ; null: xor eax,eax ; store: ... mov [esi+8],eax ; ... ret 0Ch
// The manifest label ("InitializeWithPhysics") is propagated from the caller; the volume class matches the landed 0x00C2D2F0 constructor.
void* __cdecl operator new(unsigned int size);

struct C3DVector_00c16050 { float x, y, z; };

struct C3DMeshVolume_00c16050 {            // 0x340 bytes
    unsigned char _body[0x340];
    C3DMeshVolume_00c16050(void* binder, const C3DVector_00c16050* origin, void* a, void* b, void* c, float scale);   // 0x00C2D2F0
};

struct CMeshDesc_00c16050 {
    unsigned short m_0;
    unsigned short m_2;
    void* m_4;
    void* m_8;
    void* m_c;
    C3DVector_00c16050 m_origin;           // +0x10
};

struct CMeshOwner_00c16050 {
    unsigned char _pad[0x5c];
    void* m_pBinder;                       // +0x5c
};

struct CMeshBase_00c16050 {                // polymorphic base (vtable 0x012B2A40)
    virtual void v00();
    CMeshBase_00c16050() { }
    ~CMeshBase_00c16050();                 // 0x00C15A40
};

struct C3DMeshObject_00c16050 : CMeshBase_00c16050 {
    CMeshOwner_00c16050* m_pOwner;         // +4
    C3DMeshVolume_00c16050* m_pVolume;     // +8
    unsigned short m_c;
    unsigned short m_e;
    float m_10;
    float m_14;
    float GetScale();                      // 0x00C15A70
    C3DMeshObject_00c16050(CMeshOwner_00c16050* owner, const CMeshDesc_00c16050* desc, void* unused);
};

C3DMeshObject_00c16050::C3DMeshObject_00c16050(CMeshOwner_00c16050* owner, const CMeshDesc_00c16050* desc, void* unused)
    : m_pOwner(owner), m_pVolume(0), m_c(desc->m_0), m_e(desc->m_2), m_10(1.0f), m_14(1.0f)
{
    C3DVector_00c16050 origin;             // the descriptor's Y and Z are swapped on the way in
    origin.x = desc->m_origin.x;
    origin.y = desc->m_origin.z;
    origin.z = desc->m_origin.y;
    float scale = GetScale();
    m_pVolume = new C3DMeshVolume_00c16050(m_pOwner->m_pBinder, &origin, desc->m_4, desc->m_8, desc->m_c, scale);
}
