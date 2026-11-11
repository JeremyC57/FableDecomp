#include "fable_compiler.h"
struct AstmTarget;
// agentNF17: the AstmNode / AstmMap stand-ins are the explicit specialisation CVectorMap<ETCInterfaceType, CTCBase*> (the landed 0x0074EE90
//   idiom) and its std::pair value_type; 0x0040F020 is its LowerBound (NF14 / established spelling).
class CTCBase;
enum ETCInterfaceType { };
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class C> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> value_type;
    value_type* _M_start;                                   // +0x00 (the STLport vector)
    value_type* _M_finish;                                  // +0x04
    value_type* LowerBound(const ETCInterfaceType& key);    // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
FABLE_FORCEINLINE AstmTarget* GetTCIfPresent(unsigned char* thing, int kind)
{
    AstmTarget* tc;                                // deliberately unset when the component is absent
    if (*(unsigned int*)(thing + 0x30) & 0x4000)
        {
        CInterfaceMap* map = (CInterfaceMap*)(thing + 0x44);
        ETCInterfaceType key = (ETCInterfaceType)kind;
        CInterfaceMap::value_type* node = map->LowerBound(key);
        tc = (AstmTarget*)((node == map->_M_finish || kind < node->first) ? map->_M_finish : node)->second;
    }
    return tc;
}
struct AstmTarget { void d(void* a0, float a1, bool a2, int z, int one); };
class CThingPlayerCreature;
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };   // 0x00487DC0
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
 /* 0x00449970 CPlayerManager::GetMainPlayer (Ego_r 0x004450F0) */

void __fastcall CGameScriptInterface_AddScreenTitleMessage(
    void* self_ecx,
    unsigned long,
    void* areaName,
    float fadeTime,
    bool messageFlag)
{
    void* chain = *(void**)((char*)self_ecx + 0x14);
    CPlayer* a = (&((CPlayerManager*)chain)->GetMainPlayer());
    unsigned char* obj = (unsigned char*)a->GetPControlledCreature();
    if (obj == 0) return;
    if (*(unsigned char*)(obj + 0x91) & 0x01) return;
    AstmTarget* target = GetTCIfPresent(obj, 0x8e);
    if (target == 0) return;
    target->d(areaName, fadeTime, messageFlag, 0, 1);
}
