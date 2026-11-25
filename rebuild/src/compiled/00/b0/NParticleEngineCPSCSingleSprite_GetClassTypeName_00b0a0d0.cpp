// NParticleEngine::CPSCSingleSprite::GetClassTypeName() const -- the class-type name "CPSCSingleSprite" as a CCharString
// Leaf: ?GetClassTypeName@CPSCSingleSprite@NParticleEngine@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B0A0D0, 25 bytes: push esi; mov esi,[esp+8]; push -1; push 0x0129F544 "CPSCSingleSprite"; mov ecx,esi;
//   call CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax,esi; pop esi; ret 4 -- a const virtual __thiscall returning
//   CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one stack argument (kept in esi).
// Identity (NF417, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell
//   ?GetClassTypeName@CPSCSingleSprite@NParticleEngine@@UBE?AVCCharString@@XZ; retail vftable 0x0129FB84 (RTTI
//   .?AVCPSCSingleSprite@NParticleEngine@@) slot 3 (0x0129FB90 -> 0x00B0A0D0) is NParticleEngine::CPSCBase's pure `virtual CCharString
//   GetClassTypeName() const` (engine/CPSCBase.h: GetUpdateParam, the destructor, DatabaseClone, GetClassTypeName); the literal
//   0x0129F544 = "CPSCSingleSprite" = the class name (read from the exe). Neither name set has a StatGetClassTypeName for CPSC classes,
//   so the body returns the literal directly (the landed 0x00B1AE00 CPSCLight form).
// Respell (NF417): was the stand-in self ?GetClassTypeName@CPSCSingleSprite_Methods@@QAE?AVCCharString@@XZ (a non-virtual member of a
//   made-up `_Methods` class).
// Header: engine/CPSCSingleSprite.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form); its polymorphic branch compiles
//   with the prelude FABLE_RETAIL_CPSCSPLINEBASE_PASS23 (the smallest prelude, from the header's landed includers, minimised line by
//   line, under which the polymorphic check TU compiles: static_cast to NParticleEngine::CPSCBase,
//   &NParticleEngine::CPSCBase::GetClassTypeName, sizeof(class) == sizeof(layout); NF421 tools/psearch.py). Port (NF421, T3 `nf417
//   headers debt`): was a local class over CPSCBase; no header edit was needed.
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CPSCSPLINEBASE_PASS23
#define CPSCSingleSprite CPSCSingleSpriteLayout
#include "engine/CPSCSingleSprite.h"
#undef CPSCSingleSprite

namespace NParticleEngine {
class CPSCSingleSprite : public CPSCSingleSpriteLayout
{
public:
    virtual CCharString GetClassTypeName() const;
};
}

CCharString NParticleEngine::CPSCSingleSprite::GetClassTypeName() const
{
    return CCharString("CPSCSingleSprite", -1);
}
