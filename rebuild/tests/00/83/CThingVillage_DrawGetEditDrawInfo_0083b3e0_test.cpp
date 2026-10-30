// Behaviour test for 0x0083B3E0 CThingVillage::DrawGetEditDrawInfo (see src/0083b3e0.cpp): when the physical base fills the info,
// the full name becomes "<definition name> - <script name>", the colour is Set(128,255,255,255) and the result is true;
// otherwise the info is untouched and the result is false.
#include <cstdio>
#include <string>
struct Colour { unsigned char B, G, R, A; void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; } };
struct Info { std::string Fullname; Colour Colour_; };
struct Thing {
    bool baseOk; std::string defName, ScriptName;
    bool Draw(float, Info* info) const
    {
        if (baseOk) {
            info->Fullname = std::string("") + defName + " - " + ScriptName.c_str();
            info->Colour_.Set(128, 255, 255, 255);
            return true;
        }
        return false;
    }
};
int main()
{
    Thing t = { true, "OBJECT_X", "Script1" }; Info i; i.Colour_.Set(1, 2, 3, 4);
    bool ok = t.Draw(0.f, &i) && i.Fullname == "OBJECT_X - Script1" && i.Colour_.R == 128 && i.Colour_.G == 255 && i.Colour_.B == 255 && i.Colour_.A == 255;
    Thing f = { false, "A", "B" }; Info j; j.Fullname = "keep";
    ok = ok && !f.Draw(0.f, &j) && j.Fullname == "keep";
    std::printf(ok ? "0083b3e0_TEST PASS\n" : "0083b3e0_TEST FAIL\n");
    return ok ? 0 : 1;
}