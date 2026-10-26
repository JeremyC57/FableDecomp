// C3DMeshObject::GetNoDummyObjects(const CCharString& name) const -- how many dummy objects the mesh statistics hold under the name's CRC (FindDummyObjectsEnd - FindDummyObjectsBegin over 56-byte pairs)
// Leaf: GetNoDummyObjects
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A36480, 90 bytes (+ int3 padding), `ret 4`.
// Identity: Ego_r.pdb `?GetNoDummyObjects@C3DMeshObject@@QBEJABVCCharString@@@Z`; callers 0x004BE09A / 0x004C0AD5. The catalog's
//   "CTCInventoryAbilities_Helper_GetCount" is propagated/descriptive (the callees carry the propagated
//   "CTCInventoryAbilities::AddLearnedExpression" label; they are the C3DMeshStats range finders).
// Callees: CRC Calc @ 0x004014A0, C3DMeshStats::FindDummyObjectsEnd(unsigned long) const @ 0x00A2E970 (UpperBound 0x00A317E0
//   inside), C3DMeshStats::FindDummyObjectsBegin(unsigned long) const @ 0x00A2E960 (LowerBound 0x00A31790 inside).
// Element: std::pair<unsigned long, C3DDummyObject> = 56 bytes (CMatrix3x4 Transform, long BoneIndex), hence the
//   0x92492493 / `add edx,esi ; sar edx,5` divide of the pointer difference. Twin of GetNoHelperPoints 0x00A36140.
// Shape: the CRC is a named local (edi, pushed twice); the statistics pointer is re-read from the counted pointer's Data for
//   each call (`mov ecx,[ebx+8]`); End is called before Begin and its result is a named local (esi) -- the one-expression
//   `End(h) - Begin(h)` calls Begin first and keeps the statistics pointer cached in edi.
class CCharStringData { public:
    const char*   m_pChars;                        // +0x00
    unsigned long m_length;                        // +0x04
};
extern const char g_emptyLiteral_0129aaf4[];       // 0x0129AAF4: ""
class CCharString { public:
    CCharStringData* m_pStringData;
    unsigned long GetLength() const { return m_pStringData == 0 ? 0 : m_pStringData->m_length; }
    const char* GetChars() const { return m_pStringData == 0 ? g_emptyLiteral_0129aaf4 : m_pStringData->m_pChars; }
};
unsigned long __fastcall Calc_004014a0(unsigned long seed, const char* data, unsigned long length);   // 0x004014A0: CRC

class CMatrix3x4 { public: float E[12]; };
class C3DDummyObject { public:                            // 0x34 bytes
    CMatrix3x4 Transform;                          // +0x00
    long       BoneIndex;                          // +0x30
};
namespace std {
template <class T1, class T2> struct pair { T1 first; T2 second; };
}
typedef std::pair<unsigned long, C3DDummyObject> TDummyObjectEntry;   // 56 bytes

class C3DMeshStats { public:
    const TDummyObjectEntry* FindDummyObjectsBegin(unsigned long nameHash) const;   // 0x00A2E900
    const TDummyObjectEntry* FindDummyObjectsEnd(unsigned long nameHash) const;     // 0x00A2E910
};

template <class T> class CCountedPointer { public:
    T*    Data;                                    // +0
    void* Info;                                    // +4
    T* operator->() const { return Data; }
};

class C3DMeshInfo;

class C3DMeshObject { public:
    virtual void v00();
    C3DMeshInfo*                         MeshInfo; // +0x04
    CCountedPointer<const C3DMeshStats>  Mesh;     // +0x08
    long GetNoDummyObjects(const CCharString& name) const;
};

long C3DMeshObject::GetNoDummyObjects(const CCharString& name) const
{
    unsigned long nameHash = Calc_004014a0(0, name.GetChars(), name.GetLength());
    const TDummyObjectEntry* last = Mesh->FindDummyObjectsEnd(nameHash);
    return last - Mesh->FindDummyObjectsBegin(nameHash);
}