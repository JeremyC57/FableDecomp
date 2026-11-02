# h18_gtests2.py: naming wave 18, family pclass (CPlayerGui) -- the 16 CGameScriptInterface behaviour tests that defined the
# row's old global stand-in (`<Owner>* <Gui>;`) now define the singleton's private static PClass through the same
# CTBaseSingletonNoDefaultAlloc<CPlayerGui> model the source reads (Get()), and set it with the retail Set() static.
import os, sys, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
MODEL = '''class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; T& operator*() const { return *Data; } };
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the source reads the private static PClass through Get()
    static CCountedPointer<T> PClass;
public:
    static T& __fastcall Get() { return *PClass; }
    static void __fastcall Set(T* p) { PClass.Data = p; PClass.Info = 0; }   // ?Set@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@SIXPAVCPlayerGui@@@Z
};
template <class T> CCountedPointer<T> CTBaseSingletonNoDefaultAlloc<T>::PClass;
template class CTBaseSingletonNoDefaultAlloc<CPlayerGui>;
'''
def fix(a, cls, old):
    p, rel, t = he._rw(a, 'tst')
    if 'CTBaseSingletonNoDefaultAlloc<CPlayerGui>' in t: print('already', a); return
    assert 'CPlayerGui' not in t, a
    n0 = t.count(cls)
    t = re.sub(r'\bstruct\s+' + cls + r'\s*\{', 'class CPlayerGui\n{\npublic:', t, count=1)
    t = re.sub(r'\b' + cls + r'\b', 'CPlayerGui', t)
    t, n = re.subn(r'(?m)^CPlayerGui\*\s*\n?\s*' + old + r'\s*;[^\n]*\n', MODEL, t); assert n == 1, (a, n)
    t, n = re.subn(r'(?m)^(\s*)' + old + r'\s*=\s*&(\w+)\s*;', r'\1CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Set(&\2);', t); assert n >= 1, (a, n)
    assert old not in t, (a, old)
    he._w(p, t)
    print('ok', a, cls, old, n)
ROWS = [l.split() for l in '''00891920 CGSIDisplayQuestInfo_GuiOwner CGSIDisplayQuestInfo_Gui
00891b20 CGSIAddQuestInfoTimer_GuiOwner CGSIAddQuestInfoTimer_Gui
00891b30 CGSIAddQuestInfoCounter_GuiOwner CGSIAddQuestInfoCounter_Gui
00891b40 CGSIAddQuestInfoCounterScalar_GuiOwner CGSIAddQuestInfoCounterScalar_Gui
00891b50 CGSIAddQuestInfoCounterList_GuiOwner CGSIAddQuestInfoCounterList_Gui
00891b60 CGSIAddQuestInfoTickText_GuiOwner CGSIAddQuestInfoTickText_Gui
00891b70 CGSIAddQuestInfoTickAction_GuiOwner CGSIAddQuestInfoTickAction_Gui
00891b80 CGSIUpdateQuestInfoBar_GuiOwner CGSIUpdateQuestInfoBar_Gui
00891ba0 CGSIChangeQuestInfoBarColour_GuiOwner CGSIChangeQuestInfoBarColour_Gui
00891bd0 CGSIUpdateQuestInfoCounter_GuiOwner CGSIUpdateQuestInfoCounter_Gui
00891be0 CGSIUpdateQuestInfoCounterList_GuiOwner CGSIUpdateQuestInfoCounterList_Gui
00891bf0 CGSIUpdateQuestInfoTick_GuiOwner CGSIUpdateQuestInfoTick_Gui
00891c10 CGSIRemoveQuestInfoElement_GuiOwner CGSIRemoveQuestInfoElement_Gui
00892280 CGSISetCutsceneSkippable_Manager CGSISetCutsceneSkippable_ManagerInstance
008922a0 CGSISetCutsceneSkippablePaused_Manager CGSISetCutsceneSkippablePaused_ManagerInstance
008922d0 CGSISetScreenMessagesEnabled_Manager CGSISetScreenMessagesEnabled_ManagerInstance'''.split('\n')]
if __name__ == '__main__':
    for a, cls, old in ROWS: fix(a, cls, old)
