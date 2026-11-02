# h18_gtests3.py: naming wave 18, family pgui -- the 0x0076C830 behaviour test defined the old global stand-in g_pPlayerGui (a static
# object); it now defines the singleton's private static PClass through the CTBaseSingletonNoDefaultAlloc<CPlayerGui> model and
# sets it with the retail Set() static before main (static initializer).
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit
edit('0076c830', 'tst', 'CPlayerGui* g_pPlayerGui = &g_obj;\n', '''class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; T& operator*() const { return *Data; } };
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the source reads the private static PClass through Get()
    static CCountedPointer<T> PClass;
public:
    static T& __fastcall Get() { return *PClass; }
    static void __fastcall Set(T* p) { PClass.Data = p; PClass.Info = 0; }   // ?Set@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@SIXPAVCPlayerGui@@@Z
};
template <class T> CCountedPointer<T> CTBaseSingletonNoDefaultAlloc<T>::PClass;
template class CTBaseSingletonNoDefaultAlloc<CPlayerGui>;
static int g_setPClass = (CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Set(&g_obj), 0);
''')
