// CPlayerGui::UpdateMoney() -- refresh the money stat bar from the hero's gold: when the gold differs from the bar's current value, queue a "+N"/"-N" change text (the drop also plays the GOLD_PAYMENT sound), then animate an initialised bar towards the gold or snap an uninitialised one to it
// Retail-asm: 0x00439C3D `fld dword ptr [esp + 0x1c]` ; 0x00439C41 `fistp dword ptr [esp + 0x18]` -- the engine's inline-assembly float-to-long helper (rasm1 family (a)): the value is stored to the helper's by-value parameter slot and reloaded for a bare dword fistp into a frame word; VC7.1 C++ `(long)x` calls __ftol2, /QIfist emits `fistp qword` and /arch:SSE2 cvttss2si (rasm1 probe family (a)). Everything around it is C++.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00439BB0; reconstructed from the parked reference body (rebuild/candidates/manual/pending), the C++ rounding stand-in replaced by the inline-assembly helper. CurrentValue rounded by the helper; the steps conversion stays a __ftol2 cast.
// the engine's inline-assembly float-to-long helper (VC7.1 expands the __forceinline body and its __asm in place)
__forceinline long FloatToLong(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}
#include <math.h>
#include <stdlib.h>
#pragma intrinsic(abs)
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct CWideString {
    void* PStringData;
    CWideString();                                     // 0x0099AED0
    ~CWideString();                                    // 0x0099B510
    const CWideString& operator=(const wchar_t* text); // 0x0099B800
};
CWideString __fastcall operator+(const CWideString& a, const CWideString& b);   // 0x0099BE70
CWideString __fastcall IntToWideString(long value);    // 0x00405F00
struct CRGBColour { unsigned long Value; };
struct CPlayerGuiDef {
    unsigned char _pad000[0x4c4];
    CRGBColour    MoneyGainColour;                     // +0x4c4 (descriptive)
    CRGBColour    MoneyLossColour;                     // +0x4c8 (descriptive)
    unsigned char _pad4cc[0x4e8 - 0x4cc];
    float         StatChangeTextSpeed;                 // +0x4e8 (descriptive)
    float         StatChangeTextTime;                  // +0x4ec (descriptive)
};
extern CPlayerGuiDef* __fastcall PeekGuiDef();         // 0x00438570

struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);              // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                    // +0x20
    CInterfaceMap m_interfaces;                        // +0x44
    unsigned char _pad4c[0x45];
    unsigned char m_statusFlags;                       // +0x91 (bit 0: dead)
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
struct CTCHeroStats {                                  // interface type 4
    unsigned char _pad[0x3c];
    long          Gold;                                // +0x3c
};
struct CTCSoundPlayer {                                // interface type 0x10
    void PlaySound(const CCharString& name, bool a, bool b, bool c, long d);   // 0x0066BD00
};
struct CPlayer {
    CThing* GetPlayerCharacter();                      // 0x00487B70
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                          // 0x00449970
};
namespace NPlayerGui {
struct CStatBar {
    void*         __vftable;
    CPlayerGuiDef* GuiDef;                             // +0x04
    bool          ValueInitialised;                    // +0x08
    float         CurrentValue;                        // +0x0c
    float         OldValue;                            // +0x10
    float         LastOldValue;                        // +0x14
    unsigned char _pad18[0x28 - 0x18];
    long          GuiMode;                             // +0x28
    unsigned char _pad2c[0x48 - 0x2c];
    unsigned long GraphicFrame;                        // +0x48
    unsigned long OldGraphicFrame;                     // +0x4c
    void UpdateChangeTexts();                          // 0x00646B12
    void AddChangeText(const CRGBColour& colour, const CWideString& text, long change, bool flag);   // 0x0064DAE0
};
}
struct CPlayerGui {
    unsigned char          _pad000[8];
    CPlayerManager*        PlayerManager;              // +0x08
    unsigned char          _pad00c[0x2cc - 0xc];
    NPlayerGui::CStatBar*  PMoney;                     // +0x2cc
    void UpdateStat(float value, float rate, float steps, float speed, float time, NPlayerGui::CStatBar* bar);   // 0x00437AE0
    void UpdateMoney();
};

void CPlayerGui::UpdateMoney()
{
    PMoney->UpdateChangeTexts();
    CThing* hero = PlayerManager->GetMainPlayer()->GetPlayerCharacter();
    if (!hero || (hero->m_statusFlags & 1))
        return;
    CTCHeroStats* stats;
    hero->GetInterface(&stats, 4);
    long gold = stats->Gold;
    if (PMoney->ValueInitialised) {
        long current = FloatToLong(PMoney->CurrentValue);
        if (gold != current) {
            CWideString sign;
            CPlayerGuiDef* def = PeekGuiDef();
            CRGBColour colour;
            if (gold > current) {
                colour = def->MoneyGainColour;
                sign = L"+";
            } else {
                colour = def->MoneyLossColour;
                sign = L"-";
                CTCSoundPlayer* sound;
                if (hero->GetInterface(&sound, 0x10))
                    sound->PlaySound(CCharString("GOLD_PAYMENT"), true, false, false, -1);
            }
            long change = gold - current;
            CWideString text = sign + IntToWideString(abs(change));
            PMoney->AddChangeText(colour, text, change, true);
        }
        long steps = (long)((fabs(PMoney->OldValue - PMoney->CurrentValue) - 0.2f) * 0.5f);
        if (steps < 1)
            steps = 1;
        UpdateStat((float)gold, 0.01f, (float)steps, PeekGuiDef()->StatChangeTextSpeed, PeekGuiDef()->StatChangeTextTime, PMoney);
        PMoney->OldGraphicFrame = PMoney->GraphicFrame;
        ++PMoney->GraphicFrame;
    } else {
        float value = (float)gold;
        PMoney->CurrentValue = value;
        PMoney->OldValue = value;
        PMoney->LastOldValue = value;
        PMoney->ValueInitialised = true;
    }
}
