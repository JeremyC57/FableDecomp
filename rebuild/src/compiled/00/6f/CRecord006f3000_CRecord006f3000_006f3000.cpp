// CRecord_006f3000::CRecord_006f3000(const CRecord_006f3000& other) -- memberwise copy: a long, a flag, a long, two flags, a CCharString, two retail vectors
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006F3000, length 79 bytes.
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30, the 12-byte retail vector
// copy constructor @ 0x006F0810 (x2; the one the NShape::CShape point list uses).
// Name fix (NF387, T3 nf383 0x009B7870): the member copies bind STLport ??0?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@QAE@ABV01@@Z
//   at 0x006F0810 (NF383 R2: 15 exclusive vector-context FableWin callers; landed self row) through the shared container header; was the
//   body-less local CRetailVector<C3DVector>.
// The manifest label "CBoast::CBoast" is propagated: CBoast's copy constructor is @ 0x004B73A0 and
// its layout (ID/Cost/Reward/strings) does not fit these bytes. Members are typed by their copies: +0 long,
// +4 byte, +8 long, +0xc byte, +0xd byte, +0x10 CCharString, +0x14 vector, +0x20 vector (the catalog names the
// function CCombatDebugInfo::CCombatDebugInfo; the self respell is queued separately, NF387 NAME_FIXES).
// The copy constructor is written out of line so the TU carries the symbol.
class CCharString { public:
    void* m_pStringData;
    CCharString(const CCharString& other);      // 0x0099EC30
};

// The two point lists are STLport vector<C3DVector>: Ego_r CCombatDebugInfo has the same member order and types (long, bool,
// long, bool, bool, CCharString, PlacementVectors / AttackerPos as vector<C3DVector>); its vector is 16 bytes against retail
// STLport's 12, so its 52 bytes are this record's 0x2c plus 4 per vector (Ego_r CVertexGroupBlock 32 v retail 0x18 likewise).
// stlport/fable_containers.h with the copy constructor declared only (FABLE_RETAIL_STLP_VECTOR_COPY_DECL_ONLY,
// as the landed CCombatManager::GetCombatDebugInfo 0x006EAC50 TU), so each member copy calls 0x006F0810 out of line.
#define FABLE_RETAIL_STLP_VECTOR_COPY_DECL_ONLY
// ~vector<C3DVector> declared only (FABLE_RETAIL_STLP_VECTOR_DTOR_OUTOFLINE; retail has it out of line, 0x004DCFBB): this copy
// constructor never destroys a vector, and the inline body would make VC7.1 emit the destructor chain as pick-any COMDATs here.
#define FABLE_RETAIL_STLP_VECTOR_DTOR_OUTOFLINE
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"

struct CRecord_006f3000 {
    long                     m_id;              // +0x00
    unsigned char            m_flag04;          // +0x04
    long                     m_value;           // +0x08
    unsigned char            m_flag0c;          // +0x0c
    unsigned char            m_flag0d;          // +0x0d
    CCharString              m_name;            // +0x10
    std::vector<C3DVector>   m_points0;         // +0x14
    std::vector<C3DVector>   m_points1;         // +0x20
    CRecord_006f3000(const CRecord_006f3000& other);
};

CRecord_006f3000::CRecord_006f3000(const CRecord_006f3000& other)
    : m_id(other.m_id), m_flag04(other.m_flag04), m_value(other.m_value), m_flag0c(other.m_flag0c)
    , m_flag0d(other.m_flag0d), m_name(other.m_name), m_points0(other.m_points0), m_points1(other.m_points1)
{
}