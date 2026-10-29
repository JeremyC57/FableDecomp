// Behaviour test for NTCScriptedControl::CActionTalkBase::~CActionTalkBase @ 0x0071a430 (implicit): the conversation
// counted pointer is released (its data deleted on the last reference), every subtitled message is destroyed and the array
// freed, then the target script thing, then the action base chain.
#include <stdio.h>
#include <stdlib.h>
static char g_log[64];
static void logc(char c) { int n = 0; while (g_log[n]) ++n; g_log[n] = c; g_log[n + 1] = 0; }
class CCPPointerInfo { public: unsigned int RefCount; void (*DeleteFunc)(void*); void* Data; };
static void DeleteConversation(void*) { logc('c'); }
static void DeletePImp(void*) { logc('p'); }
template <class T> class CCountedPointer { public:
    T* Data; CCPPointerInfo* Info;
    ~CCountedPointer()
    {
        if (Info) { --Info->RefCount; if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; } }
        Data = 0; Info = 0;
    }
};
class CBaseClass { public: virtual ~CBaseClass() { logc('B'); } };
class CScriptThing : public CBaseClass { public:
    CCountedPointer<CScriptThing> PImp;
    virtual ~CScriptThing() { logc('s'); }
};
class CSubtitledMessage { public: int Id; ~CSubtitledMessage() { logc((char)('0' + Id)); } };
template <class T> class CArray { public:
    T* Start; T* Finish; T* End;
    ~CArray() { for (T* p = Start; p != Finish; ++p) p->~T(); if (Start) { logc('f'); free(Start); } }
};
class CActionBase : public CBaseClass { public: void* PTCScript; bool Activated, Terminated; };
class CScriptConversation;
class CActionTalkBase : public CActionBase { public:
    CScriptThing PTargetThing;
    long UpdateCounter;
    CArray<CSubtitledMessage> Messages;
    long CurrentMessageIndex;
    CCountedPointer<CScriptConversation> Conversation;
};
int main()
{
    CActionTalkBase* t = new CActionTalkBase;
    CCPPointerInfo* ci = new CCPPointerInfo; ci->RefCount = 1; ci->DeleteFunc = DeleteConversation; ci->Data = 0;
    t->Conversation.Data = 0; t->Conversation.Info = ci;
    CCPPointerInfo* pi = new CCPPointerInfo; pi->RefCount = 2; pi->DeleteFunc = DeletePImp; pi->Data = 0;
    t->PTargetThing.PImp.Data = 0; t->PTargetThing.PImp.Info = pi;
    t->Messages.Start = (CSubtitledMessage*)malloc(3 * sizeof(CSubtitledMessage));
    for (int i = 0; i < 3; ++i) t->Messages.Start[i].Id = i + 1;
    t->Messages.Finish = t->Messages.End = t->Messages.Start + 3;
    CBaseClass* b = t; delete b;
    const char* want = "c123fsBB";
    for (int i = 0; want[i] || g_log[i]; ++i) if (want[i] != g_log[i]) { printf("0071a430_TEST FAIL\n"); return 1; }
    if (pi->RefCount != 1) { printf("0071a430_TEST FAIL\n"); return 1; }
    delete pi;
    printf("0071a430_TEST PASS\n");
    return 0;
}