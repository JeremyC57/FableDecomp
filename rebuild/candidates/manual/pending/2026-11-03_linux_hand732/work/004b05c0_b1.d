004b05c0 CQuestManager::TransferBoasts DIFFER(1051v1126) flags= []
@@ retail 0-1 built 0-1
 - 004b05c0  83ec64                 sub esp, 0x64
 + 004b05c0  83ec5c                 sub esp, 0x5c
   004b05c3  53                     push ebx
   004b05c4  55                     push ebp
@@ retail 5-6 built 5-6
   004b05c5  56                     push esi
   004b05c6  57                     push edi
 - 004b05c7  8b7c2478               mov edi, dword ptr [esp + 0x78]
 + 004b05c7  8b7c2470               mov edi, dword ptr [esp + 0x70]
   004b05cb  8b4718                 mov eax, dword ptr [edi + 0x18]
   004b05ce  83f801                 cmp eax, 1
@@ retail 9-10 built 9-10
   004b05ce  83f801                 cmp eax, 1
   004b05d1  8bf1                   mov esi, ecx
 - 004b05d3  8974244c               mov dword ptr [esp + 0x4c], esi
 + 004b05d3  89742448               mov dword ptr [esp + 0x48], esi
   004b05d7  7409                   je 0x4b05e2
   004b05d9  83f803                 cmp eax, 3
@@ retail 24-25 built 24-25
   004b05f3  3bc1                   cmp eax, ecx
   004b05f5  75f9                   jne 0x4b05f0
 - 004b05f7  8d442478               lea eax, [esp + 0x78]
 + 004b05f7  8d442470               lea eax, [esp + 0x70]
   004b05fb  50                     push eax
   004b05fc  8d4c2428               lea ecx, [esp + 0x28]
@@ retail 26-27 built 26-27
   004b05f7  8d442478               lea eax, [esp + 0x78]
   004b05fb  50                     push eax
 - 004b05fc  8d4c2428               lea ecx, [esp + 0x28]
 + 004b05fc  8d4c2420               lea ecx, [esp + 0x20]
   004b0600  51                     push ecx
   004b0601  6820942301             push 0x1239420
@@ retail 30-32 built 30-32
   004b0601  6820942301             push 0x1239420
   004b0606  8bcf                   mov ecx, edi
 - 004b0608  89542430               mov dword ptr [esp + 0x30], edx
 - 004b060c  899c2484000000         mov dword ptr [esp + 0x84], ebx
 + 004b0608  89542428               mov dword ptr [esp + 0x28], edx
 + 004b060c  895c247c               mov dword ptr [esp + 0x7c], ebx
   004b0613  e858870000             call 0x4b8d70
   004b0618  8b466c                 mov eax, dword ptr [esi + 0x6c]
@@ retail 37-39 built 37-39
   004b061d  3bf0                   cmp esi, eax
   004b061f  0f84f7030000           je 0x4b0a1c
 - 004b0625  895c242c               mov dword ptr [esp + 0x2c], ebx
 - 004b0629  c644247800             mov byte ptr [esp + 0x78], 0
 + 004b0622  895c2428               mov dword ptr [esp + 0x28], ebx
 + 004b0626  c644247000             mov byte ptr [esp + 0x70], 0
   004b062e  c644241200             mov byte ptr [esp + 0x12], 0
   004b0633  8d5614                 lea edx, [esi + 0x14]
@@ retail 42-43 built 42-43
   004b0633  8d5614                 lea edx, [esi + 0x14]
   004b0636  52                     push edx
 - 004b0637  8d4c2420               lea ecx, [esp + 0x20]
 + 004b0634  8d4c241c               lea ecx, [esp + 0x1c]
   004b063b  e8f0e54e00             call 0x99ec30
   004b0640  8b4608                 mov eax, dword ptr [esi + 8]
@@ retail 44-44 built 44-46
   004b0637  8d4c2420               lea ecx, [esp + 0x20]
   004b063b  e8f0e54e00             call 0x99ec30
 + 004b063d  8a4e21                 mov cl, byte ptr [esi + 0x21]
 + 004b0640  8a5622                 mov dl, byte ptr [esi + 0x22]
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
@@ retail 51-52 built 50-52
   004b0651  88542413               mov byte ptr [esp + 0x13], dl
   004b0655  8bd3                   mov edx, ebx
 - 004b0657  8d4c2428               lea ecx, [esp + 0x28]
 + 004b0650  8d4c2424               lea ecx, [esp + 0x24]
 + 004b0654  8944242c               mov dword ptr [esp + 0x2c], eax
   004b065b  e8d0f14e00             call 0x99f830
   004b0660  50                     push eax
@@ retail 65-65 built 65-68
   004b0685  50                     push eax
   004b0686  8d4c241c               lea ecx, [esp + 0x1c]
 + 004b0687  51                     push ecx
 + 004b0688  55                     push ebp
 + 004b0689  8bcf                   mov ecx, edi
   004b068a  e8a1e54e00             call 0x99ec30
   004b068f  8d4c2414               lea ecx, [esp + 0x14]
@@ retail 68-77 built 71-71
   004b068f  8d4c2414               lea ecx, [esp + 0x14]
   004b0693  e848e44e00             call 0x99eae0
 - 004b0698  8d4c2418               lea ecx, [esp + 0x18]
 - 004b069c  51                     push ecx
 - 004b069d  8d542420               lea edx, [esp + 0x20]
 - 004b06a1  52                     push edx
 - 004b06a2  55                     push ebp
 - 004b06a3  8bcf                   mov ecx, edi
 - 004b06a5  e8f602f6ff             call 0x4109a0
 - 004b06aa  8d4c2418               lea ecx, [esp + 0x18]
 - 004b06ae  e82de44e00             call 0x99eae0
   004b06b3  8d4c2420               lea ecx, [esp + 0x20]
   004b06b7  e824e44e00             call 0x99eae0
@@ retail 79-80 built 73-74
   004b06b3  8d4c2420               lea ecx, [esp + 0x20]
   004b06b7  e824e44e00             call 0x99eae0
 - 004b06bc  8d4c2428               lea ecx, [esp + 0x28]
 + 004b06a2  8d4c2424               lea ecx, [esp + 0x24]
   004b06c0  e81be44e00             call 0x99eae0
   004b06c5  8bd3                   mov edx, ebx
@@ retail 82-83 built 76-77
   004b06c0  e81be44e00             call 0x99eae0
   004b06c5  8bd3                   mov edx, ebx
 - 004b06c7  8d4c2438               lea ecx, [esp + 0x38]
 + 004b06ad  8d4c2434               lea ecx, [esp + 0x34]
   004b06cb  e860f14e00             call 0x99f830
   004b06d0  50                     push eax
@@ retail 86-87 built 80-81
   004b06d0  50                     push eax
   004b06d1  ba08942301             mov edx, 0x1239408
 - 004b06d6  8d4c2438               lea ecx, [esp + 0x38]
 + 004b06bc  8d4c2434               lea ecx, [esp + 0x34]
   004b06da  e8b1ef4e00             call 0x99f690
   004b06df  8bc8                   mov ecx, eax
@@ retail 90-91 built 84-87
   004b06df  8bc8                   mov ecx, eax
   004b06e1  e8dadd4e00             call 0x99e4c0
 - 004b06e6  8d4c242c               lea ecx, [esp + 0x2c]
 + 004b06cc  8d542428               lea edx, [esp + 0x28]
 + 004b06d0  52                     push edx
 + 004b06d1  8d4c2430               lea ecx, [esp + 0x30]
   004b06ea  51                     push ecx
   004b06eb  8d542434               lea edx, [esp + 0x34]
@@ retail 92-94 built 88-88
   004b06e6  8d4c242c               lea ecx, [esp + 0x2c]
   004b06ea  51                     push ecx
 - 004b06eb  8d542434               lea edx, [esp + 0x34]
 - 004b06ef  52                     push edx
   004b06f0  50                     push eax
   004b06f1  8bcf                   mov ecx, edi
@@ retail 97-97 built 91-93
   004b06f1  8bcf                   mov ecx, edi
   004b06f3  e8e804f6ff             call 0x410be0
 + 004b06de  8d4c2430               lea ecx, [esp + 0x30]
 + 004b06e2  e800000000             call 0x4b06e7
   004b06f8  8d4c2434               lea ecx, [esp + 0x34]
   004b06fc  e8dfe34e00             call 0x99eae0
@@ retail 98-98 built 94-110
   004b06f3  e8e804f6ff             call 0x410be0
   004b06f8  8d4c2434               lea ecx, [esp + 0x34]
 + 004b06eb  e800000000             call 0x4b06f0
 + 004b06f0  8bd3                   mov edx, ebx
 + 004b06f2  8d4c243c               lea ecx, [esp + 0x3c]
 + 004b06f6  e800000000             call 0x4b06fb
 + 004b06fb  50                     push eax
 + 004b06fc  ba00000000             mov edx, 0
 + 004b0701  8d4c243c               lea ecx, [esp + 0x3c]
 + 004b0705  e800000000             call 0x4b070a
 + 004b070a  8bc8                   mov ecx, eax
 + 004b070c  e800000000             call 0x4b0711
 + 004b0711  8d542470               lea edx, [esp + 0x70]
 + 004b0715  52                     push edx
 + 004b0716  8d4c2415               lea ecx, [esp + 0x15]
 + 004b071a  51                     push ecx
 + 004b071b  50                     push eax
 + 004b071c  8bcf                   mov ecx, edi
   004b06fc  e8dfe34e00             call 0x99eae0
   004b0701  8d4c2438               lea ecx, [esp + 0x38]
@@ retail 101-101 built 113-115
   004b0701  8d4c2438               lea ecx, [esp + 0x38]
   004b0705  e8d6e34e00             call 0x99eae0
 + 004b072c  8d4c243c               lea ecx, [esp + 0x3c]
 + 004b0730  e800000000             call 0x4b0735
   004b070a  8bd3                   mov edx, ebx
   004b070c  8d4c2440               lea ecx, [esp + 0x40]
@@ retail 102-103 built 116-117
   004b0705  e8d6e34e00             call 0x99eae0
   004b070a  8bd3                   mov edx, ebx
 - 004b070c  8d4c2440               lea ecx, [esp + 0x40]
 + 004b0737  8d4c2444               lea ecx, [esp + 0x44]
   004b0710  e81bf14e00             call 0x99f830
   004b0715  50                     push eax
@@ retail 106-107 built 120-121
   004b0715  50                     push eax
   004b0716  bafc932301             mov edx, 0x12393fc
 - 004b071b  8d4c2440               lea ecx, [esp + 0x40]
 + 004b0746  8d4c2444               lea ecx, [esp + 0x44]
   004b071f  e86cef4e00             call 0x99f690
   004b0724  8bc8                   mov ecx, eax
@@ retail 110-111 built 124-127
   004b0724  8bc8                   mov ecx, eax
   004b0726  e895dd4e00             call 0x99e4c0
 - 004b072b  8d4c2478               lea ecx, [esp + 0x78]
 + 004b0756  8d542412               lea edx, [esp + 0x12]
 + 004b075a  52                     push edx
 + 004b075b  8d4c2417               lea ecx, [esp + 0x17]
   004b072f  51                     push ecx
   004b0730  8d542415               lea edx, [esp + 0x15]
@@ retail 112-114 built 128-128
   004b072b  8d4c2478               lea ecx, [esp + 0x78]
   004b072f  51                     push ecx
 - 004b0730  8d542415               lea edx, [esp + 0x15]
 - 004b0734  52                     push edx
   004b0735  50                     push eax
   004b0736  8bcf                   mov ecx, edi
@@ retail 117-119 built 131-131
   004b0736  8bcf                   mov ecx, edi
   004b0738  e8833ef5ff             call 0x4045c0
 - 004b073d  8d4c243c               lea ecx, [esp + 0x3c]
 - 004b0741  e89ae34e00             call 0x99eae0
   004b0746  8d4c2440               lea ecx, [esp + 0x40]
   004b074a  e891e34e00             call 0x99eae0
@@ retail 121-123 built 133-172
   004b0746  8d4c2440               lea ecx, [esp + 0x40]
   004b074a  e891e34e00             call 0x99eae0
 - 004b074f  8bd3                   mov edx, ebx
 - 004b0751  8d4c2448               lea ecx, [esp + 0x48]
 + 004b0771  8d4c2444               lea ecx, [esp + 0x44]
 + 004b0775  e800000000             call 0x4b077a
 + 004b077a  8d4c2418               lea ecx, [esp + 0x18]
 + 004b077e  43                     inc ebx
 + 004b077f  e800000000             call 0x4b0784
 + 004b0784  8b542448               mov edx, dword ptr [esp + 0x48]
 + 004b0788  8b36                   mov esi, dword ptr [esi]
 + 004b078a  3b726c                 cmp esi, dword ptr [edx + 0x6c]
 + 004b078d  0f859dfeffff           jne 0x4b0630
 + 004b0793  5f                     pop edi
 + 004b0794  5e                     pop esi
 + 004b0795  5d                     pop ebp
 + 004b0796  5b                     pop ebx
 + 004b0797  83c45c                 add esp, 0x5c
 + 004b079a  c20400                 ret 4
 + 004b079d  8d5e70                 lea ebx, [esi + 0x70]
 + 004b07a0  8bcb                   mov ecx, ebx
 + 004b07a2  895c2448               mov dword ptr [esp + 0x48], ebx
 + 004b07a6  e800000000             call 0x4b07ab
 + 004b07ab  8d442470               lea eax, [esp + 0x70]
 + 004b07af  50                     push eax
 + 004b07b0  8d4c2420               lea ecx, [esp + 0x20]
 + 004b07b4  51                     push ecx
 + 004b07b5  33f6                   xor esi, esi
 + 004b07b7  6800000000             push 0
 + 004b07bc  8bcf                   mov ecx, edi
 + 004b07be  8974247c               mov dword ptr [esp + 0x7c], esi
 + 004b07c2  e800000000             call 0x4b07c7
 + 004b07c7  3974241c               cmp dword ptr [esp + 0x1c], esi
 + 004b07cb  0f8400020000           je 0x4b09d1
 + 004b07d1  8974243c               mov dword ptr [esp + 0x3c], esi
 + 004b07d5  c644247000             mov byte ptr [esp + 0x70], 0
 + 004b07da  c644241300             mov byte ptr [esp + 0x13], 0
 + 004b07df  eb04                   jmp 0x4b07e5
 + 004b07e1  8b5c2448               mov ebx, dword ptr [esp + 0x48]
 + 004b07e5  8d4c2414               lea ecx, [esp + 0x14]
 + 004b07e9  e800000000             call 0x4b07ee
 + 004b07ee  8bd6                   mov edx, esi
 + 004b07f0  8d4c2440               lea ecx, [esp + 0x40]
   004b0755  e8d6f04e00             call 0x99f830
   004b075a  50                     push eax
@@ retail 130-186 built 179-180
   004b0769  8bc8                   mov ecx, eax
   004b076b  e850dd4e00             call 0x99e4c0
 - 004b0770  8d4c2412               lea ecx, [esp + 0x12]
 - 004b0774  51                     push ecx
 - 004b0775  8d542417               lea edx, [esp + 0x17]
 - 004b0779  52                     push edx
 - 004b077a  50                     push eax
 - 004b077b  8bcf                   mov ecx, edi
 - 004b077d  e83e3ef5ff             call 0x4045c0
 - 004b0782  8d4c2444               lea ecx, [esp + 0x44]
 - 004b0786  e855e34e00             call 0x99eae0
 - 004b078b  8d4c2448               lea ecx, [esp + 0x48]
 - 004b078f  e84ce34e00             call 0x99eae0
 - 004b0794  8d4c241c               lea ecx, [esp + 0x1c]
 - 004b0798  43                     inc ebx
 - 004b0799  e842e34e00             call 0x99eae0
 - 004b079e  8b44244c               mov eax, dword ptr [esp + 0x4c]
 - 004b07a2  8b36                   mov esi, dword ptr [esi]
 - 004b07a4  3b706c                 cmp esi, dword ptr [eax + 0x6c]
 - 004b07a7  0f8586feffff           jne 0x4b0633
 - 004b07ad  5f                     pop edi
 - 004b07ae  5e                     pop esi
 - 004b07af  5d                     pop ebp
 - 004b07b0  5b                     pop ebx
 - 004b07b1  83c464                 add esp, 0x64
 - 004b07b4  c20400                 ret 4
 - 004b07b7  8d5e70                 lea ebx, [esi + 0x70]
 - 004b07ba  8bcb                   mov ecx, ebx
 - 004b07bc  895c244c               mov dword ptr [esp + 0x4c], ebx
 - 004b07c0  e83b840000             call 0x4b8c00
 - 004b07c5  8d4c2478               lea ecx, [esp + 0x78]
 - 004b07c9  51                     push ecx
 - 004b07ca  8d542428               lea edx, [esp + 0x28]
 - 004b07ce  52                     push edx
 - 004b07cf  33f6                   xor esi, esi
 - 004b07d1  6820942301             push 0x1239420
 - 004b07d6  8bcf                   mov ecx, edi
 - 004b07d8  89b42484000000         mov dword ptr [esp + 0x84], esi
 - 004b07df  e88c850000             call 0x4b8d70
 - 004b07e4  39742424               cmp dword ptr [esp + 0x24], esi
 - 004b07e8  0f842e020000           je 0x4b0a1c
 - 004b07ee  89742440               mov dword ptr [esp + 0x40], esi
 - 004b07f2  c644247800             mov byte ptr [esp + 0x78], 0
 - 004b07f7  c644241300             mov byte ptr [esp + 0x13], 0
 - 004b07fc  eb04                   jmp 0x4b0802
 - 004b07fe  8b5c244c               mov ebx, dword ptr [esp + 0x4c]
 - 004b0802  8d4c2414               lea ecx, [esp + 0x14]
 - 004b0806  e8a5dc4e00             call 0x99e4b0
 - 004b080b  8bd6                   mov edx, esi
 - 004b080d  8d4c2444               lea ecx, [esp + 0x44]
 - 004b0811  e81af04e00             call 0x99f830
 - 004b0816  50                     push eax
 - 004b0817  ba10942301             mov edx, 0x1239410
 - 004b081c  8d4c244c               lea ecx, [esp + 0x4c]
 - 004b0820  e86bee4e00             call 0x99f690
 - 004b0825  8bc8                   mov ecx, eax
 - 004b0827  e894dc4e00             call 0x99e4c0
 - 004b082c  8d4c241c               lea ecx, [esp + 0x1c]
 + 004b080f  8d4c2418               lea ecx, [esp + 0x18]
   004b0830  8be8                   mov ebp, eax
   004b0832  e879dc4e00             call 0x99e4b0
@@ retail 188-196 built 182-182
   004b0830  8be8                   mov ebp, eax
   004b0832  e879dc4e00             call 0x99e4b0
 - 004b0837  8d44241c               lea eax, [esp + 0x1c]
 - 004b083b  50                     push eax
 - 004b083c  8d4c241c               lea ecx, [esp + 0x1c]
 - 004b0840  e8ebe34e00             call 0x99ec30
 - 004b0845  8d4c241c               lea ecx, [esp + 0x1c]
 - 004b0849  e892e24e00             call 0x99eae0
 - 004b084e  8d4c2418               lea ecx, [esp + 0x18]
 - 004b0852  51                     push ecx
   004b0853  8d542418               lea edx, [esp + 0x18]
   004b0857  52                     push edx
@@ retail 198-198 built 184-186
   004b0853  8d542418               lea edx, [esp + 0x18]
   004b0857  52                     push edx
 + 004b081f  8d442418               lea eax, [esp + 0x18]
 + 004b0823  50                     push eax
   004b0858  55                     push ebp
   004b0859  8bcf                   mov ecx, edi
@@ retail 203-205 built 191-191
   004b0860  8d4c2418               lea ecx, [esp + 0x18]
   004b0864  e877e24e00             call 0x99eae0
 - 004b0869  8d4c2448               lea ecx, [esp + 0x48]
 - 004b086d  e86ee24e00             call 0x99eae0
   004b0872  8d4c2444               lea ecx, [esp + 0x44]
   004b0876  e865e24e00             call 0x99eae0
@@ retail 207-207 built 193-195
   004b0872  8d4c2444               lea ecx, [esp + 0x44]
   004b0876  e865e24e00             call 0x99eae0
 + 004b083e  8d4c2440               lea ecx, [esp + 0x40]
 + 004b0842  e800000000             call 0x4b0847
   004b087b  8bd6                   mov edx, esi
   004b087d  8d4c2438               lea ecx, [esp + 0x38]
@@ retail 208-209 built 196-197
   004b0876  e865e24e00             call 0x99eae0
   004b087b  8bd6                   mov edx, esi
 - 004b087d  8d4c2438               lea ecx, [esp + 0x38]
 + 004b0849  8d4c2434               lea ecx, [esp + 0x34]
   004b0881  e8aaef4e00             call 0x99f830
   004b0886  50                     push eax
@@ retail 212-213 built 200-201
   004b0886  50                     push eax
   004b0887  ba08942301             mov edx, 0x1239408
 - 004b088c  8d4c2440               lea ecx, [esp + 0x40]
 + 004b0858  8d4c243c               lea ecx, [esp + 0x3c]
   004b0890  e8fbed4e00             call 0x99f690
   004b0895  8bc8                   mov ecx, eax
@@ retail 216-217 built 204-205
   004b0895  8bc8                   mov ecx, eax
   004b0897  e824dc4e00             call 0x99e4c0
 - 004b089c  8d4c2440               lea ecx, [esp + 0x40]
 + 004b0868  8d4c243c               lea ecx, [esp + 0x3c]
   004b08a0  51                     push ecx
   004b08a1  8d542454               lea edx, [esp + 0x54]
@@ retail 218-219 built 206-207
   004b089c  8d4c2440               lea ecx, [esp + 0x40]
   004b08a0  51                     push ecx
 - 004b08a1  8d542454               lea edx, [esp + 0x54]
 + 004b086d  8d542424               lea edx, [esp + 0x24]
   004b08a5  52                     push edx
   004b08a6  50                     push eax
@@ retail 223-225 built 211-211
   004b08a7  8bcf                   mov ecx, edi
   004b08a9  e83203f6ff             call 0x410be0
 - 004b08ae  8d4c243c               lea ecx, [esp + 0x3c]
 - 004b08b2  e829e24e00             call 0x99eae0
   004b08b7  8d4c2438               lea ecx, [esp + 0x38]
   004b08bb  e820e24e00             call 0x99eae0
@@ retail 227-227 built 213-215
   004b08b7  8d4c2438               lea ecx, [esp + 0x38]
   004b08bb  e820e24e00             call 0x99eae0
 + 004b0883  8d4c2434               lea ecx, [esp + 0x34]
 + 004b0887  e800000000             call 0x4b088c
   004b08c0  8bd6                   mov edx, esi
   004b08c2  8d4c2430               lea ecx, [esp + 0x30]
@@ retail 228-229 built 216-217
   004b08bb  e820e24e00             call 0x99eae0
   004b08c0  8bd6                   mov edx, esi
 - 004b08c2  8d4c2430               lea ecx, [esp + 0x30]
 + 004b088e  8d4c242c               lea ecx, [esp + 0x2c]
   004b08c6  e865ef4e00             call 0x99f830
   004b08cb  50                     push eax
@@ retail 232-233 built 220-221
   004b08cb  50                     push eax
   004b08cc  bafc932301             mov edx, 0x12393fc
 - 004b08d1  8d4c2438               lea ecx, [esp + 0x38]
 + 004b089d  8d4c2434               lea ecx, [esp + 0x34]
   004b08d5  e8b6ed4e00             call 0x99f690
   004b08da  8bc8                   mov ecx, eax
@@ retail 236-237 built 224-225
   004b08da  8bc8                   mov ecx, eax
   004b08dc  e8dfdb4e00             call 0x99e4c0
 - 004b08e1  8d4c2478               lea ecx, [esp + 0x78]
 + 004b08ad  8d4c2470               lea ecx, [esp + 0x70]
   004b08e5  51                     push ecx
   004b08e6  8d542416               lea edx, [esp + 0x16]
@@ retail 243-245 built 231-231
   004b08ec  8bcf                   mov ecx, edi
   004b08ee  e8cd3cf5ff             call 0x4045c0
 - 004b08f3  8d4c2434               lea ecx, [esp + 0x34]
 - 004b08f7  e8e4e14e00             call 0x99eae0
   004b08fc  8d4c2430               lea ecx, [esp + 0x30]
   004b0900  e8dbe14e00             call 0x99eae0
@@ retail 247-247 built 233-235
   004b08fc  8d4c2430               lea ecx, [esp + 0x30]
   004b0900  e8dbe14e00             call 0x99eae0
 + 004b08c8  8d4c242c               lea ecx, [esp + 0x2c]
 + 004b08cc  e800000000             call 0x4b08d1
   004b0905  8bd6                   mov edx, esi
   004b0907  8d4c2428               lea ecx, [esp + 0x28]
@@ retail 248-249 built 236-237
   004b0900  e8dbe14e00             call 0x99eae0
   004b0905  8bd6                   mov edx, esi
 - 004b0907  8d4c2428               lea ecx, [esp + 0x28]
 + 004b08d3  8d4c2424               lea ecx, [esp + 0x24]
   004b090b  e820ef4e00             call 0x99f830
   004b0910  50                     push eax
@@ retail 252-253 built 240-241
   004b0910  50                     push eax
   004b0911  baec932301             mov edx, 0x12393ec
 - 004b0916  8d4c2430               lea ecx, [esp + 0x30]
 + 004b08e2  8d4c242c               lea ecx, [esp + 0x2c]
   004b091a  e871ed4e00             call 0x99f690
   004b091f  8bc8                   mov ecx, eax
@@ retail 263-264 built 251-252
   004b0931  8bcf                   mov ecx, edi
   004b0933  e8883cf5ff             call 0x4045c0
 - 004b0938  8d4c242c               lea ecx, [esp + 0x2c]
 + 004b0904  8d4c2428               lea ecx, [esp + 0x28]
   004b093c  e89fe14e00             call 0x99eae0
   004b0941  8d4c2428               lea ecx, [esp + 0x28]
@@ retail 265-266 built 253-258
   004b0938  8d4c242c               lea ecx, [esp + 0x2c]
   004b093c  e89fe14e00             call 0x99eae0
 - 004b0941  8d4c2428               lea ecx, [esp + 0x28]
 + 004b090d  8d4c2424               lea ecx, [esp + 0x24]
 + 004b0911  e800000000             call 0x4b0916
 + 004b0916  8d4c2458               lea ecx, [esp + 0x58]
 + 004b091a  e800000000             call 0x4b091f
 + 004b091f  8d4c245c               lea ecx, [esp + 0x5c]
   004b0945  e896e14e00             call 0x99eae0
   004b094a  8d4c2460               lea ecx, [esp + 0x60]
@@ retail 269-273 built 261-261
   004b094a  8d4c2460               lea ecx, [esp + 0x60]
   004b094e  e85ddb4e00             call 0x99e4b0
 - 004b0953  8d4c2464               lea ecx, [esp + 0x64]
 - 004b0957  e874a54e00             call 0x99aed0
 - 004b095c  8d4c2468               lea ecx, [esp + 0x68]
 - 004b0960  e86ba54e00             call 0x99aed0
   004b0965  8d442414               lea eax, [esp + 0x14]
   004b0969  50                     push eax
@@ retail 275-285 built 263-267
   004b0965  8d442414               lea eax, [esp + 0x14]
   004b0969  50                     push eax
 - 004b096a  8d4c2424               lea ecx, [esp + 0x24]
 - 004b096e  c644247100             mov byte ptr [esp + 0x71], 0
 - 004b0973  c644247200             mov byte ptr [esp + 0x72], 0
 - 004b0978  c744247400000000       mov dword ptr [esp + 0x74], 0
 - 004b0980  e8abe24e00             call 0x99ec30
 - 004b0985  8d4c2420               lea ecx, [esp + 0x20]
 - 004b0989  51                     push ecx
 - 004b098a  8d4c2464               lea ecx, [esp + 0x64]
 - 004b098e  e81de64e00             call 0x99efb0
 - 004b0993  8d4c2420               lea ecx, [esp + 0x20]
 + 004b0936  8d4c245c               lea ecx, [esp + 0x5c]
 + 004b093a  c644246900             mov byte ptr [esp + 0x69], 0
 + 004b093f  c644246a00             mov byte ptr [esp + 0x6a], 0
 + 004b0944  c744246c00000000       mov dword ptr [esp + 0x6c], 0
   004b0997  e844e14e00             call 0x99eae0
   004b099c  8a442412               mov al, byte ptr [esp + 0x12]
@@ retail 288-290 built 270-272
   004b099c  8a442412               mov al, byte ptr [esp + 0x12]
   004b09a0  84c0                   test al, al
 - 004b09a2  8b542450               mov edx, dword ptr [esp + 0x50]
 - 004b09a6  89542454               mov dword ptr [esp + 0x54], edx
 + 004b0957  8b4c2420               mov ecx, dword ptr [esp + 0x20]
 + 004b095b  894c244c               mov dword ptr [esp + 0x4c], ecx
   004b09aa  7405                   je 0x4b09b1
   004b09ac  c644246d01             mov byte ptr [esp + 0x6d], 1
@@ retail 291-292 built 273-274
   004b09a6  89542454               mov dword ptr [esp + 0x54], edx
   004b09aa  7405                   je 0x4b09b1
 - 004b09ac  c644246d01             mov byte ptr [esp + 0x6d], 1
 + 004b0961  c644246501             mov byte ptr [esp + 0x65], 1
   004b09b1  8a442411               mov al, byte ptr [esp + 0x11]
   004b09b5  84c0                   test al, al
@@ retail 295-296 built 277-278
   004b09b5  84c0                   test al, al
   004b09b7  7405                   je 0x4b09be
 - 004b09b9  c644246e01             mov byte ptr [esp + 0x6e], 1
 + 004b096e  c644246601             mov byte ptr [esp + 0x66], 1
   004b09be  8b1b                   mov ebx, dword ptr [ebx]
   004b09c0  6a28                   push 0x28
@@ retail 304-306 built 286-288
   004b09cf  85c9                   test ecx, ecx
   004b09d1  740a                   je 0x4b09dd
 - 004b09d3  8d442454               lea eax, [esp + 0x54]
 - 004b09d7  50                     push eax
 + 004b0988  8d54244c               lea edx, [esp + 0x4c]
 + 004b098c  52                     push edx
   004b09d8  e8c3690000             call 0x4b73a0
   004b09dd  8b4304                 mov eax, dword ptr [ebx + 4]
@@ retail 311-312 built 293-294
   004b09e3  894504                 mov dword ptr [ebp + 4], eax
   004b09e6  8928                   mov dword ptr [eax], ebp
 - 004b09e8  8d4c2468               lea ecx, [esp + 0x68]
 + 004b099d  8d4c2460               lea ecx, [esp + 0x60]
   004b09ec  896b04                 mov dword ptr [ebx + 4], ebp
   004b09ef  e81cab4e00             call 0x99b510
@@ retail 314-315 built 296-297
   004b09ec  896b04                 mov dword ptr [ebx + 4], ebp
   004b09ef  e81cab4e00             call 0x99b510
 - 004b09f4  8d4c2464               lea ecx, [esp + 0x64]
 + 004b09a9  8d4c245c               lea ecx, [esp + 0x5c]
   004b09f8  e813ab4e00             call 0x99b510
   004b09fd  8d4c2460               lea ecx, [esp + 0x60]
@@ retail 316-317 built 298-299
   004b09f4  8d4c2464               lea ecx, [esp + 0x64]
   004b09f8  e813ab4e00             call 0x99b510
 - 004b09fd  8d4c2460               lea ecx, [esp + 0x60]
 + 004b09b2  8d4c2458               lea ecx, [esp + 0x58]
   004b0a01  e8dae04e00             call 0x99eae0
   004b0a06  8d4c2414               lea ecx, [esp + 0x14]
@@ retail 320-321 built 302-303
   004b0a06  8d4c2414               lea ecx, [esp + 0x14]
   004b0a0a  e8d1e04e00             call 0x99eae0
 - 004b0a0f  8b442424               mov eax, dword ptr [esp + 0x24]
 + 004b09c4  8b44241c               mov eax, dword ptr [esp + 0x1c]
   004b0a13  46                     inc esi
   004b0a14  3bf0                   cmp esi, eax
@@ retail 328-329 built 310-311
   004b0a1e  5d                     pop ebp
   004b0a1f  5b                     pop ebx
 - 004b0a20  83c464                 add esp, 0x64
 + 004b09d5  83c45c                 add esp, 0x5c
   004b0a23  c20400                 ret 4
DIFFLINES 223 retail_instr 330 built_instr 312
