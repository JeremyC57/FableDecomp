// Retail 0x004c3360
// Leaf: ?_Dest_val_SymbolPair@@YIXPAUSymbolPair@@0@Z
// _Dest_val<allocator<pair<CCharString,CSymbolMap::CSymbolInfo> >, pair<...> >
//
// Body:
//   push esi; mov esi,ecx
//   lea ecx,[esi+0x30]; call 0x99a2e0   ; CCharString::~CCharString on sub at +0x30
//   lea ecx,[esi+0x14]; call 0x99a2e0   ; CCharString::~CCharString on sub at +0x14
//   mov ecx,esi; pop esi; jmp 0x4c2e90  ; tail-call pair-level dtor on self
//
// The two identical calls destroy two CCharString subobjects (at +0x14 and
// +0x30); the tail-jmp runs the pair-level destructor on self. All three are
// external non-virtual __fastcall helpers; the harness masks the relocation
// dwords so any consistent targets reproduce the bytes.

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CCharString;
struct SymbolPair;

// Naming (NF450, 2026-11-25): callee-only respell of the tail call to the PDB destructor (T1 tracker; target self row's Leaf),
//   0x004C2E90 ??1C3DAnimationInterpolator@@UAE@XZ (was @SymbolPair_Dtor@4), a qualified call through engine/C3DAnimationInterpolator.h
//   (ruling r2; r449d: the shared header, not a bodiless class).
#include "engine/C3DAnimationInterpolator.h"           // ~C3DAnimationInterpolator 0x004C2E90

class CCharString { public:};

struct SymbolPair
{
    char        pad0[0x14];   // 0x00 .. 0x13  (pair.first CCharString lives here)
    CCharString strA;         // 0x14  (subobject destroyed second)
    char        pad1[0x30 - 0x14 - 1];
    CCharString strB;         // 0x30  (subobject destroyed first)
};

// self (allocator/pair base) arrives in ecx; the second pair* param (edx) is
// unused by the body, matching retail.
void __fastcall _Dest_val_SymbolPair(SymbolPair *self, SymbolPair *unused)
{
    ((CBaseClass*)(&self->strB))->CBaseClass::~CBaseClass();   // lea ecx,[esi+0x30]; call
    ((CBaseClass*)(&self->strA))->CBaseClass::~CBaseClass();   // lea ecx,[esi+0x14]; call
    reinterpret_cast<C3DAnimationInterpolator*>(self)->C3DAnimationInterpolator::~C3DAnimationInterpolator();   // mov ecx,esi; jmp (tail call)
}