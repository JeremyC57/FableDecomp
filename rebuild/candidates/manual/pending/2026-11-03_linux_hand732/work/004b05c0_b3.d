004b05c0 CQuestManager::TransferBoasts DIFFER(1126v1126) flags= []
@@ retail 26-27 built 26-27
   004b05f7  8d442478               lea eax, [esp + 0x78]
   004b05fb  50                     push eax
 - 004b05fc  8d4c2428               lea ecx, [esp + 0x28]
 + 004b05fc  8d4c2418               lea ecx, [esp + 0x18]
   004b0600  51                     push ecx
   004b0601  6820942301             push 0x1239420
@@ retail 30-31 built 30-31
   004b0601  6820942301             push 0x1239420
   004b0606  8bcf                   mov ecx, edi
 - 004b0608  89542430               mov dword ptr [esp + 0x30], edx
 + 004b0608  89542420               mov dword ptr [esp + 0x20], edx
   004b060c  899c2484000000         mov dword ptr [esp + 0x84], ebx
   004b0613  e858870000             call 0x4b8d70
@@ retail 42-43 built 42-43
   004b0633  8d5614                 lea edx, [esi + 0x14]
   004b0636  52                     push edx
 - 004b0637  8d4c2420               lea ecx, [esp + 0x20]
 + 004b0637  8d4c2424               lea ecx, [esp + 0x24]
   004b063b  e8f0e54e00             call 0x99ec30
   004b0640  8b4608                 mov eax, dword ptr [esi + 8]
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
@@ retail 59-60 built 59-60
   004b066f  8bc8                   mov ecx, eax
   004b0671  e84ade4e00             call 0x99e4c0
 - 004b0676  8d4c2414               lea ecx, [esp + 0x14]
 + 004b0676  8d4c2418               lea ecx, [esp + 0x18]
   004b067a  8be8                   mov ebp, eax
   004b067c  e82fde4e00             call 0x99e4b0
@@ retail 62-63 built 62-63
   004b067a  8be8                   mov ebp, eax
   004b067c  e82fde4e00             call 0x99e4b0
 - 004b0681  8d442414               lea eax, [esp + 0x14]
 + 004b0681  8d442418               lea eax, [esp + 0x18]
   004b0685  50                     push eax
   004b0686  8d4c241c               lea ecx, [esp + 0x1c]
@@ retail 64-67 built 64-65
   004b0681  8d442414               lea eax, [esp + 0x14]
   004b0685  50                     push eax
 - 004b0686  8d4c241c               lea ecx, [esp + 0x1c]
 - 004b068a  e8a1e54e00             call 0x99ec30
 - 004b068f  8d4c2414               lea ecx, [esp + 0x14]
 + 004b0686  8d4c2420               lea ecx, [esp + 0x20]
   004b0693  e848e44e00             call 0x99eae0
   004b0698  8d4c2418               lea ecx, [esp + 0x18]
@@ retail 69-69 built 67-69
   004b0693  e848e44e00             call 0x99eae0
   004b0698  8d4c2418               lea ecx, [esp + 0x18]
 + 004b0693  e800000000             call 0x4b0698
 + 004b0698  8d4c241c               lea ecx, [esp + 0x1c]
   004b069c  51                     push ecx
   004b069d  8d542420               lea edx, [esp + 0x20]
@@ retail 70-71 built 70-71
   004b0698  8d4c2418               lea ecx, [esp + 0x18]
   004b069c  51                     push ecx
 - 004b069d  8d542420               lea edx, [esp + 0x20]
 + 004b069d  8d542424               lea edx, [esp + 0x24]
   004b06a1  52                     push edx
   004b06a2  55                     push ebp
@@ retail 75-76 built 75-76
   004b06a3  8bcf                   mov ecx, edi
   004b06a5  e8f602f6ff             call 0x4109a0
 - 004b06aa  8d4c2418               lea ecx, [esp + 0x18]
 + 004b06aa  8d4c241c               lea ecx, [esp + 0x1c]
   004b06ae  e82de44e00             call 0x99eae0
   004b06b3  8d4c2420               lea ecx, [esp + 0x20]
@@ retail 77-78 built 77-78
   004b06aa  8d4c2418               lea ecx, [esp + 0x18]
   004b06ae  e82de44e00             call 0x99eae0
 - 004b06b3  8d4c2420               lea ecx, [esp + 0x20]
 + 004b06b3  8d4c2424               lea ecx, [esp + 0x24]
   004b06b7  e824e44e00             call 0x99eae0
   004b06bc  8d4c2428               lea ecx, [esp + 0x28]
@@ retail 141-142 built 141-142
   004b078b  8d4c2448               lea ecx, [esp + 0x48]
   004b078f  e84ce34e00             call 0x99eae0
 - 004b0794  8d4c241c               lea ecx, [esp + 0x1c]
 + 004b0794  8d4c2420               lea ecx, [esp + 0x20]
   004b0798  43                     inc ebx
   004b0799  e842e34e00             call 0x99eae0
@@ retail 160-161 built 160-161
   004b07c5  8d4c2478               lea ecx, [esp + 0x78]
   004b07c9  51                     push ecx
 - 004b07ca  8d542428               lea edx, [esp + 0x28]
 + 004b07ca  8d542418               lea edx, [esp + 0x18]
   004b07ce  52                     push edx
   004b07cf  33f6                   xor esi, esi
@@ retail 167-168 built 167-168
   004b07d8  89b42484000000         mov dword ptr [esp + 0x84], esi
   004b07df  e88c850000             call 0x4b8d70
 - 004b07e4  39742424               cmp dword ptr [esp + 0x24], esi
 + 004b07e4  39742414               cmp dword ptr [esp + 0x14], esi
   004b07e8  0f842e020000           je 0x4b0a1c
   004b07ee  89742440               mov dword ptr [esp + 0x40], esi
@@ retail 174-175 built 174-175
   004b07fc  eb04                   jmp 0x4b0802
   004b07fe  8b5c244c               mov ebx, dword ptr [esp + 0x4c]
 - 004b0802  8d4c2414               lea ecx, [esp + 0x14]
 + 004b0802  8d4c2418               lea ecx, [esp + 0x18]
   004b0806  e8a5dc4e00             call 0x99e4b0
   004b080b  8bd6                   mov edx, esi
@@ retail 185-186 built 185-186
   004b0825  8bc8                   mov ecx, eax
   004b0827  e894dc4e00             call 0x99e4c0
 - 004b082c  8d4c241c               lea ecx, [esp + 0x1c]
 + 004b082c  8d4c2420               lea ecx, [esp + 0x20]
   004b0830  8be8                   mov ebp, eax
   004b0832  e879dc4e00             call 0x99e4b0
@@ retail 188-189 built 188-189
   004b0830  8be8                   mov ebp, eax
   004b0832  e879dc4e00             call 0x99e4b0
 - 004b0837  8d44241c               lea eax, [esp + 0x1c]
 + 004b0837  8d442420               lea eax, [esp + 0x20]
   004b083b  50                     push eax
   004b083c  8d4c241c               lea ecx, [esp + 0x1c]
@@ retail 190-191 built 190-193
   004b0837  8d44241c               lea eax, [esp + 0x1c]
   004b083b  50                     push eax
 - 004b083c  8d4c241c               lea ecx, [esp + 0x1c]
 + 004b083c  8d4c2420               lea ecx, [esp + 0x20]
 + 004b0840  e800000000             call 0x4b0845
 + 004b0845  8d4c2420               lea ecx, [esp + 0x20]
   004b0840  e8ebe34e00             call 0x99ec30
   004b0845  8d4c241c               lea ecx, [esp + 0x1c]
@@ retail 193-195 built 195-195
   004b0840  e8ebe34e00             call 0x99ec30
   004b0845  8d4c241c               lea ecx, [esp + 0x1c]
 - 004b0849  e892e24e00             call 0x99eae0
 - 004b084e  8d4c2418               lea ecx, [esp + 0x18]
   004b0852  51                     push ecx
   004b0853  8d542418               lea edx, [esp + 0x18]
@@ retail 196-197 built 196-197
   004b084e  8d4c2418               lea ecx, [esp + 0x18]
   004b0852  51                     push ecx
 - 004b0853  8d542418               lea edx, [esp + 0x18]
 + 004b0853  8d54241c               lea edx, [esp + 0x1c]
   004b0857  52                     push edx
   004b0858  55                     push ebp
@@ retail 201-202 built 201-202
   004b0859  8bcf                   mov ecx, edi
   004b085b  e84001f6ff             call 0x4109a0
 - 004b0860  8d4c2418               lea ecx, [esp + 0x18]
 + 004b0860  8d4c241c               lea ecx, [esp + 0x1c]
   004b0864  e877e24e00             call 0x99eae0
   004b0869  8d4c2448               lea ecx, [esp + 0x48]
@@ retail 273-274 built 273-274
   004b095c  8d4c2468               lea ecx, [esp + 0x68]
   004b0960  e86ba54e00             call 0x99aed0
 - 004b0965  8d442414               lea eax, [esp + 0x14]
 + 004b0965  8d442418               lea eax, [esp + 0x18]
   004b0969  50                     push eax
   004b096a  8d4c2424               lea ecx, [esp + 0x24]
@@ retail 275-276 built 275-276
   004b0965  8d442414               lea eax, [esp + 0x14]
   004b0969  50                     push eax
 - 004b096a  8d4c2424               lea ecx, [esp + 0x24]
 + 004b096a  8d4c2428               lea ecx, [esp + 0x28]
   004b096e  c644247100             mov byte ptr [esp + 0x71], 0
   004b0973  c644247200             mov byte ptr [esp + 0x72], 0
@@ retail 280-281 built 280-281
   004b0978  c744247400000000       mov dword ptr [esp + 0x74], 0
   004b0980  e8abe24e00             call 0x99ec30
 - 004b0985  8d4c2420               lea ecx, [esp + 0x20]
 + 004b0985  8d4c2424               lea ecx, [esp + 0x24]
   004b0989  51                     push ecx
   004b098a  8d4c2464               lea ecx, [esp + 0x64]
@@ retail 284-285 built 284-285
   004b098a  8d4c2464               lea ecx, [esp + 0x64]
   004b098e  e81de64e00             call 0x99efb0
 - 004b0993  8d4c2420               lea ecx, [esp + 0x20]
 + 004b0993  8d4c2424               lea ecx, [esp + 0x24]
   004b0997  e844e14e00             call 0x99eae0
   004b099c  8a442412               mov al, byte ptr [esp + 0x12]
@@ retail 318-319 built 318-319
   004b09fd  8d4c2460               lea ecx, [esp + 0x60]
   004b0a01  e8dae04e00             call 0x99eae0
 - 004b0a06  8d4c2414               lea ecx, [esp + 0x14]
 + 004b0a06  8d4c2418               lea ecx, [esp + 0x18]
   004b0a0a  e8d1e04e00             call 0x99eae0
   004b0a0f  8b442424               mov eax, dword ptr [esp + 0x24]
@@ retail 320-321 built 320-321
   004b0a06  8d4c2414               lea ecx, [esp + 0x14]
   004b0a0a  e8d1e04e00             call 0x99eae0
 - 004b0a0f  8b442424               mov eax, dword ptr [esp + 0x24]
 + 004b0a0f  8b442414               mov eax, dword ptr [esp + 0x14]
   004b0a13  46                     inc esi
   004b0a14  3bf0                   cmp esi, eax
DIFFLINES 39 retail_instr 330 built_instr 330
