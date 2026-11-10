// Behaviour test for 0x0066BC10: CTCSoundPlayer::GetPotentialCriteriaStrings (host model: std::string for CCharString, the
// owner's def and its entity sound sub-def as plain stubs)
#include <cstdio>
#include <string>
#include <vector>

typedef std::string CCharString;
class CEntitySoundDef { public: CCharString SoundIdentifier; };
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    T* operator->() const { return Object; }
};
class CThingBaseDef { public:
    CEntitySoundDef* SoundDef;                  // the sub-def GetSubDef<CEntitySoundDef> finds (0 = none)
    bool GetSubDef(CDefPointer<const CEntitySoundDef>* def) const {
        if (SoundDef == 0) return false;
        def->Object = SoundDef;
        return true;
    }
};
class CThing { public:
    CThingBaseDef* PBaseDef;
    bool HasBaseDef() const { return PBaseDef != 0; }
    const CThingBaseDef& PeekBaseDef() const { return *PBaseDef; }
};
class CTCSoundPlayer {
public:
    CThing* ParentThing;
    void GetPotentialCriteriaStrings(const CCharString& criteria, std::vector<CCharString>& criteria_strings);
};

// the row's body
void CTCSoundPlayer::GetPotentialCriteriaStrings(const CCharString& criteria, std::vector<CCharString>& criteria_strings)
{
    CDefPointer<const CEntitySoundDef> p_entity_sound_def;
    if (ParentThing->HasBaseDef() && ParentThing->PeekBaseDef().GetSubDef(&p_entity_sound_def)) {
        CCharString criteria_string = p_entity_sound_def->SoundIdentifier + ";CS_" + criteria;
        criteria_strings.push_back(criteria_string);
    }
    criteria_strings.push_back("CS_" + criteria);
}

static int g_fail = 0;
static void Check(bool c) { if (!c) g_fail = 1; }

int main()
{
    CEntitySoundDef sound; sound.SoundIdentifier = "HOBBE";
    CThingBaseDef def; def.SoundDef = &sound;
    CThing thing; thing.PBaseDef = &def;
    CTCSoundPlayer player; player.ParentThing = &thing;

    std::vector<CCharString> names;
    player.GetPotentialCriteriaStrings("HIT", names);
    Check(names.size() == 2 && names[0] == "HOBBE;CS_HIT" && names[1] == "CS_HIT");

    names.push_back("x");                       // appends after what is there
    player.GetPotentialCriteriaStrings("DIE", names);
    Check(names.size() == 5 && names[3] == "HOBBE;CS_DIE" && names[4] == "CS_DIE");

    def.SoundDef = 0;                           // no entity sound sub-def
    std::vector<CCharString> plain;
    player.GetPotentialCriteriaStrings("HIT", plain);
    Check(plain.size() == 1 && plain[0] == "CS_HIT");

    thing.PBaseDef = 0;                         // no def at all
    std::vector<CCharString> none;
    player.GetPotentialCriteriaStrings("IDLE", none);
    Check(none.size() == 1 && none[0] == "CS_IDLE");

    if (g_fail) { std::printf("0066bc10_TEST FAIL\n"); return 1; }
    std::printf("0066bc10_TEST PASS\n");
    return 0;
}