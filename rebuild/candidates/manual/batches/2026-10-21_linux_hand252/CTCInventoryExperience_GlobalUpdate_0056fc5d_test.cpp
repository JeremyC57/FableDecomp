#include <cstdio>
// Behaviour test for CTCInventoryExperience::GlobalUpdate (0x0056FC5D): same body as src/, with the base update, the
// fade update, the component colours, the two own virtuals and the screen save/restore stubbed and recorded.
static int g_baseCalls = 0, g_updates = 0, g_slot87 = 0, g_slot90 = 0, g_saves = 0, g_restores = 0, g_order = 0;
static int g_saveAt = 0, g_slot90At = 0, g_slot87At = 0, g_restoreAt = 0;

struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour() {}
};
struct CComponent {
    CRGBColour colour;
    int sets;
    void SetColour(const CRGBColour& c) { colour = c; ++sets; }
    CRGBColour GetColour() const { return colour; }
};
struct CSliderAnimator {
    CComponent* pSlider;
    float StartValue;
    float EndValue;
    bool Play;
    void Update() { ++g_updates; }
};
struct SAnimatorNode {
    SAnimatorNode* _M_next;
    SAnimatorNode* _M_prev;
    CSliderAnimator _M_data;
};
struct CAnimatorIterator {
    SAnimatorNode* _M_node;
    CSliderAnimator* operator->() const { return &_M_node->_M_data; }
    CAnimatorIterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator==(const CAnimatorIterator& o) const { return _M_node == o._M_node; }
    bool operator!=(const CAnimatorIterator& o) const { return _M_node != o._M_node; }
};
struct ExperienceSpendEvent {                        // Ego_r.pdb, 8 bytes
    bool enable;                                     // +0x00
    bool initiate;                                   // +0x01
    long time;                                       // +0x04
};
struct CAnimatorList {
    SAnimatorNode* _M_node;
    CAnimatorIterator end() const { CAnimatorIterator it; it._M_node = _M_node; return it; }
};
static int g_listDtors = 0;
struct CPickupList {
    int tag;
    ~CPickupList() { ++g_listDtors; }
};
static int g_restoredTag = 0;
struct CScreenObject {
    CPickupList SaveState_slot159() { ++g_saves; g_saveAt = ++g_order; CPickupList l; l.tag = 77; return l; }
    void RestoreState_slot160(const CPickupList& s) { ++g_restores; g_restoreAt = ++g_order; g_restoredTag = s.tag; }
};
struct CScreenHolder { CScreenObject* Screen; };

namespace NInventory {
struct CTCInventoryBase {
    CScreenHolder* PScreen;
    bool InventoryOpen;
    void GlobalUpdate(bool) { ++g_baseCalls; }
    void Slot87(long v) { ++g_slot87; g_slot87At = ++g_order; (void)v; }
    void Slot90(long v) { ++g_slot90; g_slot90At = ++g_order; (void)v; }
};
}

struct CTCInventoryExperience : NInventory::CTCInventoryBase {
    CAnimatorList     Animators;
    CAnimatorIterator AnimIt;
    ExperienceSpendEvent ExperienceSpendAdd;
    ExperienceSpendEvent ExperienceSpendRemove;
    bool          DestroyEverything;

    void GlobalUpdate(bool paused);
};

void CTCInventoryExperience::GlobalUpdate(bool paused)
{
    NInventory::CTCInventoryBase::GlobalUpdate(paused);
    if (InventoryOpen && AnimIt != Animators.end()) {
        for (SAnimatorNode* n = Animators._M_node->_M_next; n != Animators._M_node; n = n->_M_next)
            n->_M_data.Update();
        if (!AnimIt->Play) {
            for (;;) {
                ++AnimIt;
                if (AnimIt._M_node == Animators._M_node)
                    break;
                if (AnimIt->StartValue == AnimIt->EndValue)
                    continue;
                AnimIt->Play = true;
                break;
            }
        }
        for (SAnimatorNode* n = Animators._M_node->_M_next; n != Animators._M_node; n = n->_M_next) {
            CRGBColour colour = n->_M_data.pSlider->GetColour();
            if (colour.A >= 0x46) {
                if (AnimIt._M_node != Animators._M_node)
                    colour.A = !n->_M_data.Play ? 0x78 : 0xff;
                else
                    colour.A = 0xff;
                n->_M_data.pSlider->SetColour(colour);
            }
        }
    }
    if (ExperienceSpendAdd.initiate) {
        long& time = ExperienceSpendAdd.time;
        if (--time < 0) {
            time = 0;
            ExperienceSpendAdd.enable = true;
        }
    }
    if (ExperienceSpendRemove.initiate) {
        long& time = ExperienceSpendRemove.time;
        if (--time < 0) {
            time = 0;
            ExperienceSpendRemove.enable = true;
        }
    }
    if (InventoryOpen) {
        bool& pending = DestroyEverything;
        if (pending) {
            pending = false;
            CPickupList state = PScreen->Screen->SaveState_slot159();
            Slot90(0);
            Slot87(0);
            PScreen->Screen->RestoreState_slot160(state);
        }
    }
}

int main()
{
    int fails = 0;
    CComponent comp[4];
    SAnimatorNode head, n[4];
    head._M_next = &n[0]; head._M_prev = &n[3];
    for (int i = 0; i < 4; ++i) {
        n[i]._M_next = i < 3 ? &n[i + 1] : &head;
        n[i]._M_prev = i > 0 ? &n[i - 1] : &head;
        n[i]._M_data.pSlider = &comp[i];
        n[i]._M_data.Play = false;
        comp[i].sets = 0;
        comp[i].colour.A = 0x80;
    }
    comp[2].colour.A = 0x20;                                   // below the visible threshold: left alone
    n[0]._M_data.Play = false;
    n[1]._M_data.StartValue = 1.0f; n[1]._M_data.EndValue = 1.0f;          // nothing to show: skipped
    n[2]._M_data.StartValue = 1.0f; n[2]._M_data.EndValue = 2.0f;          // first change: becomes current and shown
    CScreenObject screen; CScreenHolder holder; holder.Screen = &screen;
    CTCInventoryExperience tc;
    tc.PScreen = &holder; tc.InventoryOpen = true;
    tc.Animators._M_node = &head; tc.AnimIt._M_node = &n[0];
    tc.ExperienceSpendAdd.initiate = true; tc.ExperienceSpendAdd.time = 0; tc.ExperienceSpendAdd.enable = false;
    tc.ExperienceSpendRemove.initiate = true; tc.ExperienceSpendRemove.time = 5; tc.ExperienceSpendRemove.enable = false;
    tc.DestroyEverything = true;
    tc.GlobalUpdate(false);
    if (g_baseCalls != 1 || g_updates != 4) ++fails;
    if (tc.AnimIt._M_node != &n[2] || !n[2]._M_data.Play || n[1]._M_data.Play) ++fails;
    if (comp[0].colour.A != 0x78 || comp[1].colour.A != 0x78 || comp[3].colour.A != 0x78) ++fails;
    if (comp[2].sets != 0 || comp[2].colour.A != 0x20 || comp[0].sets != 1) ++fails;
    if (tc.ExperienceSpendAdd.time != 0 || !tc.ExperienceSpendAdd.enable || tc.ExperienceSpendRemove.time != 4 || tc.ExperienceSpendRemove.enable) ++fails;
    if (tc.DestroyEverything || g_saves != 1 || g_restores != 1 || g_restoredTag != 77) ++fails;
    if (!(g_saveAt < g_slot90At && g_slot90At < g_slot87At && g_slot87At < g_restoreAt)) ++fails;
    if (g_listDtors < 1) ++fails;
    // Second tick: the current entry is shown, so it stays; shown entries get full alpha.
    comp[2].colour.A = 0x50;
    tc.GlobalUpdate(false);
    if (tc.AnimIt._M_node != &n[2] || comp[2].colour.A != 0xff || comp[0].colour.A != 0x78) ++fails;
    if (g_saves != 1) ++fails;                                  // no refresh pending
    // Advance past the end: the remaining entry has nothing to show, so current reaches the head; all full alpha.
    n[2]._M_data.Play = false; n[3]._M_data.StartValue = 3.0f; n[3]._M_data.EndValue = 3.0f;
    tc.GlobalUpdate(false);
    if (tc.AnimIt._M_node != &head || comp[0].colour.A != 0xff || comp[3].colour.A != 0xff) ++fails;
    int before = g_updates;
    tc.GlobalUpdate(false);                                     // current == end: no fade work
    if (g_updates != before) ++fails;
    tc.InventoryOpen = false; tc.DestroyEverything = true;
    tc.GlobalUpdate(false);
    if (!tc.DestroyEverything || g_saves != 1 || g_baseCalls != 5) ++fails;
    if (fails) { std::printf("0056fc5d_TEST FAIL %d\n", fails); return 1; }
    std::printf("0056fc5d_TEST PASS\n");
    return 0;
}
