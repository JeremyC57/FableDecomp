// Grade: DIFFER(435v428) -- register allocation: retail keeps the hero in edi (homed at [ebp-0x10], later reused as the TC key slot) and computes i*8 ONCE into edi for every Targets[i] access (ebx = &Targets[i] for the compare); ours homes the hero at [ebp-0xc], the out-param at [ebp-0x14] and recomputes i*8 at each access (frame 0x78 vs 0x74).
// NPlayerGui::CDrawSpiritTargetingCursor::Draw(const CEnginePrimitivePrepareInfo& info) -- for each of the 4 non-main players with a living hero:
// refresh the remembered target from the hero's targeting component (TC 8, slot 33), and when that target is alive and the GUI is not hidden,
// draw the spirit targeting cursor sprite 0.35 above the target's height in the player's multiplayer colour.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064C869, length 428 bytes, `ret 4`.
// Callees: CPlayerManager::GetPlayer 0x004498C0 / GetMultiplayerColour 0x00449B60; CPlayer::GetCharacterThing 0x00487DC0 / IsMainPlayer 0x00487520;
// IS_THING_ALIVE 0x0043DBF0 (CThing*) / 0x004CC340 (intelligent pointer); bitset::test 0x00410DE0; LowerBound 0x004365B0; 0x005CBDD2 (__fastcall
// target compare, descriptive); CIntelligentPointer(T*) 0x004A96F0 / operator= 0x004A9760 / dtor 0x00A01C10 / GetPItem 0x00A01B50; PeekGuiDef
// 0x00438570; 0x007EA2F0 (__fastcall, C3DVector by value; manifest "Creature::GetTargetFirePoint"); CEnginePrimitiveSprite(15 args) 0x006558CE;
// renderer slot 20 (+0x50). 0.35f 0x0125A264.
// Layout (descriptive): player manager +8, camera +0xc, owner +0x10 (+0x28 state, hidden byte +0x70), drawn +0x18, targets vector +0x28.
// Tried: POD vs copy-constructible C3DVector (the latter elides the position copy: 462 -> 435).
// Leaf: Draw
#pragma optimize("y", off)
#pragma optimize("s", on)
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour(const CRGBColour& o) : B(o.B), G(o.G), R(o.R), A(o.A) {}
    CRGBColour();
};
struct CTCBase;
struct CTCEntry { long Type; CTCBase* TC; };
struct CTCMap {
    CTCEntry* _M_start;
    CTCEntry* _M_finish;
    CTCEntry* _M_end_of_storage;
    CTCEntry* LowerBound(const unsigned long& type) const;   // 0x004365B0
};
struct CTCBitset {
    unsigned long _M_w[9];
    bool test(long bit) const;                           // 0x00410DE0
};
struct CThing {
    unsigned char _pad00[0x20];
    CTCBitset     UsingComponents;                       // +0x20
    CTCMap        Components;                            // +0x44
    unsigned char _pad50[0x60 - 0x50];
    struct SPhysics { unsigned char _pad[0x34]; float Height; }* PPhysics;   // +0x60
    template<class T> __forceinline bool GetInterface(T** out, long type) const
    {
        if (UsingComponents.test(type)) {
            unsigned long key = type;
            const CTCMap* map = &Components;
            CTCEntry* it = map->LowerBound(key);
            if (it == map->_M_finish || type < it->Type)
                it = map->_M_finish;
            *out = (T*)it->TC;
            return true;
        }
        return false;
    }
};
template<class T> bool __fastcall IS_THING_ALIVE(const T& thing);   // 0x0043DBF0 (CThing*), 0x004CC340 (CIntelligentPointer)
struct CBaseIntelligentPointer {
    void* m_pItem;
    void* m_pData;
    CThing* GetPItem() const;                      // 0x00A01B50
    ~CBaseIntelligentPointer();                    // 0x00A01C10
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer(T* item);                  // 0x004A96F0
    CIntelligentPointer& operator=(const CIntelligentPointer& other);   // 0x004A9760
};
bool __fastcall TargetDiffers(CThing* thing, const CIntelligentPointer<CThing>& target);   // 0x005CBDD2
struct CTCTargeting {                                    // interface type 8
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32();
    virtual CThing* GetTarget();                         // slot 33 (+0x84)
};
struct CPlayer {
    unsigned char _pad00[0x28];
    long PlayerNumber;                                   // +0x28
    CThing* GetCharacterThing();                         // 0x00487DC0
    bool IsMainPlayer() const;                           // 0x00487520
};
struct CPlayerManager {
    CPlayer* GetPlayer(long index);                      // 0x004498C0
    CRGBColour GetMultiplayerColour(long player);        // 0x00449B60
};
struct CPlayerGuiDef {
    unsigned char _pad000[0x740];
    unsigned long SpiritTargetingCursorGraphic;          // +0x740
};
const CPlayerGuiDef* __fastcall PeekGuiDef();            // 0x00438570
struct CTargetingCamera;
C3DVector __fastcall GetTargetPosition(CTargetingCamera* camera, CThing* target, float time);   // 0x007EA2F0
struct CEnginePrimitiveSprite {
    unsigned char _body[0x48];
    CEnginePrimitiveSprite(const C3DVector& pos, unsigned long graphic, unsigned long a, unsigned short flags, float b, float c, float d,
                           long blend, unsigned char alpha, unsigned char red, unsigned char green, unsigned char blue, float e,
                           long interpolation, long f);   // 0x006558CE
};
struct CEnginePrimitiveRenderer {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19();
    virtual void AddPrimitive(const CEnginePrimitiveSprite& sprite);   // slot 20 (+0x50)
};
struct CEnginePrimitivePrepareInfo {
    unsigned char _pad00[4];
    float Time;                                          // +0x04
    unsigned char _pad08[0x34 - 8];
    CEnginePrimitiveRenderer* Renderer;                  // +0x34
};
struct CGuiState { unsigned char _pad[0x70]; bool Hidden; };   // +0x70
struct CGuiOwner { unsigned char _pad[0x28]; CGuiState* State; };   // +0x28

namespace NPlayerGui {
struct CDrawSpiritTargetingCursor {
    unsigned char _pad00[8];
    CPlayerManager* PPlayerManager;                      // +0x08
    CTargetingCamera* PCamera;                           // +0x0c
    CGuiOwner* POwner;                                   // +0x10
    unsigned char _pad14[4];
    bool Drawn;                                          // +0x18
    unsigned char _pad19[0x28 - 0x19];
    CIntelligentPointer<CThing>* Targets;                // +0x28 vector start
    void Draw(const CEnginePrimitivePrepareInfo& info);
};

void CDrawSpiritTargetingCursor::Draw(const CEnginePrimitivePrepareInfo& info)
{
    for (long i = 0; i < 4; ++i) {
        CPlayer* player = PPlayerManager->GetPlayer(i);
        CThing* hero = player->GetCharacterThing();
        if (player->IsMainPlayer() || !IS_THING_ALIVE(hero))
            continue;
        CTCTargeting* targeting;
        hero->GetInterface(&targeting, 8);
        if (TargetDiffers(targeting->GetTarget(), Targets[i]))
            Targets[i] = CIntelligentPointer<CThing>(targeting->GetTarget());
        if (IS_THING_ALIVE(Targets[i]) && !POwner->State->Hidden) {
            Drawn = true;
            const CPlayerGuiDef* def = PeekGuiDef();
            CThing* target = Targets[i].GetPItem();
            C3DVector pos = GetTargetPosition(PCamera, target, info.Time);
            pos.Z = pos.Z + target->PPhysics->Height + 0.35f;
            CRGBColour colour = PPlayerManager->GetMultiplayerColour(player->PlayerNumber);
            CEnginePrimitiveSprite sprite(pos, def->SpiritTargetingCursorGraphic, 0, 0x81, 0.0f, 1.0f, 1.0f, 2, 0xff,
                                          colour.R, colour.G, colour.B, 0.0f, 0, 0);
            info.Renderer->AddPrimitive(sprite);
        }
    }
}
}
