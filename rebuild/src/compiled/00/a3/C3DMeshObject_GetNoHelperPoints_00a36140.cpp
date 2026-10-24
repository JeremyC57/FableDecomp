// C3DMeshObject::GetNoHelperPoints(const CCharString& name) const -- how many helper points the mesh statistics hold under the name's CRC (FindHelperPointsEnd - FindHelperPointsBegin over 20-byte pairs)
// Leaf: GetNoHelperPoints
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A36140, 88 bytes (+ int3 padding), `ret 4`.
// Identity: Ego_r.pdb `?GetNoHelperPoints@C3DMeshObject@@QBEJABVCCharString@@@Z`; callers 0x004BDF2A / 0x004C0794. The catalog's
//   "CTCInventoryAbilities_Helper_CRC" is propagated/descriptive (the callees carry the propagated
//   "CTCInventoryAbilities::AddLearnedExpression" label; they are the C3DMeshStats range finders).
// Callees: CRC Calc @ 0x004014A0, C3DMeshStats::FindHelperPointsEnd(unsigned long) const @ 0x00A2E910 (UpperBound 0x00A31700
//   inside), C3DMeshStats::FindHelperPointsBegin(unsigned long) const @ 0x00A2E900 (LowerBound 0x00A316B0 inside).
// Element: std::pair<unsigned long, C3DHelperPoint> = 20 bytes (C3DVector Position, long BoneIndex), hence the
//   0x66666667 / `sar edx,3` divide of the pointer difference.
// Shape: the CRC is a named local (edi, pushed twice); the statistics pointer is re-read from the counted pointer's Data for
//   each call (`mov ecx,[ebx+8]`); End is called before Begin and its result is a named local (esi) -- the one-expression
//   `End(h) - Begin(h)` calls Begin first and keeps the statistics pointer cached in edi.
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

struct C3DVector { float X, Y, Z; };
struct C3DHelperPoint {                            // 16 bytes
    C3DVector Position;                            // +0x00
    long      BoneIndex;                           // +0x0c
};
namespace std {
template <class T1, class T2> struct pair { T1 first; T2 second; };
}
typedef std::pair<unsigned long, C3DHelperPoint> THelperPointEntry;   // 20 bytes

struct C3DMeshStats {
    const THelperPointEntry* FindHelperPointsBegin(unsigned long nameHash) const;   // 0x00A2E900
    const THelperPointEntry* FindHelperPointsEnd(unsigned long nameHash) const;     // 0x00A2E910
};

template <class T> struct CCountedPointer {
    T*    Data;                                    // +0
    void* Info;                                    // +4
    T* operator->() const { return Data; }
};

struct C3DMeshInfo;

struct C3DMeshObject {
    virtual void v00();
    C3DMeshInfo*                         MeshInfo; // +0x04
    CCountedPointer<const C3DMeshStats>  Mesh;     // +0x08
    long GetNoHelperPoints(const CCharString& name) const;
};

long C3DMeshObject::GetNoHelperPoints(const CCharString& name) const
{
    unsigned long nameHash = Calc_004014a0(0, name.GetChars(), name.GetLength());
    const THelperPointEntry* last = Mesh->FindHelperPointsEnd(nameHash);
    return last - Mesh->FindHelperPointsBegin(nameHash);
}