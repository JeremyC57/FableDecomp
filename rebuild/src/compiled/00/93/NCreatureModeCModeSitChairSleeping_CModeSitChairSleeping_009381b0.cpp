// NCreatureMode::CModeSitChairSleeping::CModeSitChairSleeping(CThing& thing) -- Initialise the sitting mode and its two animation groups.
// Leaf: ??0CModeSitChairSleeping@NCreatureMode@@QAE@AAVCThing@@@Z
// Identity: retail RTTI01292398 proves owner; strings and CModeSitBase008d7ca0/SetGroups0073e040 prove construction.
// Layout: no active mode/filter/string shared headers exist. CModeSitBase is44bytes; Filter+44 is CAnimFilterOf2Groups20bytes. PDB fields and retail ctor stores agree.
class CThing;
class CCharString {public:void* PData;CCharString();CCharString(const char*,long=-1);~CCharString();};
class CAnimFilterBase {public:virtual ~CAnimFilterBase();};
class CAnimFilterOf2Groups:public CAnimFilterBase {public:CCharString Group1,Group2;unsigned long Group1CRC,Group2CRC;
virtual ~CAnimFilterOf2Groups();
CAnimFilterOf2Groups(const CCharString& a,const CCharString& b){SetGroups(a,b);}
void SetGroups(const CCharString&,const CCharString&);
};
namespace NCreatureMode {
class CModeSitBase {public:virtual ~CModeSitBase();unsigned char BaseState[40];CModeSitBase(CThing&);};
class CModeSitChairSleeping:public CModeSitBase {public:CAnimFilterOf2Groups Filter;virtual ~CModeSitChairSleeping();CModeSitChairSleeping(CThing&);};
CModeSitChairSleeping::CModeSitChairSleeping(CThing& thing):CModeSitBase(thing),Filter("SIT_CHAIR","SIT_CHAIR_SLEEPING"){}
}