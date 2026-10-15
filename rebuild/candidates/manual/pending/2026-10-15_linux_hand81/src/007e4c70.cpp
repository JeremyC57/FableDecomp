// CTCCreditsUI::~CTCCreditsUI() -- destroy CreditPart (+0x20) and free the Components buffer (+0x10), then ~CTCBase by tail call
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E4C70, length 35 bytes.
// Callees: CCharString::~CCharString @ 0x0099EAE0 (manifest label "_Cons_val<...>" is propagated),
// free @ 0x00BFEA14 (explicit null test, `add esp,4`), CTCBase::~CTCBase @ 0x00686830 (tail jump; it
// stores the CTCBase vtable 0x0125BE0C; manifest label "~CChunkedFileChunk" is propagated).
// Identity: the only caller is the deleting destructor @ 0x007E4C50, slot 0 of vtable 0x012705AC =
// CTCCreditsUI (rtti-vtables.tsv); the manifest label "CTCAssassinRush::FinishMove" is propagated.
// Layout = Ego_r.pdb CTCCreditsUI with the retail 12-byte vector: Active +0xc, Components
// (vector<NUISystem::CComponent*>) +0x10, Quitting +0x1c, CreditPart +0x20, LastRenderTime +0x28.
// engine/CTCBase.h is a layout-only header, so the base is modelled with its destructor (as the
// landed 0x007B2720). Shape: no vtable store of its own before the member destructors =
// `__declspec(novtable)`; the members go in reverse declaration order; a non-virtual destructor
// beside a virtual function keeps the lander on `??1`.
// Leaf: ??1CTCCreditsUI
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

namespace NUISystem { struct CComponent; }

struct CCharString {
    void* PStringData;
    ~CCharString();                                         // 0x0099EAE0
};

struct CComponentVector {                                   // std::vector<NUISystem::CComponent*>, retail 12 bytes
    NUISystem::CComponent** m_pFirst;
    NUISystem::CComponent** m_pLast;
    NUISystem::CComponent** m_pEnd;
    ~CComponentVector()
    {
        if (m_pFirst != 0)
            free(m_pFirst);
    }
};

struct CTCBase_Calls {                                      // engine/CTCBase.h layout (0xc bytes)
    virtual void v00();
    unsigned char _pad04[0x8];                              // ParentThing, ToBeRemoved, IsInGlobalUpdateTCs
    ~CTCBase_Calls();                                       // 0x00686830
};

struct __declspec(novtable) CTCCreditsUI : CTCBase_Calls {
    bool             Active;                                // +0x0c
    CComponentVector Components;                            // +0x10
    bool             Quitting;                              // +0x1c
    CCharString      CreditPart;                            // +0x20
    double           LastRenderTime;                        // +0x28
    virtual void v01();
    ~CTCCreditsUI();
};

CTCCreditsUI::~CTCCreditsUI()
{
}
