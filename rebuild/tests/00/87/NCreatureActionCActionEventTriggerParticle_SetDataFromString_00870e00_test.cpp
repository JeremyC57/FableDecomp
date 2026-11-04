// Behaviour test for NCreatureAction::CActionEventTriggerParticle::SetDataFromString (0x00870E00): a model of the row -- field 0
// is the effect name, field 1 PREFIX_TYPE (prefix WEAPON sets Weapon; type HELPER / DUMMY add flag 4 / 2 to the default 8), the
// attachment object is read when flag 4 or 2 is set, and any next field sets DropRatherThanAttach.
#include <stdio.h>
#include <string>
#include <vector>
static bool GetField(const std::vector<std::string>& f, long i, std::string& out) { if (i >= (long)f.size()) return false; out = f[i]; return true; }
struct Particle {
    long AttachmentFlags; std::string AttachmentObject; std::string EffectName; bool Weapon; bool DropRatherThanAttach;
    Particle() : AttachmentFlags(0), Weapon(true), DropRatherThanAttach(false) {}
    bool SetDataFromString(const std::vector<std::string>& data)
    {
        long field = 0;
        if (!GetField(data, field++, EffectName)) return false;
        Weapon = false; AttachmentFlags = 8;
        {
            std::string at; if (!GetField(data, field++, at)) return false;
            std::string::size_type u = at.find('_');
            if (u != std::string::npos) {
                if (at.substr(0, u) == "WEAPON") Weapon = true;
                std::string type = at.substr(u + 1);
                if (type == "HELPER") AttachmentFlags |= 4; else if (type == "DUMMY") AttachmentFlags |= 2;
            }
            if (AttachmentFlags & 6) { if (!GetField(data, field++, AttachmentObject)) return false; }
        }
        { std::string drop; if (GetField(data, field, drop)) DropRatherThanAttach = true; }
        return true;
    }
};
int main()
{
    std::vector<std::string> a; a.push_back("fx"); a.push_back("WEAPON_HELPER"); a.push_back("obj"); a.push_back("drop");
    Particle p1; bool r1 = p1.SetDataFromString(a);
    std::vector<std::string> b; b.push_back("fx"); b.push_back("NONE");
    Particle p2; bool r2 = p2.SetDataFromString(b);
    std::vector<std::string> c; c.push_back("fx"); c.push_back("X_DUMMY");
    Particle p3; bool r3 = p3.SetDataFromString(c);
    int ok = r1 && p1.Weapon && p1.AttachmentFlags == 12 && p1.AttachmentObject == "obj" && p1.DropRatherThanAttach
          && r2 && !p2.Weapon && p2.AttachmentFlags == 8 && !p2.DropRatherThanAttach
          && !r3 && p3.AttachmentFlags == 10;
    printf(ok ? "00870e00_TEST PASS\n" : "00870e00_TEST FAIL\n");
    return ok ? 0 : 1;
}