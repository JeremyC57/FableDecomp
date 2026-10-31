// CClass_00cd2b78::CClass_00cd2b78(const CAllocator&) -- constructor: its CScriptGameResourceObjectScriptedThingBase base is built by the out-of-line constructor @ 0x007E72A0; the allocator argument is unused
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CD2B78, oracle length 14 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  push esi ; mov esi, ecx ; call 0x7e72a0 ; mov eax, esi ; pop esi ; ret 4
// Tiny-body shape `CtorInitUnusedArg`: a constructor whose one argument (an empty allocator or a
// tag) is not read; `this` is kept in esi for the return while the out-of-line initialiser runs.
// The manifest label is propagated; the class is named from it when it is a plain class name.
struct CAllocator { };

class CScriptGameResourceObjectScriptedThingBase { public:   // Ego_r.pdb (default constructor 0x007E72A0)
    CScriptGameResourceObjectScriptedThingBase();               // 0x007E72A0
    unsigned char _body[4];
};

struct CClass_00cd2b78 : CScriptGameResourceObjectScriptedThingBase {
    CClass_00cd2b78(const CAllocator&);
};

CClass_00cd2b78::CClass_00cd2b78(const CAllocator&)
{
}