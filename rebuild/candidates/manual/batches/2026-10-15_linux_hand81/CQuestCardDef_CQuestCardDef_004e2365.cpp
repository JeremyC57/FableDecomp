// CQuestCardDef::~CQuestCardDef() -- destroy the Prerequisites map (+0x5c) and free the RewardObjects buffer (+0x4c), then the inlined CDefPointeeBase destructor stores its vtable and tail-calls the root destructor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E2365, length 39 bytes.
// Callees: the map destructor @ 0x004DD118 (out of line: clear, then free the head node),
// free @ 0x00BFEA14 (the vector buffer, explicit null test, `pop ecx`: a size-optimised TU),
// CResource::~CResource @ 0x009FC550 (tail call; manifest label "~CMemoryBlock" is propagated).
// Identity: the only caller is the deleting destructor @ 0x004E2349, slot 0 of vtable 0x01241E44 =
// CQuestCardDef (rtti-vtables.tsv); the manifest label "CTCAssassinRush::FinishMove" is propagated.
// Vtable 0x01230BA0 = CDefPointeeBase: the store of its inline, empty destructor.
// Layout = Ego_r.pdb CQuestCardDef with the retail 12-byte vector: RewardObjects (vector<long>) at
// +0x4c, the four bools at +0x58..+0x5b, Prerequisites (map<EQuestCardPrerequisite,long>) at +0x5c
// (PDB +0x60: the PDB vector is 16 bytes). Same shape as the landed 0x004E017A: the most-derived
// class is `__declspec(novtable)` (its own vtable store never appears), the members are destroyed in
// reverse declaration order, and a non-virtual destructor beside a virtual function keeps the
// lander on the plain `??1` body.
// Leaf: ??1CQuestCardDef
#pragma optimize("s", on)
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

struct CRetailVectorLong {                                  // std::vector<long>, retail 12 bytes
    long* m_pFirst;
    long* m_pLast;
    long* m_pEnd;
    ~CRetailVectorLong()
    {
        if (m_pFirst != 0)
            free(m_pFirst);
    }
};

struct CRetailMapPrerequisites {                            // std::map<EQuestCardPrerequisite, long>
    void* m_pHead;
    unsigned char _rest[0xc];
    ~CRetailMapPrerequisites();                             // 0x004DD118
};

struct CResource_Calls {                                    // engine/CResource.h: the root of every def
    virtual void v00();
    ~CResource_Calls();                                     // 0x009FC550
};

struct CDefPointeeBase : CResource_Calls {                  // vtable 0x01230BA0
    virtual void v00();
    ~CDefPointeeBase() { }                                  // inline, empty: inlined into every derived destructor
};

struct __declspec(novtable) CQuestCardDef : CDefPointeeBase {
    unsigned char           _pad04[0x4c - 0x04];            // def header, QuestName .. GoldReward
    CRetailVectorLong       RewardObjects;                  // +0x4c
    bool                    IsCore;                         // +0x58
    bool                    IsVignette;                     // +0x59
    bool                    IsExclusive;                    // +0x5a
    bool                    MakeVignetteRouteAppearOnMinimap; // +0x5b
    CRetailMapPrerequisites Prerequisites;                  // +0x5c
    virtual void v00();
    ~CQuestCardDef();
};

CQuestCardDef::~CQuestCardDef()
{
}
