// C3DMeshObject::GetNoHelperPoints(const CCharString& name) const -- how many helper points the mesh statistics hold under the name's CRC (FindHelperPointsEnd - FindHelperPointsBegin over 20-byte pairs)
// Leaf: GetNoHelperPoints
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
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
class CCRC {
public:
    static unsigned long __fastcall Calc(unsigned long, const void*, unsigned long);   // 0x004014A0
};   // 0x004014A0: CRC

#include "engine/C3DVector.h"
class C3DHelperPoint { public:                            // 16 bytes
    C3DVector Position;                            // +0x00
    long      BoneIndex;                           // +0x0c
};
namespace std {
template <class T1, class T2> struct pair { T1 first; T2 second; };
}
typedef std::pair<unsigned long, C3DHelperPoint> THelperPointEntry;   // 20 bytes

class C3DMeshStats { public:
    const THelperPointEntry* FindHelperPointsBegin(unsigned long nameHash) const;   // 0x00A2E900
    const THelperPointEntry* FindHelperPointsEnd(unsigned long nameHash) const;     // 0x00A2E910
};

#include "engine/CCountedPointer.h"

class C3DMeshInfo;

class C3DMeshObject { public:
    virtual void v00();
    C3DMeshInfo*                         MeshInfo; // +0x04
    CCountedPointer<const C3DMeshStats>  Mesh;     // +0x08
    long GetNoHelperPoints(const CCharString& name) const;
};

long C3DMeshObject::GetNoHelperPoints(const CCharString& name) const
{
    unsigned long nameHash = CCRC::Calc(0, name.GetChars(), name.GetLength());
    const THelperPointEntry* last = Mesh->FindHelperPointsEnd(nameHash);
    return last - Mesh->FindHelperPointsBegin(nameHash);
}