#include <stdio.h>

class CCharString;

class CQuestManager { public:
  int gotA, gotB;
  CCharString* gotCs;
  void Activate(CCharString* cs, int a, int b){ gotCs=cs; gotA=a; gotB=b; }
};

static CQuestManager gMgr;
CQuestManager* g_questMgr = &gMgr;

class CGameScriptInterface { public:
  void ActivateQuestWithoutLoadingResources(CCharString* cs);
};

void CGameScriptInterface::ActivateQuestWithoutLoadingResources(CCharString* cs)
{
  g_questMgr->Activate(cs, 1, 0);
}

int main(){
  CGameScriptInterface gsi;
  CCharString* fake = (CCharString*)0x1234;
  gsi.ActivateQuestWithoutLoadingResources(fake);
  if (gMgr.gotCs==fake && gMgr.gotA==1 && gMgr.gotB==0)
    printf("OK_00892ec0\n");
  else
    printf("FAIL\n");
  return 0;
}