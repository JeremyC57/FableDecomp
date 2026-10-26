// Behaviour test for CTCHeroCentreDoorMarker::OnSerialise (retail 0x0077F650): model copy + stub callees.
#include <cstdio>
#include <cstring>

class CCharString { public:
    char text[32];
    CCharString() { text[0] = 0; }
    CCharString(const CCharString& o) { std::strcpy(text, o.text); }
    ~CCharString() {}
    bool operator!=(const char* t) const { return std::strcmp(text, t) != 0; }
};
static inline CCharString EmptyCCharString() { CCharString empty; return empty; }
enum EHeroCentreDoorType { HERO_CENTRE_DOOR_NONE = 0 };
static int g_enumTransfers = 0;
class CPersistContext { public:
    long Mode;
    float savedRadius; long savedDoor; const char* legacyName;
    bool IsSaving() const { return Mode == 1 || Mode == 3; }
    void Transfer(const char*, float& value, const float& def) { if (IsSaving()) savedRadius = value; else value = savedRadius ? savedRadius : def; }
    void Transfer(const char*, EHeroCentreDoorType& value, const EHeroCentreDoorType& def) { ++g_enumTransfers; if (IsSaving()) savedDoor = value; else value = savedDoor >= 0 ? (EHeroCentreDoorType)savedDoor : def; }
    void Transfer(const char*, CCharString& value, const CCharString& def) { if (legacyName) std::strcpy(value.text, legacyName); else value = def; }
};
class CSymbolMap { public:
    bool Lookup(const CCharString& name, long* value) const { *value = std::strcmp(name.text, "DOOR_TRAINING") == 0 ? 3 : 1; return true; }
};
static CSymbolMap g_map;
CSymbolMap* GetDoorTypeSymbolMap() { return &g_map; }
class CTCHeroCentreDoorMarker {
public:
    float Radius;
    EHeroCentreDoorType DoorType;
    void OnSerialise(CPersistContext& context, bool transfer);
};
void CTCHeroCentreDoorMarker::OnSerialise(CPersistContext& context, bool transfer)
{
    context.Transfer("Radius", Radius, 0.0f);
    if (context.IsSaving()) {
        context.Transfer("DoorType2", DoorType, HERO_CENTRE_DOOR_NONE);
    } else {
        context.Transfer("DoorType2", DoorType, HERO_CENTRE_DOOR_NONE);
        CCharString doorTypeName;
        context.Transfer("DoorType", doorTypeName, EmptyCCharString());
        if (doorTypeName != "") {
            long value;
            CSymbolMap* doorTypes = GetDoorTypeSymbolMap();
            doorTypes->Lookup(doorTypeName, &value);
            DoorType = (EHeroCentreDoorType)value;
        }
    }
}
int main()
{
    int fail = 0;
    CTCHeroCentreDoorMarker m; m.Radius = 2.5f; m.DoorType = (EHeroCentreDoorType)2;
    CPersistContext save = { 1, 0.0f, -1, 0 };
    m.OnSerialise(save, true);
    if (save.savedRadius != 2.5f || save.savedDoor != 2 || g_enumTransfers != 1) fail = 1;
    CTCHeroCentreDoorMarker n; n.Radius = 0; n.DoorType = HERO_CENTRE_DOOR_NONE;
    CPersistContext load = { 0, 2.5f, 2, 0 };
    n.OnSerialise(load, true);
    if (n.Radius != 2.5f || n.DoorType != 2 || g_enumTransfers != 2) fail = 2;
    CPersistContext legacy = { 2, 1.0f, -1, "DOOR_TRAINING" };
    n.OnSerialise(legacy, true);
    if (n.DoorType != 3) fail = 3;
    if (fail) { std::printf("0077f650_TEST FAIL\n"); return 1; }
    std::printf("0077f650_TEST PASS\n");
    return 0;
}