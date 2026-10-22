// NPlayerGui::CDrawGuildSeal::Draw(CEnginePrimitivePrepareInfo*) -- run the guild-seal reminder timer (re-enable the flash with the seal graphic once it passes the GUI def's reminder time), step the flash fade (close when open, else re-open at half the flash time while cycles remain), store the flash alpha and, when ready to level up and visible, draw the seal (quest-core back graphic when the current graphic is HUD_ORB_QUEST_CORE) if the hero carries OBJECT_GUILD_SEAL_1, then one sprite per queued position; disable once the cycles run out
// Grade: DIFFER(911v911) -- same length, call sequence and x87 order; the frame differs (0x80 v 0x78): retail shares the hero slot with the interface-map pointer and the seal-name string and the key slot with the seal index, and hoists the loop's zero UV stores after the loop guard. Tried: inline HasGuildSeal (fixed length), scoped hero / inventory blocks, named vs temporary CCharString with a definitions local, UV locals before / inside the loop, node block, hero as inline parameter
// Leaf: Draw
#pragma optimize("y", off)
#pragma optimize("s", on)
struct C2DVector {
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector operator+(const C2DVector& o) const { return C2DVector(X + o.X, Y + o.Y); }
};
struct C2DCoordF {
    float X, Y;
    C2DCoordF(const C2DVector& v) : X(v.X), Y(v.Y) {}
};
C2DVector __fastcall GFAnchorToBottomLeft(const C2DVector& in);         // 0x00643CDA
struct CRGBColour {
    unsigned char B, G, R, A;
    __forceinline CRGBColour(const CRGBColour& o) : B(o.B), G(o.G), R(o.R), A(o.A) {}
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
struct CEnginePrimitiveHandle { unsigned long Data[2]; };
class CEnginePrimitive2DSprite {
public:
    unsigned char _body[0x54];
    CEnginePrimitive2DSprite(unsigned __int64 id, const C2DCoordF& pos, long layer, unsigned long graphic, unsigned long b, long blend,
                             long interpolation, float scaleX, float scaleY, unsigned char alpha, unsigned char red, unsigned char green,
                             unsigned char blue, unsigned long e, const C2DVector& uv0, const C2DVector& uv1);   // 0x00493AC2
};
class CEnginePrimitiveRenderer {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22();
    virtual void AddPrimitive(CEnginePrimitiveHandle* handle, const CEnginePrimitive2DSprite& primitive, long flags);   // slot 23
};
class CEnginePrimitivePrepareInfo {
public:
    float                     FrameTime;            // +0x00
    unsigned char             _pad04[0x30];
    CEnginePrimitiveRenderer* Renderer;             // +0x34
};
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length);    // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
};
struct CTCBase;
struct CPresenceBits {
    bool test(long bit);                           // 0x00410DE0 (std::bitset::test)
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
struct CThing {
    unsigned char _pad0[0x20];
    CPresenceBits m_present;                       // +0x20
    unsigned char _pad1[0x23];
    CInterfaceMap m_interfaces;                    // +0x44
    bool HasInterface(long type) { return m_present.test(type); }
    template <class T> __forceinline T* GetInterfaceUncheckedConst(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
};
struct CThingCreatureBase : CThing {
    bool IsChild();                                // 0x00661F70
};
struct CThingPlayerCreature : CThingCreatureBase { };
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);   // 0x0043DBF0
struct CPlayer {
    CThingPlayerCreature* GetControlledCreature(); // 0x00487DD0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                      // 0x00449970
};
struct CTCInventoryBase {                          // the thing's interface 0x11
    long GetNumberOfItemsOfTypeInInventory(long defIndex);   // 0x005BDF08
};
struct CDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
struct CWorld {
    unsigned char       _pad00[0x10];
    CDefinitionManager* Definitions;               // +0x10
    unsigned char       _pad14[0xa4 - 0x14];
    bool                Paused;                    // +0xa4
};
struct CGraphicNode {                              // map<CCharString, long> node
    unsigned char _pad00[0x10];
    CCharString   Name;                            // +0x10
    long          Graphic;                         // +0x14
};
struct CGraphicMap {
    CGraphicNode* Header;
    CGraphicNode* Find(const CCharString& name);   // 0x0042B467
};
struct CGuiDef {
    unsigned char _pad000[0x6c];
    float         GuildSealFlashTime;              // +0x6c
    float         GuildSealReminderTime;           // +0x70
    unsigned char _pad074[0x174 - 0x74];
    C2DVector     GuildSealOffset;                 // +0x174
    unsigned char _pad17c[0x180 - 0x17c];
    C2DVector     GuildSealPos;                    // +0x180
    unsigned char _pad188[0x1bc - 0x188];
    long          GuildSealGraphic;                // +0x1bc
    unsigned char _pad1c0[0x1f4 - 0x1c0];
    long          GuildSealBackGraphic;            // +0x1f4
    long          GuildSealQuestBackGraphic;       // +0x1f8
    unsigned char _pad1fc[0x934 - 0x1fc];
    CGraphicMap   Graphics;                        // +0x934
};
extern CGuiDef* __fastcall PeekGuiDef();           // 0x00438570
struct CFadeInFadeOutBase {
    bool       Active;                             // +0x00
    float      FadeInTime;                         // +0x04
    float      FadeOutTime;                        // +0x08
    bool       Closing;                            // +0x0c
    bool       Opening;                            // +0x0d
    float      OpenTimer;                          // +0x10
    float      CloseTimer;                         // +0x14
    CRGBColour ToColour;                           // +0x18
    void Open(float fade_in_time, float fade_out_time, CRGBColour to_colour);   // 0x00434840
    void Update(float timeDelta);                  // 0x00434870
    float GetFadeInOutAlpha();                     // 0x004348D0
    bool IsOpen();                                 // 0x0048DA30
};
struct CHandleVector { CEnginePrimitiveHandle *Start, *Finish, *End; };
struct CPositionVector {
    C2DVector *Start, *Finish, *End;
    unsigned int size() const { return Finish - Start; }
};

namespace NPlayerGui {
struct CDrawGuildSeal {
    virtual void v00();
    void*           PlayerGui;                     // +0x04
    CPlayerManager* PlayerManager;                 // +0x08
    CWorld*         World;                         // +0x0c
    void*           Resources;                     // +0x10
    void*           Bank;                          // +0x14
    bool            Drawing;                       // +0x18
    CHandleVector   Handles;                       // +0x1c
    bool            Enabled;                       // +0x28
    bool            ReadyToLevelUp;                // +0x29
    unsigned char   FlashAlpha;                    // +0x2a
    long            FlashCycles;                   // +0x2c
    CFadeInFadeOutBase FadeInOut;                  // +0x30
    float           ReminderTime;                  // +0x4c
    CPositionVector Positions;                     // +0x50
    long            Graphic;                       // +0x5c
    unsigned char GetFadedAlpha(unsigned char alpha);   // 0x00643D5D
    void SetEnabled(bool enabled);                 // 0x00643F7C
    __forceinline bool HasGuildSeal()
    {
        CThingCreatureBase* hero = PlayerManager->GetMainPlayer()->GetControlledCreature();
        if (IS_THING_ALIVE<CThingCreatureBase*>(hero) && !hero->IsChild() && hero->HasInterface(0x11)) {
            CTCInventoryBase* inventory = hero->GetInterfaceUncheckedConst<CTCInventoryBase>(0x11);
            long seal = World->Definitions->GetDefGlobalIndexFromName(CCharString("OBJECT_GUILD_SEAL_1", -1));
            if (seal > 0 && inventory->GetNumberOfItemsOfTypeInInventory(seal) > 0)
                return true;
        }
        return false;
    }
    void Draw(CEnginePrimitivePrepareInfo* info);
};

void CDrawGuildSeal::Draw(CEnginePrimitivePrepareInfo* info)
{
    if (ReadyToLevelUp && !World->Paused) {
        CGuiDef* def = PeekGuiDef();
        ReminderTime += info->FrameTime;
        if (ReminderTime > def->GuildSealReminderTime) {
            Graphic = def->GuildSealGraphic;
            SetEnabled(true);
        }
    }
    if (FlashCycles > 0) {
        if (Enabled) {
            CGuiDef* def = PeekGuiDef();
            if (!World->Paused) {
                FadeInOut.Update(info->FrameTime);
                if (FadeInOut.IsOpen()) {
                    FadeInOut.Opening = false;
                    FadeInOut.Closing = true;
                } else if (!FadeInOut.Active) {
                    if (--FlashCycles > 0) {
                        FadeInOut.Open(def->GuildSealFlashTime * 0.5f, def->GuildSealFlashTime * 0.5f, CRGBColour(0, 0, 0, 0xff));
                    }
                }
            }
            FlashAlpha = (unsigned char)(FadeInOut.GetFadeInOutAlpha() * 255.0f);
            if (ReadyToLevelUp && FlashAlpha > 0) {
                if (HasGuildSeal()) {
                    Drawing = true;
                    long back = def->GuildSealBackGraphic;
                    CGraphicNode* node;
                    {
                        CCharString name("HUD_ORB_QUEST_CORE", -1);
                        node = def->Graphics.Find(name);
                    }
                    if (node != def->Graphics.Header && node->Graphic == Graphic)
                        back = def->GuildSealQuestBackGraphic;
                    C2DVector uv1(0.0f, 0.0f);
                    C2DVector uv0(0.0f, 0.0f);
                    CEnginePrimitive2DSprite sprite(0, GFAnchorToBottomLeft(def->GuildSealPos + def->GuildSealOffset), 0x44, back, 0, 2, 0,
                                                    0.0f, 0.0f, GetFadedAlpha(FlashAlpha), 0xff, 0xff, 0xff, 0, uv0, uv1);
                    info->Renderer->AddPrimitive(&Handles.Start[0], sprite, 0xc0);
                }
                if (Positions.Start != Positions.Finish) {
                    Drawing = true;
                    if (Graphic == 0)
                        Graphic = def->GuildSealGraphic;
                    C2DVector uv1(0.0f, 0.0f);
                    C2DVector uv0(0.0f, 0.0f);
                    for (unsigned int i = 0; i < Positions.size(); ++i) {
                        CEnginePrimitive2DSprite sprite(0, Positions.Start[i], 0x44, Graphic, 0, 2, 0, 0.0f, 0.0f, GetFadedAlpha(FlashAlpha),
                                                        0xff, 0xff, 0xff, 0, uv0, uv1);
                        info->Renderer->AddPrimitive(&Handles.Start[i + 1], sprite, 0xc0);
                    }
                }
            }
        }
    }
    if (FlashCycles == 0)
        SetEnabled(false);
}
}
