// Behaviour test for 0x007022C0 CTCQuestCard::IsOptional (plain C++, links the row object).
#include <stdio.h>
class CQuestCardDef { public: unsigned char Pad[0x58]; bool IsCore; bool IsVignette; bool IsExclusive; bool Route; };
class CTCQuestCard { public:
    void* VTable; unsigned char Pad04[8];
    CQuestCardDef* PDef;                    // +0x0c
    bool Finished; bool Replayable; unsigned char Pad12[2];
    long FeatIndex;                         // +0x14
    unsigned char Rest[0x1c];
    bool IsOptional() const;
};
int main()
{
    CQuestCardDef def; def.IsCore = true;
    CTCQuestCard card; card.PDef = &def;
    card.FeatIndex = -1;
    if (card.IsOptional()) { printf("FAIL core\n"); return 1; }
    def.IsCore = false;
    if (!card.IsOptional()) { printf("FAIL non-core\n"); return 1; }
    def.IsCore = true; card.FeatIndex = 0;
    if (!card.IsOptional()) { printf("FAIL feat 0\n"); return 1; }
    card.FeatIndex = 7;
    if (!card.IsOptional()) { printf("FAIL feat 7\n"); return 1; }
    card.FeatIndex = -5;
    if (card.IsOptional()) { printf("FAIL -5\n"); return 1; }
    card.PDef = 0; card.FeatIndex = 3;      // a feat card never reads the def
    if (!card.IsOptional()) { printf("FAIL no def\n"); return 1; }
    printf("OK_007022c0\n");
    return 0;
}
