# h18_gtests.py: naming wave 18, family pclass -- the behaviour tests that link against the source object and defined the old
# global stand-in now define the singleton's private static PClass through the same template model (the retail
# ?Set@?$CTBaseSingletonNoDefaultAlloc@VCQuestManager@@@@SIXPAVCQuestManager@@@Z static sets it).
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit
a = '00892f10'
edit(a, 'tst', '''CQuestManager* FableGlobalQuestManager;
''', '''class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; T& operator*() const { return *Data; } };
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the source reads the private static PClass through Get()
    static CCountedPointer<T> PClass;
public:
    static T& __fastcall Get() { return *PClass; }
    static void __fastcall Set(T* p) { PClass.Data = p; PClass.Info = 0; }   // ?Set@...@SIXPAVCQuestManager@@@Z
};
template <class T> CCountedPointer<T> CTBaseSingletonNoDefaultAlloc<T>::PClass;
template class CTBaseSingletonNoDefaultAlloc<CQuestManager>;
''')
edit(a, 'tst', '    FableGlobalQuestManager = &manager;\n', '    CTBaseSingletonNoDefaultAlloc<CQuestManager>::Set(&manager);\n')
