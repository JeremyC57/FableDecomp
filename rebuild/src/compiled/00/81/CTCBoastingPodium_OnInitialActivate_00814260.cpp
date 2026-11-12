// CTCBoastingPodium::OnInitialActivate() -- spawn a boasting position object at every "boast position" dummy of the podium's mesh (numbered by the suffix after the prefix), tell each position its podium and file it by number; anything that is not a boasting position is killed; finally sort the position map
// Leaf: ?OnInitialActivate@CTCBoastingPodium@@UAEXXZ
// Flags: none
// Retail 0x00814260, 839 bytes, `and esp,-8` frame, `ret`. Identity: Ego_r.pdb / FableWin.pdb ?OnInitialActivate@CTCBoastingPodium@@UAEXXZ
//   (CTCBase vtable slot 6).
// Callees: NGraphicAppearance::CTCGraphicAppearance::GetAllDummyObjectNames 0x004BDB40 / GetDummyObject 0x004C08C0, CCharString(const char*,
//   long) 0x0099EBF0 / ~CCharString 0x0099EAE0 / operator=(const CCharString&) 0x0099EFB0 / GetSubString 0x0099EC70, CBasicString<char>
//   Compare 0x00411570 (__fastcall, the inline operator==), strncmp 0x00BFEAF8 (the inline StartsWith), CStringParser ctor 0x00404720 /
//   ReadAsInteger 0x009BA540 (its implicit dtor: ~list<CStringParserCommentBoundaries> 0x004046B0, the two strings, ~CBaseClass
//   0x0099A2E0), CTCBase::GetDefinitionManager 0x006869D0, CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410,
//   CThingObject::Create 0x00703210 (__fastcall), CVectorMap::LowerBound 0x0040F020, CTCBoastingPosition::SetBoastingPodium 0x008147D0,
//   CThing::Kill 0x004C9B80, CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10
//   (CIntelligentPointer<CThing> vtable 0x01238C6C), vector<pair<long, CIntelligentPointer<CThing> > >::push_back 0x006A3CD0, std::sort
//   0x006A3F20 (__fastcall), free 0x00BFEA14 (the name vector's storage).
// Globals: BOASTING_POSITION_NAME_PREFIX 0x013BAF24 and BOASTING_DRUM_DEF_NAME 0x013BAF28 (anonymous-namespace const CCharStrings; Ego_r /
//   FableWin `dynamic initializer for` names, reloc_bindings 0x0121F4B0 / 0x0121F4D0).
// Layout: CTCBoastingPodium has no shared header: a local class on the shared CTCBase with the Ego_r.pdb members (BoastingPositions
//   CVectorMap<long, CIntelligentPointer<CThing> > +0x0C (Compare +0x18, Dirty +0x19), LastPositionsWithBoastCount +0x20,
//   FrameBoastListFrom +0x24, BoastingActive +0x28, PDef +0x2C). CThing PTCGraphicAppearance +0x64, PlayerNumber +0x90.
// Shape: the boasting position is reached with GetTC(TCI_BOASTING_POSITION, &position) (the expanded presence test + LowerBound);
//   CVectorMap::Insert(key, value) / Update() as Ego_r.pdb declares them (inline in retail, push_back and sort out of line). The player
//   number and the podium passed to the position go through CTCBase::GetParentThing() (Ego_r.pdb inline): read straight off
//   ParentThing, VC7.1 schedules the player's movsx after the script name's push and rotates eax/ecx/edx through the rest (the old
//   draft's whole residue). The name compare / assign work on a block-scoped `const CCharString& name = names[i]` (retail keeps
//   &names[i] in esi for the operator= push), and the empty script name is a named local of the Create call's block.
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_STARTSWITH
#define FABLE_RETAIL_CCHARSTRING_SUBSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_EQ
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGOBJECT_CREATE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_CVECTORMAP_INSERT
#define FABLE_RETAIL_CVECTORMAP_INSERT_KEYVALUE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_DUMMYOBJECT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_DUMMYOBJECTNAMES
#define FABLE_RETAIL_CTCBOASTINGPOSITION_SETBOASTINGPODIUM
#define FABLE_RETAIL_C3DVECTOR
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/ETCInterfaceType.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CVectorMap.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingObject.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CStringParser.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CTCBoastingPosition.h"

namespace {
extern const CCharString BOASTING_POSITION_NAME_PREFIX;     // 0x013BAF24
extern const CCharString BOASTING_DRUM_DEF_NAME;            // 0x013BAF28
}

class CBoastingPodiumDef;
class CTCBoastingPodium : public CTCBase {                  // RTTI .?AVCTCBoastingPodium@@ : CTCBase; Ego_r.pdb members
public:
    virtual void OnInitialActivate();
private:
    CVectorMap<long, CIntelligentPointer<CThing>, CKeyPairCompareLess<long, CIntelligentPointer<CThing> > > BoastingPositions;   // +0x0c
    long LastPositionsWithBoastCount;                       // +0x1c
    unsigned long FrameBoastListFrom;                       // +0x20
    bool BoastingActive;                                    // +0x24
    CDefPointer<const CBoastingPodiumDef> PDef;             // +0x28
};

void CTCBoastingPodium::OnInitialActivate()
{
    std::vector<CCharString> names;
    NGraphicAppearance::CTCGraphicAppearance* appearance = ParentThing->GetTCGraphicAppearance();
    appearance->GetAllDummyObjectNames(names);
    CCharString last_name("");
    long index = 0;
    for (unsigned long i = 0; i < names.size(); ++i) {
        if (names[i].StartsWith(BOASTING_POSITION_NAME_PREFIX)) {
            {
                const CCharString& name = names[i];
                if (name == last_name)
                    ++index;
                else {
                    last_name = name;
                    index = 0;
                }
            }
            C3DVector position;
            CRightHandedSet rh_set;
            appearance->GetDummyObject(names[i], index, 0, position, rh_set, 1.0f);
            CCharString number = names[i].GetSubString(BOASTING_POSITION_NAME_PREFIX.GetLength(), names[i].GetLength());
            if (number.GetLength() > 0) {
                CStringParser parser(number);
                long position_number = parser.ReadAsInteger();
                CThingObject* thing;
                {
                    CCharString script_name("");
                    thing = CThingObject::Create(GetDefinitionManager().GetDefGlobalIndexFromName(BOASTING_DRUM_DEF_NAME), position,
                                                 GetParentThing().GetPlayerNumber(), &rh_set, 0, script_name);
                }
                CTCBoastingPosition* boasting_position;
                if (thing->GetTC(TCI_BOASTING_POSITION, &boasting_position)) {
                    boasting_position->SetBoastingPodium(GetParentThing());
                    BoastingPositions.Insert(position_number, CIntelligentPointer<CThing>(thing));
                } else {
                    thing->Kill(false);
                }
            }
        }
    }
    BoastingPositions.Update();
}