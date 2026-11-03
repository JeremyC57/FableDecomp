0053a0a1 55                   push ebp
0053a0a2 8bec                 mov ebp, esp
0053a0a4 83ec5c               sub esp, 0x5c
0053a0a7 8b4508               mov eax, dword ptr [ebp + 8]
0053a0aa 53                   push ebx
0053a0ab 56                   push esi
0053a0ac 57                   push edi
0053a0ad 33ff                 xor edi, edi
0053a0af 2bc7                 sub eax, edi
0053a0b1 8bf1                 mov esi, ecx
0053a0b3 0f84500b0000         je 0x53ac09
0053a0b9 48                   dec eax
0053a0ba 0f84170b0000         je 0x53abd7
0053a0c0 48                   dec eax
0053a0c1 48                   dec eax
0053a0c2 0f846a090000         je 0x53aa32
0053a0c8 48                   dec eax
0053a0c9 0f8471080000         je 0x53a940
0053a0cf 48                   dec eax
0053a0d0 0f849c000000         je 0x53a172
0053a0d6 48                   dec eax
0053a0d7 7408                 je 0x53a0e1
0053a0d9 ff7508               push dword ptr [ebp + 8]
0053a0dc e9180c0000           jmp 0x53acf9
0053a0e1 e80c45eeff           call 0x41e5f2
0053a0e6 8b10                 mov edx, dword ptr [eax]
0053a0e8 8bce                 mov ecx, esi
0053a0ea f7d9                 neg ecx
0053a0ec 1bc9                 sbb ecx, ecx
0053a0ee 8d7e04               lea edi, [esi + 4]
0053a0f1 23cf                 and ecx, edi
0053a0f3 51                   push ecx
0053a0f4 8bc8                 mov ecx, eax
0053a0f6 ff5214               call dword ptr [edx + 0x14]
0053a0f9 8d9ed8010000         lea ebx, [esi + 0x1d8]
0053a0ff 33ff                 xor edi, edi
0053a101 393b                 cmp dword ptr [ebx], edi
0053a103 7426                 je 0x53a12b
0053a105 8b03                 mov eax, dword ptr [ebx]
0053a107 8d4804               lea ecx, [eax + 4]
0053a10a 8b01                 mov eax, dword ptr [ecx]
0053a10c 6a19                 push 0x19
0053a10e ff5010               call dword ptr [eax + 0x10]
0053a111 8b1b                 mov ebx, dword ptr [ebx]
0053a113 3bdf                 cmp ebx, edi
0053a115 7405                 je 0x53a11c
0053a117 83c304               add ebx, 4
0053a11a eb02                 jmp 0x53a11e
0053a11c 33db                 xor ebx, ebx
0053a11e e8cf44eeff           call 0x41e5f2
0053a123 8b10                 mov edx, dword ptr [eax]
0053a125 53                   push ebx
0053a126 8bc8                 mov ecx, eax
0053a128 ff5214               call dword ptr [edx + 0x14]
0053a12b 8d9edc010000         lea ebx, [esi + 0x1dc]
0053a131 393b                 cmp dword ptr [ebx], edi
0053a133 7426                 je 0x53a15b
0053a135 8b03                 mov eax, dword ptr [ebx]
0053a137 8d4804               lea ecx, [eax + 4]
0053a13a 8b01                 mov eax, dword ptr [ecx]
0053a13c 6a19                 push 0x19
0053a13e ff5010               call dword ptr [eax + 0x10]
0053a141 8b1b                 mov ebx, dword ptr [ebx]
0053a143 3bdf                 cmp ebx, edi
0053a145 7405                 je 0x53a14c
0053a147 83c304               add ebx, 4
0053a14a eb02                 jmp 0x53a14e
0053a14c 33db                 xor ebx, ebx
0053a14e e89f44eeff           call 0x41e5f2
0053a153 8b10                 mov edx, dword ptr [eax]
0053a155 53                   push ebx
0053a156 8bc8                 mov ecx, eax
0053a158 ff5214               call dword ptr [edx + 0x14]
0053a15b 39be44010000         cmp dword ptr [esi + 0x144], edi
0053a161 8bce                 mov ecx, esi
0053a163 7506                 jne 0x53a16b
0053a165 57                   push edi
0053a166 e9900b0000           jmp 0x53acfb
0053a16b 6a06                 push 6
0053a16d e9890b0000           jmp 0x53acfb
0053a172 8a86e8010000         mov al, byte ptr [esi + 0x1e8]
0053a178 84c0                 test al, al
0053a17a 88450b               mov byte ptr [ebp + 0xb], al
0053a17d 7415                 je 0x53a194
0053a17f 8b06                 mov eax, dword ptr [esi]
0053a181 8bce                 mov ecx, esi
0053a183 ff90e0020000         call dword ptr [eax + 0x2e0]
0053a189 8b06                 mov eax, dword ptr [esi]
0053a18b 57                   push edi
0053a18c 8bce                 mov ecx, esi
0053a18e ff90d4020000         call dword ptr [eax + 0x2d4]
0053a194 33db                 xor ebx, ebx
0053a196 43                   inc ebx
0053a197 807d0b00             cmp byte ptr [ebp + 0xb], 0
0053a19b 746a                 je 0x53a207
0053a19d 8b8664010000         mov eax, dword ptr [esi + 0x164]
0053a1a3 3b8668010000         cmp eax, dword ptr [esi + 0x168]
0053a1a9 745c                 je 0x53a207
0053a1ab 8bbe68010000         mov edi, dword ptr [esi + 0x168]
0053a1b1 2bbe64010000         sub edi, dword ptr [esi + 0x164]
0053a1b7 8b8668010000         mov eax, dword ptr [esi + 0x168]
0053a1bd 2b8664010000         sub eax, dword ptr [esi + 0x164]
0053a1c3 c1ff02               sar edi, 2
0053a1c6 d1ef                 shr edi, 1
0053a1c8 a804                 test al, 4
0053a1ca 7501                 jne 0x53a1cd
0053a1cc 4f                   dec edi
0053a1cd 8b865c010000         mov eax, dword ptr [esi + 0x15c]
0053a1d3 3bf8                 cmp edi, eax
0053a1d5 7d18                 jge 0x53a1ef
0053a1d7 8b06                 mov eax, dword ptr [esi]
0053a1d9 6a00                 push 0
0053a1db 53                   push ebx
0053a1dc 8bce                 mov ecx, esi
0053a1de ff9098020000         call dword ptr [eax + 0x298]
0053a1e4 47                   inc edi
0053a1e5 3bbe5c010000         cmp edi, dword ptr [esi + 0x15c]
0053a1eb 75ea                 jne 0x53a1d7
0053a1ed eb18                 jmp 0x53a207
0053a1ef 7416                 je 0x53a207
0053a1f1 8b06                 mov eax, dword ptr [esi]
0053a1f3 6a00                 push 0
0053a1f5 53                   push ebx
0053a1f6 8bce                 mov ecx, esi
0053a1f8 ff90a0020000         call dword ptr [eax + 0x2a0]
0053a1fe 4f                   dec edi
0053a1ff 3bbe5c010000         cmp edi, dword ptr [esi + 0x15c]
0053a205 75ea                 jne 0x53a1f1
0053a207 e8e643eeff           call 0x41e5f2
0053a20c 8bf8                 mov edi, eax
0053a20e e8df43eeff           call 0x41e5f2
0053a213 8d4df8               lea ecx, [ebp - 8]
0053a216 894508               mov dword ptr [ebp + 8], eax
0053a219 8b07                 mov eax, dword ptr [edi]
0053a21b 51                   push ecx
0053a21c 8bcf                 mov ecx, edi
0053a21e ff90a8000000         call dword ptr [eax + 0xa8]
0053a224 8b00                 mov eax, dword ptr [eax]
0053a226 8b4d08               mov ecx, dword ptr [ebp + 8]
0053a229 8b11                 mov edx, dword ptr [ecx]
0053a22b 53                   push ebx
0053a22c 05a0010000           add eax, 0x1a0
0053a231 50                   push eax
0053a232 ff92a0000000         call dword ptr [edx + 0xa0]
0053a238 8b45f8               mov eax, dword ptr [ebp - 8]
0053a23b 85c0                 test eax, eax
0053a23d 7415                 je 0x53a254
0053a23f 8bc8                 mov ecx, eax
0053a241 83c004               add eax, 4
0053a244 ff08                 dec dword ptr [eax]
0053a246 833800               cmp dword ptr [eax], 0
0053a249 7505                 jne 0x53a250
0053a24b 8b01                 mov eax, dword ptr [ecx]
0053a24d ff5004               call dword ptr [eax + 4]
0053a250 8365f800             and dword ptr [ebp - 8], 0
0053a254 8b8ec4010000         mov ecx, dword ptr [esi + 0x1c4]
0053a25a 85c9                 test ecx, ecx
0053a25c 7c54                 jl 0x53a2b2
0053a25e 8d8664010000         lea eax, [esi + 0x164]
0053a264 8b5004               mov edx, dword ptr [eax + 4]
0053a267 2b10                 sub edx, dword ptr [eax]
0053a269 c1fa02               sar edx, 2
0053a26c 3bca                 cmp ecx, edx
0053a26e 7342                 jae 0x53a2b2
0053a270 8b865c010000         mov eax, dword ptr [esi + 0x15c]
0053a276 3bc8                 cmp ecx, eax
0053a278 7438                 je 0x53a2b2
0053a27a 7d1c                 jge 0x53a298
0053a27c 8b06                 mov eax, dword ptr [esi]
0053a27e 53                   push ebx
0053a27f 53                   push ebx
0053a280 8bce                 mov ecx, esi
0053a282 ff90a0020000         call dword ptr [eax + 0x2a0]
0053a288 8b86c4010000         mov eax, dword ptr [esi + 0x1c4]
0053a28e 3b865c010000         cmp eax, dword ptr [esi + 0x15c]
0053a294 75e6                 jne 0x53a27c
0053a296 eb1a                 jmp 0x53a2b2
0053a298 8b06                 mov eax, dword ptr [esi]
0053a29a 53                   push ebx
0053a29b 53                   push ebx
0053a29c 8bce                 mov ecx, esi
0053a29e ff9098020000         call dword ptr [eax + 0x298]
0053a2a4 8b86c4010000         mov eax, dword ptr [esi + 0x1c4]
0053a2aa 3b865c010000         cmp eax, dword ptr [esi + 0x15c]
0053a2b0 75e6                 jne 0x53a298
0053a2b2 8b06                 mov eax, dword ptr [esi]
0053a2b4 8bce                 mov ecx, esi
0053a2b6 ff90ac020000         call dword ptr [eax + 0x2ac]
0053a2bc 84c0                 test al, al
0053a2be 0f84dc020000         je 0x53a5a0
0053a2c4 8b8668010000         mov eax, dword ptr [esi + 0x168]
0053a2ca 2b8664010000         sub eax, dword ptr [esi + 0x164]
0053a2d0 8365fc00             and dword ptr [ebp - 4], 0
0053a2d4 c1f802               sar eax, 2
0053a2d7 0f84c3020000         je 0x53a5a0
0053a2dd 83650800             and dword ptr [ebp + 8], 0
0053a2e1 6a04                 push 4
0053a2e3 58                   pop eax
0053a2e4 6a05                 push 5
0053a2e6 59                   pop ecx
0053a2e7 895dec               mov dword ptr [ebp - 0x14], ebx
0053a2ea 895de8               mov dword ptr [ebp - 0x18], ebx
0053a2ed 8945e0               mov dword ptr [ebp - 0x20], eax
0053a2f0 8945dc               mov dword ptr [ebp - 0x24], eax
0053a2f3 894dd8               mov dword ptr [ebp - 0x28], ecx
0053a2f6 894dd4               mov dword ptr [ebp - 0x2c], ecx
0053a2f9 895dd0               mov dword ptr [ebp - 0x30], ebx
0053a2fc 895dcc               mov dword ptr [ebp - 0x34], ebx
0053a2ff 8945c8               mov dword ptr [ebp - 0x38], eax
0053a302 8945c4               mov dword ptr [ebp - 0x3c], eax
0053a305 894dc0               mov dword ptr [ebp - 0x40], ecx
0053a308 894dbc               mov dword ptr [ebp - 0x44], ecx
0053a30b 895db8               mov dword ptr [ebp - 0x48], ebx
0053a30e 895db4               mov dword ptr [ebp - 0x4c], ebx
0053a311 8945b0               mov dword ptr [ebp - 0x50], eax
0053a314 8945ac               mov dword ptr [ebp - 0x54], eax
0053a317 894da8               mov dword ptr [ebp - 0x58], ecx
0053a31a 894da4               mov dword ptr [ebp - 0x5c], ecx
0053a31d 8b8664010000         mov eax, dword ptr [esi + 0x164]
0053a323 8b4dfc               mov ecx, dword ptr [ebp - 4]
0053a326 8b3c88               mov edi, dword ptr [eax + ecx*4]
0053a329 8b867c010000         mov eax, dword ptr [esi + 0x17c]
0053a32f 8d4dec               lea ecx, [ebp - 0x14]
0053a332 51                   push ecx
0053a333 8b4d08               mov ecx, dword ptr [ebp + 8]
0053a336 03c8                 add ecx, eax
0053a338 e87432efff           call 0x42d5b1
0053a33d 8b5808               mov ebx, dword ptr [eax + 8]
0053a340 8b400c               mov eax, dword ptr [eax + 0xc]
0053a343 8b17                 mov edx, dword ptr [edi]
0053a345 8d4de8               lea ecx, [ebp - 0x18]
0053a348 51                   push ecx
0053a349 8bcf                 mov ecx, edi
0053a34b 8945e4               mov dword ptr [ebp - 0x1c], eax
0053a34e ff9248010000         call dword ptr [edx + 0x148]
0053a354 8bc8                 mov ecx, eax
0053a356 e85632efff           call 0x42d5b1
0053a35b 8b4dfc               mov ecx, dword ptr [ebp - 4]
0053a35e 895808               mov dword ptr [eax + 8], ebx
0053a361 8b5de4               mov ebx, dword ptr [ebp - 0x1c]
0053a364 89580c               mov dword ptr [eax + 0xc], ebx
0053a367 8b8664010000         mov eax, dword ptr [esi + 0x164]
0053a36d 8b3c88               mov edi, dword ptr [eax + ecx*4]
0053a370 8b867c010000         mov eax, dword ptr [esi + 0x17c]
0053a376 8d4de0               lea ecx, [ebp - 0x20]
0053a379 51                   push ecx
0053a37a 8b4d08               mov ecx, dword ptr [ebp + 8]
0053a37d 03c8                 add ecx, eax
0053a37f e82d32efff           call 0x42d5b1
0053a384 8b5808               mov ebx, dword ptr [eax + 8]
0053a387 8b400c               mov eax, dword ptr [eax + 0xc]
0053a38a 8b17                 mov edx, dword ptr [edi]
0053a38c 8d4ddc               lea ecx, [ebp - 0x24]
0053a38f 51                   push ecx
0053a390 8bcf                 mov ecx, edi
0053a392 8945e4               mov dword ptr [ebp - 0x1c], eax
0053a395 ff9248010000         call dword ptr [edx + 0x148]
0053a39b 8bc8                 mov ecx, eax
0053a39d e80f32efff           call 0x42d5b1
0053a3a2 8b4dfc               mov ecx, dword ptr [ebp - 4]
0053a3a5 895808               mov dword ptr [eax + 8], ebx
0053a3a8 8b5de4               mov ebx, dword ptr [ebp - 0x1c]
0053a3ab 89580c               mov dword ptr [eax + 0xc], ebx
0053a3ae 8b8664010000         mov eax, dword ptr [esi + 0x164]
0053a3b4 8b3c88               mov edi, dword ptr [eax + ecx*4]
0053a3b7 8b867c010000         mov eax, dword ptr [esi + 0x17c]
0053a3bd 8d4dd8               lea ecx, [ebp - 0x28]
0053a3c0 51                   push ecx
0053a3c1 8b4d08               mov ecx, dword ptr [ebp + 8]
0053a3c4 03c8                 add ecx, eax
0053a3c6 e8e631efff           call 0x42d5b1
0053a3cb 8b5808               mov ebx, dword ptr [eax + 8]
0053a3ce 8b400c               mov eax, dword ptr [eax + 0xc]
0053a3d1 8b17                 mov edx, dword ptr [edi]
0053a3d3 8d4dd4               lea ecx, [ebp - 0x2c]
0053a3d6 51                   push ecx
0053a3d7 8bcf                 mov ecx, edi
0053a3d9 8945e4               mov dword ptr [ebp - 0x1c], eax
0053a3dc ff9248010000         call dword ptr [edx + 0x148]
0053a3e2 8bc8                 mov ecx, eax
0053a3e4 e8c831efff           call 0x42d5b1
0053a3e9 8b7dfc               mov edi, dword ptr [ebp - 4]
0053a3ec 895808               mov dword ptr [eax + 8], ebx
0053a3ef 8b5de4               mov ebx, dword ptr [ebp - 0x1c]
0053a3f2 89580c               mov dword ptr [eax + 0xc], ebx
0053a3f5 8b8664010000         mov eax, dword ptr [esi + 0x164]
0053a3fb 8b1cb8               mov ebx, dword ptr [eax + edi*4]
0053a3fe 8b867c010000         mov eax, dword ptr [esi + 0x17c]
0053a404 8d4dd0               lea ecx, [ebp - 0x30]
0053a407 51                   push ecx
0053a408 8b4d08               mov ecx, dword ptr [ebp + 8]
0053a40b 03c8                 add ecx, eax
0053a40d e89f31efff           call 0x42d5b1
0053a412 8b13                 mov edx, dword ptr [ebx]
0053a414 8d4dcc               lea ecx, [ebp - 0x34]
0053a417 51                   push ecx
0053a418 8bcb                 mov ecx, ebx
0053a41a 8945e4               mov dword ptr [ebp - 0x1c], eax
0053a41d ff9248010000         call dword ptr [edx + 0x148]
0053a423 8bc8                 mov ecx, eax
0053a425 e88731efff           call 0x42d5b1
0053a42a 8b4de4               mov ecx, dword ptr [ebp - 0x1c]
0053a42d 8a491b               mov cl, byte ptr [ecx + 0x1b]
0053a430 88481b               mov byte ptr [eax + 0x1b], cl
0053a433 8b8664010000         mov eax, dword ptr [esi + 0x164]
0053a439 8b1cb8               mov ebx, dword ptr [eax + edi*4]
0053a43c 8b867c010000         mov eax, dword ptr [esi + 0x17c]
0053a442 8d4dc8               lea ecx, [ebp - 0x38]
0053a445 51                   push ecx
0053a446 8b4d08               mov ecx, dword ptr [ebp + 8]
0053a449 03c8                 add ecx, eax
0053a44b e86131efff           call 0x42d5b1
0053a450 8b13                 mov edx, dword ptr [ebx]
0053a452 8d4dc4               lea ecx, [ebp - 0x3c]
0053a455 51                   push ecx
0053a456 8bcb                 mov ecx, ebx
0053a458 8945e4               mov dword ptr [ebp - 0x1c], eax
0053a45b ff9248010000         call dword ptr [edx + 0x148]
0053a461 8bc8                 mov ecx, eax
0053a463 e84931efff           call 0x42d5b1
0053a468 8b4de4               mov ecx, dword ptr [ebp - 0x1c]
0053a46b 8a491b               mov cl, byte ptr [ecx + 0x1b]
0053a46e 88481b               mov byte ptr [eax + 0x1b], cl
0053a471 8b8664010000         mov eax, dword ptr [esi + 0x164]
0053a477 8b1cb8               mov ebx, dword ptr [eax + edi*4]
0053a47a 8b867c010000         mov eax, dword ptr [esi + 0x17c]
0053a480 8d4dc0               lea ecx, [ebp - 0x40]
0053a483 51                   push ecx
0053a484 8b4d08               mov ecx, dword ptr [ebp + 8]
0053a487 03c8                 add ecx, eax
0053a489 e82331efff           call 0x42d5b1
0053a48e 8b13                 mov edx, dword ptr [ebx]
0053a490 8d4dbc               lea ecx, [ebp - 0x44]
0053a493 51                   push ecx
0053a494 8bcb                 mov ecx, ebx
0053a496 8945e4               mov dword ptr [ebp - 0x1c], eax
0053a499 ff9248010000         call dword ptr [edx + 0x148]
0053a49f 8bc8                 mov ecx, eax
0053a4a1 e80b31efff           call 0x42d5b1
0053a4a6 8b4de4               mov ecx, dword ptr [ebp - 0x1c]
0053a4a9 8a491b               mov cl, byte ptr [ecx + 0x1b]
0053a4ac 88481b               mov byte ptr [eax + 0x1b], cl
0053a4af 8b8664010000         mov eax, dword ptr [esi + 0x164]
0053a4b5 8b3cb8               mov edi, dword ptr [eax + edi*4]
0053a4b8 8b867c010000         mov eax, dword ptr [esi + 0x17c]
0053a4be 8d4db8               lea ecx, [ebp - 0x48]
0053a4c1 51                   push ecx
0053a4c2 8b4d08               mov ecx, dword ptr [ebp + 8]
0053a4c5 03c8                 add ecx, eax
0053a4c7 e8e530efff           call 0x42d5b1
0053a4cc 8b5810               mov ebx, dword ptr [eax + 0x10]
0053a4cf 8b4014               mov eax, dword ptr [eax + 0x14]
0053a4d2 8b17                 mov edx, dword ptr [edi]
0053a4d4 8d4db4               lea ecx, [ebp - 0x4c]
0053a4d7 51                   push ecx
0053a4d8 8bcf                 mov ecx, edi
0053a4da 8945e4               mov dword ptr [ebp - 0x1c], eax
0053a4dd ff9248010000         call dword ptr [edx + 0x148]
0053a4e3 8bc8                 mov ecx, eax
0053a4e5 e8c730efff           call 0x42d5b1
0053a4ea 8b4dfc               mov ecx, dword ptr [ebp - 4]
0053a4ed 895810               mov dword ptr [eax + 0x10], ebx
0053a4f0 8b5de4               mov ebx, dword ptr [ebp - 0x1c]
0053a4f3 895814               mov dword ptr [eax + 0x14], ebx
0053a4f6 8b8664010000         mov eax, dword ptr [esi + 0x164]
0053a4fc 8b3c88               mov edi, dword ptr [eax + ecx*4]
0053a4ff 8b867c010000         mov eax, dword ptr [esi + 0x17c]
0053a505 8d4db0               lea ecx, [ebp - 0x50]
0053a508 51                   push ecx
0053a509 8b4d08               mov ecx, dword ptr [ebp + 8]
0053a50c 03c8                 add ecx, eax
0053a50e e89e30efff           call 0x42d5b1
0053a513 8b5810               mov ebx, dword ptr [eax + 0x10]
0053a516 8b4014               mov eax, dword ptr [eax + 0x14]
0053a519 8b17                 mov edx, dword ptr [edi]
0053a51b 8d4dac               lea ecx, [ebp - 0x54]
0053a51e 51                   push ecx
0053a51f 8bcf                 mov ecx, edi
0053a521 8945e4               mov dword ptr [ebp - 0x1c], eax
0053a524 ff9248010000         call dword ptr [edx + 0x148]
0053a52a 8bc8                 mov ecx, eax
0053a52c e88030efff           call 0x42d5b1
0053a531 8b4dfc               mov ecx, dword ptr [ebp - 4]
0053a534 895810               mov dword ptr [eax + 0x10], ebx
0053a537 8b5de4               mov ebx, dword ptr [ebp - 0x1c]
0053a53a 895814               mov dword ptr [eax + 0x14], ebx
0053a53d 8b8664010000         mov eax, dword ptr [esi + 0x164]
0053a543 8b3c88               mov edi, dword ptr [eax + ecx*4]
0053a546 8b867c010000         mov eax, dword ptr [esi + 0x17c]
0053a54c 8d4da8               lea ecx, [ebp - 0x58]
0053a54f 51                   push ecx
0053a550 8b4d08               mov ecx, dword ptr [ebp + 8]
0053a553 03c8                 add ecx, eax
0053a555 e85730efff           call 0x42d5b1
0053a55a 8b5810               mov ebx, dword ptr [eax + 0x10]
0053a55d 8b4014               mov eax, dword ptr [eax + 0x14]
0053a560 8b17                 mov edx, dword ptr [edi]
0053a562 8d4da4               lea ecx, [ebp - 0x5c]
0053a565 51                   push ecx
0053a566 8bcf                 mov ecx, edi
0053a568 8945e4               mov dword ptr [ebp - 0x1c], eax
0053a56b ff9248010000         call dword ptr [edx + 0x148]
0053a571 8bc8                 mov ecx, eax
0053a573 e83930efff           call 0x42d5b1
0053a578 ff45fc               inc dword ptr [ebp - 4]
0053a57b 8345080c             add dword ptr [ebp + 8], 0xc
0053a57f 895810               mov dword ptr [eax + 0x10], ebx
0053a582 8b5de4               mov ebx, dword ptr [ebp - 0x1c]
0053a585 895814               mov dword ptr [eax + 0x14], ebx
0053a588 8b8668010000         mov eax, dword ptr [esi + 0x168]
0053a58e 2b8664010000         sub eax, dword ptr [esi + 0x164]
0053a594 c1f802               sar eax, 2
0053a597 3945fc               cmp dword ptr [ebp - 4], eax
0053a59a 0f827dfdffff         jb 0x53a31d
0053a5a0 8b06                 mov eax, dword ptr [esi]
0053a5a2 8d4df4               lea ecx, [ebp - 0xc]
0053a5a5 51                   push ecx
0053a5a6 8bce                 mov ecx, esi
0053a5a8 ff90b0010000         call dword ptr [eax + 0x1b0]
0053a5ae 8b00                 mov eax, dword ptr [eax]
0053a5b0 8a980a020000         mov bl, byte ptr [eax + 0x20a]
0053a5b6 8b45f4               mov eax, dword ptr [ebp - 0xc]
0053a5b9 33ff                 xor edi, edi
0053a5bb 3bc7                 cmp eax, edi
0053a5bd 7413                 je 0x53a5d2
0053a5bf 8bc8                 mov ecx, eax
0053a5c1 83c004               add eax, 4
0053a5c4 ff08                 dec dword ptr [eax]
0053a5c6 3938                 cmp dword ptr [eax], edi
0053a5c8 7505                 jne 0x53a5cf
0053a5ca 8b01                 mov eax, dword ptr [ecx]
0053a5cc ff5004               call dword ptr [eax + 4]
0053a5cf 897df4               mov dword ptr [ebp - 0xc], edi
0053a5d2 84db                 test bl, bl
0053a5d4 0f8466010000         je 0x53a740
0053a5da a1988a3b01           mov eax, dword ptr [0x13b8a98]
0053a5df 8b10                 mov edx, dword ptr [eax]
0053a5e1 8bca                 mov ecx, edx
0053a5e3 3bc8                 cmp ecx, eax
0053a5e5 0f8444010000         je 0x53a72f
0053a5eb 8b09                 mov ecx, dword ptr [ecx]
0053a5ed 47                   inc edi
0053a5ee 3bc8                 cmp ecx, eax
0053a5f0 75f9                 jne 0x53a5eb
0053a5f2 85ff                 test edi, edi
0053a5f4 0f8635010000         jbe 0x53a72f
0053a5fa 8b5208               mov edx, dword ptr [edx + 8]
0053a5fd 85d2                 test edx, edx
0053a5ff 7405                 je 0x53a606
0053a601 8d7a04               lea edi, [edx + 4]
0053a604 eb02                 jmp 0x53a608
0053a606 33ff                 xor edi, edi
0053a608 e8e53feeff           call 0x41e5f2
0053a60d 8b10                 mov edx, dword ptr [eax]
0053a60f 57                   push edi
0053a610 8bc8                 mov ecx, eax
0053a612 ff5214               call dword ptr [edx + 0x14]
0053a615 a1988a3b01           mov eax, dword ptr [0x13b8a98]
0053a61a 8b00                 mov eax, dword ptr [eax]
0053a61c 8b4008               mov eax, dword ptr [eax + 8]
0053a61f 8b80d8010000         mov eax, dword ptr [eax + 0x1d8]
0053a625 85c0                 test eax, eax
0053a627 7433                 je 0x53a65c
0053a629 8d4804               lea ecx, [eax + 4]
0053a62c 8b01                 mov eax, dword ptr [ecx]
0053a62e 6a19                 push 0x19
0053a630 ff5010               call dword ptr [eax + 0x10]
0053a633 a1988a3b01           mov eax, dword ptr [0x13b8a98]
0053a638 8b00                 mov eax, dword ptr [eax]
0053a63a 8b4008               mov eax, dword ptr [eax + 8]
0053a63d 05d8010000           add eax, 0x1d8
0053a642 8b00                 mov eax, dword ptr [eax]
0053a644 85c0                 test eax, eax
0053a646 7405                 je 0x53a64d
0053a648 8d7804               lea edi, [eax + 4]
0053a64b eb02                 jmp 0x53a64f
0053a64d 33ff                 xor edi, edi
0053a64f e89e3feeff           call 0x41e5f2
0053a654 8b10                 mov edx, dword ptr [eax]
0053a656 57                   push edi
0053a657 8bc8                 mov ecx, eax
0053a659 ff5214               call dword ptr [edx + 0x14]
0053a65c a1988a3b01           mov eax, dword ptr [0x13b8a98]
0053a661 8b00                 mov eax, dword ptr [eax]
0053a663 8b4008               mov eax, dword ptr [eax + 8]
0053a666 8b88dc010000         mov ecx, dword ptr [eax + 0x1dc]
0053a66c 85c9                 test ecx, ecx
0053a66e 7433                 je 0x53a6a3
0053a670 83c104               add ecx, 4
0053a673 8b01                 mov eax, dword ptr [ecx]
0053a675 6a19                 push 0x19
0053a677 ff5010               call dword ptr [eax + 0x10]
0053a67a a1988a3b01           mov eax, dword ptr [0x13b8a98]
0053a67f 8b00                 mov eax, dword ptr [eax]
0053a681 8b4008               mov eax, dword ptr [eax + 8]
0053a684 05dc010000           add eax, 0x1dc
0053a689 8b00                 mov eax, dword ptr [eax]
0053a68b 85c0                 test eax, eax
0053a68d 7405                 je 0x53a694
0053a68f 8d7804               lea edi, [eax + 4]
0053a692 eb02                 jmp 0x53a696
0053a694 33ff                 xor edi, edi
0053a696 e8573feeff           call 0x41e5f2
0053a69b 8b10                 mov edx, dword ptr [eax]
0053a69d 57                   push edi
0053a69e 8bc8                 mov ecx, eax
0053a6a0 ff5214               call dword ptr [edx + 0x14]
0053a6a3 a1988a3b01           mov eax, dword ptr [0x13b8a98]
0053a6a8 8b00                 mov eax, dword ptr [eax]
0053a6aa 8b4008               mov eax, dword ptr [eax + 8]
0053a6ad 8b8868010000         mov ecx, dword ptr [eax + 0x168]
0053a6b3 0564010000           add eax, 0x164
0053a6b8 2b08                 sub ecx, dword ptr [eax]
0053a6ba c1f902               sar ecx, 2
0053a6bd 7470                 je 0x53a72f
0053a6bf 8b4804               mov ecx, dword ptr [eax + 4]
0053a6c2 2b08                 sub ecx, dword ptr [eax]
0053a6c4 33ff                 xor edi, edi
0053a6c6 c1f902               sar ecx, 2
0053a6c9 7464                 je 0x53a72f
0053a6cb 8b00                 mov eax, dword ptr [eax]
0053a6cd 8bdf                 mov ebx, edi
0053a6cf c1e302               shl ebx, 2
0053a6d2 03c3                 add eax, ebx
0053a6d4 8b00                 mov eax, dword ptr [eax]
0053a6d6 85c0                 test eax, eax
0053a6d8 7408                 je 0x53a6e2
0053a6da 83c004               add eax, 4
0053a6dd 894508               mov dword ptr [ebp + 8], eax
0053a6e0 eb04                 jmp 0x53a6e6
0053a6e2 83650800             and dword ptr [ebp + 8], 0
0053a6e6 e8073feeff           call 0x41e5f2
0053a6eb ff7508               push dword ptr [ebp + 8]
0053a6ee 8b10                 mov edx, dword ptr [eax]
0053a6f0 8bc8                 mov ecx, eax
0053a6f2 ff5214               call dword ptr [edx + 0x14]
0053a6f5 a1988a3b01           mov eax, dword ptr [0x13b8a98]
0053a6fa 8b00                 mov eax, dword ptr [eax]
0053a6fc 8b4008               mov eax, dword ptr [eax + 8]
0053a6ff 8b8064010000         mov eax, dword ptr [eax + 0x164]
0053a705 8b0c03               mov ecx, dword ptr [ebx + eax]
0053a708 8b01                 mov eax, dword ptr [ecx]
0053a70a ff90b4010000         call dword ptr [eax + 0x1b4]
0053a710 a1988a3b01           mov eax, dword ptr [0x13b8a98]
0053a715 8b00                 mov eax, dword ptr [eax]
0053a717 8b4008               mov eax, dword ptr [eax + 8]
0053a71a 8b8868010000         mov ecx, dword ptr [eax + 0x168]
0053a720 0564010000           add eax, 0x164
0053a725 2b08                 sub ecx, dword ptr [eax]
0053a727 47                   inc edi
0053a728 c1f902               sar ecx, 2
0053a72b 3bf9                 cmp edi, ecx
0053a72d 729c                 jb 0x53a6cb
0053a72f 8d4508               lea eax, [ebp + 8]
0053a732 50                   push eax
0053a733 b9988a3b01           mov ecx, 0x13b8a98
0053a738 897508               mov dword ptr [ebp + 8], esi
0053a73b e8fc220000           call 0x53ca3c
0053a740 8b86b4000000         mov eax, dword ptr [esi + 0xb4]
0053a746 2b86b0000000         sub eax, dword ptr [esi + 0xb0]
0053a74c 33ff                 xor edi, edi
0053a74e c1f803               sar eax, 3
0053a751 7426                 je 0x53a779
0053a753 8b86b0000000         mov eax, dword ptr [esi + 0xb0]
0053a759 8b0cf8               mov ecx, dword ptr [eax + edi*8]
0053a75c 8b01                 mov eax, dword ptr [ecx]
0053a75e 56                   push esi
0053a75f ff90cc000000         call dword ptr [eax + 0xcc]
0053a765 8b86b4000000         mov eax, dword ptr [esi + 0xb4]
0053a76b 2b86b0000000         sub eax, dword ptr [esi + 0xb0]
0053a771 47                   inc edi
0053a772 c1f803               sar eax, 3
0053a775 3bf8                 cmp edi, eax
0053a777 72da                 jb 0x53a753
0053a779 8b8668010000         mov eax, dword ptr [esi + 0x168]
0053a77f 2b8664010000         sub eax, dword ptr [esi + 0x164]
0053a785 c1f802               sar eax, 2
0053a788 0f84cc000000         je 0x53a85a
0053a78e 8b8668010000         mov eax, dword ptr [esi + 0x168]
0053a794 2b8664010000         sub eax, dword ptr [esi + 0x164]
0053a79a 33ff                 xor edi, edi
0053a79c c1f802               sar eax, 2
0053a79f 7455                 je 0x53a7f6
0053a7a1 8b8664010000         mov eax, dword ptr [esi + 0x164]
0053a7a7 8bdf                 mov ebx, edi
0053a7a9 c1e302               shl ebx, 2
0053a7ac 03c3                 add eax, ebx
0053a7ae 8b00                 mov eax, dword ptr [eax]
0053a7b0 85c0                 test eax, eax
0053a7b2 7408                 je 0x53a7bc
0053a7b4 83c004               add eax, 4
0053a7b7 894508               mov dword ptr [ebp + 8], eax
0053a7ba eb04                 jmp 0x53a7c0
0053a7bc 83650800             and dword ptr [ebp + 8], 0
0053a7c0 e82d3eeeff           call 0x41e5f2
0053a7c5 ff7508               push dword ptr [ebp + 8]
0053a7c8 8b10                 mov edx, dword ptr [eax]
0053a7ca 8bc8                 mov ecx, eax
0053a7cc ff5208               call dword ptr [edx + 8]
0053a7cf 8b8664010000         mov eax, dword ptr [esi + 0x164]
0053a7d5 8b0403               mov eax, dword ptr [ebx + eax]
0053a7d8 8d4804               lea ecx, [eax + 4]
0053a7db 8b01                 mov eax, dword ptr [ecx]
0053a7dd 6a19                 push 0x19
0053a7df ff500c               call dword ptr [eax + 0xc]
0053a7e2 8b8668010000         mov eax, dword ptr [esi + 0x168]
0053a7e8 2b8664010000         sub eax, dword ptr [esi + 0x164]
0053a7ee 47                   inc edi
0053a7ef c1f802               sar eax, 2
0053a7f2 3bf8                 cmp edi, eax
0053a7f4 72ab                 jb 0x53a7a1
0053a7f6 8b865c010000         mov eax, dword ptr [esi + 0x15c]
0053a7fc 8b8e64010000         mov ecx, dword ptr [esi + 0x164]
0053a802 8b0c81               mov ecx, dword ptr [ecx + eax*4]
0053a805 83c104               add ecx, 4
0053a808 8b01                 mov eax, dword ptr [ecx]
0053a80a 6a02                 push 2
0053a80c ff500c               call dword ptr [eax + 0xc]
0053a80f 8b865c010000         mov eax, dword ptr [esi + 0x15c]
0053a815 8b8e64010000         mov ecx, dword ptr [esi + 0x164]
0053a81b 8b0c81               mov ecx, dword ptr [ecx + eax*4]
0053a81e 83c104               add ecx, 4
0053a821 8b01                 mov eax, dword ptr [ecx]
0053a823 6a03                 push 3
0053a825 ff500c               call dword ptr [eax + 0xc]
0053a828 8b865c010000         mov eax, dword ptr [esi + 0x15c]
0053a82e 8b8e64010000         mov ecx, dword ptr [esi + 0x164]
0053a834 8b0c81               mov ecx, dword ptr [ecx + eax*4]
0053a837 83c104               add ecx, 4
0053a83a 8b01                 mov eax, dword ptr [ecx]
0053a83c 6a04                 push 4
0053a83e ff500c               call dword ptr [eax + 0xc]
0053a841 8b865c010000         mov eax, dword ptr [esi + 0x15c]
0053a847 8b8e64010000         mov ecx, dword ptr [esi + 0x164]
0053a84d 8b0c81               mov ecx, dword ptr [ecx + eax*4]
0053a850 83c104               add ecx, 4
0053a853 8b01                 mov eax, dword ptr [ecx]
0053a855 6a05                 push 5
0053a857 ff500c               call dword ptr [eax + 0xc]
0053a85a 8bbed8010000         mov edi, dword ptr [esi + 0x1d8]
0053a860 85ff                 test edi, edi
0053a862 7420                 je 0x53a884
0053a864 e8893deeff           call 0x41e5f2
0053a869 8b10                 mov edx, dword ptr [eax]
0053a86b 83c704               add edi, 4
0053a86e 57                   push edi
0053a86f 8bc8                 mov ecx, eax
0053a871 ff5208               call dword ptr [edx + 8]
0053a874 8b86d8010000         mov eax, dword ptr [esi + 0x1d8]
0053a87a 8d4804               lea ecx, [eax + 4]
0053a87d 8b01                 mov eax, dword ptr [ecx]
0053a87f 6a19                 push 0x19
0053a881 ff500c               call dword ptr [eax + 0xc]
0053a884 8bbedc010000         mov edi, dword ptr [esi + 0x1dc]
0053a88a 85ff                 test edi, edi
0053a88c 7420                 je 0x53a8ae
0053a88e e85f3deeff           call 0x41e5f2
0053a893 8b10                 mov edx, dword ptr [eax]
0053a895 83c704               add edi, 4
0053a898 57                   push edi
0053a899 8bc8                 mov ecx, eax
0053a89b ff5208               call dword ptr [edx + 8]
0053a89e 8b86dc010000         mov eax, dword ptr [esi + 0x1dc]
0053a8a4 8d4804               lea ecx, [eax + 4]
0053a8a7 8b01                 mov eax, dword ptr [ecx]
0053a8a9 6a19                 push 0x19
0053a8ab ff500c               call dword ptr [eax + 0xc]
0053a8ae 6a05                 push 5
0053a8b0 8bce                 mov ecx, esi
0053a8b2 e88926ffff           call 0x52cf40
0053a8b7 8b06                 mov eax, dword ptr [esi]
0053a8b9 8d4df0               lea ecx, [ebp - 0x10]
0053a8bc 51                   push ecx
0053a8bd 8bce                 mov ecx, esi
0053a8bf ff90b0010000         call dword ptr [eax + 0x1b0]
0053a8c5 8b00                 mov eax, dword ptr [eax]
0053a8c7 8a980a020000         mov bl, byte ptr [eax + 0x20a]
0053a8cd 8b45f0               mov eax, dword ptr [ebp - 0x10]
0053a8d0 85c0                 test eax, eax
0053a8d2 7415                 je 0x53a8e9
0053a8d4 8bc8                 mov ecx, eax
0053a8d6 83c004               add eax, 4
0053a8d9 ff08                 dec dword ptr [eax]
0053a8db 833800               cmp dword ptr [eax], 0
0053a8de 7505                 jne 0x53a8e5
0053a8e0 8b01                 mov eax, dword ptr [ecx]
0053a8e2 ff5004               call dword ptr [eax + 4]
0053a8e5 8365f000             and dword ptr [ebp - 0x10], 0
0053a8e9 84db                 test bl, bl
0053a8eb 741a                 je 0x53a907
0053a8ed a1988a3b01           mov eax, dword ptr [0x13b8a98]
0053a8f2 8b08                 mov ecx, dword ptr [eax]
0053a8f4 8b09                 mov ecx, dword ptr [ecx]
0053a8f6 3bc8                 cmp ecx, eax
0053a8f8 740d                 je 0x53a907
0053a8fa 8b4908               mov ecx, dword ptr [ecx + 8]
0053a8fd 8b01                 mov eax, dword ptr [ecx]
0053a8ff 6a07                 push 7
0053a901 ff90c0000000         call dword ptr [eax + 0xc0]
0053a907 8b8668010000         mov eax, dword ptr [esi + 0x168]
0053a90d 2b8664010000         sub eax, dword ptr [esi + 0x164]
0053a913 c1f802               sar eax, 2
0053a916 7419                 je 0x53a931
0053a918 8b865c010000         mov eax, dword ptr [esi + 0x15c]
0053a91e 8b8e64010000         mov ecx, dword ptr [esi + 0x164]
0053a924 8b0c81               mov ecx, dword ptr [ecx + eax*4]
0053a927 8b01                 mov eax, dword ptr [ecx]
0053a929 6a03                 push 3
0053a92b ff90c0000000         call dword ptr [eax + 0xc0]
0053a931 8b06                 mov eax, dword ptr [esi]
0053a933 8bce                 mov ecx, esi
0053a935 ff90d0020000         call dword ptr [eax + 0x2d0]
0053a93b e9c0030000           jmp 0x53ad00
0053a940 e8ad3ceeff           call 0x41e5f2
0053a945 8b10                 mov edx, dword ptr [eax]
0053a947 8bce                 mov ecx, esi
0053a949 f7d9                 neg ecx
0053a94b 1bc9                 sbb ecx, ecx
0053a94d 8d5e04               lea ebx, [esi + 4]
0053a950 23cb                 and ecx, ebx
0053a952 51                   push ecx
0053a953 8bc8                 mov ecx, eax
0053a955 ff5214               call dword ptr [edx + 0x14]
0053a958 8b8668010000         mov eax, dword ptr [esi + 0x168]
0053a95e 2b8664010000         sub eax, dword ptr [esi + 0x164]
0053a964 c1f802               sar eax, 2
0053a967 7464                 je 0x53a9cd
0053a969 8b8668010000         mov eax, dword ptr [esi + 0x168]
0053a96f 2b8664010000         sub eax, dword ptr [esi + 0x164]
0053a975 c1f802               sar eax, 2
0053a978 7453                 je 0x53a9cd
0053a97a 8b8664010000         mov eax, dword ptr [esi + 0x164]
0053a980 8bdf                 mov ebx, edi
0053a982 c1e302               shl ebx, 2
0053a985 03c3                 add eax, ebx
0053a987 8b00                 mov eax, dword ptr [eax]
0053a989 85c0                 test eax, eax
0053a98b 7408                 je 0x53a995
0053a98d 83c004               add eax, 4
0053a990 894508               mov dword ptr [ebp + 8], eax
0053a993 eb04                 jmp 0x53a999
0053a995 83650800             and dword ptr [ebp + 8], 0
0053a999 e8543ceeff           call 0x41e5f2
0053a99e ff7508               push dword ptr [ebp + 8]
0053a9a1 8b10                 mov edx, dword ptr [eax]
0053a9a3 8bc8                 mov ecx, eax
0053a9a5 ff5214               call dword ptr [edx + 0x14]
0053a9a8 8b8664010000         mov eax, dword ptr [esi + 0x164]
0053a9ae 8b0c03               mov ecx, dword ptr [ebx + eax]
0053a9b1 8b01                 mov eax, dword ptr [ecx]
0053a9b3 ff90b4010000         call dword ptr [eax + 0x1b4]
0053a9b9 8b8668010000         mov eax, dword ptr [esi + 0x168]
0053a9bf 2b8664010000         sub eax, dword ptr [esi + 0x164]
0053a9c5 47                   inc edi
0053a9c6 c1f802               sar eax, 2
0053a9c9 3bf8                 cmp edi, eax
0053a9cb 72ad                 jb 0x53a97a
0053a9cd 8d9ed8010000         lea ebx, [esi + 0x1d8]
0053a9d3 33ff                 xor edi, edi
0053a9d5 393b                 cmp dword ptr [ebx], edi
0053a9d7 7426                 je 0x53a9ff
0053a9d9 8b03                 mov eax, dword ptr [ebx]
0053a9db 8d4804               lea ecx, [eax + 4]
0053a9de 8b01                 mov eax, dword ptr [ecx]
0053a9e0 6a19                 push 0x19
0053a9e2 ff5010               call dword ptr [eax + 0x10]
0053a9e5 8b1b                 mov ebx, dword ptr [ebx]
0053a9e7 3bdf                 cmp ebx, edi
0053a9e9 7405                 je 0x53a9f0
0053a9eb 83c304               add ebx, 4
0053a9ee eb02                 jmp 0x53a9f2
0053a9f0 33db                 xor ebx, ebx
0053a9f2 e8fb3beeff           call 0x41e5f2
0053a9f7 8b10                 mov edx, dword ptr [eax]
0053a9f9 53                   push ebx
0053a9fa 8bc8                 mov ecx, eax
0053a9fc ff5214               call dword ptr [edx + 0x14]
0053a9ff 8d9edc010000         lea ebx, [esi + 0x1dc]
0053aa05 393b                 cmp dword ptr [ebx], edi
0053aa07 7422                 je 0x53aa2b
0053aa09 8b03                 mov eax, dword ptr [ebx]
0053aa0b 8d4804               lea ecx, [eax + 4]
0053aa0e 8b01                 mov eax, dword ptr [ecx]
0053aa10 6a19                 push 0x19
0053aa12 ff5010               call dword ptr [eax + 0x10]
0053aa15 8b1b                 mov ebx, dword ptr [ebx]
0053aa17 3bdf                 cmp ebx, edi
0053aa19 7403                 je 0x53aa1e
0053aa1b 8d7b04               lea edi, [ebx + 4]
0053aa1e e8cf3beeff           call 0x41e5f2
0053aa23 8b10                 mov edx, dword ptr [eax]
0053aa25 57                   push edi
0053aa26 8bc8                 mov ecx, eax
0053aa28 ff5214               call dword ptr [edx + 0x14]
0053aa2b 6a04                 push 4
0053aa2d e9c7020000           jmp 0x53acf9
0053aa32 e8bb3beeff           call 0x41e5f2
0053aa37 8b10                 mov edx, dword ptr [eax]
0053aa39 8bce                 mov ecx, esi
0053aa3b f7d9                 neg ecx
0053aa3d 1bc9                 sbb ecx, ecx
0053aa3f 8d5e04               lea ebx, [esi + 4]
0053aa42 23cb                 and ecx, ebx
0053aa44 51                   push ecx
0053aa45 8bc8                 mov ecx, eax
0053aa47 ff5208               call dword ptr [edx + 8]
0053aa4a 8b03                 mov eax, dword ptr [ebx]
0053aa4c 6a01                 push 1
0053aa4e 8bcb                 mov ecx, ebx
0053aa50 ff500c               call dword ptr [eax + 0xc]
0053aa53 8b03                 mov eax, dword ptr [ebx]
0053aa55 6a00                 push 0
0053aa57 8bcb                 mov ecx, ebx
0053aa59 ff500c               call dword ptr [eax + 0xc]
0053aa5c 8b8668010000         mov eax, dword ptr [esi + 0x168]
0053aa62 2b8664010000         sub eax, dword ptr [esi + 0x164]
0053aa68 c1f802               sar eax, 2
0053aa6b 0f84cc000000         je 0x53ab3d
0053aa71 8b8668010000         mov eax, dword ptr [esi + 0x168]
0053aa77 2b8664010000         sub eax, dword ptr [esi + 0x164]
0053aa7d 33ff                 xor edi, edi
0053aa7f c1f802               sar eax, 2
0053aa82 7455                 je 0x53aad9
0053aa84 8b8664010000         mov eax, dword ptr [esi + 0x164]
0053aa8a 8bdf                 mov ebx, edi
0053aa8c c1e302               shl ebx, 2
0053aa8f 03c3                 add eax, ebx
0053aa91 8b00                 mov eax, dword ptr [eax]
0053aa93 85c0                 test eax, eax
0053aa95 7408                 je 0x53aa9f
0053aa97 83c004               add eax, 4
0053aa9a 894508               mov dword ptr [ebp + 8], eax
0053aa9d eb04                 jmp 0x53aaa3
0053aa9f 83650800             and dword ptr [ebp + 8], 0
0053aaa3 e84a3beeff           call 0x41e5f2
0053aaa8 ff7508               push dword ptr [ebp + 8]
0053aaab 8b10                 mov edx, dword ptr [eax]
0053aaad 8bc8                 mov ecx, eax
0053aaaf ff5208               call dword ptr [edx + 8]
0053aab2 8b8664010000         mov eax, dword ptr [esi + 0x164]
0053aab8 8b0403               mov eax, dword ptr [ebx + eax]
0053aabb 8d4804               lea ecx, [eax + 4]
0053aabe 8b01                 mov eax, dword ptr [ecx]
0053aac0 6a19                 push 0x19
0053aac2 ff500c               call dword ptr [eax + 0xc]
0053aac5 8b8668010000         mov eax, dword ptr [esi + 0x168]
0053aacb 2b8664010000         sub eax, dword ptr [esi + 0x164]
0053aad1 47                   inc edi
0053aad2 c1f802               sar eax, 2
0053aad5 3bf8                 cmp edi, eax
0053aad7 72ab                 jb 0x53aa84
0053aad9 8b8e64010000         mov ecx, dword ptr [esi + 0x164]
0053aadf 8b865c010000         mov eax, dword ptr [esi + 0x15c]
0053aae5 8b0481               mov eax, dword ptr [ecx + eax*4]
0053aae8 8d4804               lea ecx, [eax + 4]
0053aaeb 8b01                 mov eax, dword ptr [ecx]
0053aaed 6a02                 push 2
0053aaef ff500c               call dword ptr [eax + 0xc]
0053aaf2 8b8e64010000         mov ecx, dword ptr [esi + 0x164]
0053aaf8 8b865c010000         mov eax, dword ptr [esi + 0x15c]
0053aafe 8b0481               mov eax, dword ptr [ecx + eax*4]
0053ab01 8d4804               lea ecx, [eax + 4]
0053ab04 8b01                 mov eax, dword ptr [ecx]
0053ab06 6a03                 push 3
0053ab08 ff500c               call dword ptr [eax + 0xc]
0053ab0b 8b8e64010000         mov ecx, dword ptr [esi + 0x164]
0053ab11 8b865c010000         mov eax, dword ptr [esi + 0x15c]
0053ab17 8b0481               mov eax, dword ptr [ecx + eax*4]
0053ab1a 8d4804               lea ecx, [eax + 4]
0053ab1d 8b01                 mov eax, dword ptr [ecx]
0053ab1f 6a04                 push 4
0053ab21 ff500c               call dword ptr [eax + 0xc]
0053ab24 8b8e64010000         mov ecx, dword ptr [esi + 0x164]
0053ab2a 8b865c010000         mov eax, dword ptr [esi + 0x15c]
0053ab30 8b0481               mov eax, dword ptr [ecx + eax*4]
0053ab33 8d4804               lea ecx, [eax + 4]
0053ab36 8b01                 mov eax, dword ptr [ecx]
0053ab38 6a05                 push 5
0053ab3a ff500c               call dword ptr [eax + 0xc]
0053ab3d 6a03                 push 3
0053ab3f 8bce                 mov ecx, esi
0053ab41 e8fa23ffff           call 0x52cf40
0053ab46 8b8668010000         mov eax, dword ptr [esi + 0x168]
0053ab4c 2b8664010000         sub eax, dword ptr [esi + 0x164]
0053ab52 c1f802               sar eax, 2
0053ab55 7419                 je 0x53ab70
0053ab57 8b865c010000         mov eax, dword ptr [esi + 0x15c]
0053ab5d 8b8e64010000         mov ecx, dword ptr [esi + 0x164]
0053ab63 8b0c81               mov ecx, dword ptr [ecx + eax*4]
0053ab66 8b01                 mov eax, dword ptr [ecx]
0053ab68 6a03                 push 3
0053ab6a ff90c0000000         call dword ptr [eax + 0xc0]
0053ab70 8b06                 mov eax, dword ptr [esi]
0053ab72 8bce                 mov ecx, esi
0053ab74 ff90d0020000         call dword ptr [eax + 0x2d0]
0053ab7a 8b9ed8010000         mov ebx, dword ptr [esi + 0x1d8]
0053ab80 85db                 test ebx, ebx
0053ab82 7420                 je 0x53aba4
0053ab84 e8693aeeff           call 0x41e5f2
0053ab89 8b10                 mov edx, dword ptr [eax]
0053ab8b 83c304               add ebx, 4
0053ab8e 53                   push ebx
0053ab8f 8bc8                 mov ecx, eax
0053ab91 ff5208               call dword ptr [edx + 8]
0053ab94 8b86d8010000         mov eax, dword ptr [esi + 0x1d8]
0053ab9a 8d4804               lea ecx, [eax + 4]
0053ab9d 8b01                 mov eax, dword ptr [ecx]
0053ab9f 6a19                 push 0x19
0053aba1 ff500c               call dword ptr [eax + 0xc]
0053aba4 8b9edc010000         mov ebx, dword ptr [esi + 0x1dc]
0053abaa 85db                 test ebx, ebx
0053abac 0f844e010000         je 0x53ad00
0053abb2 e83b3aeeff           call 0x41e5f2
0053abb7 8b10                 mov edx, dword ptr [eax]
0053abb9 83c304               add ebx, 4
0053abbc 53                   push ebx
0053abbd 8bc8                 mov ecx, eax
0053abbf ff5208               call dword ptr [edx + 8]
0053abc2 8bb6dc010000         mov esi, dword ptr [esi + 0x1dc]
0053abc8 8d4e04               lea ecx, [esi + 4]
0053abcb 8b01                 mov eax, dword ptr [ecx]
0053abcd 6a19                 push 0x19
0053abcf ff500c               call dword ptr [eax + 0xc]
0053abd2 e929010000           jmp 0x53ad00
0053abd7 e8163aeeff           call 0x41e5f2
0053abdc 8b10                 mov edx, dword ptr [eax]
0053abde 8bce                 mov ecx, esi
0053abe0 f7d9                 neg ecx
0053abe2 1bc9                 sbb ecx, ecx
0053abe4 8d7e04               lea edi, [esi + 4]
0053abe7 23cf                 and ecx, edi
0053abe9 51                   push ecx
0053abea 8bc8                 mov ecx, eax
0053abec ff5208               call dword ptr [edx + 8]
0053abef 8b07                 mov eax, dword ptr [edi]
0053abf1 33db                 xor ebx, ebx
0053abf3 43                   inc ebx
0053abf4 53                   push ebx
0053abf5 8bcf                 mov ecx, edi
0053abf7 ff500c               call dword ptr [eax + 0xc]
0053abfa 8b07                 mov eax, dword ptr [edi]
0053abfc 6a00                 push 0
0053abfe 8bcf                 mov ecx, edi
0053ac00 ff500c               call dword ptr [eax + 0xc]
0053ac03 53                   push ebx
0053ac04 e9f0000000           jmp 0x53acf9
0053ac09 e8e439eeff           call 0x41e5f2
0053ac0e 8b10                 mov edx, dword ptr [eax]
0053ac10 8bce                 mov ecx, esi
0053ac12 f7d9                 neg ecx
0053ac14 1bc9                 sbb ecx, ecx
0053ac16 8d5e04               lea ebx, [esi + 4]
0053ac19 23cb                 and ecx, ebx
0053ac1b 51                   push ecx
0053ac1c 8bc8                 mov ecx, eax
0053ac1e ff5214               call dword ptr [edx + 0x14]
0053ac21 8b8668010000         mov eax, dword ptr [esi + 0x168]
0053ac27 2b8664010000         sub eax, dword ptr [esi + 0x164]
0053ac2d c1f802               sar eax, 2
0053ac30 7464                 je 0x53ac96
0053ac32 8b8668010000         mov eax, dword ptr [esi + 0x168]
0053ac38 2b8664010000         sub eax, dword ptr [esi + 0x164]
0053ac3e c1f802               sar eax, 2
0053ac41 7453                 je 0x53ac96
0053ac43 8b8664010000         mov eax, dword ptr [esi + 0x164]
0053ac49 8bdf                 mov ebx, edi
0053ac4b c1e302               shl ebx, 2
0053ac4e 03c3                 add eax, ebx
0053ac50 8b00                 mov eax, dword ptr [eax]
0053ac52 85c0                 test eax, eax
0053ac54 7408                 je 0x53ac5e
0053ac56 83c004               add eax, 4
0053ac59 894508               mov dword ptr [ebp + 8], eax
0053ac5c eb04                 jmp 0x53ac62
0053ac5e 83650800             and dword ptr [ebp + 8], 0
0053ac62 e88b39eeff           call 0x41e5f2
0053ac67 ff7508               push dword ptr [ebp + 8]
0053ac6a 8b10                 mov edx, dword ptr [eax]
0053ac6c 8bc8                 mov ecx, eax
0053ac6e ff5214               call dword ptr [edx + 0x14]
0053ac71 8b8664010000         mov eax, dword ptr [esi + 0x164]
0053ac77 8b0c03               mov ecx, dword ptr [ebx + eax]
0053ac7a 8b01                 mov eax, dword ptr [ecx]
0053ac7c ff90b4010000         call dword ptr [eax + 0x1b4]
0053ac82 8b8668010000         mov eax, dword ptr [esi + 0x168]
0053ac88 2b8664010000         sub eax, dword ptr [esi + 0x164]
0053ac8e 47                   inc edi
0053ac8f c1f802               sar eax, 2
0053ac92 3bf8                 cmp edi, eax
0053ac94 72ad                 jb 0x53ac43
0053ac96 8d9ed8010000         lea ebx, [esi + 0x1d8]
0053ac9c 33ff                 xor edi, edi
0053ac9e 393b                 cmp dword ptr [ebx], edi
0053aca0 7426                 je 0x53acc8
0053aca2 8b03                 mov eax, dword ptr [ebx]
0053aca4 8d4804               lea ecx, [eax + 4]
0053aca7 8b01                 mov eax, dword ptr [ecx]
0053aca9 6a19                 push 0x19
0053acab ff5010               call dword ptr [eax + 0x10]
0053acae 8b1b                 mov ebx, dword ptr [ebx]
0053acb0 3bdf                 cmp ebx, edi
0053acb2 7405                 je 0x53acb9
0053acb4 83c304               add ebx, 4
0053acb7 eb02                 jmp 0x53acbb
0053acb9 33db                 xor ebx, ebx
0053acbb e83239eeff           call 0x41e5f2
0053acc0 8b10                 mov edx, dword ptr [eax]
0053acc2 53                   push ebx
0053acc3 8bc8                 mov ecx, eax
0053acc5 ff5214               call dword ptr [edx + 0x14]
0053acc8 8d9edc010000         lea ebx, [esi + 0x1dc]
0053acce 393b                 cmp dword ptr [ebx], edi
0053acd0 7426                 je 0x53acf8
0053acd2 8b03                 mov eax, dword ptr [ebx]
0053acd4 8d4804               lea ecx, [eax + 4]
0053acd7 8b01                 mov eax, dword ptr [ecx]
0053acd9 6a19                 push 0x19
0053acdb ff5010               call dword ptr [eax + 0x10]
0053acde 8b1b                 mov ebx, dword ptr [ebx]
0053ace0 3bdf                 cmp ebx, edi
0053ace2 7405                 je 0x53ace9
0053ace4 83c304               add ebx, 4
0053ace7 eb02                 jmp 0x53aceb
0053ace9 33db                 xor ebx, ebx
0053aceb e80239eeff           call 0x41e5f2
0053acf0 8b10                 mov edx, dword ptr [eax]
0053acf2 53                   push ebx
0053acf3 8bc8                 mov ecx, eax
0053acf5 ff5214               call dword ptr [edx + 0x14]
0053acf8 57                   push edi
0053acf9 8bce                 mov ecx, esi
0053acfb e84022ffff           call 0x52cf40
0053ad00 5f                   pop edi
0053ad01 5e                   pop esi
0053ad02 5b                   pop ebx
0053ad03 c9                   leave 
0053ad04 c20400               ret 4
