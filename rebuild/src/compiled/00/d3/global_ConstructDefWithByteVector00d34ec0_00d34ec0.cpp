// _global::Construct_DefWithByteVector_00d34ec0(CDefWithByteVector*, const CDefWithByteVector&) -- STLport _Construct: placement copy-construct one def-class object (CDefClassBase base + a byte vector member)
// Leaf: Construct_DefWithByteVector_00d34ec0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D34EC0, length 37 bytes (+ int3 padding).
// Callees: CDefClassBase::CDefClassBase(const CDefClassBase&) @ 0x0046644C, the vector<unsigned char> copy constructor
//          @ 0x00D34F00 (allocates end-begin bytes and copies them; catalogued as the propagated `Map_InsertOrUpdatePair`).
// Shape: /Gr TU (__fastcall: destination in ecx, source in edx). `new (p) T(v)` null-tests the placement result, then the
// implicit copy constructor: the base copy constructor, the class's own vtable store (0x012CCC50, no RTTI locator),
// and the out-of-line copy of the vector member at +0x28 (CDefClassBase is 0x25 bytes, byte packed; the derived
// member is 4-aligned).
// Label correction: the catalog's `_Cons_val<allocator<pair<CCharString,CArray<unsigned long>>>>` is propagated; no
// CCharString is touched. The class name is unknown (descriptive).
inline void* operator new(unsigned int, void* p) { return p; }

#pragma pack(push, 1)
struct CDefClassBase {
    virtual void v00();
    unsigned char _body[0x21];                     // +0x04..+0x25 (CResource, PDefManager, GlobalIndex, flags)
    CDefClassBase(const CDefClassBase& other);     // 0x0046644C
};
#pragma pack(pop)
struct CByteVector {                               // std::vector<unsigned char> (12 bytes)
    unsigned char* m_first;
    unsigned char* m_last;
    unsigned char* m_end;
    CByteVector(const CByteVector& other);         // 0x00D34F00
};
struct CDefWithByteVector : CDefClassBase {        // vtable 0x012CCC50
    CByteVector m_data;                            // +0x28
    virtual void v00();
};

void __fastcall Construct_DefWithByteVector_00d34ec0(CDefWithByteVector* p, const CDefWithByteVector& value)
{
    new (p) CDefWithByteVector(value);
}