// Grade: DIFFER(837v865) -- retail frame 0x68 (separate slots for the debug-name info and each text primitive) vs ours 0x54; retail keeps &info in ebx and pushes ebp/edi only after the four guards, ours holds a zero register; colour bytes stored before the subsystem call; same non-virtual TC (0x3d) eax/mov ecx,eax residue as 00663c40
// CThingPhysical::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&) -- for a drawable thing (slot
// 27 true, slot 50 false) whose CThing base pass succeeds and when the prepare info is not a shadow pass: within 48
// units of the camera, draw the thing's debug name (slots 33/34) in green above its static focal point; let a creature
// generator TC (0x3d) draw its debug info; and under the global debug switch draw the thing's state letters
// ("M ", "g", "m", "c", "i", "r" from slot 35) above its helper point
// Leaf: ?DrawPreparePrimitivesForRendering@CThingPhysical@@UAE_NABVCEnginePrimitivePrepareInfo@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006A64D0, `ret 4`.
// Callees: CThing::DrawPreparePrimitivesForRendering 0x004C7540, camera getter 0x004C7AA0 (slot 8 = position by value),
//   CThing::GetPosition 0x004C73D0, GFWorld3DDistanceSmallerThan 0x006614F0 (__fastcall), debug-name info ctor
//   0x006A4B30, CTCGraphicAppearance::GetStaticFocalPosIfLoaded 0x004C11F0 / helper point 0x004C1060, CCharString copy
//   0x0099EC30 / default 0x0099E4B0 / operator+= 0x0099F100 / ToWideString 0x0099E480 / dtor 0x0099EAE0, game subsystem
//   getter 0x004C7A70 (+0x1614c debug font, +0x16144 font whose +4 is the line height), CEnginePrimitiveText 0x004C3120,
//   ~CWideString 0x0099B510, LowerBound 0x0040F020, CTCCreatureGenerator::DrawDebugInfo 0x0074BC20.
// Layout: retail CThing UID +0x18, interface bitset +0x20 / map +0x44, graphic TC +0x64; prepare info shadow-pass byte
//   +0x38 and renderer +0x34 (slot 20 adds a primitive). Globals: debug switch byte 0x013B83C5; 0.2f 0x012305A0, 1.5f
//   0x0125AADC. Text primitive as the landed CTCEmoteIcon::DrawEmoteTypeText 0x0071AB20.
struct C3DVector { float X, Y, Z; };
struct CRGBColour {
    unsigned char B, G, R, A;
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { Set(r, g, b, a); }
};
struct CWideString {
    void* PStringData;
    ~CWideString();                                     // 0x0099B510
};
struct CCharString {
    void* PStringData;
    CCharString();                                      // 0x0099E4B0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator+=(const char* text);          // 0x0099F100
    CWideString ToWideString() const;                   // 0x0099E480
};
struct CFontBank;
struct CFontMetrics { long _pad00; long LineHeight; };  // +0x04
struct CEnginePrimitiveText {                           // 0x2c bytes
    unsigned char _base[0x20];
    CWideString   Text;                                 // +0x20
    CFontBank*    PFont;                                // +0x24
    bool          ScaleIn3D;                            // +0x28
    CEnginePrimitiveText(unsigned __int64 handle, const C3DVector& pos, float scale, const CWideString& text,
                         const CFontBank& font, const CRGBColour& colour, bool scaleIn3D, long interpolationSet);   // 0x004C3120
};
struct CEnginePrimitiveRenderer {
#define VF(n) virtual void v##n();
    VF(00) VF(01) VF(02) VF(03) VF(04) VF(05) VF(06) VF(07) VF(08) VF(09)
    VF(10) VF(11) VF(12) VF(13) VF(14) VF(15) VF(16) VF(17) VF(18) VF(19)
    virtual void AddPrimitive(const CEnginePrimitiveText& primitive);   // slot 20 (+0x50)
};
struct CEnginePrimitivePrepareInfo {
    unsigned char _pad00[0x34];
    CEnginePrimitiveRenderer* PRenderer;                // +0x34
    bool          ShadowPass;                           // +0x38
};
struct CCamera {
    VF(00) VF(01) VF(02) VF(03) VF(04) VF(05) VF(06) VF(07)
    virtual C3DVector GetPosition() const;              // slot 8 (+0x20)
};
struct CTCGraphicAppearance {
    bool GetStaticFocalPosIfLoaded(C3DVector* pos);     // 0x004C11F0
    bool GetHelperPointOr(C3DVector* pos, float scale); // 0x004C1060
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CTCCreatureGenerator { void DrawDebugInfo(const CEnginePrimitivePrepareInfo& info); };   // 0x0074BC20
struct CMainGameComponent {
    unsigned char _pad00[0x16144];
    CFontMetrics* Font;                                 // +0x16144
    unsigned char _pad16148[0x1614c - 0x16148];
    CFontBank*    DebugFont;                            // +0x1614c
};
struct CDebugNameInfo {                                 // built by 0x006A4B30, filled by slot 34
    CCharString   Name;                                 // +0x00
    unsigned char _pad04[0x11 - 0x04];
    bool          Show;                                 // +0x11
    unsigned char _pad12[0x14 - 0x12];
    CDebugNameInfo();                                   // 0x006A4B30
};
struct CDebugStateFlags {                               // filled by slot 35
    bool          Mounted;                              // +0x00 "m"
    bool          Guard;                                // +0x01 "g"
    unsigned char _pad02[0x08 - 0x02];
    bool          Moving;                               // +0x08 "M "
    bool          Combat;                               // +0x09 "c"
    bool          Interacting;                          // +0x0a "i"
    bool          Running;                              // +0x0b "r"
};
bool __fastcall GFWorld3DDistanceSmallerThan(const C3DVector& a, const C3DVector& b, float distance);   // 0x006614F0
extern bool g_DrawThingDebugState;                      // 0x013B83C5

class CBaseObjectThing {                                // the vtable and the words before the 64-bit UID (a base
public:                                                 // class, so the UID stays at +0x18 under 8-byte alignment)
    VF(00) VF(01) VF(02) VF(03) VF(04) VF(05) VF(06) VF(07) VF(08) VF(09)
    VF(10) VF(11) VF(12) VF(13) VF(14) VF(15) VF(16) VF(17) VF(18) VF(19)
    VF(20) VF(21) VF(22) VF(23) VF(24) VF(25) VF(26)
    virtual bool IsDrawable();                          // slot 27 (+0x6c)
    VF(28) VF(29) VF(30) VF(31) VF(32)
    virtual bool ShouldDrawDebugName();                 // slot 33 (+0x84)
    virtual void GetDebugNameInfo(CDebugNameInfo* info);   // slot 34 (+0x88)
    virtual bool GetDebugStateFlags(CDebugStateFlags* flags);   // slot 35 (+0x8c)
    VF(36) VF(37) VF(38) VF(39) VF(40) VF(41) VF(42) VF(43) VF(44) VF(45) VF(46) VF(47) VF(48) VF(49)
    virtual bool IsHidden();                            // slot 50 (+0xc8)
    unsigned char _pad04[0x18 - 0x04];
};
class CThing : public CBaseObjectThing {
public:
    unsigned __int64 UID;                               // +0x18
    unsigned long UsingComponents[9];                   // +0x20
    CInterfaceMap Components;                           // +0x44
    unsigned char _pad4c[0x64 - 0x4c];
    CTCGraphicAppearance* GraphicTC;                    // +0x64
    __forceinline bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (it == map->last || type < it->type ? map->last : it)->component;
    }
    const C3DVector& GetPosition() const;               // 0x004C73D0
    const CCamera* GetCamera() const;                   // 0x004C7AA0
    CMainGameComponent* GetGameSubsystem() const;       // 0x004C7A70
    bool DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);   // 0x004C7540
};
class CThingPhysical : public CThing {
public:
    bool DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);
};

bool CThingPhysical::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info)
{
    if (IsDrawable() && !IsHidden() && CThing::DrawPreparePrimitivesForRendering(info) && !info.ShadowPass) {
        C3DVector focal;
        if (ShouldDrawDebugName()) {
            const CCamera* camera = GetCamera();
            if (GFWorld3DDistanceSmallerThan(camera->GetPosition(), GetPosition(), 48.0f)) {
                CDebugNameInfo nameInfo;
                if (GraphicTC->GetStaticFocalPosIfLoaded(&focal)) {
                    GetDebugNameInfo(&nameInfo);
                    if (nameInfo.Show) {
                        CCharString text(nameInfo.Name);
                        CRGBColour colour(0, 255, 0, 255);
                        info.PRenderer->AddPrimitive(CEnginePrimitiveText(UID, focal, 1.0f, text.ToWideString(),
                            *GetGameSubsystem()->DebugFont, colour, false, 0));
                        focal.Z += GetGameSubsystem()->Font->LineHeight * 0.2f;
                    }
                }
            }
        }
        if (HasTC(0x3d))
            ((CTCCreatureGenerator*)GetTC(0x3d))->DrawDebugInfo(info);
        if (g_DrawThingDebugState) {
            CDebugStateFlags flags;
            if (GetDebugStateFlags(&flags)) {
                GraphicTC->GetHelperPointOr(&focal, 1.0f);
                focal.Z += 1.5f;
                CCharString text;
                text += flags.Moving ? "M " : "";
                text += flags.Guard ? "g" : "";
                text += flags.Mounted ? "m" : "";
                text += flags.Combat ? "c" : "";
                text += flags.Interacting ? "i" : "";
                text += flags.Running ? "r" : "";
                CRGBColour colour(0, 255, 0, 255);
                info.PRenderer->AddPrimitive(CEnginePrimitiveText(UID, focal, 1.0f, text.ToWideString(),
                    *GetGameSubsystem()->DebugFont, colour, false, 0));
                focal.Z += GetGameSubsystem()->Font->LineHeight * 0.2f;
            }
        }
        return true;
    }
    return false;
}
