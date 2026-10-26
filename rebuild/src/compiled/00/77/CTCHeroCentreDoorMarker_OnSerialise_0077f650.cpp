// CTCHeroCentreDoorMarker::OnSerialise(CPersistContext& context, bool transfer) -- persist the radius and the door type; when loading, an old save's door type name ("DoorType") is translated through the door-type symbol map
// Leaf: OnSerialise
//
// Retail 0x0077F650, 256 bytes incl. padding (`ret 8`). Callees: CPersistContext::Transfer<float> @ 0x00410620 ("Radius",
// default 0), CPersistContext::Transfer<EHeroCentreDoorType> @ 0x0077FDE0 ("DoorType2", default 0; twice), CCharString::CCharString()
// @ 0x0099E4B0 (twice), CCharString(const CCharString&) @ 0x0099EC30, ~CCharString @ 0x0099EAE0, CPersistContext::Transfer<CCharString>
// @ 0x004109A0 ("DoorType"; label names the intelligent-pointer instantiation, a byte twin), CCharString::operator!=(const char*)
// @ 0x0099E960 ("" @ 0x0122D70E), GetDoorTypeSymbolMap @ 0x0077F510 (static __fastcall), CSymbolMap::Lookup(const CCharString&,
// long*) @ 0x009E5170.
// Layout = Ego_r.pdb CTCHeroCentreDoorMarker: Radius +0x0c, DoorType +0x10; CPersistContext Mode +0x18 (IsSaving: 1 or 3).
// Shape: an if/else on IsSaving (VC7.1 duplicates the epilogue into the saving arm, whose default temporary takes the dead
// `transfer` slot; an early return keeps it in the frame); the loading arm transfers the enum too, then the legacy name with a
// default built by an inline by-value getter (VC7.1 builds the named local, copies it into the argument and destroys it); the
// symbol map is a named local fetched before the lookup pushes; the looked-up value is stored unconditionally after the lookup; `transfer` is unused (its slot holds the name).
class CCharStringData;
class CCharString { public:
    CCharStringData* PStringData;
    CCharString();                                          // 0x0099E4B0
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
    bool operator!=(const char* text) const;                // 0x0099E960
};
static inline CCharString EmptyCCharString()
{
    CCharString empty;
    return empty;
}

enum EHeroCentreDoorType { HERO_CENTRE_DOOR_NONE = 0 };

class CPersistContext { public:
    unsigned char _pad00[0x18];
    long          Mode;                                     // +0x18: 0 text load, 1 text save, 2 binary load, 3 binary save
    bool IsSaving() const { return Mode == 1 || Mode == 3; }
    void Transfer(const char* name, float& value, const float& defaultValue);   // 0x00410620
    void Transfer(const char* name, EHeroCentreDoorType& value, const EHeroCentreDoorType& defaultValue);   // 0x0077FDE0
    void Transfer(const char* name, CCharString& value, const CCharString& defaultValue);   // 0x004109A0
};

class CSymbolMap { public:
    bool Lookup(const CCharString& name, long* value) const;   // 0x009E5170
};
CSymbolMap* __fastcall GetDoorTypeSymbolMap();              // 0x0077F510

class CThing;
class CTCBase {
public:
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
};

class CTCHeroCentreDoorMarker : public CTCBase {
public:
    float               Radius;                             // +0x0c
    EHeroCentreDoorType DoorType;                           // +0x10
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