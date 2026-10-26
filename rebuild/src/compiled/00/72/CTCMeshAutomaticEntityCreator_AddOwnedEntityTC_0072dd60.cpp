// CTCMeshAutomaticEntityCreator::AddOwnedEntityTC(CThing* created) -- give a freshly created thing a "CTCOwnedEntity" component (not activated, no params) and make the creator's own thing its owner
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072DD60, length 67 bytes (`ret 4`). Callees: CCharString::CCharString(const
// char*, long) @ 0x0099EBF0 with the literal "CTCOwnedEntity" @ 0x0123AA74, CThing::AddTC(const CCharString&, bool,
// CTCParamsBase*) @ 0x004C9D60, CCharString::~CCharString @ 0x0099EAE0, CTCOwnedEntity::SetPOwner(CThing*) @ 0x007E8590.
// The manifest label (COpinionDeedEffectsDef::~COpinionDeedEffectsDef) is propagated and wrong: the body takes a thing
// argument and its only callers are CTCMeshAutomaticEntityCreator::CreateObject (0x0072DF1A) and ::CreateBuilding
// (0x0072E0EE), which pass the entity they just created; `this`+4 is the creator component's owning thing.
// Shape: the name is an IMPLICIT conversion temporary (AddTC takes `const CCharString&`, the defaulted -1 length is
// pushed) built in the `push ecx` frame word and re-addressed with `lea` for the call; it dies at the end of the full
// expression, so the component pointer is a named local kept in edi across the string destructor.
// Leaf: AddOwnedEntityTC
class CCharString {
public:
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    void* m_pStringData;
};

class CTCParamsBase;
class CThing;

class CTCBase {
public:
    void SetPOwner(CThing* owner);                     // 0x007E8590 (CTCOwnedEntity::SetPOwner)
};

class CThing {
public:
public:
    CTCBase* AddTC(const CCharString&, bool, const CTCParamsBase*);   // 0x004C9D60
};

class CTCMeshAutomaticEntityCreator {
public:
    void*   m_vtable;                                  // +0x00
    CThing* m_pThing;                                  // +0x04 the creator's owning thing
    void AddOwnedEntityTC(CThing* created);
};

void CTCMeshAutomaticEntityCreator::AddOwnedEntityTC(CThing* created)
{
    CTCBase* owned = created->AddTC("CTCOwnedEntity", false, 0);
    owned->SetPOwner(m_pThing);
}