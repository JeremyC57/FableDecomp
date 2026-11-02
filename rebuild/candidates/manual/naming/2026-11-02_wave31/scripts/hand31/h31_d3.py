# h31_d3.py: naming wave 31, family d3 (DECISIONS_W30 D3, transitional spellings -> PDB forms): the two CTCHeroStats members calling
#  0x00686AF0 (wave 30 bound the transitional ?GetThingManager@CTCHeroStats(_Retail)@@QBEABVCThingManager@@XZ) now call the protected
#  ?GetThingManager@CTCBase@@IBEABVCThingManager@@XZ (Ego_r.pdb / FableWin.pdb), inherited: CTCHeroStats derives from engine/CTCBase.h's
#  retail branch (family tcb; RTTI .?AVCTCHeroStats@@ : CTCBase). 0x0057ABD6's own row is also respelled to the PDB
#  ?GetPMaritalHome@CTCHeroStats@@QBEPBVCThingBuilding@@PBVCThingAICreature@@@Z (was the invented CTCHeroStats_Retail with CThing* types).
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
from common import note
INC = ('#define FABLE_RETAIL_CTCBASE\n'
       '#include "engine/CTCBase.h"                              // GetThingManager() const 0x00686AF0 (protected)\n')
a = '0057abd6'
he.edit(a, 'src', '// CTCHeroStats::GetPMaritalHome(const CThing*) const --', '// CTCHeroStats::GetPMaritalHome(const CThingAICreature*) const --')
he.edit(a, 'src', '''class CThing { public:
    unsigned char    _pad_0x00[0x10];
    unsigned char    ThingType;                // +0x10 (3 = building)
    unsigned char    _pad_0x11[0x7];
    unsigned __int64 UniqueID;                 // +0x18
};
''', INC + '''
class CThing { public:
    unsigned char    _pad_0x00[0x10];
    unsigned char    ThingType;                // +0x10 (3 = building)
    unsigned char    _pad_0x11[0x7];
    unsigned __int64 UniqueID;                 // +0x18
};
class CThingBuilding : public CThing { };     // the PDB return type (single inheritance from CThing)
class CThingAICreature : public CThing { };   // the PDB parameter type
''')
he.edit(a, 'src', '''struct CTCHeroStats_Retail {
    unsigned char          _pad_0x00[0x1D8];
    CHeroMarriageInfoNode* Marriages_Head;     // +0x1D8 (PDB +0x1F0)

    const CThingManager& GetThingManager() const; // 0x00686AF0 (Ego_r.pdb / FableWin.pdb: CTCBase, protected, const)
    CThing* GetPMaritalHome(const CThing* wife) const;
};

CThing* CTCHeroStats_Retail::GetPMaritalHome(const CThing* wife) const
''', '''class CTCHeroStats : public CTCBase {          // RTTI .?AVCTCHeroStats@@ : CTCBase
public:
    unsigned char          _pad_0x0c[0x1D8 - 0x0C];
    CHeroMarriageInfoNode* Marriages_Head;     // +0x1D8 (PDB +0x1F0)

    const CThingBuilding* GetPMaritalHome(const CThingAICreature* wife) const;
};

const CThingBuilding* CTCHeroStats::GetPMaritalHome(const CThingAICreature* wife) const
''')
he.edit(a, 'src', '                return (CThing*)home;', '                return static_cast<const CThingBuilding*>(home);')
note(a, 'd3', 'DECISIONS_W30 D3: GetThingManager is the inherited protected ?GetThingManager@CTCBase@@IBEABVCThingManager@@XZ\n'
     '//   0x00686AF0 (engine/CTCBase.h retail branch; was the transitional ?GetThingManager@CTCHeroStats_Retail@@QBE...), and the own row is\n'
     '//   the PDB ?GetPMaritalHome@CTCHeroStats@@QBEPBVCThingBuilding@@PBVCThingAICreature@@@Z (Ego_r.pdb / FableWin.pdb; was the invented\n'
     '//   ?GetPMaritalHome@CTCHeroStats_Retail@@QBEPAVCThing@@PBV2@@Z)', leaf='?GetPMaritalHome@CTCHeroStats@@QBEPBVCThingBuilding@@PBVCThingAICreature@@@Z')
a = '0057ae53'
he.edit(a, 'src', '''class CThing { public:
    unsigned char _pad_0x00[0x10];''', INC + '''
class CThing { public:
    unsigned char _pad_0x00[0x10];''')
he.edit(a, 'src', '''class CTCHeroStats { public:
    void*                  __vftable;                // +0x00
    void*                  ParentThing;              // +0x04 CTCBase
    unsigned char          _pad_0x08[0x1D0];
    CHeroMarriageInfoNode* Marriages_Head;           // +0x1D8 list sentinel (PDB +0x1F0)

    const CThingManager& GetThingManager() const;    // 0x00686AF0 (Ego_r.pdb / FableWin.pdb: CTCBase, protected, const)
''', '''class CTCHeroStats : public CTCBase {               // RTTI .?AVCTCHeroStats@@ : CTCBase (vptr, ParentThing +0x04 from the header)
public:
    unsigned char          _pad_0x0c[0x1D8 - 0x0C];
    CHeroMarriageInfoNode* Marriages_Head;           // +0x1D8 list sentinel (PDB +0x1F0)

''')
note(a, 'd3', 'DECISIONS_W30 D3: GetThingManager is the inherited protected ?GetThingManager@CTCBase@@IBEABVCThingManager@@XZ\n'
     '//   0x00686AF0 (engine/CTCBase.h retail branch; was the transitional ?GetThingManager@CTCHeroStats@@QBE...). The own row\'s PDB\n'
     '//   signature (list<const CThingAICreature*>&, EMarriageState) waits on the CThingList stand-in (not this family)')
# 0x008976F0 (D3: 0x006A1440): the carrying component's call is the PDB const ?GetThingInPrimarySlot@CTCCarrying@@QBEPAVCThing@@XZ (Ego_r.pdb /
#  FableWin.pdb; the landed own row 0x006A1440 is CTCCarrying::GetThingInPrimarySlot), was the transitional
#  ?GetPrimarySlotThing@CarryingMethods@@QAEPAVCThing@@XZ.
a = '008976f0'
he.edit(a, 'src', 'struct CarryingMethods{CThing* GetPrimarySlotThing();}; //006A1440',
        'class CTCCarrying{ public:CThing* GetThingInPrimarySlot() const;}; //006A1440 ?GetThingInPrimarySlot@CTCCarrying@@QBEPAVCThing@@XZ (Ego_r.pdb / FableWin.pdb)')
he.edit(a, 'src', 'CarryingMethods* carrying=static_cast<CarryingMethods*>(', 'CTCCarrying* carrying=static_cast<CTCCarrying*>(')
he.edit(a, 'src', 'carrying->GetPrimarySlotThing()', 'carrying->GetThingInPrimarySlot()')
note(a, 'd3', 'DECISIONS_W30 D3: the carrying component is CTCCarrying and its call the PDB const\n'
     '//   ?GetThingInPrimarySlot@CTCCarrying@@QBEPAVCThing@@XZ 0x006A1440 (was the transitional ?GetPrimarySlotThing@CarryingMethods@@QAEPAVCThing@@XZ)')
