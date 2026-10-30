# sub-wave 586 (hand part): the two module locks are PKCCriticalSection objects (ctor 0x00C1C810 / dtor 0x00C1C820, the landed
# PKCCriticalSection rows' Leaf spellings): their dynamic initializers and the 0x0143BAFC atexit thunk name the objects
# g_soundLock (0x0143BAFC, chunk 586) and g_lock_0143bb14 (0x0143BB14, the landed PKC rows' spelling). Code unchanged.
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit
OLD = 'struct CStatic_00c1c810 {\n    CStatic_00c1c810();                 // 0x00C1C810\n    ~CStatic_00c1c810();                // 0x00C1C820, reached through the atexit thunk\n};\n\n'
NEW = 'class PKCCriticalSection {                // a Win32 CRITICAL_SECTION (0x18 bytes)\npublic:\n    PKCCriticalSection();               // 0x00C1C810\n    ~PKCCriticalSection();              // 0x00C1C820, reached through the atexit thunk\n    unsigned char m_section[0x18];\n};\n\n'
edit('01224b00', 'src', OLD + 'CStatic_00c1c810 g_CStatic_00c1c810_0143bafc;', NEW + 'PKCCriticalSection g_soundLock;            // 0x0143BAFC the audio library\'s module lock (chunk 586)')
edit('01224b00', 'src', '// dynamic initializer for CStatic_00c1c810 g_CStatic_00c1c810_0143bafc @ 0x0143BAFC', '// dynamic initializer for PKCCriticalSection g_soundLock @ 0x0143BAFC')
edit('01224b20', 'src', OLD + 'CStatic_00c1c810 g_CStatic_00c1c810_0143bb14;', NEW + 'PKCCriticalSection g_lock_0143bb14;        // 0x0143BB14 the other module lock (the landed PKC rows\' spelling)')
edit('01224b20', 'src', '// dynamic initializer for CStatic_00c1c810 g_CStatic_00c1c810_0143bb14 @ 0x0143BB14', '// dynamic initializer for PKCCriticalSection g_lock_0143bb14 @ 0x0143BB14')
# 0x0122ADD0 (the atexit thunk) is not respelled: `g_soundLock.~PKCCriticalSection()` makes VC7.1 emit ??_G + operator delete
# (tried 2026-10-30); its AutoTinyThunkTarget stand-in stays (DEFERRED.tsv)
