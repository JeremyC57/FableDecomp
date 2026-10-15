// CThingSearchClient::CThingSearchClient(CThingSearchOwner* owner, const SSearchRecord& record) -- constructor: own vtable, the owner, a copy of the 20-byte search record, then registration with the owner's host registry
// Leaf: ??0CThingSearchClient
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1E5A0, length 71 bytes.
// Callees: virtual slot 9 (+0x24) of the registry object at owner->m_pHost + 0x50 (the register call; the
// matching destructor 0x00C1E260, landed as "CAIStateGroupBase::PeekThingSearchTools", stores the same
// vtable and calls slot 8 of the same object to unregister).
// Vtable: 0x012B33F0 (no complete-object locator: a no-RTTI module).
// Shape: both arguments loaded first; the vtable store, the owner store, then the record copied as a whole
// plain struct through a re-based pointer (`lea ecx,[esi+8]` + five dword moves through edi); the owner
// ARGUMENT (still in eax) feeds the registration, `this` pushed.
// The manifest label "CScriptModuleBase::SetCoordAxis" is propagated from an unrelated body; the class and
// member names are descriptive.
struct SSearchRecord {                         // 20-byte plain record
    long m_a, m_b, m_c, m_d, m_e;
};

struct CThingSearchClient;

struct CThingSearchRegistry {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void Unregister(CThingSearchClient* client);   // slot 8 (+0x20)
    virtual void Register(CThingSearchClient* client);     // slot 9 (+0x24)
};

struct CThingSearchHost {
    unsigned char        _pad[0x50];
    CThingSearchRegistry m_registry;           // +0x50
};

struct CThingSearchOwner {
    void*             _pad;
    CThingSearchHost* m_pHost;                 // +0x04
};

struct CThingSearchClient {                    // vtable 0x012B33F0
    virtual void v00();
    CThingSearchOwner* m_pOwner;               // +0x04
    SSearchRecord      m_record;               // +0x08
    CThingSearchClient(CThingSearchOwner* owner, const SSearchRecord& record);
};

CThingSearchClient::CThingSearchClient(CThingSearchOwner* owner, const SSearchRecord& record)
    : m_pOwner(owner), m_record(record)
{
    owner->m_pHost->m_registry.Register(this);
}