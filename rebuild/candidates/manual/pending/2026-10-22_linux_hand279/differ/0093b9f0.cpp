// Grade: DIFFER(1084v1012) -- control flow right (do-while on the move result, goto to the shared Terminate), but ours keeps the move target in registers (retail homes it at [esp+0x1c], shared with the look position), pushes the creature argument before each GetPWorld() call (retail loads it into edi first and pushes after), and the 0x34 key slot / perceive register (ebx v ebp) differ
// CAIStateGroup_SeekEnemy::Update() -- while runnable and the enemy lives: run (weapon out) to the enemy's perceived position, or to its real position with a one-off battle cry or combat speech; mark the enemy's stealth as in danger; on arrival look at the perceived position, play WHATS_THAT animations and wander while the perceived position stays put; terminate when done
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0093B9F0, length 1024 bytes (incl. int3 padding), frame 0xd0, plain `ret`.
// Callees: IsStillRunnable @ 0x0093B8B0, CThingCreatureBase::IsPerformingAnyAction @ 0x006622A0 / IsFreeToFinishAction
// @ 0x006621C0 / FinishCurrentAction @ 0x00662650 / GetRandomAnimFromName @ 0x00662D80, CVectorMap::LowerBound const
// @ 0x004365B0 (0x34 perception), CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CTCPerceiveThing::GetPerceivedThing
// @ 0x007446A0, CThing::GetPosition @ 0x004C73D0, GetPWorld @ 0x008FCFC0, CWorld::GetRandomSeed @ 0x0049D810, GFROR13
// @ 0x00497890, CombatDialogueManager::SaySpeech @ 0x008AFC40 / PlayBattleCry @ 0x008AF850, CCombatManager::
// CreatureUnsheathWeapon @ 0x006EBDD0, AIAction_MoveToPosProximity @ 0x0097E890, CTCStealth::SetAsInDanger @ 0x00747B80,
// AIAction_LookAtPos @ 0x0097D580, CCharString ctor @ 0x0099EBF0 / dtor @ 0x0099EAE0 ("WHATS_THAT" @ 0x01273694),
// CCreatureAction_PlayAnimation ctor @ 0x00842570 / dtor @ 0x00693EF0, AIAction_PerformAction @ 0x0097D8B0,
// AIAction_MinionWander @ 0x0097F870, Terminate @ 0x008FCF40. Globals: proximity float 0x0138CC98, epsilon 0x0129BA3C.
// Layout: local models (engine/CAIStateGroup_SeekEnemy.h names PEnemy +0x14); CTCPerceiveThing perceived position +0x20,
// flags +0x2d / +0x31; the world's combat manager +0x4c (dialogue +0x38, ref-counted speech set +0x44, id +0x48).
// Shape: the 0x34 lookup is the word-local HasTC (byte homed); the enemy's 0x36 stealth lookup inlines the lower_bound.
struct CTCBase;
struct C3DVector {
    float X, Y, Z;
};
class CCharString {
public:
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
private:
    void* PStringData;
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;       // 0x004365B0
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
    bool HasTC(long interfaceType) const
    {
        unsigned long word = m_presentBits[interfaceType >> 5];
        return ((word >> (interfaceType & 31)) & 1) != 0;
    }
    template<class T> __forceinline bool GetInterfaceWord(T** ppOut, long interfaceType) const
    {
        const bool& present = HasTC(interfaceType);
        if (present) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
        }
        return present != 0;
    }
    template<class T> __forceinline bool GetInterfaceInline(T** ppOut, long interfaceType)
    {
        const bool& present = HasTC(interfaceType);
        if (present) {
            CInterfaceMap::SEntry* last = m_interfaces.last;
            CInterfaceMap::SEntry* it = m_interfaces.first;
            int len = int(last - it);
            while (len > 0) {
                int half = len / 2;
                if (it[half].type < interfaceType) {
                    it += half + 1;
                    len = len - half - 1;
                } else
                    len = half;
            }
            if (it == last || interfaceType < it->type)
                it = last;
            *ppOut = (T*)it->component;
        }
        return present != 0;
    }
};
struct CAnimationEntry;
struct CThingCreatureBase : CThing {
    bool IsFreeToFinishAction();                // 0x006621C0
    bool IsPerformingAnyAction();               // 0x006622A0
    void FinishCurrentAction();                 // 0x00662650
    CAnimationEntry* GetRandomAnimFromName(const CCharString& name, long index);   // 0x00662D80
};
struct CTCPerceiveThing {                       // interface 0x34
    unsigned char _pad[0x20];
    C3DVector PerceivedPos;                     // +0x20
    unsigned char _pad2c;
    bool m_flag2d;                              // +0x2d
    unsigned char _pad2e[3];
    bool m_flag31;                              // +0x31
    CThing* GetPerceivedThing();                // 0x007446A0
};
struct CTCStealth {                             // interface 0x36
    void SetAsInDanger(CThing* source, long status);   // 0x00747B80
};
struct CRefCounted {
    virtual void v00();
    virtual void Release();                     // slot 1
    long RefCount;                              // +0x04
};
struct CCombatSpeechSet : CRefCounted {
    unsigned char _pad08[0x40];
    long SpeechID;                              // +0x48
};
struct CSpeechSetPtr {
    CCombatSpeechSet* P;
    CSpeechSetPtr(const CSpeechSetPtr& o) : P(o.P) { if (P) ++P->RefCount; }
    ~CSpeechSetPtr() { if (--P->RefCount == 0) P->Release(); }
    CCombatSpeechSet* operator->() const { return P; }
};
struct CombatDialogueManager {
    bool PlayBattleCry(CThing* creature);       // 0x008AF850
    bool SaySpeech(CThing* creature, long speech);   // 0x008AFC40
};
struct CCombatManager {
    unsigned char _pad[0x38];
    CombatDialogueManager Dialogue;             // +0x38
    unsigned char _pad39[0x44 - 0x39];
    CSpeechSetPtr PSpeechSet;                   // +0x44
    void CreatureUnsheathWeapon(CThing* creature, long mode);   // 0x006EBDD0
};
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890
struct CWorld {
    unsigned char _pad[0x4c];
    CCombatManager* PCombatManager;             // +0x4c
    unsigned long* GetRandomSeed();             // 0x0049D810
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* Creature;               // +0x18
};
struct CBlendTimes {
    long In, Out;
    CBlendTimes() : In(0), Out(0) { }
};
class CCreatureAction_PlayAnimation {
public:
    CCreatureAction_PlayAnimation(CThingCreatureBase* creature, CAnimationEntry* anim, const CBlendTimes& blend,
                                  long a, bool b, long c, long d, long e);   // 0x00842570
    ~CCreatureAction_PlayAnimation();           // 0x00693EF0
private:
    unsigned char _body[0xa4];
};
extern float g_SeekEnemyProximity;              // 0x0138CC98
extern const float g_SeekEnemyStillDistance;    // 0x0129BA3C
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->Creature; }
    CWorld* GetPWorld();                        // 0x008FCFC0
    void Terminate();                           // 0x008FCF40
    long AIAction_MoveToPosProximity(long repeat, C3DVector pos, float proximity, bool* arrived, long movement,
                                     bool a, bool b, bool c, const CThing* ignore, bool d, float e);   // 0x0097E890
    long AIAction_LookAtPos(long repeat, C3DVector pos, bool a, float speed);   // 0x0097D580
    long AIAction_PerformAction(CCreatureAction_PlayAnimation* action);       // 0x0097D8B0
    long AIAction_MinionWander(long repeat, bool a);                          // 0x0097F870
    unsigned long Random(unsigned long range)
    {
        unsigned long* seed = GetPWorld()->GetRandomSeed();
        unsigned long next = *seed * 0x24A1U + 0x24DFU;
        *seed = next;
        unsigned long rotated = GFROR13(next);
        *seed = rotated;
        return rotated % range;
    }
};
struct CAIStateGroup_SeekEnemy : CAIStateGroupBase {
    CBaseIntelligentPointer PEnemy;             // +0x14 CIntelligentPointer<CThing>
    bool IsStillRunnable();                     // 0x0093B8B0
    virtual void Update();
};

void CAIStateGroup_SeekEnemy::Update()
{
    if (IsStillRunnable()) {
        if (GetCreature()->IsPerformingAnyAction() && GetCreature()->IsFreeToFinishAction())
            GetCreature()->FinishCurrentAction();
        CTCPerceiveThing* perceive;
        GetCreature()->GetInterfaceWord(&perceive, 0x34);
        bool arrived = false;
        bool shouted = false;
        long result;
        do {
            if (!PEnemy.GetPItem() || (PEnemy.GetPItem()->m_flags91 & 1))
                goto done;
            C3DVector target;
            if (perceive->GetPerceivedThing() == PEnemy.GetPItem() && !perceive->m_flag2d && !perceive->m_flag31)
                target = perceive->PerceivedPos;
            else {
                target = PEnemy.GetPItem()->GetPosition();
                if (!shouted) {
                    if (Random(100) > 50) {
                        long speech = CSpeechSetPtr(GetPWorld()->PCombatManager->PSpeechSet)->SpeechID;
                        GetPWorld()->PCombatManager->Dialogue.SaySpeech(GetCreature(), speech);
                        shouted = true;
                    } else
                        shouted = GetPWorld()->PCombatManager->Dialogue.PlayBattleCry(GetCreature());
                }
            }
            GetPWorld()->PCombatManager->CreatureUnsheathWeapon(GetCreature(), 4);
            result = AIAction_MoveToPosProximity(2, target, g_SeekEnemyProximity, &arrived, 5, true, true, true,
                                                 PEnemy.GetPItem(), false, 0.0f);
            if (result == 3 || !IsStillRunnable())
                goto done;
            CTCStealth* stealth;
            if (PEnemy.GetPItem()->GetInterfaceInline(&stealth, 0x36))
                stealth->SetAsInDanger(GetCreature(), 2);
        } while (result == 4);
        if (result == 2) {
            C3DVector lookPos = perceive->PerceivedPos;
            if (AIAction_LookAtPos(1, lookPos, true, 0.125f) != 1 && IsStillRunnable()) {
                CAnimationEntry* anim = GetCreature()->GetRandomAnimFromName(CCharString("WHATS_THAT"), -1);
                for (;;) {
                    long played = AIAction_PerformAction(&CCreatureAction_PlayAnimation(GetCreature(), anim, CBlendTimes(),
                                                                                         0, true, 0, 0, 0));
                    if (!IsStillRunnable() || played != 2)
                        break;
                    float dx = perceive->PerceivedPos.X - lookPos.X;
                    float dy = perceive->PerceivedPos.Y - lookPos.Y;
                    float dz = perceive->PerceivedPos.Z - lookPos.Z;
                    if (!(dx * dx + dy * dy + dz * dz > g_SeekEnemyStillDistance * g_SeekEnemyStillDistance))
                        break;
                    long wander = AIAction_MinionWander(5, false);
                    if (wander != 2 && wander != 4)
                        return;
                }
            }
        }
    }
done:
    Terminate();
}
