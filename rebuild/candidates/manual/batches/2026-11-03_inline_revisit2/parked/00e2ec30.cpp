// NScript::CV_AmbushScamScript::GetTrickster() -- find the living "AS_Trickster" whose data string matches the quest's morality (Good when MoralitySwitch is set, else Evil); a null thing when none
// Leaf: ?GetTrickster@CV_AmbushScamScript@NScript@@QAE?AVCScriptThing@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E2EC30, 1336 bytes, `ret 4` (hidden CScriptThing result).
// Grade: DIFFER(1053v1336) -- mixed inline/out-of-line ~CScriptThing (0x004AA840), ~vector<CScriptThing> (0x008AC970), CScriptThing copy ctor (0x004ABE90) and GetDataString (inline at the Good site, 0x004AA900 at the Evil site); inline_depth family
// Callees: CBaseClass() 0x0099A2D0 / ~CBaseClass 0x0099A2E0, CScriptThing copy ctor 0x004ABE90, ~CScriptThing 0x004AA840,
//   CCharString() 0x0099E4B0 / (const char*, long) 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0, IsActiveThreadTerminating
//   0x00CB7940, CScriptThing::GetDataString 0x004AA900, Compare 0x00411570, ~vector<CScriptThing> 0x008AC970, free 0x00BFEA14,
//   operator delete 0x00BFE9BC; interface slot 75 GetAllThingsWithScriptName (+0x12C); thing slots 3 GetDataString (+0x0C),
//   75 IsAlive (+0x12C). Strings "AS_Trickster", "Good", "Evil".
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT
#include "stlport/fable_containers.h"
#include "engine/CV_AmbushScamScript.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
inline CScriptThing::CScriptThing() { }
template<class T> inline bool CBasicString<T>::operator==(const T* text) const { return Compare(PData,text)==0; }
inline bool CScriptThing::IsAlive() const { if (!PImp.Data) return false; return PImp.Data->IsAlive(); }
inline CCharString CScriptThing::GetDataString() const { if (!PImp.Data) return NullString; return PImp.Data->GetDataString(); }

CScriptThing NScript::CV_AmbushScamScript::GetTrickster()
{
    CScriptThing null_thing;
    CScriptThing terminate_result(null_thing);
    std::vector<CScriptThing> things;
    CScriptThing thing;
    CCharString data_string;
    long count = Game->GetAllThingsWithScriptName("AS_Trickster", things);
    for (long i = 0; i < count; ++i) {
        if (IsActiveThreadTerminating())
            return terminate_result;
        thing = things[i];
        if (thing.IsAlive()) {
            if (IsActiveThreadTerminating())
                return terminate_result;
            if (MoralitySwitch && thing.GetDataString() == "Good") {
                if (IsActiveThreadTerminating())
                    return terminate_result;
                return thing;
            }
            if (!MoralitySwitch && thing.GetDataString() == "Evil") {
                if (IsActiveThreadTerminating())
                    return terminate_result;
                return thing;
            }
        }
    }
    if (IsActiveThreadTerminating())
        return terminate_result;
    return null_thing;
}
