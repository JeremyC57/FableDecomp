// CEngineDef::operator=(const CEngineDef&) -- the implicit copy assignment: the parent-def base, then the seventeen engine tuning values
// Leaf: ??4CEngineDef@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00434370, length 128 bytes, `ret 4`, returns this.
// Name: CEngineDef's vtable 0x0123118C slot 19 is the stub 0x00434360 (`jmp 0x00434370`: the virtual copy tail-calling this
// assignment, as CControlsDef's 0x00431E70 -> 0x00431E80); the dwords copied are exactly Ego_r.pdb CEngineDef's members. The
// catalog's "CParentDefClassBase_CopyAssign_2" is a label.
// Callee: CParentDefClassBase::operator= @ 0x00431ED0 (landed, compiler-generated).
// Layout: Ego_r.pdb CEngineDef (132 bytes) -> retail 0x80: CParentDefClassBase (pack 1, 0x39 bytes, the landed 0x00431ED0 model),
// then from +0x3C (PDB +0x40) nine floats LODErrorTolerance .. FOV_2D and eight longs LocalDetailBooleanAlphaDefaultAlphaRef ..
// InvalidThemeStandin (+0x7C). No engine header exists for CEngineDef; the base is local for the landed 0x00431ED0 reason.
// Shape: /O2; compiler-generated member-wise assignment -- the base call, then one dword move per member (the floats too: an
// implicit copy moves float members through integer registers, eax/ecx/edx rotating).
// It is emitted out of line by an assignment in a helper (retail's caller is the slot-19 stub).
#pragma pack(push, 1)
// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#include "/tmp/claude-0/-home-user-FableDecomp/a013f3ec-d440-5b47-87a1-ed975cb80dfb/scratchpad/mig3/work/x/chain.h"
#pragma pack(pop)
class CEngineDef : public CParentDefClassBase {
public:
    float LODErrorTolerance;                             // +0x3C
    float CharacterLODErrorTolerance;                    // +0x40
    float LODErrorFactor;                                // +0x44
    float SeaHeight;                                     // +0x48
    float GamePrimitiveDefaultFadeStart;                 // +0x4C
    float GamePrimitiveDefaultFadeRangeRatio;            // +0x50
    float LocalDetailDefaultFadeStart;                   // +0x54
    float LocalDetailDefaultFadeRangeRatio;              // +0x58
    float FOV_2D;                                        // +0x5C
    long  LocalDetailBooleanAlphaDefaultAlphaRef;        // +0x60
    long  DefaultPrimitiveAlphaRef;                      // +0x64
    long  TestStaticMesh;                                // +0x68
    long  TestAnimatedMesh;                              // +0x6C
    long  TestAnim;                                      // +0x70
    long  TestGraphic;                                   // +0x74
    long  InvalidTextureStandin;                         // +0x78
    long  InvalidThemeStandin;                           // +0x7C
};
void AssignCEngineDef(CEngineDef& to, const CEngineDef& from)   // emission helper, not a retail function
{
    to = from;
}