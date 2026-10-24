// std::_Construct(QuadricOptimiserInternals::COptimisedBone*, const QuadricOptimiserInternals::COptimisedBone&) -- placement copy of an optimiser bone: name string, parent, remap flag, new index
// Retail: Fable.exe @ 0x00AD15C0, 37 bytes; __fastcall-shaped STLport helper (ecx = element slot, edx = source element).
// Callees: CCharString copy ctor @ 0x0099EC30.
// Leaf: _Construct
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)
namespace N00ad15c0 {
struct CCharString {
    void* m_pData;                               // ref-counted string body
    CCharString(const CCharString& other);       // 0x0099EC30
};
struct COptimisedBone {                          // QuadricOptimiserInternals::COptimisedBone (PDB, sizeof 0x10)
    CCharString BoneName;                        // +0x00
    long        Parent;                          // +0x04
    bool        Remapped;                        // +0x08
    long        NewIndex;                        // +0x0C
};

// STLport _Construct: placement-copy one element (null-checked placement new, then the implicit copy constructor)
void __fastcall _Construct(COptimisedBone* p, const COptimisedBone& value)
{
    if (p != 0)
        new (p) COptimisedBone(value);
}
}