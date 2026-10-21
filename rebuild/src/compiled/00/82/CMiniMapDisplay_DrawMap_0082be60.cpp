// CMiniMapDisplay::DrawMap(CEnginePrimitivePrepareInfo& info, const C2DBoxF& box, unsigned char alpha, bool front) -- draw the minimap sprite (switching the prison-courtyard map on the hero's height and the Headsman's Hill half on its y), then its markers
// Retail 0x0082BE60, 511 bytes, `ret 0x10`.
// Callees: CPlayerManager::GetMainPlayer 0x00449970 (label "Catch" is wrong), GetHeroCreature 0x00487DC0, __fastcall
//   PeekGuiDef 0x00438570, the hero's position getter 0x004C73D0 (label "CVertexBufferWin32::DoSizeof" is wrong),
//   CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, the gui def's minimap-name map find
//   0x0042B467 (node pointer; value at +0x14), __fastcall ToScreenBox(const C2DBoxF&) 0x00643BEF (a GFToScreen
//   wrapper; label "operator&&<...>" is wrong), CEnginePrimitive2DSprite ctor 0x0041BEB0, CIEngine slot 23 (+0x5C)
//   SetPersistentPrimitive, DrawMarkers 0x0082B7E0.
// Constants: 51.0f 0x01272580 (courtyard height), 4740.0f 0x01272584 (hill split). Strings 0x01253D24 / 0x01253D5C.
// Layout (retail; the PDB-generated engine/CMiniMapDisplay.h has the 16-byte debug vectors and lists):
//   MapPrimitiveHandles +0x04 (3-word vector), MarkerPrimitiveHandles +0x10, MiniMapGraphic +0x1C, Markers +0x20
//   (list head), MarkerFlashFade +0x24 (0x1c), the four map flags +0x40..+0x43. CEnginePrimitivePrepareInfo Engine
//   +0x34 (PDB). CPlayerGuiDef minimap map +0x94C. Hero flag byte +0x91 bit 0. Main game component 0x013B86A0
//   (PPlayerManager +0x1C).
// Shape: the layer is an initialised local overwritten under `!front`; the courtyard names are named CCharStrings in
//   their own blocks (one frame slot); C2DBoxF needs its user copy constructor, else the ToScreenBox result is copied
//   into a second frame temporary and the call is hoisted ahead of the sprite constructor's argument pushes.
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length);    // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
};
struct C2DVector {
    float x, y;
    C2DVector(float ax, float ay) : x(ax), y(ay) { }
};
struct C3DVector { float X, Y, Z; };
struct C2DBoxF {
    C2DVector TopLeft, BottomRight;
    C2DBoxF(const C2DBoxF& other) : TopLeft(other.TopLeft), BottomRight(other.BottomRight) { }
};
C2DBoxF __fastcall ToScreenBox(const C2DBoxF& box);   // 0x00643BEF
struct CEnginePrimitive2DSprite {                  // 0x54 bytes
    unsigned char _data[0x54];
    CEnginePrimitive2DSprite(unsigned __int64 id, const C2DBoxF& box, long layer, unsigned long graphic,
        unsigned long frame, long blend, long interpolation, float a, float b, unsigned char alpha,
        unsigned char red, unsigned char green, unsigned char blue, unsigned long flags,
        const C2DVector& uv0 = C2DVector(0.0f, 0.0f), const C2DVector& uv1 = C2DVector(0.0f, 0.0f));   // 0x0041BEB0
};
struct CEnginePrimitiveHandle;
struct CIEngine {
    virtual void e00(); virtual void e01(); virtual void e02(); virtual void e03(); virtual void e04(); virtual void e05(); virtual void e06(); virtual void e07();
    virtual void e08(); virtual void e09(); virtual void e10(); virtual void e11(); virtual void e12(); virtual void e13(); virtual void e14(); virtual void e15();
    virtual void e16(); virtual void e17(); virtual void e18(); virtual void e19(); virtual void e20(); virtual void e21(); virtual void e22();
    virtual void SetPersistentPrimitive(CEnginePrimitiveHandle& handle, const CEnginePrimitive2DSprite& primitive, unsigned long flags);   // slot 23 (+0x5C)
};
struct CEnginePrimitivePrepareInfo {
    unsigned char _pad00[0x34];
    CIEngine*     Engine;                          // +0x34
};
struct CHero {
    unsigned char _pad00[0x91];
    unsigned char Flags91;                         // +0x91
    const C3DVector& GetPosition() const;          // 0x004C73D0
};
struct CPlayer { CHero* GetHeroCreature(); };      // 0x00487DC0
struct CPlayerManager { CPlayer* GetMainPlayer(); };   // 0x00449970
struct CMainGameComponent {
    unsigned char   _pad00[0x1c];
    CPlayerManager* PPlayerManager;                // +0x1C
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0
struct CMinimapNode {                              // map<CCharString, long> node
    unsigned char _pad00[0x10];
    CCharString   Name;                            // +0x10
    long          Graphic;                         // +0x14
};
struct CMinimapMap { CMinimapNode* Find(const CCharString& name); };   // 0x0042B467
struct CPlayerGuiDef {
    unsigned char _pad00[0x94c];
    CMinimapMap   Minimaps;                        // +0x94C
};
CPlayerGuiDef* __fastcall PeekGuiDef();            // 0x00438570

struct CMiniMapDisplay {
    void*                   __vftable;
    CEnginePrimitiveHandle* MapPrimitiveHandles[3];       // +0x04 vector (start, finish, end)
    CEnginePrimitiveHandle* MarkerPrimitiveHandles[3];    // +0x10
    long                    MiniMapGraphic;               // +0x1C
    void*                   Markers;                      // +0x20
    unsigned char           MarkerFlashFade[0x1c];        // +0x24
    bool                    MiniMapIsPrisonCourtyard1;    // +0x40
    bool                    MiniMapIsPrisonCourtyard2;    // +0x41
    bool                    MiniMapIsHeadsmansHillNorth;  // +0x42
    bool                    MiniMapIsHeadsmansHillSouth;  // +0x43
    void DrawMarkers(CEnginePrimitivePrepareInfo& info, const C2DBoxF& box, unsigned char alpha, bool front);   // 0x0082B7E0
    void DrawMap(CEnginePrimitivePrepareInfo& info, const C2DBoxF& box, unsigned char alpha, bool front);
};

void CMiniMapDisplay::DrawMap(CEnginePrimitivePrepareInfo& info, const C2DBoxF& box, unsigned char alpha, bool front)
{
    CHero* hero = g_pMainGameComponent->PPlayerManager->GetMainPlayer()->GetHeroCreature();
    if (!hero || (hero->Flags91 & 1) || MiniMapGraphic <= 0)
        return;
    long layer = 0x44;
    if (!front)
        layer = 0x47;
    CPlayerGuiDef* def = PeekGuiDef();
    if (MiniMapIsPrisonCourtyard1 && hero->GetPosition().Z > 51.0f) {
        CMinimapNode* node;
        {
            CCharString name("MINIMAP_PRISONCOURTYARD2", -1);
            node = def->Minimaps.Find(name);
        }
        MiniMapGraphic = node->Graphic;
        MiniMapIsPrisonCourtyard1 = false;
        MiniMapIsPrisonCourtyard2 = true;
    } else if (MiniMapIsPrisonCourtyard2 && hero->GetPosition().Z <= 51.0f) {
        CMinimapNode* node;
        {
            CCharString name("MINIMAP_PRISONCOURTYARD1", -1);
            node = def->Minimaps.Find(name);
        }
        MiniMapGraphic = node->Graphic;
        MiniMapIsPrisonCourtyard1 = true;
        MiniMapIsPrisonCourtyard2 = false;
    } else if (MiniMapIsHeadsmansHillNorth && hero->GetPosition().Y <= 4740.0f) {
        MiniMapIsHeadsmansHillNorth = false;
        MiniMapIsHeadsmansHillSouth = true;
    } else if (MiniMapIsHeadsmansHillSouth && hero->GetPosition().Y > 4740.0f) {
        MiniMapIsHeadsmansHillNorth = true;
        MiniMapIsHeadsmansHillSouth = false;
    }
    CEnginePrimitive2DSprite sprite(0, ToScreenBox(box), layer, MiniMapGraphic, 0, 2, 0, 0.0f, 0.0f, alpha,
        0x80, 0x80, 0x80, 2);
    info.Engine->SetPersistentPrimitive(*MapPrimitiveHandles[0], sprite, 0xc0);
    DrawMarkers(info, box, alpha, front);
}