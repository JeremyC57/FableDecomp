#pragma optimize("s",on)
struct CFieldA { int a; };
struct CFieldB { int b; };

class CPersistContext { public:
    template <class T> void Transfer(T& value);
    void TransferB(CFieldB* p);
};

class CCombatAbilityStrafeDef { public:
    unsigned char _pad28[0x28];
    CFieldB f28;
    unsigned char _pad40[0x40 - 0x28 - sizeof(CFieldB)];
    CFieldA f40;
};

void __fastcall CCombatAbilityStrafeDef_Transfer(CCombatAbilityStrafeDef* def, int, CPersistContext* ctx)
{
    ctx->Transfer(*(float*)(&def->f40));
    ctx->TransferB(&def->f28);
}