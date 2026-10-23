// Grade: DIFFER(592v558) -- same-TU GetSubDef stand-in fixed the def releases; def esi v edi, string/def frame slots swapped, the name==""/"NULL" tests and the char index tail laid out differently
// CTCTavernGameCoinGolf::EnableCourse(long course) -- for a valid course, show the course pieces whose name flags that course with '1' (refreshing and re-targeting them) and hide the rest, then reset the putting power and wrap the aim angle into [0, 1)
// Leaf: ?EnableCourse@CTCTavernGameCoinGolf
// Retail: Fable.exe (TLC, VC7.1) @ 0x007D2CC0, 556 bytes (+ int3 padding), `ret 4`.
#include <math.h>
#include <string.h>
#pragma intrinsic(strcmp)
#pragma pack(push, 1)
struct CBasicStringChar {
    char*         PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
    bool operator==(const char* text) const;        // 0x004115A0
};
struct CCharStringData {
    CBasicStringChar Data;
    long             NoRefs;
};
#pragma pack(pop)
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    __forceinline bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
    __forceinline bool operator!=(const char* text) const { return !(*this == text); }
    unsigned long Length() const { return PStringData ? PStringData->Data.StringLength : 0; }
    const char* GetString() const { return PStringData ? PStringData->Data.PData : ""; }
};
class CThing {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void SetVisible(bool visible);      // slot 28 (+0x70) (descriptive)
    unsigned char _pad04[0x74 - 0x04];
    CCharString ScriptName;                     // +0x74 (descriptive)
    void SetAsFrameUpdated();                   // 0x004C7C40
    void DetachTarget(long how);                // 0x004C7590 (descriptive)
};
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
#pragma pack(push, 1)
struct Coin {                                   // 0x3c bytes
    unsigned char _pad00[0x25];
    CBaseIntelligentPointer Thing;              // +0x25
    unsigned char _pad_0x2d[0x3c - 0x2d];
};
#pragma pack(pop)
struct CDefObject {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_refCount;                            // +0x04
};
struct CTavernGameCoinGolfDef : CDefObject {
    unsigned char _pad08[0x50 - 0x08];
    long NumCourses;                            // +0x50 (descriptive)
};
struct CDefPointer {
    CTavernGameCoinGolfDef* m_p;
    CDefPointer() : m_p(0) {}
    ~CDefPointer()
    {
        if (m_p) {
            if (--m_p->m_refCount == 0)
                m_p->Destroy();
        }
    }
    CTavernGameCoinGolfDef* operator->() const { return m_p; }
};
struct CDefClassBase {
    bool GetSubDef(CDefPointer* out);           // 0x007D4BD0
};
long* GetSubDefIndex(CDefClassBase* def, const char* name);
CTavernGameCoinGolfDef* GetCoinGolfDefFromGlobalIndex(long index);
#pragma auto_inline(off)
// Same-TU stand-in for GetSubDef 0x007D4BD0: VC7.1 then knows the out pointer does not escape (retail's view), so the
//   def stays in edi and the null tests before its releases fold.
bool CDefClassBase::GetSubDef(CDefPointer* out)
{
    long* pIndex = GetSubDefIndex(this, "CTavernGameCoinGolfDef");
    if (pIndex) {
        out->m_p = GetCoinGolfDefFromGlobalIndex(*pIndex);
        return true;
    }
    return false;
}
#pragma auto_inline(on)
struct CThingDef {
    unsigned char _pad[0x70];
    CDefClassBase* m_pDefinition;               // +0x70
};
struct CTCTavernGameCoinGolf {
    void* __vftable;
    CThingDef* ParentThing;                     // +0x04
    unsigned char _pad08[0xfc - 0x08];
    Coin* CoinsStart;                           // +0xfc
    Coin* CoinsFinish;                          // +0x100
    unsigned char _pad104[0x148 - 0x104];
    float AimAngle;                             // +0x148 (descriptive)
    float Power;                                // +0x14c (descriptive)
    unsigned char _pad150[0x168 - 0x150];
    float AimY;                                 // +0x168 (descriptive)
    float AimX;                                 // +0x16c (descriptive)
    void EnableCourse(long course);
};

void CTCTavernGameCoinGolf::EnableCourse(long course)
{
    CDefPointer def;
    ParentThing->m_pDefinition->GetSubDef(&def);
    if (course >= def->NumCourses)
        return;
    for (unsigned long i = 0; i < (unsigned long)(CoinsFinish - CoinsStart); ++i) {
        CCharString name(CoinsStart[i].Thing.GetPItem()->ScriptName);
        if (name != "" && name != "NULL" && (unsigned long)(course + 1) < name.Length()) {
            if (name.GetString()[course + 1] == '1') {
                CoinsStart[i].Thing.GetPItem()->SetVisible(true);
                CoinsStart[i].Thing.GetPItem()->SetAsFrameUpdated();
                CoinsStart[i].Thing.GetPItem()->DetachTarget(4);
            } else {
                CoinsStart[i].Thing.GetPItem()->SetVisible(false);
            }
        }
    }
    Power = 0.45f;
    float angle = (float)atan2(AimY, AimX) * 0.15915494f;
    if (angle < 0.0f || angle >= 1.0f) {
        angle = (float)fmod((double)angle, 1.0);
        if (angle < 0.0f)
            angle += 1.0f;
    }
    AimAngle = angle;
}
