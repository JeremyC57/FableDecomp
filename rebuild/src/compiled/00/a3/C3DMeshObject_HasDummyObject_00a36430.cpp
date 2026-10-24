// C3DMeshObject::HasDummyObject(const CCharString& name) const -- false without mesh statistics, else ask them for the name's CRC (C3DMeshStats::HasDummyObject @ 0x00A2EA60)
// Leaf: HasDummyObject
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A36430, 66 bytes (+ int3 padding), `ret 4`.
// Identity: Ego_r.pdb `?HasDummyObject@C3DMeshObject@@QBE_NABVCCharString@@@Z`; callers 0x004C0A53 / 0x004C0E1C (the graphic
//   appearance). The catalog's "HasDummyObject_Wrapper" is descriptive. Sibling of the landed HasHelperPoint 0x00A36100
//   (which has no null test).
// Callees: CRC Calc @ 0x004014A0 (`__fastcall(seed 0, chars, length)`), C3DMeshStats::HasDummyObject(unsigned long) const
//   @ 0x00A2EA60 (landed, returns the engine's byte flag), the empty literal 0x0129AAF4 for a null string.
// Layout (Ego_r.pdb C3DMeshObject, 0x78): vptr +0, MeshInfo +4, Mesh = CCountedPointer<const C3DMeshStats> +8 (Data, Info).
// Shape: the statistics pointer is read once into esi and null-tested first; the call is the `if` body and `return false` the fall-through, which VC places at the end (`xor al,al`); the CCharString
//   accessors are inlined length-first (call arguments evaluate right to left).
struct CCharStringData {
    const char*   m_pChars;                        // +0x00
    unsigned long m_length;                        // +0x04
};
extern const char g_emptyLiteral_0129aaf4[];       // 0x0129AAF4: ""
struct CCharString {
    CCharStringData* m_pStringData;
    unsigned long GetLength() const { return m_pStringData == 0 ? 0 : m_pStringData->m_length; }
    const char* GetChars() const { return m_pStringData == 0 ? g_emptyLiteral_0129aaf4 : m_pStringData->m_pChars; }
};
unsigned long __fastcall Calc_004014a0(unsigned long seed, const char* data, unsigned long length);   // 0x004014A0: CRC

struct C3DMeshStats {
    bool HasDummyObject(unsigned long nameHash) const;   // 0x00A2EA60
};

template <class T> struct CCountedPointer {
    T*    Data;                                    // +0
    void* Info;                                    // +4
};

struct C3DMeshInfo;

struct C3DMeshObject {
    virtual void v00();
    C3DMeshInfo*                         MeshInfo; // +0x04
    CCountedPointer<const C3DMeshStats>  Mesh;     // +0x08
    bool HasDummyObject(const CCharString& name) const;
};

bool C3DMeshObject::HasDummyObject(const CCharString& name) const
{
    const C3DMeshStats* stats = Mesh.Data;
    if (stats != 0)
        return stats->HasDummyObject(Calc_004014a0(0, name.GetChars(), name.GetLength()));
    return false;
}