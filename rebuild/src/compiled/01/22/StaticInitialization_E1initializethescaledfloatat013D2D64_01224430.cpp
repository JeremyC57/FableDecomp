// StaticInitialization::_$E1 -- initialize the scaled float at013D2D64.
// Leaf: _$E1
// Identity: descriptive data/function identity, proven static initializer role; Own native static initializer calls00a76320 (loads float0.5 from0122f59c with no native args), multiplies actual0.9f literal01253af0 and stores exactglobal013D2D64. Original global/provider names and provider highlevel role unknown; old GetCombatMinDistance is unsupported catalog propagation. Cdecl zeroarg float-return projection matches actual call ABI; not adopted from donor-address labels.
// Layout: actual single float global; compiler emits initialization helper from ordinary dynamic global initializer. No applicable shared class header.
#ifndef _MSC_VER
#define __cdecl
#endif
extern "C" float __cdecl CUnprovenFloatProvider_00A76320();
float g_UnprovenScaledFloat_013D2D64=CUnprovenFloatProvider_00A76320()*0.9f;