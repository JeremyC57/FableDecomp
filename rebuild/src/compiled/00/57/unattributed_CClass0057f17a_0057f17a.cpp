// CClass_0057f17a::CClass_0057f17a(const CAllocator&) -- constructor: constructs its CTCHeroStats::MirroredCriminalStatus member through the out-of-line ctor @ 0x0057A5DD; the allocator argument is unused
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057F17A, oracle length 14 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  push esi ; mov esi, ecx ; call 0x57a5dd ; mov eax, esi ; pop esi ; ret 4
// Tiny-body shape `CtorInitUnusedArg`: a constructor whose one argument (an empty allocator or a
// tag) is not read; `this` is kept in esi for the return while the out-of-line initialiser runs.
// The manifest label is propagated; the class is named from it when it is a plain class name.
struct CAllocator { };

class CTCHeroStats { public:
    class MirroredCriminalStatus { public:
        MirroredCriminalStatus();                    // 0x0057A5DD
    };
};

struct CClass_0057f17a {
    CTCHeroStats::MirroredCriminalStatus m_0;   // +0x0 (out-of-line ctor 0x0057A5DD)
    unsigned char _body[4 - 1];
    CClass_0057f17a(const CAllocator&);
};

CClass_0057f17a::CClass_0057f17a(const CAllocator&)
    : m_0()
{
}