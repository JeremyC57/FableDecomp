// Behaviour test for CPlayerGui::UpdateMoney (0x00439BB0), a portable model. The bar's change texts are updated first. Nothing else
// happens without a living hero. An uninitialised bar snaps its current, old and last-old values to the hero's money and becomes
// initialised. An initialised bar compares the money with its rounded current value: on a gain it queues "+N" in the increase colour,
// on a loss "-N" in the decrease colour and plays GOLD_PAYMENT when the hero has a sound player; the queued value is the signed change.
// Then it animates: steps = (long)((|old - current| - 0.2) * 0.5), at least 1, passed with the money, 0.01 and the def's two
// stat-bar times; the old graphic frame takes the graphic frame, which advances by one.
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string>
struct Bar { bool initialised; float current, old_value, last_old; unsigned long frame, old_frame; int change_text_updates;
             std::wstring text; unsigned long colour; long value; int queued; };
struct Hero { bool alive, has_sound; long money; int sounds; };
struct Stat { float value, min_change, steps, back, show; int calls; };
static long Near(float v) { return (long)floor(v + 0.5f); }   // round to nearest (no .5 ties used below)
static std::wstring Digits(long n)
{
    std::wstring out;
    do { out.insert(out.begin(), (wchar_t)(L'0' + n % 10)); n /= 10; } while (n);
    return out;
}
static void UpdateMoney(Bar& bar, Hero* hero, Stat& stat)
{
    const unsigned long increase = 0x00ff00, decrease = 0xff0000;
    const float back = 1.5f, show = 3.0f;
    ++bar.change_text_updates;
    if (!hero || !hero->alive) return;
    long current_money = hero->money;
    if (bar.initialised) {
        long stored = Near(bar.current);
        if (current_money != stored) {
            std::wstring prefix; unsigned long colour;
            if (current_money > stored) { colour = increase; prefix = L"+"; }
            else { colour = decrease; prefix = L"-"; if (hero->has_sound) ++hero->sounds; }
            bar.text = prefix + Digits(labs(current_money - stored)); bar.colour = colour; bar.value = current_money - stored; ++bar.queued;
        }
        long steps = (long)((fabsf(bar.old_value - bar.current) - 0.2f) * 0.5f);
        if (steps < 1) steps = 1;
        stat.value = (float)current_money; stat.min_change = 0.01f; stat.steps = (float)steps; stat.back = back; stat.show = show; ++stat.calls;
        bar.old_frame = bar.frame; bar.frame++;
    } else {
        bar.current = bar.old_value = bar.last_old = (float)current_money;
        bar.initialised = true;
    }
}
int main()
{
    int bad = 0;
    Bar bar = { false, 0, 0, 0, 7, 0, 0, L"", 0, 0, 0 };
    Stat stat = { 0, 0, 0, 0, 0, 0 };
    UpdateMoney(bar, 0, stat);                                   // no hero: only the change texts update
    if (bar.change_text_updates != 1 || bar.initialised) ++bad;
    Hero dead = { false, true, 50, 0 };
    UpdateMoney(bar, &dead, stat);
    if (bar.initialised || stat.calls) ++bad;
    Hero hero = { true, true, 120, 0 };
    UpdateMoney(bar, &hero, stat);                               // snap
    if (!bar.initialised || bar.current != 120.0f || bar.old_value != 120.0f || bar.last_old != 120.0f || bar.queued || stat.calls) ++bad;
    hero.money = 150;                                            // gain of 30
    UpdateMoney(bar, &hero, stat);
    if (bar.text != L"+30" || bar.colour != 0x00ff00 || bar.value != 30 || hero.sounds || stat.calls != 1 || stat.steps != 1.0f
        || stat.value != 150.0f || stat.min_change != 0.01f || stat.back != 1.5f || stat.show != 3.0f || bar.old_frame != 7 || bar.frame != 8) ++bad;
    bar.current = 149.6f; bar.old_value = 109.6f; hero.money = 100;   // rounds to 150; loss of 50; |old - current| = 40 -> 19 steps
    UpdateMoney(bar, &hero, stat);
    if (bar.text != L"-50" || bar.colour != 0xff0000 || bar.value != -50 || hero.sounds != 1 || stat.steps != 19.0f || bar.frame != 9) ++bad;
    hero.has_sound = false; hero.money = 99;                     // loss without a sound player
    UpdateMoney(bar, &hero, stat);
    if (bar.text != L"-51" || hero.sounds != 1 || bar.queued != 3) ++bad;
    bar.current = 99.2f; bar.old_value = 99.2f;                  // rounds to 99: no change text
    UpdateMoney(bar, &hero, stat);
    if (bar.queued != 3 || stat.calls != 4 || stat.steps != 1.0f) ++bad;
    printf(bad ? "00439bb0_TEST FAIL\n" : "00439bb0_TEST PASS\n");
    return bad ? 1 : 0;
}