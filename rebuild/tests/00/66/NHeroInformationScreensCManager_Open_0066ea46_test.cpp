#include <cstdio>
#include "engine/CFadeInFadeOutBase.h"
class CRGBColour {                                      // Ego_r.pdb: 4 bytes, one anonymous union (CRGBColour_u_0)
public:
    union {
        struct {
            unsigned char B;                             // +0
            unsigned char G;                             // +1
            unsigned char R;                             // +2
            unsigned char A;                             // +3
        };
        unsigned long Colour;
    };
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(const CRGBColour& c) : Colour(c.Colour) {}
};
struct CFadeInFadeOutBase_Methods : CFadeInFadeOutBase {
    void Open(float fade_in_time, float fade_out_time, CRGBColour to_colour);   // 0x00434840
};
class CWorld;
namespace NHeroInformationScreens {
class CBase;
template <class T> class CVector {                       // STLport vector, 12 bytes
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
};
class CManager {
public:
    CWorld*                       World;                 // +0x00
    CVector<const CBase*>         Screens;               // +0x04
    CVector<const CBase*>         InventoryScreens;      // +0x10
    CVector<long>                 ScrollBarIndices;      // +0x1c
    long                          CurrentScreen;         // +0x28
    long                          LastScreen;            // +0x2c
    long                          HighlightedScreen;     // +0x30
    bool                          OpenFlag;              // +0x34
    unsigned char                 _pad35[3];             // +0x35 (engine/CFadeInFadeOutBase.h is pack(1))
    CFadeInFadeOutBase_Methods    ScreenEffectFade;      // +0x38
    void Open();
private:
    void GetAllRegisteredInventoryScreens(CVector<const CBase*>& screens) const;   // 0x0066E9E9
    friend struct CManager_Test;                         // behaviour-test hook
};
void CManager::Open()
{
    if (!OpenFlag)
        ScreenEffectFade.Open(1.0f, 0.0f, CRGBColour(0, 0, 0, 255));
    OpenFlag = true;
    GetAllRegisteredInventoryScreens(InventoryScreens);
}
}

static int g_opens = 0, g_collects = 0; static float g_in = -1, g_out = -1; static unsigned long g_col = 0;
void CFadeInFadeOutBase_Methods::Open(float fade_in_time, float fade_out_time, CRGBColour to_colour)
{ ++g_opens; g_in = fade_in_time; g_out = fade_out_time; g_col = to_colour.Colour; }
namespace NHeroInformationScreens {
void CManager::GetAllRegisteredInventoryScreens(CVector<const CBase*>& screens) const { if (&screens == &InventoryScreens) ++g_collects; }
}
int main()
{
    static NHeroInformationScreens::CManager m;
    m.OpenFlag = false;
    m.Open();
    bool ok = g_opens == 1 && g_in == 1.0f && g_out == 0.0f && g_col == 0xff000000UL && m.OpenFlag && g_collects == 1;
    m.Open();
    ok = ok && g_opens == 1 && g_collects == 2;
    if (ok) std::printf("0066ea46_TEST PASS\n");
    return 0;
}