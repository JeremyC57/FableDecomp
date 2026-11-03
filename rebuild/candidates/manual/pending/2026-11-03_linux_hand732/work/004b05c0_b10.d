004b05c0 CQuestManager::TransferBoasts DIFFER(1126v1126) flags= []
@@ retail 40-40 built 40-44
   004b0629  c644247800             mov byte ptr [esp + 0x78], 0
   004b062e  c644241200             mov byte ptr [esp + 0x12], 0
 + 004b0633  8b5608                 mov edx, dword ptr [esi + 8]
 + 004b0636  8a4e22                 mov cl, byte ptr [esi + 0x22]
 + 004b0639  8a4621                 mov al, byte ptr [esi + 0x21]
 + 004b063c  89542430               mov dword ptr [esp + 0x30], edx
   004b0633  8d5614                 lea edx, [esi + 0x14]
   004b0636  52                     push edx
@@ retail 41-41 built 45-46
   004b062e  c644241200             mov byte ptr [esp + 0x12], 0
   004b0633  8d5614                 lea edx, [esi + 0x14]
 + 004b0643  884c2413               mov byte ptr [esp + 0x13], cl
   004b0636  52                     push edx
   004b0637  8d4c2420               lea ecx, [esp + 0x20]
@@ retail 43-43 built 48-49
   004b0636  52                     push edx
   004b0637  8d4c2420               lea ecx, [esp + 0x20]
 + 004b064c  88442415               mov byte ptr [esp + 0x15], al
   004b063b  e8f0e54e00             call 0x99ec30
   004b0640  8b4608                 mov eax, dword ptr [esi + 8]
@@ retail 44-50 built 50-50
   004b0637  8d4c2420               lea ecx, [esp + 0x20]
   004b063b  e8f0e54e00             call 0x99ec30
 - 004b0640  8b4608                 mov eax, dword ptr [esi + 8]
 - 004b0643  89442430               mov dword ptr [esp + 0x30], eax
 - 004b0647  8a4e21                 mov cl, byte ptr [esi + 0x21]
 - 004b064a  884c2411               mov byte ptr [esp + 0x11], cl
 - 004b064e  8a5622                 mov dl, byte ptr [esi + 0x22]
 - 004b0651  88542413               mov byte ptr [esp + 0x13], dl
   004b0655  8bd3                   mov edx, ebx
   004b0657  8d4c2428               lea ecx, [esp + 0x28]
DIFFLINES 12 retail_instr 330 built_instr 330
