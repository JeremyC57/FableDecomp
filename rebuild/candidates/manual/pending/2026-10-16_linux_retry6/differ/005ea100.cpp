// CTCTavernGame::vfunc_42() const -- the end-of-game result text: empty while the game is unfinished, else the def's win or lose text looked up through GetText
// Grade: DIFFER(184v158) -- def release shape: retail releases the def with a null test on the unfinished arm and without one (def.Object kept in edi, `dec [edi+4]`) on the finished arm, and zeroes the def after the GetSubDef push; ours null-tests both and re-reads the frame word | retry 6 (Fable, 3 tries): (1) def per arm: 8-byte frame; (2) one def before the branch + early return of CCharString("").ToWideString(): frame now matches (push ecx, temp in the hidden-pointer slot) and only the release differs -- ours re-reads RefCount after `dec` and re-reads def.Object from the frame on the finished arm where retail keeps it in edi; (3) release through a local copy of Object: worse (dec in a register). CDefPointer release re-read remains a lane gap.
// Leaf: vfunc_42
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EA100, length 158 bytes (`ret 4`, CWideString through the hidden pointer); CTCTavernGame
// vtable 0x012560BC slot 42.
// Callees: CDefClassBase::GetSubDef<CTavernGameDef>(CDefPointer<const CTavernGameDef>*) @ 0x005ED4A0 (owner def +0x70; manifest instantiation
// "<CAICreatureWillPowerIndicatorDef>" is a byte twin), CCharString::CCharString(const char*, long) @ 0x0099EBF0 ("" @ 0x0122D70E),
// CCharString::ToWideString() const @ 0x0099E480, ~CCharString @ 0x0099EAE0, CTCTavernGame::GetText(unsigned long) @ 0x005E9E70 (manifest
// "CTCBetting::GetText" is the shared body), the def's virtual slot 1 (destroy).
// Layout (retail; the PDB header's 0x14-byte UIComponents vector is 12 bytes in retail): UIComponents +0x0C, LastRenderTime +0x18,
// IsBeingPlayed +0x20, GameWon +0x21, NumRounds +0x24, GameFinished +0x28. Tavern game def: win text +0xA4, lose text +0xA8.
// Shape: the def pointer is a zeroed local filled by GetSubDef; the unfinished arm converts a CCharString temporary (built in the dead
// hidden-pointer slot) and releases the def with a null test, the finished arm dereferences the def and releases it without one.
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    struct CWideString ToWideString() const;           // 0x0099E480
};
struct CWideString {
    void* m_pData;
    CWideString(const CWideString& other);
    ~CWideString();
};
struct CTavernGameDef {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long RefCount;                              // +0x04
    unsigned char _pad08[0xa4 - 0x08];
    unsigned long WinText;                      // +0xa4
    unsigned long LoseText;                     // +0xa8
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        if (Object && --Object->RefCount == 0)
            Object->Destroy();
    }
    T* operator->() const { return Object; }
};
struct CDefClassBase {
    bool GetSubDef(CDefPointer<CTavernGameDef>* out) const;   // 0x005ED4A0
};
struct CThingDefHolder {
    unsigned char _pad00[0x70];
    CDefClassBase* PDef;                        // +0x70
};
struct CTCTavernGame {
    void* __vfptr;                              // +0x00 (CTCBase)
    CThingDefHolder* ParentThing;               // +0x04
    unsigned char _base08[4];
    void* UIComponents[3];                      // +0x0c
    double LastRenderTime;                      // +0x18
    bool IsBeingPlayed;                         // +0x20
    bool GameWon;                               // +0x21
    long NumRounds;                             // +0x24
    bool GameFinished;                          // +0x28
    CWideString GetText(unsigned long textID) const;   // 0x005E9E70
    CWideString vfunc_42() const;
};

CWideString CTCTavernGame::vfunc_42() const
{
    CDefPointer<CTavernGameDef> def;
    ParentThing->PDef->GetSubDef(&def);
    if (!GameFinished)
        return CCharString("").ToWideString();
    CTavernGameDef* object = def.Object;
    return GetText(GameWon ? object->WinText : object->LoseText);
}
