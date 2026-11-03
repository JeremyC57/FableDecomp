004b05c0 CQuestManager::TransferBoasts DIFFER(1126v1126) flags= []
@@ retail 44-44 built 44-46
   004b0637  8d4c2420               lea ecx, [esp + 0x20]
   004b063b  e8f0e54e00             call 0x99ec30
 + 004b0640  8a4e21                 mov cl, byte ptr [esi + 0x21]
 + 004b0643  8a5622                 mov dl, byte ptr [esi + 0x22]
   004b0640  8b4608                 mov eax, dword ptr [esi + 8]
   004b0643  89442430               mov dword ptr [esp + 0x30], eax
@@ retail 45-47 built 47-47
   004b063b  e8f0e54e00             call 0x99ec30
   004b0640  8b4608                 mov eax, dword ptr [esi + 8]
 - 004b0643  89442430               mov dword ptr [esp + 0x30], eax
 - 004b0647  8a4e21                 mov cl, byte ptr [esi + 0x21]
   004b064a  884c2411               mov byte ptr [esp + 0x11], cl
   004b064e  8a5622                 mov dl, byte ptr [esi + 0x22]
@@ retail 48-49 built 48-48
   004b0647  8a4e21                 mov cl, byte ptr [esi + 0x21]
   004b064a  884c2411               mov byte ptr [esp + 0x11], cl
 - 004b064e  8a5622                 mov dl, byte ptr [esi + 0x22]
   004b0651  88542413               mov byte ptr [esp + 0x13], dl
   004b0655  8bd3                   mov edx, ebx
@@ retail 52-52 built 51-52
   004b0655  8bd3                   mov edx, ebx
   004b0657  8d4c2428               lea ecx, [esp + 0x28]
 + 004b0657  89442430               mov dword ptr [esp + 0x30], eax
   004b065b  e8d0f14e00             call 0x99f830
   004b0660  50                     push eax
DIFFLINES 6 retail_instr 330 built_instr 330
