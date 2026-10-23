// CBS_Teacher::GetBookSpecificArgs(long, map<CCharString, CCharString>&) -- fill the text arguments of the teacher's comment on a returned book: books 15/17/19/21/22/23 pick their line by the game-flow position, the hero's attractiveness or scariness, the Twinblade outcome or the teddy solution ($ARG1..$ARG3)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E57020, length 1296 bytes, `ret 8`.
// Callees: IsActiveThreadTerminating 0x00F35B30, CCharString(const char*, long) 0x0099EBF0 / (const CCharString&)
//   0x0099EC30 / ~ 0x0099EAE0 / operator=(const char*) 0x0099EFE0 / operator=(const CCharString&) 0x0099EFB0,
//   operator+(const char*, const CCharString&) 0x0099F690 (`__fastcall`, hidden result), map<CCharString,
//   CCharString>::operator[] 0x009AC700, Compare 0x00411570 (the inline CCharString == const char*).
//   CGameScriptInterface slots 178 GetHeroScariness, 179 GetHeroAttractiveness.
// Layout: Ego_r.pdb CBS_Teacher (68 bytes): entity-script base {vptr, Game +4, Me +8}, ParentClass +0x14, PMasterData
//   +0x18 (CQ_SunnyvaleMasterData: PostSavePosition +4, BanditCampTwinbladeKilled +0x3a, TeddySolution +0x54).
#include <string.h>
#pragma intrinsic(strcmp)
long __fastcall Compare_00411570(const char* a, const char* b);   // 0x00411570
#pragma pack(push, 1)
struct CBasicStringChar {                       // CBasicString<char>, 13 bytes
    char*         PData;                        // +0x00
    unsigned long StringLength;                 // +0x04
    unsigned long DataLength : 31;              // +0x08
    bool          UseFastExtend : 1;            // +0x0c
    bool operator==(const char* text) const { return Compare_00411570(PData, text) ? false : true; }
};
struct CCharStringData {                        // 17 bytes
    CBasicStringChar Data;                      // +0x00
    long             NoRefs;                    // +0x0d
};
#pragma pack(pop)
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
    CCharString& operator=(const char* text);          // 0x0099EFE0
    CCharString& operator=(const CCharString& other);  // 0x0099EFB0
    __forceinline bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690
struct CTextArgMap {                                   // std::map<CCharString, CCharString>
    CCharString& operator[](const CCharString& key);   // 0x009AC700
};
#define VPAD1(a) virtual void a() const;
#define VPAD2(a) VPAD1(a##0) VPAD1(a##1)
#define VPAD4(a) VPAD2(a##0) VPAD2(a##1)
#define VPAD8(a) VPAD4(a##0) VPAD4(a##1)
#define VPAD16(a) VPAD8(a##0) VPAD8(a##1)
#define VPAD32(a) VPAD16(a##0) VPAD16(a##1)
#define VPAD64(a) VPAD32(a##0) VPAD32(a##1)
#define VPAD128(a) VPAD64(a##0) VPAD64(a##1)
struct CGameScriptInterface {
    VPAD128(p0) VPAD32(p1) VPAD16(p2) VPAD2(p3)        // slots 0..177
    virtual float GetHeroScariness() const;            // slot 178 (+0x2c8)
    virtual float GetHeroAttractiveness() const;       // slot 179 (+0x2cc)
};
struct CQ_SunnyvaleMasterData {
    long        _pad0;
    long        PostSavePosition;                      // +0x04
    unsigned char _pad8[0x3a - 0x8];
    bool        BanditCampTwinbladeKilled;             // +0x3a
    unsigned char _pad3b[0x54 - 0x3b];
    CCharString TeddySolution;                         // +0x54
};
struct CScriptThing { void* vptr; void* PImp[2]; };
struct CV_BookCollectingScript;
class CBS_Teacher {
public:
    void*                    __vftable;                // +0x00
    CGameScriptInterface*    Game;                     // +0x04
    CScriptThing             Me;                       // +0x08
    CV_BookCollectingScript* ParentClass;              // +0x14
    CQ_SunnyvaleMasterData*  PMasterData;              // +0x18
    bool IsActiveThreadTerminating();                  // 0x00F35B30
    void GetBookSpecificArgs(long book, CTextArgMap& args);
};

void CBS_Teacher::GetBookSpecificArgs(long book, CTextArgMap& args)
{
    if (book == 15) {
        if (IsActiveThreadTerminating())
            return;
        if (PMasterData->PostSavePosition <= 1250) {
            if (IsActiveThreadTerminating())
                return;
            args["$ARG1"] = "TEXT_CS_B16_BOOK15_90D";
        } else if (PMasterData->PostSavePosition <= 1700) {
            if (IsActiveThreadTerminating())
                return;
            args["$ARG1"] = "TEXT_CS_B16_BOOK15_90A";
        } else if (PMasterData->PostSavePosition <= 2300) {
            if (IsActiveThreadTerminating())
                return;
            args["$ARG1"] = "TEXT_CS_B16_BOOK15_90B";
        } else if (PMasterData->PostSavePosition > 2600) {
            if (IsActiveThreadTerminating())
                return;
            args["$ARG1"] = "TEXT_CS_B16_BOOK15_90B";
        } else {
            if (IsActiveThreadTerminating())
                return;
            args["$ARG1"] = "TEXT_CS_B16_BOOK15_90C";
        }
    } else if (book == 17) {
        if (IsActiveThreadTerminating())
            return;
        if (Game->GetHeroAttractiveness() > 0.5) {
            if (IsActiveThreadTerminating())
                return;
            args["$ARG1"] = "TEXT_CS_B16_BOOK17_20B";
        } else {
            if (IsActiveThreadTerminating())
                return;
            args["$ARG1"] = "TEXT_CS_B16_BOOK17_20A";
        }
    } else if (book == 19) {
        if (IsActiveThreadTerminating())
            return;
        if (PMasterData->PostSavePosition <= 700) {
            if (IsActiveThreadTerminating())
                return;
            args["$ARG1"] = "TEXT_CS_B16_BOOK19_80A";
        } else if (PMasterData->BanditCampTwinbladeKilled) {
            if (IsActiveThreadTerminating())
                return;
            args["$ARG1"] = "TEXT_CS_B16_BOOK19_80C";
        } else {
            if (IsActiveThreadTerminating())
                return;
            args["$ARG1"] = "TEXT_CS_B16_BOOK19_80B";
        }
    } else if (book == 21) {
        if (IsActiveThreadTerminating())
            return;
        CCharString solution(PMasterData->TeddySolution);
        args["$ARG1"] = "TEXT_CS_B16_BOOK21_20" + solution;
        args["$ARG2"] = "TEXT_CS_B16_BOOK21_30" + solution;
        if (solution == "A") {
            if (IsActiveThreadTerminating())
                return;
            args["$ARG3"] = "TEXT_CS_B16_BOOK21_40A";
        } else {
            if (IsActiveThreadTerminating())
                return;
            args["$ARG3"] = "TEXT_CS_B16_BOOK21_40B";
        }
    } else if (book == 22) {
        if (IsActiveThreadTerminating())
            return;
        if (Game->GetHeroScariness() > 0.5) {
            if (IsActiveThreadTerminating())
                return;
            args["$ARG1"] = "TEXT_CS_B16_BOOK22_20B";
        } else {
            if (IsActiveThreadTerminating())
                return;
            args["$ARG1"] = "TEXT_CS_B16_BOOK22_20A";
        }
    } else if (book == 23) {
        if (IsActiveThreadTerminating())
            return;
        if (Game->GetHeroAttractiveness() < -0.5) {
            if (IsActiveThreadTerminating())
                return;
            args["$ARG1"] = "TEXT_CS_B16_BOOK23_10A";
        } else if (Game->GetHeroAttractiveness() > 0.5) {
            if (IsActiveThreadTerminating())
                return;
            args["$ARG1"] = "TEXT_CS_B16_BOOK23_10C";
        } else {
            if (IsActiveThreadTerminating())
                return;
            args["$ARG1"] = "TEXT_CS_B16_BOOK23_10B";
        }
    }
}
