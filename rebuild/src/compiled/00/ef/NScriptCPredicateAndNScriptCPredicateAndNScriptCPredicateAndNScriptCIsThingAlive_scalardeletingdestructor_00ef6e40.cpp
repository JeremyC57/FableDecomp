// NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>::`scalar deleting destructor'(unsigned int) -- destroy a three-level And of four CIsThingAlive predicates and free it when bit 0 is set
// Leaf: ??_G
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EF6E40, 90 bytes (+6 int3 padding), `__fastcall`, `ret 4`.
// Callees: NScript::CIsThingAlive::~CIsThingAlive @ 0x00E241B0 (out of line, x4), operator delete @ 0x00BFE9BC.
// Vtables: outer And 0x012F6780, middle And 0x012F6628, inner And 0x012E1B70, the shared CPredicate base 0x012C2FF4;
//   CIsThingAlive is 16 bytes (vtable 0x012C32F0 + a 12-byte CScriptThing, see landed 0x00E6EA30).
// Layout: each CPredicate_And<A,B> is a CPredicate (vptr) followed by its two operands by value, so the nested left
//   operand's vptr sits 4 bytes after its parent's: outer vptr +0x0, middle +0x4, inner +0x8; the four leaves are at
//   +0x0C, +0x1C (inner), +0x2C (middle) and +0x3C (outer). The catalog's `vector deleting destructor` label is the
//   scalar ??_G (no array loop).
// Shape: each And declares a destructor (its own vtable store), destroying the right operand, then the left one (whose
//   inline dtor stores its vtable at +4 / +8 first); the three CPredicate base dtors (user-declared, inline) reset the
//   vptrs innermost first, and VC7.1 merges them into one `mov eax,0x012C2FF4` and three stores (+8, +4, +0).
#include <stddef.h>
void __cdecl operator delete(void* block);               // 0x00BFE9BC

namespace NScript {
struct CScriptThing {                                    // 12 bytes
    void* m_vtable;
    unsigned char m_data[8];
};

template <class T> struct CPredicate {                   // vtable 0x012C2FF4
    virtual ~CPredicate() { }
    virtual bool operator()(const T& thing) const = 0;
};

class CIsThingAlive : public CPredicate<CScriptThing> { public:        // vtable 0x012C32F0, 16 bytes
    CScriptThing m_thing;                                // +0x04
    ~CIsThingAlive();                                    // 0x00E241B0
    bool operator()(const CScriptThing& thing) const;
};

template <class A, class B> class CPredicate_And : public CPredicate<CScriptThing> { public:
    A m_left;                                            // +0x04
    B m_right;
    ~CPredicate_And() { }
    bool operator()(const CScriptThing& thing) const;
};
}

template class NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive, NScript::CIsThingAlive>,
    NScript::CIsThingAlive>, NScript::CIsThingAlive>;