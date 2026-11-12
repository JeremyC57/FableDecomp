// Behaviour test for 0x005F6793 CTCInventoryAbilities::CycleAssignedExpressions: the paging logic on a copy of the model.
#include <cstdio>
#include <vector>

static const long& MaxRef(const long& a, const long& b) { return a < b ? b : a; }
static const long& MinRef(const long& a, const long& b) { return b < a ? b : a; }

struct Model {
    long CurrentAssignedExpressionLevel;
    long NumButtons;
    std::vector<long> Order;       // prioritised expression defs
    long Buttons[8];               // expression per button
    int  SoundsPlayed;
    int  SetCalls;
    bool HasSoundManager;

    long GetNumAssignableSpecialMoveButtons() const { return NumButtons; }
    void GetPrioritisedExpressionDefs(std::vector<long>& defs) const
    {
        for (unsigned long i = 0; i < Order.size(); ++i)
            defs.push_back(Order[i]);
    }
    void SetExpressionToButton(long expression, long button) { Buttons[button] = expression; ++SetCalls; }

    void CycleAssignedExpressions(bool forward)
    {
        std::vector<long> expressions;
        GetPrioritisedExpressionDefs(expressions);
        if (expressions.empty()) return;
        for (long button_index = 0; button_index < GetNumAssignableSpecialMoveButtons(); ++button_index)
            SetExpressionToButton(0, button_index);
        std::vector<long>::const_iterator it = expressions.begin();
        long neg = -GetNumAssignableSpecialMoveButtons();
        long original_level = MaxRef(CurrentAssignedExpressionLevel, neg);
        bool valid_set = false;
        do {
            if (forward) CurrentAssignedExpressionLevel += GetNumAssignableSpecialMoveButtons();
            else CurrentAssignedExpressionLevel -= GetNumAssignableSpecialMoveButtons();
            if (CurrentAssignedExpressionLevel >= (long)expressions.size())
                CurrentAssignedExpressionLevel = MinRef(original_level, 0L);
            else if (CurrentAssignedExpressionLevel < 0)
                CurrentAssignedExpressionLevel = (long)expressions.size() - GetNumAssignableSpecialMoveButtons();
            it = expressions.begin();
            if (CurrentAssignedExpressionLevel > 0) it += CurrentAssignedExpressionLevel;
            long c1 = 0;
            if (c1 < GetNumAssignableSpecialMoveButtons()) {
                std::vector<long>::const_iterator expression_it = it;
                do {
                    if (expression_it != expressions.end()) {
                        if (*expression_it > 0) { valid_set = true; break; }
                    }
                    ++c1; ++expression_it;
                } while (c1 < GetNumAssignableSpecialMoveButtons());
            }
        } while (!valid_set && CurrentAssignedExpressionLevel != original_level);
        if (valid_set) {
            for (long button_index = 0; it != expressions.end(); ++it) {
                if (*it > 0) SetExpressionToButton(*it, button_index);
                else SetExpressionToButton(0, button_index);
                if (++button_index == GetNumAssignableSpecialMoveButtons()) break;
            }
            if (HasSoundManager) ++SoundsPlayed;
        }
    }
};

static void Reset(Model& m, long level, long buttons)
{
    m.CurrentAssignedExpressionLevel = level;
    m.NumButtons = buttons;
    m.Order.clear();
    for (int i = 0; i < 8; ++i) m.Buttons[i] = -99;
    m.SoundsPlayed = 0;
    m.SetCalls = 0;
    m.HasSoundManager = true;
}

int main()
{
    bool ok = true;
    Model m;

    // 1. empty list: nothing touched, not even the reset loop.
    Reset(m, 0, 4);
    m.CycleAssignedExpressions(true);
    ok = ok && m.SetCalls == 0 && m.Buttons[0] == -99 && m.SoundsPlayed == 0 && m.CurrentAssignedExpressionLevel == 0;

    // 2. forward one row: 8 expressions, 4 buttons, level 0 -> 4.
    Reset(m, 0, 4);
    for (long i = 1; i <= 8; ++i) m.Order.push_back(i * 10);
    m.CycleAssignedExpressions(true);
    ok = ok && m.CurrentAssignedExpressionLevel == 4 && m.Buttons[0] == 50 && m.Buttons[1] == 60 && m.Buttons[2] == 70
            && m.Buttons[3] == 80 && m.SoundsPlayed == 1 && m.SetCalls == 8;

    // 3. forward again wraps to MinRef(original_level, 0) = 0.
    m.SoundsPlayed = 0;
    m.CycleAssignedExpressions(true);
    ok = ok && m.CurrentAssignedExpressionLevel == 0 && m.Buttons[0] == 10 && m.Buttons[3] == 40 && m.SoundsPlayed == 1;

    // 4. backward from 0 goes to size - buttons = 4.
    m.CycleAssignedExpressions(false);
    ok = ok && m.CurrentAssignedExpressionLevel == 4 && m.Buttons[0] == 50 && m.Buttons[3] == 80;

    // 5. a row of non-positive entries is skipped: rows {1,2},{0,-1},{5,0}; forward from 0 lands on row 2 (level 4);
    //    the non-positive entry of that row clears its button.
    Reset(m, 0, 2);
    { long v[6] = { 1, 2, 0, -1, 5, 0 }; for (int i = 0; i < 6; ++i) m.Order.push_back(v[i]); }
    m.CycleAssignedExpressions(true);
    ok = ok && m.CurrentAssignedExpressionLevel == 4 && m.Buttons[0] == 5 && m.Buttons[1] == 0 && m.SoundsPlayed == 1;

    // 6. no valid row anywhere: buttons reset to 0, level comes back to the original, no sound.
    Reset(m, 0, 2);
    { long v[4] = { 0, 0, -3, 0 }; for (int i = 0; i < 4; ++i) m.Order.push_back(v[i]); }
    m.CycleAssignedExpressions(true);
    ok = ok && m.CurrentAssignedExpressionLevel == 0 && m.Buttons[0] == 0 && m.Buttons[1] == 0 && m.SoundsPlayed == 0
            && m.SetCalls == 2;

    // 7. short last row: 3 expressions, 2 buttons, forward from 0 -> level 2, only button 0 assigned (iterator hits end()).
    Reset(m, 0, 2);
    { long v[3] = { 7, 8, 9 }; for (int i = 0; i < 3; ++i) m.Order.push_back(v[i]); }
    m.CycleAssignedExpressions(true);
    ok = ok && m.CurrentAssignedExpressionLevel == 2 && m.Buttons[0] == 9 && m.Buttons[1] == 0 && m.SetCalls == 3;

    // 8. no sound manager: buttons still assigned, no sample played.
    Reset(m, 0, 2);
    { long v[4] = { 1, 2, 3, 4 }; for (int i = 0; i < 4; ++i) m.Order.push_back(v[i]); }
    m.HasSoundManager = false;
    m.CycleAssignedExpressions(true);
    ok = ok && m.CurrentAssignedExpressionLevel == 2 && m.Buttons[0] == 3 && m.Buttons[1] == 4 && m.SoundsPlayed == 0;

    if (!ok) { std::printf("005f6793_TEST FAIL\n"); return 1; }
    std::printf("005f6793_TEST PASS\n");
    return 0;
}