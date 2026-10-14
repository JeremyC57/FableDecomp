# Batch `2026-10-14_linux_eh32` — hand-authored EH-framed bodies: 8 landed

Thirty-second hand pass (Wine lane, `/EHsc`): the animation owner's `CreateNavigationLayer`
(twin of the landed `InitializeAnimationGroup`, 0x628-byte groups), the script loader's
`LoadScript` (profiling lock, 20-byte record builder, range clamping), the `~CActivationReceptor`
destructor (seven-state unwind over the receptor's members), the four `CEventSubscription`
constructors (entry tables of 0x2c / 0xc / 0xb4 bytes built with a raw `malloc` or `new[]` over
the class allocator with `__ehvec_ctor`, the two "Memory allocation failure" warnings from
PKCFrallocStatic.h lines 63 / 65 with the first string and trace homed in the dead argument slot),
and the master object's `LoadScriptInfo` (8-byte profiling lock on +0x10, "Dependancy already
finalised" warning, `new CSBAgents` sized from the reader's count times the stride, the reader's
load handed the agents' buffer and the global slave class object, reset or commit).

Every body is plain compiler-emitted C++ (`#include <string.h>` + `#pragma intrinsic` for the
inline `memset`/`memcpy` where retail inlines them); the shared MFC-style string, trace and lock
helpers are modelled per file as in the earlier passes. Behaviour tests (`*_test.cpp`) exercise
every allocation / warning / early-out path with stand-in types.

Not landed from this pass (parked with evidence in the journal): `00c4cab0` (vtable caching across
two virtual calls), `00caf230` (the DirectShow-style dispatch container: the lane folds the inlined
`&local != NULL` test retail keeps), `00c11d00` (`CTCHero` constructor: store scheduling only),
`00c1a9c0` (async read thread: the lane rotates the top-tested loop).
