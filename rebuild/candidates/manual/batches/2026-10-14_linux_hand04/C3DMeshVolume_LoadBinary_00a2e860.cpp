// C3DMeshVolume::LoadBinary(CMemoryDataInputStream* in, CMovableResourceMemoryPool* pool) -- read the name, create the containment volume and load it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A2E860, oracle length 89 bytes.
// Callees: CMemoryDataInputStream virtual slot 6 (+0x18, returns a CCharString by value through the hidden pointer),
//          CCharString::operator=(const CCharString&) @ 0x0099EFB0 (landed), CCharString::~CCharString @ 0x0099EAE0,
//          operator new @ 0x00BFEA1A (landed-trusted), the 16-byte volume's constructor @ 0x00A752D0 (manifest "C3DGroup2::C3DGroup2"
//          is propagated; it is the concrete CContainmentVolume constructor), CCountedPointer::Reset(T*) @ 0x00A31640 (landed),
//          CContainmentVolume::LoadBinary(CMemoryDataInputStream*, CMovableResourceMemoryPool*) @ 0x00A754E0 (manifest).
//
// Retail bytes:  push esi ; mov esi,ecx ; push edi ; mov edi,[esp+0Ch] ; mov eax,[edi] ; lea ecx,[esp+0Ch] ; push ecx ; mov ecx,edi ; call [eax+18h] ;
//                push eax ; mov ecx,esi ; call CCharString::operator= ; lea ecx,[esp+0Ch] ; call CCharString::~CCharString ; push 10h ; call operator new ;
//                add esp,4 ; test eax,eax ; je Z ; mov ecx,eax ; call CContainmentVolume::CContainmentVolume ; jmp R ; Z: xor eax,eax ; R: add esi,4 ;
//                push eax ; mov ecx,esi ; call CCountedPointer::Reset ; mov edx,[esp+10h] ; mov ecx,[esi] ; push edx ; push edi ; call LoadBinary ; pop edi ; pop esi ; ret 8
// PDB layout (struct_layouts_egor.tsv, C3DMeshVolume = 12 bytes): +0 Name (CCharString), +4 ConvexVolume
// (CCountedPointer<CContainmentVolume>). The stream's ReadString() temporary is built in the dead first
// argument's home slot (esp+0xC after the two pushes), assigned to Name and destroyed; `new` is the usual
// null-checked allocation + constructor; the result of the volume's own LoadBinary is returned straight through.
struct CCharString {
    char* m_pString;
    ~CCharString();                                        // 0x0099EAE0
    CCharString& operator=(const CCharString& other);     // 0x0099EFB0
};

struct CMovableResourceMemoryPool;

struct CMemoryDataInputStream {
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c(); virtual void v10(); virtual void v14();
    virtual CCharString ReadString();                      // slot 6 (+0x18)
};

struct CContainmentVolume {
    unsigned char m_body[0x10];
    CContainmentVolume();                                  // 0x00A752D0
    bool LoadBinary(CMemoryDataInputStream* in, CMovableResourceMemoryPool* pool);   // 0x00A754E0
};

struct CCountedPointer {                   // CCountedPointer<CContainmentVolume>
    CContainmentVolume* m_pObject;
    void*               m_pCount;
    void Reset(CContainmentVolume* object); // 0x00A31640
};

struct C3DMeshVolume {
    CCharString     Name;                  // +0
    CCountedPointer ConvexVolume;          // +4
    bool LoadBinary(CMemoryDataInputStream* in, CMovableResourceMemoryPool* pool);
};

bool C3DMeshVolume::LoadBinary(CMemoryDataInputStream* in, CMovableResourceMemoryPool* pool)
{
    Name = in->ReadString();
    ConvexVolume.Reset(new CContainmentVolume);
    return ConvexVolume.m_pObject->LoadBinary(in, pool);
}
