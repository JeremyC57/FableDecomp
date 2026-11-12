// std::vector<CArenaWaveDef>::~vector() -- STLport 4.6 vector destructor: each element's virtual destructor over [_M_start, _M_finish), then the buffer freed
// Leaf: ??1?$vector@VCArenaWaveDef@@V?$allocator@VCArenaWaveDef@@@std@@@std@@QAE@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F25B70, oracle length 50 bytes.
// Name (agentNF23, name-fix queue land1011da): agentFAM07: ~vector<CArenaWaveDef> called by the renamed scalar deleting destructor 0x00F25B40 (graded RELOCATION_MATCH, shared engine headers); element stride of the body matches sizeof(CArenaWaveDef); CArenaRoundDef::Waves is Ego_r.pdb vector<CArenaWaveDef>, and the body steps
//   0x3C-byte polymorphic elements (sizeof(CArenaWaveDef) in retail STLport layout), destroying each through vtable slot 0 with flag 0.
//   Was the generic stand-in CUnprovenMember_00f25b40 (??1CUnprovenMember_00f25b40@@QAE@XZ) / catalog CRetailVector_poly0x3c::~CRetailVector_poly0x3c.
// Callees: the element's virtual (scalar deleting) destructor, slot 0 with 0; free 0x00BFEA14 (allocator::deallocate, null-checked).
// Model: the shared STLport 4.6 vector (stlport/fable_containers.h); the explicit instantiation emits the destructor as its own COMDAT.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#include "engine/CArenaRoundDef.h"
class CArenaCreatureDef;
// CArenaWaveDef has no shared header (as in agentFAM07's 0x00F25B40): Ego_r.pdb CArenaWaveDef : CSubDefClassBase, NumWaveCreatures +0x28,
// vector<CArenaCreatureDef> Creatures +0x2C, ShortWave +0x38 (Dinkumware +0x3C; retail STLport vector is 12), 0x3C bytes (this body's
// stride). Only its virtual destructor matters here.
class CArenaWaveDef : public CSubDefClassBase {
public:
    CArenaWaveDef();
    virtual ~CArenaWaveDef();
    long NumWaveCreatures;                                                        // +0x28
    std::vector<CArenaCreatureDef, std::allocator<CArenaCreatureDef> > Creatures; // +0x2c
    bool ShortWave;                                                               // +0x38
};
template std::vector<CArenaWaveDef, std::allocator<CArenaWaveDef> >::~vector();
