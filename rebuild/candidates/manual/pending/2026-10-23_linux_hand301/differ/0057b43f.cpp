// Grade: DIFFER(830v836) -- size-optimised TU (#pragma optimize s); cut-scene / TC 0x16 handle clears, drunk gate, box, wobble update/wrap and sprite args match in shape; retail spills the first UV block (cos(V) as float, cos/sin of the rotation as doubles) while ours keeps them on the FP stack, the handle Clear reloads Data after OnLastHandle, and the IsInCutScene vcall loads straight into ecx
// CTCHeroStats::RenderHeroDrunkenness(CDisplayEngine*) -- drops the drunken overlay sprite during cut-scenes or when the hero carries TC 0x16 with its flag set; otherwise, while drunk (or for one frame after sobering), scrolls/rotates the UV wobble counters and re-adds a full-screen sprite whose alpha scales with Drunkenness/GetMaxDrunkenness
#pragma optimize("s", on)
#include <math.h>
struct C2DVector { float X, Y; C2DVector(float x, float y) : X(x), Y(y) {} };
struct C2DBoxF { float X0, Y0, X1, Y1; C2DBoxF(float x0, float y0, float x1, float y1) : X0(x0), Y0(y0), X1(x1), Y1(y1) {} };
float GFAXToVX(float x);                                // 0x009E1E10
float GFAYToVY(float y);                                // 0x009E1E40
extern double VIRTUAL_SCREEN_WIDTH;                     // 0x013961E8
extern double VIRTUAL_SCREEN_HEIGHT;                    // 0x013961F0
class CEnginePrimitive2DSprite {
public:
    unsigned char Data[0x54];
    CEnginePrimitive2DSprite(long a0, long a1, const C2DBoxF& box, long layer, unsigned long bank, unsigned long frame,
        long blend, long clipIsPos, float startAngle, float endAngle, long alpha, long r, long g, long b, long flags,
        const C2DVector& uvStart, const C2DVector& uvEnd);   // 0x0041BEB0
};
class CEnginePrimitiveHandle;
class CEnginePersistentPrimitiveBase {
public:
    virtual void v00();
    virtual void DeleteThis();
    virtual void v02();
    virtual void OnLastHandle(CEnginePrimitiveHandle* handle);
    long RefCount;
    __forceinline void DecRef() { if (--RefCount == 0) DeleteThis(); }
};
class CEnginePrimitiveHandle {
public:
    long _pad0;
    CEnginePersistentPrimitiveBase* Data;               // +0x04
    __forceinline void Clear()
    {
        if ((Data ? Data->RefCount : 0) == 1)
            Data->OnLastHandle(this);
        if (Data) {
            Data->DecRef();
            Data = 0;
        }
        Data = 0;
    }
};
class CCutSceneQuery {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v00a(); virtual void v00b(); virtual void v00c(); virtual void v00d(); virtual void v00e(); virtual void v00f(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v01a(); virtual void v01b(); virtual void v01c(); virtual void v01d(); virtual void v01e(); virtual void v01f(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v02a(); virtual void v02b(); virtual void v02c(); virtual void v02d(); virtual void v02e(); virtual void v02f(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v03a(); virtual void v03b(); virtual void v03c(); virtual void v03d(); virtual void v03e(); virtual void v03f(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v04a(); virtual void v04b(); virtual void v04c(); virtual void v04d(); virtual void v04e(); virtual void v04f(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v05a(); virtual void v05b(); virtual void v05c(); virtual void v05d(); virtual void v05e(); virtual void v05f(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v06a(); virtual void v06b(); virtual void v06c(); virtual void v06d(); virtual void v06e(); virtual void v06f(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v07a(); virtual void v07b(); virtual void v07c(); virtual void v07d(); virtual void v07e(); virtual void v07f(); virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089(); virtual void v08a(); virtual void v08b(); virtual void v08c(); virtual void v08d(); virtual void v08e(); virtual void v08f(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v09a(); virtual void v09b(); virtual void v09c(); virtual void v09d(); virtual void v09e(); virtual void v09f(); virtual void v0a0(); virtual void v0a1(); virtual void v0a2(); virtual void v0a3(); virtual void v0a4(); virtual void v0a5(); virtual void v0a6(); virtual void v0a7(); virtual void v0a8(); virtual void v0a9(); virtual void v0aa(); virtual void v0ab(); virtual void v0ac(); virtual void v0ad(); virtual void v0ae(); virtual void v0af(); virtual void v0b0(); virtual void v0b1(); virtual void v0b2(); virtual void v0b3(); virtual void v0b4(); virtual void v0b5(); virtual void v0b6(); virtual void v0b7(); virtual void v0b8(); virtual void v0b9(); virtual void v0ba(); virtual void v0bb(); virtual void v0bc(); virtual void v0bd(); virtual void v0be(); virtual void v0bf(); virtual void v0c0(); virtual void v0c1(); virtual void v0c2(); virtual void v0c3(); virtual void v0c4(); virtual void v0c5(); virtual void v0c6(); virtual void v0c7(); virtual void v0c8(); virtual void v0c9(); virtual void v0ca(); virtual void v0cb(); virtual void v0cc(); virtual void v0cd(); virtual void v0ce(); virtual void v0cf(); virtual void v0d0(); virtual void v0d1(); virtual void v0d2(); virtual void v0d3(); virtual void v0d4(); virtual void v0d5(); virtual void v0d6(); virtual void v0d7(); virtual void v0d8(); virtual void v0d9(); virtual void v0da(); virtual void v0db(); virtual void v0dc(); virtual void v0dd(); virtual void v0de(); virtual void v0df(); virtual void v0e0(); virtual void v0e1(); virtual void v0e2(); virtual void v0e3(); virtual void v0e4(); virtual void v0e5(); virtual void v0e6(); virtual void v0e7(); virtual void v0e8(); virtual void v0e9(); virtual void v0ea(); virtual void v0eb(); virtual void v0ec(); virtual void v0ed(); virtual void v0ee(); virtual void v0ef(); virtual void v0f0(); virtual void v0f1(); virtual void v0f2(); virtual void v0f3(); virtual void v0f4(); virtual void v0f5(); virtual void v0f6(); virtual void v0f7(); virtual void v0f8(); virtual void v0f9(); virtual void v0fa(); virtual void v0fb(); virtual void v0fc(); virtual void v0fd(); virtual void v0fe(); virtual void v0ff(); virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109(); virtual void v10a(); virtual void v10b(); virtual void v10c(); virtual void v10d(); virtual void v10e(); virtual void v10f(); virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119(); virtual void v11a(); virtual void v11b(); virtual void v11c(); virtual void v11d(); virtual void v11e(); virtual void v11f(); virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129(); virtual void v12a(); virtual void v12b(); virtual void v12c(); virtual void v12d(); virtual void v12e(); virtual void v12f(); virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139(); virtual void v13a(); virtual void v13b(); virtual void v13c(); virtual void v13d(); virtual void v13e(); virtual void v13f(); virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143(); virtual void v144(); virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149(); virtual void v14a(); virtual void v14b(); virtual void v14c(); virtual void v14d(); virtual void v14e(); virtual void v14f(); virtual void v150(); virtual void v151(); virtual void v152(); virtual void v153(); virtual void v154(); virtual void v155(); virtual void v156(); virtual void v157(); virtual void v158(); virtual void v159(); virtual void v15a(); virtual void v15b(); virtual void v15c(); virtual void v15d(); virtual void v15e(); virtual void v15f(); virtual void v160(); virtual void v161(); virtual void v162(); virtual void v163(); virtual void v164(); virtual void v165(); virtual void v166(); virtual void v167(); virtual void v168(); virtual void v169(); virtual void v16a(); virtual void v16b(); virtual void v16c(); virtual void v16d(); virtual void v16e(); virtual void v16f(); virtual void v170(); virtual void v171(); virtual void v172(); virtual void v173(); virtual void v174(); virtual void v175(); virtual void v176(); virtual void v177(); virtual void v178(); virtual void v179(); virtual void v17a(); virtual void v17b(); virtual void v17c(); virtual void v17d(); virtual void v17e(); virtual void v17f(); virtual void v180(); virtual void v181(); virtual void v182(); virtual void v183(); virtual void v184(); virtual void v185(); virtual void v186(); virtual void v187(); virtual void v188(); virtual void v189(); virtual void v18a();
    virtual bool IsInCutScene();                        // +0x62C
};
class CWorldView {
public:
    unsigned char _pad00[0x38];
    CCutSceneQuery* Query;                              // +0x38
};
class CDisplayEngine {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v0a(); virtual void v0b(); virtual void v0c(); virtual void v0d(); virtual void v0e(); virtual void v0f(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16();
    virtual void AddPrimitive(CEnginePrimitiveHandle* handle, const CEnginePrimitive2DSprite& prim, unsigned long flags);   // +0x5C
};
struct CTCPair { long Type; void* TC; };
struct CTCMap {
    CTCPair* _M_start;
    CTCPair* _M_finish;
    CTCPair* LowerBound(const long& key);               // 0x0040F020
    __forceinline void* Find(long type)
    {
        long key = type;
        CTCPair* it = LowerBound(key);
        if (it == _M_finish || it->Type > type)
            it = _M_finish;
        return it->TC;
    }
};
struct CTCBitset {
    unsigned long Bits[9];
    bool test(unsigned int pos) const;                  // 0x00410DE0
};
class CThing {
public:
    virtual void v00();
    unsigned char _pad04[0x20 - 0x04];
    CTCBitset PresentBits;                              // +0x20
    CTCMap TCs;                                         // +0x44
};
struct CTCWithFlag58 { unsigned char _pad[0x58]; bool Flag; };
struct CSpecialAbilitiesDrunkennessDef {
    unsigned char _pad00[0x3c];
    long DrunkenGraphic;                                // +0x3C
    long MaxAlpha;                                      // +0x40
    float USpeed;                                       // +0x44
    float VSpeed;                                       // +0x48
    float RotSpeed;                                     // +0x4C
};
class CTCHeroStats {
public:
    virtual void v00();
    CThing* Thing;                                      // +0x04
    unsigned char _pad08[0x19c - 0x08];
    float Drunkenness;                                  // +0x19C
    unsigned char _pad1a0[0x1b8 - 0x1a0];
    bool UpdateDrunkenness;                             // +0x1B8
    const CSpecialAbilitiesDrunkennessDef* PDef;        // +0x1BC
    CEnginePrimitiveHandle PrimitiveHandle;             // +0x1C0
    float UCounter;                                     // +0x1C8
    float VCounter;                                     // +0x1CC
    float RotCounter;                                   // +0x1D0
    bool IsInCutSceneFirstTime;                         // +0x1D4
    CWorldView* GetDisplayEngine();                     // 0x00686A70
    float GetMaxDrunkenness();                          // 0x0057A89D
    void RenderHeroDrunkenness(CDisplayEngine* engine);
};

void CTCHeroStats::RenderHeroDrunkenness(CDisplayEngine* engine)
{
    if (GetDisplayEngine()->Query->IsInCutScene()) {
        if (IsInCutSceneFirstTime) {
            IsInCutSceneFirstTime = false;
            PrimitiveHandle.Clear();
        }
        return;
    }
    CThing* thing = Thing;
    if (thing->PresentBits.test(0x16) && ((CTCWithFlag58*)thing->TCs.Find(0x16))->Flag) {
        PrimitiveHandle.Clear();
        return;
    }
    float drunk = Drunkenness;
    IsInCutSceneFirstTime = true;
    if (!(drunk > 0.0f) && !UpdateDrunkenness)
        return;
    if (!PDef)
        return;
    UpdateDrunkenness = false;
    C2DBoxF box(0.0f, 0.0f, (float)GFAXToVX((float)VIRTUAL_SCREEN_WIDTH), GFAYToVY((float)VIRTUAL_SCREEN_HEIGHT));
    const double TWO_PI = 6.2831854820251465;
    float u0 = (float)cos(UCounter * TWO_PI);
    float v0 = (float)cos(VCounter * TWO_PI);
    double r0 = RotCounter * TWO_PI;
    double c0 = cos(r0), s0 = sin(r0);
    float startY = (float)(v0 * c0 - u0 * s0);
    float startX = (float)(v0 * s0 + u0 * c0);
    UCounter += PDef->USpeed;
    VCounter += PDef->VSpeed;
    RotCounter += PDef->RotSpeed;
    while (UCounter > 1.0f) UCounter -= 1.0f;
    while (VCounter > 1.0f) VCounter -= 1.0f;
    while (RotCounter > 1.0f) RotCounter -= 1.0f;
    float u = (float)cos(UCounter * TWO_PI);
    float v = (float)cos(VCounter * TWO_PI);
    double r = RotCounter * TWO_PI;
    float endY = (float)(v * cos(r) - u * sin(r));
    float endX = (float)(v * sin(r) + u * cos(r));
    float frac = drunk / GetMaxDrunkenness();
    long alpha = (long)((float)PDef->MaxAlpha * frac);
    if (alpha > 255)
        alpha = 255;
    CEnginePrimitive2DSprite sprite(0, 0, box, 0x43, PDef->DrunkenGraphic, 0, 2, 0, 0.0f, 0.0f,
        alpha, 255, 255, 255, 2, C2DVector(startX, startY), C2DVector(endX, endY));
    engine->AddPrimitive(&PrimitiveHandle, sprite, 0x440);
}
