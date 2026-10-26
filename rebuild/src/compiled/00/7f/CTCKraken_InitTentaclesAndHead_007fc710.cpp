// CTCKraken::InitTentaclesAndHead @ 0x007fc710
// Leaf: InitTentaclesAndHead
struct ListNode { ListNode* next; };
class CTCKraken { public:
    char pad[0x44];
    ListNode* list; // +0x44

public:
    void RaiseSunkenTentacles();   // 0x007FB610
};

void __fastcall CTCKraken_ClearTentacles(CTCKraken* self);   // 0x7fc210
void __fastcall CTCKraken_SetupHead(CTCKraken* self);        // 0x7fb150

void __fastcall InitTentaclesAndHead(CTCKraken* self)
{
    ListNode* head = self->list;
    if (head->next == (ListNode*)head)
        CTCKraken_ClearTentacles(self);
    ((CTCKraken*)(self))->RaiseSunkenTentacles();
    CTCKraken_SetupHead(self);
}