// CTCAISummon::OnInitialActivate() -- run the base hook, then fetch the summon sub-def from the parent thing's def into PSummonDef
// Leaf: OnInitialActivate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007FDE70, length 54 bytes (+ int3 padding).
// Callees: CTCBase's hook @ 0x00686870 (called non-virtually; manifest "$E2"), CDefClassBase::GetSubDef<
// CSummonDef>(CDefPointer<T>*) const @ 0x007FE5B0 (manifest "GetSubDef<CAICreatureWillPowerIndicatorDef>" names
// a byte-identical instantiation), and the thing def's virtual slot 1 (`jmp [edx+4]`, the release's
// destroy as a sibling call) when the local copy drops the last count.
//
// Layout: the thing keeps its counted def pointer at +0x70 (retail); CTCAISummon::PSummonDef
// (CDefPointer<CSummonDef>) is +0x0c (Ego_r.pdb).
// Shape: the def pointer is COPIED into a local (`mov esi,[eax+70h] ; test ; inc [esi+4]`, the copy
// constructor's add-ref) that is never address-taken, so it lives in esi through the GetSubDef call
// and its destructor (`dec [esi+4] ; jne`, no null store on a dead local) -- the virtual destroy is
// the last action and becomes a tail jump.
// Header note: engine/CDefClassBase.h and CSummonDef.h hold flattened layouts without the counted-def
// virtuals, so the def classes are modelled locally with a _Retail suffix.
template <class T> struct CDefPointer;
struct CSummonDef_Retail;

struct CDefClassBase_Retail
{
    virtual void v00();
    virtual void Destroy();                         // slot 1 (+0x04)
    long m_refCount;                                // +0x04
    bool GetSubDef(CDefPointer<CSummonDef_Retail>* out) const;   // GetSubDef<CSummonDef> @ 0x007FE5B0
};

template <class T>
struct CDefPointer
{
    T* Object;
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->m_refCount; }
    ~CDefPointer()
    {
        T* object = Object;
        if (object != 0)
        {
            if (--object->m_refCount == 0)
                object->Destroy();
        }
    }
    T* operator->() const { return Object; }
};

struct CThingDef : CDefClassBase_Retail {};
struct CSummonDef_Retail : CDefClassBase_Retail {};

struct CThing
{
    unsigned char          _pad_0x00[0x70];
    CDefPointer<CThingDef> PDef;                    // +0x70
};

#pragma pack(push, 1)
struct CTCBase_Calls                                   // engine/CTCBase.h layout (the header holds the layout only)
{
    void*   __vftable;                              // +0x00
    CThing* ParentThing;                            // +0x04
    bool    ToBeRemoved;                            // +0x08
    bool    IsInGlobalUpdateTCs;                    // +0x09
    unsigned char _pad_0x0a[0x2];                   // +0x0a
    void OnInitialActivate();                       // 0x00686870
};

struct CTCAISummon : CTCBase_Calls
{
    CDefPointer<CSummonDef_Retail> PSummonDef;             // +0x0c
    void OnInitialActivate();
};
#pragma pack(pop)

void CTCAISummon::OnInitialActivate()
{
    CTCBase_Calls::OnInitialActivate();
    CDefPointer<CThingDef> thingDef(ParentThing->PDef);
    thingDef->GetSubDef(&PSummonDef);
}
