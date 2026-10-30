# sub-wave 586 follow-up: three fam_pkc rows took the cast route for the guard but kept a row-local stand-in name for the
# 0x0143BAFC section (g_ProfileSection_ / g_RenderQueueProfileSection_ / g_emptyName_0143bafc); respell the global to the
# coordinator's ?g_soundLock@@3VPKCCriticalSection@@A (chunk 586) and pass its address to the guard (same push imm32).
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit
edit('00c1bed0', 'src', 'extern unsigned char g_ProfileSection_0143bafc[1];   // 0x0143BAFC', 'extern PKCCriticalSection g_soundLock;   // 0x0143BAFC')
edit('00c1bed0', 'src', 'PKCProtectionInstance lock((PKCCriticalSection*)g_ProfileSection_0143bafc);', 'PKCProtectionInstance lock(&g_soundLock);')
edit('00c279a0', 'src', 'extern unsigned char g_RenderQueueProfileSection_0143bafc[1];   // 0x0143BAFC', 'extern PKCCriticalSection g_soundLock;   // 0x0143BAFC')
edit('00c279a0', 'src', 'PKCProtectionInstance lock((PKCCriticalSection*)g_RenderQueueProfileSection_0143bafc);', 'PKCProtectionInstance lock(&g_soundLock);')
edit('00c279e0', 'src', 'extern const unsigned short g_emptyName_0143bafc[];   // 0x0143BAFC (a wide string; VC7.1 without /Zc:wchar_t)', 'extern PKCCriticalSection g_soundLock;   // 0x0143BAFC (the sound critical section, chunk 586)')
edit('00c279e0', 'src', 'PKCProtectionInstance lock((PKCCriticalSection*)g_emptyName_0143bafc);', 'PKCProtectionInstance lock(&g_soundLock);')
