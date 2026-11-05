// Leaf: ?Transfer@CWhisperBattleDef@@QAEXAAVCPersistContext@@@Z
#pragma optimize("s",on)
struct CSubA { char b[4]; };
struct CSubB { char b[4]; };

class CPersistContext { public:
    // this=ecx=ctx, member pointer pushed on stack -> __fastcall push;call
    template <class T> void Transfer(T& value);   // callee at 0x431102
};

class CWhisperBattleDef { public:
    char pad3c[0x3c];
    CSubA m3c;   // sub-object at +0x3c
    CSubB m40;   // sub-object at +0x40
    void Transfer(CPersistContext& ctx);
    void TransferB(CPersistContext& ctx); // base transfer at 0x4595a3, takes &self->m40
};

void CWhisperBattleDef::Transfer(CPersistContext& ctx)
{
    ctx.Transfer(*(long*)(&this->m3c));
    // second call: this=ecx=ctx, pushes &self->m40, target 0x4595a3
    reinterpret_cast<CPersistContext*>(&ctx)->Transfer(*(long*)(reinterpret_cast<CSubA*>(&this->m40)));
}
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?Transfer@CWhisperBattleDef@@UAEXAAVCPersistContext@@@Z); evidence: RTTI vtable slot + PDB name set.
