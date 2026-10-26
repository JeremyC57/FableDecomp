// CTCDParticleEmitter::OnSerialise(CPersistContext& context, bool transfer) -- persist an independent emitter: when saving (only for independent objects) its flag and its particle type by name; when loading the flag and the type name, translating the name back to the type id (a type that no longer exists kills the thing, unless front-end definitions are being compiled)
// Leaf: OnSerialise
//
// Retail 0x006E0730, 330 bytes (`ret 8`).
// Callees: CPersistContext::Transfer<bool> @ 0x004045C0 ("IndependantObject", default false), the particle emitter database
// getter @ 0x00AEAAE0 (static, returns the global at 0x013D2E14), CSymbolMap::FindFirstSymbolWithVal(long) @ 0x009E5090,
// CCharString() @ 0x0099E4B0, CCharString(const CCharString&) @ 0x0099EC30, operator= @ 0x0099EFB0, ~CCharString @ 0x0099EAE0,
// CPersistContext::Transfer(const char*, CCharString&) @ 0x00411010 (save), Transfer(const char*, CCharString&, const
// CCharString&) @ 0x004109A0 (load, "ParticleTypeName"), CSymbolMap::Lookup(const CCharString&, long*) @ 0x009E5170,
// CThing::Kill(bool) @ 0x004C9B80. Global g_bCompileFrontendDefinitions @ 0x013B8648.
// Layout = Ego_r.pdb CTCDParticleEmitter (0x48): EmitterName +0x0c, ParticleTypeID +0x10, the flag byte +0x44 (IndependantObject is
// its third bit); CPersistContext Mode +0x18 (saving: 1 or 3); CSymbolInfo EntryName +4; the database's symbol map is at +0.
// Shape: if/else on IsSaving (the saving arm returns early for a dependent emitter); the loading default string is built by the
// inline by-value getter returning a named local (no NRVO: built, copied into the argument and destroyed); the database is a
// named local fetched before the argument pushes.
class CCharStringData;
class CCharString { public:
    CCharStringData* PStringData;
    CCharString();                                          // 0x0099E4B0
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
    CCharString& operator=(const CCharString& other);       // 0x0099EFB0
};
static inline CCharString EmptyCCharString()
{
    CCharString empty;
    return empty;
}

class CPersistContext { public:
    unsigned char _pad00[0x18];
    long          Mode;                                     // +0x18: 0 text load, 1 text save, 2 binary load, 3 binary save
    bool IsSaving() const { return Mode == 1 || Mode == 3; }
    void Transfer(const char* name, bool& value, const bool& defaultValue);   // 0x004045C0
    void Transfer(const char* name, CCharString& value);                      // 0x00411010
    void Transfer(const char* name, CCharString& value, const CCharString& defaultValue);   // 0x004109A0
};

struct CSymbolInfo {
    CCharString GroupName;                                  // +0x00
    CCharString EntryName;                                  // +0x04
    long        Val;                                        // +0x08
};
class CSymbolMap { public:
    CSymbolInfo* FindFirstSymbolWithVal(long value);        // 0x009E5090
    bool Lookup(const CCharString& name, long* value);      // 0x009E5170
};
struct CParticleEmitterDatabase {
    CSymbolMap SymbolMap;                                   // +0x00
    static CParticleEmitterDatabase* __fastcall Get();      // 0x00AEAAE0
};
extern bool g_bCompileFrontendDefinitions;                  // 0x013B8648

class CThing {
public:
    void Kill(bool immediately);                            // 0x004C9B80
};
class CTCBase {
public:
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
};
class CTCDParticleEmitter : public CTCBase {
public:
    CCharString   EmitterName;                              // +0x0c
    long          ParticleTypeID;                           // +0x10
    unsigned char _pad14[0x44 - 0x14];
    bool WaitingForParticleToLoad : 1;                      // +0x44
    bool ForceParticleToLoad : 1;
    bool IndependantObject : 1;
    bool ParticleCreationDisabled : 1;
    bool ResetEmitterPosition : 1;
    bool ResetEmitterOrientation : 1;
    bool AttachedToCamera : 1;
    bool Moving : 1;
    void OnSerialise(CPersistContext& context, bool transfer);
};

void CTCDParticleEmitter::OnSerialise(CPersistContext& context, bool transfer)
{
    if (context.IsSaving()) {
        if (!IndependantObject)
            return;
        bool independant = IndependantObject;
        context.Transfer("IndependantObject", independant, false);
        CParticleEmitterDatabase* database = CParticleEmitterDatabase::Get();
        CSymbolInfo* info = database->SymbolMap.FindFirstSymbolWithVal(ParticleTypeID);
        CCharString typeName;
        if (info)
            typeName = info->EntryName;
        context.Transfer("ParticleTypeName", typeName);
    } else {
        bool independant = false;
        context.Transfer("IndependantObject", independant, false);
        IndependantObject = independant;
        CCharString typeName;
        context.Transfer("ParticleTypeName", typeName, EmptyCCharString());
        if (!g_bCompileFrontendDefinitions) {
            CParticleEmitterDatabase* database = CParticleEmitterDatabase::Get();
            database->SymbolMap.Lookup(typeName, &ParticleTypeID);
            if (ParticleTypeID == 0)
                ParentThing->Kill(true);
        }
    }
}