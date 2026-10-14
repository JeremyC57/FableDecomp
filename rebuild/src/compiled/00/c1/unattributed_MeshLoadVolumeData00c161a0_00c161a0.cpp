// CMesh_00c161a0::LoadVolumeData(buffer, source) -- EH-framed: a profiling lock on the module's global section (0x0143BB14; ctor @ 0x00C0FFC0, dtor @ 0x00C0FFE0 reports slow holds) and a 0x1c-byte volume record (ctor @ 0x00C2D1A0: a zeroed scale, an owned buffer at +4, 1, 0, 0, 1.0f; its inline destructor frees the buffer @ 0x00C167C0, states 0-1); the record's scale is the source's +0x10 as an unsigned-to-float conversion (`fild` plus the 2^32 fix-up), its buffer takes the caller's buffer (@ 0x00C16970), its four words at +0xc.. come from the source's +4 / +0 / +0xc / +8, then the mesh volume at +8 receives the record with the source's +0x14 as a float (@ 0x00C2D5E0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C161A0, oracle length 212 bytes. Built under /EHsc.
// Retail bytes:  ... mov edi,ecx ; push offset g_section ; lea ecx,[esp+0Ch] ; call CProfileLock::CProfileLock ; lea ecx,[esp+10h] ; mov dword ptr [esp+34h],0 ; call CVolumeRecord::CVolumeRecord ; mov esi,[esp+40h] ; mov eax,[esi+10h] ; fild dword ptr [esi+10h] ; test eax,eax ; mov byte ptr [esp+34h],1 ; jge a ; fadd dword ptr [2^32] ; a: mov ecx,[esp+3Ch] ; fstp dword ptr [esp+10h] ; push ecx ; lea ecx,[esp+18h] ; call CBuffer::Assign ; fild dword ptr [esi+14h] ; ... (four word copies) ... lea eax,[esp+10h] ; mov [esp+28h],edx ; push eax ; jge b ; fadd dword ptr [2^32] ; b: push ecx ; mov ecx,[edi+8] ; fstp dword ptr [esp] ; call CMeshVolume::AddSlice ; lea ecx,[esp+14h] ; mov byte ptr [esp+34h],0 ; call CBuffer::Release ; lea ecx,[esp+8] ; mov dword ptr [esp+34h],-1 ; call CProfileLock::~CProfileLock ; ... ret 8
// The manifest label is propagated; the classes are named by address.
extern unsigned char g_section_0143bb14[];  // 0x0143BB14: the module's critical section

struct CProfileLock_00c161a0 {             // 8 bytes: section + entry tick
    void* m_pSection;
    unsigned long m_tick;
    CProfileLock_00c161a0(void* section);  // 0x00C0FFC0
    ~CProfileLock_00c161a0();              // 0x00C0FFE0
};

struct CBuffer_00c161a0 {                  // 8 bytes: owned block + size
    void* m_pData;
    long m_size;
    void Assign(const CBuffer_00c161a0* source);   // 0x00C16970
    void Release();                        // 0x00C167C0
};

struct CVolumeRecord_00c161a0 {            // 0x1c bytes
    float m_scale;                         // +0
    CBuffer_00c161a0 m_buffer;             // +4
    long m_c;
    long m_10;
    long m_14;
    long m_18;
    CVolumeRecord_00c161a0();              // 0x00C2D1A0
    ~CVolumeRecord_00c161a0() { m_buffer.Release(); }
};

struct CVolumeSource_00c161a0 {
    long m_0;
    long m_4;
    long m_8;
    long m_c;
    unsigned long m_10;
    unsigned long m_14;
};

struct CMeshVolume_00c161a0 { long AddSlice(float position, CVolumeRecord_00c161a0* record); };   // 0x00C2D5E0

struct CMesh_00c161a0 {
    unsigned char _pad[8];
    CMeshVolume_00c161a0* m_pVolume;       // +8
    void LoadVolumeData(const CBuffer_00c161a0* buffer, const CVolumeSource_00c161a0* source);
};

void CMesh_00c161a0::LoadVolumeData(const CBuffer_00c161a0* buffer, const CVolumeSource_00c161a0* source)
{
    CProfileLock_00c161a0 lock(g_section_0143bb14);
    CVolumeRecord_00c161a0 record;
    record.m_scale = (float)source->m_10;
    record.m_buffer.Assign(buffer);
    record.m_c = source->m_4;
    record.m_10 = source->m_0;
    record.m_14 = source->m_c;
    record.m_18 = source->m_8;
    m_pVolume->AddSlice((float)source->m_14, &record);
}