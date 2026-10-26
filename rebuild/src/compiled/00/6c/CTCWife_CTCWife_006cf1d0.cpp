// CTCWife::~CTCWife() -- wife component dtor: un-pause the game (event 9, false) for each of a pending divorce, dowry or sex scene, end a pending screen fade-out, leave the global TC list, then destroy the seen-clothing set, the gifts map and the marriage def pointer and run ~CTCBase
// Leaf: ??1CTCWife
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CF1D0, length 480 bytes (/GS cookie 0x0139C8A8); vtable 0x0125ED5C.
// Callees: CTCBase::PeekWorld() const 0x00686A80; CWorld::GetCurrentPlayer() 0x0049D9D0; CMemoryBuffer::Copy 0x009F1760;
//   CMainGameComponent::AddGameEvent 0x0041649C on g_pMainGameComponent 0x013B86A0; CDisplayEngine::EndScreenFadeOut
//   0x00434C90 (display engine at g_pMainGameComponent+0x28); CTCBase::RemoveFromGlobalTCs 0x00686DF0;
//   _Rb_tree<set<long>>::_M_erase 0x004A9CB0, _Rb_tree<map<long,long>>::_M_erase 0x004DA2AC; free 0x00BFEA14;
//   CTCBase::~CTCBase 0x00686830 (catalogued ~CChunkedFileChunk).
// Layout: the landed CTCWife ctor 0x006CF040 model (retail STLport trees are 12 bytes): PHeroMarriageDef +0x0C,
//   WaitingForFadeOut +0x27, WaitingForDivorce +0x29, WaitingForDowry +0x2B, WaitingForSex +0x2C, GiftsReceived +0x78,
//   ThingsSeenThePlayerWear +0x98.
// Shape: the un-pause send is a `__forceinline` member building the landed CGameEvent model and adding a literal false
//   through a BY-VALUE AddBool(bool) (its parameter copy is the dword slot stored after the Copy pushes; a const& temp lands
//   on a byte slot, the out-of-line SendPauseEvent(bool&) form keeps the setne normalisation); the event's constant 9 lives in edi; the STLport tree dtor is `clear()` (erase, re-link the header, zero the count) then the header
//   node released through `free`; the def pointer release is the pointee's inline `Release()` (`if (--RefCount == 0) DeleteThis()`,
//   through ecx; open-coded in the holder dtor it goes through eax) with the zero store inside the null test.
extern "C" void __cdecl free(void* p);          // 0x00BFEA14
class CThing;
struct CMemoryBuffer {
    void Copy(unsigned long length, const void* data);   // 0x009F1760
};
class CGameEvent : public CMemoryBuffer { public:
    long          Type;                            // +0x00
    signed char   Player;                          // +0x04
    unsigned char Data[32];                        // +0x05
    unsigned char EndPos;                          // +0x25
    bool          Valid;                           // +0x26
    bool          Replacement;                     // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void AddBool(bool value) { Copy(1, &value); }
};
class CDisplayEngine { public:
    void EndScreenFadeOut();                       // 0x00434C90
};
class CMainGameComponent { public:
    unsigned char   _pad00[0x28];
    CDisplayEngine* PDisplayEngine;                // +0x28
    void AddGameEvent(const CGameEvent& event);    // 0x0041649C
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0
class CPlayer { public:
    unsigned char _pad_0x00[0x28];
    signed char   m_playerNumber;                  // +0x28
};
class CWorld { public:
    CPlayer* GetCurrentPlayer() const;             // 0x0049D9D0
};
class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                           // +0x04
    bool ToBeRemoved;                              // +0x08
    bool IsInGlobalUpdateTCs;                      // +0x09
    virtual ~CTCBase();                                    // 0x00686830
    void RemoveFromGlobalTCs();                    // 0x00686DF0
protected:
    const CWorld& PeekWorld() const;   // 0x00686A80
};
struct SLongLongNode {                             // map<long, long> node
    unsigned char m_color;
    SLongLongNode* m_pParent;
    SLongLongNode* m_pLeft;
    SLongLongNode* m_pRight;
    long m_key;
    long m_value;
};
struct SLongNode {                                 // set<long> node
    unsigned char m_color;
    SLongNode* m_pParent;
    SLongNode* m_pLeft;
    SLongNode* m_pRight;
    long m_key;
};
struct CLongLongTree {                             // map<long, long> (12 bytes)
    SLongLongNode* m_pHeader;
    unsigned long m_nodeCount;
    unsigned long m_compare;
    void _M_erase(SLongLongNode* node);            // 0x004DA2AC
    void clear()
    {
        if (m_nodeCount != 0) {
            _M_erase(m_pHeader->m_pParent);
            m_pHeader->m_pLeft = m_pHeader;
            m_pHeader->m_pParent = 0;
            m_pHeader->m_pRight = m_pHeader;
            m_nodeCount = 0;
        }
    }
    ~CLongLongTree() { clear(); if (m_pHeader) free(m_pHeader); }
};
struct CLongTree {                                 // set<long> (12 bytes)
    SLongNode* m_pHeader;
    unsigned long m_nodeCount;
    unsigned long m_compare;
    void _M_erase(SLongNode* node);                // 0x004A9CB0
    void clear()
    {
        if (m_nodeCount != 0) {
            _M_erase(m_pHeader->m_pParent);
            m_pHeader->m_pLeft = m_pHeader;
            m_pHeader->m_pParent = 0;
            m_pHeader->m_pRight = m_pHeader;
            m_nodeCount = 0;
        }
    }
    ~CLongTree() { clear(); if (m_pHeader) free(m_pHeader); }
};
class CHeroMarriageDef { public:
    virtual void v00();
    virtual void DeleteThis();                     // slot 1
    long RefCount;                                 // +0x04
    void Release() { if (--RefCount == 0) DeleteThis(); }
};
struct CDefPointerHeroMarriage {
    CHeroMarriageDef* Object;
    ~CDefPointerHeroMarriage()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
};
class CTCWife : public CTCBase { public:                         // vtable 0x0125ED5C
    CDefPointerHeroMarriage PHeroMarriageDef;      // +0x0c
    long OpinionAttitude;                          // +0x10
    unsigned char AttitudeUpdateCounter;           // +0x14
    bool CourtingBlocked;                          // +0x15
    long CurrentRegionNumber;                      // +0x18
    bool PermittedToRegionFollow;                  // +0x1c
    long FrameGotMarriedToThePlayer;               // +0x20
    bool DivorcedHero;                             // +0x24
    bool JustMarried;                              // +0x25
    bool NeedsToChangeBrain;                       // +0x26
    bool WaitingForFadeOut;                        // +0x27
    bool WaitingForFadeIn;                         // +0x28
    bool WaitingForDivorce;                        // +0x29
    bool WaitingForDivorceRenting;                 // +0x2a
    bool WaitingForDowry;                          // +0x2b
    bool WaitingForSex;                            // +0x2c
    bool WaitingForSexCutscene;                    // +0x2d
    unsigned char _pad30[0x78 - 0x30];
    CLongLongTree GiftsReceived;                   // +0x78
    unsigned char _pad84[0x98 - 0x84];
    CLongTree ThingsSeenThePlayerWear;             // +0x98
    unsigned char _padA4[0xec - 0xa4];
    virtual void v01();
    ~CTCWife();
    __forceinline void SendUnpauseEvent()
    {
        CGameEvent gameEvent(9);
        gameEvent.Player = PeekWorld().GetCurrentPlayer()->m_playerNumber;
        gameEvent.AddBool(false);
        g_pMainGameComponent->AddGameEvent(gameEvent);
    }
};

CTCWife::~CTCWife()
{
    if (WaitingForDivorce)
        SendUnpauseEvent();
    if (WaitingForDowry)
        SendUnpauseEvent();
    if (WaitingForSex)
        SendUnpauseEvent();
    if (WaitingForFadeOut)
        g_pMainGameComponent->PDisplayEngine->EndScreenFadeOut();
    RemoveFromGlobalTCs();
}