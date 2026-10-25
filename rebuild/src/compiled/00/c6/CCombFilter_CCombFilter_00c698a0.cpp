// CCombFilter::~CCombFilter() -- the compiler-generated destructor of a libpk reverb comb filter: nothing of its own, so only the CDspObject base (0x00C74450), reached by a tail jump
// Leaf: ??1CCombFilter@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C698A0, 5 bytes (`jmp 0x00C74450`, + int3 padding).
// Identity (sweep1, 2026-10-25; follows FINDINGS chunk 444, the libpk reverb family):
//   - the scalar deleting destructor 0x00C699F0 (the only slot of the comb vtable 0x012B9588, stored by the comb
//     constructor 0x00C69880) calls this;
//   - the reverb constructor 0x00C6A650 and destructor 0x00C6A780 (agent443's CReverbProcessor, vtable 0x012B9594) pass it
//     to the `eh vector` iterators as the element destructor of their four combs at +0x1f8 (element 0x14 bytes);
//   - its combs are the ones the landed ProcessCombs 0x00C69FB0 walks (blend mode +0x10).
//   The landed CTCHeroStats::CTCHeroStats is a propagated manifest label; descriptive names (libpk: no PDB/RTTI).
// Shape: no store of 0x012B9588 because the destructor is IMPLICIT (a compiler-generated destructor does not reset the
//   vptr). The array holder below is a TU device: destroying an array member makes the compiler pass the element
//   destructor's address to the vector destructor iterator, which emits this COMDAT out of line (retail: the reverb's
//   own +0x1f8 array).
class CDspObject {                                      // vtable 0x012BF050: the reverb DSP parts' base
public:
    CDspObject();                                       // 0x00C74420
    virtual ~CDspObject();                              // 0x00C74450
};

class CCombFilter : public CDspObject {                 // vtable 0x012B9588, 0x14 bytes (destructor compiler-generated)
public:
    CCombFilter();                                      // 0x00C69880
    unsigned char _pad04[0x0c];
    long          m_mode;                               // +0x10 blend mode
};

class CReverbCombs {                                    // the reverb's four comb filters (CReverbProcessor +0x1f8)
public:
    ~CReverbCombs();
    CCombFilter m_combs[4];
};

CReverbCombs::~CReverbCombs()
{
}
