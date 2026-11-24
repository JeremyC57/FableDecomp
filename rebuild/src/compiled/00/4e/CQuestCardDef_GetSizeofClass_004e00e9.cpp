// Leaf: ?GetSizeofClass@CQuestCardDef@@UAEKXZ
#pragma optimize("s",on)
class CQuestCardDef { public:
    char data[0x70];
    virtual unsigned long GetSizeofClass();
};

unsigned long CQuestCardDef::GetSizeofClass()
{
    return sizeof(CQuestCardDef);
}