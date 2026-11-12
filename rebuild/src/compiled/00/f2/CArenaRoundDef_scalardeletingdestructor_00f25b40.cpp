// CArenaRoundDef::`scalar deleting destructor'(unsigned int flags) -- the implicit destructor (Waves, then the inline base destructors), then free this when bit 0 of flags is set
// Leaf: ??_GCArenaRoundDef@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F25B40, 46 bytes, thiscall, `ret 4`, returns this.
// Identity: slot 0 of vtable 0x012FB288, the vtable CArenaRoundDef::CArenaRoundDef 0x00F34F80 stores (that row's own note names
//   slot 0 0x00F25B40); the member at +0x2C is destroyed by 0x00F25B70, which steps 0x3C-byte polymorphic elements (CArenaWaveDef,
//   Ego_r Waves +0x2C); Ego_r.pdb ??_GCArenaRoundDef@@UAEPAXI@Z / ??1CArenaRoundDef@@UAE@XZ name it.
// Callees: 0x00F25B70 (~vector<CArenaWaveDef>), the CDefPointeeBase vptr 0x01230BA0, CResource::~CResource 0x009FC550,
//   operator delete 0x00BFE9BC.
// Shape (family K050): the compiler-generated destructor stores no vptr of its own (header opt-in
//   FABLE_RETAIL_CARENAROUNDDEF_IMPLICIT_DTOR); the inline ctor below is only this TU's emitter of the vtable and so of ??_G.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CARENAROUNDDEF_IMPLICIT_DTOR
#include "engine/CArenaRoundDef.h"
class CArenaCreatureDef;
// CArenaWaveDef has no shared header: Ego_r.pdb CArenaWaveDef : CSubDefClassBase, NumWaveCreatures +0x28,
// vector<CArenaCreatureDef> Creatures +0x2C, ShortWave +0x38 (Dinkumware +0x3C; retail STLport vector is 12), 0x3C bytes (the
// 0x00F25B70 stride). Only its virtual destructor matters here.
class CArenaWaveDef : public CSubDefClassBase {
public:
    CArenaWaveDef();
    virtual ~CArenaWaveDef();
    long NumWaveCreatures;                                                        // +0x28
    std::vector<CArenaCreatureDef, std::allocator<CArenaCreatureDef> > Creatures; // +0x2c
    bool ShortWave;                                                               // +0x38
};
inline CArenaRoundDef::CArenaRoundDef() { }
CArenaRoundDef* NewArenaRoundDef() { return new CArenaRoundDef; }
