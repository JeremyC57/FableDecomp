004b05c0 CQuestManager::TransferBoasts DIFFER(1122v1126) flags= []
@@ retail 26-27 built 26-27
   004b05f7  8d442478               lea eax, [esp + 0x78]
   004b05fb  50                     push eax
 - 004b05fc  8d4c2428               lea ecx, [esp + 0x28]
 + 004b05fc  8d4c2424               lea ecx, [esp + 0x24]
   004b0600  51                     push ecx
   004b0601  6820942301             push 0x1239420
@@ retail 30-31 built 30-31
   004b0601  6820942301             push 0x1239420
   004b0606  8bcf                   mov ecx, edi
 - 004b0608  89542430               mov dword ptr [esp + 0x30], edx
 + 004b0608  8954242c               mov dword ptr [esp + 0x2c], edx
   004b060c  899c2484000000         mov dword ptr [esp + 0x84], ebx
   004b0613  e858870000             call 0x4b8d70
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
@@ retail 55-56 built 55-56
   004b0660  50                     push eax
   004b0661  ba10942301             mov edx, 0x1239410
 - 004b0666  8d4c2424               lea ecx, [esp + 0x24]
 + 004b0666  8d4c2428               lea ecx, [esp + 0x28]
   004b066a  e821f04e00             call 0x99f690
   004b066f  8bc8                   mov ecx, eax
@@ retail 77-78 built 77-78
   004b06aa  8d4c2418               lea ecx, [esp + 0x18]
   004b06ae  e82de44e00             call 0x99eae0
 - 004b06b3  8d4c2420               lea ecx, [esp + 0x20]
 + 004b06b3  8d4c2424               lea ecx, [esp + 0x24]
   004b06b7  e824e44e00             call 0x99eae0
   004b06bc  8d4c2428               lea ecx, [esp + 0x28]
@@ retail 160-161 built 160-161
   004b07c5  8d4c2478               lea ecx, [esp + 0x78]
   004b07c9  51                     push ecx
 - 004b07ca  8d542428               lea edx, [esp + 0x28]
 + 004b07ca  8d542424               lea edx, [esp + 0x24]
   004b07ce  52                     push edx
   004b07cf  33f6                   xor esi, esi
@@ retail 167-168 built 167-168
   004b07d8  89b42484000000         mov dword ptr [esp + 0x84], esi
   004b07df  e88c850000             call 0x4b8d70
 - 004b07e4  39742424               cmp dword ptr [esp + 0x24], esi
 + 004b07e4  39742420               cmp dword ptr [esp + 0x20], esi
   004b07e8  0f842e020000           je 0x4b0a1c
   004b07ee  89742440               mov dword ptr [esp + 0x40], esi
@@ retail 275-276 built 275-276
   004b0965  8d442414               lea eax, [esp + 0x14]
   004b0969  50                     push eax
 - 004b096a  8d4c2424               lea ecx, [esp + 0x24]
 + 004b096a  8d4c2428               lea ecx, [esp + 0x28]
   004b096e  c644247100             mov byte ptr [esp + 0x71], 0
   004b0973  c644247200             mov byte ptr [esp + 0x72], 0
@@ retail 280-282 built 280-281
   004b0978  c744247400000000       mov dword ptr [esp + 0x74], 0
   004b0980  e8abe24e00             call 0x99ec30
 - 004b0985  8d4c2420               lea ecx, [esp + 0x20]
 - 004b0989  51                     push ecx
 + 004b0985  50                     push eax
   004b098a  8d4c2464               lea ecx, [esp + 0x64]
   004b098e  e81de64e00             call 0x99efb0
@@ retail 284-285 built 283-284
   004b098a  8d4c2464               lea ecx, [esp + 0x64]
   004b098e  e81de64e00             call 0x99efb0
 - 004b0993  8d4c2420               lea ecx, [esp + 0x20]
 + 004b098f  8d4c2424               lea ecx, [esp + 0x24]
   004b0997  e844e14e00             call 0x99eae0
   004b099c  8a442412               mov al, byte ptr [esp + 0x12]
@@ retail 288-290 built 287-289
   004b099c  8a442412               mov al, byte ptr [esp + 0x12]
   004b09a0  84c0                   test al, al
 - 004b09a2  8b542450               mov edx, dword ptr [esp + 0x50]
 - 004b09a6  89542454               mov dword ptr [esp + 0x54], edx
 + 004b099e  8b4c2450               mov ecx, dword ptr [esp + 0x50]
 + 004b09a2  894c2454               mov dword ptr [esp + 0x54], ecx
   004b09aa  7405                   je 0x4b09b1
   004b09ac  c644246d01             mov byte ptr [esp + 0x6d], 1
@@ retail 304-306 built 303-305
   004b09cf  85c9                   test ecx, ecx
   004b09d1  740a                   je 0x4b09dd
 - 004b09d3  8d442454               lea eax, [esp + 0x54]
 - 004b09d7  50                     push eax
 + 004b09cf  8d542454               lea edx, [esp + 0x54]
 + 004b09d3  52                     push edx
   004b09d8  e8c3690000             call 0x4b73a0
   004b09dd  8b4304                 mov eax, dword ptr [ebx + 4]
@@ retail 320-321 built 319-320
   004b0a06  8d4c2414               lea ecx, [esp + 0x14]
   004b0a0a  e8d1e04e00             call 0x99eae0
 - 004b0a0f  8b442424               mov eax, dword ptr [esp + 0x24]
 + 004b0a0b  8b442420               mov eax, dword ptr [esp + 0x20]
   004b0a13  46                     inc esi
   004b0a14  3bf0                   cmp esi, eax
DIFFLINES 21 retail_instr 330 built_instr 329
