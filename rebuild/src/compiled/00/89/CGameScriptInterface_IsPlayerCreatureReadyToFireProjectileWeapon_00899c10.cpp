/* CGameScriptInterface::IsPlayerCreatureReadyToFireProjectileWeapon @ 0x00899c10
 * VC7.1 / __fastcall member modelled as free __fastcall(this in ecx).
 * Signed bool return in al; single out-parameter (float*) => ret 4.
 *
 * Register-alloc fix: retail passes &key to the map lookup as a PUSHED stack
 * argument with the map in ecx (a __fastcall member), NOT via edx (__fastcall).
 * Modelling the lookup as a real __fastcall member of the map struct makes
 * VC7.1 emit  lea eax,[esp+4] / push eax / mov ecx,esi  exactly as retail.
 */

/* --- self / object overlays (only the offsets retail touches) --- */
struct IsPCReady_Self
{
    char   pad00[0x14];
    void*  m_Field14;   /* [this+0x14] fed to the subsystem accessor */
};

struct IsPCReady_Creature
{
    char           pad00[0x30];
    unsigned char  m_Flags30;    /* [+0x30] : bit1 (0x2) must be set        */
    char           pad31[0x91 - 0x31];
    unsigned char  m_Flags91;    /* [+0x91] : bit0 (0x1) must be clear       */
};
class CThingPlayerCreature : public IsPCReady_Creature { };
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };   // 0x00487DC0

/* std-style ordered map living at creature+0x44: begin at +0, end at +4. */
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
typedef std::pair<ETCInterfaceType, CTCBase*> IsPCReady_MapNode;

template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    IsPCReady_MapNode* m_Begin;  /* [+0x00] (unused by this fn) */
    IsPCReady_MapNode* m_End;    /* [+0x04] end sentinel        */

    /* __fastcall member: this(=map) in ecx, &key pushed on stack. */
    const IsPCReady_MapNode* LowerBound(const ETCInterfaceType& key) const;   /* call ffb9c9a0 */
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > IsPCReady_Map;

/* --- engine callees / globals: relocation-masked, declared extern --- */
class CPlayerManager { public: CPlayer& GetMainPlayer(); }; /* 0x00449970 CPlayerManager::GetMainPlayer (Ego_r 0x004450F0) */
extern float  __fastcall IsPCReady_GetReadiness(void* weapon);               /* call ffeefe50 */
extern const float IsPCReady_ReadyThreshold;                                 /* ds:0x129ba3c  */

bool __fastcall CGameScriptInterface_IsPlayerCreatureReadyToFireProjectileWeapon(
    void* thisptr, unsigned long /*edx*/, float* out)
{
    IsPCReady_Self* self = (IsPCReady_Self*)thisptr;

    CPlayer* subsystem = (&((CPlayerManager*)self->m_Field14)->GetMainPlayer());
    IsPCReady_Creature* creature = subsystem->GetPControlledCreature();

    if (creature == 0)
        return false;
    if (creature->m_Flags91 & 0x1)
        return false;
    if (!(creature->m_Flags30 & 0x2))
        return false;

    IsPCReady_Map* map = (IsPCReady_Map*)((char*)creature + 0x44);

    ETCInterfaceType key = (ETCInterfaceType)(0x81);
    const IsPCReady_MapNode* node = map->LowerBound(key);
    IsPCReady_MapNode* end = map->m_End;

    if (node != end && node->first <= 0x81)
    {
        /* found: keep node */
    }
    else
    {
        node = end;
    }

    float readiness = IsPCReady_GetReadiness(((void*)node->second));

    /* fcom threshold; taken (return false) when readiness <= threshold */
    if (readiness > IsPCReady_ReadyThreshold)
    {
        *out = readiness;
        return true;
    }
    return false;
}