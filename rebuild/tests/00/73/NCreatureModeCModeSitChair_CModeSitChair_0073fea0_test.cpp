#include <stdio.h>
#include <string.h>
// NCreatureMode::CModeSitChair::CModeSitChair(CThing& thing) -- Initialise the sitting mode and its two animation groups.
// Leaf: ??0CModeSitChair@NCreatureMode@@QAE@AAVCThing@@@Z
// Identity: retail RTTI01267b74 proves owner; strings and CModeSitBase008d7ca0/SetGroups0073e040 prove construction.
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
class CModeSitChair:public CModeSitBase {public:CAnimFilterOf2Groups Filter;virtual ~CModeSitChair();CModeSitChair(CThing&);};
CModeSitChair::CModeSitChair(CThing& thing):CModeSitBase(thing),Filter("SIT_CHAIR","SIT_CHAIR_NORMAL"){}
}

class CThing {};
static CThing* expected;static int bases,defaults,converts,dtors,sets,bad;
static const char* converted[2];
CCharString::CCharString():PData(0){++defaults;}
CCharString::CCharString(const char* text,long n):PData((void*)text){if(n!=-1)bad=1;if(converts<2)converted[converts]=text;++converts;}
CCharString::~CCharString(){++dtors;}
CAnimFilterBase::~CAnimFilterBase(){}
CAnimFilterOf2Groups::~CAnimFilterOf2Groups(){}
void CAnimFilterOf2Groups::SetGroups(const CCharString& a,const CCharString& b){++sets;if(defaults!=2 || converts!=2 || Group1.PData || Group2.PData)bad=1;Group1.PData=a.PData;Group2.PData=b.PData;Group1CRC=17;Group2CRC=31;}
namespace NCreatureMode {
CModeSitBase::CModeSitBase(CThing& thing){++bases;if(&thing!=expected)bad=1;memset(BaseState,0x5a,sizeof(BaseState));}
CModeSitBase::~CModeSitBase(){}
CModeSitChair::~CModeSitChair(){}
}
int main(){CThing thing;expected=&thing;int ok;
 {NCreatureMode::CModeSitChair mode(thing);
 ok=!bad && bases==1 && defaults==2 && converts==2 && dtors==2 && sets==1 && strcmp((const char*)mode.Filter.Group1.PData,"SIT_CHAIR")==0 && strcmp((const char*)mode.Filter.Group2.PData,"SIT_CHAIR_NORMAL")==0 && mode.Filter.Group1CRC==17 && mode.Filter.Group2CRC==31;
 for(unsigned int i=0;i<sizeof(mode.BaseState);++i)ok=ok && mode.BaseState[i]==0x5a;
 }
 ok=ok && dtors==4;
 puts(ok?"0073fea0_TEST PASS":"0073fea0_TEST FAIL");return ok?0:1;
}