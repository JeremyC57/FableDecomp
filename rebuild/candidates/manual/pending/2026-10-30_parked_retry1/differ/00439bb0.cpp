// Grade: DIFFER(592v592) -- the rounding is the landed FloatToLong retail_asm helper and the GOLD_PAYMENT name is a named local (built before the PlaySound pushes); residue is a one-step scratch-register rotation (eax/edx/ecx) in the UpdateStat argument loads and the three bar stores of the uninitialised branch
// Retail-asm: 0x00439C3D fld dword ptr [esp+1Ch] ; fistp dword ptr [esp+18h] -- the engine FloatToLong helper
// CPlayerGui::UpdateMoney() -- refresh the money stat bar from the hero's gold: when the gold differs from the bar's current value, queue a "+N"/"-N" change text (the drop also plays the GOLD_PAYMENT sound), then animate an initialised bar towards the gold or snap an uninitialised one to it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00439BB0, length 592 bytes, plain `ret`.
// Callees: NPlayerGui::CStatBar::UpdateChangeTexts 0x00646B12, CPlayerManager::GetMainPlayer 0x00449970,
// CPlayer::GetPlayerCharacter 0x00487B70, LowerBound 0x0040F020 (the inlined GetInterface for types 4 and 0x10),
// CWideString ctor 0x0099AED0 / operator=(const wchar_t*) 0x0099B800 / ~CWideString 0x0099B510, PeekGuiDef 0x00438570,
// CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CTCSoundPlayer::PlaySound 0x0066BD00,
// IntToWideString 0x00405F00 (__fastcall), operator+(const CWideString&, const CWideString&) 0x0099BE70 (__fastcall),
// CStatBar::AddChangeText 0x0064DAE0, __ftol2 0x00BFEA70, CPlayerGui::UpdateStat 0x00437AE0.
// Layout: retail-offset view as the landed UpdateRenown 0x00439E00 / UpdateMorality 0x00439F20: PlayerManager +0x08,
// PMoney +0x2cc; CStatBar ValueInitialised +8, CurrentValue +0xc, OldValue +0x10, LastOldValue +0x14, GuiMode +0x28,
// GraphicFrame +0x48, OldGraphicFrame +0x4c; GUI def money colours +0x4c4/+0x4c8, stat-change params +0x4e8/+0x4ec.
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

__forceinline long FloatToLong(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}

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
                if (hero->GetInterface(&sound, 0x10)) {
                    CCharString name("GOLD_PAYMENT");
                    sound->PlaySound(name, true, false, false, -1);
                }
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
