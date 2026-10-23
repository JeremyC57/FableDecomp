// Grade: DIFFER(748v756, /GS) -- 112 difflines: the uninitialised appearance local lives in memory (retail keeps it in ebp, with the SymbolsFiring walk pointer in ebx spilled per pass); the three ftol tint channels are evaluated in another order (retail +0x40, +0x3c, +0x38; tried RGBA and BGRA ctor orders and named channel locals); the pending-list count loop copies the head into edi where retail reloads InputsPending for front(). Event block (const-ref hero, inline CGameEvent ctor, AddLONG) and the jump table match.
// CTCOracleMinigame::UpdateTints() -- when no symbol is firing and the parent is not paused, take the next pending input: raise the expression game event for it, pop it, play the sequence-incorrect sound and quit when it is not the expected output, play the correct sound, then start that symbol firing; every firing symbol then fades its tint in over TimeBetweenOutputs (sine-eased towards the def's tint colour) and clears it when done
// Leaf: UpdateTints
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E6450, 738 bytes + a 4-entry jump table, plain `ret`.
#include <math.h>
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour(unsigned char b, unsigned char g, unsigned char r, unsigned char a) : B(b), G(g), R(r), A(a) {}
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CTCGraphicAppearance {
    void ClearTintColour(void* owner);                          // 0x004BFAE0
    void SetTintColour(const CRGBColour& colour, void* owner);  // 0x004C00B0
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x90 - 0x4c];
    signed char   PlayerNumber;                 // +0x90
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CParentThing {
    unsigned char _pad00[0x30];
    unsigned long Flags30;                      // +0x30 bit 15: paused
};
struct CPlayer { CThing* GetCharacterThing() const; };                      // 0x00487DC0
struct CPlayerManager { const CPlayer* GetMainPlayer() const; };      // 0x00449960
struct CMemoryBuffer { void Copy(unsigned long length, const void* data); };   // 0x009F1760
struct CGameEvent : CMemoryBuffer {
    long          Type;                           // +0x00
    signed char   Player;                         // +0x04
    unsigned char Data[32];                       // +0x05
    unsigned char EndPos;                         // +0x25
    bool          Valid;                          // +0x26
    bool          Replacement;                    // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void AddLONG(long value) { Copy(4, &value); }
};
struct CMainGameComponent {
    unsigned char   _pad00[0x1c];
    CPlayerManager* PlayerManager;                // +0x1c
    void AddGameEvent(const CGameEvent& event);   // 0x0041649C
};
extern CMainGameComponent* g_pMainGameComponent;  // 0x013B86A0
extern long g_FramesPerSecond;                    // 0x01375550
struct CWorld {
    static long GetFramesPerSecond() { return g_FramesPerSecond; }
};
long __fastcall GetWorldFrame();                  // 0x0049D870

struct COracleMinigameDef {
    unsigned char _pad00[0x38];
    unsigned long TintR;                          // +0x38
    unsigned long TintG;                          // +0x3c
    unsigned long TintB;                          // +0x40
};
struct SInputNode { SInputNode* Next; SInputNode* Prev; long Data; };

extern void free(void*);

struct CTCOracleMinigame {
    virtual ~CTCOracleMinigame();
    CParentThing* ParentThing;                    // +0x04
    unsigned char _pad08[0x20 - 0x08];
    float         TimeBetweenOutputs;             // +0x20
    long          NumOutputsFired;                // +0x24
    long          Output[32];                     // +0x28
    unsigned char _padA8[0xb0 - 0xa8];
    CThing*       Symbols[4];                     // +0xb0
    long          SymbolsFiring[4];               // +0xc0
    long          FramesFired[4];                 // +0xd0
    SInputNode*   InputsPending;                  // +0xe0 (STLport list head node)
    CThing*       SpeechMarker;                   // +0xe4
    const COracleMinigameDef* PDef;               // +0xe8
    CWorld* GetWorld();                           // 0x00686A70
    unsigned long PlaySound(const CCharString& name);   // 0x007E60F0
    void Quit();                                  // 0x007E62D0
    void UpdateTints();
};

void CTCOracleMinigame::UpdateTints()
{
    CTCGraphicAppearance* appearance;
    bool anyFiring = false;
    for (long i = 0; i < 4; ++i) {
        if (SymbolsFiring[i] != 0) {
            anyFiring = true;
            break;
        }
    }
    if (!anyFiring && !(ParentThing->Flags30 & 0x8000)) {
        long pending = 0;
        for (SInputNode* n = InputsPending->Next; n != InputsPending; n = n->Next)
            ++pending;
        if (pending != 0) {
            long input = InputsPending->Next->Data;
            long expression = -1;
            switch (input) {
            case 0: expression = 0; break;
            case 1: expression = 1; break;
            case 2: expression = 2; break;
            case 3: expression = 3; break;
            }
            CThing* const& hero = g_pMainGameComponent->PlayerManager->GetMainPlayer()->GetCharacterThing();
            CGameEvent event(0x27);
            event.Player = hero->PlayerNumber;
            event.AddLONG(expression);
            g_pMainGameComponent->AddGameEvent(event);
            SInputNode* node = InputsPending->Next;
            SInputNode* next = node->Next;
            SInputNode* prev = node->Prev;
            prev->Next = next;
            next->Prev = prev;
            free(node);
            if (Output[NumOutputsFired++] != input) {
                PlaySound("SND_MM_EXPRESSION_SEQUENCE_INCORRECT");
                Quit();
            }
            PlaySound("SND_MM_EXPRESSION_SEQUENCE_CORRECT");
            FramesFired[input] = GetWorldFrame();
            ++SymbolsFiring[input];
        }
    }
    for (long i = 0; i < 4; ++i) {
        if (SymbolsFiring[i] == 0)
            continue;
        Symbols[i]->GetInterface(&appearance, 0x5b);
        long fps = GetWorld()->GetFramesPerSecond();
        float t = (float)(GetWorldFrame() - FramesFired[i]) / ((float)fps * TimeBetweenOutputs);
        if (t > 1.0f) {
            if (--SymbolsFiring[i] != 0)
                FramesFired[i] = GetWorldFrame();
            appearance->ClearTintColour(this);
        } else {
            float s = (float)sin(t * 0.5f * 3.141592653589793);
            CRGBColour colour((unsigned char)(255.0f - PDef->TintB * s), (unsigned char)(255.0f - PDef->TintG * s),
                              (unsigned char)(255.0f - PDef->TintR * s), 0xff);
            appearance->SetTintColour(colour, this);
        }
    }
}
