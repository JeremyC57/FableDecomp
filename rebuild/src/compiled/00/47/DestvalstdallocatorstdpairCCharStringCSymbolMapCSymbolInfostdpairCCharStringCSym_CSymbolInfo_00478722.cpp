#pragma optimize("s",on)
// Leaf: CSymbolInfo>_>
// Byte-exact reconstruction of _Dest_val<allocator<pair<CCharString,CSymbolMap::CSymbolInfo>>, pair<...>>
// retail 0x00478722
//
// void __fastcall _Dest_val(allocator* self_alloc, pair* p)
// The pair layout: first = CCharString at +0, second = CSymbolInfo at +0x40.
// Destroys second (CSymbolInfo) which itself has an inner subobject at +0 that
// needs (ecx=*(sec+0), edx=*(sec+4)) then the CSymbolInfo dtor, then CCharString dtor.

// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
class CCharString { public:
    void* p;
};

// CSymbolInfo has, at its start, an inner object destroyed via a helper that takes
// two loaded dwords (ecx, edx). Then the CSymbolInfo dtor itself.
struct InnerVal { void* a; void* b; };

struct CSymbolInfo {
    InnerVal inner;   // at +0 / +4
};

struct SymPair {
    CCharString first;      // +0
    char pad[0x40 - sizeof(CCharString)];
    CSymbolInfo second;     // +0x40
};

// callee 0x478751: destroys the inner val given the two loaded dwords in ecx/edx.
extern "C" void __fastcall destroy_inner_val(void* ecx_val, void* edx_val);
// callee 0x478743: CSymbolInfo dtor-helper on (self+0x40).
extern "C" void __fastcall symbolinfo_dtor(CSymbolInfo* self);
// tail 0x430300: CCharString dtor on (self+0).

void __fastcall _Dest_val_CSymbolInfo___(SymPair* p)
{
    CSymbolInfo* sec = &p->second;
    destroy_inner_val(sec->inner.a, sec->inner.b);
    symbolinfo_dtor(sec);
    ((CParentDefClassBase*)(&p->first))->CParentDefClassBase::~CParentDefClassBase();
}