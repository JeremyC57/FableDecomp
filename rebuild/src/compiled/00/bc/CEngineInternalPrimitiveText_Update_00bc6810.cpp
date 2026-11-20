// Leaf: ?Update@CEngineInternalPrimitiveText@@QAEXAAUP@@K@Z
// Naming (NF129, 2026-11-20, q4933): the 4-byte member at +0x60 (and the primitive's at +0x20) is a CWideString: the call is
//   CWideString::operator=(const CWideString&) 0x0099B7D0 (Ego_r.pdb / FableWin.pdb ??4CWideString@@QAEABV0@ABV0@@Z; fable_string.h), so the
//   stand-in `struct Sub { void M(void*); }` is replaced by the shared CWideString (in place: same offsets).
#include "fable_string.h"
struct V { int x,y,z; };
struct P { char a[0xc]; V vc; int f18,f1c; CWideString f20; int f24; unsigned char f28; };
struct CEngineInternalPrimitiveText { char a[0xc]; unsigned long fc; char b[0x48-0x10]; V v48; int f54,f58; unsigned char f5c; char c[0x60-0x5d]; CWideString sub60; int f64; void Update(P& p, unsigned long k); };
void CEngineInternalPrimitiveText::Update(P& p, unsigned long k){ this->fc=k; this->v48=p.vc; this->f54=p.f18; this->f58=p.f1c; this->f5c=p.f28; this->sub60 = p.f20; this->f64=p.f24; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?Update@CEngineInternalPrimitiveText@@UAEXABVCEnginePrimitive@@K@Z); evidence: RTTI vtable slot + PDB name set.
