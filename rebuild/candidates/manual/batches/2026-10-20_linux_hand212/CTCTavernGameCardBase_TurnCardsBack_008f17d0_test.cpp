// Behaviour test for 008f17d0: a self-contained copy of the candidate's model and body, run over stub callees.
// CTCTavernGameCardBase::TurnCardsBack() -- clears every card's highlight and flips each card that is face up and not already turning back over (with the flip sound), then goes to state 0x19
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __stdcall
#define __cdecl
#define __declspec(x)
#endif
struct CTCBase;
struct CCard;
struct CThingObject;

struct CCardVector {                            // STLport vector<CCard*> (12 bytes)
    CCard** _M_start;
    CCard** _M_finish;
    CCard** _M_end_of_storage;
    CCard** begin() { return _M_start; }
    CCard** end() { return _M_finish; }
};

struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

namespace NGraphicAppearance {
struct CTCGraphicAppearance {
    void ClearMeshEffect(void* key);            // 0x004BFFC0
};
}

struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
};
struct CCard {                                  // Ego_r.pdb CCard (0x58 bytes)
    CThing* Thing;                              // +0x00 CThingObject*
    long    Name;                               // +0x04
    long    Value;                              // +0x08
    float   InitialRotation;                    // +0x0c
    float   FinalRotation;                      // +0x10
    float   TurnSpeed;                          // +0x14
    float   TurnFraction;                       // +0x18
    float   MoveSpeed;                          // +0x1c
    float   MoveFraction;                       // +0x20
    bool    Turning;                            // +0x24
    bool    FaceDown;                           // +0x25
    void Turn()                                 // out-of-line copy 0x008F0620 (landed)
    {
        bool faceDown = FaceDown;
        Turning = true;
        TurnFraction = 0.0f;
        if (faceDown == true) {
            InitialRotation = 0.5f;
            FinalRotation = 0.0f;
        } else {
            InitialRotation = 0.0f;
            FinalRotation = 0.5f;
        }
        FaceDown = !faceDown;
    }
};

struct CTCTavernGame {
    virtual void v00();
    unsigned char _body04[0xf0 - 0x04];         // CTCBase + CTCTavernGame members (retail 0xF0)
    unsigned long PlaySound(const CCharString& name);   // 0x005EA950
    void SetState(long state);                  // 0x005E9B20
};

struct CTCTavernGameCardBase : CTCTavernGame {  // vtable 0x01288294
    unsigned char _pad_0xf0[0x38];
    CCardVector   Cards;                        // +0x128
    virtual void TurnCardsBack();
};

void CTCTavernGameCardBase::TurnCardsBack()
{
    for (CCard** it = Cards.begin(); it != Cards.end(); ++it) {
        CThing* thing = (*it)->Thing;
        NGraphicAppearance::CTCGraphicAppearance* appearance = 0;
        if (thing->HasTC(0x5b))
            appearance = (NGraphicAppearance::CTCGraphicAppearance*)thing->GetTC(0x5b);
        appearance->ClearMeshEffect(this);
        if (!(*it)->Turning && !(*it)->FaceDown) {
            {
                CCharString sound("TAVERN_GAME_CARD_FLIP");
                PlaySound(sound);
            }
            (*it)->Turn();
        }
    }
    SetState(0x19);
}

// ---- stubs and driver ----
void CTCTavernGame::v00() {}
static int g_sounds; static long g_state = -1;
CCharString::CCharString(const char* text, long) { m_pData = (void*)text; }
CCharString::~CCharString() {}
unsigned long CTCTavernGame::PlaySound(const CCharString& name) { if (std::strcmp((const char*)name.m_pData, "TAVERN_GAME_CARD_FLIP") == 0) ++g_sounds; return 0; }
void CTCTavernGame::SetState(long state) { g_state = state; }
static CInterfaceMap::SEntry g_entries[1];
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{ SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
static NGraphicAppearance::CTCGraphicAppearance g_appearance;
static int g_clears; static void* g_key;
void NGraphicAppearance::CTCGraphicAppearance::ClearMeshEffect(void* key) { if (this == &g_appearance) ++g_clears; g_key = key; }

int main()
{
    static CTCTavernGameCardBase game;
    static CThing thing;
    static CCard cards[3];
    static CCard* list[3] = { &cards[0], &cards[1], &cards[2] };
    thing.m_presentBits[0x5b >> 5] = 1UL << (0x5b & 31);
    g_entries[0].type = 0x5b; g_entries[0].component = (CTCBase*)&g_appearance;
    thing.m_interfaces.first = g_entries; thing.m_interfaces.last = g_entries + 1;
    for (int i = 0; i < 3; ++i) { cards[i].Thing = &thing; cards[i].InitialRotation = 9; cards[i].FinalRotation = 9; cards[i].TurnFraction = 9; }
    cards[1].FaceDown = true;          // already face down: left alone
    cards[2].Turning = true;           // mid-turn: left alone
    game.Cards._M_start = list; game.Cards._M_finish = list + 3; game.Cards._M_end_of_storage = list + 3;
    game.TurnCardsBack();
    if (g_clears != 3 || g_key != &game || g_sounds != 1 || g_state != 0x19) { std::printf("008f17d0_TEST FAIL (calls)\n"); return 1; }
    if (!cards[0].Turning || !cards[0].FaceDown || cards[0].TurnFraction != 0.0f || cards[0].InitialRotation != 0.0f || cards[0].FinalRotation != 0.5f)
        { std::printf("008f17d0_TEST FAIL (turned card)\n"); return 1; }
    if (cards[1].Turning || !cards[1].FaceDown || cards[1].TurnFraction != 9 || !cards[2].Turning || cards[2].FaceDown || cards[2].TurnFraction != 9)
        { std::printf("008f17d0_TEST FAIL (other cards)\n"); return 1; }
    std::printf("008f17d0_TEST PASS\n");
    return 0;
}
