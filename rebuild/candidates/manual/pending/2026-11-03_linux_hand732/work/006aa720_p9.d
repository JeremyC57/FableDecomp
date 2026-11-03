006aa720 CThingPlayerCreature::SetPlayerCreatureActionForMelee DIFFER(1265v1212) flags= []
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
@@ retail 260-261 built 259-261
   006aaa24  e8a774fbff             call 0x661ed0
   006aaa29  8b5028                 mov edx, dword ptr [eax + 0x28]
 - 006aaa2c  52                     push edx
 + 006aaa27  8bc2                   mov eax, edx
 + 006aaa29  50                     push eax
   006aaa2d  8bcd                   mov ecx, ebp
   006aaa2f  e89cc9e1ff             call 0x4c73d0
@@ retail 262-262 built 262-263
   006aaa2c  52                     push edx
   006aaa2d  8bcd                   mov ecx, ebp
 + 006aaa2c  8954242c               mov dword ptr [esp + 0x2c], edx
   006aaa2f  e89cc9e1ff             call 0x4c73d0
   006aaa34  50                     push eax
@@ retail 279-280 built 280-281
   006aaa5d  f7472400004000         test dword ptr [edi + 0x24], 0x400000
   006aaa64  7450                   je 0x6aaab6
 - 006aaa66  8d442428               lea eax, [esp + 0x28]
 + 006aaa67  8d4c2428               lea ecx, [esp + 0x28]
   006aaa6a  8d7744                 lea esi, [edi + 0x44]
   006aaa6d  50                     push eax
@@ retail 281-282 built 282-283
   006aaa66  8d442428               lea eax, [esp + 0x28]
   006aaa6a  8d7744                 lea esi, [edi + 0x44]
 - 006aaa6d  50                     push eax
 + 006aaa6e  51                     push ecx
   006aaa6e  8bce                   mov ecx, esi
   006aaa70  c744242c36000000       mov dword ptr [esp + 0x2c], 0x36
@@ retail 321-324 built 322-324
   006aaad8  50                     push eax
   006aaad9  e8b2e2fbff             call 0x668d90
 - 006aaade  8b4c2430               mov ecx, dword ptr [esp + 0x30]
 - 006aaae2  85c9                   test ecx, ecx
 - 006aaae4  8b74241c               mov esi, dword ptr [esp + 0x1c]
 + 006aaadf  8b442430               mov eax, dword ptr [esp + 0x30]
 + 006aaae3  85c0                   test eax, eax
   006aaae8  0f84a0000000           je 0x6aab8e
   006aaaee  ff4904                 dec dword ptr [ecx + 4]
@@ retail 325-326 built 325-332
   006aaae4  8b74241c               mov esi, dword ptr [esp + 0x1c]
   006aaae8  0f84a0000000           je 0x6aab8e
 - 006aaaee  ff4904                 dec dword ptr [ecx + 4]
 + 006aaaeb  8b7004                 mov esi, dword ptr [eax + 4]
 + 006aaaee  8bc8                   mov ecx, eax
 + 006aaaf0  83c004                 add eax, 4
 + 006aaaf3  4e                     dec esi
 + 006aaaf4  8bd6                   mov edx, esi
 + 006aaaf6  85d2                   test edx, edx
 + 006aaaf8  8930                   mov dword ptr [eax], esi
   006aaaf1  0f8597000000           jne 0x6aab8e
   006aaaf7  8b11                   mov edx, dword ptr [ecx]
@@ retail 329-329 built 335-349
   006aaaf7  8b11                   mov edx, dword ptr [ecx]
   006aaaf9  ff5204                 call dword ptr [edx + 4]
 + 006aab05  e9b4000000             jmp 0x6aabbe
 + 006aab0a  8b442430               mov eax, dword ptr [esp + 0x30]
 + 006aab0e  85c0                   test eax, eax
 + 006aab10  0f84d3000000           je 0x6aabe9
 + 006aab16  8b7004                 mov esi, dword ptr [eax + 4]
 + 006aab19  8bc8                   mov ecx, eax
 + 006aab1b  83c004                 add eax, 4
 + 006aab1e  4e                     dec esi
 + 006aab1f  8bd6                   mov edx, esi
 + 006aab21  85d2                   test edx, edx
 + 006aab23  8930                   mov dword ptr [eax], esi
 + 006aab25  0f85be000000           jne 0x6aabe9
 + 006aab2b  8b01                   mov eax, dword ptr [ecx]
 + 006aab2d  ff5004                 call dword ptr [eax + 4]
   006aaafc  e98d000000             jmp 0x6aab8e
   006aab01  8b742410               mov esi, dword ptr [esp + 0x10]
@@ retail 377-378 built 397-397
   006aab84  50                     push eax
   006aab85  e806e2fbff             call 0x668d90
 - 006aab8a  8b74241c               mov esi, dword ptr [esp + 0x1c]
   006aab8e  8b442418               mov eax, dword ptr [esp + 0x18]
   006aab92  85c0                   test eax, eax
@@ retail 384-384 built 403-404
   006aab97  8bcf                   mov ecx, edi
   006aab99  e85299fbff             call 0x6644f0
 + 006aabce  8b74241c               mov esi, dword ptr [esp + 0x1c]
   006aab9e  85f6                   test esi, esi
   006aaba0  7413                   je 0x6aabb5
@@ retail 396-396 built 416-417
   006aabb9  85f6                   test esi, esi
   006aabbb  7415                   je 0x6aabd2
 + 006aabf1  8b5604                 mov edx, dword ptr [esi + 4]
   006aabbd  8d4604                 lea eax, [esi + 4]
   006aabc0  8bce                   mov ecx, esi
@@ retail 397-397 built 418-424
   006aabbb  7415                   je 0x6aabd2
   006aabbd  8d4604                 lea eax, [esi + 4]
 + 006aabf7  4a                     dec edx
 + 006aabf8  8bca                   mov ecx, edx
 + 006aabfa  85c9                   test ecx, ecx
 + 006aabfc  8910                   mov dword ptr [eax], edx
 + 006aabfe  7507                   jne 0x6aac07
 + 006aac00  8b06                   mov eax, dword ptr [esi]
   006aabc0  8bce                   mov ecx, esi
   006aabc2  8b30                   mov esi, dword ptr [eax]
@@ retail 398-405 built 425-425
   006aabbd  8d4604                 lea eax, [esi + 4]
   006aabc0  8bce                   mov ecx, esi
 - 006aabc2  8b30                   mov esi, dword ptr [eax]
 - 006aabc4  4e                     dec esi
 - 006aabc5  8bd6                   mov edx, esi
 - 006aabc7  85d2                   test edx, edx
 - 006aabc9  8930                   mov dword ptr [eax], esi
 - 006aabcb  7505                   jne 0x6aabd2
 - 006aabcd  8b01                   mov eax, dword ptr [ecx]
   006aabcf  ff5004                 call dword ptr [eax + 4]
   006aabd2  5f                     pop edi
DIFFLINES 70 retail_instr 412 built_instr 432
