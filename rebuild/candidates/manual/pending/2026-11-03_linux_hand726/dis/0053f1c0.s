0053f1c0 83ec44               sub esp, 0x44
0053f1c3 53                   push ebx
0053f1c4 56                   push esi
0053f1c5 8bf1                 mov esi, ecx
0053f1c7 8b8678010000         mov eax, dword ptr [esi + 0x178]
0053f1cd 57                   push edi
0053f1ce 8d4c2438             lea ecx, [esp + 0x38]
0053f1d2 51                   push ecx
0053f1d3 89442424             mov dword ptr [esp + 0x24], eax
0053f1d7 e816f4edff           call 0x41e5f2
0053f1dc 8bc8                 mov ecx, eax
0053f1de e864ddedff           call 0x41cf47
0053f1e3 8b16                 mov edx, dword ptr [esi]
0053f1e5 8bce                 mov ecx, esi
0053f1e7 8bf8                 mov edi, eax
0053f1e9 ff5264               call dword ptr [edx + 0x64]
0053f1ec d9442420             fld dword ptr [esp + 0x20]
0053f1f0 d80f                 fmul dword ptr [edi]
0053f1f2 8d4c2438             lea ecx, [esp + 0x38]
0053f1f6 51                   push ecx
0053f1f7 d800                 fadd dword ptr [eax]
0053f1f9 8b867c010000         mov eax, dword ptr [esi + 0x17c]
0053f1ff 89442424             mov dword ptr [esp + 0x24], eax
0053f203 d95c2444             fstp dword ptr [esp + 0x44]
0053f207 e8e6f3edff           call 0x41e5f2
0053f20c 8bc8                 mov ecx, eax
0053f20e e834ddedff           call 0x41cf47
0053f213 d9442420             fld dword ptr [esp + 0x20]
0053f217 d84804               fmul dword ptr [eax + 4]
0053f21a 8b16                 mov edx, dword ptr [esi]
0053f21c 8bce                 mov ecx, esi
0053f21e d95c2420             fstp dword ptr [esp + 0x20]
0053f222 ff5264               call dword ptr [edx + 0x64]
0053f225 d9442420             fld dword ptr [esp + 0x20]
0053f229 d84004               fadd dword ptr [eax + 4]
0053f22c 8d442430             lea eax, [esp + 0x30]
0053f230 50                   push eax
0053f231 d95c2448             fstp dword ptr [esp + 0x48]
0053f235 d98680010000         fld dword ptr [esi + 0x180]
0053f23b d8a678010000         fsub dword ptr [esi + 0x178]
0053f241 d95c243c             fstp dword ptr [esp + 0x3c]
0053f245 e8a8f3edff           call 0x41e5f2
0053f24a 8bc8                 mov ecx, eax
0053f24c e8f6dcedff           call 0x41cf47
0053f251 d9442438             fld dword ptr [esp + 0x38]
0053f255 d808                 fmul dword ptr [eax]
0053f257 8d4c2430             lea ecx, [esp + 0x30]
0053f25b 51                   push ecx
0053f25c d8442444             fadd dword ptr [esp + 0x44]
0053f260 d95c244c             fstp dword ptr [esp + 0x4c]
0053f264 d98684010000         fld dword ptr [esi + 0x184]
0053f26a d8a67c010000         fsub dword ptr [esi + 0x17c]
0053f270 d95c2440             fstp dword ptr [esp + 0x40]
0053f274 e879f3edff           call 0x41e5f2
0053f279 8bc8                 mov ecx, eax
0053f27b e8c7dcedff           call 0x41cf47
0053f280 d944243c             fld dword ptr [esp + 0x3c]
0053f284 d84804               fmul dword ptr [eax + 4]
0053f287 8b96b4000000         mov edx, dword ptr [esi + 0xb4]
0053f28d 2b96b0000000         sub edx, dword ptr [esi + 0xb0]
0053f293 d8442444             fadd dword ptr [esp + 0x44]
0053f297 c1fa03               sar edx, 3
0053f29a 33ff                 xor edi, edi
0053f29c 85d2                 test edx, edx
0053f29e d95c244c             fstp dword ptr [esp + 0x4c]
0053f2a2 c744241000000000     mov dword ptr [esp + 0x10], 0
0053f2aa c744240c00000000     mov dword ptr [esp + 0xc], 0
0053f2b2 b301                 mov bl, 1
0053f2b4 0f8686000000         jbe 0x53f340
0053f2ba 8d9b00000000         lea ebx, [ebx]
0053f2c0 8b86b0000000         mov eax, dword ptr [esi + 0xb0]
0053f2c6 8b0cf8               mov ecx, dword ptr [eax + edi*8]
0053f2c9 8b11                 mov edx, dword ptr [ecx]
0053f2cb ff9288010000         call dword ptr [edx + 0x188]
0053f2d1 84c0                 test al, al
0053f2d3 7457                 je 0x53f32c
0053f2d5 8b86b0000000         mov eax, dword ptr [esi + 0xb0]
0053f2db 8b0cf8               mov ecx, dword ptr [eax + edi*8]
0053f2de 3b8e88010000         cmp ecx, dword ptr [esi + 0x188]
0053f2e4 7446                 je 0x53f32c
0053f2e6 3b8e8c010000         cmp ecx, dword ptr [esi + 0x18c]
0053f2ec 743e                 je 0x53f32c
0053f2ee 8b11                 mov edx, dword ptr [ecx]
0053f2f0 ff5264               call dword ptr [edx + 0x64]
0053f2f3 8b86b0000000         mov eax, dword ptr [esi + 0xb0]
0053f2f9 8b0cf8               mov ecx, dword ptr [eax + edi*8]
0053f2fc 8b11                 mov edx, dword ptr [ecx]
0053f2fe 8d442440             lea eax, [esp + 0x40]
0053f302 50                   push eax
0053f303 6a01                 push 1
0053f305 ff9260010000         call dword ptr [edx + 0x160]
0053f30b d9442410             fld dword ptr [esp + 0x10]
0053f30f d9e1                 fabs 
0053f311 d9c1                 fld st(1)
0053f313 d9e1                 fabs 
0053f315 d9c9                 fxch st(1)
0053f317 d9c9                 fxch st(1)
0053f319 ded9                 fcompp 
0053f31b dfe0                 fnstsw ax
0053f31d f6c441               test ah, 0x41
0053f320 7506                 jne 0x53f328
0053f322 d95c2410             fstp dword ptr [esp + 0x10]
0053f326 eb02                 jmp 0x53f32a
0053f328 ddd8                 fstp st(0)
0053f32a 32db                 xor bl, bl
0053f32c 8b8eb4000000         mov ecx, dword ptr [esi + 0xb4]
0053f332 2b8eb0000000         sub ecx, dword ptr [esi + 0xb0]
0053f338 47                   inc edi
0053f339 c1f903               sar ecx, 3
0053f33c 3bf9                 cmp edi, ecx
0053f33e 7280                 jb 0x53f2c0
0053f340 8b9668010000         mov edx, dword ptr [esi + 0x168]
0053f346 2b9664010000         sub edx, dword ptr [esi + 0x164]
0053f34c c1fa02               sar edx, 2
0053f34f 33ff                 xor edi, edi
0053f351 85d2                 test edx, edx
0053f353 7670                 jbe 0x53f3c5
0053f355 8b8664010000         mov eax, dword ptr [esi + 0x164]
0053f35b 8b0cb8               mov ecx, dword ptr [eax + edi*4]
0053f35e 8b11                 mov edx, dword ptr [ecx]
0053f360 ff9288010000         call dword ptr [edx + 0x188]
0053f366 84c0                 test al, al
0053f368 7447                 je 0x53f3b1
0053f36a 8b8664010000         mov eax, dword ptr [esi + 0x164]
0053f370 8b0cb8               mov ecx, dword ptr [eax + edi*4]
0053f373 8b11                 mov edx, dword ptr [ecx]
0053f375 ff5264               call dword ptr [edx + 0x64]
0053f378 8b8664010000         mov eax, dword ptr [esi + 0x164]
0053f37e 8b0cb8               mov ecx, dword ptr [eax + edi*4]
0053f381 8b11                 mov edx, dword ptr [ecx]
0053f383 8d442440             lea eax, [esp + 0x40]
0053f387 50                   push eax
0053f388 6a01                 push 1
0053f38a ff9260010000         call dword ptr [edx + 0x160]
0053f390 d9442410             fld dword ptr [esp + 0x10]
0053f394 d9e1                 fabs 
0053f396 d9c1                 fld st(1)
0053f398 d9e1                 fabs 
0053f39a d9c9                 fxch st(1)
0053f39c d9c9                 fxch st(1)
0053f39e ded9                 fcompp 
0053f3a0 dfe0                 fnstsw ax
0053f3a2 f6c441               test ah, 0x41
0053f3a5 7506                 jne 0x53f3ad
0053f3a7 d95c2410             fstp dword ptr [esp + 0x10]
0053f3ab eb02                 jmp 0x53f3af
0053f3ad ddd8                 fstp st(0)
0053f3af 32db                 xor bl, bl
0053f3b1 8b8e68010000         mov ecx, dword ptr [esi + 0x168]
0053f3b7 2b8e64010000         sub ecx, dword ptr [esi + 0x164]
0053f3bd 47                   inc edi
0053f3be c1f902               sar ecx, 2
0053f3c1 3bf9                 cmp edi, ecx
0053f3c3 7290                 jb 0x53f355
0053f3c5 8b96b4000000         mov edx, dword ptr [esi + 0xb4]
0053f3cb 2b96b0000000         sub edx, dword ptr [esi + 0xb0]
0053f3d1 c1fa03               sar edx, 3
0053f3d4 33ff                 xor edi, edi
0053f3d6 85d2                 test edx, edx
0053f3d8 0f8680000000         jbe 0x53f45e
0053f3de 8bff                 mov edi, edi
0053f3e0 8b86b0000000         mov eax, dword ptr [esi + 0xb0]
0053f3e6 8b0cf8               mov ecx, dword ptr [eax + edi*8]
0053f3e9 8b11                 mov edx, dword ptr [ecx]
0053f3eb ff9288010000         call dword ptr [edx + 0x188]
0053f3f1 84c0                 test al, al
0053f3f3 7455                 je 0x53f44a
0053f3f5 8b86b0000000         mov eax, dword ptr [esi + 0xb0]
0053f3fb 8b0cf8               mov ecx, dword ptr [eax + edi*8]
0053f3fe 3b8e88010000         cmp ecx, dword ptr [esi + 0x188]
0053f404 7444                 je 0x53f44a
0053f406 3b8e8c010000         cmp ecx, dword ptr [esi + 0x18c]
0053f40c 743c                 je 0x53f44a
0053f40e 8b11                 mov edx, dword ptr [ecx]
0053f410 ff5264               call dword ptr [edx + 0x64]
0053f413 8b86b0000000         mov eax, dword ptr [esi + 0xb0]
0053f419 8b0cf8               mov ecx, dword ptr [eax + edi*8]
0053f41c 8b11                 mov edx, dword ptr [ecx]
0053f41e 8d442440             lea eax, [esp + 0x40]
0053f422 50                   push eax
0053f423 6a00                 push 0
0053f425 ff9260010000         call dword ptr [edx + 0x160]
0053f42b d944240c             fld dword ptr [esp + 0xc]
0053f42f d9e1                 fabs 
0053f431 d9c1                 fld st(1)
0053f433 d9e1                 fabs 
0053f435 d9c9                 fxch st(1)
0053f437 d9c9                 fxch st(1)
0053f439 ded9                 fcompp 
0053f43b dfe0                 fnstsw ax
0053f43d f6c441               test ah, 0x41
0053f440 7506                 jne 0x53f448
0053f442 d95c240c             fstp dword ptr [esp + 0xc]
0053f446 eb02                 jmp 0x53f44a
0053f448 ddd8                 fstp st(0)
0053f44a 8b8eb4000000         mov ecx, dword ptr [esi + 0xb4]
0053f450 2b8eb0000000         sub ecx, dword ptr [esi + 0xb0]
0053f456 47                   inc edi
0053f457 c1f903               sar ecx, 3
0053f45a 3bf9                 cmp edi, ecx
0053f45c 7282                 jb 0x53f3e0
0053f45e 8b9668010000         mov edx, dword ptr [esi + 0x168]
0053f464 2b9664010000         sub edx, dword ptr [esi + 0x164]
0053f46a c1fa02               sar edx, 2
0053f46d 33ff                 xor edi, edi
0053f46f 85d2                 test edx, edx
0053f471 766e                 jbe 0x53f4e1
0053f473 8b8664010000         mov eax, dword ptr [esi + 0x164]
0053f479 8b0cb8               mov ecx, dword ptr [eax + edi*4]
0053f47c 8b11                 mov edx, dword ptr [ecx]
0053f47e ff9288010000         call dword ptr [edx + 0x188]
0053f484 84c0                 test al, al
0053f486 7445                 je 0x53f4cd
0053f488 8b8664010000         mov eax, dword ptr [esi + 0x164]
0053f48e 8b0cb8               mov ecx, dword ptr [eax + edi*4]
0053f491 8b11                 mov edx, dword ptr [ecx]
0053f493 ff5264               call dword ptr [edx + 0x64]
0053f496 8b8664010000         mov eax, dword ptr [esi + 0x164]
0053f49c 8b0cb8               mov ecx, dword ptr [eax + edi*4]
0053f49f 8b11                 mov edx, dword ptr [ecx]
0053f4a1 8d442440             lea eax, [esp + 0x40]
0053f4a5 50                   push eax
0053f4a6 6a00                 push 0
0053f4a8 ff9260010000         call dword ptr [edx + 0x160]
0053f4ae d944240c             fld dword ptr [esp + 0xc]
0053f4b2 d9e1                 fabs 
0053f4b4 d9c1                 fld st(1)
0053f4b6 d9e1                 fabs 
0053f4b8 d9c9                 fxch st(1)
0053f4ba d9c9                 fxch st(1)
0053f4bc ded9                 fcompp 
0053f4be dfe0                 fnstsw ax
0053f4c0 f6c441               test ah, 0x41
0053f4c3 7506                 jne 0x53f4cb
0053f4c5 d95c240c             fstp dword ptr [esp + 0xc]
0053f4c9 eb02                 jmp 0x53f4cd
0053f4cb ddd8                 fstp st(0)
0053f4cd 8b8e68010000         mov ecx, dword ptr [esi + 0x168]
0053f4d3 2b8e64010000         sub ecx, dword ptr [esi + 0x164]
0053f4d9 47                   inc edi
0053f4da c1f902               sar ecx, 2
0053f4dd 3bf9                 cmp edi, ecx
0053f4df 7292                 jb 0x53f473
0053f4e1 d9442410             fld dword ptr [esp + 0x10]
0053f4e5 d98694010000         fld dword ptr [esi + 0x194]
0053f4eb dae9                 fucompp 
0053f4ed dfe0                 fnstsw ax
0053f4ef f6c444               test ah, 0x44
0053f4f2 7a13                 jp 0x53f507
0053f4f4 d944240c             fld dword ptr [esp + 0xc]
0053f4f8 d98698010000         fld dword ptr [esi + 0x198]
0053f4fe dae9                 fucompp 
0053f500 dfe0                 fnstsw ax
0053f502 f6c444               test ah, 0x44
0053f505 7b1b                 jnp 0x53f522
0053f507 8b54240c             mov edx, dword ptr [esp + 0xc]
0053f50b 8b442410             mov eax, dword ptr [esp + 0x10]
0053f50f c6869001000001       mov byte ptr [esi + 0x190], 1
0053f516 899698010000         mov dword ptr [esi + 0x198], edx
0053f51c 898694010000         mov dword ptr [esi + 0x194], eax
0053f522 8b8e88010000         mov ecx, dword ptr [esi + 0x188]
0053f528 85c9                 test ecx, ecx
0053f52a 0f84b5020000         je 0x53f7e5
0053f530 84db                 test bl, bl
0053f532 d944244c             fld dword ptr [esp + 0x4c]
0053f536 d8642444             fsub dword ptr [esp + 0x44]
0053f53a c74424380000803f     mov dword ptr [esp + 0x38], 0x3f800000
0053f542 d944240c             fld dword ptr [esp + 0xc]
0053f546 d8642410             fsub dword ptr [esp + 0x10]
0053f54a d9542414             fst dword ptr [esp + 0x14]
0053f54e d8c1                 fadd st(1)
0053f550 d83dd8de2201         fdivr dword ptr [0x122ded8]
0053f556 d95c2420             fstp dword ptr [esp + 0x20]
0053f55a d84c2420             fmul dword ptr [esp + 0x20]
0053f55e d95c243c             fstp dword ptr [esp + 0x3c]
0053f562 7408                 je 0x53f56c
0053f564 c744243c00000000     mov dword ptr [esp + 0x3c], 0
0053f56c 8b11                 mov edx, dword ptr [ecx]
0053f56e 6a00                 push 0
0053f570 6a00                 push 0
0053f572 8d442440             lea eax, [esp + 0x40]
0053f576 50                   push eax
0053f577 ff928c000000         call dword ptr [edx + 0x8c]
0053f57d 8b8e88010000         mov ecx, dword ptr [esi + 0x188]
0053f583 8b01                 mov eax, dword ptr [ecx]
0053f585 8d542418             lea edx, [esp + 0x18]
0053f589 52                   push edx
0053f58a c744241c00000000     mov dword ptr [esp + 0x1c], 0
0053f592 ff9048010000         call dword ptr [eax + 0x148]
0053f598 8bc8                 mov ecx, eax
0053f59a e812e0eeff           call 0x42d5b1
0053f59f 8b4808               mov ecx, dword ptr [eax + 8]
0053f5a2 83c008               add eax, 8
0053f5a5 894c2430             mov dword ptr [esp + 0x30], ecx
0053f5a9 8b5004               mov edx, dword ptr [eax + 4]
0053f5ac 8b8e88010000         mov ecx, dword ptr [esi + 0x188]
0053f5b2 89542434             mov dword ptr [esp + 0x34], edx
0053f5b6 8b01                 mov eax, dword ptr [ecx]
0053f5b8 8d542424             lea edx, [esp + 0x24]
0053f5bc 52                   push edx
0053f5bd ff90b0010000         call dword ptr [eax + 0x1b0]
0053f5c3 8b8e88010000         mov ecx, dword ptr [esi + 0x188]
0053f5c9 8b38                 mov edi, dword ptr [eax]
0053f5cb 8b01                 mov eax, dword ptr [ecx]
0053f5cd 8d542418             lea edx, [esp + 0x18]
0053f5d1 52                   push edx
0053f5d2 ff90b0010000         call dword ptr [eax + 0x1b0]
0053f5d8 d944243c             fld dword ptr [esp + 0x3c]
0053f5dc 8b00                 mov eax, dword ptr [eax]
0053f5de d88846010000         fmul dword ptr [eax + 0x146]
0053f5e4 8b442418             mov eax, dword ptr [esp + 0x18]
0053f5e8 85c0                 test eax, eax
0053f5ea d8af46010000         fsubr dword ptr [edi + 0x146]
0053f5f0 d95c242c             fstp dword ptr [esp + 0x2c]
0053f5f4 741e                 je 0x53f614
0053f5f6 8b7804               mov edi, dword ptr [eax + 4]
0053f5f9 8bc8                 mov ecx, eax
0053f5fb 83c004               add eax, 4
0053f5fe 4f                   dec edi
0053f5ff 8bd7                 mov edx, edi
0053f601 85d2                 test edx, edx
0053f603 8938                 mov dword ptr [eax], edi
0053f605 7505                 jne 0x53f60c
0053f607 8b11                 mov edx, dword ptr [ecx]
0053f609 ff5204               call dword ptr [edx + 4]
0053f60c c744241800000000     mov dword ptr [esp + 0x18], 0
0053f614 8b442424             mov eax, dword ptr [esp + 0x24]
0053f618 85c0                 test eax, eax
0053f61a 7416                 je 0x53f632
0053f61c 8b7804               mov edi, dword ptr [eax + 4]
0053f61f 8bc8                 mov ecx, eax
0053f621 83c004               add eax, 4
0053f624 4f                   dec edi
0053f625 8bd7                 mov edx, edi
0053f627 85d2                 test edx, edx
0053f629 8938                 mov dword ptr [eax], edi
0053f62b 7505                 jne 0x53f632
0053f62d 8b01                 mov eax, dword ptr [ecx]
0053f62f ff5004               call dword ptr [eax + 4]
0053f632 d9442414             fld dword ptr [esp + 0x14]
0053f636 d81ddcde2201         fcomp dword ptr [0x122dedc]
0053f63c dfe0                 fnstsw ax
0053f63e f6c441               test ah, 0x41
0053f641 7514                 jne 0x53f657
0053f643 d944240c             fld dword ptr [esp + 0xc]
0053f647 d8742414             fdiv dword ptr [esp + 0x14]
0053f64b d84c242c             fmul dword ptr [esp + 0x2c]
0053f64f d8442434             fadd dword ptr [esp + 0x34]
0053f653 d95c2434             fstp dword ptr [esp + 0x34]
0053f657 8bbe88010000         mov edi, dword ptr [esi + 0x188]
0053f65d 85ff                 test edi, edi
0053f65f 7464                 je 0x53f6c5
0053f661 8a8690010000         mov al, byte ptr [esi + 0x190]
0053f667 84c0                 test al, al
0053f669 745a                 je 0x53f6c5
0053f66b d905dcde2201         fld dword ptr [0x122dedc]
0053f671 d944240c             fld dword ptr [esp + 0xc]
0053f675 dae9                 fucompp 
0053f677 dfe0                 fnstsw ax
0053f679 f6c444               test ah, 0x44
0053f67c 7b21                 jnp 0x53f69f
0053f67e d905dcde2201         fld dword ptr [0x122dedc]
0053f684 d9442410             fld dword ptr [esp + 0x10]
0053f688 dae9                 fucompp 
0053f68a dfe0                 fnstsw ax
0053f68c f6c444               test ah, 0x44
0053f68f 7b0e                 jnp 0x53f69f
0053f691 8b17                 mov edx, dword ptr [edi]
0053f693 8bcf                 mov ecx, edi
0053f695 ff9258020000         call dword ptr [edx + 0x258]
0053f69b 84c0                 test al, al
0053f69d 7526                 jne 0x53f6c5
0053f69f 8b8e88010000         mov ecx, dword ptr [esi + 0x188]
0053f6a5 8b01                 mov eax, dword ptr [ecx]
0053f6a7 6a00                 push 0
0053f6a9 68cdcc4c3e           push 0x3e4ccccd
0053f6ae 8d542438             lea edx, [esp + 0x38]
0053f6b2 52                   push edx
0053f6b3 ff507c               call dword ptr [eax + 0x7c]
0053f6b6 8b07                 mov eax, dword ptr [edi]
0053f6b8 8d4c2430             lea ecx, [esp + 0x30]
0053f6bc 51                   push ecx
0053f6bd 8bcf                 mov ecx, edi
0053f6bf ff905c020000         call dword ptr [eax + 0x25c]
0053f6c5 d9442414             fld dword ptr [esp + 0x14]
0053f6c9 d81ddcde2201         fcomp dword ptr [0x122dedc]
0053f6cf dfe0                 fnstsw ax
0053f6d1 f6c441               test ah, 0x41
0053f6d4 0f850b010000         jne 0x53f7e5
0053f6da 8b8e88010000         mov ecx, dword ptr [esi + 0x188]
0053f6e0 8b11                 mov edx, dword ptr [ecx]
0053f6e2 8d44242c             lea eax, [esp + 0x2c]
0053f6e6 50                   push eax
0053f6e7 ff92b0010000         call dword ptr [edx + 0x1b0]
0053f6ed d9442420             fld dword ptr [esp + 0x20]
0053f6f1 8b08                 mov ecx, dword ptr [eax]
0053f6f3 d88946010000         fmul dword ptr [ecx + 0x146]
0053f6f9 8b44242c             mov eax, dword ptr [esp + 0x2c]
0053f6fd 85c0                 test eax, eax
0053f6ff c744242400000000     mov dword ptr [esp + 0x24], 0
0053f707 d95c2428             fstp dword ptr [esp + 0x28]
0053f70b 7416                 je 0x53f723
0053f70d 8b7804               mov edi, dword ptr [eax + 4]
0053f710 8bc8                 mov ecx, eax
0053f712 83c004               add eax, 4
0053f715 4f                   dec edi
0053f716 8bd7                 mov edx, edi
0053f718 85d2                 test edx, edx
0053f71a 8938                 mov dword ptr [eax], edi
0053f71c 7505                 jne 0x53f723
0053f71e 8b11                 mov edx, dword ptr [ecx]
0053f720 ff5204               call dword ptr [edx + 4]
0053f723 8bbe88010000         mov edi, dword ptr [esi + 0x188]
0053f729 85ff                 test edi, edi
0053f72b c7442418000080bf     mov dword ptr [esp + 0x18], 0xbf800000
0053f733 c744241c000080bf     mov dword ptr [esp + 0x1c], 0xbf800000
0053f73b 0f84a4000000         je 0x53f7e5
0053f741 d944240c             fld dword ptr [esp + 0xc]
0053f745 d81ddcde2201         fcomp dword ptr [0x122dedc]
0053f74b dfe0                 fnstsw ax
0053f74d f6c441               test ah, 0x41
0053f750 7525                 jne 0x53f777
0053f752 8b07                 mov eax, dword ptr [edi]
0053f754 8d4c2424             lea ecx, [esp + 0x24]
0053f758 51                   push ecx
0053f759 8bcf                 mov ecx, edi
0053f75b ff9068020000         call dword ptr [eax + 0x268]
0053f761 d944240c             fld dword ptr [esp + 0xc]
0053f765 8b1f                 mov ebx, dword ptr [edi]
0053f767 e804f36b00           call 0xbfea70
0053f76c 50                   push eax
0053f76d 8bcf                 mov ecx, edi
0053f76f ff9378020000         call dword ptr [ebx + 0x278]
0053f775 eb1b                 jmp 0x53f792
0053f777 8b17                 mov edx, dword ptr [edi]
0053f779 8d442418             lea eax, [esp + 0x18]
0053f77d 50                   push eax
0053f77e 8bcf                 mov ecx, edi
0053f780 ff9268020000         call dword ptr [edx + 0x268]
0053f786 8b17                 mov edx, dword ptr [edi]
0053f788 6a00                 push 0
0053f78a 8bcf                 mov ecx, edi
0053f78c ff9278020000         call dword ptr [edx + 0x278]
0053f792 d9442410             fld dword ptr [esp + 0x10]
0053f796 d81ddcde2201         fcomp dword ptr [0x122dedc]
0053f79c dfe0                 fnstsw ax
0053f79e f6c405               test ah, 5
0053f7a1 7a27                 jp 0x53f7ca
0053f7a3 8b07                 mov eax, dword ptr [edi]
0053f7a5 8d4c2424             lea ecx, [esp + 0x24]
0053f7a9 51                   push ecx
0053f7aa 8bcf                 mov ecx, edi
0053f7ac ff906c020000         call dword ptr [eax + 0x26c]
0053f7b2 d9442410             fld dword ptr [esp + 0x10]
0053f7b6 8b1f                 mov ebx, dword ptr [edi]
0053f7b8 d9e0                 fchs 
0053f7ba e8b1f26b00           call 0xbfea70
0053f7bf 50                   push eax
0053f7c0 8bcf                 mov ecx, edi
0053f7c2 ff937c020000         call dword ptr [ebx + 0x27c]
0053f7c8 eb1b                 jmp 0x53f7e5
0053f7ca 8b17                 mov edx, dword ptr [edi]
0053f7cc 8d442418             lea eax, [esp + 0x18]
0053f7d0 50                   push eax
0053f7d1 8bcf                 mov ecx, edi
0053f7d3 ff926c020000         call dword ptr [edx + 0x26c]
0053f7d9 8b17                 mov edx, dword ptr [edi]
0053f7db 6a00                 push 0
0053f7dd 8bcf                 mov ecx, edi
0053f7df ff927c020000         call dword ptr [edx + 0x27c]
0053f7e5 8b442454             mov eax, dword ptr [esp + 0x54]
0053f7e9 50                   push eax
0053f7ea 8bce                 mov ecx, esi
0053f7ec c6869001000000       mov byte ptr [esi + 0x190], 0
0053f7f3 e8e8cffeff           call 0x52c7e0
0053f7f8 5f                   pop edi
0053f7f9 5e                   pop esi
0053f7fa 5b                   pop ebx
0053f7fb 83c444               add esp, 0x44
0053f7fe c20400               ret 4
