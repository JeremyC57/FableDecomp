// std::vector<CPrizeScoreDef>::resize(unsigned int n) -- one-argument resize: build a default CPrizeScoreDef temporary, call resize(n, value), destroy the temporary
// Leaf: resize
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004EDA32, 58 bytes (a /Os module: unaligned entry, ebp
// frame, `leave`), `ret 4`.
// Label correction: the catalog name ~WaypointData is propagated.  The temporary's vtable 0x012437D4
// is CPrizeScoreDef (rtti-vtables.tsv), and the only caller 0x004EDA06 is the CPrizeScoreDef
// TransferVectorOfSubComponents loop (resize, then each 0x30-byte element's virtual Transfer, slot
// 0x48).  The callee 0x004EDA6C (catalog "CArray<pair<ulong,CSoundPair>>::push_back", propagated) is
// the two-argument resize(n, const T&).
// Callees: CSubDefClassBase::CSubDefClassBase @ 0x0044C0C0 (out of line in this module: CResource
// ctor, DefIndex / flag bits, its vtable 0x01231D54), resize(n, value) @ 0x004EDA6C,
// CResource::~CResource @ 0x009FC550.
// CPrizeScoreDef (Ego_r.pdb, 48 bytes) : CSubDefClassBase (vtable 0x01231D54) : CDefPointeeBase
// (vtable 0x01230BA0) : CResource; own members Score +0x28, Mult +0x2c (not initialised by the
// default ctor).
// Shape (the landed 0x0067D610 model): the inline default ctor is the out-of-line base ctor plus the
// CPrizeScoreDef vtable store; the derived destructors are IMPLICIT (no vtable store), the inline
// CDefPointeeBase destructor stores 0x01230BA0 before the out-of-line CResource destructor.
#pragma optimize("y", off)
#pragma optimize("s", on)

class CResource { public:
    virtual ~CResource();                          // 0x009FC550
    unsigned long m_refCount;                      // +0x04
    void*         m_pResourceList;                 // +0x08
    CResource*    m_pPrevResource;                 // +0x0c
    CResource*    m_pNextResource;                 // +0x10
    unsigned long m_resourceSize;                  // +0x14
    unsigned long m_lastUsedFrame;                 // +0x18
};

class CDefPointeeBase : public CResource { public:               // vtable 0x01230BA0
    virtual ~CDefPointeeBase() {}
};

class CSubDefClassBase : public CDefPointeeBase { public:        // vtable 0x01231D54
    long          m_defIndex;                      // +0x1c
    long          m_reserved;                      // +0x20
    unsigned char m_flags;                         // +0x24
    CSubDefClassBase();                            // 0x0044C0C0
};

class CPrizeScoreDef : public CSubDefClassBase { public:         // vtable 0x012437D4, 0x30 bytes
    float Score;                                   // +0x28
    long  Mult;                                    // +0x2c
    CPrizeScoreDef() {}
};

struct CPrizeScoreDefVector {                      // std::vector<CPrizeScoreDef>
    unsigned char _Alval;
    CPrizeScoreDef* _Myfirst;
    CPrizeScoreDef* _Mylast;
    CPrizeScoreDef* _Myend;
    void resize(unsigned int n, const CPrizeScoreDef& value);   // 0x004EDA6C
    void resize(unsigned int n);
};

void CPrizeScoreDefVector::resize(unsigned int n)
{
    resize(n, CPrizeScoreDef());
}