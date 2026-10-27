// CVtable_012b3224::Create(long key) -- ask the factory at +4 (its slot 0) for the object behind key and wrap it in a new 12-byte CVtable_012b4f60 handle over (this, object); NULL when the factory has none
// Flags: /EHsc
// Leaf: ?Create@CVtable_012b3224@@UAEPAUCVtable_012b4f60@@J@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1B8B0, 118 bytes, thiscall, `ret 4`. Built under /EHsc (the EH frame
//   unwinds the `new` allocation should the constructor throw).
// Identity (descriptive): slot 0 of the non-RTTI vtable 0x012B3224 (slot 1 its scalar deleting destructor 0x00C1B970);
//   libpk has no PDB or RTTI, so the class is named by its vtable.
// Callees: the factory's slot 0, scalar operator new @ 0x00BFEA1A (MSVCR71), CVtable_012b4f60::CVtable_012b4f60(void*,
//   void*) @ 0x00C330A0 (landed: vtable, second argument at +4, first at +8).
// Shape: `new T(this, object)` with the null test of the allocation merged into the shared `return 0` block.
void* __cdecl operator new(unsigned int size);  // 0x00BFEA1A

struct CVtable_012b4f60 {                       // the 12-byte handle (vtable 0x012B4F60)
    virtual void v00();
    void* m_pSecond;                            // +0x04
    void* m_pFirst;                             // +0x08
    CVtable_012b4f60(void* first, void* second);   // 0x00C330A0
};

struct CUnprovenFactory_012b3224 {
    virtual void* Find(long key);               // slot 0
};

class CVtable_012b3224 {                        // vtable 0x012B3224: { Create, scalar deleting destructor }
public:
    virtual CVtable_012b4f60* Create(long key);
    CUnprovenFactory_012b3224* m_pFactory;      // +0x04
};

CVtable_012b4f60* CVtable_012b3224::Create(long key)
{
    void* object = m_pFactory->Find(key);
    if (!object)
        return 0;
    return new CVtable_012b4f60(this, object);
}