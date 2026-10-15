// CTCActionUseReadable::OnSerialise(CPersistContext& context, bool loading) -- transfer the TextTag wide string under the name "GameTextDefName" with an empty default
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E8160, length 77 bytes.
// Leaf: OnSerialise
// Callees: CWideString::CWideString() @ 0x0099AED0, CWideString::CWideString(const CWideString&) @ 0x0099B720,
// CWideString::~CWideString @ 0x0099B510, CPersistContext::Transfer(const char*, T&, const T&) @ 0x00410AC0 (the ICF-shared
// template body; the manifest names its CIntelligentPointer<CThing> instantiation). Literal 0x01270BDC = "GameTextDefName".
// Shape: two frame strings -- A default-constructed, B copy-constructed from A, A destroyed BEFORE the call, B passed by
// const reference and destroyed after: the default value comes from an inline helper that returns a NAMED local by value
// (VC7.1 has no NRVO, so the local is copied into the return temporary and destroyed at the helper's end). The base
// OnSerialise is not called; `loading` is unused.
struct CWideString {
    void* m_pData;
    CWideString();                             // 0x0099AED0
    CWideString(const CWideString& other);     // 0x0099B720
    ~CWideString();                            // 0x0099B510
};

struct CPersistContext {
    void Transfer(const char* name, CWideString& value, const CWideString& defaultValue);   // 0x00410AC0
};

inline CWideString EmptyWideString()
{
    CWideString empty;
    return empty;
}

struct CThing;

struct CTCBase {                               // engine/CTCBase.h layout, modelled polymorphic
    virtual void v00();
    CThing* ParentThing;                       // +0x04
    bool    ToBeRemoved;                       // +0x08
    bool    IsInGlobalUpdateTCs;               // +0x09
};

struct CTCActionUseBase : CTCBase {
    unsigned char _pad0c[8];                   // +0x0c PActionUseDef, +0x10 Usable
};

struct CTCActionUseReadable : CTCActionUseBase {
    void*       PDef;                          // +0x14 CDefPointer<CReadableDef const>
    CWideString TextTag;                       // +0x18
    virtual void OnSerialise(CPersistContext& context, bool loading);
};

void CTCActionUseReadable::OnSerialise(CPersistContext& context, bool)
{
    context.Transfer("GameTextDefName", TextTag, EmptyWideString());
}