005385ae 55                   push ebp
005385af 8bec                 mov ebp, esp
005385b1 83ec78               sub esp, 0x78
005385b4 56                   push esi
005385b5 8bf1                 mov esi, ecx
005385b7 8b8668010000         mov eax, dword ptr [esi + 0x168]
005385bd 2b8664010000         sub eax, dword ptr [esi + 0x164]
005385c3 c1f802               sar eax, 2
005385c6 83f801               cmp eax, 1
005385c9 0f86280d0000         jbe 0x5392f7
005385cf 807d1400             cmp byte ptr [ebp + 0x14], 0
005385d3 57                   push edi
005385d4 8b7d08               mov edi, dword ptr [ebp + 8]
005385d7 0f848f000000         je 0x53866c
005385dd 8b8668010000         mov eax, dword ptr [esi + 0x168]
005385e3 2b8664010000         sub eax, dword ptr [esi + 0x164]
005385e9 c1f802               sar eax, 2
005385ec 48                   dec eax
005385ed 39865c010000         cmp dword ptr [esi + 0x15c], eax
005385f3 7510                 jne 0x538605
005385f5 8b06                 mov eax, dword ptr [esi]
005385f7 ff90a8020000         call dword ptr [eax + 0x2a8]
005385fd 84c0                 test al, al
005385ff 0f84f10c0000         je 0x5392f6
00538605 8b865c010000         mov eax, dword ptr [esi + 0x15c]
0053860b 3bc7                 cmp eax, edi
0053860d 725d                 jb 0x53866c
0053860f 3b450c               cmp eax, dword ptr [ebp + 0xc]
00538612 7758                 ja 0x53866c
00538614 8b8e64010000         mov ecx, dword ptr [esi + 0x164]
0053861a 8b0c81               mov ecx, dword ptr [ecx + eax*4]
0053861d 8b01                 mov eax, dword ptr [ecx]
0053861f 6a04                 push 4
00538621 ff90c0000000         call dword ptr [eax + 0xc0]
00538627 ff865c010000         inc dword ptr [esi + 0x15c]
0053862d 8b8668010000         mov eax, dword ptr [esi + 0x168]
00538633 2b8664010000         sub eax, dword ptr [esi + 0x164]
00538639 8b8e5c010000         mov ecx, dword ptr [esi + 0x15c]
0053863f 8b16                 mov edx, dword ptr [esi]
00538641 c1f802               sar eax, 2
00538644 3bc8                 cmp ecx, eax
00538646 1bc0                 sbb eax, eax
00538648 23c1                 and eax, ecx
0053864a 50                   push eax
0053864b 8bce                 mov ecx, esi
0053864d ff9284020000         call dword ptr [edx + 0x284]
00538653 8b865c010000         mov eax, dword ptr [esi + 0x15c]
00538659 8b8e64010000         mov ecx, dword ptr [esi + 0x164]
0053865f 8b0c81               mov ecx, dword ptr [ecx + eax*4]
00538662 8b01                 mov eax, dword ptr [ecx]
00538664 6a03                 push 3
00538666 ff90c0000000         call dword ptr [eax + 0xc0]
0053866c 8b06                 mov eax, dword ptr [esi]
0053866e 8bce                 mov ecx, esi
00538670 ff90ac020000         call dword ptr [eax + 0x2ac]
00538676 84c0                 test al, al
00538678 7509                 jne 0x538683
0053867a 384518               cmp byte ptr [ebp + 0x18], al
0053867d 0f84730c0000         je 0x5392f6
00538683 8b06                 mov eax, dword ptr [esi]
00538685 8bce                 mov ecx, esi
00538687 ff90d8000000         call dword ptr [eax + 0xd8]
0053868d d905e0de2201         fld dword ptr [0x122dee0]
00538693 dae9                 fucompp 
00538695 dfe0                 fnstsw ax
00538697 f6c444               test ah, 0x44
0053869a 7b0c                 jnp 0x5386a8
0053869c 8b06                 mov eax, dword ptr [esi]
0053869e 8bce                 mov ecx, esi
005386a0 ff90d8000000         call dword ptr [eax + 0xd8]
005386a6 eb02                 jmp 0x5386aa
005386a8 d9ee                 fldz 
005386aa 85ff                 test edi, edi
005386ac d95dfc               fstp dword ptr [ebp - 4]
005386af 53                   push ebx
005386b0 6a05                 push 5
005386b2 5b                   pop ebx
005386b3 0f85c0000000         jne 0x538779
005386b9 807d1000             cmp byte ptr [ebp + 0x10], 0
005386bd 7463                 je 0x538722
005386bf 8b450c               mov eax, dword ptr [ebp + 0xc]
005386c2 8b8e7c010000         mov ecx, dword ptr [esi + 0x17c]
005386c8 8d0440               lea eax, [eax + eax*2]
005386cb c1e002               shl eax, 2
005386ce 8d55f8               lea edx, [ebp - 8]
005386d1 52                   push edx
005386d2 03c8                 add ecx, eax
005386d4 895df8               mov dword ptr [ebp - 8], ebx
005386d7 894518               mov dword ptr [ebp + 0x18], eax
005386da e8d24eefff           call 0x42d5b1
005386df 8b4808               mov ecx, dword ptr [eax + 8]
005386e2 894dd0               mov dword ptr [ebp - 0x30], ecx
005386e5 8b400c               mov eax, dword ptr [eax + 0xc]
005386e8 8d4df8               lea ecx, [ebp - 8]
005386eb 51                   push ecx
005386ec 8b4d18               mov ecx, dword ptr [ebp + 0x18]
005386ef 8945d4               mov dword ptr [ebp - 0x2c], eax
005386f2 8b867c010000         mov eax, dword ptr [esi + 0x17c]
005386f8 03c8                 add ecx, eax
005386fa 895df8               mov dword ptr [ebp - 8], ebx
005386fd e8af4eefff           call 0x42d5b1
00538702 8b4810               mov ecx, dword ptr [eax + 0x10]
00538705 894dc8               mov dword ptr [ebp - 0x38], ecx
00538708 8b4014               mov eax, dword ptr [eax + 0x14]
0053870b 8d4df8               lea ecx, [ebp - 8]
0053870e 51                   push ecx
0053870f 8b4d18               mov ecx, dword ptr [ebp + 0x18]
00538712 8945cc               mov dword ptr [ebp - 0x34], eax
00538715 8b867c010000         mov eax, dword ptr [esi + 0x17c]
0053871b 03c8                 add ecx, eax
0053871d e9be000000           jmp 0x5387e0
00538722 8b8664010000         mov eax, dword ptr [esi + 0x164]
00538728 8b4d0c               mov ecx, dword ptr [ebp + 0xc]
0053872b 8b0c88               mov ecx, dword ptr [eax + ecx*4]
0053872e 8b01                 mov eax, dword ptr [ecx]
00538730 8d55d8               lea edx, [ebp - 0x28]
00538733 52                   push edx
00538734 ff90dc000000         call dword ptr [eax + 0xdc]
0053873a 8b08                 mov ecx, dword ptr [eax]
0053873c 894dd0               mov dword ptr [ebp - 0x30], ecx
0053873f 8b4004               mov eax, dword ptr [eax + 4]
00538742 8b4d0c               mov ecx, dword ptr [ebp + 0xc]
00538745 8945d4               mov dword ptr [ebp - 0x2c], eax
00538748 8b8664010000         mov eax, dword ptr [esi + 0x164]
0053874e 8b0c88               mov ecx, dword ptr [eax + ecx*4]
00538751 8b01                 mov eax, dword ptr [ecx]
00538753 8d55d8               lea edx, [ebp - 0x28]
00538756 52                   push edx
00538757 ff90e4000000         call dword ptr [eax + 0xe4]
0053875d 8b08                 mov ecx, dword ptr [eax]
0053875f 894dc8               mov dword ptr [ebp - 0x38], ecx
00538762 8b4004               mov eax, dword ptr [eax + 4]
00538765 8b4d0c               mov ecx, dword ptr [ebp + 0xc]
00538768 8945cc               mov dword ptr [ebp - 0x34], eax
0053876b 8b8664010000         mov eax, dword ptr [esi + 0x164]
00538771 8b0c88               mov ecx, dword ptr [eax + ecx*4]
00538774 e9c0000000           jmp 0x538839
00538779 807d1000             cmp byte ptr [ebp + 0x10], 0
0053877d 746e                 je 0x5387ed
0053877f 8b8e7c010000         mov ecx, dword ptr [esi + 0x17c]
00538785 8d047f               lea eax, [edi + edi*2]
00538788 c1e002               shl eax, 2
0053878b 8d55f8               lea edx, [ebp - 8]
0053878e 52                   push edx
0053878f 8d4c08f4             lea ecx, [eax + ecx - 0xc]
00538793 895df8               mov dword ptr [ebp - 8], ebx
00538796 894518               mov dword ptr [ebp + 0x18], eax
00538799 e8134eefff           call 0x42d5b1
0053879e 8b4808               mov ecx, dword ptr [eax + 8]
005387a1 894dd0               mov dword ptr [ebp - 0x30], ecx
005387a4 8b400c               mov eax, dword ptr [eax + 0xc]
005387a7 8d4df8               lea ecx, [ebp - 8]
005387aa 51                   push ecx
005387ab 8b4d18               mov ecx, dword ptr [ebp + 0x18]
005387ae 8945d4               mov dword ptr [ebp - 0x2c], eax
005387b1 8b867c010000         mov eax, dword ptr [esi + 0x17c]
005387b7 8d4c01f4             lea ecx, [ecx + eax - 0xc]
005387bb 895df8               mov dword ptr [ebp - 8], ebx
005387be e8ee4defff           call 0x42d5b1
005387c3 8b4810               mov ecx, dword ptr [eax + 0x10]
005387c6 894dc8               mov dword ptr [ebp - 0x38], ecx
005387c9 8b4014               mov eax, dword ptr [eax + 0x14]
005387cc 8d4df8               lea ecx, [ebp - 8]
005387cf 51                   push ecx
005387d0 8b4d18               mov ecx, dword ptr [ebp + 0x18]
005387d3 8945cc               mov dword ptr [ebp - 0x34], eax
005387d6 8b867c010000         mov eax, dword ptr [esi + 0x17c]
005387dc 8d4c01f4             lea ecx, [ecx + eax - 0xc]
005387e0 895df8               mov dword ptr [ebp - 8], ebx
005387e3 e8c94defff           call 0x42d5b1
005387e8 8b4018               mov eax, dword ptr [eax + 0x18]
005387eb eb5a                 jmp 0x538847
005387ed 8b8664010000         mov eax, dword ptr [esi + 0x164]
005387f3 8b4cb8fc             mov ecx, dword ptr [eax + edi*4 - 4]
005387f7 8b01                 mov eax, dword ptr [ecx]
005387f9 8d55d8               lea edx, [ebp - 0x28]
005387fc 52                   push edx
005387fd ff90dc000000         call dword ptr [eax + 0xdc]
00538803 8b08                 mov ecx, dword ptr [eax]
00538805 894dd0               mov dword ptr [ebp - 0x30], ecx
00538808 8b4004               mov eax, dword ptr [eax + 4]
0053880b 8945d4               mov dword ptr [ebp - 0x2c], eax
0053880e 8b8664010000         mov eax, dword ptr [esi + 0x164]
00538814 8b4cb8fc             mov ecx, dword ptr [eax + edi*4 - 4]
00538818 8b01                 mov eax, dword ptr [ecx]
0053881a 8d55d8               lea edx, [ebp - 0x28]
0053881d 52                   push edx
0053881e ff90e4000000         call dword ptr [eax + 0xe4]
00538824 8b08                 mov ecx, dword ptr [eax]
00538826 894dc8               mov dword ptr [ebp - 0x38], ecx
00538829 8b4004               mov eax, dword ptr [eax + 4]
0053882c 8945cc               mov dword ptr [ebp - 0x34], eax
0053882f 8b8664010000         mov eax, dword ptr [esi + 0x164]
00538835 8b4cb8fc             mov ecx, dword ptr [eax + edi*4 - 4]
00538839 8b01                 mov eax, dword ptr [ecx]
0053883b 8d5518               lea edx, [ebp + 0x18]
0053883e 52                   push edx
0053883f ff90e0000000         call dword ptr [eax + 0xe0]
00538845 8b00                 mov eax, dword ptr [eax]
00538847 894514               mov dword ptr [ebp + 0x14], eax
0053884a 8b450c               mov eax, dword ptr [ebp + 0xc]
0053884d 3bc7                 cmp eax, edi
0053884f 89450c               mov dword ptr [ebp + 0xc], eax
00538852 0f8664050000         jbe 0x538dbc
00538858 8d0440               lea eax, [eax + eax*2]
0053885b c1e002               shl eax, 2
0053885e 894518               mov dword ptr [ebp + 0x18], eax
00538861 8b5d0c               mov ebx, dword ptr [ebp + 0xc]
00538864 d945fc               fld dword ptr [ebp - 4]
00538867 8b8664010000         mov eax, dword ptr [esi + 0x164]
0053886d c1e302               shl ebx, 2
00538870 03c3                 add eax, ebx
00538872 8b48fc               mov ecx, dword ptr [eax - 4]
00538875 8b00                 mov eax, dword ptr [eax]
00538877 8b38                 mov edi, dword ptr [eax]
00538879 6a00                 push 0
0053887b 51                   push ecx
0053887c 8d55c0               lea edx, [ebp - 0x40]
0053887f d91c24               fstp dword ptr [esp]
00538882 8945f4               mov dword ptr [ebp - 0xc], eax
00538885 8b01                 mov eax, dword ptr [ecx]
00538887 52                   push edx
00538888 ff90dc000000         call dword ptr [eax + 0xdc]
0053888e 8b4df4               mov ecx, dword ptr [ebp - 0xc]
00538891 50                   push eax
00538892 ff577c               call dword ptr [edi + 0x7c]
00538895 8b8664010000         mov eax, dword ptr [esi + 0x164]
0053889b 8b0c03               mov ecx, dword ptr [ebx + eax]
0053889e 8b01                 mov eax, dword ptr [ecx]
005388a0 8d55f8               lea edx, [ebp - 8]
005388a3 52                   push edx
005388a4 ff90e0000000         call dword ptr [eax + 0xe0]
005388aa 8b8664010000         mov eax, dword ptr [esi + 0x164]
005388b0 8b4c03fc             mov ecx, dword ptr [ebx + eax - 4]
005388b4 8b01                 mov eax, dword ptr [ecx]
005388b6 8d55e8               lea edx, [ebp - 0x18]
005388b9 52                   push edx
005388ba ff90e0000000         call dword ptr [eax + 0xe0]
005388c0 d945fc               fld dword ptr [ebp - 4]
005388c3 8a4003               mov al, byte ptr [eax + 3]
005388c6 8845fb               mov byte ptr [ebp - 5], al
005388c9 8b8664010000         mov eax, dword ptr [esi + 0x164]
005388cf 8b0c03               mov ecx, dword ptr [ebx + eax]
005388d2 8b01                 mov eax, dword ptr [ecx]
005388d4 6a00                 push 0
005388d6 51                   push ecx
005388d7 8d55f8               lea edx, [ebp - 8]
005388da d91c24               fstp dword ptr [esp]
005388dd 52                   push edx
005388de ff9098000000         call dword ptr [eax + 0x98]
005388e4 d945fc               fld dword ptr [ebp - 4]
005388e7 8b8664010000         mov eax, dword ptr [esi + 0x164]
005388ed 03c3                 add eax, ebx
005388ef 8b48fc               mov ecx, dword ptr [eax - 4]
005388f2 8b00                 mov eax, dword ptr [eax]
005388f4 8b38                 mov edi, dword ptr [eax]
005388f6 6a00                 push 0
005388f8 51                   push ecx
005388f9 8d55b8               lea edx, [ebp - 0x48]
005388fc d91c24               fstp dword ptr [esp]
005388ff 8945f4               mov dword ptr [ebp - 0xc], eax
00538902 8b01                 mov eax, dword ptr [ecx]
00538904 52                   push edx
00538905 ff90e4000000         call dword ptr [eax + 0xe4]
0053890b 8b4df4               mov ecx, dword ptr [ebp - 0xc]
0053890e 50                   push eax
0053890f ff978c000000         call dword ptr [edi + 0x8c]
00538915 807d1000             cmp byte ptr [ebp + 0x10], 0
00538919 0f8424020000         je 0x538b43
0053891f 8b9e7c010000         mov ebx, dword ptr [esi + 0x17c]
00538925 8b7d18               mov edi, dword ptr [ebp + 0x18]
00538928 33c0                 xor eax, eax
0053892a 40                   inc eax
0053892b 8945f4               mov dword ptr [ebp - 0xc], eax
0053892e 8945f0               mov dword ptr [ebp - 0x10], eax
00538931 8d45f4               lea eax, [ebp - 0xc]
00538934 50                   push eax
00538935 8d4c3bf4             lea ecx, [ebx + edi - 0xc]
00538939 e8734cefff           call 0x42d5b1
0053893e 8b4808               mov ecx, dword ptr [eax + 8]
00538941 8b400c               mov eax, dword ptr [eax + 0xc]
00538944 8d55f0               lea edx, [ebp - 0x10]
00538947 894df4               mov dword ptr [ebp - 0xc], ecx
0053894a 52                   push edx
0053894b 8d0c3b               lea ecx, [ebx + edi]
0053894e 8945ec               mov dword ptr [ebp - 0x14], eax
00538951 e85b4cefff           call 0x42d5b1
00538956 8b4df4               mov ecx, dword ptr [ebp - 0xc]
00538959 894808               mov dword ptr [eax + 8], ecx
0053895c 8b4dec               mov ecx, dword ptr [ebp - 0x14]
0053895f 89480c               mov dword ptr [eax + 0xc], ecx
00538962 8b9e7c010000         mov ebx, dword ptr [esi + 0x17c]
00538968 6a04                 push 4
0053896a 58                   pop eax
0053896b 8945ec               mov dword ptr [ebp - 0x14], eax
0053896e 8945f0               mov dword ptr [ebp - 0x10], eax
00538971 8d45ec               lea eax, [ebp - 0x14]
00538974 50                   push eax
00538975 8d4c3bf4             lea ecx, [ebx + edi - 0xc]
00538979 e8334cefff           call 0x42d5b1
0053897e 8b4808               mov ecx, dword ptr [eax + 8]
00538981 8b400c               mov eax, dword ptr [eax + 0xc]
00538984 8d55f0               lea edx, [ebp - 0x10]
00538987 894dec               mov dword ptr [ebp - 0x14], ecx
0053898a 52                   push edx
0053898b 8d0c3b               lea ecx, [ebx + edi]
0053898e 8945f4               mov dword ptr [ebp - 0xc], eax
00538991 e81b4cefff           call 0x42d5b1
00538996 8b4dec               mov ecx, dword ptr [ebp - 0x14]
00538999 894808               mov dword ptr [eax + 8], ecx
0053899c 8b4df4               mov ecx, dword ptr [ebp - 0xc]
0053899f 89480c               mov dword ptr [eax + 0xc], ecx
005389a2 8b9e7c010000         mov ebx, dword ptr [esi + 0x17c]
005389a8 6a05                 push 5
005389aa 58                   pop eax
005389ab 8945ec               mov dword ptr [ebp - 0x14], eax
005389ae 8945f0               mov dword ptr [ebp - 0x10], eax
005389b1 8d45ec               lea eax, [ebp - 0x14]
005389b4 50                   push eax
005389b5 8d4c3bf4             lea ecx, [ebx + edi - 0xc]
005389b9 e8f34befff           call 0x42d5b1
005389be 8b4808               mov ecx, dword ptr [eax + 8]
005389c1 8b400c               mov eax, dword ptr [eax + 0xc]
005389c4 8d55f0               lea edx, [ebp - 0x10]
005389c7 894dec               mov dword ptr [ebp - 0x14], ecx
005389ca 52                   push edx
005389cb 8d0c3b               lea ecx, [ebx + edi]
005389ce 8945f4               mov dword ptr [ebp - 0xc], eax
005389d1 e8db4befff           call 0x42d5b1
005389d6 8b4dec               mov ecx, dword ptr [ebp - 0x14]
005389d9 894808               mov dword ptr [eax + 8], ecx
005389dc 8b4df4               mov ecx, dword ptr [ebp - 0xc]
005389df 89480c               mov dword ptr [eax + 0xc], ecx
005389e2 8b9e7c010000         mov ebx, dword ptr [esi + 0x17c]
005389e8 33c0                 xor eax, eax
005389ea 40                   inc eax
005389eb 8945ec               mov dword ptr [ebp - 0x14], eax
005389ee 8945f0               mov dword ptr [ebp - 0x10], eax
005389f1 8d45ec               lea eax, [ebp - 0x14]
005389f4 50                   push eax
005389f5 8d4c3bf4             lea ecx, [ebx + edi - 0xc]
005389f9 e8b34befff           call 0x42d5b1
005389fe 8d4df0               lea ecx, [ebp - 0x10]
00538a01 51                   push ecx
00538a02 8d0c3b               lea ecx, [ebx + edi]
00538a05 8945ec               mov dword ptr [ebp - 0x14], eax
00538a08 e8a44befff           call 0x42d5b1
00538a0d 8b4dec               mov ecx, dword ptr [ebp - 0x14]
00538a10 8a491b               mov cl, byte ptr [ecx + 0x1b]
00538a13 88481b               mov byte ptr [eax + 0x1b], cl
00538a16 8b9e7c010000         mov ebx, dword ptr [esi + 0x17c]
00538a1c 6a04                 push 4
00538a1e 58                   pop eax
00538a1f 8945ec               mov dword ptr [ebp - 0x14], eax
00538a22 8945f0               mov dword ptr [ebp - 0x10], eax
00538a25 8d45ec               lea eax, [ebp - 0x14]
00538a28 50                   push eax
00538a29 8d4c3bf4             lea ecx, [ebx + edi - 0xc]
00538a2d e87f4befff           call 0x42d5b1
00538a32 8d4df0               lea ecx, [ebp - 0x10]
00538a35 51                   push ecx
00538a36 8d0c3b               lea ecx, [ebx + edi]
00538a39 8945ec               mov dword ptr [ebp - 0x14], eax
00538a3c e8704befff           call 0x42d5b1
00538a41 8b4dec               mov ecx, dword ptr [ebp - 0x14]
00538a44 8a491b               mov cl, byte ptr [ecx + 0x1b]
00538a47 88481b               mov byte ptr [eax + 0x1b], cl
00538a4a 8b9e7c010000         mov ebx, dword ptr [esi + 0x17c]
00538a50 6a05                 push 5
00538a52 58                   pop eax
00538a53 8945ec               mov dword ptr [ebp - 0x14], eax
00538a56 8945f0               mov dword ptr [ebp - 0x10], eax
00538a59 8d45ec               lea eax, [ebp - 0x14]
00538a5c 50                   push eax
00538a5d 8d4c3bf4             lea ecx, [ebx + edi - 0xc]
00538a61 e84b4befff           call 0x42d5b1
00538a66 8d4df0               lea ecx, [ebp - 0x10]
00538a69 51                   push ecx
00538a6a 8d0c3b               lea ecx, [ebx + edi]
00538a6d 8945ec               mov dword ptr [ebp - 0x14], eax
00538a70 e83c4befff           call 0x42d5b1
00538a75 8b4dec               mov ecx, dword ptr [ebp - 0x14]
00538a78 8a491b               mov cl, byte ptr [ecx + 0x1b]
00538a7b 88481b               mov byte ptr [eax + 0x1b], cl
00538a7e 8b9e7c010000         mov ebx, dword ptr [esi + 0x17c]
00538a84 33c0                 xor eax, eax
00538a86 40                   inc eax
00538a87 8945ec               mov dword ptr [ebp - 0x14], eax
00538a8a 8945f0               mov dword ptr [ebp - 0x10], eax
00538a8d 8d45ec               lea eax, [ebp - 0x14]
00538a90 50                   push eax
00538a91 8d4c3bf4             lea ecx, [ebx + edi - 0xc]
00538a95 e8174befff           call 0x42d5b1
00538a9a 8b4810               mov ecx, dword ptr [eax + 0x10]
00538a9d 8b4014               mov eax, dword ptr [eax + 0x14]
00538aa0 8d55f0               lea edx, [ebp - 0x10]
00538aa3 894dec               mov dword ptr [ebp - 0x14], ecx
00538aa6 52                   push edx
00538aa7 8d0c3b               lea ecx, [ebx + edi]
00538aaa 8945f4               mov dword ptr [ebp - 0xc], eax
00538aad e8ff4aefff           call 0x42d5b1
00538ab2 8b4dec               mov ecx, dword ptr [ebp - 0x14]
00538ab5 894810               mov dword ptr [eax + 0x10], ecx
00538ab8 8b4df4               mov ecx, dword ptr [ebp - 0xc]
00538abb 894814               mov dword ptr [eax + 0x14], ecx
00538abe 8b9e7c010000         mov ebx, dword ptr [esi + 0x17c]
00538ac4 6a04                 push 4
00538ac6 58                   pop eax
00538ac7 8945ec               mov dword ptr [ebp - 0x14], eax
00538aca 8945f0               mov dword ptr [ebp - 0x10], eax
00538acd 8d45ec               lea eax, [ebp - 0x14]
00538ad0 50                   push eax
00538ad1 8d4c3bf4             lea ecx, [ebx + edi - 0xc]
00538ad5 e8d74aefff           call 0x42d5b1
00538ada 8b4810               mov ecx, dword ptr [eax + 0x10]
00538add 8b4014               mov eax, dword ptr [eax + 0x14]
00538ae0 8d55f0               lea edx, [ebp - 0x10]
00538ae3 894dec               mov dword ptr [ebp - 0x14], ecx
00538ae6 52                   push edx
00538ae7 8d0c3b               lea ecx, [ebx + edi]
00538aea 8945f4               mov dword ptr [ebp - 0xc], eax
00538aed e8bf4aefff           call 0x42d5b1
00538af2 8b4dec               mov ecx, dword ptr [ebp - 0x14]
00538af5 894810               mov dword ptr [eax + 0x10], ecx
00538af8 8b4df4               mov ecx, dword ptr [ebp - 0xc]
00538afb 894814               mov dword ptr [eax + 0x14], ecx
00538afe 8b9e7c010000         mov ebx, dword ptr [esi + 0x17c]
00538b04 6a05                 push 5
00538b06 58                   pop eax
00538b07 8945ec               mov dword ptr [ebp - 0x14], eax
00538b0a 8945f0               mov dword ptr [ebp - 0x10], eax
00538b0d 8d45ec               lea eax, [ebp - 0x14]
00538b10 50                   push eax
00538b11 8d4c3bf4             lea ecx, [ebx + edi - 0xc]
00538b15 e8974aefff           call 0x42d5b1
00538b1a 8b4810               mov ecx, dword ptr [eax + 0x10]
00538b1d 8b4014               mov eax, dword ptr [eax + 0x14]
00538b20 8d55f0               lea edx, [ebp - 0x10]
00538b23 894dec               mov dword ptr [ebp - 0x14], ecx
00538b26 52                   push edx
00538b27 8d0c3b               lea ecx, [ebx + edi]
00538b2a 8945f4               mov dword ptr [ebp - 0xc], eax
00538b2d e87f4aefff           call 0x42d5b1
00538b32 8b4dec               mov ecx, dword ptr [ebp - 0x14]
00538b35 894810               mov dword ptr [eax + 0x10], ecx
00538b38 8b4df4               mov ecx, dword ptr [ebp - 0xc]
00538b3b 894814               mov dword ptr [eax + 0x14], ecx
00538b3e e963020000           jmp 0x538da6
00538b43 8b8664010000         mov eax, dword ptr [esi + 0x164]
00538b49 8b4c03fc             mov ecx, dword ptr [ebx + eax - 4]
00538b4d 8b01                 mov eax, dword ptr [ecx]
00538b4f 8bbe7c010000         mov edi, dword ptr [esi + 0x17c]
00538b55 8d55b0               lea edx, [ebp - 0x50]
00538b58 52                   push edx
00538b59 c745ec01000000       mov dword ptr [ebp - 0x14], 1
00538b60 ff90dc000000         call dword ptr [eax + 0xdc]
00538b66 8b18                 mov ebx, dword ptr [eax]
00538b68 8b4004               mov eax, dword ptr [eax + 4]
00538b6b 8d4dec               lea ecx, [ebp - 0x14]
00538b6e 51                   push ecx
00538b6f 8b4d18               mov ecx, dword ptr [ebp + 0x18]
00538b72 03cf                 add ecx, edi
00538b74 8945f0               mov dword ptr [ebp - 0x10], eax
00538b77 e8354aefff           call 0x42d5b1
00538b7c 895808               mov dword ptr [eax + 8], ebx
00538b7f 8b5df0               mov ebx, dword ptr [ebp - 0x10]
00538b82 89580c               mov dword ptr [eax + 0xc], ebx
00538b85 8b450c               mov eax, dword ptr [ebp + 0xc]
00538b88 8b8e64010000         mov ecx, dword ptr [esi + 0x164]
00538b8e 8bbe7c010000         mov edi, dword ptr [esi + 0x17c]
00538b94 c1e002               shl eax, 2
00538b97 8b4c08fc             mov ecx, dword ptr [eax + ecx - 4]
00538b9b 8b01                 mov eax, dword ptr [ecx]
00538b9d 8d55a8               lea edx, [ebp - 0x58]
00538ba0 52                   push edx
00538ba1 c745ec04000000       mov dword ptr [ebp - 0x14], 4
00538ba8 ff90dc000000         call dword ptr [eax + 0xdc]
00538bae 8b18                 mov ebx, dword ptr [eax]
00538bb0 8b4004               mov eax, dword ptr [eax + 4]
00538bb3 8d4dec               lea ecx, [ebp - 0x14]
00538bb6 51                   push ecx
00538bb7 8b4d18               mov ecx, dword ptr [ebp + 0x18]
00538bba 03cf                 add ecx, edi
00538bbc 8945f0               mov dword ptr [ebp - 0x10], eax
00538bbf e8ed49efff           call 0x42d5b1
00538bc4 895808               mov dword ptr [eax + 8], ebx
00538bc7 8b5df0               mov ebx, dword ptr [ebp - 0x10]
00538bca 89580c               mov dword ptr [eax + 0xc], ebx
00538bcd 8b450c               mov eax, dword ptr [ebp + 0xc]
00538bd0 8b8e64010000         mov ecx, dword ptr [esi + 0x164]
00538bd6 8bbe7c010000         mov edi, dword ptr [esi + 0x17c]
00538bdc c1e002               shl eax, 2
00538bdf 8b4c08fc             mov ecx, dword ptr [eax + ecx - 4]
00538be3 8b01                 mov eax, dword ptr [ecx]
00538be5 8d55a0               lea edx, [ebp - 0x60]
00538be8 52                   push edx
00538be9 c745ec05000000       mov dword ptr [ebp - 0x14], 5
00538bf0 ff90dc000000         call dword ptr [eax + 0xdc]
00538bf6 8b18                 mov ebx, dword ptr [eax]
00538bf8 8b4004               mov eax, dword ptr [eax + 4]
00538bfb 8d4dec               lea ecx, [ebp - 0x14]
00538bfe 51                   push ecx
00538bff 8b4d18               mov ecx, dword ptr [ebp + 0x18]
00538c02 03cf                 add ecx, edi
00538c04 8945f0               mov dword ptr [ebp - 0x10], eax
00538c07 e8a549efff           call 0x42d5b1
00538c0c 8b7d0c               mov edi, dword ptr [ebp + 0xc]
00538c0f 895808               mov dword ptr [eax + 8], ebx
00538c12 8b5df0               mov ebx, dword ptr [ebp - 0x10]
00538c15 89580c               mov dword ptr [eax + 0xc], ebx
00538c18 8b8664010000         mov eax, dword ptr [esi + 0x164]
00538c1e 8b9e7c010000         mov ebx, dword ptr [esi + 0x17c]
00538c24 c1e702               shl edi, 2
00538c27 8b4c07fc             mov ecx, dword ptr [edi + eax - 4]
00538c2b 8b01                 mov eax, dword ptr [ecx]
00538c2d 8d55e4               lea edx, [ebp - 0x1c]
00538c30 52                   push edx
00538c31 c745ec01000000       mov dword ptr [ebp - 0x14], 1
00538c38 ff90e0000000         call dword ptr [eax + 0xe0]
00538c3e 8d4dec               lea ecx, [ebp - 0x14]
00538c41 51                   push ecx
00538c42 8b4d18               mov ecx, dword ptr [ebp + 0x18]
00538c45 03cb                 add ecx, ebx
00538c47 8945f0               mov dword ptr [ebp - 0x10], eax
00538c4a e86249efff           call 0x42d5b1
00538c4f 8b4df0               mov ecx, dword ptr [ebp - 0x10]
00538c52 8a4903               mov cl, byte ptr [ecx + 3]
00538c55 88481b               mov byte ptr [eax + 0x1b], cl
00538c58 c745ec04000000       mov dword ptr [ebp - 0x14], 4
00538c5f 8b8664010000         mov eax, dword ptr [esi + 0x164]
00538c65 8b4c07fc             mov ecx, dword ptr [edi + eax - 4]
00538c69 8b01                 mov eax, dword ptr [ecx]
00538c6b 8b9e7c010000         mov ebx, dword ptr [esi + 0x17c]
00538c71 8d55e0               lea edx, [ebp - 0x20]
00538c74 52                   push edx
00538c75 ff90e0000000         call dword ptr [eax + 0xe0]
00538c7b 8d4dec               lea ecx, [ebp - 0x14]
00538c7e 51                   push ecx
00538c7f 8b4d18               mov ecx, dword ptr [ebp + 0x18]
00538c82 03cb                 add ecx, ebx
00538c84 8945f0               mov dword ptr [ebp - 0x10], eax
00538c87 e82549efff           call 0x42d5b1
00538c8c 8b4df0               mov ecx, dword ptr [ebp - 0x10]
00538c8f 8a4903               mov cl, byte ptr [ecx + 3]
00538c92 88481b               mov byte ptr [eax + 0x1b], cl
00538c95 8b8664010000         mov eax, dword ptr [esi + 0x164]
00538c9b 8b4c07fc             mov ecx, dword ptr [edi + eax - 4]
00538c9f 8b01                 mov eax, dword ptr [ecx]
00538ca1 8b9e7c010000         mov ebx, dword ptr [esi + 0x17c]
00538ca7 8d55dc               lea edx, [ebp - 0x24]
00538caa 52                   push edx
00538cab c745ec05000000       mov dword ptr [ebp - 0x14], 5
00538cb2 ff90e0000000         call dword ptr [eax + 0xe0]
00538cb8 8d4dec               lea ecx, [ebp - 0x14]
00538cbb 51                   push ecx
00538cbc 8b4d18               mov ecx, dword ptr [ebp + 0x18]
00538cbf 03cb                 add ecx, ebx
00538cc1 8945f0               mov dword ptr [ebp - 0x10], eax
00538cc4 e8e848efff           call 0x42d5b1
00538cc9 8b4df0               mov ecx, dword ptr [ebp - 0x10]
00538ccc 8a4903               mov cl, byte ptr [ecx + 3]
00538ccf 88481b               mov byte ptr [eax + 0x1b], cl
00538cd2 8b8664010000         mov eax, dword ptr [esi + 0x164]
00538cd8 8b7c07fc             mov edi, dword ptr [edi + eax - 4]
00538cdc 8b07                 mov eax, dword ptr [edi]
00538cde 8b9e7c010000         mov ebx, dword ptr [esi + 0x17c]
00538ce4 8d4d98               lea ecx, [ebp - 0x68]
00538ce7 51                   push ecx
00538ce8 8bcf                 mov ecx, edi
00538cea c745ec01000000       mov dword ptr [ebp - 0x14], 1
00538cf1 ff90e4000000         call dword ptr [eax + 0xe4]
00538cf7 8b38                 mov edi, dword ptr [eax]
00538cf9 8b4004               mov eax, dword ptr [eax + 4]
00538cfc 8d4dec               lea ecx, [ebp - 0x14]
00538cff 51                   push ecx
00538d00 8b4d18               mov ecx, dword ptr [ebp + 0x18]
00538d03 03cb                 add ecx, ebx
00538d05 8945f0               mov dword ptr [ebp - 0x10], eax
00538d08 e8a448efff           call 0x42d5b1
00538d0d 897810               mov dword ptr [eax + 0x10], edi
00538d10 8b7df0               mov edi, dword ptr [ebp - 0x10]
00538d13 897814               mov dword ptr [eax + 0x14], edi
00538d16 8b450c               mov eax, dword ptr [ebp + 0xc]
00538d19 8b8e64010000         mov ecx, dword ptr [esi + 0x164]
00538d1f 8bbe7c010000         mov edi, dword ptr [esi + 0x17c]
00538d25 c1e002               shl eax, 2
00538d28 8b4c08fc             mov ecx, dword ptr [eax + ecx - 4]
00538d2c 8b01                 mov eax, dword ptr [ecx]
00538d2e 8d5590               lea edx, [ebp - 0x70]
00538d31 52                   push edx
00538d32 c745ec04000000       mov dword ptr [ebp - 0x14], 4
00538d39 ff90e4000000         call dword ptr [eax + 0xe4]
00538d3f 8b18                 mov ebx, dword ptr [eax]
00538d41 8b4004               mov eax, dword ptr [eax + 4]
00538d44 8d4dec               lea ecx, [ebp - 0x14]
00538d47 51                   push ecx
00538d48 8b4d18               mov ecx, dword ptr [ebp + 0x18]
00538d4b 03cf                 add ecx, edi
00538d4d 8945f0               mov dword ptr [ebp - 0x10], eax
00538d50 e85c48efff           call 0x42d5b1
00538d55 895810               mov dword ptr [eax + 0x10], ebx
00538d58 8b5df0               mov ebx, dword ptr [ebp - 0x10]
00538d5b 895814               mov dword ptr [eax + 0x14], ebx
00538d5e 8b450c               mov eax, dword ptr [ebp + 0xc]
00538d61 8b8e64010000         mov ecx, dword ptr [esi + 0x164]
00538d67 8bbe7c010000         mov edi, dword ptr [esi + 0x17c]
00538d6d c1e002               shl eax, 2
00538d70 8b4c08fc             mov ecx, dword ptr [eax + ecx - 4]
00538d74 8b01                 mov eax, dword ptr [ecx]
00538d76 c745ec05000000       mov dword ptr [ebp - 0x14], 5
00538d7d 8d5588               lea edx, [ebp - 0x78]
00538d80 52                   push edx
00538d81 ff90e4000000         call dword ptr [eax + 0xe4]
00538d87 8b18                 mov ebx, dword ptr [eax]
00538d89 8b4004               mov eax, dword ptr [eax + 4]
00538d8c 8d4dec               lea ecx, [ebp - 0x14]
00538d8f 51                   push ecx
00538d90 8b4d18               mov ecx, dword ptr [ebp + 0x18]
00538d93 03cf                 add ecx, edi
00538d95 8945f0               mov dword ptr [ebp - 0x10], eax
00538d98 e81448efff           call 0x42d5b1
00538d9d 895810               mov dword ptr [eax + 0x10], ebx
00538da0 8b5df0               mov ebx, dword ptr [ebp - 0x10]
00538da3 895814               mov dword ptr [eax + 0x14], ebx
00538da6 8b7d08               mov edi, dword ptr [ebp + 8]
00538da9 ff4d0c               dec dword ptr [ebp + 0xc]
00538dac 836d180c             sub dword ptr [ebp + 0x18], 0xc
00538db0 397d0c               cmp dword ptr [ebp + 0xc], edi
00538db3 0f87a8faffff         ja 0x538861
00538db9 6a05                 push 5
00538dbb 5b                   pop ebx
00538dbc 8b06                 mov eax, dword ptr [esi]
00538dbe 8bce                 mov ecx, esi
00538dc0 ff90a8020000         call dword ptr [eax + 0x2a8]
00538dc6 84c0                 test al, al
00538dc8 0f85ed020000         jne 0x5390bb
00538dce 85ff                 test edi, edi
00538dd0 0f85e5020000         jne 0x5390bb
00538dd6 807d1000             cmp byte ptr [ebp + 0x10], 0
00538dda 0f84f3000000         je 0x538ed3
00538de0 8b8e7c010000         mov ecx, dword ptr [esi + 0x17c]
00538de6 8d4510               lea eax, [ebp + 0x10]
00538de9 50                   push eax
00538dea 895d10               mov dword ptr [ebp + 0x10], ebx
00538ded e8bf47efff           call 0x42d5b1
00538df2 d9400c               fld dword ptr [eax + 0xc]
00538df5 d8a68c010000         fsub dword ptr [esi + 0x18c]
00538dfb 8b8e7c010000         mov ecx, dword ptr [esi + 0x17c]
00538e01 8d4510               lea eax, [ebp + 0x10]
00538e04 50                   push eax
00538e05 d95dd4               fstp dword ptr [ebp - 0x2c]
00538e08 895d10               mov dword ptr [ebp + 0x10], ebx
00538e0b e8a147efff           call 0x42d5b1
00538e10 d94008               fld dword ptr [eax + 8]
00538e13 d88688010000         fadd dword ptr [esi + 0x188]
00538e19 8b8e7c010000         mov ecx, dword ptr [esi + 0x17c]
00538e1f 8d4510               lea eax, [ebp + 0x10]
00538e22 50                   push eax
00538e23 d95dd0               fstp dword ptr [ebp - 0x30]
00538e26 895d10               mov dword ptr [ebp + 0x10], ebx
00538e29 e88347efff           call 0x42d5b1
00538e2e 0fb6401b             movzx eax, byte ptr [eax + 0x1b]
00538e32 894510               mov dword ptr [ebp + 0x10], eax
00538e35 db4510               fild dword ptr [ebp + 0x10]
00538e38 d98698010000         fld dword ptr [esi + 0x198]
00538e3e d80d14002301         fmul dword ptr [0x1230014]
00538e44 dee9                 fsubp st(1)
00538e46 d81ddcde2201         fcomp dword ptr [0x122dedc]
00538e4c dfe0                 fnstsw ax
00538e4e f6c405               test ah, 5
00538e51 7a04                 jp 0x538e57
00538e53 d9ee                 fldz 
00538e55 eb2a                 jmp 0x538e81
00538e57 8b8e7c010000         mov ecx, dword ptr [esi + 0x17c]
00538e5d 8d4510               lea eax, [ebp + 0x10]
00538e60 50                   push eax
00538e61 895d10               mov dword ptr [ebp + 0x10], ebx
00538e64 e84847efff           call 0x42d5b1
00538e69 0fb6401b             movzx eax, byte ptr [eax + 0x1b]
00538e6d 894510               mov dword ptr [ebp + 0x10], eax
00538e70 db4510               fild dword ptr [ebp + 0x10]
00538e73 d98698010000         fld dword ptr [esi + 0x198]
00538e79 d80d14002301         fmul dword ptr [0x1230014]
00538e7f dee9                 fsubp st(1)
00538e81 e8ea5b6c00           call 0xbfea70
00538e86 8b8e7c010000         mov ecx, dword ptr [esi + 0x17c]
00538e8c 884517               mov byte ptr [ebp + 0x17], al
00538e8f 8d4510               lea eax, [ebp + 0x10]
00538e92 50                   push eax
00538e93 895d10               mov dword ptr [ebp + 0x10], ebx
00538e96 e81647efff           call 0x42d5b1
00538e9b 8a401a               mov al, byte ptr [eax + 0x1a]
00538e9e 8b8e7c010000         mov ecx, dword ptr [esi + 0x17c]
00538ea4 884516               mov byte ptr [ebp + 0x16], al
00538ea7 8d4510               lea eax, [ebp + 0x10]
00538eaa 50                   push eax
00538eab 895d10               mov dword ptr [ebp + 0x10], ebx
00538eae e8fe46efff           call 0x42d5b1
00538eb3 8a4019               mov al, byte ptr [eax + 0x19]
00538eb6 8b8e7c010000         mov ecx, dword ptr [esi + 0x17c]
00538ebc 884515               mov byte ptr [ebp + 0x15], al
00538ebf 8d4510               lea eax, [ebp + 0x10]
00538ec2 50                   push eax
00538ec3 895d10               mov dword ptr [ebp + 0x10], ebx
00538ec6 e8e646efff           call 0x42d5b1
00538ecb 8a4018               mov al, byte ptr [eax + 0x18]
00538ece e9fa000000           jmp 0x538fcd
00538ed3 8b8664010000         mov eax, dword ptr [esi + 0x164]
00538ed9 8b08                 mov ecx, dword ptr [eax]
00538edb 8b01                 mov eax, dword ptr [ecx]
00538edd 8d5588               lea edx, [ebp - 0x78]
00538ee0 52                   push edx
00538ee1 ff90dc000000         call dword ptr [eax + 0xdc]
00538ee7 d94004               fld dword ptr [eax + 4]
00538eea d8a68c010000         fsub dword ptr [esi + 0x18c]
00538ef0 8b8664010000         mov eax, dword ptr [esi + 0x164]
00538ef6 8d5588               lea edx, [ebp - 0x78]
00538ef9 52                   push edx
00538efa d95dd4               fstp dword ptr [ebp - 0x2c]
00538efd 8b08                 mov ecx, dword ptr [eax]
00538eff 8b01                 mov eax, dword ptr [ecx]
00538f01 ff90dc000000         call dword ptr [eax + 0xdc]
00538f07 d98688010000         fld dword ptr [esi + 0x188]
00538f0d d800                 fadd dword ptr [eax]
00538f0f 8b8664010000         mov eax, dword ptr [esi + 0x164]
00538f15 8d5518               lea edx, [ebp + 0x18]
00538f18 52                   push edx
00538f19 d95dd0               fstp dword ptr [ebp - 0x30]
00538f1c 8b08                 mov ecx, dword ptr [eax]
00538f1e 8b01                 mov eax, dword ptr [ecx]
00538f20 ff90e0000000         call dword ptr [eax + 0xe0]
00538f26 0fb64003             movzx eax, byte ptr [eax + 3]
00538f2a 894510               mov dword ptr [ebp + 0x10], eax
00538f2d db4510               fild dword ptr [ebp + 0x10]
00538f30 d98698010000         fld dword ptr [esi + 0x198]
00538f36 d80d14002301         fmul dword ptr [0x1230014]
00538f3c dee9                 fsubp st(1)
00538f3e d81ddcde2201         fcomp dword ptr [0x122dedc]
00538f44 dfe0                 fnstsw ax
00538f46 f6c405               test ah, 5
00538f49 7a04                 jp 0x538f4f
00538f4b d9ee                 fldz 
00538f4d eb2c                 jmp 0x538f7b
00538f4f 8b8664010000         mov eax, dword ptr [esi + 0x164]
00538f55 8b08                 mov ecx, dword ptr [eax]
00538f57 8b01                 mov eax, dword ptr [ecx]
00538f59 8d550c               lea edx, [ebp + 0xc]
00538f5c 52                   push edx
00538f5d ff90e0000000         call dword ptr [eax + 0xe0]
00538f63 0fb64003             movzx eax, byte ptr [eax + 3]
00538f67 894510               mov dword ptr [ebp + 0x10], eax
00538f6a db4510               fild dword ptr [ebp + 0x10]
00538f6d d98698010000         fld dword ptr [esi + 0x198]
00538f73 d80d14002301         fmul dword ptr [0x1230014]
00538f79 dee9                 fsubp st(1)
00538f7b e8f05a6c00           call 0xbfea70
00538f80 884517               mov byte ptr [ebp + 0x17], al
00538f83 8b8664010000         mov eax, dword ptr [esi + 0x164]
00538f89 8b08                 mov ecx, dword ptr [eax]
00538f8b 8b01                 mov eax, dword ptr [ecx]
00538f8d 8d5510               lea edx, [ebp + 0x10]
00538f90 52                   push edx
00538f91 ff90e0000000         call dword ptr [eax + 0xe0]
00538f97 8a4002               mov al, byte ptr [eax + 2]
00538f9a 884516               mov byte ptr [ebp + 0x16], al
00538f9d 8b8664010000         mov eax, dword ptr [esi + 0x164]
00538fa3 8b08                 mov ecx, dword ptr [eax]
00538fa5 8b01                 mov eax, dword ptr [ecx]
00538fa7 8d5510               lea edx, [ebp + 0x10]
00538faa 52                   push edx
00538fab ff90e0000000         call dword ptr [eax + 0xe0]
00538fb1 8a4001               mov al, byte ptr [eax + 1]
00538fb4 884515               mov byte ptr [ebp + 0x15], al
00538fb7 8b8664010000         mov eax, dword ptr [esi + 0x164]
00538fbd 8b08                 mov ecx, dword ptr [eax]
00538fbf 8b01                 mov eax, dword ptr [ecx]
00538fc1 8d5510               lea edx, [ebp + 0x10]
00538fc4 52                   push edx
00538fc5 ff90e0000000         call dword ptr [eax + 0xe0]
00538fcb 8a00                 mov al, byte ptr [eax]
00538fcd d945fc               fld dword ptr [ebp - 4]
00538fd0 884514               mov byte ptr [ebp + 0x14], al
00538fd3 8b8664010000         mov eax, dword ptr [esi + 0x164]
00538fd9 8b08                 mov ecx, dword ptr [eax]
00538fdb 8b01                 mov eax, dword ptr [ecx]
00538fdd 6a00                 push 0
00538fdf 51                   push ecx
00538fe0 8d55d0               lea edx, [ebp - 0x30]
00538fe3 d91c24               fstp dword ptr [esp]
00538fe6 52                   push edx
00538fe7 ff507c               call dword ptr [eax + 0x7c]
00538fea d945fc               fld dword ptr [ebp - 4]
00538fed 8b8664010000         mov eax, dword ptr [esi + 0x164]
00538ff3 8b08                 mov ecx, dword ptr [eax]
00538ff5 8b01                 mov eax, dword ptr [ecx]
00538ff7 6a00                 push 0
00538ff9 51                   push ecx
00538ffa 8d5514               lea edx, [ebp + 0x14]
00538ffd d91c24               fstp dword ptr [esp]
00539000 52                   push edx
00539001 ff9098000000         call dword ptr [eax + 0x98]
00539007 8b8e7c010000         mov ecx, dword ptr [esi + 0x17c]
0053900d 33ff                 xor edi, edi
0053900f 8d4510               lea eax, [ebp + 0x10]
00539012 47                   inc edi
00539013 50                   push eax
00539014 897d10               mov dword ptr [ebp + 0x10], edi
00539017 e89545efff           call 0x42d5b1
0053901c 8b4dd0               mov ecx, dword ptr [ebp - 0x30]
0053901f 894808               mov dword ptr [eax + 8], ecx
00539022 8b4dd4               mov ecx, dword ptr [ebp - 0x2c]
00539025 89480c               mov dword ptr [eax + 0xc], ecx
00539028 8b8e7c010000         mov ecx, dword ptr [esi + 0x17c]
0053902e 8d4510               lea eax, [ebp + 0x10]
00539031 50                   push eax
00539032 c7451004000000       mov dword ptr [ebp + 0x10], 4
00539039 e87345efff           call 0x42d5b1
0053903e 8b4dd0               mov ecx, dword ptr [ebp - 0x30]
00539041 894808               mov dword ptr [eax + 8], ecx
00539044 8b4dd4               mov ecx, dword ptr [ebp - 0x2c]
00539047 89480c               mov dword ptr [eax + 0xc], ecx
0053904a 8b8e7c010000         mov ecx, dword ptr [esi + 0x17c]
00539050 8d4510               lea eax, [ebp + 0x10]
00539053 50                   push eax
00539054 895d10               mov dword ptr [ebp + 0x10], ebx
00539057 e85545efff           call 0x42d5b1
0053905c 8b4dd0               mov ecx, dword ptr [ebp - 0x30]
0053905f 894808               mov dword ptr [eax + 8], ecx
00539062 8b4dd4               mov ecx, dword ptr [ebp - 0x2c]
00539065 89480c               mov dword ptr [eax + 0xc], ecx
00539068 8b8e7c010000         mov ecx, dword ptr [esi + 0x17c]
0053906e 8d4510               lea eax, [ebp + 0x10]
00539071 50                   push eax
00539072 897d10               mov dword ptr [ebp + 0x10], edi
00539075 e83745efff           call 0x42d5b1
0053907a 8a4d17               mov cl, byte ptr [ebp + 0x17]
0053907d 88481b               mov byte ptr [eax + 0x1b], cl
00539080 8b8e7c010000         mov ecx, dword ptr [esi + 0x17c]
00539086 8d4510               lea eax, [ebp + 0x10]
00539089 50                   push eax
0053908a c7451004000000       mov dword ptr [ebp + 0x10], 4
00539091 e81b45efff           call 0x42d5b1
00539096 8a4d17               mov cl, byte ptr [ebp + 0x17]
00539099 88481b               mov byte ptr [eax + 0x1b], cl
0053909c 8bb67c010000         mov esi, dword ptr [esi + 0x17c]
005390a2 8d4510               lea eax, [ebp + 0x10]
005390a5 50                   push eax
005390a6 8bce                 mov ecx, esi
005390a8 895d10               mov dword ptr [ebp + 0x10], ebx
005390ab e80145efff           call 0x42d5b1
005390b0 8a4d17               mov cl, byte ptr [ebp + 0x17]
005390b3 88481b               mov byte ptr [eax + 0x1b], cl
005390b6 e93a020000           jmp 0x5392f5
005390bb 8b8664010000         mov eax, dword ptr [esi + 0x164]
005390c1 d945fc               fld dword ptr [ebp - 4]
005390c4 8b0cb8               mov ecx, dword ptr [eax + edi*4]
005390c7 8b01                 mov eax, dword ptr [ecx]
005390c9 6a00                 push 0
005390cb 51                   push ecx
005390cc 8d55d0               lea edx, [ebp - 0x30]
005390cf d91c24               fstp dword ptr [esp]
005390d2 52                   push edx
005390d3 ff507c               call dword ptr [eax + 0x7c]
005390d6 807d1000             cmp byte ptr [ebp + 0x10], 0
005390da 8d5510               lea edx, [ebp + 0x10]
005390dd 52                   push edx
005390de 7458                 je 0x539138
005390e0 8b8e7c010000         mov ecx, dword ptr [esi + 0x17c]
005390e6 8d047f               lea eax, [edi + edi*2]
005390e9 c1e002               shl eax, 2
005390ec 03c8                 add ecx, eax
005390ee 895d10               mov dword ptr [ebp + 0x10], ebx
005390f1 894518               mov dword ptr [ebp + 0x18], eax
005390f4 e8b844efff           call 0x42d5b1
005390f9 8a401a               mov al, byte ptr [eax + 0x1a]
005390fc 8d4d10               lea ecx, [ebp + 0x10]
005390ff 51                   push ecx
00539100 8b4d18               mov ecx, dword ptr [ebp + 0x18]
00539103 884516               mov byte ptr [ebp + 0x16], al
00539106 8b867c010000         mov eax, dword ptr [esi + 0x17c]
0053910c 03c8                 add ecx, eax
0053910e 895d10               mov dword ptr [ebp + 0x10], ebx
00539111 e89b44efff           call 0x42d5b1
00539116 8a4019               mov al, byte ptr [eax + 0x19]
00539119 8d4d10               lea ecx, [ebp + 0x10]
0053911c 51                   push ecx
0053911d 8b4d18               mov ecx, dword ptr [ebp + 0x18]
00539120 884515               mov byte ptr [ebp + 0x15], al
00539123 8b867c010000         mov eax, dword ptr [esi + 0x17c]
00539129 03c8                 add ecx, eax
0053912b 895d10               mov dword ptr [ebp + 0x10], ebx
0053912e e87e44efff           call 0x42d5b1
00539133 8a4018               mov al, byte ptr [eax + 0x18]
00539136 eb49                 jmp 0x539181
00539138 8b8664010000         mov eax, dword ptr [esi + 0x164]
0053913e 8b0cb8               mov ecx, dword ptr [eax + edi*4]
00539141 8b01                 mov eax, dword ptr [ecx]
00539143 ff90e0000000         call dword ptr [eax + 0xe0]
00539149 8a4002               mov al, byte ptr [eax + 2]
0053914c 884516               mov byte ptr [ebp + 0x16], al
0053914f 8b8664010000         mov eax, dword ptr [esi + 0x164]
00539155 8b0cb8               mov ecx, dword ptr [eax + edi*4]
00539158 8b01                 mov eax, dword ptr [ecx]
0053915a 8d5510               lea edx, [ebp + 0x10]
0053915d 52                   push edx
0053915e ff90e0000000         call dword ptr [eax + 0xe0]
00539164 8a4001               mov al, byte ptr [eax + 1]
00539167 884515               mov byte ptr [ebp + 0x15], al
0053916a 8b8664010000         mov eax, dword ptr [esi + 0x164]
00539170 8b0cb8               mov ecx, dword ptr [eax + edi*4]
00539173 8b01                 mov eax, dword ptr [ecx]
00539175 8d5510               lea edx, [ebp + 0x10]
00539178 52                   push edx
00539179 ff90e0000000         call dword ptr [eax + 0xe0]
0053917f 8a00                 mov al, byte ptr [eax]
00539181 d945fc               fld dword ptr [ebp - 4]
00539184 884514               mov byte ptr [ebp + 0x14], al
00539187 8b8664010000         mov eax, dword ptr [esi + 0x164]
0053918d 8b0cb8               mov ecx, dword ptr [eax + edi*4]
00539190 8b01                 mov eax, dword ptr [ecx]
00539192 6a00                 push 0
00539194 51                   push ecx
00539195 8d5514               lea edx, [ebp + 0x14]
00539198 d91c24               fstp dword ptr [esp]
0053919b 52                   push edx
0053919c ff9098000000         call dword ptr [eax + 0x98]
005391a2 d945fc               fld dword ptr [ebp - 4]
005391a5 8b8664010000         mov eax, dword ptr [esi + 0x164]
005391ab 8b0cb8               mov ecx, dword ptr [eax + edi*4]
005391ae 8b01                 mov eax, dword ptr [ecx]
005391b0 6a00                 push 0
005391b2 51                   push ecx
005391b3 8d55c8               lea edx, [ebp - 0x38]
005391b6 d91c24               fstp dword ptr [esp]
005391b9 52                   push edx
005391ba ff908c000000         call dword ptr [eax + 0x8c]
005391c0 8b867c010000         mov eax, dword ptr [esi + 0x17c]
005391c6 8d3c7f               lea edi, [edi + edi*2]
005391c9 8d4d10               lea ecx, [ebp + 0x10]
005391cc c1e702               shl edi, 2
005391cf 51                   push ecx
005391d0 8d0c07               lea ecx, [edi + eax]
005391d3 c7451001000000       mov dword ptr [ebp + 0x10], 1
005391da e8d243efff           call 0x42d5b1
005391df 8b4dd0               mov ecx, dword ptr [ebp - 0x30]
005391e2 894808               mov dword ptr [eax + 8], ecx
005391e5 8b4dd4               mov ecx, dword ptr [ebp - 0x2c]
005391e8 89480c               mov dword ptr [eax + 0xc], ecx
005391eb 8b867c010000         mov eax, dword ptr [esi + 0x17c]
005391f1 8d4d10               lea ecx, [ebp + 0x10]
005391f4 51                   push ecx
005391f5 8d0c07               lea ecx, [edi + eax]
005391f8 c7451004000000       mov dword ptr [ebp + 0x10], 4
005391ff e8ad43efff           call 0x42d5b1
00539204 8b4dd0               mov ecx, dword ptr [ebp - 0x30]
00539207 894808               mov dword ptr [eax + 8], ecx
0053920a 8b4dd4               mov ecx, dword ptr [ebp - 0x2c]
0053920d 89480c               mov dword ptr [eax + 0xc], ecx
00539210 8b867c010000         mov eax, dword ptr [esi + 0x17c]
00539216 8d4d10               lea ecx, [ebp + 0x10]
00539219 51                   push ecx
0053921a 8d0c07               lea ecx, [edi + eax]
0053921d 895d10               mov dword ptr [ebp + 0x10], ebx
00539220 e88c43efff           call 0x42d5b1
00539225 8b4dd0               mov ecx, dword ptr [ebp - 0x30]
00539228 894808               mov dword ptr [eax + 8], ecx
0053922b 8b4dd4               mov ecx, dword ptr [ebp - 0x2c]
0053922e 89480c               mov dword ptr [eax + 0xc], ecx
00539231 8b867c010000         mov eax, dword ptr [esi + 0x17c]
00539237 8d4d10               lea ecx, [ebp + 0x10]
0053923a 51                   push ecx
0053923b 8d0c07               lea ecx, [edi + eax]
0053923e c7451001000000       mov dword ptr [ebp + 0x10], 1
00539245 e86743efff           call 0x42d5b1
0053924a 8a4d17               mov cl, byte ptr [ebp + 0x17]
0053924d 88481b               mov byte ptr [eax + 0x1b], cl
00539250 8b867c010000         mov eax, dword ptr [esi + 0x17c]
00539256 8d4d10               lea ecx, [ebp + 0x10]
00539259 51                   push ecx
0053925a 8d0c07               lea ecx, [edi + eax]
0053925d c7451004000000       mov dword ptr [ebp + 0x10], 4
00539264 e84843efff           call 0x42d5b1
00539269 8a4d17               mov cl, byte ptr [ebp + 0x17]
0053926c 88481b               mov byte ptr [eax + 0x1b], cl
0053926f 8b867c010000         mov eax, dword ptr [esi + 0x17c]
00539275 8d4d10               lea ecx, [ebp + 0x10]
00539278 51                   push ecx
00539279 8d0c07               lea ecx, [edi + eax]
0053927c 895d10               mov dword ptr [ebp + 0x10], ebx
0053927f e82d43efff           call 0x42d5b1
00539284 8a4d17               mov cl, byte ptr [ebp + 0x17]
00539287 88481b               mov byte ptr [eax + 0x1b], cl
0053928a 8b867c010000         mov eax, dword ptr [esi + 0x17c]
00539290 8d4d10               lea ecx, [ebp + 0x10]
00539293 c7451001000000       mov dword ptr [ebp + 0x10], 1
0053929a 51                   push ecx
0053929b 8d0c07               lea ecx, [edi + eax]
0053929e e80e43efff           call 0x42d5b1
005392a3 8b4dc8               mov ecx, dword ptr [ebp - 0x38]
005392a6 894810               mov dword ptr [eax + 0x10], ecx
005392a9 8b4dcc               mov ecx, dword ptr [ebp - 0x34]
005392ac 894814               mov dword ptr [eax + 0x14], ecx
005392af 8b867c010000         mov eax, dword ptr [esi + 0x17c]
005392b5 8d4d10               lea ecx, [ebp + 0x10]
005392b8 51                   push ecx
005392b9 8d0c07               lea ecx, [edi + eax]
005392bc c7451004000000       mov dword ptr [ebp + 0x10], 4
005392c3 e8e942efff           call 0x42d5b1
005392c8 8b4dc8               mov ecx, dword ptr [ebp - 0x38]
005392cb 894810               mov dword ptr [eax + 0x10], ecx
005392ce 8b4dcc               mov ecx, dword ptr [ebp - 0x34]
005392d1 894814               mov dword ptr [eax + 0x14], ecx
005392d4 8bb67c010000         mov esi, dword ptr [esi + 0x17c]
005392da 8d4510               lea eax, [ebp + 0x10]
005392dd 50                   push eax
005392de 8d0c37               lea ecx, [edi + esi]
005392e1 895d10               mov dword ptr [ebp + 0x10], ebx
005392e4 e8c842efff           call 0x42d5b1
005392e9 8b4dc8               mov ecx, dword ptr [ebp - 0x38]
005392ec 894810               mov dword ptr [eax + 0x10], ecx
005392ef 8b4dcc               mov ecx, dword ptr [ebp - 0x34]
005392f2 894814               mov dword ptr [eax + 0x14], ecx
005392f5 5b                   pop ebx
005392f6 5f                   pop edi
005392f7 5e                   pop esi
005392f8 c9                   leave 
005392f9 c21400               ret 0x14
