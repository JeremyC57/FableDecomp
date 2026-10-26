// Byte-exact reconstruction of CTCKraken::InitTentaclesAndHead @ 0x007fc6f0
// Leaf: InitTentaclesAndHead

struct ListNode {
    ListNode* next;
};

class CTCKraken { public:
    char pad[0x44];
    ListNode* list; // +0x44

public:
    void RaiseSunkenTentacles();   // 0x007FB610
};

// callees (thiscall members modeled as __fastcall free fns)
void __fastcall Kraken_sub_7fc210(CTCKraken* self);
void __fastcall Kraken_sub_7fb150(CTCKraken* self);

void __fastcall InitTentaclesAndHead(CTCKraken* self)
{
    ListNode* n = self->list;
    if (n->next == n) {
        Kraken_sub_7fc210(self);
    }
    ((CTCKraken*)(self))->RaiseSunkenTentacles();
    Kraken_sub_7fb150(self);
}