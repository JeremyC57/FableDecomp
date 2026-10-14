// C3DMeshVolume::C3DMeshVolume() -- empty Name string and empty ConvexVolume counted pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0A3D0, oracle length 20 bytes.
// Callees: CCharString::CCharString @ 0x0099E4B0 (landed; the default constructor that nulls the
// string pointer and bumps the global CCharString counter).
//
// Retail bytes:  push esi ; mov esi,ecx ; call CCharString::CCharString ; xor eax,eax ; mov [esi+4],eax ; mov [esi+8],eax ; mov eax,esi ; pop esi ; ret
// Identity: the manifest label "C3DSkeletalMorphTarget::C3DSkeletalMorphTarget" is propagated and rejected --
// that class is {CCharString Name; CArray BoneMorphs} and the retail 12-byte CArray would need three zero
// stores, not two. The body is the default constructor of a 12-byte {CCharString, CCountedPointer<T>}
// class; C3DMeshVolume (PDB: +0 Name, +4 ConvexVolume = CCountedPointer<CContainmentVolume>, 12 bytes)
// is exactly that and its LoadBinary (0x00A2E860) resets the same pointer, so the body is landed under
// that name. Every other {CCharString, CCountedPointer} class shares the COMDAT-folded body.
struct CCharString {
    char* m_pString;
    CCharString();                         // 0x0099E4B0
};

struct CCountedPointer {
    void* m_pObject;
    void* m_pCount;
    CCountedPointer() : m_pObject(0), m_pCount(0) { }
};

struct C3DMeshVolume {
    CCharString     Name;                  // +0
    CCountedPointer ConvexVolume;          // +4  CCountedPointer<CContainmentVolume>
    C3DMeshVolume();
};

C3DMeshVolume::C3DMeshVolume()
    : Name(), ConvexVolume()
{
}
