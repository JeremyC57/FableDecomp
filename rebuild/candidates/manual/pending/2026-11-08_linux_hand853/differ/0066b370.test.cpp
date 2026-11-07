// Behaviour model of CTCSoundPlayer::PlayAllSoundEvents (0x0066B370): per anim set pick the anim with the largest blend alpha; when the
// set advanced, each event between the times either goes to the attached emitter (bit 31) or is played as the first criteria name that
// resolves: criteria;voice;extra (or criteria;voice) when a voice is set, then criteria;material;extra, criteria;material, criteria, event.
#include <cstdio>
#include <string>
#include <vector>
#include <set>
struct Bank { std::set<std::string> Known; std::vector<std::string> Tried; bool Play(const std::string& n) { Tried.push_back(n); return Known.count(n) != 0; } };
static std::string PlayEvent(Bank& bank, const std::string& sound_id, const std::string& event, const std::string& voice, const std::string& extra, const std::string& material)
{
    std::string criteria = sound_id + ";" + event;
    bool played = false;
    if (voice != "") {
        std::string name = extra != "" ? criteria + ";" + voice + ";" + extra : criteria + ";" + voice;
        if (bank.Play(name)) played = true;
    }
    if (!played) {
        if (bank.Play(criteria + ";" + material + ";" + extra)) return bank.Tried.back();
        if (bank.Play(criteria + ";" + material)) return bank.Tried.back();
        if (bank.Play(criteria)) return bank.Tried.back();
        bank.Play(event);
    }
    return bank.Tried.back();
}
static long BestAnim(const std::vector<short>& anims, const std::vector<float>& alphas)
{
    float best_alpha = 0.0f; long best_anim = 0;
    long count = (long)anims.size();
    for (long i = 0; i < count; ++i)
        if (alphas[i] > best_alpha) { best_anim = anims[i]; best_alpha = alphas[i]; }
    return best_anim;
}
int main()
{
    std::vector<short> anims; std::vector<float> alphas;
    anims.push_back(4); anims.push_back(9); anims.push_back(2); anims.push_back(7); anims.push_back(5);
    alphas.push_back(0.1f); alphas.push_back(0.6f); alphas.push_back(0.6f); alphas.push_back(0.2f); alphas.push_back(0.0f);
    if (BestAnim(anims, alphas) != 9) { std::printf("FAIL best\n"); return 1; }
    std::vector<float> zero(5, 0.0f);
    if (BestAnim(anims, zero) != 0) { std::printf("FAIL zero\n"); return 1; }
    Bank b1; b1.Known.insert("HERO;STEP;STONE");
    if (PlayEvent(b1, "HERO", "STEP", "", "", "STONE") != "HERO;STEP;STONE" || b1.Tried.size() != 2 || b1.Tried[0] != "HERO;STEP;STONE;") { std::printf("FAIL material\n"); return 1; }
    Bank b2; b2.Known.insert("HERO;STEP;BOB;WET");
    if (PlayEvent(b2, "HERO", "STEP", "BOB", "WET", "STONE") != "HERO;STEP;BOB;WET" || b2.Tried.size() != 1) { std::printf("FAIL voice\n"); return 1; }
    Bank b3;
    PlayEvent(b3, "HERO", "STEP", "BOB", "", "MUD");
    if (b3.Tried.size() != 5 || b3.Tried[0] != "HERO;STEP;BOB" || b3.Tried[3] != "HERO;STEP" || b3.Tried[4] != "STEP") { std::printf("FAIL chain\n"); return 1; }
    std::printf("0066b370_TEST PASS\n");
    return 0;
}
