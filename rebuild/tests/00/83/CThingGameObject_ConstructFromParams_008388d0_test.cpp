// Behaviour test for 0x008388D0 CThingGameObject::ConstructFromParams
#include <cstdio>
#include <cstring>
class C3DVector { public: float x, y, z; };
struct CThingPhysicalArg { int x; };
class CCharString { public:
    const char* m_text;
    bool operator!=(const char* text) const { return std::strcmp(m_text, text) != 0; }
    CCharString& operator=(const CCharString& other) { m_text = other.m_text; return *this; }
};
class CThingPhysical { public:
    int m_baseCalls; bool m_baseResult; long m_lastIndex;
    bool ConstructFromParams(long defIndex, const C3DVector*, long, unsigned long, CThingPhysicalArg*, CThingPhysicalArg*, bool)
    { ++m_baseCalls; m_lastIndex = defIndex; return m_baseResult; }
};
class CThingGameObject : public CThingPhysical { public:
    CCharString ScriptName;
    bool ConstructFromParams(long defIndex, const C3DVector* pos, long a3, unsigned long a4, const CCharString& scriptName,
                             CThingPhysicalArg* creator, CThingPhysicalArg* originalCreator, bool a8)
    {
        if (defIndex > 0 && CThingPhysical::ConstructFromParams(defIndex, pos, a3, a4, creator, originalCreator, a8)) {
            if (scriptName != "")
                ScriptName = scriptName;
            return true;
        }
        return false;
    }
};
int main()
{
    CThingGameObject o; o.m_baseCalls = 0; o.m_baseResult = true; o.m_lastIndex = 0; o.ScriptName.m_text = "OLD";
    C3DVector p = { 0.0f, 0.0f, 0.0f };
    CCharString empty; empty.m_text = "";
    CCharString name; name.m_text = "HERO_SPAWN";
    bool ok = !o.ConstructFromParams(0, &p, 0, 0, name, 0, 0, false) && o.m_baseCalls == 0;
    if (!o.ConstructFromParams(5, &p, 0, 0, empty, 0, 0, false) || std::strcmp(o.ScriptName.m_text, "OLD") != 0) ok = false;
    if (!o.ConstructFromParams(5, &p, 0, 0, name, 0, 0, false) || std::strcmp(o.ScriptName.m_text, "HERO_SPAWN") != 0) ok = false;
    o.m_baseResult = false; o.ScriptName.m_text = "OLD";
    if (o.ConstructFromParams(7, &p, 0, 0, name, 0, 0, false) || std::strcmp(o.ScriptName.m_text, "OLD") != 0 || o.m_lastIndex != 7) ok = false;
    if (!ok) { std::printf("008388d0_TEST FAIL\n"); return 1; }
    std::printf("008388d0_TEST PASS\n");
    return 0;
}