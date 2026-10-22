// Grade: DIFFER(556v541) -- in the 3 marker loops retail reads the hero marker graphic as mov eax,[eax+958h] where ours adds 958h first (add eax,958h ; mov eax,[eax]); the 15 extra bytes then shift loop alignment. A direct *m_pBegin deref gives retail's load but re-enregisters info into ebx. Best probe/b7e/d.cpp.
// CMiniMapDisplay::DrawMarkers(const CEnginePrimitivePrepareInfo&, const C2DBoxF&, unsigned char, bool)
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour() {}
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
struct CFadeInFadeOutBase {
    bool       Active;                             // +0x00
    float      FadeInTime;                         // +0x04
    float      FadeOutTime;                        // +0x08
    bool       Closing;                            // +0x0c
    bool       Opening;                            // +0x0d
    float      OpenTimer;                          // +0x10
    float      CloseTimer;                         // +0x14
    CRGBColour ToColour;                           // +0x18
    void Open(float fade_in_time, float fade_out_time, CRGBColour to_colour)
    {
        Active = true;
        FadeInTime = fade_in_time;
        FadeOutTime = fade_out_time;
        Closing = false;
        Opening = true;
        OpenTimer = 0.0f;
        CloseTimer = fade_out_time;
        ToColour = to_colour;
    }
    void Update(float timeDelta);                  // 0x00434870
    bool IsFullyOpen() { return Active && !Opening && !Closing; }
    void FadeOut()
    {
        Opening = false;
        Closing = true;
    }
};
struct CEnginePrimitivePrepareInfo {
    float TimeDelta;                               // +0x00 (CInterpolationInfo Interpolate, first word)
};
struct C2DBoxF;
struct CEnginePrimitiveHandle;
struct CPrimitiveHandleVector {                    // STLport vector<CEnginePrimitiveHandle>, 12 bytes
    CEnginePrimitiveHandle* _M_start;
    CEnginePrimitiveHandle* _M_finish;
    CEnginePrimitiveHandle* _M_end_of_storage;
    CEnginePrimitiveHandle* begin() { return _M_start; }
    CEnginePrimitiveHandle* end() { return _M_finish; }
    CEnginePrimitiveHandle* erase(CEnginePrimitiveHandle* first, CEnginePrimitiveHandle* last);   // 0x00440D70
    void reserve(unsigned int n);                  // 0x00443F90
    void clear() { erase(begin(), end()); }
};
struct CThing {
    unsigned char _pad00[0x91];
    unsigned char m_dead : 1;                      // +0x91 bit 0
    unsigned char m_91_1 : 7;
};
struct CPlayer {
    CThing* GetHeroThing();                        // 0x00487DC0
};
struct CPlayerManager {
    CPlayer* GetPlayer();                          // 0x00449970
};
struct CWorld {
    unsigned char _pad00[0xa4];
    bool          MiniMapPaused;                   // +0xa4 (name unverified)
    unsigned char _pada5[0x104 - 0xa5];
    long          RegionLoadStage;                 // +0x104
};
struct CGameComponentState {
    unsigned char _pad00[0xbc];
    bool          Busy;                            // +0xbc (name unverified)
};
struct CGame {
    unsigned char        _pad00[0x1c];
    CPlayerManager*      m_pPlayerManager;         // +0x1c
    unsigned char        _pad20[4];
    CWorld*              m_pWorld;                 // +0x24
    CGameComponentState* m_pState;                 // +0x28
};
extern CGame* g_pGame;                             // 0x013B86A0
struct CArray_long {
    long* m_pBegin;
    long* m_pEnd;
    long& front() { return *m_pBegin; }
};
struct CPlayerGuiDef {
    unsigned char _pad00[0x958];
    CArray_long   m_markerGraphics;                // +0x958
};
CPlayerGuiDef* __fastcall PeekGuiDef();            // 0x00438570
struct CRegion;
struct CMiniMapDisplay {
    struct CMarkerInfo {
        unsigned char _pad00[8];
        long          Graphic;                     // +0x08
        bool          Flash;                       // +0x0c (name unverified)
    };
    struct CMarkerNode {
        CMarkerNode* _M_next;
        CMarkerNode* _M_prev;
        CMarkerInfo  _M_data;
    };
    virtual void v00();
    CPrimitiveHandleVector MapPrimitiveHandles;    // +0x04
    CPrimitiveHandleVector MarkerPrimitiveHandles; // +0x10
    long                   MiniMapGraphic;         // +0x1c
    CMarkerNode*           Markers;                // +0x20 (STLport list node)
    CFadeInFadeOutBase     MarkerFlashFade;        // +0x24
    unsigned char          _pad40[0xb8 - 0x40];
    CRegion*               PRegion;                // +0xb8
    unsigned int MarkerCount()
    {
        unsigned int n = 0;
        for (CMarkerNode* node = Markers->_M_next; node != Markers; node = node->_M_next)
            ++n;
        return n;
    }
    void AddMarker(const CThing* thing, long graphicIndex);   // 0x0082A500
    bool DrawMarker(const CEnginePrimitivePrepareInfo& info, const CMarkerInfo& marker, long index, const C2DBoxF& box,
                    unsigned char alpha, bool flag);         // 0x0082A570
    void DrawMarkers(const CEnginePrimitivePrepareInfo& info, const C2DBoxF& box, unsigned char alpha, bool flag);
};

void CMiniMapDisplay::DrawMarkers(const CEnginePrimitivePrepareInfo& info, const C2DBoxF& box, unsigned char alpha, bool flag)
{
    MarkerPrimitiveHandles.clear();
    MarkerPrimitiveHandles.reserve(MarkerCount());
    CWorld* world = g_pGame->m_pWorld;
    if ((world->RegionLoadStage == 0 || world->RegionLoadStage == 9) && !g_pGame->m_pState->Busy) {
        if (!world->MiniMapPaused) {
            if (!MarkerFlashFade.Active)
                MarkerFlashFade.Open(0.5f, 0.5f, CRGBColour(0, 0, 0, 255));
            MarkerFlashFade.Update(info.TimeDelta);
            if (MarkerFlashFade.IsFullyOpen())
                MarkerFlashFade.FadeOut();
        }
        long index = 0;
        bool heroDrawn = false;
        CMarkerNode* node;
        for (node = Markers->_M_next; node != Markers; node = node->_M_next) {
            const CMarkerInfo& marker = node->_M_data;
            if (marker.Graphic == PeekGuiDef()->m_markerGraphics.front()) {
                DrawMarker(info, marker, index, box, alpha, flag);
                ++index;
                heroDrawn = true;
            }
        }
        for (node = Markers->_M_next; node != Markers; node = node->_M_next) {
            const CMarkerInfo& marker = node->_M_data;
            if (marker.Graphic != PeekGuiDef()->m_markerGraphics.front() && !marker.Flash) {
                DrawMarker(info, marker, index, box, alpha, flag);
                ++index;
            }
        }
        for (node = Markers->_M_next; node != Markers; node = node->_M_next) {
            const CMarkerInfo& marker = node->_M_data;
            if (marker.Graphic != PeekGuiDef()->m_markerGraphics.front() && marker.Flash) {
                DrawMarker(info, marker, index, box, alpha, flag);
                ++index;
            }
        }
        if (!heroDrawn && PRegion) {
            CThing* hero = g_pGame->m_pPlayerManager->GetPlayer()->GetHeroThing();
            if (hero && !hero->m_dead)
                AddMarker(hero, PeekGuiDef()->m_markerGraphics.front());
        }
    }
}
