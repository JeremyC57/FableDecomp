// CPooledResource::SetRecord54(long unused, const SUnprovenRecord_00c26650& record) -- copy a 16-byte record into the resource at +0x54
// Leaf: ?SetRecord54@CPooledResource@@UAEXJABUSUnprovenRecord_00c26650@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C26650, 32 bytes, thiscall, `ret 8`.
// Identity (descriptive): slot 5 of the non-RTTI vtable 0x012B3FDC (stored by its constructor 0x00C266E0), the vtable
//   of the pooled resource whose IsActive 0x00C265A0 / Release 0x00C265B0 are landed as CPooledResource; its other
//   slots are word setters (+0x48, +0x4C, +0x64) of the same two-argument shape. libpk has no PDB or RTTI.
// Shape: a POD struct assignment copies the four dwords through edx with ecx = &member (`add ecx,54h`).
struct SUnprovenRecord_00c26650 {               // 16 bytes
    long m_a;
    long m_b;
    long m_c;
    long m_d;
};

struct CPooledResource {                        // vtable 0x012B3FDC
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void SetRecord54(long unused, const SUnprovenRecord_00c26650& record);   // slot 5
    long          m_handle;                     // +0x04
    unsigned char _pad08[0x54 - 0x08];
    SUnprovenRecord_00c26650 m_record;          // +0x54
};

void CPooledResource::SetRecord54(long unused, const SUnprovenRecord_00c26650& record)
{
    m_record = record;
}