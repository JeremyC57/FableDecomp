// CVtable_012b3858::Visit(CLifetimeRef_00c26060* ref) -- release a lifetime reference whose object's key word (+4) equals this visitor's key (+4)
// Leaf: ?Visit@CVtable_012b3858@@UAEXPAUCLifetimeRef_00c26060@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C21F00, 28 bytes, thiscall, `ret 4`.
// Identity (descriptive): slot 0 of the two-slot non-RTTI visitor vtable 0x012B3858 (slot 1 its scalar deleting
//   destructor 0x00C242F0); libpk has no PDB or RTTI, so the functor is named by its vtable.
// Callees: CLifetimeRef_00c26060::~CLifetimeRef_00c26060 @ 0x00C26060 (landed: drops the object's lifetime counter
//   and nulls the holder), called explicitly on the reference.
// Shape: a null test of the held object, the key compare, then the explicit destructor call with ecx = the reference
//   (`mov ecx,edx`).
struct CLifetimedObject {
    void* _00;
    long  m_key;                                // +0x04
};

struct CLifetimeRef_00c26060 {
    CLifetimedObject* m_pObject;                // +0x00
    ~CLifetimeRef_00c26060();                   // 0x00C26060
};

class CVtable_012b3858 {                        // vtable 0x012B3858: { Visit, scalar deleting destructor }
public:
    virtual void Visit(CLifetimeRef_00c26060* ref);
    long m_key;                                 // +0x04
};

void CVtable_012b3858::Visit(CLifetimeRef_00c26060* ref)
{
    if (ref->m_pObject && ref->m_pObject->m_key == m_key)
        ref->~CLifetimeRef_00c26060();
}