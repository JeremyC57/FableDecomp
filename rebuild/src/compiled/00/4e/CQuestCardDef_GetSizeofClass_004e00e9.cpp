#pragma optimize("s",on)
class CQuestCardDef { public:
    char data[0x74];
    unsigned long GetSizeofClass();
};

unsigned long CQuestCardDef::GetSizeofClass()
{
    return sizeof(CQuestCardDef);
}