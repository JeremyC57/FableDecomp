// CTCTavernTable::GetSeat — reserve a named seat and decode its zero-based index.
// Leaf: ?GetSeat@CTCTavernTable@@QAE_NAAVCThing@@AAVC3DVector@@AAVCRightHandedSet@@AAJ3PBVCCharString@@@Z
// Flags: default
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_SUBSTRING
#define FABLE_RETAIL_CCHARSTRING_CONTAINS
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTCNAMEDUSERACTIONPOINTS
#define FABLE_RETAIL_CTCNAMEDUSERACTIONPOINTS_GETNAME
#define FABLE_RETAIL_CTCNAMEDUSERACTIONPOINTS_STATTYPE_PASS27
#define FABLE_RETAIL_CTCTAVERNTABLE_GETSEAT
#define FABLE_RETAIL_CTCTAVERNTABLE_FULL_LAYOUT
#define FABLE_RETAIL_CTCNAMEDUSERACTIONPOINTS_LIFETIME_PASS27
#define FABLE_RETAIL_CTCTAVERNTABLE_LIFETIMES_PASS23
#define FABLE_RETAIL_STLP_BITSET_INDEX
#define protected public
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CTCNamedUserActionPoints.h"
#include "engine/CTCTavernTable.h"
#undef protected

long FABLE_FASTCALL GFCharStringToInt(const CCharString& text);

bool CTCTavernTable::GetSeat(CThing& thing, C3DVector& pos, CRightHandedSet& rhset,
                             long& action_point_index, long& seat_index, const CCharString* text)
{
    if (!AvailableForUse)
        return false;

    CCharString name("ACTION_POINT_SIT_");
    if (text)
        name = *text;

    CTCNamedUserActionPoints* named_user_action_points;
    GetParentThing().GetTC(&named_user_action_points);
    if (!named_user_action_points->GetFreeNamedActionPoint(thing, pos, rhset, name, action_point_index, 0, true))
        return false;

    CCharString find_text;
    named_user_action_points->GetName(action_point_index, find_text);
    if (find_text.Contains(CCharString("SIT")))
        seat_index = GFCharStringToInt(find_text.GetSubString(ActionPointSitStrlen, find_text.GetLength()));
    else if (find_text.Contains(CCharString("GAME")))
        seat_index = GFCharStringToInt(find_text.GetSubString(ActionPointGameMasterStrlen, find_text.GetLength()));
    else if (find_text.Contains(CCharString("HERO")))
        seat_index = GFCharStringToInt(find_text.GetSubString(ActionPointHeroStrlen, find_text.GetLength()));

    if (seat_index <= 0)
        return false;
    --seat_index;
    return true;
}

#include <stdio.h>

CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseObject::CBaseObject() : IntelligentPointer(0) {}
CBaseObject::~CBaseObject() {}
void CBaseObject::Validate() const {}
void CClass::Save(CCharString&) {}
CCharString::CCharString() : PStringData(0) {}

CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CTCBase::CTCBase(CThing& thing) : ParentThing(&thing) {}
CTCBase::~CTCBase() {}
void CTCBase::OnSerialise(CPersistContext&, bool) {}
void CTCBase::OnRemove() {}
void CTCBase::ConstructFromParams(const CTCParamsBase&) {}
void CTCBase::OnCreate() {}
void CTCBase::InitialActivate() {}
void CTCBase::OnInitialActivate() {}
void CTCBase::RenderUpdate(float) {}
bool CTCBase::HasFrameUpdate() const { return false; }
void CTCBase::FrameUpdate() {}
void CTCBase::BulletTimeFrameUpdate() {}
void CTCBase::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&) {}
void CTCBase::Copy(const CTCBase&) {}
void CTCBase::OnDie() {}
void CTCBase::OnKill() {}
void CTCBase::OnLevelUnload() {}
void CTCBase::OnSetInLimbo(bool) {}
bool CTCBase::Validate(CCharString&) const { return true; }
bool CTCBase::IsFreeForDeletion() const { return true; }
bool CTCBase::IsSerialiseCreatable() const { return false; }
bool CTCBase::IsSerialisableForHeroState() const { return false; }
bool CTCBase::IsComponentSerialisable() const { return false; }
bool CTCBase::IsDriver() const { return false; }
bool CTCBase::IsCopyable() const { return false; }
void CTCBase::GlobalUpdate(bool) {}
void CTCBase::HandleMessage(const NTCMessages::CMessageBase&) {}

#include <stdlib.h>

const char CCharString::NullChar = 0;
CCharString::CCharString(const char* text, long length)
{
    if (length < 0)
        length = (long)strlen(text);
    PStringData = new CCharStringData;
    PStringData->Data.PData = new char[length + 1];
    memcpy(PStringData->Data.PData, text, length);
    PStringData->Data.PData[length] = 0;
    PStringData->Data.StringLength = length;
    PStringData->Data.DataLength = length + 1;
    PStringData->Data.UseFastExtend = false;
    PStringData->NoRefs = 1;
}
CCharString::CCharString(const CCharString& other) : PStringData(other.PStringData)
{
    if (PStringData)
        ++PStringData->NoRefs;
}
CCharString::~CCharString()
{
    if (PStringData && --PStringData->NoRefs == 0) {
        delete[] PStringData->Data.PData;
        delete PStringData;
    }
}
const CCharString& CCharString::operator=(const CCharString& other)
{
    CCharString next(other);
    CCharStringData* old = PStringData;
    PStringData = next.PStringData;
    next.PStringData = old;
    return *this;
}
CCharString CCharString::GetSubString(unsigned long start, unsigned long length) const
{
    const unsigned long text_length = GetLength();
    if (start > text_length)
        start = text_length;
    if (length > text_length - start)
        length = text_length - start;
    return CCharString(GetAsCharArray() + start, length);
}
long FABLE_FASTCALL GFCharStringToInt(const CCharString& text)
{
    return strtol(text.GetAsCharArray(), 0, 10);
}
CBaseIntelligentPointer::CBaseIntelligentPointer() : PData(0) {}
CBaseIntelligentPointer::~CBaseIntelligentPointer() {}
CTCUserActionPointBase::CTCUserActionPointBase(CThing& thing)
    : CTCBase(thing), NumberOfActionPoints(0), FrameOfNextCheck(0) {}
bool CTCUserActionPointBase::IsActionPointIndexBlocked(CThing&, C3DVector&, CRightHandedSet&, long) const
{
    return false;
}
CTCNamedUserActionPoints::CTCNamedUserActionPoints(CThing& thing) : CTCUserActionPointBase(thing) {}
ETCInterfaceType CTCNamedUserActionPoints::GetInterfaceType() const { return TCI_NAMED_USER_ACTION_POINTS; }
CCharString CTCNamedUserActionPoints::GetClassTypeName() const { return CCharString(); }
bool CTCNamedUserActionPoints::IsActionPointIndexBlocked(CThing&, C3DVector&, CRightHandedSet&, long) const
{
    return false;
}
CTCTavernTable::CTCTavernTable(CThing& thing)
    : CTCBase(thing), ActionPointSitStrlen(17), ActionPointGameMasterStrlen(18),
      ActionPointHeroStrlen(18), AvailableForUse(true) {}
CTCTavernTable::~CTCTavernTable() {}
void CTCTavernTable::OnInitialActivate() {}

class TestTavernTable : public CTCTavernTable {
public:
    TestTavernTable(CThing& thing) : CTCTavernTable(thing) {}
    ETCInterfaceType GetInterfaceType() const { return TCI_TAVERN_TABLE; }
    CCharString GetClassTypeName() const { return CCharString(); }
};

static unsigned long claim_calls;
static unsigned long name_calls;
static bool claim_succeeds;
static bool claim_correct;
static const char* action_name;
static const char* expected_prefix;
static const CThing* expected_thing;

bool CTCNamedUserActionPoints::GetFreeNamedActionPoint(CThing& thing, C3DVector& position,
    CRightHandedSet&, CCharString name, long& index, long start, bool check_blocked)
{
    ++claim_calls;
    claim_correct = &thing == expected_thing && !strcmp(name.GetAsCharArray(), expected_prefix)
                    && start == 0 && check_blocked;
    if (!claim_succeeds)
        return false;
    index = 4;
    position.X = 7.0f;
    return true;
}

void CTCNamedUserActionPoints::GetName(long index, CCharString& name)
{
    ++name_calls;
    if (index == 4)
        name = CCharString(action_name);
}

int main()
{
    CThing owner, visitor;
    CTCNamedUserActionPoints points(owner);
    memset(owner.UsingComponents._M_w, 0, sizeof(owner.UsingComponents._M_w));
    const ETCInterfaceType point_type = TCI_NAMED_USER_ACTION_POINTS;
    owner.UsingComponents[point_type] = true;
    owner.Components.push_back(std::pair<ETCInterfaceType,CTCBase*>(point_type, &points));
    TestTavernTable table(owner);
    C3DVector position;
    CRightHandedSet facing;
    long point = -1;
    long seat = -1;
    expected_thing = &visitor;
    expected_prefix = "ACTION_POINT_SIT_";

    table.AvailableForUse = false;
    if (table.GetSeat(visitor, position, facing, point, seat, 0) || claim_calls || name_calls)
        return 1;

    table.AvailableForUse = true;
    if (table.GetSeat(visitor, position, facing, point, seat, 0) || claim_calls != 1
        || name_calls || !claim_correct || point != -1 || seat != -1)
        return 2;

    claim_succeeds = true;
    action_name = "ACTION_POINT_SIT_7";
    if (!table.GetSeat(visitor, position, facing, point, seat, 0) || !claim_correct
        || point != 4 || position.X != 7.0f || seat != 6 || name_calls != 1)
        return 3;

    const CCharString requested("ACTION_POINT_EXTRA_");
    expected_prefix = "ACTION_POINT_EXTRA_";
    action_name = "ACTION_POINT_GAME_2";
    if (!table.GetSeat(visitor, position, facing, point, seat, &requested) || seat != 1 || !claim_correct)
        return 4;

    action_name = "ACTION_POINT_HERO_1";
    if (!table.GetSeat(visitor, position, facing, point, seat, &requested) || seat != 0)
        return 5;

    action_name = "ACTION_POINT_HERO_0";
    if (table.GetSeat(visitor, position, facing, point, seat, &requested) || seat != 0)
        return 6;

    action_name = "OTHER";
    seat = -1;
    if (table.GetSeat(visitor, position, facing, point, seat, &requested) || seat != -1)
        return 7;

    puts("007b8840_TEST PASS");
    return 0;
}