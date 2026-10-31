// COpinionReactionManager::GetAttitudeOrReactionType(const CThing* thing, bool flag, const bool* result, CTCOpinionSource*
// Grade: DIFFER(69v1405) 435 diff lines (triage-only 0v1405/448): head draft (creature view, counted def temporary, no-def / no-TC-0x49 returns); TC 0x49/0x4F lookups and the reaction selection not drafted (retry wave 2 sec2c)
// source) const -- HEAD DRAFT (first block of 1405 bytes): the creature view of the thing (type 1), the source's def tested
// through a counted temporary (add-ref / release), and the no-def / no-TC-0x49 returns of reaction 0; the TC 0x49 / 0x4F
// lookups, def-driven reaction selection and opinion maths (0x00701083..) are not drafted.
// Leaf: GetAttitudeOrReactionType
// Retail: Fable.exe (TLC, VC7.1) @ 0x00701010, 1405 bytes, `ret 10h`; Ego_r
//   ?GetAttitudeOrReactionType@COpinionReactionManager@@QBE?AW4EOpinionReactionType@@PBVCThing@@_NPB_NPAVCTCOpinionSource@@@Z.
enum EOpinionReactionType { OPINION_REACTION_NONE = 0 };
struct CRefCounted { virtual void v00(); virtual void Destroy(); long RefCount; };
struct CDefRef {
    CRefCounted* Object;
    CDefRef(CRefCounted* object) : Object(object) { if (Object) ++Object->RefCount; }
    ~CDefRef() { if (Object && --Object->RefCount == 0) Object->Destroy(); }
    bool IsNull() const { return Object == 0; }
};
class CThing { public: unsigned char _pad00[0x10]; unsigned char ThingType; unsigned char _pad11[0x28 - 0x11];
    unsigned long UsingComponents2; };
class CTCOpinionSource { public: unsigned char _pad00[0x20]; CRefCounted* PDef; };
class COpinionReactionManager {
public:
    EOpinionReactionType GetAttitudeOrReactionType(const CThing* thing, bool flag, const bool* result, CTCOpinionSource* source) const;
};
EOpinionReactionType COpinionReactionManager::GetAttitudeOrReactionType(const CThing* thing, bool flag, const bool* result,
                                                                        CTCOpinionSource* source) const
{
    const CThing* creature = 0;
    if (thing->ThingType == 1)
        creature = thing;
    bool hasDef = source && !CDefRef(source->PDef).IsNull();
    if (!hasDef)
        return OPINION_REACTION_NONE;
    if (!(thing->UsingComponents2 & 0x200))
        return OPINION_REACTION_NONE;
    (void)creature; (void)flag; (void)result;
    return OPINION_REACTION_NONE;
}
