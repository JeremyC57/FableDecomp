00b068f0 53                   push ebx
00b068f1 55                   push ebp
00b068f2 56                   push esi
00b068f3 8b742410             mov esi, dword ptr [esp + 0x10]
00b068f7 8b4604               mov eax, dword ptr [esi + 4]
00b068fa 57                   push edi
00b068fb 8bf9                 mov edi, ecx
00b068fd 8d4804               lea ecx, [eax + 4]
00b06900 33c0                 xor eax, eax
00b06902 7c44                 jl 0xb06948
00b06904 7f04                 jg 0xb0690a
00b06906 85c9                 test ecx, ecx
00b06908 723e                 jb 0xb06948
00b0690a 85c0                 test eax, eax
00b0690c 7f3a                 jg 0xb06948
00b0690e 7c08                 jl 0xb06918
00b06910 81f9ffffff7f         cmp ecx, 0x7fffffff
00b06916 7730                 ja 0xb06948
00b06918 8b5614               mov edx, dword ptr [esi + 0x14]
00b0691b 83fa04               cmp edx, 4
00b0691e 7c1a                 jl 0xb0693a
00b06920 8b460c               mov eax, dword ptr [esi + 0xc]
00b06923 8b18                 mov ebx, dword ptr [eax]
00b06925 83c004               add eax, 4
00b06928 83c2fc               add edx, -4
00b0692b 895c2414             mov dword ptr [esp + 0x14], ebx
00b0692f 89460c               mov dword ptr [esi + 0xc], eax
00b06932 895614               mov dword ptr [esi + 0x14], edx
00b06935 894e04               mov dword ptr [esi + 4], ecx
00b06938 eb0e                 jmp 0xb06948
00b0693a 6a04                 push 4
00b0693c 8d442418             lea eax, [esp + 0x18]
00b06940 50                   push eax
00b06941 8bce                 mov ecx, esi
00b06943 e858d3e8ff           call 0x993ca0
00b06948 8b4c2414             mov ecx, dword ptr [esp + 0x14]
00b0694c 8d542414             lea edx, [esp + 0x14]
00b06950 894f34               mov dword ptr [edi + 0x34], ecx
00b06953 52                   push edx
00b06954 8bce                 mov ecx, esi
00b06956 e885dce8ff           call 0x9945e0
00b0695b 8b00                 mov eax, dword ptr [eax]
00b0695d 8d4c2414             lea ecx, [esp + 0x14]
00b06961 51                   push ecx
00b06962 8bce                 mov ecx, esi
00b06964 894738               mov dword ptr [edi + 0x38], eax
00b06967 e874dce8ff           call 0x9945e0
00b0696c 8b10                 mov edx, dword ptr [eax]
00b0696e 8d442414             lea eax, [esp + 0x14]
00b06972 50                   push eax
00b06973 8bce                 mov ecx, esi
00b06975 89573c               mov dword ptr [edi + 0x3c], edx
00b06978 e863dce8ff           call 0x9945e0
00b0697d 8b08                 mov ecx, dword ptr [eax]
00b0697f 894f40               mov dword ptr [edi + 0x40], ecx
00b06982 8b4604               mov eax, dword ptr [esi + 4]
00b06985 8d4804               lea ecx, [eax + 4]
00b06988 33c0                 xor eax, eax
00b0698a 7c44                 jl 0xb069d0
00b0698c 7f04                 jg 0xb06992
00b0698e 85c9                 test ecx, ecx
00b06990 723e                 jb 0xb069d0
00b06992 85c0                 test eax, eax
00b06994 7f3a                 jg 0xb069d0
00b06996 7c08                 jl 0xb069a0
00b06998 81f9ffffff7f         cmp ecx, 0x7fffffff
00b0699e 7730                 ja 0xb069d0
00b069a0 8b5614               mov edx, dword ptr [esi + 0x14]
00b069a3 83fa04               cmp edx, 4
00b069a6 7c1a                 jl 0xb069c2
00b069a8 8b460c               mov eax, dword ptr [esi + 0xc]
00b069ab 8b18                 mov ebx, dword ptr [eax]
00b069ad 83c004               add eax, 4
00b069b0 83c2fc               add edx, -4
00b069b3 895c2414             mov dword ptr [esp + 0x14], ebx
00b069b7 89460c               mov dword ptr [esi + 0xc], eax
00b069ba 895614               mov dword ptr [esi + 0x14], edx
00b069bd 894e04               mov dword ptr [esi + 4], ecx
00b069c0 eb0e                 jmp 0xb069d0
00b069c2 6a04                 push 4
00b069c4 8d542418             lea edx, [esp + 0x18]
00b069c8 52                   push edx
00b069c9 8bce                 mov ecx, esi
00b069cb e8d0d2e8ff           call 0x993ca0
00b069d0 8b442414             mov eax, dword ptr [esp + 0x14]
00b069d4 894744               mov dword ptr [edi + 0x44], eax
00b069d7 8b4604               mov eax, dword ptr [esi + 4]
00b069da 8d4804               lea ecx, [eax + 4]
00b069dd 33c0                 xor eax, eax
00b069df 7c44                 jl 0xb06a25
00b069e1 7f04                 jg 0xb069e7
00b069e3 85c9                 test ecx, ecx
00b069e5 723e                 jb 0xb06a25
00b069e7 85c0                 test eax, eax
00b069e9 7f3a                 jg 0xb06a25
00b069eb 7c08                 jl 0xb069f5
00b069ed 81f9ffffff7f         cmp ecx, 0x7fffffff
00b069f3 7730                 ja 0xb06a25
00b069f5 8b5614               mov edx, dword ptr [esi + 0x14]
00b069f8 83fa04               cmp edx, 4
00b069fb 7c1a                 jl 0xb06a17
00b069fd 8b460c               mov eax, dword ptr [esi + 0xc]
00b06a00 8b18                 mov ebx, dword ptr [eax]
00b06a02 83c004               add eax, 4
00b06a05 83c2fc               add edx, -4
00b06a08 895c2414             mov dword ptr [esp + 0x14], ebx
00b06a0c 89460c               mov dword ptr [esi + 0xc], eax
00b06a0f 895614               mov dword ptr [esi + 0x14], edx
00b06a12 894e04               mov dword ptr [esi + 4], ecx
00b06a15 eb0e                 jmp 0xb06a25
00b06a17 6a04                 push 4
00b06a19 8d4c2418             lea ecx, [esp + 0x18]
00b06a1d 51                   push ecx
00b06a1e 8bce                 mov ecx, esi
00b06a20 e87bd2e8ff           call 0x993ca0
00b06a25 8b542414             mov edx, dword ptr [esp + 0x14]
00b06a29 895748               mov dword ptr [edi + 0x48], edx
00b06a2c 8b4604               mov eax, dword ptr [esi + 4]
00b06a2f 8d4804               lea ecx, [eax + 4]
00b06a32 33c0                 xor eax, eax
00b06a34 7c44                 jl 0xb06a7a
00b06a36 7f04                 jg 0xb06a3c
00b06a38 85c9                 test ecx, ecx
00b06a3a 723e                 jb 0xb06a7a
00b06a3c 85c0                 test eax, eax
00b06a3e 7f3a                 jg 0xb06a7a
00b06a40 7c08                 jl 0xb06a4a
00b06a42 81f9ffffff7f         cmp ecx, 0x7fffffff
00b06a48 7730                 ja 0xb06a7a
00b06a4a 8b5614               mov edx, dword ptr [esi + 0x14]
00b06a4d 83fa04               cmp edx, 4
00b06a50 7c1a                 jl 0xb06a6c
00b06a52 8b460c               mov eax, dword ptr [esi + 0xc]
00b06a55 8b18                 mov ebx, dword ptr [eax]
00b06a57 83c004               add eax, 4
00b06a5a 83c2fc               add edx, -4
00b06a5d 895c2414             mov dword ptr [esp + 0x14], ebx
00b06a61 89460c               mov dword ptr [esi + 0xc], eax
00b06a64 895614               mov dword ptr [esi + 0x14], edx
00b06a67 894e04               mov dword ptr [esi + 4], ecx
00b06a6a eb0e                 jmp 0xb06a7a
00b06a6c 6a04                 push 4
00b06a6e 8d442418             lea eax, [esp + 0x18]
00b06a72 50                   push eax
00b06a73 8bce                 mov ecx, esi
00b06a75 e826d2e8ff           call 0x993ca0
00b06a7a 8b4c2414             mov ecx, dword ptr [esp + 0x14]
00b06a7e 894f4c               mov dword ptr [edi + 0x4c], ecx
00b06a81 8b4604               mov eax, dword ptr [esi + 4]
00b06a84 8d4804               lea ecx, [eax + 4]
00b06a87 33c0                 xor eax, eax
00b06a89 7c44                 jl 0xb06acf
00b06a8b 7f04                 jg 0xb06a91
00b06a8d 85c9                 test ecx, ecx
00b06a8f 723e                 jb 0xb06acf
00b06a91 85c0                 test eax, eax
00b06a93 7f3a                 jg 0xb06acf
00b06a95 7c08                 jl 0xb06a9f
00b06a97 81f9ffffff7f         cmp ecx, 0x7fffffff
00b06a9d 7730                 ja 0xb06acf
00b06a9f 8b5614               mov edx, dword ptr [esi + 0x14]
00b06aa2 83fa04               cmp edx, 4
00b06aa5 7c1a                 jl 0xb06ac1
00b06aa7 8b460c               mov eax, dword ptr [esi + 0xc]
00b06aaa 8b18                 mov ebx, dword ptr [eax]
00b06aac 83c004               add eax, 4
00b06aaf 83c2fc               add edx, -4
00b06ab2 895c2414             mov dword ptr [esp + 0x14], ebx
00b06ab6 89460c               mov dword ptr [esi + 0xc], eax
00b06ab9 895614               mov dword ptr [esi + 0x14], edx
00b06abc 894e04               mov dword ptr [esi + 4], ecx
00b06abf eb0e                 jmp 0xb06acf
00b06ac1 6a04                 push 4
00b06ac3 8d542418             lea edx, [esp + 0x18]
00b06ac7 52                   push edx
00b06ac8 8bce                 mov ecx, esi
00b06aca e8d1d1e8ff           call 0x993ca0
00b06acf 8b442414             mov eax, dword ptr [esp + 0x14]
00b06ad3 894750               mov dword ptr [edi + 0x50], eax
00b06ad6 8b4604               mov eax, dword ptr [esi + 4]
00b06ad9 8d4804               lea ecx, [eax + 4]
00b06adc 33c0                 xor eax, eax
00b06ade 7c44                 jl 0xb06b24
00b06ae0 7f04                 jg 0xb06ae6
00b06ae2 85c9                 test ecx, ecx
00b06ae4 723e                 jb 0xb06b24
00b06ae6 85c0                 test eax, eax
00b06ae8 7f3a                 jg 0xb06b24
00b06aea 7c08                 jl 0xb06af4
00b06aec 81f9ffffff7f         cmp ecx, 0x7fffffff
00b06af2 7730                 ja 0xb06b24
00b06af4 8b5614               mov edx, dword ptr [esi + 0x14]
00b06af7 83fa04               cmp edx, 4
00b06afa 7c1a                 jl 0xb06b16
00b06afc 8b460c               mov eax, dword ptr [esi + 0xc]
00b06aff 8b18                 mov ebx, dword ptr [eax]
00b06b01 83c004               add eax, 4
00b06b04 83c2fc               add edx, -4
00b06b07 895c2414             mov dword ptr [esp + 0x14], ebx
00b06b0b 89460c               mov dword ptr [esi + 0xc], eax
00b06b0e 895614               mov dword ptr [esi + 0x14], edx
00b06b11 894e04               mov dword ptr [esi + 4], ecx
00b06b14 eb0e                 jmp 0xb06b24
00b06b16 6a04                 push 4
00b06b18 8d4c2418             lea ecx, [esp + 0x18]
00b06b1c 51                   push ecx
00b06b1d 8bce                 mov ecx, esi
00b06b1f e87cd1e8ff           call 0x993ca0
00b06b24 8b542414             mov edx, dword ptr [esp + 0x14]
00b06b28 895754               mov dword ptr [edi + 0x54], edx
00b06b2b 8b4604               mov eax, dword ptr [esi + 4]
00b06b2e 8d4804               lea ecx, [eax + 4]
00b06b31 33c0                 xor eax, eax
00b06b33 7c44                 jl 0xb06b79
00b06b35 7f04                 jg 0xb06b3b
00b06b37 85c9                 test ecx, ecx
00b06b39 723e                 jb 0xb06b79
00b06b3b 85c0                 test eax, eax
00b06b3d 7f3a                 jg 0xb06b79
00b06b3f 7c08                 jl 0xb06b49
00b06b41 81f9ffffff7f         cmp ecx, 0x7fffffff
00b06b47 7730                 ja 0xb06b79
00b06b49 8b5614               mov edx, dword ptr [esi + 0x14]
00b06b4c 83fa04               cmp edx, 4
00b06b4f 7c1a                 jl 0xb06b6b
00b06b51 8b460c               mov eax, dword ptr [esi + 0xc]
00b06b54 8b18                 mov ebx, dword ptr [eax]
00b06b56 83c004               add eax, 4
00b06b59 83c2fc               add edx, -4
00b06b5c 895c2414             mov dword ptr [esp + 0x14], ebx
00b06b60 89460c               mov dword ptr [esi + 0xc], eax
00b06b63 895614               mov dword ptr [esi + 0x14], edx
00b06b66 894e04               mov dword ptr [esi + 4], ecx
00b06b69 eb0e                 jmp 0xb06b79
00b06b6b 6a04                 push 4
00b06b6d 8d442418             lea eax, [esp + 0x18]
00b06b71 50                   push eax
00b06b72 8bce                 mov ecx, esi
00b06b74 e827d1e8ff           call 0x993ca0
00b06b79 8b4c2414             mov ecx, dword ptr [esp + 0x14]
00b06b7d 894f58               mov dword ptr [edi + 0x58], ecx
00b06b80 8b4604               mov eax, dword ptr [esi + 4]
00b06b83 8d4804               lea ecx, [eax + 4]
00b06b86 33c0                 xor eax, eax
00b06b88 7c44                 jl 0xb06bce
00b06b8a 7f04                 jg 0xb06b90
00b06b8c 85c9                 test ecx, ecx
00b06b8e 723e                 jb 0xb06bce
00b06b90 85c0                 test eax, eax
00b06b92 7f3a                 jg 0xb06bce
00b06b94 7c08                 jl 0xb06b9e
00b06b96 81f9ffffff7f         cmp ecx, 0x7fffffff
00b06b9c 7730                 ja 0xb06bce
00b06b9e 8b5614               mov edx, dword ptr [esi + 0x14]
00b06ba1 83fa04               cmp edx, 4
00b06ba4 7c1a                 jl 0xb06bc0
00b06ba6 8b460c               mov eax, dword ptr [esi + 0xc]
00b06ba9 8b18                 mov ebx, dword ptr [eax]
00b06bab 83c004               add eax, 4
00b06bae 83c2fc               add edx, -4
00b06bb1 895c2414             mov dword ptr [esp + 0x14], ebx
00b06bb5 89460c               mov dword ptr [esi + 0xc], eax
00b06bb8 895614               mov dword ptr [esi + 0x14], edx
00b06bbb 894e04               mov dword ptr [esi + 4], ecx
00b06bbe eb0e                 jmp 0xb06bce
00b06bc0 6a04                 push 4
00b06bc2 8d542418             lea edx, [esp + 0x18]
00b06bc6 52                   push edx
00b06bc7 8bce                 mov ecx, esi
00b06bc9 e8d2d0e8ff           call 0x993ca0
00b06bce 8b442414             mov eax, dword ptr [esp + 0x14]
00b06bd2 89475c               mov dword ptr [edi + 0x5c], eax
00b06bd5 8b4604               mov eax, dword ptr [esi + 4]
00b06bd8 8d4804               lea ecx, [eax + 4]
00b06bdb 33c0                 xor eax, eax
00b06bdd 7c44                 jl 0xb06c23
00b06bdf 7f04                 jg 0xb06be5
00b06be1 85c9                 test ecx, ecx
00b06be3 723e                 jb 0xb06c23
00b06be5 85c0                 test eax, eax
00b06be7 7f3a                 jg 0xb06c23
00b06be9 7c08                 jl 0xb06bf3
00b06beb 81f9ffffff7f         cmp ecx, 0x7fffffff
00b06bf1 7730                 ja 0xb06c23
00b06bf3 8b5614               mov edx, dword ptr [esi + 0x14]
00b06bf6 83fa04               cmp edx, 4
00b06bf9 7c1a                 jl 0xb06c15
00b06bfb 8b460c               mov eax, dword ptr [esi + 0xc]
00b06bfe 8b18                 mov ebx, dword ptr [eax]
00b06c00 83c004               add eax, 4
00b06c03 83c2fc               add edx, -4
00b06c06 895c2414             mov dword ptr [esp + 0x14], ebx
00b06c0a 89460c               mov dword ptr [esi + 0xc], eax
00b06c0d 895614               mov dword ptr [esi + 0x14], edx
00b06c10 894e04               mov dword ptr [esi + 4], ecx
00b06c13 eb0e                 jmp 0xb06c23
00b06c15 6a04                 push 4
00b06c17 8d4c2418             lea ecx, [esp + 0x18]
00b06c1b 51                   push ecx
00b06c1c 8bce                 mov ecx, esi
00b06c1e e87dd0e8ff           call 0x993ca0
00b06c23 8b542414             mov edx, dword ptr [esp + 0x14]
00b06c27 895760               mov dword ptr [edi + 0x60], edx
00b06c2a 8b4604               mov eax, dword ptr [esi + 4]
00b06c2d 8d4804               lea ecx, [eax + 4]
00b06c30 33c0                 xor eax, eax
00b06c32 7c44                 jl 0xb06c78
00b06c34 7f04                 jg 0xb06c3a
00b06c36 85c9                 test ecx, ecx
00b06c38 723e                 jb 0xb06c78
00b06c3a 85c0                 test eax, eax
00b06c3c 7f3a                 jg 0xb06c78
00b06c3e 7c08                 jl 0xb06c48
00b06c40 81f9ffffff7f         cmp ecx, 0x7fffffff
00b06c46 7730                 ja 0xb06c78
00b06c48 8b5614               mov edx, dword ptr [esi + 0x14]
00b06c4b 83fa04               cmp edx, 4
00b06c4e 7c1a                 jl 0xb06c6a
00b06c50 8b460c               mov eax, dword ptr [esi + 0xc]
00b06c53 8b18                 mov ebx, dword ptr [eax]
00b06c55 83c004               add eax, 4
00b06c58 83c2fc               add edx, -4
00b06c5b 895c2414             mov dword ptr [esp + 0x14], ebx
00b06c5f 89460c               mov dword ptr [esi + 0xc], eax
00b06c62 895614               mov dword ptr [esi + 0x14], edx
00b06c65 894e04               mov dword ptr [esi + 4], ecx
00b06c68 eb0e                 jmp 0xb06c78
00b06c6a 6a04                 push 4
00b06c6c 8d442418             lea eax, [esp + 0x18]
00b06c70 50                   push eax
00b06c71 8bce                 mov ecx, esi
00b06c73 e828d0e8ff           call 0x993ca0
00b06c78 8b4c2414             mov ecx, dword ptr [esp + 0x14]
00b06c7c 894f64               mov dword ptr [edi + 0x64], ecx
00b06c7f 8b4604               mov eax, dword ptr [esi + 4]
00b06c82 8d4804               lea ecx, [eax + 4]
00b06c85 33c0                 xor eax, eax
00b06c87 7c44                 jl 0xb06ccd
00b06c89 7f04                 jg 0xb06c8f
00b06c8b 85c9                 test ecx, ecx
00b06c8d 723e                 jb 0xb06ccd
00b06c8f 85c0                 test eax, eax
00b06c91 7f3a                 jg 0xb06ccd
00b06c93 7c08                 jl 0xb06c9d
00b06c95 81f9ffffff7f         cmp ecx, 0x7fffffff
00b06c9b 7730                 ja 0xb06ccd
00b06c9d 8b5614               mov edx, dword ptr [esi + 0x14]
00b06ca0 83fa04               cmp edx, 4
00b06ca3 7c1a                 jl 0xb06cbf
00b06ca5 8b460c               mov eax, dword ptr [esi + 0xc]
00b06ca8 8b18                 mov ebx, dword ptr [eax]
00b06caa 83c004               add eax, 4
00b06cad 83c2fc               add edx, -4
00b06cb0 895c2414             mov dword ptr [esp + 0x14], ebx
00b06cb4 89460c               mov dword ptr [esi + 0xc], eax
00b06cb7 895614               mov dword ptr [esi + 0x14], edx
00b06cba 894e04               mov dword ptr [esi + 4], ecx
00b06cbd eb0e                 jmp 0xb06ccd
00b06cbf 6a04                 push 4
00b06cc1 8d542418             lea edx, [esp + 0x18]
00b06cc5 52                   push edx
00b06cc6 8bce                 mov ecx, esi
00b06cc8 e8d3cfe8ff           call 0x993ca0
00b06ccd 8b442414             mov eax, dword ptr [esp + 0x14]
00b06cd1 894768               mov dword ptr [edi + 0x68], eax
00b06cd4 8b4604               mov eax, dword ptr [esi + 4]
00b06cd7 8d4804               lea ecx, [eax + 4]
00b06cda 33c0                 xor eax, eax
00b06cdc 7c44                 jl 0xb06d22
00b06cde 7f04                 jg 0xb06ce4
00b06ce0 85c9                 test ecx, ecx
00b06ce2 723e                 jb 0xb06d22
00b06ce4 85c0                 test eax, eax
00b06ce6 7f3a                 jg 0xb06d22
00b06ce8 7c08                 jl 0xb06cf2
00b06cea 81f9ffffff7f         cmp ecx, 0x7fffffff
00b06cf0 7730                 ja 0xb06d22
00b06cf2 8b5614               mov edx, dword ptr [esi + 0x14]
00b06cf5 83fa04               cmp edx, 4
00b06cf8 7c1a                 jl 0xb06d14
00b06cfa 8b460c               mov eax, dword ptr [esi + 0xc]
00b06cfd 8b18                 mov ebx, dword ptr [eax]
00b06cff 83c004               add eax, 4
00b06d02 83c2fc               add edx, -4
00b06d05 895c2414             mov dword ptr [esp + 0x14], ebx
00b06d09 89460c               mov dword ptr [esi + 0xc], eax
00b06d0c 895614               mov dword ptr [esi + 0x14], edx
00b06d0f 894e04               mov dword ptr [esi + 4], ecx
00b06d12 eb0e                 jmp 0xb06d22
00b06d14 6a04                 push 4
00b06d16 8d4c2418             lea ecx, [esp + 0x18]
00b06d1a 51                   push ecx
00b06d1b 8bce                 mov ecx, esi
00b06d1d e87ecfe8ff           call 0x993ca0
00b06d22 8b542414             mov edx, dword ptr [esp + 0x14]
00b06d26 89576c               mov dword ptr [edi + 0x6c], edx
00b06d29 8b4604               mov eax, dword ptr [esi + 4]
00b06d2c 8d4804               lea ecx, [eax + 4]
00b06d2f 33c0                 xor eax, eax
00b06d31 7c44                 jl 0xb06d77
00b06d33 7f04                 jg 0xb06d39
00b06d35 85c9                 test ecx, ecx
00b06d37 723e                 jb 0xb06d77
00b06d39 85c0                 test eax, eax
00b06d3b 7f3a                 jg 0xb06d77
00b06d3d 7c08                 jl 0xb06d47
00b06d3f 81f9ffffff7f         cmp ecx, 0x7fffffff
00b06d45 7730                 ja 0xb06d77
00b06d47 8b5614               mov edx, dword ptr [esi + 0x14]
00b06d4a 83fa04               cmp edx, 4
00b06d4d 7c1a                 jl 0xb06d69
00b06d4f 8b460c               mov eax, dword ptr [esi + 0xc]
00b06d52 8b18                 mov ebx, dword ptr [eax]
00b06d54 83c004               add eax, 4
00b06d57 83c2fc               add edx, -4
00b06d5a 895c2414             mov dword ptr [esp + 0x14], ebx
00b06d5e 89460c               mov dword ptr [esi + 0xc], eax
00b06d61 895614               mov dword ptr [esi + 0x14], edx
00b06d64 894e04               mov dword ptr [esi + 4], ecx
00b06d67 eb0e                 jmp 0xb06d77
00b06d69 6a04                 push 4
00b06d6b 8d442418             lea eax, [esp + 0x18]
00b06d6f 50                   push eax
00b06d70 8bce                 mov ecx, esi
00b06d72 e829cfe8ff           call 0x993ca0
00b06d77 8b4c2414             mov ecx, dword ptr [esp + 0x14]
00b06d7b 894f70               mov dword ptr [edi + 0x70], ecx
00b06d7e 8b4604               mov eax, dword ptr [esi + 4]
00b06d81 8d4804               lea ecx, [eax + 4]
00b06d84 33c0                 xor eax, eax
00b06d86 7c44                 jl 0xb06dcc
00b06d88 7f04                 jg 0xb06d8e
00b06d8a 85c9                 test ecx, ecx
00b06d8c 723e                 jb 0xb06dcc
00b06d8e 85c0                 test eax, eax
00b06d90 7f3a                 jg 0xb06dcc
00b06d92 7c08                 jl 0xb06d9c
00b06d94 81f9ffffff7f         cmp ecx, 0x7fffffff
00b06d9a 7730                 ja 0xb06dcc
00b06d9c 8b5614               mov edx, dword ptr [esi + 0x14]
00b06d9f 83fa04               cmp edx, 4
00b06da2 7c1a                 jl 0xb06dbe
00b06da4 8b460c               mov eax, dword ptr [esi + 0xc]
00b06da7 8b18                 mov ebx, dword ptr [eax]
00b06da9 83c004               add eax, 4
00b06dac 83c2fc               add edx, -4
00b06daf 895c2414             mov dword ptr [esp + 0x14], ebx
00b06db3 89460c               mov dword ptr [esi + 0xc], eax
00b06db6 895614               mov dword ptr [esi + 0x14], edx
00b06db9 894e04               mov dword ptr [esi + 4], ecx
00b06dbc eb0e                 jmp 0xb06dcc
00b06dbe 6a04                 push 4
00b06dc0 8d542418             lea edx, [esp + 0x18]
00b06dc4 52                   push edx
00b06dc5 8bce                 mov ecx, esi
00b06dc7 e8d4cee8ff           call 0x993ca0
00b06dcc 8b442414             mov eax, dword ptr [esp + 0x14]
00b06dd0 894774               mov dword ptr [edi + 0x74], eax
00b06dd3 8b4604               mov eax, dword ptr [esi + 4]
00b06dd6 8d4804               lea ecx, [eax + 4]
00b06dd9 33c0                 xor eax, eax
00b06ddb 7c44                 jl 0xb06e21
00b06ddd 7f04                 jg 0xb06de3
00b06ddf 85c9                 test ecx, ecx
00b06de1 723e                 jb 0xb06e21
00b06de3 85c0                 test eax, eax
00b06de5 7f3a                 jg 0xb06e21
00b06de7 7c08                 jl 0xb06df1
00b06de9 81f9ffffff7f         cmp ecx, 0x7fffffff
00b06def 7730                 ja 0xb06e21
00b06df1 8b5614               mov edx, dword ptr [esi + 0x14]
00b06df4 83fa04               cmp edx, 4
00b06df7 7c1a                 jl 0xb06e13
00b06df9 8b460c               mov eax, dword ptr [esi + 0xc]
00b06dfc 8b18                 mov ebx, dword ptr [eax]
00b06dfe 83c004               add eax, 4
00b06e01 83c2fc               add edx, -4
00b06e04 895c2414             mov dword ptr [esp + 0x14], ebx
00b06e08 89460c               mov dword ptr [esi + 0xc], eax
00b06e0b 895614               mov dword ptr [esi + 0x14], edx
00b06e0e 894e04               mov dword ptr [esi + 4], ecx
00b06e11 eb0e                 jmp 0xb06e21
00b06e13 6a04                 push 4
00b06e15 8d4c2418             lea ecx, [esp + 0x18]
00b06e19 51                   push ecx
00b06e1a 8bce                 mov ecx, esi
00b06e1c e87fcee8ff           call 0x993ca0
00b06e21 8b542414             mov edx, dword ptr [esp + 0x14]
00b06e25 895778               mov dword ptr [edi + 0x78], edx
00b06e28 8b4604               mov eax, dword ptr [esi + 4]
00b06e2b 8d4804               lea ecx, [eax + 4]
00b06e2e 33c0                 xor eax, eax
00b06e30 7c44                 jl 0xb06e76
00b06e32 7f04                 jg 0xb06e38
00b06e34 85c9                 test ecx, ecx
00b06e36 723e                 jb 0xb06e76
00b06e38 85c0                 test eax, eax
00b06e3a 7f3a                 jg 0xb06e76
00b06e3c 7c08                 jl 0xb06e46
00b06e3e 81f9ffffff7f         cmp ecx, 0x7fffffff
00b06e44 7730                 ja 0xb06e76
00b06e46 8b5614               mov edx, dword ptr [esi + 0x14]
00b06e49 83fa04               cmp edx, 4
00b06e4c 7c1a                 jl 0xb06e68
00b06e4e 8b460c               mov eax, dword ptr [esi + 0xc]
00b06e51 8b18                 mov ebx, dword ptr [eax]
00b06e53 83c004               add eax, 4
00b06e56 83c2fc               add edx, -4
00b06e59 895c2414             mov dword ptr [esp + 0x14], ebx
00b06e5d 89460c               mov dword ptr [esi + 0xc], eax
00b06e60 895614               mov dword ptr [esi + 0x14], edx
00b06e63 894e04               mov dword ptr [esi + 4], ecx
00b06e66 eb0e                 jmp 0xb06e76
00b06e68 6a04                 push 4
00b06e6a 8d442418             lea eax, [esp + 0x18]
00b06e6e 50                   push eax
00b06e6f 8bce                 mov ecx, esi
00b06e71 e82acee8ff           call 0x993ca0
00b06e76 8b4c2414             mov ecx, dword ptr [esp + 0x14]
00b06e7a 894f7c               mov dword ptr [edi + 0x7c], ecx
00b06e7d 8b4604               mov eax, dword ptr [esi + 4]
00b06e80 8d4804               lea ecx, [eax + 4]
00b06e83 33c0                 xor eax, eax
00b06e85 7c44                 jl 0xb06ecb
00b06e87 7f04                 jg 0xb06e8d
00b06e89 85c9                 test ecx, ecx
00b06e8b 723e                 jb 0xb06ecb
00b06e8d 85c0                 test eax, eax
00b06e8f 7f3a                 jg 0xb06ecb
00b06e91 7c08                 jl 0xb06e9b
00b06e93 81f9ffffff7f         cmp ecx, 0x7fffffff
00b06e99 7730                 ja 0xb06ecb
00b06e9b 8b5614               mov edx, dword ptr [esi + 0x14]
00b06e9e 83fa04               cmp edx, 4
00b06ea1 7c1a                 jl 0xb06ebd
00b06ea3 8b460c               mov eax, dword ptr [esi + 0xc]
00b06ea6 8b18                 mov ebx, dword ptr [eax]
00b06ea8 83c004               add eax, 4
00b06eab 83c2fc               add edx, -4
00b06eae 895c2414             mov dword ptr [esp + 0x14], ebx
00b06eb2 89460c               mov dword ptr [esi + 0xc], eax
00b06eb5 895614               mov dword ptr [esi + 0x14], edx
00b06eb8 894e04               mov dword ptr [esi + 4], ecx
00b06ebb eb0e                 jmp 0xb06ecb
00b06ebd 6a04                 push 4
00b06ebf 8d542418             lea edx, [esp + 0x18]
00b06ec3 52                   push edx
00b06ec4 8bce                 mov ecx, esi
00b06ec6 e8d5cde8ff           call 0x993ca0
00b06ecb 8b442414             mov eax, dword ptr [esp + 0x14]
00b06ecf 898780000000         mov dword ptr [edi + 0x80], eax
00b06ed5 8b4604               mov eax, dword ptr [esi + 4]
00b06ed8 8d4804               lea ecx, [eax + 4]
00b06edb 33c0                 xor eax, eax
00b06edd 7c44                 jl 0xb06f23
00b06edf 7f04                 jg 0xb06ee5
00b06ee1 85c9                 test ecx, ecx
00b06ee3 723e                 jb 0xb06f23
00b06ee5 85c0                 test eax, eax
00b06ee7 7f3a                 jg 0xb06f23
00b06ee9 7c08                 jl 0xb06ef3
00b06eeb 81f9ffffff7f         cmp ecx, 0x7fffffff
00b06ef1 7730                 ja 0xb06f23
00b06ef3 8b5614               mov edx, dword ptr [esi + 0x14]
00b06ef6 83fa04               cmp edx, 4
00b06ef9 7c1a                 jl 0xb06f15
00b06efb 8b460c               mov eax, dword ptr [esi + 0xc]
00b06efe 8b18                 mov ebx, dword ptr [eax]
00b06f00 83c004               add eax, 4
00b06f03 83c2fc               add edx, -4
00b06f06 895c2414             mov dword ptr [esp + 0x14], ebx
00b06f0a 89460c               mov dword ptr [esi + 0xc], eax
00b06f0d 895614               mov dword ptr [esi + 0x14], edx
00b06f10 894e04               mov dword ptr [esi + 4], ecx
00b06f13 eb0e                 jmp 0xb06f23
00b06f15 6a04                 push 4
00b06f17 8d4c2418             lea ecx, [esp + 0x18]
00b06f1b 51                   push ecx
00b06f1c 8bce                 mov ecx, esi
00b06f1e e87dcde8ff           call 0x993ca0
00b06f23 8b542414             mov edx, dword ptr [esp + 0x14]
00b06f27 899784000000         mov dword ptr [edi + 0x84], edx
00b06f2d 8b4604               mov eax, dword ptr [esi + 4]
00b06f30 8d4804               lea ecx, [eax + 4]
00b06f33 33c0                 xor eax, eax
00b06f35 7c44                 jl 0xb06f7b
00b06f37 7f04                 jg 0xb06f3d
00b06f39 85c9                 test ecx, ecx
00b06f3b 723e                 jb 0xb06f7b
00b06f3d 85c0                 test eax, eax
00b06f3f 7f3a                 jg 0xb06f7b
00b06f41 7c08                 jl 0xb06f4b
00b06f43 81f9ffffff7f         cmp ecx, 0x7fffffff
00b06f49 7730                 ja 0xb06f7b
00b06f4b 8b5614               mov edx, dword ptr [esi + 0x14]
00b06f4e 83fa04               cmp edx, 4
00b06f51 7c1a                 jl 0xb06f6d
00b06f53 8b460c               mov eax, dword ptr [esi + 0xc]
00b06f56 8b18                 mov ebx, dword ptr [eax]
00b06f58 83c004               add eax, 4
00b06f5b 83c2fc               add edx, -4
00b06f5e 895c2414             mov dword ptr [esp + 0x14], ebx
00b06f62 89460c               mov dword ptr [esi + 0xc], eax
00b06f65 895614               mov dword ptr [esi + 0x14], edx
00b06f68 894e04               mov dword ptr [esi + 4], ecx
00b06f6b eb0e                 jmp 0xb06f7b
00b06f6d 6a04                 push 4
00b06f6f 8d442418             lea eax, [esp + 0x18]
00b06f73 50                   push eax
00b06f74 8bce                 mov ecx, esi
00b06f76 e825cde8ff           call 0x993ca0
00b06f7b 8b4c2414             mov ecx, dword ptr [esp + 0x14]
00b06f7f 898f88000000         mov dword ptr [edi + 0x88], ecx
00b06f85 8b4604               mov eax, dword ptr [esi + 4]
00b06f88 8d4804               lea ecx, [eax + 4]
00b06f8b 33c0                 xor eax, eax
00b06f8d 7c44                 jl 0xb06fd3
00b06f8f 7f04                 jg 0xb06f95
00b06f91 85c9                 test ecx, ecx
00b06f93 723e                 jb 0xb06fd3
00b06f95 85c0                 test eax, eax
00b06f97 7f3a                 jg 0xb06fd3
00b06f99 7c08                 jl 0xb06fa3
00b06f9b 81f9ffffff7f         cmp ecx, 0x7fffffff
00b06fa1 7730                 ja 0xb06fd3
00b06fa3 8b5614               mov edx, dword ptr [esi + 0x14]
00b06fa6 83fa04               cmp edx, 4
00b06fa9 7c1a                 jl 0xb06fc5
00b06fab 8b460c               mov eax, dword ptr [esi + 0xc]
00b06fae 8b18                 mov ebx, dword ptr [eax]
00b06fb0 83c004               add eax, 4
00b06fb3 83c2fc               add edx, -4
00b06fb6 895c2414             mov dword ptr [esp + 0x14], ebx
00b06fba 89460c               mov dword ptr [esi + 0xc], eax
00b06fbd 895614               mov dword ptr [esi + 0x14], edx
00b06fc0 894e04               mov dword ptr [esi + 4], ecx
00b06fc3 eb0e                 jmp 0xb06fd3
00b06fc5 6a04                 push 4
00b06fc7 8d542418             lea edx, [esp + 0x18]
00b06fcb 52                   push edx
00b06fcc 8bce                 mov ecx, esi
00b06fce e8cdcce8ff           call 0x993ca0
00b06fd3 8b442414             mov eax, dword ptr [esp + 0x14]
00b06fd7 89878c000000         mov dword ptr [edi + 0x8c], eax
00b06fdd 8b4604               mov eax, dword ptr [esi + 4]
00b06fe0 8d4801               lea ecx, [eax + 1]
00b06fe3 33c0                 xor eax, eax
00b06fe5 7c40                 jl 0xb07027
00b06fe7 7f04                 jg 0xb06fed
00b06fe9 85c9                 test ecx, ecx
00b06feb 723a                 jb 0xb07027
00b06fed 85c0                 test eax, eax
00b06fef 7f36                 jg 0xb07027
00b06ff1 7c08                 jl 0xb06ffb
00b06ff3 81f9ffffff7f         cmp ecx, 0x7fffffff
00b06ff9 772c                 ja 0xb07027
00b06ffb 8b6e14               mov ebp, dword ptr [esi + 0x14]
00b06ffe 83fd01               cmp ebp, 1
00b07001 7c16                 jl 0xb07019
00b07003 8b460c               mov eax, dword ptr [esi + 0xc]
00b07006 8a10                 mov dl, byte ptr [eax]
00b07008 40                   inc eax
00b07009 4d                   dec ebp
00b0700a 88542414             mov byte ptr [esp + 0x14], dl
00b0700e 89460c               mov dword ptr [esi + 0xc], eax
00b07011 896e14               mov dword ptr [esi + 0x14], ebp
00b07014 894e04               mov dword ptr [esi + 4], ecx
00b07017 eb0e                 jmp 0xb07027
00b07019 6a01                 push 1
00b0701b 8d442418             lea eax, [esp + 0x18]
00b0701f 50                   push eax
00b07020 8bce                 mov ecx, esi
00b07022 e879cce8ff           call 0x993ca0
00b07027 8a5c2414             mov bl, byte ptr [esp + 0x14]
00b0702b 84db                 test bl, bl
00b0702d 0f95c1               setne cl
00b07030 888f90000000         mov byte ptr [edi + 0x90], cl
00b07036 8b4604               mov eax, dword ptr [esi + 4]
00b07039 8d4801               lea ecx, [eax + 1]
00b0703c 33c0                 xor eax, eax
00b0703e 7c40                 jl 0xb07080
00b07040 7f04                 jg 0xb07046
00b07042 85c9                 test ecx, ecx
00b07044 723a                 jb 0xb07080
00b07046 85c0                 test eax, eax
00b07048 7f36                 jg 0xb07080
00b0704a 7c08                 jl 0xb07054
00b0704c 81f9ffffff7f         cmp ecx, 0x7fffffff
00b07052 772c                 ja 0xb07080
00b07054 8b6e14               mov ebp, dword ptr [esi + 0x14]
00b07057 83fd01               cmp ebp, 1
00b0705a 7c16                 jl 0xb07072
00b0705c 8b460c               mov eax, dword ptr [esi + 0xc]
00b0705f 8a10                 mov dl, byte ptr [eax]
00b07061 40                   inc eax
00b07062 4d                   dec ebp
00b07063 88542414             mov byte ptr [esp + 0x14], dl
00b07067 89460c               mov dword ptr [esi + 0xc], eax
00b0706a 896e14               mov dword ptr [esi + 0x14], ebp
00b0706d 894e04               mov dword ptr [esi + 4], ecx
00b07070 eb0e                 jmp 0xb07080
00b07072 6a01                 push 1
00b07074 8d442418             lea eax, [esp + 0x18]
00b07078 50                   push eax
00b07079 8bce                 mov ecx, esi
00b0707b e820cce8ff           call 0x993ca0
00b07080 8a5c2414             mov bl, byte ptr [esp + 0x14]
00b07084 84db                 test bl, bl
00b07086 0f95c1               setne cl
00b07089 888f91000000         mov byte ptr [edi + 0x91], cl
00b0708f 8b4604               mov eax, dword ptr [esi + 4]
00b07092 8d4801               lea ecx, [eax + 1]
00b07095 33c0                 xor eax, eax
00b07097 7c40                 jl 0xb070d9
00b07099 7f04                 jg 0xb0709f
00b0709b 85c9                 test ecx, ecx
00b0709d 723a                 jb 0xb070d9
00b0709f 85c0                 test eax, eax
00b070a1 7f36                 jg 0xb070d9
00b070a3 7c08                 jl 0xb070ad
00b070a5 81f9ffffff7f         cmp ecx, 0x7fffffff
00b070ab 772c                 ja 0xb070d9
00b070ad 8b6e14               mov ebp, dword ptr [esi + 0x14]
00b070b0 83fd01               cmp ebp, 1
00b070b3 7c16                 jl 0xb070cb
00b070b5 8b460c               mov eax, dword ptr [esi + 0xc]
00b070b8 8a10                 mov dl, byte ptr [eax]
00b070ba 40                   inc eax
00b070bb 4d                   dec ebp
00b070bc 88542414             mov byte ptr [esp + 0x14], dl
00b070c0 89460c               mov dword ptr [esi + 0xc], eax
00b070c3 896e14               mov dword ptr [esi + 0x14], ebp
00b070c6 894e04               mov dword ptr [esi + 4], ecx
00b070c9 eb0e                 jmp 0xb070d9
00b070cb 6a01                 push 1
00b070cd 8d442418             lea eax, [esp + 0x18]
00b070d1 50                   push eax
00b070d2 8bce                 mov ecx, esi
00b070d4 e8c7cbe8ff           call 0x993ca0
00b070d9 8a5c2414             mov bl, byte ptr [esp + 0x14]
00b070dd 84db                 test bl, bl
00b070df 0f95c1               setne cl
00b070e2 888f92000000         mov byte ptr [edi + 0x92], cl
00b070e8 8b4604               mov eax, dword ptr [esi + 4]
00b070eb 8d4801               lea ecx, [eax + 1]
00b070ee 33c0                 xor eax, eax
00b070f0 7c40                 jl 0xb07132
00b070f2 7f04                 jg 0xb070f8
00b070f4 85c9                 test ecx, ecx
00b070f6 723a                 jb 0xb07132
00b070f8 85c0                 test eax, eax
00b070fa 7f36                 jg 0xb07132
00b070fc 7c08                 jl 0xb07106
00b070fe 81f9ffffff7f         cmp ecx, 0x7fffffff
00b07104 772c                 ja 0xb07132
00b07106 8b6e14               mov ebp, dword ptr [esi + 0x14]
00b07109 83fd01               cmp ebp, 1
00b0710c 7c16                 jl 0xb07124
00b0710e 8b460c               mov eax, dword ptr [esi + 0xc]
00b07111 8a10                 mov dl, byte ptr [eax]
00b07113 40                   inc eax
00b07114 4d                   dec ebp
00b07115 88542414             mov byte ptr [esp + 0x14], dl
00b07119 89460c               mov dword ptr [esi + 0xc], eax
00b0711c 896e14               mov dword ptr [esi + 0x14], ebp
00b0711f 894e04               mov dword ptr [esi + 4], ecx
00b07122 eb0e                 jmp 0xb07132
00b07124 6a01                 push 1
00b07126 8d442418             lea eax, [esp + 0x18]
00b0712a 50                   push eax
00b0712b 8bce                 mov ecx, esi
00b0712d e86ecbe8ff           call 0x993ca0
00b07132 8a4c2414             mov cl, byte ptr [esp + 0x14]
00b07136 8a8793000000         mov al, byte ptr [edi + 0x93]
00b0713c 84c9                 test cl, cl
00b0713e 0f95c1               setne cl
00b07141 32c8                 xor cl, al
00b07143 80e101               and cl, 1
00b07146 8ad8                 mov bl, al
00b07148 32d9                 xor bl, cl
00b0714a 889f93000000         mov byte ptr [edi + 0x93], bl
00b07150 8b4604               mov eax, dword ptr [esi + 4]
00b07153 8d4801               lea ecx, [eax + 1]
00b07156 33c0                 xor eax, eax
00b07158 7c40                 jl 0xb0719a
00b0715a 7f04                 jg 0xb07160
00b0715c 85c9                 test ecx, ecx
00b0715e 723a                 jb 0xb0719a
00b07160 85c0                 test eax, eax
00b07162 7f36                 jg 0xb0719a
00b07164 7c08                 jl 0xb0716e
00b07166 81f9ffffff7f         cmp ecx, 0x7fffffff
00b0716c 772c                 ja 0xb0719a
00b0716e 8b6e14               mov ebp, dword ptr [esi + 0x14]
00b07171 83fd01               cmp ebp, 1
00b07174 7c16                 jl 0xb0718c
00b07176 8b460c               mov eax, dword ptr [esi + 0xc]
00b07179 8a10                 mov dl, byte ptr [eax]
00b0717b 40                   inc eax
00b0717c 4d                   dec ebp
00b0717d 88542414             mov byte ptr [esp + 0x14], dl
00b07181 89460c               mov dword ptr [esi + 0xc], eax
00b07184 896e14               mov dword ptr [esi + 0x14], ebp
00b07187 894e04               mov dword ptr [esi + 4], ecx
00b0718a eb0e                 jmp 0xb0719a
00b0718c 6a01                 push 1
00b0718e 8d442418             lea eax, [esp + 0x18]
00b07192 50                   push eax
00b07193 8bce                 mov ecx, esi
00b07195 e806cbe8ff           call 0x993ca0
00b0719a 8a5c2414             mov bl, byte ptr [esp + 0x14]
00b0719e 8a8793000000         mov al, byte ptr [edi + 0x93]
00b071a4 84db                 test bl, bl
00b071a6 0f95c1               setne cl
00b071a9 d0e1                 shl cl, 1
00b071ab 32c8                 xor cl, al
00b071ad 80e102               and cl, 2
00b071b0 8ad8                 mov bl, al
00b071b2 32d9                 xor bl, cl
00b071b4 889f93000000         mov byte ptr [edi + 0x93], bl
00b071ba 8b4604               mov eax, dword ptr [esi + 4]
00b071bd 8d4801               lea ecx, [eax + 1]
00b071c0 33c0                 xor eax, eax
00b071c2 7c40                 jl 0xb07204
00b071c4 7f04                 jg 0xb071ca
00b071c6 85c9                 test ecx, ecx
00b071c8 723a                 jb 0xb07204
00b071ca 85c0                 test eax, eax
00b071cc 7f36                 jg 0xb07204
00b071ce 7c08                 jl 0xb071d8
00b071d0 81f9ffffff7f         cmp ecx, 0x7fffffff
00b071d6 772c                 ja 0xb07204
00b071d8 8b6e14               mov ebp, dword ptr [esi + 0x14]
00b071db 83fd01               cmp ebp, 1
00b071de 7c16                 jl 0xb071f6
00b071e0 8b460c               mov eax, dword ptr [esi + 0xc]
00b071e3 8a10                 mov dl, byte ptr [eax]
00b071e5 40                   inc eax
00b071e6 4d                   dec ebp
00b071e7 88542414             mov byte ptr [esp + 0x14], dl
00b071eb 89460c               mov dword ptr [esi + 0xc], eax
00b071ee 896e14               mov dword ptr [esi + 0x14], ebp
00b071f1 894e04               mov dword ptr [esi + 4], ecx
00b071f4 eb0e                 jmp 0xb07204
00b071f6 6a01                 push 1
00b071f8 8d442418             lea eax, [esp + 0x18]
00b071fc 50                   push eax
00b071fd 8bce                 mov ecx, esi
00b071ff e89ccae8ff           call 0x993ca0
00b07204 8a5c2414             mov bl, byte ptr [esp + 0x14]
00b07208 8a8793000000         mov al, byte ptr [edi + 0x93]
00b0720e 84db                 test bl, bl
00b07210 0f95c1               setne cl
00b07213 c0e102               shl cl, 2
00b07216 32c8                 xor cl, al
00b07218 80e104               and cl, 4
00b0721b 8ad8                 mov bl, al
00b0721d 32d9                 xor bl, cl
00b0721f 889f93000000         mov byte ptr [edi + 0x93], bl
00b07225 8b4604               mov eax, dword ptr [esi + 4]
00b07228 8d4801               lea ecx, [eax + 1]
00b0722b 33c0                 xor eax, eax
00b0722d 7c40                 jl 0xb0726f
00b0722f 7f04                 jg 0xb07235
00b07231 85c9                 test ecx, ecx
00b07233 723a                 jb 0xb0726f
00b07235 85c0                 test eax, eax
00b07237 7f36                 jg 0xb0726f
00b07239 7c08                 jl 0xb07243
00b0723b 81f9ffffff7f         cmp ecx, 0x7fffffff
00b07241 772c                 ja 0xb0726f
00b07243 8b6e14               mov ebp, dword ptr [esi + 0x14]
00b07246 83fd01               cmp ebp, 1
00b07249 7c16                 jl 0xb07261
00b0724b 8b460c               mov eax, dword ptr [esi + 0xc]
00b0724e 8a10                 mov dl, byte ptr [eax]
00b07250 40                   inc eax
00b07251 4d                   dec ebp
00b07252 88542414             mov byte ptr [esp + 0x14], dl
00b07256 89460c               mov dword ptr [esi + 0xc], eax
00b07259 896e14               mov dword ptr [esi + 0x14], ebp
00b0725c 894e04               mov dword ptr [esi + 4], ecx
00b0725f eb0e                 jmp 0xb0726f
00b07261 6a01                 push 1
00b07263 8d442418             lea eax, [esp + 0x18]
00b07267 50                   push eax
00b07268 8bce                 mov ecx, esi
00b0726a e831cae8ff           call 0x993ca0
00b0726f 8a5c2414             mov bl, byte ptr [esp + 0x14]
00b07273 8a8793000000         mov al, byte ptr [edi + 0x93]
00b07279 84db                 test bl, bl
00b0727b 0f95c1               setne cl
00b0727e c0e103               shl cl, 3
00b07281 32c8                 xor cl, al
00b07283 80e108               and cl, 8
00b07286 8ad8                 mov bl, al
00b07288 32d9                 xor bl, cl
00b0728a 889f93000000         mov byte ptr [edi + 0x93], bl
00b07290 8b4604               mov eax, dword ptr [esi + 4]
00b07293 8d4801               lea ecx, [eax + 1]
00b07296 33c0                 xor eax, eax
00b07298 7c40                 jl 0xb072da
00b0729a 7f04                 jg 0xb072a0
00b0729c 85c9                 test ecx, ecx
00b0729e 723a                 jb 0xb072da
00b072a0 85c0                 test eax, eax
00b072a2 7f36                 jg 0xb072da
00b072a4 7c08                 jl 0xb072ae
00b072a6 81f9ffffff7f         cmp ecx, 0x7fffffff
00b072ac 772c                 ja 0xb072da
00b072ae 8b6e14               mov ebp, dword ptr [esi + 0x14]
00b072b1 83fd01               cmp ebp, 1
00b072b4 7c16                 jl 0xb072cc
00b072b6 8b460c               mov eax, dword ptr [esi + 0xc]
00b072b9 8a10                 mov dl, byte ptr [eax]
00b072bb 40                   inc eax
00b072bc 4d                   dec ebp
00b072bd 88542414             mov byte ptr [esp + 0x14], dl
00b072c1 89460c               mov dword ptr [esi + 0xc], eax
00b072c4 896e14               mov dword ptr [esi + 0x14], ebp
00b072c7 894e04               mov dword ptr [esi + 4], ecx
00b072ca eb0e                 jmp 0xb072da
00b072cc 6a01                 push 1
00b072ce 8d442418             lea eax, [esp + 0x18]
00b072d2 50                   push eax
00b072d3 8bce                 mov ecx, esi
00b072d5 e8c6c9e8ff           call 0x993ca0
00b072da 8a5c2414             mov bl, byte ptr [esp + 0x14]
00b072de 8a8793000000         mov al, byte ptr [edi + 0x93]
00b072e4 84db                 test bl, bl
00b072e6 0f95c1               setne cl
00b072e9 c0e104               shl cl, 4
00b072ec 32c8                 xor cl, al
00b072ee 80e110               and cl, 0x10
00b072f1 8ad8                 mov bl, al
00b072f3 32d9                 xor bl, cl
00b072f5 889f93000000         mov byte ptr [edi + 0x93], bl
00b072fb 8b4604               mov eax, dword ptr [esi + 4]
00b072fe 8d4801               lea ecx, [eax + 1]
00b07301 33c0                 xor eax, eax
00b07303 7c40                 jl 0xb07345
00b07305 7f04                 jg 0xb0730b
00b07307 85c9                 test ecx, ecx
00b07309 723a                 jb 0xb07345
00b0730b 85c0                 test eax, eax
00b0730d 7f36                 jg 0xb07345
00b0730f 7c08                 jl 0xb07319
00b07311 81f9ffffff7f         cmp ecx, 0x7fffffff
00b07317 772c                 ja 0xb07345
00b07319 8b6e14               mov ebp, dword ptr [esi + 0x14]
00b0731c 83fd01               cmp ebp, 1
00b0731f 7c16                 jl 0xb07337
00b07321 8b460c               mov eax, dword ptr [esi + 0xc]
00b07324 8a10                 mov dl, byte ptr [eax]
00b07326 40                   inc eax
00b07327 4d                   dec ebp
00b07328 88542414             mov byte ptr [esp + 0x14], dl
00b0732c 89460c               mov dword ptr [esi + 0xc], eax
00b0732f 896e14               mov dword ptr [esi + 0x14], ebp
00b07332 894e04               mov dword ptr [esi + 4], ecx
00b07335 eb0e                 jmp 0xb07345
00b07337 6a01                 push 1
00b07339 8d442418             lea eax, [esp + 0x18]
00b0733d 50                   push eax
00b0733e 8bce                 mov ecx, esi
00b07340 e85bc9e8ff           call 0x993ca0
00b07345 8a5c2414             mov bl, byte ptr [esp + 0x14]
00b07349 8a8793000000         mov al, byte ptr [edi + 0x93]
00b0734f 84db                 test bl, bl
00b07351 0f95c1               setne cl
00b07354 c0e105               shl cl, 5
00b07357 32c8                 xor cl, al
00b07359 80e120               and cl, 0x20
00b0735c 8ad8                 mov bl, al
00b0735e 32d9                 xor bl, cl
00b07360 889f93000000         mov byte ptr [edi + 0x93], bl
00b07366 8b4604               mov eax, dword ptr [esi + 4]
00b07369 8d4801               lea ecx, [eax + 1]
00b0736c 33c0                 xor eax, eax
00b0736e 7c40                 jl 0xb073b0
00b07370 7f04                 jg 0xb07376
00b07372 85c9                 test ecx, ecx
00b07374 723a                 jb 0xb073b0
00b07376 85c0                 test eax, eax
00b07378 7f36                 jg 0xb073b0
00b0737a 7c08                 jl 0xb07384
00b0737c 81f9ffffff7f         cmp ecx, 0x7fffffff
00b07382 772c                 ja 0xb073b0
00b07384 8b6e14               mov ebp, dword ptr [esi + 0x14]
00b07387 83fd01               cmp ebp, 1
00b0738a 7c16                 jl 0xb073a2
00b0738c 8b460c               mov eax, dword ptr [esi + 0xc]
00b0738f 8a10                 mov dl, byte ptr [eax]
00b07391 40                   inc eax
00b07392 4d                   dec ebp
00b07393 88542414             mov byte ptr [esp + 0x14], dl
00b07397 89460c               mov dword ptr [esi + 0xc], eax
00b0739a 896e14               mov dword ptr [esi + 0x14], ebp
00b0739d 894e04               mov dword ptr [esi + 4], ecx
00b073a0 eb0e                 jmp 0xb073b0
00b073a2 6a01                 push 1
00b073a4 8d442418             lea eax, [esp + 0x18]
00b073a8 50                   push eax
00b073a9 8bce                 mov ecx, esi
00b073ab e8f0c8e8ff           call 0x993ca0
00b073b0 8a5c2414             mov bl, byte ptr [esp + 0x14]
00b073b4 8a8793000000         mov al, byte ptr [edi + 0x93]
00b073ba 84db                 test bl, bl
00b073bc 0f95c1               setne cl
00b073bf c0e106               shl cl, 6
00b073c2 32c8                 xor cl, al
00b073c4 80e140               and cl, 0x40
00b073c7 8ad8                 mov bl, al
00b073c9 32d9                 xor bl, cl
00b073cb 889f93000000         mov byte ptr [edi + 0x93], bl
00b073d1 8b4604               mov eax, dword ptr [esi + 4]
00b073d4 8d4801               lea ecx, [eax + 1]
00b073d7 33c0                 xor eax, eax
00b073d9 7c40                 jl 0xb0741b
00b073db 7f04                 jg 0xb073e1
00b073dd 85c9                 test ecx, ecx
00b073df 723a                 jb 0xb0741b
00b073e1 85c0                 test eax, eax
00b073e3 7f36                 jg 0xb0741b
00b073e5 7c08                 jl 0xb073ef
00b073e7 81f9ffffff7f         cmp ecx, 0x7fffffff
00b073ed 772c                 ja 0xb0741b
00b073ef 8b6e14               mov ebp, dword ptr [esi + 0x14]
00b073f2 83fd01               cmp ebp, 1
00b073f5 7c16                 jl 0xb0740d
00b073f7 8b460c               mov eax, dword ptr [esi + 0xc]
00b073fa 8a10                 mov dl, byte ptr [eax]
00b073fc 40                   inc eax
00b073fd 4d                   dec ebp
00b073fe 88542414             mov byte ptr [esp + 0x14], dl
00b07402 89460c               mov dword ptr [esi + 0xc], eax
00b07405 896e14               mov dword ptr [esi + 0x14], ebp
00b07408 894e04               mov dword ptr [esi + 4], ecx
00b0740b eb0e                 jmp 0xb0741b
00b0740d 6a01                 push 1
00b0740f 8d442418             lea eax, [esp + 0x18]
00b07413 50                   push eax
00b07414 8bce                 mov ecx, esi
00b07416 e885c8e8ff           call 0x993ca0
00b0741b 8a4c2414             mov cl, byte ptr [esp + 0x14]
00b0741f 8a9793000000         mov dl, byte ptr [edi + 0x93]
00b07425 84c9                 test cl, cl
00b07427 0f95c1               setne cl
00b0742a c0e107               shl cl, 7
00b0742d 80e27f               and dl, 0x7f
00b07430 0aca                 or cl, dl
00b07432 888f93000000         mov byte ptr [edi + 0x93], cl
00b07438 8b4604               mov eax, dword ptr [esi + 4]
00b0743b 8d4801               lea ecx, [eax + 1]
00b0743e 33c0                 xor eax, eax
00b07440 7c40                 jl 0xb07482
00b07442 7f04                 jg 0xb07448
00b07444 85c9                 test ecx, ecx
00b07446 723a                 jb 0xb07482
00b07448 85c0                 test eax, eax
00b0744a 7f36                 jg 0xb07482
00b0744c 7c08                 jl 0xb07456
00b0744e 81f9ffffff7f         cmp ecx, 0x7fffffff
00b07454 772c                 ja 0xb07482
00b07456 8b6e14               mov ebp, dword ptr [esi + 0x14]
00b07459 83fd01               cmp ebp, 1
00b0745c 7c16                 jl 0xb07474
00b0745e 8b460c               mov eax, dword ptr [esi + 0xc]
00b07461 8a10                 mov dl, byte ptr [eax]
00b07463 40                   inc eax
00b07464 4d                   dec ebp
00b07465 88542414             mov byte ptr [esp + 0x14], dl
00b07469 89460c               mov dword ptr [esi + 0xc], eax
00b0746c 896e14               mov dword ptr [esi + 0x14], ebp
00b0746f 894e04               mov dword ptr [esi + 4], ecx
00b07472 eb0e                 jmp 0xb07482
00b07474 6a01                 push 1
00b07476 8d442418             lea eax, [esp + 0x18]
00b0747a 50                   push eax
00b0747b 8bce                 mov ecx, esi
00b0747d e81ec8e8ff           call 0x993ca0
00b07482 8a4c2414             mov cl, byte ptr [esp + 0x14]
00b07486 8a8794000000         mov al, byte ptr [edi + 0x94]
00b0748c 84c9                 test cl, cl
00b0748e 0f95c1               setne cl
00b07491 32c8                 xor cl, al
00b07493 80e101               and cl, 1
00b07496 8ad8                 mov bl, al
00b07498 32d9                 xor bl, cl
00b0749a 889f94000000         mov byte ptr [edi + 0x94], bl
00b074a0 8b4604               mov eax, dword ptr [esi + 4]
00b074a3 8d4801               lea ecx, [eax + 1]
00b074a6 33c0                 xor eax, eax
00b074a8 7c40                 jl 0xb074ea
00b074aa 7f04                 jg 0xb074b0
00b074ac 85c9                 test ecx, ecx
00b074ae 723a                 jb 0xb074ea
00b074b0 85c0                 test eax, eax
00b074b2 7f36                 jg 0xb074ea
00b074b4 7c08                 jl 0xb074be
00b074b6 81f9ffffff7f         cmp ecx, 0x7fffffff
00b074bc 772c                 ja 0xb074ea
00b074be 8b6e14               mov ebp, dword ptr [esi + 0x14]
00b074c1 83fd01               cmp ebp, 1
00b074c4 7c16                 jl 0xb074dc
00b074c6 8b460c               mov eax, dword ptr [esi + 0xc]
00b074c9 8a10                 mov dl, byte ptr [eax]
00b074cb 40                   inc eax
00b074cc 4d                   dec ebp
00b074cd 88542414             mov byte ptr [esp + 0x14], dl
00b074d1 89460c               mov dword ptr [esi + 0xc], eax
00b074d4 896e14               mov dword ptr [esi + 0x14], ebp
00b074d7 894e04               mov dword ptr [esi + 4], ecx
00b074da eb0e                 jmp 0xb074ea
00b074dc 6a01                 push 1
00b074de 8d442418             lea eax, [esp + 0x18]
00b074e2 50                   push eax
00b074e3 8bce                 mov ecx, esi
00b074e5 e8b6c7e8ff           call 0x993ca0
00b074ea 8a5c2414             mov bl, byte ptr [esp + 0x14]
00b074ee 84db                 test bl, bl
00b074f0 0f95c1               setne cl
00b074f3 888f95000000         mov byte ptr [edi + 0x95], cl
00b074f9 8b4604               mov eax, dword ptr [esi + 4]
00b074fc 8d4801               lea ecx, [eax + 1]
00b074ff 33c0                 xor eax, eax
00b07501 7c40                 jl 0xb07543
00b07503 7f04                 jg 0xb07509
00b07505 85c9                 test ecx, ecx
00b07507 723a                 jb 0xb07543
00b07509 85c0                 test eax, eax
00b0750b 7f36                 jg 0xb07543
00b0750d 7c08                 jl 0xb07517
00b0750f 81f9ffffff7f         cmp ecx, 0x7fffffff
00b07515 772c                 ja 0xb07543
00b07517 8b6e14               mov ebp, dword ptr [esi + 0x14]
00b0751a 83fd01               cmp ebp, 1
00b0751d 7c16                 jl 0xb07535
00b0751f 8b460c               mov eax, dword ptr [esi + 0xc]
00b07522 8a10                 mov dl, byte ptr [eax]
00b07524 40                   inc eax
00b07525 4d                   dec ebp
00b07526 88542414             mov byte ptr [esp + 0x14], dl
00b0752a 89460c               mov dword ptr [esi + 0xc], eax
00b0752d 896e14               mov dword ptr [esi + 0x14], ebp
00b07530 894e04               mov dword ptr [esi + 4], ecx
00b07533 eb0e                 jmp 0xb07543
00b07535 6a01                 push 1
00b07537 8d442418             lea eax, [esp + 0x18]
00b0753b 50                   push eax
00b0753c 8bce                 mov ecx, esi
00b0753e e85dc7e8ff           call 0x993ca0
00b07543 8a4c2414             mov cl, byte ptr [esp + 0x14]
00b07547 84c9                 test cl, cl
00b07549 0f95c1               setne cl
00b0754c 888f96000000         mov byte ptr [edi + 0x96], cl
00b07552 56                   push esi
00b07553 8bcf                 mov ecx, edi
00b07555 e8e6b50100           call 0xb22b40
00b0755a 5f                   pop edi
00b0755b 5e                   pop esi
00b0755c 5d                   pop ebp
00b0755d 5b                   pop ebx
00b0755e c20400               ret 4
