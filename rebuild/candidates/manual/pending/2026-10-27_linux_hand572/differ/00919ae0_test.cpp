// Behaviour test for 0x00919AE0 CAIStateGroup_BuyFromSeller::Update (model copy of the control flow;
// engine services, actions, messages and the RNG are stubs that log what the state group asks for).
#include <cstdio>
#include <cstring>

struct C3DVector {
    float X, Y, Z;
    C3DVector() : X(0), Y(0), Z(0) { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};

// --- stubs ----------------------------------------------------------------------------------------------------
static char g_log[128]; static int g_logn = 0;
static void Log(char c) { g_log[g_logn++] = c; g_log[g_logn] = 0; }
static long g_frame = 100; static long g_fps = 30;
static float g_proximity = 1.0f;                       // g_BuyFromSellerProximity 0x0138C690
static int g_moveResults[8]; static int g_moveN = 0;   // AIAction_MoveToPosProximity results, in order
static int g_speakResults[8]; static int g_speakN = 0; // AIAction_SpeakAISpeechToTarget results, in order
static int g_doNothing = 2;
static bool g_workTime = true;
static int g_replyAfterPolls = -1; static int g_polls = 0; static long g_replyExtra = 0;
static unsigned long g_randomPick = 0; static unsigned long g_lastRange = 0;
static unsigned long g_spoken[8]; static int g_spokenN = 0;
static int g_messagesLive = 0;
static bool g_snap = true; static int g_killAtPoll = -1; static int g_stopWorkAfterMoves = -1;

struct CThing {
    C3DVector Pos; bool Dead; bool IsStockItem; unsigned long BuyersPhrase; bool IsShopKeeper;
    CThing() : Dead(false), IsStockItem(false), BuyersPhrase(0), IsShopKeeper(false) { }
};
struct CMessageEvent { long ExtraData; bool Live; };
static CMessageEvent g_request, g_reply;

static float Dist2(const CThing& a, const CThing& b)
{
    float x = a.Pos.X - b.Pos.X, y = a.Pos.Y - b.Pos.Y, z = a.Pos.Z - b.Pos.Z;
    return x * x + y * y + z * z;
}
static bool IsFurtherThan(const CThing& a, const CThing& b, float d) { return Dist2(a, b) > d * d; }
static bool IS_THING_ALIVE(const CThing* t) { return t != 0 && !t->Dead; }

struct CAIStateGroup_BuyFromSeller {
    CThing* Creature; CThing* Participants[4]; int ParticipantCount;
    CThing* ObjectList[8]; int ObjectCount; bool HasShop;
    unsigned long ThankingPhrase, IgnoringPhrase; long FrameLastBought;
    CAIStateGroup_BuyFromSeller() : Creature(0), ParticipantCount(0), ObjectCount(0), HasShop(true),
        ThankingPhrase(0x77), IgnoringPhrase(0x66), FrameLastBought(-1) { }
    void Terminate() { Log('T'); }
    bool IsInWorkHours() const { return g_workTime; }
    int Move()
    {
        Log('V');
        int r = g_moveResults[g_moveN++];
        if (g_moveN == g_stopWorkAfterMoves) g_workTime = false;
        if (r == 2 && g_snap) { C3DVector p = Participants[1]->Pos; Creature->Pos = C3DVector(p.X - 0.5f, p.Y, p.Z); }
        return r;
    }
    int Speak(unsigned long phrase) { Log('S'); g_spoken[g_spokenN++] = phrase; return g_speakResults[g_speakN++]; }
    int DoNothing() { Log('N'); return g_doNothing; }
    CMessageEvent* AddMessage() { Log('A'); g_request.Live = true; ++g_messagesLive; return &g_request; }
    void RemoveMessage(const CMessageEvent* m) { Log(m == &g_request ? 'q' : 'r'); --g_messagesLive; }
    const CMessageEvent* GetSellersReply(const CThing*)
    {
        if (g_killAtPoll == g_polls) Participants[1]->Dead = true;
        if (g_replyAfterPolls >= 0 && g_polls++ >= g_replyAfterPolls) {
            g_reply.ExtraData = g_replyExtra; ++g_messagesLive; return &g_reply;
        }
        if (g_replyAfterPolls < 0) ++g_polls;
        g_frame += 10;                                     // each poll costs a few frames
        return 0;
    }
    void RemoveFromCreatureInteraction() { Log('X'); }

    // the reconstructed control flow of 0x00919AE0
    void Update()
    {
        CThing* seller = 0;
        for (int i = 0; i < ParticipantCount; ++i)
            if (Participants[i] != Creature) seller = Participants[i];
        if (!seller) { Terminate(); return; }
        if (!IsFurtherThan(*seller, *Creature, g_proximity)) { Terminate(); return; }
        int result;
        do {
            if (!IsInWorkHours()) { Terminate(); return; }
            if (!IS_THING_ALIVE(seller)) { Terminate(); return; }
            result = Move();
        } while (result == 4);
        if (result != 2) { Terminate(); return; }
        if (!seller->IsShopKeeper) { Terminate(); return; }
        if (!HasShop) { Terminate(); return; }
        CThing* stock[8]; unsigned long count = 0;
        for (int i = 0; i < ObjectCount; ++i)
            if (IS_THING_ALIVE(ObjectList[i]) && ObjectList[i]->IsStockItem) stock[count++] = ObjectList[i];
        if (count == 0) { Terminate(); return; }
        g_lastRange = count;
        unsigned long index = g_randomPick % count;
        CThing* item = stock[index];
        unsigned long phrase = item->BuyersPhrase;
        CMessageEvent* request = AddMessage();
        Speak(phrase);
        bool accepted;
        long timeout = g_frame + 20 * g_fps;
        const CMessageEvent* reply;
        while ((reply = GetSellersReply(seller)) == 0) {
            if (!IsInWorkHours() || !IS_THING_ALIVE(seller)) { RemoveMessage(request); Terminate(); return; }
            if (timeout < g_frame) break;
            if (IsFurtherThan(*seller, *Creature, g_proximity)) {
                do {
                    if (!IsInWorkHours() || !IS_THING_ALIVE(seller)) { Terminate(); return; }
                    result = Move();
                } while (result == 4);
                if (result == 1) { RemoveMessage(request); Terminate(); return; }
            } else if (DoNothing() == 1) {
                RemoveMessage(request);
                return;
            }
        }
        if (reply) { RemoveMessage(reply); accepted = reply->ExtraData != 0; }
        else { RemoveMessage(request); accepted = false; }
        unsigned long reaction = accepted ? ThankingPhrase : IgnoringPhrase;
        do {
            if (!IsInWorkHours() || !IS_THING_ALIVE(seller)) { Terminate(); return; }
            result = Speak(reaction);
        } while (result == 4);
        FrameLastBought = g_frame;
        RemoveFromCreatureInteraction();
        Terminate();
    }
};

static CThing g_me, g_seller, g_items[4];
static void Reset(CAIStateGroup_BuyFromSeller& s)
{
    g_logn = 0; g_log[0] = 0; g_frame = 100; g_moveN = 0; g_speakN = 0; g_doNothing = 2; g_workTime = true;
    g_replyAfterPolls = -1; g_polls = 0; g_replyExtra = 0; g_randomPick = 0; g_lastRange = 0; g_spokenN = 0;
    g_messagesLive = 0; g_snap = true; g_killAtPoll = -1; g_stopWorkAfterMoves = -1; g_proximity = 1.0f;
    g_me = CThing(); g_seller = CThing(); g_seller.Pos = C3DVector(5, 0, 0); g_seller.IsShopKeeper = true;
    for (int i = 0; i < 4; ++i) { g_items[i] = CThing(); g_items[i].BuyersPhrase = 0x10 + i; g_items[i].IsStockItem = true; }
    g_items[1].Dead = true; g_items[2].IsStockItem = false;
    s = CAIStateGroup_BuyFromSeller();
    s.Creature = &g_me; s.Participants[0] = &g_me; s.Participants[1] = &g_seller; s.ParticipantCount = 2;
    for (int i = 0; i < 4; ++i) s.ObjectList[i] = &g_items[i];
    s.ObjectCount = 4;
    for (int i = 0; i < 8; ++i) { g_moveResults[i] = 2; g_speakResults[i] = 2; }
}

static bool EndsWith(const char* s, const char* t) { size_t a = std::strlen(s), b = std::strlen(t); return a >= b && std::strcmp(s + a - b, t) == 0; }
static int Count(const char* s, char c) { int n = 0; for (; *s; ++s) if (*s == c) ++n; return n; }

int main()
{
    int bad = 0;
    CAIStateGroup_BuyFromSeller s;

    Reset(s); s.ParticipantCount = 1; s.Update();                           // nobody to buy from
    if (std::strcmp(g_log, "T") != 0) { std::printf("case 1 %s\n", g_log); ++bad; }

    Reset(s); g_seller.Pos = C3DVector(0.5f, 0, 0); s.Update();             // seller already within reach
    if (std::strcmp(g_log, "T") != 0) { std::printf("case 2 %s\n", g_log); ++bad; }

    Reset(s); g_workTime = false; s.Update();                               // not working hours
    if (std::strcmp(g_log, "T") != 0) { std::printf("case 3 %s\n", g_log); ++bad; }

    Reset(s); g_moveResults[0] = 1; s.Update();                             // the walk failed
    if (std::strcmp(g_log, "VT") != 0) { std::printf("case 4 %s\n", g_log); ++bad; }

    Reset(s); g_seller.IsShopKeeper = false; s.Update();                    // not a shopkeeper
    if (std::strcmp(g_log, "VT") != 0) { std::printf("case 5 %s\n", g_log); ++bad; }

    Reset(s); s.ObjectCount = 3; g_items[0].Dead = true; s.Update();        // nothing alive for sale
    if (std::strcmp(g_log, "VT") != 0) { std::printf("case 6 %s\n", g_log); ++bad; }

    // walk twice, buy the second live stock item (item 3), DoNothing once while waiting, seller accepts, thank him, leave
    Reset(s); g_moveResults[0] = 4; g_randomPick = 1; g_replyAfterPolls = 1; g_replyExtra = 1; s.Update();
    if (std::strcmp(g_log, "VVASNrSXT") != 0 || g_lastRange != 2 || g_spoken[0] != 0x13 || g_spoken[1] != 0x77
        || s.FrameLastBought != g_frame || g_messagesLive != 1) { std::printf("case 7 %s\n", g_log); ++bad; }   // the reply is dropped, the request is left to the seller

    // seller refuses: ignoring phrase
    Reset(s); g_replyAfterPolls = 0; g_replyExtra = 0; s.Update();
    if (std::strcmp(g_log, "VASrSXT") != 0 || g_spoken[0] != 0x10 || g_spoken[1] != 0x66) { std::printf("case 8 %s\n", g_log); ++bad; }

    // seller never answers: DoNothing until the 20-second timeout, drop our request, ignoring phrase
    Reset(s); s.Update();
    if (std::strncmp(g_log, "VASN", 4) != 0 || !EndsWith(g_log, "NqSXT") || Count(g_log, 'N') != 60
        || g_spoken[1] != 0x66 || g_messagesLive != 0) { std::printf("case 9 %s\n", g_log); ++bad; }

    // DoNothing reports failure: drop the request and return without Terminate
    Reset(s); g_doNothing = 1; s.Update();
    if (std::strcmp(g_log, "VASNq") != 0 || g_messagesLive != 0) { std::printf("case 10 %s\n", g_log); ++bad; }

    // seller dies while we wait
    Reset(s); g_killAtPoll = 0; s.Update();
    if (std::strcmp(g_log, "VASqT") != 0) { std::printf("case 11 %s\n", g_log); ++bad; }

    // seller wanders off while we wait, the follow-up walk fails: drop the request, Terminate
    Reset(s); g_snap = false; g_moveResults[1] = 1; s.Update();
    if (std::strcmp(g_log, "VASVqT") != 0) { std::printf("case 12 %s\n", g_log); ++bad; }

    // the follow-up walk is cut short by the end of working hours: Terminate without dropping the request (as retail)
    Reset(s); g_snap = false; g_moveResults[1] = 4; g_stopWorkAfterMoves = 2; s.Update();
    if (std::strcmp(g_log, "VASVT") != 0 || g_messagesLive != 1) { std::printf("case 13 %s\n", g_log); ++bad; }

    // final thanks loop repeats while the speech action says "continue"
    Reset(s); g_replyAfterPolls = 0; g_replyExtra = 1; g_speakResults[1] = 4; g_speakResults[2] = 4; s.Update();
    if (std::strcmp(g_log, "VASrSSSXT") != 0) { std::printf("case 14 %s\n", g_log); ++bad; }

    if (bad) { std::printf("00919ae0_TEST FAIL\n"); return 1; }
    std::printf("00919ae0_TEST PASS\n");
    return 0;
}
