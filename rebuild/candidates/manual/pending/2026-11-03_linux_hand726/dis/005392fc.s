005392fc 55                   push ebp
005392fd 8bec                 mov ebp, esp
005392ff 83ec78               sub esp, 0x78
00539302 56                   push esi
00539303 8bf1                 mov esi, ecx
00539305 8b8668010000         mov eax, dword ptr [esi + 0x168]
0053930b 2b8664010000         sub eax, dword ptr [esi + 0x164]
00539311 c1f802               sar eax, 2
00539314 83f801               cmp eax, 1
00539317 0f867f0d0000         jbe 0x53a09c
0053931d 807d1400             cmp byte ptr [ebp + 0x14], 0
00539321 57                   push edi
00539322 8b7d0c               mov edi, dword ptr [ebp + 0xc]
00539325 747d                 je 0x5393a4
00539327 83be5c01000000       cmp dword ptr [esi + 0x15c], 0
0053932e 7510                 jne 0x539340
00539330 8b06                 mov eax, dword ptr [esi]
00539332 ff90a8020000         call dword ptr [eax + 0x2a8]
00539338 84c0                 test al, al
0053933a 0f845b0d0000         je 0x53a09b
00539340 8b865c010000         mov eax, dword ptr [esi + 0x15c]
00539346 3b4508               cmp eax, dword ptr [ebp + 8]
00539349 7259                 jb 0x5393a4
0053934b 3bc7                 cmp eax, edi
0053934d 7755                 ja 0x5393a4
0053934f 8b8e64010000         mov ecx, dword ptr [esi + 0x164]
00539355 8b0c81               mov ecx, dword ptr [ecx + eax*4]
00539358 8b01                 mov eax, dword ptr [ecx]
0053935a 6a04                 push 4
0053935c ff90c0000000         call dword ptr [eax + 0xc0]
00539362 ff8e5c010000         dec dword ptr [esi + 0x15c]
00539368 8b865c010000         mov eax, dword ptr [esi + 0x15c]
0053936e 7910                 jns 0x539380
00539370 8b8668010000         mov eax, dword ptr [esi + 0x168]
00539376 2b8664010000         sub eax, dword ptr [esi + 0x164]
0053937c c1f802               sar eax, 2
0053937f 48                   dec eax
00539380 8b16                 mov edx, dword ptr [esi]
00539382 50                   push eax
00539383 8bce                 mov ecx, esi
00539385 ff9284020000         call dword ptr [edx + 0x284]
0053938b 8b865c010000         mov eax, dword ptr [esi + 0x15c]
00539391 8b8e64010000         mov ecx, dword ptr [esi + 0x164]
00539397 8b0c81               mov ecx, dword ptr [ecx + eax*4]
0053939a 8b01                 mov eax, dword ptr [ecx]
0053939c 6a03                 push 3
0053939e ff90c0000000         call dword ptr [eax + 0xc0]
005393a4 8b06                 mov eax, dword ptr [esi]
005393a6 8bce                 mov ecx, esi
005393a8 ff90ac020000         call dword ptr [eax + 0x2ac]
005393ae 84c0                 test al, al
005393b0 7509                 jne 0x5393bb
005393b2 384518               cmp byte ptr [ebp + 0x18], al
005393b5 0f84e00c0000         je 0x53a09b
005393bb 8b06                 mov eax, dword ptr [esi]
005393bd 8bce                 mov ecx, esi
005393bf ff90d8000000         call dword ptr [eax + 0xd8]
005393c5 d905e0de2201         fld dword ptr [0x122dee0]
005393cb dae9                 fucompp 
005393cd dfe0                 fnstsw ax
005393cf f6c444               test ah, 0x44
005393d2 7b0c                 jnp 0x5393e0
005393d4 8b06                 mov eax, dword ptr [esi]
005393d6 8bce                 mov ecx, esi
005393d8 ff90d8000000         call dword ptr [eax + 0xd8]
005393de eb02                 jmp 0x5393e2
005393e0 d9ee                 fldz 
005393e2 8b8668010000         mov eax, dword ptr [esi + 0x168]
005393e8 d95dfc               fstp dword ptr [ebp - 4]
005393eb 2b8664010000         sub eax, dword ptr [esi + 0x164]
005393f1 53                   push ebx
005393f2 c1f802               sar eax, 2
005393f5 48                   dec eax
005393f6 3bf8                 cmp edi, eax
005393f8 6a05                 push 5
005393fa 5b                   pop ebx
005393fb 0f85c0000000         jne 0x5394c1
00539401 807d1000             cmp byte ptr [ebp + 0x10], 0
00539405 7463                 je 0x53946a
00539407 8b4508               mov eax, dword ptr [ebp + 8]
0053940a 8b8e7c010000         mov ecx, dword ptr [esi + 0x17c]
00539410 8d0440               lea eax, [eax + eax*2]
00539413 c1e002               shl eax, 2
00539416 8d55f8               lea edx, [ebp - 8]
00539419 52                   push edx
0053941a 03c8                 add ecx, eax
0053941c 895df8               mov dword ptr [ebp - 8], ebx
0053941f 894518               mov dword ptr [ebp + 0x18], eax
00539422 e88a41efff           call 0x42d5b1
00539427 8b4808               mov ecx, dword ptr [eax + 8]
0053942a 894dd0               mov dword ptr [ebp - 0x30], ecx
0053942d 8b400c               mov eax, dword ptr [eax + 0xc]
00539430 8d4df8               lea ecx, [ebp - 8]
00539433 51                   push ecx
00539434 8b4d18               mov ecx, dword ptr [ebp + 0x18]
00539437 8945d4               mov dword ptr [ebp - 0x2c], eax
0053943a 8b867c010000         mov eax, dword ptr [esi + 0x17c]
00539440 03c8                 add ecx, eax
00539442 895df8               mov dword ptr [ebp - 8], ebx
00539445 e86741efff           call 0x42d5b1
0053944a 8b4810               mov ecx, dword ptr [eax + 0x10]
0053944d 894dc8               mov dword ptr [ebp - 0x38], ecx
00539450 8b4014               mov eax, dword ptr [eax + 0x14]
00539453 8d4df8               lea ecx, [ebp - 8]
00539456 51                   push ecx
00539457 8b4d18               mov ecx, dword ptr [ebp + 0x18]
0053945a 8945cc               mov dword ptr [ebp - 0x34], eax
0053945d 8b867c010000         mov eax, dword ptr [esi + 0x17c]
00539463 03c8                 add ecx, eax
00539465 e9be000000           jmp 0x539528
0053946a 8b8664010000         mov eax, dword ptr [esi + 0x164]
00539470 8b4d08               mov ecx, dword ptr [ebp + 8]
00539473 8b0c88               mov ecx, dword ptr [eax + ecx*4]
00539476 8b01                 mov eax, dword ptr [ecx]
00539478 8d55d8               lea edx, [ebp - 0x28]
0053947b 52                   push edx
0053947c ff90dc000000         call dword ptr [eax + 0xdc]
00539482 8b08                 mov ecx, dword ptr [eax]
00539484 894dd0               mov dword ptr [ebp - 0x30], ecx
00539487 8b4004               mov eax, dword ptr [eax + 4]
0053948a 8b4d08               mov ecx, dword ptr [ebp + 8]
0053948d 8945d4               mov dword ptr [ebp - 0x2c], eax
00539490 8b8664010000         mov eax, dword ptr [esi + 0x164]
00539496 8b0c88               mov ecx, dword ptr [eax + ecx*4]
00539499 8b01                 mov eax, dword ptr [ecx]
0053949b 8d55d8               lea edx, [ebp - 0x28]
0053949e 52                   push edx
0053949f ff90e4000000         call dword ptr [eax + 0xe4]
005394a5 8b08                 mov ecx, dword ptr [eax]
005394a7 894dc8               mov dword ptr [ebp - 0x38], ecx
005394aa 8b4004               mov eax, dword ptr [eax + 4]
005394ad 8b4d08               mov ecx, dword ptr [ebp + 8]
005394b0 8945cc               mov dword ptr [ebp - 0x34], eax
005394b3 8b8664010000         mov eax, dword ptr [esi + 0x164]
005394b9 8b0c88               mov ecx, dword ptr [eax + ecx*4]
005394bc e9c0000000           jmp 0x539581
005394c1 807d1000             cmp byte ptr [ebp + 0x10], 0
005394c5 746e                 je 0x539535
005394c7 8b8e7c010000         mov ecx, dword ptr [esi + 0x17c]
005394cd 8d047f               lea eax, [edi + edi*2]
005394d0 c1e002               shl eax, 2
005394d3 8d55f8               lea edx, [ebp - 8]
005394d6 52                   push edx
005394d7 8d4c080c             lea ecx, [eax + ecx + 0xc]
005394db 895df8               mov dword ptr [ebp - 8], ebx
005394de 894518               mov dword ptr [ebp + 0x18], eax
005394e1 e8cb40efff           call 0x42d5b1
005394e6 8b4808               mov ecx, dword ptr [eax + 8]
005394e9 894dd0               mov dword ptr [ebp - 0x30], ecx
005394ec 8b400c               mov eax, dword ptr [eax + 0xc]
005394ef 8d4df8               lea ecx, [ebp - 8]
005394f2 51                   push ecx
005394f3 8b4d18               mov ecx, dword ptr [ebp + 0x18]
005394f6 8945d4               mov dword ptr [ebp - 0x2c], eax
005394f9 8b867c010000         mov eax, dword ptr [esi + 0x17c]
005394ff 8d4c010c             lea ecx, [ecx + eax + 0xc]
00539503 895df8               mov dword ptr [ebp - 8], ebx
00539506 e8a640efff           call 0x42d5b1
0053950b 8b4810               mov ecx, dword ptr [eax + 0x10]
0053950e 894dc8               mov dword ptr [ebp - 0x38], ecx
00539511 8b4014               mov eax, dword ptr [eax + 0x14]
00539514 8d4df8               lea ecx, [ebp - 8]
00539517 51                   push ecx
00539518 8b4d18               mov ecx, dword ptr [ebp + 0x18]
0053951b 8945cc               mov dword ptr [ebp - 0x34], eax
0053951e 8b867c010000         mov eax, dword ptr [esi + 0x17c]
00539524 8d4c010c             lea ecx, [ecx + eax + 0xc]
00539528 895df8               mov dword ptr [ebp - 8], ebx
0053952b e88140efff           call 0x42d5b1
00539530 8b4018               mov eax, dword ptr [eax + 0x18]
00539533 eb5a                 jmp 0x53958f
00539535 8b8664010000         mov eax, dword ptr [esi + 0x164]
0053953b 8b4cb804             mov ecx, dword ptr [eax + edi*4 + 4]
0053953f 8b01                 mov eax, dword ptr [ecx]
00539541 8d55d8               lea edx, [ebp - 0x28]
00539544 52                   push edx
00539545 ff90dc000000         call dword ptr [eax + 0xdc]
0053954b 8b08                 mov ecx, dword ptr [eax]
0053954d 894dd0               mov dword ptr [ebp - 0x30], ecx
00539550 8b4004               mov eax, dword ptr [eax + 4]
00539553 8945d4               mov dword ptr [ebp - 0x2c], eax
00539556 8b8664010000         mov eax, dword ptr [esi + 0x164]
0053955c 8b4cb804             mov ecx, dword ptr [eax + edi*4 + 4]
00539560 8b01                 mov eax, dword ptr [ecx]
00539562 8d55d8               lea edx, [ebp - 0x28]
00539565 52                   push edx
00539566 ff90e4000000         call dword ptr [eax + 0xe4]
0053956c 8b08                 mov ecx, dword ptr [eax]
0053956e 894dc8               mov dword ptr [ebp - 0x38], ecx
00539571 8b4004               mov eax, dword ptr [eax + 4]
00539574 8945cc               mov dword ptr [ebp - 0x34], eax
00539577 8b8664010000         mov eax, dword ptr [esi + 0x164]
0053957d 8b4cb804             mov ecx, dword ptr [eax + edi*4 + 4]
00539581 8b01                 mov eax, dword ptr [ecx]
00539583 8d5518               lea edx, [ebp + 0x18]
00539586 52                   push edx
00539587 ff90e0000000         call dword ptr [eax + 0xe0]
0053958d 8b00                 mov eax, dword ptr [eax]
0053958f 894514               mov dword ptr [ebp + 0x14], eax
00539592 8b4508               mov eax, dword ptr [ebp + 8]
00539595 3bc7                 cmp eax, edi
00539597 894508               mov dword ptr [ebp + 8], eax
0053959a 0f8364050000         jae 0x539b04
005395a0 8d0440               lea eax, [eax + eax*2]
005395a3 c1e002               shl eax, 2
005395a6 894518               mov dword ptr [ebp + 0x18], eax
005395a9 8b7d08               mov edi, dword ptr [ebp + 8]
005395ac d945fc               fld dword ptr [ebp - 4]
005395af 8b8664010000         mov eax, dword ptr [esi + 0x164]
005395b5 c1e702               shl edi, 2
005395b8 03c7                 add eax, edi
005395ba 8b4804               mov ecx, dword ptr [eax + 4]
005395bd 8b00                 mov eax, dword ptr [eax]
005395bf 8b18                 mov ebx, dword ptr [eax]
005395c1 6a00                 push 0
005395c3 51                   push ecx
005395c4 8d55c0               lea edx, [ebp - 0x40]
005395c7 d91c24               fstp dword ptr [esp]
005395ca 8945f4               mov dword ptr [ebp - 0xc], eax
005395cd 8b01                 mov eax, dword ptr [ecx]
005395cf 52                   push edx
005395d0 ff90dc000000         call dword ptr [eax + 0xdc]
005395d6 8b4df4               mov ecx, dword ptr [ebp - 0xc]
005395d9 50                   push eax
005395da ff537c               call dword ptr [ebx + 0x7c]
005395dd 8b8664010000         mov eax, dword ptr [esi + 0x164]
005395e3 8b0c07               mov ecx, dword ptr [edi + eax]
005395e6 8b01                 mov eax, dword ptr [ecx]
005395e8 8d55f8               lea edx, [ebp - 8]
005395eb 52                   push edx
005395ec ff90e0000000         call dword ptr [eax + 0xe0]
005395f2 8b8664010000         mov eax, dword ptr [esi + 0x164]
005395f8 8b4c0704             mov ecx, dword ptr [edi + eax + 4]
005395fc 8b01                 mov eax, dword ptr [ecx]
005395fe 8d55e8               lea edx, [ebp - 0x18]
00539601 52                   push edx
00539602 ff90e0000000         call dword ptr [eax + 0xe0]
00539608 d945fc               fld dword ptr [ebp - 4]
0053960b 8a4003               mov al, byte ptr [eax + 3]
0053960e 8845fb               mov byte ptr [ebp - 5], al
00539611 8b8664010000         mov eax, dword ptr [esi + 0x164]
00539617 8b0c07               mov ecx, dword ptr [edi + eax]
0053961a 8b01                 mov eax, dword ptr [ecx]
0053961c 6a00                 push 0
0053961e 51                   push ecx
0053961f 8d55f8               lea edx, [ebp - 8]
00539622 d91c24               fstp dword ptr [esp]
00539625 52                   push edx
00539626 ff9098000000         call dword ptr [eax + 0x98]
0053962c d945fc               fld dword ptr [ebp - 4]
0053962f 8b8664010000         mov eax, dword ptr [esi + 0x164]
00539635 03c7                 add eax, edi
00539637 8b4804               mov ecx, dword ptr [eax + 4]
0053963a 8b00                 mov eax, dword ptr [eax]
0053963c 8b18                 mov ebx, dword ptr [eax]
0053963e 6a00                 push 0
00539640 51                   push ecx
00539641 8d55b8               lea edx, [ebp - 0x48]
00539644 d91c24               fstp dword ptr [esp]
00539647 8945f4               mov dword ptr [ebp - 0xc], eax
0053964a 8b01                 mov eax, dword ptr [ecx]
0053964c 52                   push edx
0053964d ff90e4000000         call dword ptr [eax + 0xe4]
00539653 8b4df4               mov ecx, dword ptr [ebp - 0xc]
00539656 50                   push eax
00539657 ff938c000000         call dword ptr [ebx + 0x8c]
0053965d 807d1000             cmp byte ptr [ebp + 0x10], 0
00539661 0f8424020000         je 0x53988b
00539667 8b9e7c010000         mov ebx, dword ptr [esi + 0x17c]
0053966d 8b7d18               mov edi, dword ptr [ebp + 0x18]
00539670 33c0                 xor eax, eax
00539672 40                   inc eax
00539673 8945f4               mov dword ptr [ebp - 0xc], eax
00539676 8945f0               mov dword ptr [ebp - 0x10], eax
00539679 8d45f4               lea eax, [ebp - 0xc]
0053967c 50                   push eax
0053967d 8d4c3b0c             lea ecx, [ebx + edi + 0xc]
00539681 e82b3fefff           call 0x42d5b1
00539686 8b4808               mov ecx, dword ptr [eax + 8]
00539689 8b400c               mov eax, dword ptr [eax + 0xc]
0053968c 8d55f0               lea edx, [ebp - 0x10]
0053968f 894df4               mov dword ptr [ebp - 0xc], ecx
00539692 52                   push edx
00539693 8d0c3b               lea ecx, [ebx + edi]
00539696 8945ec               mov dword ptr [ebp - 0x14], eax
00539699 e8133fefff           call 0x42d5b1
0053969e 8b4df4               mov ecx, dword ptr [ebp - 0xc]
005396a1 894808               mov dword ptr [eax + 8], ecx
005396a4 8b4dec               mov ecx, dword ptr [ebp - 0x14]
005396a7 89480c               mov dword ptr [eax + 0xc], ecx
005396aa 8b9e7c010000         mov ebx, dword ptr [esi + 0x17c]
005396b0 6a04                 push 4
005396b2 58                   pop eax
005396b3 8945ec               mov dword ptr [ebp - 0x14], eax
005396b6 8945f0               mov dword ptr [ebp - 0x10], eax
005396b9 8d45ec               lea eax, [ebp - 0x14]
005396bc 50                   push eax
005396bd 8d4c3b0c             lea ecx, [ebx + edi + 0xc]
005396c1 e8eb3eefff           call 0x42d5b1
005396c6 8b4808               mov ecx, dword ptr [eax + 8]
005396c9 8b400c               mov eax, dword ptr [eax + 0xc]
005396cc 8d55f0               lea edx, [ebp - 0x10]
005396cf 894dec               mov dword ptr [ebp - 0x14], ecx
005396d2 52                   push edx
005396d3 8d0c3b               lea ecx, [ebx + edi]
005396d6 8945f4               mov dword ptr [ebp - 0xc], eax
005396d9 e8d33eefff           call 0x42d5b1
005396de 8b4dec               mov ecx, dword ptr [ebp - 0x14]
005396e1 894808               mov dword ptr [eax + 8], ecx
005396e4 8b4df4               mov ecx, dword ptr [ebp - 0xc]
005396e7 89480c               mov dword ptr [eax + 0xc], ecx
005396ea 8b9e7c010000         mov ebx, dword ptr [esi + 0x17c]
005396f0 6a05                 push 5
005396f2 58                   pop eax
005396f3 8945ec               mov dword ptr [ebp - 0x14], eax
005396f6 8945f0               mov dword ptr [ebp - 0x10], eax
005396f9 8d45ec               lea eax, [ebp - 0x14]
005396fc 50                   push eax
005396fd 8d4c3b0c             lea ecx, [ebx + edi + 0xc]
00539701 e8ab3eefff           call 0x42d5b1
00539706 8b4808               mov ecx, dword ptr [eax + 8]
00539709 8b400c               mov eax, dword ptr [eax + 0xc]
0053970c 8d55f0               lea edx, [ebp - 0x10]
0053970f 894dec               mov dword ptr [ebp - 0x14], ecx
00539712 52                   push edx
00539713 8d0c3b               lea ecx, [ebx + edi]
00539716 8945f4               mov dword ptr [ebp - 0xc], eax
00539719 e8933eefff           call 0x42d5b1
0053971e 8b4dec               mov ecx, dword ptr [ebp - 0x14]
00539721 894808               mov dword ptr [eax + 8], ecx
00539724 8b4df4               mov ecx, dword ptr [ebp - 0xc]
00539727 89480c               mov dword ptr [eax + 0xc], ecx
0053972a 8b9e7c010000         mov ebx, dword ptr [esi + 0x17c]
00539730 33c0                 xor eax, eax
00539732 40                   inc eax
00539733 8945ec               mov dword ptr [ebp - 0x14], eax
00539736 8945f0               mov dword ptr [ebp - 0x10], eax
00539739 8d45ec               lea eax, [ebp - 0x14]
0053973c 50                   push eax
0053973d 8d4c3b0c             lea ecx, [ebx + edi + 0xc]
00539741 e86b3eefff           call 0x42d5b1
00539746 8d4df0               lea ecx, [ebp - 0x10]
00539749 51                   push ecx
0053974a 8d0c3b               lea ecx, [ebx + edi]
0053974d 8945ec               mov dword ptr [ebp - 0x14], eax
00539750 e85c3eefff           call 0x42d5b1
00539755 8b4dec               mov ecx, dword ptr [ebp - 0x14]
00539758 8a491b               mov cl, byte ptr [ecx + 0x1b]
0053975b 88481b               mov byte ptr [eax + 0x1b], cl
0053975e 8b9e7c010000         mov ebx, dword ptr [esi + 0x17c]
00539764 6a04                 push 4
00539766 58                   pop eax
00539767 8945ec               mov dword ptr [ebp - 0x14], eax
0053976a 8945f0               mov dword ptr [ebp - 0x10], eax
0053976d 8d45ec               lea eax, [ebp - 0x14]
00539770 50                   push eax
00539771 8d4c3b0c             lea ecx, [ebx + edi + 0xc]
00539775 e8373eefff           call 0x42d5b1
0053977a 8d4df0               lea ecx, [ebp - 0x10]
0053977d 51                   push ecx
0053977e 8d0c3b               lea ecx, [ebx + edi]
00539781 8945ec               mov dword ptr [ebp - 0x14], eax
00539784 e8283eefff           call 0x42d5b1
00539789 8b4dec               mov ecx, dword ptr [ebp - 0x14]
0053978c 8a491b               mov cl, byte ptr [ecx + 0x1b]
0053978f 88481b               mov byte ptr [eax + 0x1b], cl
00539792 8b9e7c010000         mov ebx, dword ptr [esi + 0x17c]
00539798 6a05                 push 5
0053979a 58                   pop eax
0053979b 8945ec               mov dword ptr [ebp - 0x14], eax
0053979e 8945f0               mov dword ptr [ebp - 0x10], eax
005397a1 8d45ec               lea eax, [ebp - 0x14]
005397a4 50                   push eax
005397a5 8d4c3b0c             lea ecx, [ebx + edi + 0xc]
005397a9 e8033eefff           call 0x42d5b1
005397ae 8d4df0               lea ecx, [ebp - 0x10]
005397b1 51                   push ecx
005397b2 8d0c3b               lea ecx, [ebx + edi]
005397b5 8945ec               mov dword ptr [ebp - 0x14], eax
005397b8 e8f43defff           call 0x42d5b1
005397bd 8b4dec               mov ecx, dword ptr [ebp - 0x14]
005397c0 8a491b               mov cl, byte ptr [ecx + 0x1b]
005397c3 88481b               mov byte ptr [eax + 0x1b], cl
005397c6 8b9e7c010000         mov ebx, dword ptr [esi + 0x17c]
005397cc 33c0                 xor eax, eax
005397ce 40                   inc eax
005397cf 8945ec               mov dword ptr [ebp - 0x14], eax
005397d2 8945f0               mov dword ptr [ebp - 0x10], eax
005397d5 8d45ec               lea eax, [ebp - 0x14]
005397d8 50                   push eax
005397d9 8d4c3b0c             lea ecx, [ebx + edi + 0xc]
005397dd e8cf3defff           call 0x42d5b1
005397e2 8b4810               mov ecx, dword ptr [eax + 0x10]
005397e5 8b4014               mov eax, dword ptr [eax + 0x14]
005397e8 8d55f0               lea edx, [ebp - 0x10]
005397eb 894dec               mov dword ptr [ebp - 0x14], ecx
005397ee 52                   push edx
005397ef 8d0c3b               lea ecx, [ebx + edi]
005397f2 8945f4               mov dword ptr [ebp - 0xc], eax
005397f5 e8b73defff           call 0x42d5b1
005397fa 8b4dec               mov ecx, dword ptr [ebp - 0x14]
005397fd 894810               mov dword ptr [eax + 0x10], ecx
00539800 8b4df4               mov ecx, dword ptr [ebp - 0xc]
00539803 894814               mov dword ptr [eax + 0x14], ecx
00539806 8b9e7c010000         mov ebx, dword ptr [esi + 0x17c]
0053980c 6a04                 push 4
0053980e 58                   pop eax
0053980f 8945ec               mov dword ptr [ebp - 0x14], eax
00539812 8945f0               mov dword ptr [ebp - 0x10], eax
00539815 8d45ec               lea eax, [ebp - 0x14]
00539818 50                   push eax
00539819 8d4c3b0c             lea ecx, [ebx + edi + 0xc]
0053981d e88f3defff           call 0x42d5b1
00539822 8b4810               mov ecx, dword ptr [eax + 0x10]
00539825 8b4014               mov eax, dword ptr [eax + 0x14]
00539828 8d55f0               lea edx, [ebp - 0x10]
0053982b 894dec               mov dword ptr [ebp - 0x14], ecx
0053982e 52                   push edx
0053982f 8d0c3b               lea ecx, [ebx + edi]
00539832 8945f4               mov dword ptr [ebp - 0xc], eax
00539835 e8773defff           call 0x42d5b1
0053983a 8b4dec               mov ecx, dword ptr [ebp - 0x14]
0053983d 894810               mov dword ptr [eax + 0x10], ecx
00539840 8b4df4               mov ecx, dword ptr [ebp - 0xc]
00539843 894814               mov dword ptr [eax + 0x14], ecx
00539846 8b9e7c010000         mov ebx, dword ptr [esi + 0x17c]
0053984c 6a05                 push 5
0053984e 58                   pop eax
0053984f 8945ec               mov dword ptr [ebp - 0x14], eax
00539852 8945f0               mov dword ptr [ebp - 0x10], eax
00539855 8d45ec               lea eax, [ebp - 0x14]
00539858 50                   push eax
00539859 8d4c3b0c             lea ecx, [ebx + edi + 0xc]
0053985d e84f3defff           call 0x42d5b1
00539862 8b4810               mov ecx, dword ptr [eax + 0x10]
00539865 8b4014               mov eax, dword ptr [eax + 0x14]
00539868 8d55f0               lea edx, [ebp - 0x10]
0053986b 894dec               mov dword ptr [ebp - 0x14], ecx
0053986e 52                   push edx
0053986f 8d0c3b               lea ecx, [ebx + edi]
00539872 8945f4               mov dword ptr [ebp - 0xc], eax
00539875 e8373defff           call 0x42d5b1
0053987a 8b4dec               mov ecx, dword ptr [ebp - 0x14]
0053987d 894810               mov dword ptr [eax + 0x10], ecx
00539880 8b4df4               mov ecx, dword ptr [ebp - 0xc]
00539883 894814               mov dword ptr [eax + 0x14], ecx
00539886 e963020000           jmp 0x539aee
0053988b 8b8664010000         mov eax, dword ptr [esi + 0x164]
00539891 8b4c0704             mov ecx, dword ptr [edi + eax + 4]
00539895 8b01                 mov eax, dword ptr [ecx]
00539897 8bbe7c010000         mov edi, dword ptr [esi + 0x17c]
0053989d 8d55b0               lea edx, [ebp - 0x50]
005398a0 52                   push edx
005398a1 c745ec01000000       mov dword ptr [ebp - 0x14], 1
005398a8 ff90dc000000         call dword ptr [eax + 0xdc]
005398ae 8b18                 mov ebx, dword ptr [eax]
005398b0 8b4004               mov eax, dword ptr [eax + 4]
005398b3 8d4dec               lea ecx, [ebp - 0x14]
005398b6 51                   push ecx
005398b7 8b4d18               mov ecx, dword ptr [ebp + 0x18]
005398ba 03cf                 add ecx, edi
005398bc 8945f0               mov dword ptr [ebp - 0x10], eax
005398bf e8ed3cefff           call 0x42d5b1
005398c4 895808               mov dword ptr [eax + 8], ebx
005398c7 8b5df0               mov ebx, dword ptr [ebp - 0x10]
005398ca 89580c               mov dword ptr [eax + 0xc], ebx
005398cd 8b4508               mov eax, dword ptr [ebp + 8]
005398d0 8b8e64010000         mov ecx, dword ptr [esi + 0x164]
005398d6 8bbe7c010000         mov edi, dword ptr [esi + 0x17c]
005398dc c1e002               shl eax, 2
005398df 8b4c0804             mov ecx, dword ptr [eax + ecx + 4]
005398e3 8b01                 mov eax, dword ptr [ecx]
005398e5 8d55a8               lea edx, [ebp - 0x58]
005398e8 52                   push edx
005398e9 c745ec04000000       mov dword ptr [ebp - 0x14], 4
005398f0 ff90dc000000         call dword ptr [eax + 0xdc]
005398f6 8b18                 mov ebx, dword ptr [eax]
005398f8 8b4004               mov eax, dword ptr [eax + 4]
005398fb 8d4dec               lea ecx, [ebp - 0x14]
005398fe 51                   push ecx
005398ff 8b4d18               mov ecx, dword ptr [ebp + 0x18]
00539902 03cf                 add ecx, edi
00539904 8945f0               mov dword ptr [ebp - 0x10], eax
00539907 e8a53cefff           call 0x42d5b1
0053990c 895808               mov dword ptr [eax + 8], ebx
0053990f 8b5df0               mov ebx, dword ptr [ebp - 0x10]
00539912 89580c               mov dword ptr [eax + 0xc], ebx
00539915 8b4508               mov eax, dword ptr [ebp + 8]
00539918 8b8e64010000         mov ecx, dword ptr [esi + 0x164]
0053991e 8bbe7c010000         mov edi, dword ptr [esi + 0x17c]
00539924 c1e002               shl eax, 2
00539927 8b4c0804             mov ecx, dword ptr [eax + ecx + 4]
0053992b 8b01                 mov eax, dword ptr [ecx]
0053992d 8d55a0               lea edx, [ebp - 0x60]
00539930 52                   push edx
00539931 c745ec05000000       mov dword ptr [ebp - 0x14], 5
00539938 ff90dc000000         call dword ptr [eax + 0xdc]
0053993e 8b18                 mov ebx, dword ptr [eax]
00539940 8b4004               mov eax, dword ptr [eax + 4]
00539943 8d4dec               lea ecx, [ebp - 0x14]
00539946 51                   push ecx
00539947 8b4d18               mov ecx, dword ptr [ebp + 0x18]
0053994a 03cf                 add ecx, edi
0053994c 8945f0               mov dword ptr [ebp - 0x10], eax
0053994f e85d3cefff           call 0x42d5b1
00539954 8b7d08               mov edi, dword ptr [ebp + 8]
00539957 895808               mov dword ptr [eax + 8], ebx
0053995a 8b5df0               mov ebx, dword ptr [ebp - 0x10]
0053995d 89580c               mov dword ptr [eax + 0xc], ebx
00539960 8b8664010000         mov eax, dword ptr [esi + 0x164]
00539966 8b9e7c010000         mov ebx, dword ptr [esi + 0x17c]
0053996c c1e702               shl edi, 2
0053996f 8b4c0704             mov ecx, dword ptr [edi + eax + 4]
00539973 8b01                 mov eax, dword ptr [ecx]
00539975 8d55e4               lea edx, [ebp - 0x1c]
00539978 52                   push edx
00539979 c745ec01000000       mov dword ptr [ebp - 0x14], 1
00539980 ff90e0000000         call dword ptr [eax + 0xe0]
00539986 8d4dec               lea ecx, [ebp - 0x14]
00539989 51                   push ecx
0053998a 8b4d18               mov ecx, dword ptr [ebp + 0x18]
0053998d 03cb                 add ecx, ebx
0053998f 8945f0               mov dword ptr [ebp - 0x10], eax
00539992 e81a3cefff           call 0x42d5b1
00539997 8b4df0               mov ecx, dword ptr [ebp - 0x10]
0053999a 8a4903               mov cl, byte ptr [ecx + 3]
0053999d 88481b               mov byte ptr [eax + 0x1b], cl
005399a0 c745ec04000000       mov dword ptr [ebp - 0x14], 4
005399a7 8b8664010000         mov eax, dword ptr [esi + 0x164]
005399ad 8b4c0704             mov ecx, dword ptr [edi + eax + 4]
005399b1 8b01                 mov eax, dword ptr [ecx]
005399b3 8b9e7c010000         mov ebx, dword ptr [esi + 0x17c]
005399b9 8d55e0               lea edx, [ebp - 0x20]
005399bc 52                   push edx
005399bd ff90e0000000         call dword ptr [eax + 0xe0]
005399c3 8d4dec               lea ecx, [ebp - 0x14]
005399c6 51                   push ecx
005399c7 8b4d18               mov ecx, dword ptr [ebp + 0x18]
005399ca 03cb                 add ecx, ebx
005399cc 8945f0               mov dword ptr [ebp - 0x10], eax
005399cf e8dd3befff           call 0x42d5b1
005399d4 8b4df0               mov ecx, dword ptr [ebp - 0x10]
005399d7 8a4903               mov cl, byte ptr [ecx + 3]
005399da 88481b               mov byte ptr [eax + 0x1b], cl
005399dd 8b8664010000         mov eax, dword ptr [esi + 0x164]
005399e3 8b4c0704             mov ecx, dword ptr [edi + eax + 4]
005399e7 8b01                 mov eax, dword ptr [ecx]
005399e9 8b9e7c010000         mov ebx, dword ptr [esi + 0x17c]
005399ef 8d55dc               lea edx, [ebp - 0x24]
005399f2 52                   push edx
005399f3 c745ec05000000       mov dword ptr [ebp - 0x14], 5
005399fa ff90e0000000         call dword ptr [eax + 0xe0]
00539a00 8d4dec               lea ecx, [ebp - 0x14]
00539a03 51                   push ecx
00539a04 8b4d18               mov ecx, dword ptr [ebp + 0x18]
00539a07 03cb                 add ecx, ebx
00539a09 8945f0               mov dword ptr [ebp - 0x10], eax
00539a0c e8a03befff           call 0x42d5b1
00539a11 8b4df0               mov ecx, dword ptr [ebp - 0x10]
00539a14 8a4903               mov cl, byte ptr [ecx + 3]
00539a17 88481b               mov byte ptr [eax + 0x1b], cl
00539a1a 8b8664010000         mov eax, dword ptr [esi + 0x164]
00539a20 8b7c0704             mov edi, dword ptr [edi + eax + 4]
00539a24 8b07                 mov eax, dword ptr [edi]
00539a26 8b9e7c010000         mov ebx, dword ptr [esi + 0x17c]
00539a2c 8d4d98               lea ecx, [ebp - 0x68]
00539a2f 51                   push ecx
00539a30 8bcf                 mov ecx, edi
00539a32 c745ec01000000       mov dword ptr [ebp - 0x14], 1
00539a39 ff90e4000000         call dword ptr [eax + 0xe4]
00539a3f 8b38                 mov edi, dword ptr [eax]
00539a41 8b4004               mov eax, dword ptr [eax + 4]
00539a44 8d4dec               lea ecx, [ebp - 0x14]
00539a47 51                   push ecx
00539a48 8b4d18               mov ecx, dword ptr [ebp + 0x18]
00539a4b 03cb                 add ecx, ebx
00539a4d 8945f0               mov dword ptr [ebp - 0x10], eax
00539a50 e85c3befff           call 0x42d5b1
00539a55 897810               mov dword ptr [eax + 0x10], edi
00539a58 8b7df0               mov edi, dword ptr [ebp - 0x10]
00539a5b 897814               mov dword ptr [eax + 0x14], edi
00539a5e 8b4508               mov eax, dword ptr [ebp + 8]
00539a61 8b8e64010000         mov ecx, dword ptr [esi + 0x164]
00539a67 8bbe7c010000         mov edi, dword ptr [esi + 0x17c]
00539a6d c1e002               shl eax, 2
00539a70 8b4c0804             mov ecx, dword ptr [eax + ecx + 4]
00539a74 8b01                 mov eax, dword ptr [ecx]
00539a76 8d5590               lea edx, [ebp - 0x70]
00539a79 52                   push edx
00539a7a c745ec04000000       mov dword ptr [ebp - 0x14], 4
00539a81 ff90e4000000         call dword ptr [eax + 0xe4]
00539a87 8b18                 mov ebx, dword ptr [eax]
00539a89 8b4004               mov eax, dword ptr [eax + 4]
00539a8c 8d4dec               lea ecx, [ebp - 0x14]
00539a8f 51                   push ecx
00539a90 8b4d18               mov ecx, dword ptr [ebp + 0x18]
00539a93 03cf                 add ecx, edi
00539a95 8945f0               mov dword ptr [ebp - 0x10], eax
00539a98 e8143befff           call 0x42d5b1
00539a9d 895810               mov dword ptr [eax + 0x10], ebx
00539aa0 8b5df0               mov ebx, dword ptr [ebp - 0x10]
00539aa3 895814               mov dword ptr [eax + 0x14], ebx
00539aa6 8b4508               mov eax, dword ptr [ebp + 8]
00539aa9 8b8e64010000         mov ecx, dword ptr [esi + 0x164]
00539aaf 8bbe7c010000         mov edi, dword ptr [esi + 0x17c]
00539ab5 c1e002               shl eax, 2
00539ab8 8b4c0804             mov ecx, dword ptr [eax + ecx + 4]
00539abc 8b01                 mov eax, dword ptr [ecx]
00539abe c745ec05000000       mov dword ptr [ebp - 0x14], 5
00539ac5 8d5588               lea edx, [ebp - 0x78]
00539ac8 52                   push edx
00539ac9 ff90e4000000         call dword ptr [eax + 0xe4]
00539acf 8b18                 mov ebx, dword ptr [eax]
00539ad1 8b4004               mov eax, dword ptr [eax + 4]
00539ad4 8d4dec               lea ecx, [ebp - 0x14]
00539ad7 51                   push ecx
00539ad8 8b4d18               mov ecx, dword ptr [ebp + 0x18]
00539adb 03cf                 add ecx, edi
00539add 8945f0               mov dword ptr [ebp - 0x10], eax
00539ae0 e8cc3aefff           call 0x42d5b1
00539ae5 895810               mov dword ptr [eax + 0x10], ebx
00539ae8 8b5df0               mov ebx, dword ptr [ebp - 0x10]
00539aeb 895814               mov dword ptr [eax + 0x14], ebx
00539aee 8b7d0c               mov edi, dword ptr [ebp + 0xc]
00539af1 ff4508               inc dword ptr [ebp + 8]
00539af4 8345180c             add dword ptr [ebp + 0x18], 0xc
00539af8 397d08               cmp dword ptr [ebp + 8], edi
00539afb 0f82a8faffff         jb 0x5395a9
00539b01 6a05                 push 5
00539b03 5b                   pop ebx
00539b04 8b06                 mov eax, dword ptr [esi]
00539b06 8bce                 mov ecx, esi
00539b08 ff90a8020000         call dword ptr [eax + 0x2a8]
00539b0e 84c0                 test al, al
00539b10 0f854a030000         jne 0x539e60
00539b16 8b8668010000         mov eax, dword ptr [esi + 0x168]
00539b1c 2b8664010000         sub eax, dword ptr [esi + 0x164]
00539b22 c1f802               sar eax, 2
00539b25 48                   dec eax
00539b26 3bf8                 cmp edi, eax
00539b28 0f8532030000         jne 0x539e60
00539b2e 807d1000             cmp byte ptr [ebp + 0x10], 0
00539b32 0f841c010000         je 0x539c54
00539b38 8b8e7c010000         mov ecx, dword ptr [esi + 0x17c]
00539b3e 8d047f               lea eax, [edi + edi*2]
00539b41 c1e002               shl eax, 2
00539b44 8d5510               lea edx, [ebp + 0x10]
00539b47 52                   push edx
00539b48 03c8                 add ecx, eax
00539b4a 895d10               mov dword ptr [ebp + 0x10], ebx
00539b4d 894518               mov dword ptr [ebp + 0x18], eax
00539b50 e85c3aefff           call 0x42d5b1
00539b55 d9400c               fld dword ptr [eax + 0xc]
00539b58 d8868c010000         fadd dword ptr [esi + 0x18c]
00539b5e 8b867c010000         mov eax, dword ptr [esi + 0x17c]
00539b64 8d4d10               lea ecx, [ebp + 0x10]
00539b67 51                   push ecx
00539b68 8b4d18               mov ecx, dword ptr [ebp + 0x18]
00539b6b d95dd4               fstp dword ptr [ebp - 0x2c]
00539b6e 03c8                 add ecx, eax
00539b70 895d10               mov dword ptr [ebp + 0x10], ebx
00539b73 e8393aefff           call 0x42d5b1
00539b78 d94008               fld dword ptr [eax + 8]
00539b7b d88688010000         fadd dword ptr [esi + 0x188]
00539b81 8b867c010000         mov eax, dword ptr [esi + 0x17c]
00539b87 8d4d10               lea ecx, [ebp + 0x10]
00539b8a 51                   push ecx
00539b8b 8b4d18               mov ecx, dword ptr [ebp + 0x18]
00539b8e d95dd0               fstp dword ptr [ebp - 0x30]
00539b91 03c8                 add ecx, eax
00539b93 895d10               mov dword ptr [ebp + 0x10], ebx
00539b96 e8163aefff           call 0x42d5b1
00539b9b 0fb6401b             movzx eax, byte ptr [eax + 0x1b]
00539b9f 894510               mov dword ptr [ebp + 0x10], eax
00539ba2 db4510               fild dword ptr [ebp + 0x10]
00539ba5 d98698010000         fld dword ptr [esi + 0x198]
00539bab d80d14002301         fmul dword ptr [0x1230014]
00539bb1 dee9                 fsubp st(1)
00539bb3 d81ddcde2201         fcomp dword ptr [0x122dedc]
00539bb9 dfe0                 fnstsw ax
00539bbb f6c405               test ah, 5
00539bbe 7a04                 jp 0x539bc4
00539bc0 d9ee                 fldz 
00539bc2 eb2f                 jmp 0x539bf3
00539bc4 8b867c010000         mov eax, dword ptr [esi + 0x17c]
00539bca 8d4d10               lea ecx, [ebp + 0x10]
00539bcd 51                   push ecx
00539bce 8b4d18               mov ecx, dword ptr [ebp + 0x18]
00539bd1 03c8                 add ecx, eax
00539bd3 895d10               mov dword ptr [ebp + 0x10], ebx
00539bd6 e8d639efff           call 0x42d5b1
00539bdb 0fb6401b             movzx eax, byte ptr [eax + 0x1b]
00539bdf 894510               mov dword ptr [ebp + 0x10], eax
00539be2 db4510               fild dword ptr [ebp + 0x10]
00539be5 d98698010000         fld dword ptr [esi + 0x198]
00539beb d80d14002301         fmul dword ptr [0x1230014]
00539bf1 dee9                 fsubp st(1)
00539bf3 e8784e6c00           call 0xbfea70
00539bf8 8d4d10               lea ecx, [ebp + 0x10]
00539bfb 51                   push ecx
00539bfc 8b4d18               mov ecx, dword ptr [ebp + 0x18]
00539bff 884517               mov byte ptr [ebp + 0x17], al
00539c02 8b867c010000         mov eax, dword ptr [esi + 0x17c]
00539c08 03c8                 add ecx, eax
00539c0a 895d10               mov dword ptr [ebp + 0x10], ebx
00539c0d e89f39efff           call 0x42d5b1
00539c12 8a401a               mov al, byte ptr [eax + 0x1a]
00539c15 8d4d10               lea ecx, [ebp + 0x10]
00539c18 51                   push ecx
00539c19 8b4d18               mov ecx, dword ptr [ebp + 0x18]
00539c1c 884516               mov byte ptr [ebp + 0x16], al
00539c1f 8b867c010000         mov eax, dword ptr [esi + 0x17c]
00539c25 03c8                 add ecx, eax
00539c27 895d10               mov dword ptr [ebp + 0x10], ebx
00539c2a e88239efff           call 0x42d5b1
00539c2f 8a4019               mov al, byte ptr [eax + 0x19]
00539c32 8d4d10               lea ecx, [ebp + 0x10]
00539c35 51                   push ecx
00539c36 8b4d18               mov ecx, dword ptr [ebp + 0x18]
00539c39 884515               mov byte ptr [ebp + 0x15], al
00539c3c 8b867c010000         mov eax, dword ptr [esi + 0x17c]
00539c42 03c8                 add ecx, eax
00539c44 895d10               mov dword ptr [ebp + 0x10], ebx
00539c47 e86539efff           call 0x42d5b1
00539c4c 8a4018               mov al, byte ptr [eax + 0x18]
00539c4f e901010000           jmp 0x539d55
00539c54 8b8664010000         mov eax, dword ptr [esi + 0x164]
00539c5a 8b0cb8               mov ecx, dword ptr [eax + edi*4]
00539c5d 8b01                 mov eax, dword ptr [ecx]
00539c5f 8d5588               lea edx, [ebp - 0x78]
00539c62 52                   push edx
00539c63 ff90dc000000         call dword ptr [eax + 0xdc]
00539c69 d94004               fld dword ptr [eax + 4]
00539c6c d8868c010000         fadd dword ptr [esi + 0x18c]
00539c72 8b8664010000         mov eax, dword ptr [esi + 0x164]
00539c78 8d5588               lea edx, [ebp - 0x78]
00539c7b 52                   push edx
00539c7c d95dd4               fstp dword ptr [ebp - 0x2c]
00539c7f 8b0cb8               mov ecx, dword ptr [eax + edi*4]
00539c82 8b01                 mov eax, dword ptr [ecx]
00539c84 ff90dc000000         call dword ptr [eax + 0xdc]
00539c8a d98688010000         fld dword ptr [esi + 0x188]
00539c90 d800                 fadd dword ptr [eax]
00539c92 8b8664010000         mov eax, dword ptr [esi + 0x164]
00539c98 8d5518               lea edx, [ebp + 0x18]
00539c9b 52                   push edx
00539c9c d95dd0               fstp dword ptr [ebp - 0x30]
00539c9f 8b0cb8               mov ecx, dword ptr [eax + edi*4]
00539ca2 8b01                 mov eax, dword ptr [ecx]
00539ca4 ff90e0000000         call dword ptr [eax + 0xe0]
00539caa 0fb64003             movzx eax, byte ptr [eax + 3]
00539cae 894510               mov dword ptr [ebp + 0x10], eax
00539cb1 db4510               fild dword ptr [ebp + 0x10]
00539cb4 d98698010000         fld dword ptr [esi + 0x198]
00539cba d80d14002301         fmul dword ptr [0x1230014]
00539cc0 dee9                 fsubp st(1)
00539cc2 d81ddcde2201         fcomp dword ptr [0x122dedc]
00539cc8 dfe0                 fnstsw ax
00539cca f6c405               test ah, 5
00539ccd 7a04                 jp 0x539cd3
00539ccf d9ee                 fldz 
00539cd1 eb2d                 jmp 0x539d00
00539cd3 8b8664010000         mov eax, dword ptr [esi + 0x164]
00539cd9 8b0cb8               mov ecx, dword ptr [eax + edi*4]
00539cdc 8b01                 mov eax, dword ptr [ecx]
00539cde 8d5508               lea edx, [ebp + 8]
00539ce1 52                   push edx
00539ce2 ff90e0000000         call dword ptr [eax + 0xe0]
00539ce8 0fb64003             movzx eax, byte ptr [eax + 3]
00539cec 894510               mov dword ptr [ebp + 0x10], eax
00539cef db4510               fild dword ptr [ebp + 0x10]
00539cf2 d98698010000         fld dword ptr [esi + 0x198]
00539cf8 d80d14002301         fmul dword ptr [0x1230014]
00539cfe dee9                 fsubp st(1)
00539d00 e86b4d6c00           call 0xbfea70
00539d05 884517               mov byte ptr [ebp + 0x17], al
00539d08 8b8664010000         mov eax, dword ptr [esi + 0x164]
00539d0e 8b0cb8               mov ecx, dword ptr [eax + edi*4]
00539d11 8b01                 mov eax, dword ptr [ecx]
00539d13 8d5510               lea edx, [ebp + 0x10]
00539d16 52                   push edx
00539d17 ff90e0000000         call dword ptr [eax + 0xe0]
00539d1d 8a4002               mov al, byte ptr [eax + 2]
00539d20 884516               mov byte ptr [ebp + 0x16], al
00539d23 8b8664010000         mov eax, dword ptr [esi + 0x164]
00539d29 8b0cb8               mov ecx, dword ptr [eax + edi*4]
00539d2c 8b01                 mov eax, dword ptr [ecx]
00539d2e 8d5510               lea edx, [ebp + 0x10]
00539d31 52                   push edx
00539d32 ff90e0000000         call dword ptr [eax + 0xe0]
00539d38 8a4001               mov al, byte ptr [eax + 1]
00539d3b 884515               mov byte ptr [ebp + 0x15], al
00539d3e 8b8664010000         mov eax, dword ptr [esi + 0x164]
00539d44 8b0cb8               mov ecx, dword ptr [eax + edi*4]
00539d47 8b01                 mov eax, dword ptr [ecx]
00539d49 8d5510               lea edx, [ebp + 0x10]
00539d4c 52                   push edx
00539d4d ff90e0000000         call dword ptr [eax + 0xe0]
00539d53 8a00                 mov al, byte ptr [eax]
00539d55 d945fc               fld dword ptr [ebp - 4]
00539d58 884514               mov byte ptr [ebp + 0x14], al
00539d5b 8b8664010000         mov eax, dword ptr [esi + 0x164]
00539d61 8b0cb8               mov ecx, dword ptr [eax + edi*4]
00539d64 8b01                 mov eax, dword ptr [ecx]
00539d66 6a00                 push 0
00539d68 51                   push ecx
00539d69 8d55d0               lea edx, [ebp - 0x30]
00539d6c d91c24               fstp dword ptr [esp]
00539d6f 52                   push edx
00539d70 ff507c               call dword ptr [eax + 0x7c]
00539d73 d945fc               fld dword ptr [ebp - 4]
00539d76 8b8664010000         mov eax, dword ptr [esi + 0x164]
00539d7c 8b0cb8               mov ecx, dword ptr [eax + edi*4]
00539d7f 8b01                 mov eax, dword ptr [ecx]
00539d81 6a00                 push 0
00539d83 51                   push ecx
00539d84 8d5514               lea edx, [ebp + 0x14]
00539d87 d91c24               fstp dword ptr [esp]
00539d8a 52                   push edx
00539d8b ff9098000000         call dword ptr [eax + 0x98]
00539d91 8b867c010000         mov eax, dword ptr [esi + 0x17c]
00539d97 8d3c7f               lea edi, [edi + edi*2]
00539d9a 8d4d10               lea ecx, [ebp + 0x10]
00539d9d c1e702               shl edi, 2
00539da0 51                   push ecx
00539da1 8d0c07               lea ecx, [edi + eax]
00539da4 c7451001000000       mov dword ptr [ebp + 0x10], 1
00539dab e80138efff           call 0x42d5b1
00539db0 8b4dd0               mov ecx, dword ptr [ebp - 0x30]
00539db3 894808               mov dword ptr [eax + 8], ecx
00539db6 8b4dd4               mov ecx, dword ptr [ebp - 0x2c]
00539db9 89480c               mov dword ptr [eax + 0xc], ecx
00539dbc 8b867c010000         mov eax, dword ptr [esi + 0x17c]
00539dc2 8d4d10               lea ecx, [ebp + 0x10]
00539dc5 51                   push ecx
00539dc6 8d0c07               lea ecx, [edi + eax]
00539dc9 c7451004000000       mov dword ptr [ebp + 0x10], 4
00539dd0 e8dc37efff           call 0x42d5b1
00539dd5 8b4dd0               mov ecx, dword ptr [ebp - 0x30]
00539dd8 894808               mov dword ptr [eax + 8], ecx
00539ddb 8b4dd4               mov ecx, dword ptr [ebp - 0x2c]
00539dde 89480c               mov dword ptr [eax + 0xc], ecx
00539de1 8b867c010000         mov eax, dword ptr [esi + 0x17c]
00539de7 8d4d10               lea ecx, [ebp + 0x10]
00539dea 51                   push ecx
00539deb 8d0c07               lea ecx, [edi + eax]
00539dee 895d10               mov dword ptr [ebp + 0x10], ebx
00539df1 e8bb37efff           call 0x42d5b1
00539df6 8b4dd0               mov ecx, dword ptr [ebp - 0x30]
00539df9 894808               mov dword ptr [eax + 8], ecx
00539dfc 8b4dd4               mov ecx, dword ptr [ebp - 0x2c]
00539dff 89480c               mov dword ptr [eax + 0xc], ecx
00539e02 8b867c010000         mov eax, dword ptr [esi + 0x17c]
00539e08 8d4d10               lea ecx, [ebp + 0x10]
00539e0b 51                   push ecx
00539e0c 8d0c07               lea ecx, [edi + eax]
00539e0f c7451001000000       mov dword ptr [ebp + 0x10], 1
00539e16 e89637efff           call 0x42d5b1
00539e1b 8a4d17               mov cl, byte ptr [ebp + 0x17]
00539e1e 88481b               mov byte ptr [eax + 0x1b], cl
00539e21 8b867c010000         mov eax, dword ptr [esi + 0x17c]
00539e27 8d4d10               lea ecx, [ebp + 0x10]
00539e2a 51                   push ecx
00539e2b 8d0c07               lea ecx, [edi + eax]
00539e2e c7451004000000       mov dword ptr [ebp + 0x10], 4
00539e35 e87737efff           call 0x42d5b1
00539e3a 8a4d17               mov cl, byte ptr [ebp + 0x17]
00539e3d 88481b               mov byte ptr [eax + 0x1b], cl
00539e40 8bb67c010000         mov esi, dword ptr [esi + 0x17c]
00539e46 8d4510               lea eax, [ebp + 0x10]
00539e49 50                   push eax
00539e4a 8d0c37               lea ecx, [edi + esi]
00539e4d 895d10               mov dword ptr [ebp + 0x10], ebx
00539e50 e85c37efff           call 0x42d5b1
00539e55 8a4d17               mov cl, byte ptr [ebp + 0x17]
00539e58 88481b               mov byte ptr [eax + 0x1b], cl
00539e5b e93a020000           jmp 0x53a09a
00539e60 8b8664010000         mov eax, dword ptr [esi + 0x164]
00539e66 d945fc               fld dword ptr [ebp - 4]
00539e69 8b0cb8               mov ecx, dword ptr [eax + edi*4]
00539e6c 8b01                 mov eax, dword ptr [ecx]
00539e6e 6a00                 push 0
00539e70 51                   push ecx
00539e71 8d55d0               lea edx, [ebp - 0x30]
00539e74 d91c24               fstp dword ptr [esp]
00539e77 52                   push edx
00539e78 ff507c               call dword ptr [eax + 0x7c]
00539e7b 807d1000             cmp byte ptr [ebp + 0x10], 0
00539e7f 8d5510               lea edx, [ebp + 0x10]
00539e82 52                   push edx
00539e83 7458                 je 0x539edd
00539e85 8b8e7c010000         mov ecx, dword ptr [esi + 0x17c]
00539e8b 8d047f               lea eax, [edi + edi*2]
00539e8e c1e002               shl eax, 2
00539e91 03c8                 add ecx, eax
00539e93 895d10               mov dword ptr [ebp + 0x10], ebx
00539e96 894518               mov dword ptr [ebp + 0x18], eax
00539e99 e81337efff           call 0x42d5b1
00539e9e 8a401a               mov al, byte ptr [eax + 0x1a]
00539ea1 8d4d10               lea ecx, [ebp + 0x10]
00539ea4 51                   push ecx
00539ea5 8b4d18               mov ecx, dword ptr [ebp + 0x18]
00539ea8 884516               mov byte ptr [ebp + 0x16], al
00539eab 8b867c010000         mov eax, dword ptr [esi + 0x17c]
00539eb1 03c8                 add ecx, eax
00539eb3 895d10               mov dword ptr [ebp + 0x10], ebx
00539eb6 e8f636efff           call 0x42d5b1
00539ebb 8a4019               mov al, byte ptr [eax + 0x19]
00539ebe 8d4d10               lea ecx, [ebp + 0x10]
00539ec1 51                   push ecx
00539ec2 8b4d18               mov ecx, dword ptr [ebp + 0x18]
00539ec5 884515               mov byte ptr [ebp + 0x15], al
00539ec8 8b867c010000         mov eax, dword ptr [esi + 0x17c]
00539ece 03c8                 add ecx, eax
00539ed0 895d10               mov dword ptr [ebp + 0x10], ebx
00539ed3 e8d936efff           call 0x42d5b1
00539ed8 8a4018               mov al, byte ptr [eax + 0x18]
00539edb eb49                 jmp 0x539f26
00539edd 8b8664010000         mov eax, dword ptr [esi + 0x164]
00539ee3 8b0cb8               mov ecx, dword ptr [eax + edi*4]
00539ee6 8b01                 mov eax, dword ptr [ecx]
00539ee8 ff90e0000000         call dword ptr [eax + 0xe0]
00539eee 8a4002               mov al, byte ptr [eax + 2]
00539ef1 884516               mov byte ptr [ebp + 0x16], al
00539ef4 8b8664010000         mov eax, dword ptr [esi + 0x164]
00539efa 8b0cb8               mov ecx, dword ptr [eax + edi*4]
00539efd 8b01                 mov eax, dword ptr [ecx]
00539eff 8d5510               lea edx, [ebp + 0x10]
00539f02 52                   push edx
00539f03 ff90e0000000         call dword ptr [eax + 0xe0]
00539f09 8a4001               mov al, byte ptr [eax + 1]
00539f0c 884515               mov byte ptr [ebp + 0x15], al
00539f0f 8b8664010000         mov eax, dword ptr [esi + 0x164]
00539f15 8b0cb8               mov ecx, dword ptr [eax + edi*4]
00539f18 8b01                 mov eax, dword ptr [ecx]
00539f1a 8d5510               lea edx, [ebp + 0x10]
00539f1d 52                   push edx
00539f1e ff90e0000000         call dword ptr [eax + 0xe0]
00539f24 8a00                 mov al, byte ptr [eax]
00539f26 d945fc               fld dword ptr [ebp - 4]
00539f29 884514               mov byte ptr [ebp + 0x14], al
00539f2c 8b8664010000         mov eax, dword ptr [esi + 0x164]
00539f32 8b0cb8               mov ecx, dword ptr [eax + edi*4]
00539f35 8b01                 mov eax, dword ptr [ecx]
00539f37 6a00                 push 0
00539f39 51                   push ecx
00539f3a 8d5514               lea edx, [ebp + 0x14]
00539f3d d91c24               fstp dword ptr [esp]
00539f40 52                   push edx
00539f41 ff9098000000         call dword ptr [eax + 0x98]
00539f47 d945fc               fld dword ptr [ebp - 4]
00539f4a 8b8664010000         mov eax, dword ptr [esi + 0x164]
00539f50 8b0cb8               mov ecx, dword ptr [eax + edi*4]
00539f53 8b01                 mov eax, dword ptr [ecx]
00539f55 6a00                 push 0
00539f57 51                   push ecx
00539f58 8d55c8               lea edx, [ebp - 0x38]
00539f5b d91c24               fstp dword ptr [esp]
00539f5e 52                   push edx
00539f5f ff908c000000         call dword ptr [eax + 0x8c]
00539f65 8b867c010000         mov eax, dword ptr [esi + 0x17c]
00539f6b 8d3c7f               lea edi, [edi + edi*2]
00539f6e 8d4d10               lea ecx, [ebp + 0x10]
00539f71 c1e702               shl edi, 2
00539f74 51                   push ecx
00539f75 8d0c07               lea ecx, [edi + eax]
00539f78 c7451001000000       mov dword ptr [ebp + 0x10], 1
00539f7f e82d36efff           call 0x42d5b1
00539f84 8b4dd0               mov ecx, dword ptr [ebp - 0x30]
00539f87 894808               mov dword ptr [eax + 8], ecx
00539f8a 8b4dd4               mov ecx, dword ptr [ebp - 0x2c]
00539f8d 89480c               mov dword ptr [eax + 0xc], ecx
00539f90 8b867c010000         mov eax, dword ptr [esi + 0x17c]
00539f96 8d4d10               lea ecx, [ebp + 0x10]
00539f99 51                   push ecx
00539f9a 8d0c07               lea ecx, [edi + eax]
00539f9d c7451004000000       mov dword ptr [ebp + 0x10], 4
00539fa4 e80836efff           call 0x42d5b1
00539fa9 8b4dd0               mov ecx, dword ptr [ebp - 0x30]
00539fac 894808               mov dword ptr [eax + 8], ecx
00539faf 8b4dd4               mov ecx, dword ptr [ebp - 0x2c]
00539fb2 89480c               mov dword ptr [eax + 0xc], ecx
00539fb5 8b867c010000         mov eax, dword ptr [esi + 0x17c]
00539fbb 8d4d10               lea ecx, [ebp + 0x10]
00539fbe 51                   push ecx
00539fbf 8d0c07               lea ecx, [edi + eax]
00539fc2 895d10               mov dword ptr [ebp + 0x10], ebx
00539fc5 e8e735efff           call 0x42d5b1
00539fca 8b4dd0               mov ecx, dword ptr [ebp - 0x30]
00539fcd 894808               mov dword ptr [eax + 8], ecx
00539fd0 8b4dd4               mov ecx, dword ptr [ebp - 0x2c]
00539fd3 89480c               mov dword ptr [eax + 0xc], ecx
00539fd6 8b867c010000         mov eax, dword ptr [esi + 0x17c]
00539fdc 8d4d10               lea ecx, [ebp + 0x10]
00539fdf 51                   push ecx
00539fe0 8d0c07               lea ecx, [edi + eax]
00539fe3 c7451001000000       mov dword ptr [ebp + 0x10], 1
00539fea e8c235efff           call 0x42d5b1
00539fef 8a4d17               mov cl, byte ptr [ebp + 0x17]
00539ff2 88481b               mov byte ptr [eax + 0x1b], cl
00539ff5 8b867c010000         mov eax, dword ptr [esi + 0x17c]
00539ffb 8d4d10               lea ecx, [ebp + 0x10]
00539ffe 51                   push ecx
00539fff 8d0c07               lea ecx, [edi + eax]
0053a002 c7451004000000       mov dword ptr [ebp + 0x10], 4
0053a009 e8a335efff           call 0x42d5b1
0053a00e 8a4d17               mov cl, byte ptr [ebp + 0x17]
0053a011 88481b               mov byte ptr [eax + 0x1b], cl
0053a014 8b867c010000         mov eax, dword ptr [esi + 0x17c]
0053a01a 8d4d10               lea ecx, [ebp + 0x10]
0053a01d 51                   push ecx
0053a01e 8d0c07               lea ecx, [edi + eax]
0053a021 895d10               mov dword ptr [ebp + 0x10], ebx
0053a024 e88835efff           call 0x42d5b1
0053a029 8a4d17               mov cl, byte ptr [ebp + 0x17]
0053a02c 88481b               mov byte ptr [eax + 0x1b], cl
0053a02f 8b867c010000         mov eax, dword ptr [esi + 0x17c]
0053a035 8d4d10               lea ecx, [ebp + 0x10]
0053a038 c7451001000000       mov dword ptr [ebp + 0x10], 1
0053a03f 51                   push ecx
0053a040 8d0c07               lea ecx, [edi + eax]
0053a043 e86935efff           call 0x42d5b1
0053a048 8b4dc8               mov ecx, dword ptr [ebp - 0x38]
0053a04b 894810               mov dword ptr [eax + 0x10], ecx
0053a04e 8b4dcc               mov ecx, dword ptr [ebp - 0x34]
0053a051 894814               mov dword ptr [eax + 0x14], ecx
0053a054 8b867c010000         mov eax, dword ptr [esi + 0x17c]
0053a05a 8d4d10               lea ecx, [ebp + 0x10]
0053a05d 51                   push ecx
0053a05e 8d0c07               lea ecx, [edi + eax]
0053a061 c7451004000000       mov dword ptr [ebp + 0x10], 4
0053a068 e84435efff           call 0x42d5b1
0053a06d 8b4dc8               mov ecx, dword ptr [ebp - 0x38]
0053a070 894810               mov dword ptr [eax + 0x10], ecx
0053a073 8b4dcc               mov ecx, dword ptr [ebp - 0x34]
0053a076 894814               mov dword ptr [eax + 0x14], ecx
0053a079 8bb67c010000         mov esi, dword ptr [esi + 0x17c]
0053a07f 8d4510               lea eax, [ebp + 0x10]
0053a082 50                   push eax
0053a083 8d0c37               lea ecx, [edi + esi]
0053a086 895d10               mov dword ptr [ebp + 0x10], ebx
0053a089 e82335efff           call 0x42d5b1
0053a08e 8b4dc8               mov ecx, dword ptr [ebp - 0x38]
0053a091 894810               mov dword ptr [eax + 0x10], ecx
0053a094 8b4dcc               mov ecx, dword ptr [ebp - 0x34]
0053a097 894814               mov dword ptr [eax + 0x14], ecx
0053a09a 5b                   pop ebx
0053a09b 5f                   pop edi
0053a09c 5e                   pop esi
0053a09d c9                   leave 
0053a09e c21400               ret 0x14
