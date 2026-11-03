006aca30 83ec24               sub esp, 0x24
006aca33 53                   push ebx
006aca34 55                   push ebp
006aca35 56                   push esi
006aca36 8bf1                 mov esi, ecx
006aca38 57                   push edi
006aca39 8d4c241c             lea ecx, [esp + 0x1c]
006aca3d 89742414             mov dword ptr [esp + 0x14], esi
006aca41 c644241300           mov byte ptr [esp + 0x13], 0
006aca46 c644242801           mov byte ptr [esp + 0x28], 1
006aca4b e8601a2f00           call 0x99e4b0
006aca50 8b6c2438             mov ebp, dword ptr [esp + 0x38]
006aca54 8b4500               mov eax, dword ptr [ebp]
006aca57 8bcd                 mov ecx, ebp
006aca59 ff90c0000000         call dword ptr [eax + 0xc0]
006aca5f 3d00040000           cmp eax, 0x400
006aca64 0f84b8050000         je 0x6ad022
006aca6a 3d00000002           cmp eax, 0x2000000
006aca6f 0f8592070000         jne 0x6ad207
006aca75 f7452800001000       test dword ptr [ebp + 0x28], 0x100000
006aca7c 7433                 je 0x6acab1
006aca7e 8d4c2424             lea ecx, [esp + 0x24]
006aca82 8d7544               lea esi, [ebp + 0x44]
006aca85 51                   push ecx
006aca86 8bce                 mov ecx, esi
006aca88 c744242854000000     mov dword ptr [esp + 0x28], 0x54
006aca90 e81b9bd8ff           call 0x4365b0
006aca95 8b7604               mov esi, dword ptr [esi + 4]
006aca98 3bc6                 cmp eax, esi
006aca9a 7405                 je 0x6acaa1
006aca9c 833854               cmp dword ptr [eax], 0x54
006aca9f 7e02                 jle 0x6acaa3
006acaa1 8bc6                 mov eax, esi
006acaa3 8b4004               mov eax, dword ptr [eax + 4]
006acaa6 8a480c               mov cl, byte ptr [eax + 0xc]
006acaa9 84c9                 test cl, cl
006acaab 0f8556070000         jne 0x6ad207
006acab1 8b4520               mov eax, dword ptr [ebp + 0x20]
006acab4 f6c420               test ah, 0x20
006acab7 0f844f030000         je 0x6ace0c
006acabd 8d542424             lea edx, [esp + 0x24]
006acac1 8d7d44               lea edi, [ebp + 0x44]
006acac4 52                   push edx
006acac5 8bcf                 mov ecx, edi
006acac7 c74424280d000000     mov dword ptr [esp + 0x28], 0xd
006acacf e8dc9ad8ff           call 0x4365b0
006acad4 8b4f04               mov ecx, dword ptr [edi + 4]
006acad7 3bc1                 cmp eax, ecx
006acad9 7405                 je 0x6acae0
006acadb 83380d               cmp dword ptr [eax], 0xd
006acade 7e02                 jle 0x6acae2
006acae0 8bc1                 mov eax, ecx
006acae2 8b7004               mov esi, dword ptr [eax + 4]
006acae5 6aff                 push -1
006acae7 68c4d22301           push 0x123d2c4
006acaec 8d4c242c             lea ecx, [esp + 0x2c]
006acaf0 e8fb202f00           call 0x99ebf0
006acaf5 8b06                 mov eax, dword ptr [esi]
006acaf7 8d4c2420             lea ecx, [esp + 0x20]
006acafb 51                   push ecx
006acafc 8bce                 mov ecx, esi
006acafe ff5058               call dword ptr [eax + 0x58]
006acb01 8b00                 mov eax, dword ptr [eax]
006acb03 8b4c2424             mov ecx, dword ptr [esp + 0x24]
006acb07 3bc1                 cmp eax, ecx
006acb09 7504                 jne 0x6acb0f
006acb0b b301                 mov bl, 1
006acb0d eb2b                 jmp 0x6acb3a
006acb0f 85c0                 test eax, eax
006acb11 7425                 je 0x6acb38
006acb13 85c9                 test ecx, ecx
006acb15 7421                 je 0x6acb38
006acb17 8b5004               mov edx, dword ptr [eax + 4]
006acb1a 3b5104               cmp edx, dword ptr [ecx + 4]
006acb1d 7406                 je 0x6acb25
006acb1f 32c0                 xor al, al
006acb21 8ad8                 mov bl, al
006acb23 eb15                 jmp 0x6acb3a
006acb25 8b11                 mov edx, dword ptr [ecx]
006acb27 8b08                 mov ecx, dword ptr [eax]
006acb29 e8424ad6ff           call 0x411570
006acb2e f7d8                 neg eax
006acb30 1ac0                 sbb al, al
006acb32 fec0                 inc al
006acb34 8ad8                 mov bl, al
006acb36 eb02                 jmp 0x6acb3a
006acb38 32db                 xor bl, bl
006acb3a 8d4c2420             lea ecx, [esp + 0x20]
006acb3e e89d1f2f00           call 0x99eae0
006acb43 8d4c2424             lea ecx, [esp + 0x24]
006acb47 e8941f2f00           call 0x99eae0
006acb4c 84db                 test bl, bl
006acb4e 0f84b8020000         je 0x6ace0c
006acb54 f7452400000010       test dword ptr [ebp + 0x24], 0x10000000
006acb5b 0f84a6060000         je 0x6ad207
006acb61 8d442424             lea eax, [esp + 0x24]
006acb65 50                   push eax
006acb66 8bcf                 mov ecx, edi
006acb68 c74424283c000000     mov dword ptr [esp + 0x28], 0x3c
006acb70 e83b9ad8ff           call 0x4365b0
006acb75 8b4f04               mov ecx, dword ptr [edi + 4]
006acb78 3bc1                 cmp eax, ecx
006acb7a 7405                 je 0x6acb81
006acb7c 83383c               cmp dword ptr [eax], 0x3c
006acb7f 7e02                 jle 0x6acb83
006acb81 8bc1                 mov eax, ecx
006acb83 8b4804               mov ecx, dword ptr [eax + 4]
006acb86 8b410c               mov eax, dword ptr [ecx + 0xc]
006acb89 894c2418             mov dword ptr [esp + 0x18], ecx
006acb8d 8b4910               mov ecx, dword ptr [ecx + 0x10]
006acb90 3bc1                 cmp eax, ecx
006acb92 0f846f060000         je 0x6ad207
006acb98 2bc8                 sub ecx, eax
006acb9a c1f903               sar ecx, 3
006acb9d 33ff                 xor edi, edi
006acb9f 85c9                 test ecx, ecx
006acba1 0f8604020000         jbe 0x6acdab
006acba7 8b14f8               mov edx, dword ptr [eax + edi*8]
006acbaa 8b6c2414             mov ebp, dword ptr [esp + 0x14]
006acbae 8d4c2420             lea ecx, [esp + 0x20]
006acbb2 51                   push ecx
006acbb3 33db                 xor ebx, ebx
006acbb5 52                   push edx
006acbb6 8bcd                 mov ecx, ebp
006acbb8 895c2428             mov dword ptr [esp + 0x28], ebx
006acbbc e8cfade1ff           call 0x4c7990
006acbc1 8bc8                 mov ecx, eax
006acbc3 e878e4d7ff           call 0x42b040
006acbc8 8b4c2420             mov ecx, dword ptr [esp + 0x20]
006acbcc 3bcb                 cmp ecx, ebx
006acbce 0f84bf010000         je 0x6acd93
006acbd4 8b8190000000         mov eax, dword ptr [ecx + 0x90]
006acbda f6c404               test ah, 4
006acbdd 0f8490000000         je 0x6acc73
006acbe3 f6452010             test byte ptr [ebp + 0x20], 0x10
006acbe7 0f8496010000         je 0x6acd83
006acbed 8bf5                 mov esi, ebp
006acbef 8d442424             lea eax, [esp + 0x24]
006acbf3 83c644               add esi, 0x44
006acbf6 50                   push eax
006acbf7 8bce                 mov ecx, esi
006acbf9 c744242804000000     mov dword ptr [esp + 0x28], 4
006acc01 e81a24d6ff           call 0x40f020
006acc06 8b7604               mov esi, dword ptr [esi + 4]
006acc09 3bc6                 cmp eax, esi
006acc0b 7405                 je 0x6acc12
006acc0d 833804               cmp dword ptr [eax], 4
006acc10 7e02                 jle 0x6acc14
006acc12 8bc6                 mov eax, esi
006acc14 8b6804               mov ebp, dword ptr [eax + 4]
006acc17 8d4c2424             lea ecx, [esp + 0x24]
006acc1b 51                   push ecx
006acc1c 8b4c2424             mov ecx, dword ptr [esp + 0x24]
006acc20 c744242800000000     mov dword ptr [esp + 0x28], 0
006acc28 e8f3160000           call 0x6ae320
006acc2d 8b742424             mov esi, dword ptr [esp + 0x24]
006acc31 85f6                 test esi, esi
006acc33 0f844a010000         je 0x6acd83
006acc39 8b5628               mov edx, dword ptr [esi + 0x28]
006acc3c 6a01                 push 1
006acc3e 52                   push edx
006acc3f 8bcd                 mov ecx, ebp
006acc41 e8e1e6ecff           call 0x57b327
006acc46 8b4c2414             mov ecx, dword ptr [esp + 0x14]
006acc4a e851ade1ff           call 0x4c79a0
006acc4f 0590000000           add eax, 0x90
006acc54 50                   push eax
006acc55 8d4c2420             lea ecx, [esp + 0x20]
006acc59 e852232f00           call 0x99efb0
006acc5e ff4e04               dec dword ptr [esi + 4]
006acc61 0f851c010000         jne 0x6acd83
006acc67 8b06                 mov eax, dword ptr [esi]
006acc69 8bce                 mov ecx, esi
006acc6b ff5004               call dword ptr [eax + 4]
006acc6e e910010000           jmp 0x6acd83
006acc73 8d542424             lea edx, [esp + 0x24]
006acc77 52                   push edx
006acc78 895c2428             mov dword ptr [esp + 0x28], ebx
006acc7c e860e1f0ff           call 0x5bade1
006acc81 8b442424             mov eax, dword ptr [esp + 0x24]
006acc85 3bc3                 cmp eax, ebx
006acc87 7407                 je 0x6acc90
006acc89 8a4860               mov cl, byte ptr [eax + 0x60]
006acc8c 84c9                 test cl, cl
006acc8e 756b                 jne 0x6accfb
006acc90 8b4538               mov eax, dword ptr [ebp + 0x38]
006acc93 f6c408               test ah, 8
006acc96 0f841e010000         je 0x6acdba
006acc9c 8d44242c             lea eax, [esp + 0x2c]
006acca0 8d7544               lea esi, [ebp + 0x44]
006acca3 50                   push eax
006acca4 8bce                 mov ecx, esi
006acca6 c7442430cb000000     mov dword ptr [esp + 0x30], 0xcb
006accae e86d23d6ff           call 0x40f020
006accb3 8b7604               mov esi, dword ptr [esi + 4]
006accb6 3bc6                 cmp eax, esi
006accb8 7408                 je 0x6accc2
006accba 8138cb000000         cmp dword ptr [eax], 0xcb
006accc0 7e02                 jle 0x6accc4
006accc2 8bc6                 mov eax, esi
006accc4 8b7004               mov esi, dword ptr [eax + 4]
006accc7 3bf3                 cmp esi, ebx
006accc9 0f84eb000000         je 0x6acdba
006acccf 8b4c2418             mov ecx, dword ptr [esp + 0x18]
006accd3 8b410c               mov eax, dword ptr [ecx + 0xc]
006accd6 8b14f8               mov edx, dword ptr [eax + edi*8]
006accd9 52                   push edx
006accda 8bce                 mov ecx, esi
006accdc e81fdb1100           call 0x7ca800
006acce1 84c0                 test al, al
006acce3 0f85d1000000         jne 0x6acdba
006acce9 8b442418             mov eax, dword ptr [esp + 0x18]
006acced 8b480c               mov ecx, dword ptr [eax + 0xc]
006accf0 8b14f9               mov edx, dword ptr [ecx + edi*8]
006accf3 52                   push edx
006accf4 8bce                 mov ecx, esi
006accf6 e855eb1100           call 0x7cb850
006accfb 8b442418             mov eax, dword ptr [esp + 0x18]
006accff 8b700c               mov esi, dword ptr [eax + 0xc]
006acd02 8b5c2414             mov ebx, dword ptr [esp + 0x14]
006acd06 0fbe9390000000       movsx edx, byte ptr [ebx + 0x90]
006acd0d 8d4cfe04             lea ecx, [esi + edi*8 + 4]
006acd11 51                   push ecx
006acd12 6a00                 push 0
006acd14 6a00                 push 0
006acd16 52                   push edx
006acd17 8bcb                 mov ecx, ebx
006acd19 e8b2a6e1ff           call 0x4c73d0
006acd1e 8b0cfe               mov ecx, dword ptr [esi + edi*8]
006acd21 8bd0                 mov edx, eax
006acd23 e8e8640500           call 0x703210
006acd28 8be8                 mov ebp, eax
006acd2a 85ed                 test ebp, ebp
006acd2c 7443                 je 0x6acd71
006acd2e 8b4338               mov eax, dword ptr [ebx + 0x38]
006acd31 f6c408               test ah, 8
006acd34 742f                 je 0x6acd65
006acd36 8d44242c             lea eax, [esp + 0x2c]
006acd3a 8d7344               lea esi, [ebx + 0x44]
006acd3d 50                   push eax
006acd3e 8bce                 mov ecx, esi
006acd40 c7442430cb000000     mov dword ptr [esp + 0x30], 0xcb
006acd48 e8d322d6ff           call 0x40f020
006acd4d 8b7604               mov esi, dword ptr [esi + 4]
006acd50 3bc6                 cmp eax, esi
006acd52 7408                 je 0x6acd5c
006acd54 8138cb000000         cmp dword ptr [eax], 0xcb
006acd5a 7e02                 jle 0x6acd5e
006acd5c 8bc6                 mov eax, esi
006acd5e 8b4804               mov ecx, dword ptr [eax + 4]
006acd61 894c2430             mov dword ptr [esp + 0x30], ecx
006acd65 8b4c2430             mov ecx, dword ptr [esp + 0x30]
006acd69 6a01                 push 1
006acd6b 55                   push ebp
006acd6c e8bfdd1100           call 0x7cab30
006acd71 8b4c2424             mov ecx, dword ptr [esp + 0x24]
006acd75 85c9                 test ecx, ecx
006acd77 740a                 je 0x6acd83
006acd79 ff4904               dec dword ptr [ecx + 4]
006acd7c 7505                 jne 0x6acd83
006acd7e 8b11                 mov edx, dword ptr [ecx]
006acd80 ff5204               call dword ptr [edx + 4]
006acd83 8b442420             mov eax, dword ptr [esp + 0x20]
006acd87 ff4804               dec dword ptr [eax + 4]
006acd8a 7507                 jne 0x6acd93
006acd8c 8bc8                 mov ecx, eax
006acd8e 8b01                 mov eax, dword ptr [ecx]
006acd90 ff5004               call dword ptr [eax + 4]
006acd93 8b4c2418             mov ecx, dword ptr [esp + 0x18]
006acd97 8b410c               mov eax, dword ptr [ecx + 0xc]
006acd9a 8b4910               mov ecx, dword ptr [ecx + 0x10]
006acd9d 2bc8                 sub ecx, eax
006acd9f 47                   inc edi
006acda0 c1f903               sar ecx, 3
006acda3 3bf9                 cmp edi, ecx
006acda5 0f82fcfdffff         jb 0x6acba7
006acdab c644241301           mov byte ptr [esp + 0x13], 1
006acdb0 c644242800           mov byte ptr [esp + 0x28], 0
006acdb5 e9ee030000           jmp 0x6ad1a8
006acdba 8b442418             mov eax, dword ptr [esp + 0x18]
006acdbe 8b400c               mov eax, dword ptr [eax + 0xc]
006acdc1 8b14f8               mov edx, dword ptr [eax + edi*8]
006acdc4 8d4cf804             lea ecx, [eax + edi*8 + 4]
006acdc8 51                   push ecx
006acdc9 6a01                 push 1
006acdcb 6aff                 push -1
006acdcd 52                   push edx
006acdce 8bcd                 mov ecx, ebp
006acdd0 e82bf4ffff           call 0x6ac200
006acdd5 85c0                 test eax, eax
006acdd7 7498                 je 0x6acd71
006acdd9 8b442418             mov eax, dword ptr [esp + 0x18]
006acddd 8b480c               mov ecx, dword ptr [eax + 0xc]
006acde0 8b14f9               mov edx, dword ptr [ecx + edi*8]
006acde3 8b0d90873b01         mov ecx, dword ptr [0x13b8790]
006acde9 52                   push edx
006acdea e891abd8ff           call 0x437980
006acdef 8b4c2414             mov ecx, dword ptr [esp + 0x14]
006acdf3 e8a8abe1ff           call 0x4c79a0
006acdf8 0594000000           add eax, 0x94
006acdfd 50                   push eax
006acdfe 8d4c2420             lea ecx, [esp + 0x20]
006ace02 e8a9212f00           call 0x99efb0
006ace07 e965ffffff           jmp 0x6acd71
006ace0c 0fb7858c000000       movzx eax, word ptr [ebp + 0x8c]
006ace13 8b4c2414             mov ecx, dword ptr [esp + 0x14]
006ace17 8d542420             lea edx, [esp + 0x20]
006ace1b 52                   push edx
006ace1c 50                   push eax
006ace1d 32db                 xor bl, bl
006ace1f c744242800000000     mov dword ptr [esp + 0x28], 0
006ace27 e864abe1ff           call 0x4c7990
006ace2c 8bc8                 mov ecx, eax
006ace2e e80de2d7ff           call 0x42b040
006ace33 8b442420             mov eax, dword ptr [esp + 0x20]
006ace37 85c0                 test eax, eax
006ace39 0f847b010000         je 0x6acfba
006ace3f 8d4c2424             lea ecx, [esp + 0x24]
006ace43 51                   push ecx
006ace44 8bc8                 mov ecx, eax
006ace46 c744242800000000     mov dword ptr [esp + 0x28], 0
006ace4e e88edff0ff           call 0x5bade1
006ace53 8b7c2424             mov edi, dword ptr [esp + 0x24]
006ace57 85ff                 test edi, edi
006ace59 0f845b010000         je 0x6acfba
006ace5f 8a4761               mov al, byte ptr [edi + 0x61]
006ace62 84c0                 test al, al
006ace64 7463                 je 0x6acec9
006ace66 8b542414             mov edx, dword ptr [esp + 0x14]
006ace6a 8b4238               mov eax, dword ptr [edx + 0x38]
006ace6d f6c408               test ah, 8
006ace70 7457                 je 0x6acec9
006ace72 8bf2                 mov esi, edx
006ace74 8d442430             lea eax, [esp + 0x30]
006ace78 83c644               add esi, 0x44
006ace7b 50                   push eax
006ace7c 8bce                 mov ecx, esi
006ace7e c7442434cb000000     mov dword ptr [esp + 0x34], 0xcb
006ace86 e89521d6ff           call 0x40f020
006ace8b 8b7604               mov esi, dword ptr [esi + 4]
006ace8e 3bc6                 cmp eax, esi
006ace90 7408                 je 0x6ace9a
006ace92 8138cb000000         cmp dword ptr [eax], 0xcb
006ace98 7e02                 jle 0x6ace9c
006ace9a 8bc6                 mov eax, esi
006ace9c 8b7004               mov esi, dword ptr [eax + 4]
006ace9f 85f6                 test esi, esi
006acea1 7426                 je 0x6acec9
006acea3 0fb78d8c000000       movzx ecx, word ptr [ebp + 0x8c]
006aceaa 51                   push ecx
006aceab 8bce                 mov ecx, esi
006acead e84ed91100           call 0x7ca800
006aceb2 84c0                 test al, al
006aceb4 7404                 je 0x6aceba
006aceb6 b301                 mov bl, 1
006aceb8 eb0f                 jmp 0x6acec9
006aceba 0fb7958c000000       movzx edx, word ptr [ebp + 0x8c]
006acec1 52                   push edx
006acec2 8bce                 mov ecx, esi
006acec4 e887e91100           call 0x7cb850
006acec9 ff4f04               dec dword ptr [edi + 4]
006acecc 7507                 jne 0x6aced5
006acece 8b07                 mov eax, dword ptr [edi]
006aced0 8bcf                 mov ecx, edi
006aced2 ff5004               call dword ptr [eax + 4]
006aced5 84db                 test bl, bl
006aced7 0f84dd000000         je 0x6acfba
006acedd f7452000000004       test dword ptr [ebp + 0x20], 0x4000000
006acee4 0f84d0000000         je 0x6acfba
006aceea 8d4c2430             lea ecx, [esp + 0x30]
006aceee 8d7544               lea esi, [ebp + 0x44]
006acef1 51                   push ecx
006acef2 8bce                 mov ecx, esi
006acef4 c74424341a000000     mov dword ptr [esp + 0x34], 0x1a
006acefc e81f21d6ff           call 0x40f020
006acf01 8b7604               mov esi, dword ptr [esi + 4]
006acf04 3bc6                 cmp eax, esi
006acf06 7405                 je 0x6acf0d
006acf08 83381a               cmp dword ptr [eax], 0x1a
006acf0b 7e02                 jle 0x6acf0f
006acf0d 8bc6                 mov eax, esi
006acf0f 8b5804               mov ebx, dword ptr [eax + 4]
006acf12 8bcb                 mov ecx, ebx
006acf14 33ff                 xor edi, edi
006acf16 e815c1f2ff           call 0x5d9030
006acf1b 8bf0                 mov esi, eax
006acf1d 81fe12010000         cmp esi, 0x112
006acf23 7212                 jb 0x6acf37
006acf25 6868e02201           push 0x122e068
006acf2a e8c31b5500           call 0xbfeaf2
006acf2f 83c404               add esp, 4
006acf32 e8b51b5500           call 0xbfeaec
006acf37 8bce                 mov ecx, esi
006acf39 83e11f               and ecx, 0x1f
006acf3c ba01000000           mov edx, 1
006acf41 d3e2                 shl edx, cl
006acf43 8b4c2414             mov ecx, dword ptr [esp + 0x14]
006acf47 8bc6                 mov eax, esi
006acf49 c1e805               shr eax, 5
006acf4c 85548120             test dword ptr [ecx + eax*4 + 0x20], edx
006acf50 7425                 je 0x6acf77
006acf52 8bf9                 mov edi, ecx
006acf54 8d542430             lea edx, [esp + 0x30]
006acf58 83c744               add edi, 0x44
006acf5b 52                   push edx
006acf5c 8bcf                 mov ecx, edi
006acf5e 89742434             mov dword ptr [esp + 0x34], esi
006acf62 e8b920d6ff           call 0x40f020
006acf67 8b7f04               mov edi, dword ptr [edi + 4]
006acf6a 3bc7                 cmp eax, edi
006acf6c 7404                 je 0x6acf72
006acf6e 3b30                 cmp esi, dword ptr [eax]
006acf70 7d02                 jge 0x6acf74
006acf72 8bc7                 mov eax, edi
006acf74 8b7804               mov edi, dword ptr [eax + 4]
006acf77 6a01                 push 1
006acf79 6a01                 push 1
006acf7b 6a00                 push 0
006acf7d 6a01                 push 1
006acf7f 6a00                 push 0
006acf81 57                   push edi
006acf82 8bcb                 mov ecx, ebx
006acf84 e8c7bdf2ff           call 0x5d8d50
006acf89 84c0                 test al, al
006acf8b 742d                 je 0x6acfba
006acf8d 0fb7858c000000       movzx eax, word ptr [ebp + 0x8c]
006acf94 8b0d90873b01         mov ecx, dword ptr [0x13b8790]
006acf9a 50                   push eax
006acf9b e8e0a9d8ff           call 0x437980
006acfa0 8b4c2414             mov ecx, dword ptr [esp + 0x14]
006acfa4 e8f7a9e1ff           call 0x4c79a0
006acfa9 0594000000           add eax, 0x94
006acfae 50                   push eax
006acfaf 8d4c2420             lea ecx, [esp + 0x20]
006acfb3 e8f81f2f00           call 0x99efb0
006acfb8 eb49                 jmp 0x6ad003
006acfba 8b4c2414             mov ecx, dword ptr [esp + 0x14]
006acfbe 8b4138               mov eax, dword ptr [ecx + 0x38]
006acfc1 f6c408               test ah, 8
006acfc4 742f                 je 0x6acff5
006acfc6 8bf1                 mov esi, ecx
006acfc8 8d542430             lea edx, [esp + 0x30]
006acfcc 83c644               add esi, 0x44
006acfcf 52                   push edx
006acfd0 8bce                 mov ecx, esi
006acfd2 c7442434cb000000     mov dword ptr [esp + 0x34], 0xcb
006acfda e84120d6ff           call 0x40f020
006acfdf 8b7604               mov esi, dword ptr [esi + 4]
006acfe2 3bc6                 cmp eax, esi
006acfe4 7408                 je 0x6acfee
006acfe6 8138cb000000         cmp dword ptr [eax], 0xcb
006acfec 7e02                 jle 0x6acff0
006acfee 8bc6                 mov eax, esi
006acff0 8b4004               mov eax, dword ptr [eax + 4]
006acff3 eb04                 jmp 0x6acff9
006acff5 8b442438             mov eax, dword ptr [esp + 0x38]
006acff9 6a01                 push 1
006acffb 55                   push ebp
006acffc 8bc8                 mov ecx, eax
006acffe e82ddb1100           call 0x7cab30
006ad003 8b4c2420             mov ecx, dword ptr [esp + 0x20]
006ad007 85c9                 test ecx, ecx
006ad009 0f8499010000         je 0x6ad1a8
006ad00f ff4904               dec dword ptr [ecx + 4]
006ad012 0f8590010000         jne 0x6ad1a8
006ad018 8b01                 mov eax, dword ptr [ecx]
006ad01a ff5004               call dword ptr [eax + 4]
006ad01d e986010000           jmp 0x6ad1a8
006ad022 f6462010             test byte ptr [esi + 0x20], 0x10
006ad026 0f84db010000         je 0x6ad207
006ad02c 8b6c2414             mov ebp, dword ptr [esp + 0x14]
006ad030 8d4c2430             lea ecx, [esp + 0x30]
006ad034 83c544               add ebp, 0x44
006ad037 51                   push ecx
006ad038 8bcd                 mov ecx, ebp
006ad03a c744243404000000     mov dword ptr [esp + 0x34], 4
006ad042 e8d91fd6ff           call 0x40f020
006ad047 8b4d04               mov ecx, dword ptr [ebp + 4]
006ad04a 3bc1                 cmp eax, ecx
006ad04c 7405                 je 0x6ad053
006ad04e 833804               cmp dword ptr [eax], 4
006ad051 7e02                 jle 0x6ad055
006ad053 8bc1                 mov eax, ecx
006ad055 8b542438             mov edx, dword ptr [esp + 0x38]
006ad059 8b5804               mov ebx, dword ptr [eax + 4]
006ad05c 8b4238               mov eax, dword ptr [edx + 0x38]
006ad05f f6c410               test ah, 0x10
006ad062 c644241301           mov byte ptr [esp + 0x13], 1
006ad067 0f843b010000         je 0x6ad1a8
006ad06d 8bf2                 mov esi, edx
006ad06f 8d442430             lea eax, [esp + 0x30]
006ad073 83c644               add esi, 0x44
006ad076 50                   push eax
006ad077 8bce                 mov ecx, esi
006ad079 c7442434cc000000     mov dword ptr [esp + 0x34], 0xcc
006ad081 e82a95d8ff           call 0x4365b0
006ad086 8b4e04               mov ecx, dword ptr [esi + 4]
006ad089 3bc1                 cmp eax, ecx
006ad08b 7408                 je 0x6ad095
006ad08d 8138cc000000         cmp dword ptr [eax], 0xcc
006ad093 7e02                 jle 0x6ad097
006ad095 8bc1                 mov eax, ecx
006ad097 8b7804               mov edi, dword ptr [eax + 4]
006ad09a 85ff                 test edi, edi
006ad09c 0f8406010000         je 0x6ad1a8
006ad0a2 6a01                 push 1
006ad0a4 8bcf                 mov ecx, edi
006ad0a6 e815ed1100           call 0x7cbdc0
006ad0ab 50                   push eax
006ad0ac 8bcb                 mov ecx, ebx
006ad0ae e874e2ecff           call 0x57b327
006ad0b3 8b4c2414             mov ecx, dword ptr [esp + 0x14]
006ad0b7 e8e4a8e1ff           call 0x4c79a0
006ad0bc 0590000000           add eax, 0x90
006ad0c1 50                   push eax
006ad0c2 8d4c2420             lea ecx, [esp + 0x20]
006ad0c6 e8e51e2f00           call 0x99efb0
006ad0cb 8b4c2438             mov ecx, dword ptr [esp + 0x38]
006ad0cf f7412400000800       test dword ptr [ecx + 0x24], 0x80000
006ad0d6 0f84cc000000         je 0x6ad1a8
006ad0dc 8d542430             lea edx, [esp + 0x30]
006ad0e0 52                   push edx
006ad0e1 8bce                 mov ecx, esi
006ad0e3 c744243433000000     mov dword ptr [esp + 0x34], 0x33
006ad0eb e8301fd6ff           call 0x40f020
006ad0f0 8b7604               mov esi, dword ptr [esi + 4]
006ad0f3 3bc6                 cmp eax, esi
006ad0f5 7405                 je 0x6ad0fc
006ad0f7 833833               cmp dword ptr [eax], 0x33
006ad0fa 7e02                 jle 0x6ad0fe
006ad0fc 8bc6                 mov eax, esi
006ad0fe 8b4004               mov eax, dword ptr [eax + 4]
006ad101 8d480d               lea ecx, [eax + 0xd]
006ad104 e8474a3500           call 0xa01b50
006ad109 f7403800002000       test dword ptr [eax + 0x38], 0x200000
006ad110 0f8492000000         je 0x6ad1a8
006ad116 8d7044               lea esi, [eax + 0x44]
006ad119 8d442430             lea eax, [esp + 0x30]
006ad11d 50                   push eax
006ad11e 8bce                 mov ecx, esi
006ad120 c7442434d5000000     mov dword ptr [esp + 0x34], 0xd5
006ad128 e8f31ed6ff           call 0x40f020
006ad12d 8b7604               mov esi, dword ptr [esi + 4]
006ad130 3bc6                 cmp eax, esi
006ad132 7408                 je 0x6ad13c
006ad134 8138d5000000         cmp dword ptr [eax], 0xd5
006ad13a 7e02                 jle 0x6ad13e
006ad13c 8bc6                 mov eax, esi
006ad13e 8b4004               mov eax, dword ptr [eax + 4]
006ad141 8a4812               mov cl, byte ptr [eax + 0x12]
006ad144 84c9                 test cl, cl
006ad146 7460                 je 0x6ad1a8
006ad148 8bcf                 mov ecx, edi
006ad14a e871ec1100           call 0x7cbdc0
006ad14f 8b4b54               mov ecx, dword ptr [ebx + 0x54]
006ad152 8b542414             mov edx, dword ptr [esp + 0x14]
006ad156 03c8                 add ecx, eax
006ad158 894b54               mov dword ptr [ebx + 0x54], ecx
006ad15b 8b4240               mov eax, dword ptr [edx + 0x40]
006ad15e f6c420               test ah, 0x20
006ad161 894c242c             mov dword ptr [esp + 0x2c], ecx
006ad165 742a                 je 0x6ad191
006ad167 8d442430             lea eax, [esp + 0x30]
006ad16b 50                   push eax
006ad16c 8bcd                 mov ecx, ebp
006ad16e c74424340d010000     mov dword ptr [esp + 0x34], 0x10d
006ad176 e8a51ed6ff           call 0x40f020
006ad17b 8b4d04               mov ecx, dword ptr [ebp + 4]
006ad17e 3bc1                 cmp eax, ecx
006ad180 7408                 je 0x6ad18a
006ad182 81380d010000         cmp dword ptr [eax], 0x10d
006ad188 7e02                 jle 0x6ad18c
006ad18a 8bc1                 mov eax, ecx
006ad18c 8b4004               mov eax, dword ptr [eax + 4]
006ad18f eb04                 jmp 0x6ad195
006ad191 8b442438             mov eax, dword ptr [esp + 0x38]
006ad195 db44242c             fild dword ptr [esp + 0x2c]
006ad199 6a00                 push 0
006ad19b 51                   push ecx
006ad19c 8bc8                 mov ecx, eax
006ad19e d91c24               fstp dword ptr [esp]
006ad1a1 6a10                 push 0x10
006ad1a3 e80822ebff           call 0x55f3b0
006ad1a8 8b4c2414             mov ecx, dword ptr [esp + 0x14]
006ad1ac f7412000000100       test dword ptr [ecx + 0x20], 0x10000
006ad1b3 743c                 je 0x6ad1f1
006ad1b5 8bf1                 mov esi, ecx
006ad1b7 8d542430             lea edx, [esp + 0x30]
006ad1bb 83c644               add esi, 0x44
006ad1be 52                   push edx
006ad1bf 8bce                 mov ecx, esi
006ad1c1 c744243410000000     mov dword ptr [esp + 0x34], 0x10
006ad1c9 e8521ed6ff           call 0x40f020
006ad1ce 8b7604               mov esi, dword ptr [esi + 4]
006ad1d1 3bc6                 cmp eax, esi
006ad1d3 7405                 je 0x6ad1da
006ad1d5 833810               cmp dword ptr [eax], 0x10
006ad1d8 7e02                 jle 0x6ad1dc
006ad1da 8bc6                 mov eax, esi
006ad1dc 8b4804               mov ecx, dword ptr [eax + 4]
006ad1df 6aff                 push -1
006ad1e1 6a00                 push 0
006ad1e3 6a00                 push 0
006ad1e5 6a01                 push 1
006ad1e7 8d44242c             lea eax, [esp + 0x2c]
006ad1eb 50                   push eax
006ad1ec e80febfbff           call 0x66bd00
006ad1f1 8a442413             mov al, byte ptr [esp + 0x13]
006ad1f5 84c0                 test al, al
006ad1f7 740e                 je 0x6ad207
006ad1f9 8b4c2428             mov ecx, dword ptr [esp + 0x28]
006ad1fd 51                   push ecx
006ad1fe 8b4c243c             mov ecx, dword ptr [esp + 0x3c]
006ad202 e879c9e1ff           call 0x4c9b80
006ad207 8d4c241c             lea ecx, [esp + 0x1c]
006ad20b e8d0182f00           call 0x99eae0
006ad210 5f                   pop edi
006ad211 5e                   pop esi
006ad212 5d                   pop ebp
006ad213 5b                   pop ebx
006ad214 83c424               add esp, 0x24
006ad217 c20400               ret 4
