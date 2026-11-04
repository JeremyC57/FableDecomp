// Behaviour test for NCreatureAction::CActionEventExplosion::SetDataFromString (0x00871290): a model of the row -- field 0 names
// the explosion def, field 1 is PREFIX_TYPE (HELPER clears Dummy, DUMMY sets it, ACTION_HELPER sets UseActionHelper, other types
// fail), field 2 is read into the attachment object unless an action helper is used; the row always returns false.
#include <stdio.h>
#include <string>
#include <vector>
static bool GetField(const std::vector<std::string>& f, long i, std::string& out) { if (i >= (long)f.size()) return false; out = f[i]; return true; }
struct Explosion {
    std::string AttachmentObject; long ExplosionDefIndex; bool Dummy; bool UseActionHelper;
    Explosion() : ExplosionDefIndex(-1), Dummy(true), UseActionHelper(false) {}
    bool SetDataFromString(const std::vector<std::string>& data)
    {
        { std::string def; if (!GetField(data, 0, def)) return false; ExplosionDefIndex = (long)def.size(); }
        {
            std::string at; if (!GetField(data, 1, at)) return false;
            std::string::size_type u = at.find('_');
            long underscore = u == std::string::npos ? -1 : (long)u;
            std::string type = at.substr(underscore + 1);
            if (type == "HELPER") Dummy = false;
            else if (type == "DUMMY") Dummy = true;
            else if (type == "ACTION_HELPER") UseActionHelper = true;
            else return false;
        }
        if (UseActionHelper) return false;
        GetField(data, 2, AttachmentObject);
        return false;
    }
};
int main()
{
    std::vector<std::string> a; a.push_back("EXPL"); a.push_back("ATTACH_HELPER"); a.push_back("obj");
    Explosion e1; bool r1 = e1.SetDataFromString(a);
    std::vector<std::string> b; b.push_back("EXPL"); b.push_back("ATTACH_ACTION_HELPER"); b.push_back("obj");
    Explosion e2; bool r2 = e2.SetDataFromString(b);
    std::vector<std::string> c; c.push_back("E"); c.push_back("X_OTHER");
    Explosion e3; bool r3 = e3.SetDataFromString(c);
    int ok = !r1 && !e1.Dummy && e1.AttachmentObject == "obj" && e1.ExplosionDefIndex == 4
          && !r2 && e2.UseActionHelper && e2.AttachmentObject.empty()
          && !r3 && e3.Dummy && e3.ExplosionDefIndex == 1;
    printf(ok ? "00871290_TEST PASS\n" : "00871290_TEST FAIL\n");
    return ok ? 0 : 1;
}