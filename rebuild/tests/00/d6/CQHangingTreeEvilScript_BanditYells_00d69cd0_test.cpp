#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _MSC_VER
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#endif
#include "engine/TRetVoid.h"

namespace NScript {
class CQ_HangingTreeEvilScript;
class ScriptServices {
public:
    ScriptServices() : script(0),frames(0),pauses(0) {}
    void bind(CQ_HangingTreeEvilScript& owner) { script=&owner; }
    void NewScriptFrame();
    void Pause(float delay);
    CQ_HangingTreeEvilScript* script;
    int frames,pauses;
};
class CQ_HangingTreeEvilScript {
public:
    explicit CQ_HangingTreeEvilScript(bool initiallyYelled,int stopAt) :
        BanditYelledRecently(initiallyYelled),stopAt(stopAt),polls(0),length(0) { trace[0]=0;Game.bind(*this); }
    void BanditYells();
    bool IsActiveThreadTerminating() { event('|');return ++polls==stopAt; }
    void event(char c) { if(length>=100) exit(2);trace[length++]=c;trace[length]=0; }
    ScriptServices Game;
    bool BanditYelledRecently;
    int stopAt,polls,length;
    char trace[102];
};
void ScriptServices::NewScriptFrame() {
    script->event('N');
    ++frames;
    if(frames==2 || frames==5) script->BanditYelledRecently=true;
}
void ScriptServices::Pause(float delay) {
    if(delay!=5.0f || !script->BanditYelledRecently) exit(3);
    script->event('P');
    ++pauses;
}
}

void NScript::CQ_HangingTreeEvilScript::BanditYells()
{
    TRetVoid _Fail_Val;
    if (false) {} else for (;; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        while (!BanditYelledRecently) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        Game.Pause(5.0f);
        BanditYelledRecently = false;
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
}

static void run(bool initiallyYelled,const char* reference) {
    int poll=0;
    for(const char* last=reference;*last;++last) {
        if(*last!='|') continue;
        NScript::CQ_HangingTreeEvilScript quest(initiallyYelled,++poll);
        quest.BanditYells();
        size_t length=static_cast<size_t>(last-reference)+1;
        if(strlen(quest.trace)!=length || strncmp(quest.trace,reference,length)!=0) {
            fprintf(stderr,"unexpected frame/guard/pause order at poll %d: %s\n",poll,quest.trace);
            exit(1);
        }
    }
}
int main() {
    run(false,"|N|N||PN|N|N||PN|N|");
    run(true,"||PN|N||PN|N|N||PN|");
    puts("00d69cd0_TEST PASS");
    return 0;
}
