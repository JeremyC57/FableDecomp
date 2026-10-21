// CGameScriptInterface::GetValidTextEntryNameWithAttitude(const CScriptThing& thing, const CCharString& baseName) const -- "<baseName>_<attitude>" for the thing's opinion attitude to the player, walking the attitude graph until such a text entry exists; the bare base name once the walk reaches "NONE"
// Leaf: GetValidTextEntryNameWithAttitude
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008920C0, length 347 bytes (`ret 0Ch`, CCharString by hidden pointer).
// Callees: own virtual slots 608 EntityGetOpinionAttitudeToPlayer (0x00890990), 607 GetOpinionAttitudeAsString (0x00890970),
//   606 GetNextInOpinionAttitudeGraph (0x00890960) and 358 TextEntryExists (0x00892070); CCharString ctor/copy ctor/
//   operator=/dtor @ 0x0099EBF0 / 0x0099EC30 / 0x0099EFB0 / 0x0099EAE0, the `__fastcall` concatenations
//   operator+(const CCharString&, const char*) @ 0x0099F600 and operator+(const CCharString&, const CCharString&)
//   @ 0x0099F570, CBasicString<char>::operator== @ 0x004115A0.
// Shape: the first name is built in the dead `thing` slot; the "NONE" test is the inline CCharString::operator==
//   written as two tests that jump to the fallback (the landed IsPlayerCarryingItemOfType 0x008973A0 form); the
//   result is copied out (no NRVO).
#include <string.h>
#pragma pack(push, 1)
struct CBasicStringChar {                   // CBasicString<char>, 13 bytes
    char*         PData;                    // +0x00
    unsigned long StringLength;             // +0x04
    unsigned long DataLength : 31;          // +0x08
    bool          UseFastExtend : 1;        // +0x0c
    bool operator==(const char* text) const;    // 0x004115A0
};
struct CCharStringData {                    // 17 bytes
    CBasicStringChar Data;                  // +0x00
    long             NoRefs;                // +0x0d
};
#pragma pack(pop)
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
CCharString __fastcall operator+(const CCharString& a, const char* b);          // 0x0099F600
CCharString __fastcall operator+(const CCharString& a, const CCharString& b);   // 0x0099F570
struct CScriptThing;
enum EOpinionAttitudeType { OPINION_ATTITUDE_NONE };

#define VPAD1(a) virtual void a();
#define VPAD2(a) VPAD1(a##0) VPAD1(a##1)
#define VPAD4(a) VPAD2(a##0) VPAD2(a##1)
#define VPAD8(a) VPAD4(a##0) VPAD4(a##1)
#define VPAD16(a) VPAD8(a##0) VPAD8(a##1)
#define VPAD32(a) VPAD16(a##0) VPAD16(a##1)
#define VPAD64(a) VPAD32(a##0) VPAD32(a##1)
#define VPAD128(a) VPAD64(a##0) VPAD64(a##1)
#define VPAD256(a) VPAD128(a##0) VPAD128(a##1)
struct CGameScriptInterface {
    VPAD256(pa) VPAD64(pb) VPAD32(pc) VPAD4(pd) VPAD2(pe)                         // slots 0..357
    virtual bool TextEntryExists(const CCharString& name) const;                   // slot 358 (+0x598)
    VPAD128(qa) VPAD64(qb) VPAD32(qc) VPAD16(qd) VPAD4(qe) VPAD2(qf) VPAD1(qg)     // slots 359..605
    virtual EOpinionAttitudeType GetNextInOpinionAttitudeGraph(EOpinionAttitudeType attitude) const;   // slot 606 (+0x978)
    virtual void GetOpinionAttitudeAsString(EOpinionAttitudeType attitude, CCharString& out) const;    // slot 607 (+0x97C)
    virtual EOpinionAttitudeType EntityGetOpinionAttitudeToPlayer(const CScriptThing& thing) const;    // slot 608 (+0x980)
    virtual CCharString GetValidTextEntryNameWithAttitude(const CScriptThing& thing, const CCharString& baseName) const;
};

CCharString CGameScriptInterface::GetValidTextEntryNameWithAttitude(const CScriptThing& thing, const CCharString& baseName) const
{
    EOpinionAttitudeType attitude = EntityGetOpinionAttitudeToPlayer(thing);
    CCharString attitudeName("");
    GetOpinionAttitudeAsString(attitude, attitudeName);
    CCharString name = baseName + "_" + attitudeName;
    if (!TextEntryExists(name)) {
        do {
            attitude = GetNextInOpinionAttitudeGraph(attitude);
            GetOpinionAttitudeAsString(attitude, attitudeName);
            name = baseName + "_" + attitudeName;
            if (attitudeName.PStringData == 0) {
                if (strcmp("NONE", "") == 0)
                    goto none;
            } else if (attitudeName.PStringData->Data == "NONE")
                goto none;
        } while (!TextEntryExists(name));
        goto done;
none:
        name = baseName;
    }
done:
    return name;
}
