006aa720 CThingPlayerCreature::SetPlayerCreatureActionForMelee DIFFER(1221v1212) flags= []
@@ retail 2-4 built 2-2
   006aa720  83ec10                 sub esp, 0x10
   006aa723  53                     push ebx
 - 006aa724  8bd9                   mov ebx, ecx
 - 006aa726  f7432400000100         test dword ptr [ebx + 0x24], 0x10000
   006aa72d  55                     push ebp
   006aa72e  56                     push esi
@@ retail 7-8 built 5-8
   006aa72e  56                     push esi
   006aa72f  57                     push edi
 - 006aa730  895c2414               mov dword ptr [esp + 0x14], ebx
 + 006aa727  8bf9                   mov edi, ecx
 + 006aa729  f7472400000100         test dword ptr [edi + 0x24], 0x10000
 + 006aa730  897c2414               mov dword ptr [esp + 0x14], edi
   006aa734  742a                   je 0x6aa760
   006aa736  8d442418               lea eax, [esp + 0x18]
@@ retail 10-11 built 10-11
   006aa734  742a                   je 0x6aa760
   006aa736  8d442418               lea eax, [esp + 0x18]
 - 006aa73a  8d7344                 lea esi, [ebx + 0x44]
 + 006aa73a  8d7744                 lea esi, [edi + 0x44]
   006aa73d  50                     push eax
   006aa73e  8bce                   mov ecx, esi
@@ retail 24-26 built 24-26
   006aa75e  eb04                   jmp 0x6aa764
   006aa760  8b6c2428               mov ebp, dword ptr [esp + 0x28]
 - 006aa764  8b7c2430               mov edi, dword ptr [esp + 0x30]
 - 006aa768  8b473c                 mov eax, dword ptr [edi + 0x3c]
 + 006aa764  8b5c2430               mov ebx, dword ptr [esp + 0x30]
 + 006aa768  8b433c                 mov eax, dword ptr [ebx + 0x3c]
   006aa76b  33f6                   xor esi, esi
   006aa76d  83f801                 cmp eax, 1
@@ retail 40-41 built 40-41
   006aa78d  eb21                   jmp 0x6aa7b0
   006aa78f  8b742410               mov esi, dword ptr [esp + 0x10]
 - 006aa793  837f3c07               cmp dword ptr [edi + 0x3c], 7
 + 006aa793  837b3c07               cmp dword ptr [ebx + 0x3c], 7
   006aa797  7575                   jne 0x6aa80e
   006aa799  8bcd                   mov ecx, ebp
@@ retail 51-53 built 51-53
   006aa7ab  e8008d0800             call 0x7334b0
   006aa7b0  8b742410               mov esi, dword ptr [esp + 0x10]
 - 006aa7b4  8bf8                   mov edi, eax
 - 006aa7b6  3b37                   cmp esi, dword ptr [edi]
 + 006aa7b4  8bd8                   mov ebx, eax
 + 006aa7b6  3b33                   cmp esi, dword ptr [ebx]
   006aa7b8  7432                   je 0x6aa7ec
   006aa7ba  85f6                   test esi, esi
@@ retail 67-68 built 67-68
   006aa7d0  ff5204                 call dword ptr [edx + 4]
   006aa7d3  c744241000000000       mov dword ptr [esp + 0x10], 0
 - 006aa7db  8b37                   mov esi, dword ptr [edi]
 + 006aa7db  8b33                   mov esi, dword ptr [ebx]
   006aa7dd  85f6                   test esi, esi
   006aa7df  89742410               mov dword ptr [esp + 0x10], esi
@@ retail 87-88 built 87-88
   006aa807  ff5004                 call dword ptr [eax + 4]
   006aa80a  8b742410               mov esi, dword ptr [esp + 0x10]
 - 006aa80e  f7432400000200         test dword ptr [ebx + 0x24], 0x20000
 + 006aa80e  f7472400000200         test dword ptr [edi + 0x24], 0x20000
   006aa815  bd46000000             mov ebp, 0x46
   006aa81a  7472                   je 0x6aa88e
@@ retail 91-92 built 91-92
   006aa81a  7472                   je 0x6aa88e
   006aa81c  8d4c2430               lea ecx, [esp + 0x30]
 - 006aa820  8d7344                 lea esi, [ebx + 0x44]
 + 006aa820  8d7744                 lea esi, [edi + 0x44]
   006aa823  51                     push ecx
   006aa824  8bce                   mov ecx, esi
@@ retail 109-110 built 109-110
   006aa850  83f822                 cmp eax, 0x22
   006aa853  7535                   jne 0x6aa88a
 - 006aa855  f6432840               test byte ptr [ebx + 0x28], 0x40
 + 006aa855  f6472840               test byte ptr [edi + 0x28], 0x40
   006aa859  7422                   je 0x6aa87d
   006aa85b  8d442430               lea eax, [esp + 0x30]
@@ retail 129-132 built 129-130
   006aa885  e87675ffff             call 0x6a1e00
   006aa88a  8b742410               mov esi, dword ptr [esp + 0x10]
 - 006aa88e  8b442414               mov eax, dword ptr [esp + 0x14]
 - 006aa892  8a4828                 mov cl, byte ptr [eax + 0x28]
 - 006aa895  33ff                   xor edi, edi
 + 006aa88e  8a4728                 mov al, byte ptr [edi + 0x28]
   006aa897  33db                   xor ebx, ebx
   006aa899  f6c140                 test cl, 0x40
@@ retail 133-134 built 131-132
   006aa895  33ff                   xor edi, edi
   006aa897  33db                   xor ebx, ebx
 - 006aa899  f6c140                 test cl, 0x40
 + 006aa893  a840                   test al, 0x40
   006aa89c  c644243000             mov byte ptr [esp + 0x30], 0
   006aa8a1  0f8486000000           je 0x6aa92d
@@ retail 137-138 built 135-136
   006aa8a1  0f8486000000           je 0x6aa92d
   006aa8a7  8d4c2418               lea ecx, [esp + 0x18]
 - 006aa8ab  8d7044                 lea esi, [eax + 0x44]
 + 006aa8a4  8d7744                 lea esi, [edi + 0x44]
   006aa8ae  51                     push ecx
   006aa8af  8bce                   mov ecx, esi
@@ retail 151-152 built 149-150
   006aa8ca  e8716bffff             call 0x6a1440
   006aa8cf  8bd8                   mov ebx, eax
 - 006aa8d1  3bdf                   cmp ebx, edi
 + 006aa8ca  85db                   test ebx, ebx
   006aa8d3  7454                   je 0x6aa929
   006aa8d5  f6839100000001         test byte ptr [ebx + 0x91], 1
@@ retail 182-185 built 180-184
   006aa929  8b742410               mov esi, dword ptr [esp + 0x10]
   006aa92d  8b6c242c               mov ebp, dword ptr [esp + 0x2c]
 - 006aa931  3bef                   cmp ebp, edi
 - 006aa933  897c2418               mov dword ptr [esp + 0x18], edi
 - 006aa937  897c241c               mov dword ptr [esp + 0x1c], edi
 + 006aa92a  33c0                   xor eax, eax
 + 006aa92c  3be8                   cmp ebp, eax
 + 006aa92e  89442418               mov dword ptr [esp + 0x18], eax
 + 006aa932  8944241c               mov dword ptr [esp + 0x1c], eax
   006aa93b  0f84c4010000           je 0x6aab05
   006aa941  f6859100000001         test byte ptr [ebp + 0x91], 1
@@ retail 204-205 built 203-206
   006aa97d  8bc7                   mov eax, edi
   006aa97f  8b4004                 mov eax, dword ptr [eax + 4]
 - 006aa982  f6400c04               test byte ptr [eax + 0xc], 4
 + 006aa97d  8b480c                 mov ecx, dword ptr [eax + 0xc]
 + 006aa980  c1e902                 shr ecx, 2
 + 006aa983  f6c101                 test cl, 1
   006aa986  0f8479010000           je 0x6aab05
   006aa98c  8b7c2414               mov edi, dword ptr [esp + 0x14]
@@ retail 215-217 built 216-217
   006aa9a3  8b7570                 mov esi, dword ptr [ebp + 0x70]
   006aa9a6  85f6                   test esi, esi
 - 006aa9a8  c644242800             mov byte ptr [esp + 0x28], 0
 - 006aa9ad  c744243000000000       mov dword ptr [esp + 0x30], 0
 + 006aa9a8  c744242800000000       mov dword ptr [esp + 0x28], 0
   006aa9b5  7403                   je 0x6aa9ba
   006aa9b7  ff4604                 inc dword ptr [esi + 4]
@@ retail 219-221 built 219-221
   006aa9b5  7403                   je 0x6aa9ba
   006aa9b7  ff4604                 inc dword ptr [esi + 4]
 - 006aa9ba  8d4c2430               lea ecx, [esp + 0x30]
 - 006aa9be  51                     push ecx
 + 006aa9b5  8d542428               lea edx, [esp + 0x28]
 + 006aa9b9  52                     push edx
   006aa9bf  8bce                   mov ecx, esi
   006aa9c1  e89a380000             call 0x6ae260
@@ retail 223-223 built 223-229
   006aa9bf  8bce                   mov ecx, esi
   006aa9c1  e89a380000             call 0x6ae260
 + 006aa9c1  84c0                   test al, al
 + 006aa9c3  7409                   je 0x6aa9ce
 + 006aa9c5  8b442428               mov eax, dword ptr [esp + 0x28]
 + 006aa9c9  8a5850                 mov bl, byte ptr [eax + 0x50]
 + 006aa9cc  eb02                   jmp 0x6aa9d0
 + 006aa9ce  32db                   xor bl, bl
   006aa9c6  85f6                   test esi, esi
   006aa9c8  8ad8                   mov bl, al
@@ retail 224-225 built 230-230
   006aa9c1  e89a380000             call 0x6ae260
   006aa9c6  85f6                   test esi, esi
 - 006aa9c8  8ad8                   mov bl, al
   006aa9ca  740c                   je 0x6aa9d8
   006aa9cc  ff4e04                 dec dword ptr [esi + 4]
@@ retail 231-237 built 236-236
   006aa9d3  8bce                   mov ecx, esi
   006aa9d5  ff5204                 call dword ptr [edx + 4]
 - 006aa9d8  84db                   test bl, bl
 - 006aa9da  7409                   je 0x6aa9e5
 - 006aa9dc  8b442430               mov eax, dword ptr [esp + 0x30]
 - 006aa9e0  8a5850                 mov bl, byte ptr [eax + 0x50]
 - 006aa9e3  eb04                   jmp 0x6aa9e9
 - 006aa9e5  8a5c2428               mov bl, byte ptr [esp + 0x28]
   006aa9e9  8a4720                 mov al, byte ptr [edi + 0x20]
   006aa9ec  84c0                   test al, al
@@ retail 240-241 built 239-240
   006aa9ec  84c0                   test al, al
   006aa9ee  792a                   jns 0x6aaa1a
 - 006aa9f0  8d4c2428               lea ecx, [esp + 0x28]
 + 006aa9e7  8d442430               lea eax, [esp + 0x30]
   006aa9f4  8d7744                 lea esi, [edi + 0x44]
   006aa9f7  51                     push ecx
@@ retail 242-243 built 241-242
   006aa9f0  8d4c2428               lea ecx, [esp + 0x28]
   006aa9f4  8d7744                 lea esi, [edi + 0x44]
 - 006aa9f7  51                     push ecx
 + 006aa9ee  50                     push eax
   006aa9f8  8bce                   mov ecx, esi
   006aa9fa  c744242c07000000       mov dword ptr [esp + 0x2c], 7
@@ retail 244-245 built 243-244
   006aa9f7  51                     push ecx
   006aa9f8  8bce                   mov ecx, esi
 - 006aa9fa  c744242c07000000       mov dword ptr [esp + 0x2c], 7
 + 006aa9f1  c744243407000000       mov dword ptr [esp + 0x34], 7
   006aaa02  e81946d6ff             call 0x40f020
   006aaa07  8b7604                 mov esi, dword ptr [esi + 4]
@@ retail 259-260 built 258-261
   006aaa22  8bcf                   mov ecx, edi
   006aaa24  e8a774fbff             call 0x661ed0
 - 006aaa29  8b5028                 mov edx, dword ptr [eax + 0x28]
 + 006aaa20  8b4828                 mov ecx, dword ptr [eax + 0x28]
 + 006aaa23  8bd1                   mov edx, ecx
 + 006aaa25  894c2430               mov dword ptr [esp + 0x30], ecx
   006aaa2c  52                     push edx
   006aaa2d  8bcd                   mov ecx, ebp
@@ retail 279-280 built 280-281
   006aaa5d  f7472400004000         test dword ptr [edi + 0x24], 0x400000
   006aaa64  7450                   je 0x6aaab6
 - 006aaa66  8d442428               lea eax, [esp + 0x28]
 + 006aaa63  8d442430               lea eax, [esp + 0x30]
   006aaa6a  8d7744                 lea esi, [edi + 0x44]
   006aaa6d  50                     push eax
@@ retail 283-284 built 284-285
   006aaa6d  50                     push eax
   006aaa6e  8bce                   mov ecx, esi
 - 006aaa70  c744242c36000000       mov dword ptr [esp + 0x2c], 0x36
 + 006aaa6d  c744243436000000       mov dword ptr [esp + 0x34], 0x36
   006aaa78  e8a345d6ff             call 0x40f020
   006aaa7d  8b7604                 mov esi, dword ptr [esi + 4]
@@ retail 321-323 built 322-324
   006aaad8  50                     push eax
   006aaad9  e8b2e2fbff             call 0x668d90
 - 006aaade  8b4c2430               mov ecx, dword ptr [esp + 0x30]
 - 006aaae2  85c9                   test ecx, ecx
 + 006aaadb  8b442428               mov eax, dword ptr [esp + 0x28]
 + 006aaadf  85c0                   test eax, eax
   006aaae4  8b74241c               mov esi, dword ptr [esp + 0x1c]
   006aaae8  0f84a0000000           je 0x6aab8e
@@ retail 325-326 built 326-333
   006aaae4  8b74241c               mov esi, dword ptr [esp + 0x1c]
   006aaae8  0f84a0000000           je 0x6aab8e
 - 006aaaee  ff4904                 dec dword ptr [ecx + 4]
 + 006aaaeb  8b5804                 mov ebx, dword ptr [eax + 4]
 + 006aaaee  8bc8                   mov ecx, eax
 + 006aaaf0  83c004                 add eax, 4
 + 006aaaf3  4b                     dec ebx
 + 006aaaf4  8bd3                   mov edx, ebx
 + 006aaaf6  85d2                   test edx, edx
 + 006aaaf8  8918                   mov dword ptr [eax], ebx
   006aaaf1  0f8597000000           jne 0x6aab8e
   006aaaf7  8b11                   mov edx, dword ptr [ecx]
DIFFLINES 62 retail_instr 412 built_instr 419
