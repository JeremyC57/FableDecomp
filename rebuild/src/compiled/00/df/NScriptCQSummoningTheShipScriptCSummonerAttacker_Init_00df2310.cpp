// NScript::CQ_SummoningTheShipScript::CSummonerAttacker::Init() -- one more summoner alive; WaveID is 2 when Me's data string is WAVE2, else 1
// Leaf: ?Init@CSummonerAttacker@CQ_SummoningTheShipScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DF2310, 106 bytes, thiscall, plain `ret`.
// Identity (proven): slot 2 (Init) of the RTTI-less vtable 0x012DE274, which the entity Alloc 0x00DF21F0 (`NScript::CQ_SummoningTheShipScript::CSummonerAttacker::Alloc`) stores; ?Init@CSummonerAttacker@CQ_SummoningTheShipScript@NScript@@EAEXXZ is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00df2310" is a gap-scan placeholder.
// Callees: CScriptThing::GetDataString (slot 3, +0x0c, by value), CBasicString<char>::Compare 0x00411570 (protected static __fastcall), ~CCharString 0x0099EAE0.
// Data: "" @ 0x0122D70E, "WAVE2" @ 0x012DE290 (the null-string arm compares the two literals with `repe cmpsb`).
// Layout: Ego_r.pdb CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}; the entity's own members keep their Ego_r offsets. CSummonerAttacker: WaveID +0x1c; parent SummonersAlive +0x48.
// Shape: the comparison lands in a named bool so the GetDataString temporary dies before the WaveID store; `wave2 ? 2 : 1` is `xor ; test ; setne ; inc`.
#include <string.h>

#pragma pack(push, 1)
template <class T>
class CBasicString {                                      // 13 bytes (pack 1)
public:
    T*            PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
    bool operator==(const T* text) const { return Compare(PData, text) ? false : true; }
protected:
    static long __fastcall Compare(const T* a, const T* b);   // 0x00411570
};
class CCharStringData {
public:
    CBasicString<char> Data;
    long               NoRefs;
};
#pragma pack(pop)
class CCharString {                                      // 4 bytes: the counted string-data pointer
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    ~CCharString();                                       // 0x0099EAE0
    bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
class CCPPointerInfo {                                   // CCountedPointer's shared block
public:
    long  RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T>
class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them inline or implicit): defined inline after the header.
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
class CGameScriptInterfaceBase;
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_SummoningTheShipScript {
public:
    class CSummonerAttacker;
    unsigned char _pad000[0x48];
    long SummonersAlive;                                // +0x48 (retail = Ego_r.pdb - 0x14)
};
class CQ_SummoningTheShipScript::CSummonerAttacker : public CEntityScriptBase {
public:
    CQ_SummoningTheShipScript* ParentClass;                // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;                // +0x18
    long                      WaveID;                     // +0x1c
private:
    virtual void Init();
};
}

void NScript::CQ_SummoningTheShipScript::CSummonerAttacker::Init()
{
    ++ParentClass->SummonersAlive;
    bool wave2 = Me.GetDataString() == "WAVE2";
    WaveID = wave2 ? 2 : 1;
}