#include <string.h>
#include <stddef.h>
#define CCharString CCharStringPdb
#define CCharStringData CCharStringDataPdb
#include "fable_string.h"
#undef CCharStringData
#undef CCharString
#pragma pack(push,1)
// NCreatureAction::CActionEventCameraShake::SetDataFromString, 0x00870FC0.
// PDB identity: egor 19895 / debug 0x00811990. The three parsed values are
// Strength(+0x14), Length(+0x18), and Global(+0x1c), as recovered from PDB.
template<class T> class CBasicString;
template<> class CBasicString<char> { public:
    char* PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool UseFastExtend;
    bool operator==(const char*) const;
};
// The 004115A0 equality body compares signed chars without modifying storage.
#pragma auto_inline(off)
bool CBasicString<char>::operator==(const char* text) const
{
    const signed char* first = (const signed char*)PData;
    const signed char* second = (const signed char*)text;
    int result = 0;
    while (*first || *second) {
        if (*first < *second) { result = -1; break; }
        if (*first > *second) { result = 1; break; }
        ++first; ++second;
    }
    return result == 0;
}
#pragma auto_inline(on)
class CCharStringData { public: CBasicString<char> Data; long NoRefs; };
#pragma pack(pop)
typedef char StringDataMatchesDonor[(sizeof(CCharStringData)==sizeof(CCharStringDataPdb))?1:-1];
class CCharString { public:
    CCharStringData* PStringData;
    CCharString();
    ~CCharString();
    __forceinline bool operator==(const char* text) const {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0) return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
namespace { bool __fastcall GetDataFromEventString(const CCharString& data, long index,
                                       CCharString& out); } // 0x00870C90

class CCreatureActionBase;
class CThingCreatureBase;
class CGameCameraManager;
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
// Original enum types; ordinal labels below are local descriptive labels.
enum EQuakeStrength { QuakeStrengthWeak=0,QuakeStrengthMedium=1,QuakeStrengthStrong=2,QuakeStrengthMadness=3 };
enum EQuakeLength { QuakeLengthShort=0,QuakeLengthMedium=1,QuakeLengthLong=2,QuakeLengthContinuous=3 };
namespace NCreatureAction {
class CActionEventBase : public CBaseClass { public:
    bool Finished;
    long ActivationFrame;
    CThingCreatureBase* PThing;
    CGameCameraManager* PCameraManager;
    virtual void Dispatch(CCreatureActionBase&)=0;
    virtual CCharString GetHook() const=0;
    virtual CActionEventBase* Alloc(long,CThingCreatureBase&,CGameCameraManager&) const=0;
    virtual bool SetDataFromString(const CCharString&)=0;
};
struct CActionEventCameraShake : CActionEventBase {
    EQuakeStrength Strength;
    EQuakeLength Length;
    bool Global;
    virtual void Dispatch(CCreatureActionBase&);
    virtual CCharString GetHook() const;
    virtual CActionEventBase* Alloc(long,CThingCreatureBase&,CGameCameraManager&) const;
    virtual bool SetDataFromString(const CCharString&);
};

bool CActionEventCameraShake::SetDataFromString(const CCharString& data)
{
    {
        CCharString value;
        if (!GetDataFromEventString(data, 0, value)) return false;
        if (value == "STRENGTH_WEAK") Strength = QuakeStrengthWeak;
        else if (value == "STRENGTH_MEDIUM") Strength = QuakeStrengthMedium;
        else if (value == "STRENGTH_STRONG") Strength = QuakeStrengthStrong;
        else if (value == "STRENGTH_MADNESS") Strength = QuakeStrengthMadness;
        else return false;
    }
    {
        CCharString length;
        if (!GetDataFromEventString(data, 1, length)) return false;
        if (length == "LENGTH_SHORT") Length = QuakeLengthShort;
        else if (length == "LENGTH_MEDIUM") Length = QuakeLengthMedium;
        else if (length == "LENGTH_LONG") Length = QuakeLengthLong;
        else if (length == "LENGTH_CONTINUOUS") Length = QuakeLengthContinuous;
        else return false;
    }
    {
        CCharString global;
        if (!GetDataFromEventString(data, 2, global)) return false;
        if (global == "GLOBAL") Global = true;
    }
    return true;
}
}