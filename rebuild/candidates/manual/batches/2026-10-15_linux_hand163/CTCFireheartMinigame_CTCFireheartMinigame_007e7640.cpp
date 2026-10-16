// CTCFireheartMinigame::CTCFireheartMinigame(CThing*) -- CTCBase(thing), no session / forced quit / initialisation / move, an empty scripted-thing handle and primitive-handle vector, then the minigame def taken from the parent's definition
// Leaf: ??0CTCFireheartMinigame
//
// Retail 0x007E7640, 109 bytes (`ret 4`). Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, the
// CBaseObject root ctor @ 0x0099A380 (vtable 0x0129A7D8, +4 nulled; manifest
// "CBaseIntelligentPointer"), CDefClassBase::GetSubDef<CFireheartMinigameDef> @ 0x007E7D80 (manifest
// template name is a byte twin). Vtables 0x012709CC = CTCFireheartMinigame, 0x0127094C =
// CScriptGameResourceObjectScriptedThingBase.
// Layout = Ego_r.pdb CTCFireheartMinigame (no shift before the vector): InSession +0x4c, ForceQuit
// +0x4d, Initialised +0x54, Moving +0x70, ScriptData +0x78 (0x10: CBaseObject + PImp {Data, Info}),
// QuitWithDelay +0x88, Handles +0x8c (12-byte retail vector), PDef +0x98; retail CThing def pointer
// object +0x70.
// Shape: plain initialisers with one zero register (ebx); ScriptData addressed once in edi (root ctor,
// vtable store, the PImp words from an embedded zeroing member, the landed 0x00D80C30 model); the body
// passes the PDef address formed for its zero store.
struct CThing;
struct CFireheartMinigameDef;

template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    T* operator->() const { return Object; }
};

struct CDefClassBase {
    bool GetSubDef(CDefPointer<const CFireheartMinigameDef>* out);   // 0x007E7D80
};

struct CThing {
    unsigned char              _pad00[0x70];
    CDefPointer<CDefClassBase> PDef;                        // +0x70
};

struct CCountedPointerWords {                               // {Data, Info}
    void* Data;
    void* Info;
    CCountedPointerWords() : Data(0), Info(0) { }
};
struct CBaseObject {                                        // root: vtable + intelligent-pointer list
    virtual void v00();
    void* m_pIntelligentPointer;                            // +0x04
    CBaseObject();                                          // 0x0099A380
};
struct CScriptGameResourceObjectScriptedThingBase : CBaseObject {   // vtable 0x0127094C, 0x10 bytes
    CCountedPointerWords PImp;                              // +0x08
    CScriptGameResourceObjectScriptedThingBase() : CBaseObject(), PImp() { }
    virtual void v00();
};

template <class T> struct SRetailAllocProxy {
    T* _M_data;
    SRetailAllocProxy(T* data) : _M_data(data) {}
};
template <class T> struct CRetailVector {                   // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    SRetailAllocProxy<T> _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
};
struct CEnginePrimitiveHandle;

struct CTCBase {
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    CTCBase(CThing* thing);                                 // 0x00686800
};

struct CTCFireheartMinigame : CTCBase {                     // vtable 0x012709CC
    CThing*  Markers[4][4];                                 // +0x0c
    bool     InSession;                                     // +0x4c
    bool     ForceQuit;                                     // +0x4d
    CThing*  CurrentMarker;                                 // +0x50
    bool     Initialised;                                   // +0x54
    bool     BlocksActive[4][4];                            // +0x55
    long     XPos;                                          // +0x68
    long     YPos;                                          // +0x6c
    bool     Moving;                                        // +0x70
    CTCBase* PTCOldPhysics;                                 // +0x74
    CScriptGameResourceObjectScriptedThingBase ScriptData;  // +0x78
    bool     QuitWithDelay;                                 // +0x88
    CRetailVector<CEnginePrimitiveHandle> Handles;          // +0x8c
    CDefPointer<const CFireheartMinigameDef> PDef;          // +0x98
    virtual void v00();
    CTCFireheartMinigame(CThing* thing);
};

CTCFireheartMinigame::CTCFireheartMinigame(CThing* thing)
    : CTCBase(thing)
    , InSession(false)
    , ForceQuit(false)
    , Initialised(false)
    , Moving(false)
    , ScriptData()
    , QuitWithDelay(false)
    , Handles()
    , PDef()
{
    ParentThing->PDef->GetSubDef(&PDef);
}
