class CCombatActionBase
{ public:
    int vtbl;
    int IntelligentPointer;
    int field_8;
    int field_c;
    int field_10;
};

extern "C" void __fastcall FrameUpdate_CCombatActionBase(CCombatActionBase* self);

class CCreatureAction_StartBlocking : public CCombatActionBase
{ public:
    void __fastcall FrameUpdate();
};

void __fastcall CCreatureAction_StartBlocking::FrameUpdate()
{
    field_10 = 2;
    FrameUpdate_CCombatActionBase(this);
}