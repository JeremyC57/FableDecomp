// CTCKraken::InitTentaclesAndHead @ 0x007fc710
// Leaf: ?CTCKraken_InitTentaclesAndHead@@YIXPAVCTCKraken@@@Z
struct ListNode { ListNode* next; };
class CTCKraken {
public:
    void EndHeadEffects();   // 0x007FB150 (Ego_r.pdb ?EndHeadEffects@CTCKraken@@QAEXXZ)
private: public:
    char pad[0x44];
    ListNode* list; // +0x44

public:
    void RaiseSunkenTentacles();   // 0x007FB610
};

void __fastcall CTCKraken_ClearTentacles(CTCKraken* self);   // 0x7fc210

void __fastcall CTCKraken_InitTentaclesAndHead(CTCKraken* self)
{
    ListNode* head = self->list;
    if (head->next == (ListNode*)head)
        CTCKraken_ClearTentacles(self);
    ((CTCKraken*)(self))->RaiseSunkenTentacles();
    ((CTCKraken*)self)->EndHeadEffects();
}