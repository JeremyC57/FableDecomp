// CTCTavernGameShoveHaPenny::EnterGame() -- coin-base EnterGame, put the camera on the def's named camera point, rounds played reset
// Leaf: ?EnterGame@CTCTavernGameShoveHaPenny
// Retail: Fable.exe @ 0x007CF530, 123 bytes; CTCTavernGameShoveHaPenny vtable 0x0126F2F4 slot 29 (EnterGame; the catalog's
//         "vfunc_29").
// Callees: CTCTavernGameCoinBase::EnterGame() 0x008EF390 (non-virtual base call); CTCTavernGame::GetTavernGameDef(CDefPointer*)
//          0x005EA000; CDefStringTable::GetString(long) 0x009D49B0 on g_DefStringTable 0x013CA828 (CCharString by hidden
//          pointer); CTCTavernGame::CameraUseCameraPoint(const CCharString&) 0x005EBFA0; CCharString::~CCharString 0x0099EAE0;
//          the def's slot 1 on release.
// Layout: CTavernGameDef::CameraName (CDefString, a string-table id) +0x90 (PDB offset holds); retail
//         CTCTavernGameShoveHaPenny::RoundsPlayed +0x148 (PDB +0x160).
// Shape: the camera name is a named local in its own block (`lea ecx ; push ecx`, destroyed before the RoundsPlayed store);
//        null-tested CDefPointer dtor with the def's inline ReleaseTemporary().
struct CCharString {
    void* m_data;
    ~CCharString();                             // 0x0099EAE0
};
struct CDefStringTable {
    CCharString GetString(long id);             // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;        // 0x013CA828

struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
struct CTavernGameDef : CDefVirtual, CRefCounted {
    unsigned char _pad_0x08[0x88];
    long CameraName;                            // +0x90 CDefString
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
struct CDefPointer {
    CTavernGameDef* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer() { if (Object) Object->ReleaseTemporary(); }
    CTavernGameDef* operator->() const { return Object; }
};

struct CTCTavernGameCoinBase {
    virtual void v00();
    void EnterGame();                           // 0x008EF390
    void GetTavernGameDef(CDefPointer* out);    // 0x005EA000 (CTCTavernGame)
    void CameraUseCameraPoint(const CCharString& name);   // 0x005EBFA0 (CTCTavernGame)
    unsigned char _pad_0x04[0x144];
};

struct CTCTavernGameShoveHaPenny : CTCTavernGameCoinBase {
    long RoundsPlayed;                          // +0x148
    void EnterGame();
};

void CTCTavernGameShoveHaPenny::EnterGame()
{
    CTCTavernGameCoinBase::EnterGame();
    CDefPointer def;
    GetTavernGameDef(&def);
    {
        CCharString camera = g_DefStringTable.GetString(def->CameraName);
        CameraUseCameraPoint(camera);
    }
    RoundsPlayed = 0;
}