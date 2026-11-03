005379f5 55                   push ebp
005379f6 8d6c2488             lea ebp, [esp - 0x78]
005379fa 81ecec000000         sub esp, 0xec
00537a00 83657000             and dword ptr [ebp + 0x70], 0
00537a04 53                   push ebx
00537a05 56                   push esi
00537a06 8bf1                 mov esi, ecx
00537a08 8b8668010000         mov eax, dword ptr [esi + 0x168]
00537a0e 2b8664010000         sub eax, dword ptr [esi + 0x164]
00537a14 57                   push edi
00537a15 c1f802               sar eax, 2
00537a18 0f84e0040000         je 0x537efe
00537a1e 8b5d70               mov ebx, dword ptr [ebp + 0x70]
00537a21 8b8664010000         mov eax, dword ptr [esi + 0x164]
00537a27 c1e302               shl ebx, 2
00537a2a 8b0c03               mov ecx, dword ptr [ebx + eax]
00537a2d 8b01                 mov eax, dword ptr [ecx]
00537a2f ff90d0000000         call dword ptr [eax + 0xd0]
00537a35 3bc6                 cmp eax, esi
00537a37 0f85a6040000         jne 0x537ee3
00537a3d 8b8664010000         mov eax, dword ptr [esi + 0x164]
00537a43 8b3c03               mov edi, dword ptr [ebx + eax]
00537a46 8b07                 mov eax, dword ptr [edi]
00537a48 8d4d8c               lea ecx, [ebp - 0x74]
00537a4b 51                   push ecx
00537a4c 8bcf                 mov ecx, edi
00537a4e ff90e8010000         call dword ptr [eax + 0x1e8]
00537a54 8b4004               mov eax, dword ptr [eax + 4]
00537a57 8d4d9c               lea ecx, [ebp - 0x64]
00537a5a 894544               mov dword ptr [ebp + 0x44], eax
00537a5d 8b07                 mov eax, dword ptr [edi]
00537a5f 51                   push ecx
00537a60 8bcf                 mov ecx, edi
00537a62 ff90e8010000         call dword ptr [eax + 0x1e8]
00537a68 8b00                 mov eax, dword ptr [eax]
00537a6a 8945e4               mov dword ptr [ebp - 0x1c], eax
00537a6d 8b8664010000         mov eax, dword ptr [esi + 0x164]
00537a73 8b3c03               mov edi, dword ptr [ebx + eax]
00537a76 8b07                 mov eax, dword ptr [edi]
00537a78 8d4dc4               lea ecx, [ebp - 0x3c]
00537a7b 51                   push ecx
00537a7c 8bcf                 mov ecx, edi
00537a7e ff505c               call dword ptr [eax + 0x5c]
00537a81 8b4004               mov eax, dword ptr [eax + 4]
00537a84 8d4db4               lea ecx, [ebp - 0x4c]
00537a87 894504               mov dword ptr [ebp + 4], eax
00537a8a 8b07                 mov eax, dword ptr [edi]
00537a8c 51                   push ecx
00537a8d 8bcf                 mov ecx, edi
00537a8f ff505c               call dword ptr [eax + 0x5c]
00537a92 8b00                 mov eax, dword ptr [eax]
00537a94 89450c               mov dword ptr [ebp + 0xc], eax
00537a97 8d4594               lea eax, [ebp - 0x6c]
00537a9a 50                   push eax
00537a9b e8526beeff           call 0x41e5f2
00537aa0 8bc8                 mov ecx, eax
00537aa2 e8a054eeff           call 0x41cf47
00537aa7 d9450c               fld dword ptr [ebp + 0xc]
00537aaa d808                 fmul dword ptr [eax]
00537aac 8d45a4               lea eax, [ebp - 0x5c]
00537aaf 50                   push eax
00537ab0 d95d0c               fstp dword ptr [ebp + 0xc]
00537ab3 e83a6beeff           call 0x41e5f2
00537ab8 8bc8                 mov ecx, eax
00537aba e88854eeff           call 0x41cf47
00537abf d94504               fld dword ptr [ebp + 4]
00537ac2 d84804               fmul dword ptr [eax + 4]
00537ac5 8b06                 mov eax, dword ptr [esi]
00537ac7 d945e4               fld dword ptr [ebp - 0x1c]
00537aca 8d4d30               lea ecx, [ebp + 0x30]
00537acd d8450c               fadd dword ptr [ebp + 0xc]
00537ad0 51                   push ecx
00537ad1 8bce                 mov ecx, esi
00537ad3 d95dd8               fstp dword ptr [ebp - 0x28]
00537ad6 d94544               fld dword ptr [ebp + 0x44]
00537ad9 d8c1                 fadd st(1)
00537adb d95ddc               fstp dword ptr [ebp - 0x24]
00537ade ddd8                 fstp st(0)
00537ae0 ff90b0010000         call dword ptr [eax + 0x1b0]
00537ae6 8b38                 mov edi, dword ptr [eax]
00537ae8 8b06                 mov eax, dword ptr [esi]
00537aea 8d4d40               lea ecx, [ebp + 0x40]
00537aed 51                   push ecx
00537aee 8bce                 mov ecx, esi
00537af0 ff90b0010000         call dword ptr [eax + 0x1b0]
00537af6 8b00                 mov eax, dword ptr [eax]
00537af8 8d4d54               lea ecx, [ebp + 0x54]
00537afb 89451c               mov dword ptr [ebp + 0x1c], eax
00537afe 8b06                 mov eax, dword ptr [esi]
00537b00 51                   push ecx
00537b01 8bce                 mov ecx, esi
00537b03 ff90b0010000         call dword ptr [eax + 0x1b0]
00537b09 8b00                 mov eax, dword ptr [eax]
00537b0b 8d4d48               lea ecx, [ebp + 0x48]
00537b0e 89456c               mov dword ptr [ebp + 0x6c], eax
00537b11 8b06                 mov eax, dword ptr [esi]
00537b13 51                   push ecx
00537b14 8bce                 mov ecx, esi
00537b16 ff90b0010000         call dword ptr [eax + 0x1b0]
00537b1c 8b00                 mov eax, dword ptr [eax]
00537b1e 894508               mov dword ptr [ebp + 8], eax
00537b21 8b06                 mov eax, dword ptr [esi]
00537b23 8d4d38               lea ecx, [ebp + 0x38]
00537b26 51                   push ecx
00537b27 8bce                 mov ecx, esi
00537b29 ff90b0010000         call dword ptr [eax + 0x1b0]
00537b2f 8b00                 mov eax, dword ptr [eax]
00537b31 8d4d24               lea ecx, [ebp + 0x24]
00537b34 894520               mov dword ptr [ebp + 0x20], eax
00537b37 8b06                 mov eax, dword ptr [esi]
00537b39 51                   push ecx
00537b3a 8bce                 mov ecx, esi
00537b3c ff90b0010000         call dword ptr [eax + 0x1b0]
00537b42 8b00                 mov eax, dword ptr [eax]
00537b44 8d4d2c               lea ecx, [ebp + 0x2c]
00537b47 894514               mov dword ptr [ebp + 0x14], eax
00537b4a 8b06                 mov eax, dword ptr [esi]
00537b4c 51                   push ecx
00537b4d 8bce                 mov ecx, esi
00537b4f ff90b0010000         call dword ptr [eax + 0x1b0]
00537b55 8b00                 mov eax, dword ptr [eax]
00537b57 8d4d28               lea ecx, [ebp + 0x28]
00537b5a 894518               mov dword ptr [ebp + 0x18], eax
00537b5d 8b06                 mov eax, dword ptr [esi]
00537b5f 51                   push ecx
00537b60 8bce                 mov ecx, esi
00537b62 ff90b0010000         call dword ptr [eax + 0x1b0]
00537b68 8b00                 mov eax, dword ptr [eax]
00537b6a db804c010000         fild dword ptr [eax + 0x14c]
00537b70 51                   push ecx
00537b71 d91c24               fstp dword ptr [esp]
00537b74 e8f768ffff           call 0x52e470
00537b79 8b4518               mov eax, dword ptr [ebp + 0x18]
00537b7c d95d5c               fstp dword ptr [ebp + 0x5c]
00537b7f db8050010000         fild dword ptr [eax + 0x150]
00537b85 51                   push ecx
00537b86 d91c24               fstp dword ptr [esp]
00537b89 e84269ffff           call 0x52e4d0
00537b8e 8b4508               mov eax, dword ptr [ebp + 8]
00537b91 d95d60               fstp dword ptr [ebp + 0x60]
00537b94 db804c010000         fild dword ptr [eax + 0x14c]
00537b9a 51                   push ecx
00537b9b d91c24               fstp dword ptr [esp]
00537b9e e8cd68ffff           call 0x52e470
00537ba3 8b4520               mov eax, dword ptr [ebp + 0x20]
00537ba6 daa04c010000         fisub dword ptr [eax + 0x14c]
00537bac 8b4514               mov eax, dword ptr [ebp + 0x14]
00537baf 51                   push ecx
00537bb0 da8054010000         fiadd dword ptr [eax + 0x154]
00537bb6 d95d64               fstp dword ptr [ebp + 0x64]
00537bb9 db8750010000         fild dword ptr [edi + 0x150]
00537bbf d91c24               fstp dword ptr [esp]
00537bc2 e80969ffff           call 0x52e4d0
00537bc7 8b451c               mov eax, dword ptr [ebp + 0x1c]
00537bca daa050010000         fisub dword ptr [eax + 0x150]
00537bd0 8b456c               mov eax, dword ptr [ebp + 0x6c]
00537bd3 33ff                 xor edi, edi
00537bd5 da8058010000         fiadd dword ptr [eax + 0x158]
00537bdb 8b4528               mov eax, dword ptr [ebp + 0x28]
00537bde 85c0                 test eax, eax
00537be0 d95d68               fstp dword ptr [ebp + 0x68]
00537be3 7413                 je 0x537bf8
00537be5 8bc8                 mov ecx, eax
00537be7 83c004               add eax, 4
00537bea ff08                 dec dword ptr [eax]
00537bec 3938                 cmp dword ptr [eax], edi
00537bee 7505                 jne 0x537bf5
00537bf0 8b01                 mov eax, dword ptr [ecx]
00537bf2 ff5004               call dword ptr [eax + 4]
00537bf5 897d28               mov dword ptr [ebp + 0x28], edi
00537bf8 8b452c               mov eax, dword ptr [ebp + 0x2c]
00537bfb 3bc7                 cmp eax, edi
00537bfd 7413                 je 0x537c12
00537bff 8bc8                 mov ecx, eax
00537c01 83c004               add eax, 4
00537c04 ff08                 dec dword ptr [eax]
00537c06 3938                 cmp dword ptr [eax], edi
00537c08 7505                 jne 0x537c0f
00537c0a 8b01                 mov eax, dword ptr [ecx]
00537c0c ff5004               call dword ptr [eax + 4]
00537c0f 897d2c               mov dword ptr [ebp + 0x2c], edi
00537c12 8b4524               mov eax, dword ptr [ebp + 0x24]
00537c15 3bc7                 cmp eax, edi
00537c17 7413                 je 0x537c2c
00537c19 8bc8                 mov ecx, eax
00537c1b 83c004               add eax, 4
00537c1e ff08                 dec dword ptr [eax]
00537c20 3938                 cmp dword ptr [eax], edi
00537c22 7505                 jne 0x537c29
00537c24 8b01                 mov eax, dword ptr [ecx]
00537c26 ff5004               call dword ptr [eax + 4]
00537c29 897d24               mov dword ptr [ebp + 0x24], edi
00537c2c 8b4538               mov eax, dword ptr [ebp + 0x38]
00537c2f 3bc7                 cmp eax, edi
00537c31 7413                 je 0x537c46
00537c33 8bc8                 mov ecx, eax
00537c35 83c004               add eax, 4
00537c38 ff08                 dec dword ptr [eax]
00537c3a 3938                 cmp dword ptr [eax], edi
00537c3c 7505                 jne 0x537c43
00537c3e 8b01                 mov eax, dword ptr [ecx]
00537c40 ff5004               call dword ptr [eax + 4]
00537c43 897d38               mov dword ptr [ebp + 0x38], edi
00537c46 8b4548               mov eax, dword ptr [ebp + 0x48]
00537c49 3bc7                 cmp eax, edi
00537c4b 7413                 je 0x537c60
00537c4d 8bc8                 mov ecx, eax
00537c4f 83c004               add eax, 4
00537c52 ff08                 dec dword ptr [eax]
00537c54 3938                 cmp dword ptr [eax], edi
00537c56 7505                 jne 0x537c5d
00537c58 8b01                 mov eax, dword ptr [ecx]
00537c5a ff5004               call dword ptr [eax + 4]
00537c5d 897d48               mov dword ptr [ebp + 0x48], edi
00537c60 8b4554               mov eax, dword ptr [ebp + 0x54]
00537c63 3bc7                 cmp eax, edi
00537c65 7413                 je 0x537c7a
00537c67 8bc8                 mov ecx, eax
00537c69 83c004               add eax, 4
00537c6c ff08                 dec dword ptr [eax]
00537c6e 3938                 cmp dword ptr [eax], edi
00537c70 7505                 jne 0x537c77
00537c72 8b01                 mov eax, dword ptr [ecx]
00537c74 ff5004               call dword ptr [eax + 4]
00537c77 897d54               mov dword ptr [ebp + 0x54], edi
00537c7a 8b4540               mov eax, dword ptr [ebp + 0x40]
00537c7d 3bc7                 cmp eax, edi
00537c7f 7413                 je 0x537c94
00537c81 8bc8                 mov ecx, eax
00537c83 83c004               add eax, 4
00537c86 ff08                 dec dword ptr [eax]
00537c88 3938                 cmp dword ptr [eax], edi
00537c8a 7505                 jne 0x537c91
00537c8c 8b01                 mov eax, dword ptr [ecx]
00537c8e ff5004               call dword ptr [eax + 4]
00537c91 897d40               mov dword ptr [ebp + 0x40], edi
00537c94 8b4530               mov eax, dword ptr [ebp + 0x30]
00537c97 3bc7                 cmp eax, edi
00537c99 7413                 je 0x537cae
00537c9b 8bc8                 mov ecx, eax
00537c9d 83c004               add eax, 4
00537ca0 ff08                 dec dword ptr [eax]
00537ca2 3938                 cmp dword ptr [eax], edi
00537ca4 7505                 jne 0x537cab
00537ca6 8b01                 mov eax, dword ptr [ecx]
00537ca8 ff5004               call dword ptr [eax + 4]
00537cab 897d30               mov dword ptr [ebp + 0x30], edi
00537cae 8d86d4010000         lea eax, [esi + 0x1d4]
00537cb4 3938                 cmp dword ptr [eax], edi
00537cb6 0f8480000000         je 0x537d3c
00537cbc 8b08                 mov ecx, dword ptr [eax]
00537cbe 8b01                 mov eax, dword ptr [ecx]
00537cc0 8d55ec               lea edx, [ebp - 0x14]
00537cc3 52                   push edx
00537cc4 ff90d8010000         call dword ptr [eax + 0x1d8]
00537cca 8b45f0               mov eax, dword ptr [ebp - 0x10]
00537ccd 8d4d4c               lea ecx, [ebp + 0x4c]
00537cd0 894560               mov dword ptr [ebp + 0x60], eax
00537cd3 8b06                 mov eax, dword ptr [esi]
00537cd5 51                   push ecx
00537cd6 8bce                 mov ecx, esi
00537cd8 ff90b0010000         call dword ptr [eax + 0x1b0]
00537cde 8b00                 mov eax, dword ptr [eax]
00537ce0 8d4d50               lea ecx, [ebp + 0x50]
00537ce3 89456c               mov dword ptr [ebp + 0x6c], eax
00537ce6 8b06                 mov eax, dword ptr [esi]
00537ce8 51                   push ecx
00537ce9 8bce                 mov ecx, esi
00537ceb ff90b0010000         call dword ptr [eax + 0x1b0]
00537cf1 8b4d6c               mov ecx, dword ptr [ebp + 0x6c]
00537cf4 db8158010000         fild dword ptr [ecx + 0x158]
00537cfa 8b00                 mov eax, dword ptr [eax]
00537cfc d845f0               fadd dword ptr [ebp - 0x10]
00537cff daa050010000         fisub dword ptr [eax + 0x150]
00537d05 8b4550               mov eax, dword ptr [ebp + 0x50]
00537d08 3bc7                 cmp eax, edi
00537d0a d95d68               fstp dword ptr [ebp + 0x68]
00537d0d 7413                 je 0x537d22
00537d0f 8bc8                 mov ecx, eax
00537d11 83c004               add eax, 4
00537d14 ff08                 dec dword ptr [eax]
00537d16 3938                 cmp dword ptr [eax], edi
00537d18 7505                 jne 0x537d1f
00537d1a 8b01                 mov eax, dword ptr [ecx]
00537d1c ff5004               call dword ptr [eax + 4]
00537d1f 897d50               mov dword ptr [ebp + 0x50], edi
00537d22 8b454c               mov eax, dword ptr [ebp + 0x4c]
00537d25 3bc7                 cmp eax, edi
00537d27 7413                 je 0x537d3c
00537d29 8bc8                 mov ecx, eax
00537d2b 83c004               add eax, 4
00537d2e ff08                 dec dword ptr [eax]
00537d30 3938                 cmp dword ptr [eax], edi
00537d32 7505                 jne 0x537d39
00537d34 8b01                 mov eax, dword ptr [ecx]
00537d36 ff5004               call dword ptr [eax + 4]
00537d39 897d4c               mov dword ptr [ebp + 0x4c], edi
00537d3c d905d8de2201         fld dword ptr [0x122ded8]
00537d42 8d45bc               lea eax, [ebp - 0x44]
00537d45 d94564               fld dword ptr [ebp + 0x64]
00537d48 50                   push eax
00537d49 d8c1                 fadd st(1)
00537d4b d95d64               fstp dword ptr [ebp + 0x64]
00537d4e d94568               fld dword ptr [ebp + 0x68]
00537d51 d8c1                 fadd st(1)
00537d53 d95d68               fstp dword ptr [ebp + 0x68]
00537d56 d9455c               fld dword ptr [ebp + 0x5c]
00537d59 d8e1                 fsub st(1)
00537d5b d95d5c               fstp dword ptr [ebp + 0x5c]
00537d5e d94560               fld dword ptr [ebp + 0x60]
00537d61 d8e1                 fsub st(1)
00537d63 d95d60               fstp dword ptr [ebp + 0x60]
00537d66 ddd8                 fstp st(0)
00537d68 e88568eeff           call 0x41e5f2
00537d6d 8bc8                 mov ecx, eax
00537d6f e8d351eeff           call 0x41cf47
00537d74 d94564               fld dword ptr [ebp + 0x64]
00537d77 d8655c               fsub dword ptr [ebp + 0x5c]
00537d7a d808                 fmul dword ptr [eax]
00537d7c 8d45ac               lea eax, [ebp - 0x54]
00537d7f 50                   push eax
00537d80 d8455c               fadd dword ptr [ebp + 0x5c]
00537d83 d95d64               fstp dword ptr [ebp + 0x64]
00537d86 e86768eeff           call 0x41e5f2
00537d8b 8bc8                 mov ecx, eax
00537d8d e8b551eeff           call 0x41cf47
00537d92 d94568               fld dword ptr [ebp + 0x68]
00537d95 d86560               fsub dword ptr [ebp + 0x60]
00537d98 8d4d5c               lea ecx, [ebp + 0x5c]
00537d9b d84804               fmul dword ptr [eax + 4]
00537d9e 8b45e4               mov eax, dword ptr [ebp - 0x1c]
00537da1 8945f4               mov dword ptr [ebp - 0xc], eax
00537da4 8b4544               mov eax, dword ptr [ebp + 0x44]
00537da7 d84560               fadd dword ptr [ebp + 0x60]
00537daa 8945f8               mov dword ptr [ebp - 8], eax
00537dad 8d45f4               lea eax, [ebp - 0xc]
00537db0 50                   push eax
00537db1 d95d68               fstp dword ptr [ebp + 0x68]
00537db4 e817dfffff           call 0x535cd0
00537db9 84c0                 test al, al
00537dbb 0f84a4000000         je 0x537e65
00537dc1 8b45d8               mov eax, dword ptr [ebp - 0x28]
00537dc4 8945fc               mov dword ptr [ebp - 4], eax
00537dc7 8b45dc               mov eax, dword ptr [ebp - 0x24]
00537dca 894500               mov dword ptr [ebp], eax
00537dcd 8d45fc               lea eax, [ebp - 4]
00537dd0 50                   push eax
00537dd1 8d4d5c               lea ecx, [ebp + 0x5c]
00537dd4 e8f7deffff           call 0x535cd0
00537dd9 84c0                 test al, al
00537ddb 0f8484000000         je 0x537e65
00537de1 8b8664010000         mov eax, dword ptr [esi + 0x164]
00537de7 8b0c03               mov ecx, dword ptr [ebx + eax]
00537dea 8b01                 mov eax, dword ptr [ecx]
00537dec 8d55cc               lea edx, [ebp - 0x34]
00537def 52                   push edx
00537df0 ff90e0000000         call dword ptr [eax + 0xe0]
00537df6 80780300             cmp byte ptr [eax + 3], 0
00537dfa 7550                 jne 0x537e4c
00537dfc 8b8664010000         mov eax, dword ptr [esi + 0x164]
00537e02 8b0c03               mov ecx, dword ptr [ebx + eax]
00537e05 8b01                 mov eax, dword ptr [ecx]
00537e07 8d553c               lea edx, [ebp + 0x3c]
00537e0a 52                   push edx
00537e0b ff5038               call dword ptr [eax + 0x38]
00537e0e d9ee                 fldz 
00537e10 8a453e               mov al, byte ptr [ebp + 0x3e]
00537e13 88455a               mov byte ptr [ebp + 0x5a], al
00537e16 8b453c               mov eax, dword ptr [ebp + 0x3c]
00537e19 886559               mov byte ptr [ebp + 0x59], ah
00537e1c 884558               mov byte ptr [ebp + 0x58], al
00537e1f 8b8664010000         mov eax, dword ptr [esi + 0x164]
00537e25 57                   push edi
00537e26 51                   push ecx
00537e27 c6455bff             mov byte ptr [ebp + 0x5b], 0xff
00537e2b d91c24               fstp dword ptr [esp]
00537e2e 8b1c03               mov ebx, dword ptr [ebx + eax]
00537e31 8b03                 mov eax, dword ptr [ebx]
00537e33 8d4d58               lea ecx, [ebp + 0x58]
00537e36 51                   push ecx
00537e37 8bcb                 mov ecx, ebx
00537e39 ff9098000000         call dword ptr [eax + 0x98]
00537e3f 8b86c8010000         mov eax, dword ptr [esi + 0x1c8]
00537e45 8b4d70               mov ecx, dword ptr [ebp + 0x70]
00537e48 c6040101             mov byte ptr [ecx + eax], 1
00537e4c 8b86c8010000         mov eax, dword ptr [esi + 0x1c8]
00537e52 8b4d70               mov ecx, dword ptr [ebp + 0x70]
00537e55 03c1                 add eax, ecx
00537e57 803800               cmp byte ptr [eax], 0
00537e5a 0f8583000000         jne 0x537ee3
00537e60 c60001               mov byte ptr [eax], 1
00537e63 eb7e                 jmp 0x537ee3
00537e65 8b8664010000         mov eax, dword ptr [esi + 0x164]
00537e6b 8b0c03               mov ecx, dword ptr [ebx + eax]
00537e6e 8b01                 mov eax, dword ptr [ecx]
00537e70 8d55e0               lea edx, [ebp - 0x20]
00537e73 52                   push edx
00537e74 ff90e0000000         call dword ptr [eax + 0xe0]
00537e7a 80780300             cmp byte ptr [eax + 3], 0
00537e7e 7450                 je 0x537ed0
00537e80 8b8664010000         mov eax, dword ptr [esi + 0x164]
00537e86 8b0c03               mov ecx, dword ptr [ebx + eax]
00537e89 8b01                 mov eax, dword ptr [ecx]
00537e8b 8d5534               lea edx, [ebp + 0x34]
00537e8e 52                   push edx
00537e8f ff5038               call dword ptr [eax + 0x38]
00537e92 d9ee                 fldz 
00537e94 8a4536               mov al, byte ptr [ebp + 0x36]
00537e97 884576               mov byte ptr [ebp + 0x76], al
00537e9a 8b4534               mov eax, dword ptr [ebp + 0x34]
00537e9d 886575               mov byte ptr [ebp + 0x75], ah
00537ea0 884574               mov byte ptr [ebp + 0x74], al
00537ea3 8b8664010000         mov eax, dword ptr [esi + 0x164]
00537ea9 57                   push edi
00537eaa 51                   push ecx
00537eab c6457700             mov byte ptr [ebp + 0x77], 0
00537eaf d91c24               fstp dword ptr [esp]
00537eb2 8b1c03               mov ebx, dword ptr [ebx + eax]
00537eb5 8b03                 mov eax, dword ptr [ebx]
00537eb7 8d4d74               lea ecx, [ebp + 0x74]
00537eba 51                   push ecx
00537ebb 8bcb                 mov ecx, ebx
00537ebd ff9098000000         call dword ptr [eax + 0x98]
00537ec3 8b86c8010000         mov eax, dword ptr [esi + 0x1c8]
00537ec9 8b4d70               mov ecx, dword ptr [ebp + 0x70]
00537ecc c6040100             mov byte ptr [ecx + eax], 0
00537ed0 8b86c8010000         mov eax, dword ptr [esi + 0x1c8]
00537ed6 8b4d70               mov ecx, dword ptr [ebp + 0x70]
00537ed9 03c1                 add eax, ecx
00537edb 803800               cmp byte ptr [eax], 0
00537ede 7403                 je 0x537ee3
00537ee0 c60000               mov byte ptr [eax], 0
00537ee3 8b8668010000         mov eax, dword ptr [esi + 0x168]
00537ee9 2b8664010000         sub eax, dword ptr [esi + 0x164]
00537eef ff4570               inc dword ptr [ebp + 0x70]
00537ef2 c1f802               sar eax, 2
00537ef5 394570               cmp dword ptr [ebp + 0x70], eax
00537ef8 0f8220fbffff         jb 0x537a1e
00537efe 8b8668010000         mov eax, dword ptr [esi + 0x168]
00537f04 2b8664010000         sub eax, dword ptr [esi + 0x164]
00537f0a 33ff                 xor edi, edi
00537f0c c1f802               sar eax, 2
00537f0f 6a03                 push 3
00537f11 5b                   pop ebx
00537f12 0f84a5000000         je 0x537fbd
00537f18 8b8664010000         mov eax, dword ptr [esi + 0x164]
00537f1e 8b0cb8               mov ecx, dword ptr [eax + edi*4]
00537f21 8b01                 mov eax, dword ptr [ecx]
00537f23 ff90d0000000         call dword ptr [eax + 0xd0]
00537f29 3bc6                 cmp eax, esi
00537f2b 7578                 jne 0x537fa5
00537f2d 8b86c8010000         mov eax, dword ptr [esi + 0x1c8]
00537f33 803c0700             cmp byte ptr [edi + eax], 0
00537f37 8b06                 mov eax, dword ptr [esi]
00537f39 8bce                 mov ecx, esi
00537f3b 7435                 je 0x537f72
00537f3d ff9058010000         call dword ptr [eax + 0x158]
00537f43 3bc3                 cmp eax, ebx
00537f45 755e                 jne 0x537fa5
00537f47 8b06                 mov eax, dword ptr [esi]
00537f49 8bce                 mov ecx, esi
00537f4b ff905c010000         call dword ptr [eax + 0x15c]
00537f51 3bc3                 cmp eax, ebx
00537f53 7550                 jne 0x537fa5
00537f55 399e4c010000         cmp dword ptr [esi + 0x14c], ebx
00537f5b 7548                 jne 0x537fa5
00537f5d 8b8664010000         mov eax, dword ptr [esi + 0x164]
00537f63 8b04b8               mov eax, dword ptr [eax + edi*4]
00537f66 8d4804               lea ecx, [eax + 4]
00537f69 8b01                 mov eax, dword ptr [ecx]
00537f6b 6a19                 push 0x19
00537f6d ff500c               call dword ptr [eax + 0xc]
00537f70 eb33                 jmp 0x537fa5
00537f72 ff9058010000         call dword ptr [eax + 0x158]
00537f78 3bc3                 cmp eax, ebx
00537f7a 7529                 jne 0x537fa5
00537f7c 8b06                 mov eax, dword ptr [esi]
00537f7e 8bce                 mov ecx, esi
00537f80 ff905c010000         call dword ptr [eax + 0x15c]
00537f86 3bc3                 cmp eax, ebx
00537f88 751b                 jne 0x537fa5
00537f8a 399e4c010000         cmp dword ptr [esi + 0x14c], ebx
00537f90 7513                 jne 0x537fa5
00537f92 8b8664010000         mov eax, dword ptr [esi + 0x164]
00537f98 8b04b8               mov eax, dword ptr [eax + edi*4]
00537f9b 8d4804               lea ecx, [eax + 4]
00537f9e 8b01                 mov eax, dword ptr [ecx]
00537fa0 6a19                 push 0x19
00537fa2 ff5010               call dword ptr [eax + 0x10]
00537fa5 8b8668010000         mov eax, dword ptr [esi + 0x168]
00537fab 2b8664010000         sub eax, dword ptr [esi + 0x164]
00537fb1 47                   inc edi
00537fb2 c1f802               sar eax, 2
00537fb5 3bf8                 cmp edi, eax
00537fb7 0f825bffffff         jb 0x537f18
00537fbd 8b865c010000         mov eax, dword ptr [esi + 0x15c]
00537fc3 33c9                 xor ecx, ecx
00537fc5 3bc1                 cmp eax, ecx
00537fc7 0f8c56020000         jl 0x538223
00537fcd 8b96cc010000         mov edx, dword ptr [esi + 0x1cc]
00537fd3 2b96c8010000         sub edx, dword ptr [esi + 0x1c8]
00537fd9 3bc2                 cmp eax, edx
00537fdb 0f8342020000         jae 0x538223
00537fe1 8b96c8010000         mov edx, dword ptr [esi + 0x1c8]
00537fe7 380c10               cmp byte ptr [eax + edx], cl
00537fea 0f8533020000         jne 0x538223
00537ff0 8b9ecc010000         mov ebx, dword ptr [esi + 0x1cc]
00537ff6 83cfff               or edi, 0xffffffff
00537ff9 2bda                 sub ebx, edx
00537ffb 897d74               mov dword ptr [ebp + 0x74], edi
00537ffe 894d6c               mov dword ptr [ebp + 0x6c], ecx
00538001 7430                 je 0x538033
00538003 397d74               cmp dword ptr [ebp + 0x74], edi
00538006 750d                 jne 0x538015
00538008 803c0a00             cmp byte ptr [edx + ecx], 0
0053800c 740d                 je 0x53801b
0053800e 3bcf                 cmp ecx, edi
00538010 894d74               mov dword ptr [ebp + 0x74], ecx
00538013 7406                 je 0x53801b
00538015 803c0a00             cmp byte ptr [edx + ecx], 0
00538019 7413                 je 0x53802e
0053801b 8b9ecc010000         mov ebx, dword ptr [esi + 0x1cc]
00538021 2b9ec8010000         sub ebx, dword ptr [esi + 0x1c8]
00538027 41                   inc ecx
00538028 3bcb                 cmp ecx, ebx
0053802a 72d7                 jb 0x538003
0053802c eb05                 jmp 0x538033
0053802e 49                   dec ecx
0053802f 3bcf                 cmp ecx, edi
00538031 750d                 jne 0x538040
00538033 8b8ecc010000         mov ecx, dword ptr [esi + 0x1cc]
00538039 2b8ec8010000         sub ecx, dword ptr [esi + 0x1c8]
0053803f 49                   dec ecx
00538040 3bc1                 cmp eax, ecx
00538042 7e04                 jle 0x538048
00538044 8bd9                 mov ebx, ecx
00538046 eb07                 jmp 0x53804f
00538048 8b5d74               mov ebx, dword ptr [ebp + 0x74]
0053804b 3bdf                 cmp ebx, edi
0053804d 740a                 je 0x538059
0053804f 3bdf                 cmp ebx, edi
00538051 0f84cc010000         je 0x538223
00538057 eb03                 jmp 0x53805c
00538059 8b5d6c               mov ebx, dword ptr [ebp + 0x6c]
0053805c 8b8e64010000         mov ecx, dword ptr [esi + 0x164]
00538062 8b0481               mov eax, dword ptr [ecx + eax*4]
00538065 8d4804               lea ecx, [eax + 4]
00538068 8b01                 mov eax, dword ptr [ecx]
0053806a 6a02                 push 2
0053806c ff5010               call dword ptr [eax + 0x10]
0053806f 8b8e64010000         mov ecx, dword ptr [esi + 0x164]
00538075 8b865c010000         mov eax, dword ptr [esi + 0x15c]
0053807b 8b0481               mov eax, dword ptr [ecx + eax*4]
0053807e 8d4804               lea ecx, [eax + 4]
00538081 8b01                 mov eax, dword ptr [ecx]
00538083 6a03                 push 3
00538085 ff5010               call dword ptr [eax + 0x10]
00538088 8b8e64010000         mov ecx, dword ptr [esi + 0x164]
0053808e 8b865c010000         mov eax, dword ptr [esi + 0x15c]
00538094 8b0481               mov eax, dword ptr [ecx + eax*4]
00538097 8d4804               lea ecx, [eax + 4]
0053809a 8b01                 mov eax, dword ptr [ecx]
0053809c 6a04                 push 4
0053809e ff5010               call dword ptr [eax + 0x10]
005380a1 8b8e64010000         mov ecx, dword ptr [esi + 0x164]
005380a7 8b865c010000         mov eax, dword ptr [esi + 0x15c]
005380ad 8b0481               mov eax, dword ptr [ecx + eax*4]
005380b0 8d4804               lea ecx, [eax + 4]
005380b3 8b01                 mov eax, dword ptr [ecx]
005380b5 6a05                 push 5
005380b7 ff5010               call dword ptr [eax + 0x10]
005380ba 8b865c010000         mov eax, dword ptr [esi + 0x15c]
005380c0 8b8e64010000         mov ecx, dword ptr [esi + 0x164]
005380c6 8b0c81               mov ecx, dword ptr [ecx + eax*4]
005380c9 8b01                 mov eax, dword ptr [ecx]
005380cb 6a04                 push 4
005380cd ff90c0000000         call dword ptr [eax + 0xc0]
005380d3 6a0a                 push 0xa
005380d5 5f                   pop edi
005380d6 8b865c010000         mov eax, dword ptr [esi + 0x15c]
005380dc d905a0052301         fld dword ptr [0x12305a0]
005380e2 8b8e64010000         mov ecx, dword ptr [esi + 0x164]
005380e8 8b0c81               mov ecx, dword ptr [ecx + eax*4]
005380eb 8b01                 mov eax, dword ptr [ecx]
005380ed 51                   push ecx
005380ee d91c24               fstp dword ptr [esp]
005380f1 ff5004               call dword ptr [eax + 4]
005380f4 4f                   dec edi
005380f5 75df                 jne 0x5380d6
005380f7 8b06                 mov eax, dword ptr [esi]
005380f9 53                   push ebx
005380fa 8bce                 mov ecx, esi
005380fc ff9084020000         call dword ptr [eax + 0x284]
00538102 8b865c010000         mov eax, dword ptr [esi + 0x15c]
00538108 8b8e64010000         mov ecx, dword ptr [esi + 0x164]
0053810e 8b0c81               mov ecx, dword ptr [ecx + eax*4]
00538111 8b01                 mov eax, dword ptr [ecx]
00538113 6a03                 push 3
00538115 ff90c0000000         call dword ptr [eax + 0xc0]
0053811b 6a0a                 push 0xa
0053811d 5f                   pop edi
0053811e 8b865c010000         mov eax, dword ptr [esi + 0x15c]
00538124 d905a0052301         fld dword ptr [0x12305a0]
0053812a 8b8e64010000         mov ecx, dword ptr [esi + 0x164]
00538130 8b0c81               mov ecx, dword ptr [ecx + eax*4]
00538133 8b01                 mov eax, dword ptr [ecx]
00538135 51                   push ecx
00538136 d91c24               fstp dword ptr [esp]
00538139 ff5004               call dword ptr [eax + 4]
0053813c 4f                   dec edi
0053813d 75df                 jne 0x53811e
0053813f 8b8e64010000         mov ecx, dword ptr [esi + 0x164]
00538145 8b865c010000         mov eax, dword ptr [esi + 0x15c]
0053814b 8b0481               mov eax, dword ptr [ecx + eax*4]
0053814e 8d4804               lea ecx, [eax + 4]
00538151 8b01                 mov eax, dword ptr [ecx]
00538153 6a02                 push 2
00538155 ff500c               call dword ptr [eax + 0xc]
00538158 8b8e64010000         mov ecx, dword ptr [esi + 0x164]
0053815e 8b865c010000         mov eax, dword ptr [esi + 0x15c]
00538164 8b0481               mov eax, dword ptr [ecx + eax*4]
00538167 8d4804               lea ecx, [eax + 4]
0053816a 8b01                 mov eax, dword ptr [ecx]
0053816c 6a03                 push 3
0053816e ff500c               call dword ptr [eax + 0xc]
00538171 8b8e64010000         mov ecx, dword ptr [esi + 0x164]
00538177 8b865c010000         mov eax, dword ptr [esi + 0x15c]
0053817d 8b0481               mov eax, dword ptr [ecx + eax*4]
00538180 8d4804               lea ecx, [eax + 4]
00538183 8b01                 mov eax, dword ptr [ecx]
00538185 6a04                 push 4
00538187 ff500c               call dword ptr [eax + 0xc]
0053818a 8b8e64010000         mov ecx, dword ptr [esi + 0x164]
00538190 8b865c010000         mov eax, dword ptr [esi + 0x15c]
00538196 8b0481               mov eax, dword ptr [ecx + eax*4]
00538199 8d4804               lea ecx, [eax + 4]
0053819c 8b01                 mov eax, dword ptr [ecx]
0053819e 6a05                 push 5
005381a0 ff500c               call dword ptr [eax + 0xc]
005381a3 8b8674010000         mov eax, dword ptr [esi + 0x174]
005381a9 2b8670010000         sub eax, dword ptr [esi + 0x170]
005381af 33ff                 xor edi, edi
005381b1 c1f802               sar eax, 2
005381b4 746d                 je 0x538223
005381b6 8b8670010000         mov eax, dword ptr [esi + 0x170]
005381bc 8b965c010000         mov edx, dword ptr [esi + 0x15c]
005381c2 8b0cb8               mov ecx, dword ptr [eax + edi*4]
005381c5 8b01                 mov eax, dword ptr [ecx]
005381c7 83c208               add edx, 8
005381ca 52                   push edx
005381cb ff90b0000000         call dword ptr [eax + 0xb0]
005381d1 84c0                 test al, al
005381d3 751f                 jne 0x5381f4
005381d5 8b8670010000         mov eax, dword ptr [esi + 0x170]
005381db 8b965c010000         mov edx, dword ptr [esi + 0x15c]
005381e1 8b0cb8               mov ecx, dword ptr [eax + edi*4]
005381e4 8b01                 mov eax, dword ptr [ecx]
005381e6 83c208               add edx, 8
005381e9 52                   push edx
005381ea ff90b4000000         call dword ptr [eax + 0xb4]
005381f0 84c0                 test al, al
005381f2 741b                 je 0x53820f
005381f4 8b8670010000         mov eax, dword ptr [esi + 0x170]
005381fa 8b965c010000         mov edx, dword ptr [esi + 0x15c]
00538200 8b0cb8               mov ecx, dword ptr [eax + edi*4]
00538203 8b01                 mov eax, dword ptr [ecx]
00538205 83c208               add edx, 8
00538208 52                   push edx
00538209 ff90c0000000         call dword ptr [eax + 0xc0]
0053820f 8b8674010000         mov eax, dword ptr [esi + 0x174]
00538215 2b8670010000         sub eax, dword ptr [esi + 0x170]
0053821b 47                   inc edi
0053821c c1f802               sar eax, 2
0053821f 3bf8                 cmp edi, eax
00538221 7293                 jb 0x5381b6
00538223 5f                   pop edi
00538224 5e                   pop esi
00538225 5b                   pop ebx
00538226 83c578               add ebp, 0x78
00538229 c9                   leave 
0053822a c3                   ret 
