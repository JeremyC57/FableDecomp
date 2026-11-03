00b193d0 53                   push ebx
00b193d1 55                   push ebp
00b193d2 56                   push esi
00b193d3 8b742410             mov esi, dword ptr [esp + 0x10]
00b193d7 8b4604               mov eax, dword ptr [esi + 4]
00b193da 57                   push edi
00b193db 8bf9                 mov edi, ecx
00b193dd 8d4804               lea ecx, [eax + 4]
00b193e0 33c0                 xor eax, eax
00b193e2 7c44                 jl 0xb19428
00b193e4 7f04                 jg 0xb193ea
00b193e6 85c9                 test ecx, ecx
00b193e8 723e                 jb 0xb19428
00b193ea 85c0                 test eax, eax
00b193ec 7f3a                 jg 0xb19428
00b193ee 7c08                 jl 0xb193f8
00b193f0 81f9ffffff7f         cmp ecx, 0x7fffffff
00b193f6 7730                 ja 0xb19428
00b193f8 8b5614               mov edx, dword ptr [esi + 0x14]
00b193fb 83fa04               cmp edx, 4
00b193fe 7c1a                 jl 0xb1941a
00b19400 8b460c               mov eax, dword ptr [esi + 0xc]
00b19403 8b18                 mov ebx, dword ptr [eax]
00b19405 83c004               add eax, 4
00b19408 83c2fc               add edx, -4
00b1940b 895c2414             mov dword ptr [esp + 0x14], ebx
00b1940f 89460c               mov dword ptr [esi + 0xc], eax
00b19412 895614               mov dword ptr [esi + 0x14], edx
00b19415 894e04               mov dword ptr [esi + 4], ecx
00b19418 eb0e                 jmp 0xb19428
00b1941a 6a04                 push 4
00b1941c 8d442418             lea eax, [esp + 0x18]
00b19420 50                   push eax
00b19421 8bce                 mov ecx, esi
00b19423 e878a8e7ff           call 0x993ca0
00b19428 8b4c2414             mov ecx, dword ptr [esp + 0x14]
00b1942c 894f18               mov dword ptr [edi + 0x18], ecx
00b1942f 8b4604               mov eax, dword ptr [esi + 4]
00b19432 8d4804               lea ecx, [eax + 4]
00b19435 33c0                 xor eax, eax
00b19437 7c44                 jl 0xb1947d
00b19439 7f04                 jg 0xb1943f
00b1943b 85c9                 test ecx, ecx
00b1943d 723e                 jb 0xb1947d
00b1943f 85c0                 test eax, eax
00b19441 7f3a                 jg 0xb1947d
00b19443 7c08                 jl 0xb1944d
00b19445 81f9ffffff7f         cmp ecx, 0x7fffffff
00b1944b 7730                 ja 0xb1947d
00b1944d 8b5614               mov edx, dword ptr [esi + 0x14]
00b19450 83fa04               cmp edx, 4
00b19453 7c1a                 jl 0xb1946f
00b19455 8b460c               mov eax, dword ptr [esi + 0xc]
00b19458 8b18                 mov ebx, dword ptr [eax]
00b1945a 83c004               add eax, 4
00b1945d 83c2fc               add edx, -4
00b19460 895c2414             mov dword ptr [esp + 0x14], ebx
00b19464 89460c               mov dword ptr [esi + 0xc], eax
00b19467 895614               mov dword ptr [esi + 0x14], edx
00b1946a 894e04               mov dword ptr [esi + 4], ecx
00b1946d eb0e                 jmp 0xb1947d
00b1946f 6a04                 push 4
00b19471 8d542418             lea edx, [esp + 0x18]
00b19475 52                   push edx
00b19476 8bce                 mov ecx, esi
00b19478 e823a8e7ff           call 0x993ca0
00b1947d 8b442414             mov eax, dword ptr [esp + 0x14]
00b19481 89471c               mov dword ptr [edi + 0x1c], eax
00b19484 8b4604               mov eax, dword ptr [esi + 4]
00b19487 8d4804               lea ecx, [eax + 4]
00b1948a 33c0                 xor eax, eax
00b1948c 7c44                 jl 0xb194d2
00b1948e 7f04                 jg 0xb19494
00b19490 85c9                 test ecx, ecx
00b19492 723e                 jb 0xb194d2
00b19494 85c0                 test eax, eax
00b19496 7f3a                 jg 0xb194d2
00b19498 7c08                 jl 0xb194a2
00b1949a 81f9ffffff7f         cmp ecx, 0x7fffffff
00b194a0 7730                 ja 0xb194d2
00b194a2 8b5614               mov edx, dword ptr [esi + 0x14]
00b194a5 83fa04               cmp edx, 4
00b194a8 7c1a                 jl 0xb194c4
00b194aa 8b460c               mov eax, dword ptr [esi + 0xc]
00b194ad 8b18                 mov ebx, dword ptr [eax]
00b194af 83c004               add eax, 4
00b194b2 83c2fc               add edx, -4
00b194b5 895c2414             mov dword ptr [esp + 0x14], ebx
00b194b9 89460c               mov dword ptr [esi + 0xc], eax
00b194bc 895614               mov dword ptr [esi + 0x14], edx
00b194bf 894e04               mov dword ptr [esi + 4], ecx
00b194c2 eb0e                 jmp 0xb194d2
00b194c4 6a04                 push 4
00b194c6 8d4c2418             lea ecx, [esp + 0x18]
00b194ca 51                   push ecx
00b194cb 8bce                 mov ecx, esi
00b194cd e8cea7e7ff           call 0x993ca0
00b194d2 8b542414             mov edx, dword ptr [esp + 0x14]
00b194d6 895720               mov dword ptr [edi + 0x20], edx
00b194d9 8b4604               mov eax, dword ptr [esi + 4]
00b194dc 8d4804               lea ecx, [eax + 4]
00b194df 33c0                 xor eax, eax
00b194e1 7c44                 jl 0xb19527
00b194e3 7f04                 jg 0xb194e9
00b194e5 85c9                 test ecx, ecx
00b194e7 723e                 jb 0xb19527
00b194e9 85c0                 test eax, eax
00b194eb 7f3a                 jg 0xb19527
00b194ed 7c08                 jl 0xb194f7
00b194ef 81f9ffffff7f         cmp ecx, 0x7fffffff
00b194f5 7730                 ja 0xb19527
00b194f7 8b5614               mov edx, dword ptr [esi + 0x14]
00b194fa 83fa04               cmp edx, 4
00b194fd 7c1a                 jl 0xb19519
00b194ff 8b460c               mov eax, dword ptr [esi + 0xc]
00b19502 8b18                 mov ebx, dword ptr [eax]
00b19504 83c004               add eax, 4
00b19507 83c2fc               add edx, -4
00b1950a 895c2414             mov dword ptr [esp + 0x14], ebx
00b1950e 89460c               mov dword ptr [esi + 0xc], eax
00b19511 895614               mov dword ptr [esi + 0x14], edx
00b19514 894e04               mov dword ptr [esi + 4], ecx
00b19517 eb0e                 jmp 0xb19527
00b19519 6a04                 push 4
00b1951b 8d442418             lea eax, [esp + 0x18]
00b1951f 50                   push eax
00b19520 8bce                 mov ecx, esi
00b19522 e879a7e7ff           call 0x993ca0
00b19527 8b4c2414             mov ecx, dword ptr [esp + 0x14]
00b1952b 894f24               mov dword ptr [edi + 0x24], ecx
00b1952e 8b4604               mov eax, dword ptr [esi + 4]
00b19531 8d4804               lea ecx, [eax + 4]
00b19534 33c0                 xor eax, eax
00b19536 7c44                 jl 0xb1957c
00b19538 7f04                 jg 0xb1953e
00b1953a 85c9                 test ecx, ecx
00b1953c 723e                 jb 0xb1957c
00b1953e 85c0                 test eax, eax
00b19540 7f3a                 jg 0xb1957c
00b19542 7c08                 jl 0xb1954c
00b19544 81f9ffffff7f         cmp ecx, 0x7fffffff
00b1954a 7730                 ja 0xb1957c
00b1954c 8b5614               mov edx, dword ptr [esi + 0x14]
00b1954f 83fa04               cmp edx, 4
00b19552 7c1a                 jl 0xb1956e
00b19554 8b460c               mov eax, dword ptr [esi + 0xc]
00b19557 8b18                 mov ebx, dword ptr [eax]
00b19559 83c004               add eax, 4
00b1955c 83c2fc               add edx, -4
00b1955f 895c2414             mov dword ptr [esp + 0x14], ebx
00b19563 89460c               mov dword ptr [esi + 0xc], eax
00b19566 895614               mov dword ptr [esi + 0x14], edx
00b19569 894e04               mov dword ptr [esi + 4], ecx
00b1956c eb0e                 jmp 0xb1957c
00b1956e 6a04                 push 4
00b19570 8d542418             lea edx, [esp + 0x18]
00b19574 52                   push edx
00b19575 8bce                 mov ecx, esi
00b19577 e824a7e7ff           call 0x993ca0
00b1957c 8b442414             mov eax, dword ptr [esp + 0x14]
00b19580 894728               mov dword ptr [edi + 0x28], eax
00b19583 8b4604               mov eax, dword ptr [esi + 4]
00b19586 8d4804               lea ecx, [eax + 4]
00b19589 33c0                 xor eax, eax
00b1958b 7c44                 jl 0xb195d1
00b1958d 7f04                 jg 0xb19593
00b1958f 85c9                 test ecx, ecx
00b19591 723e                 jb 0xb195d1
00b19593 85c0                 test eax, eax
00b19595 7f3a                 jg 0xb195d1
00b19597 7c08                 jl 0xb195a1
00b19599 81f9ffffff7f         cmp ecx, 0x7fffffff
00b1959f 7730                 ja 0xb195d1
00b195a1 8b5614               mov edx, dword ptr [esi + 0x14]
00b195a4 83fa04               cmp edx, 4
00b195a7 7c1a                 jl 0xb195c3
00b195a9 8b460c               mov eax, dword ptr [esi + 0xc]
00b195ac 8b18                 mov ebx, dword ptr [eax]
00b195ae 83c004               add eax, 4
00b195b1 83c2fc               add edx, -4
00b195b4 895c2414             mov dword ptr [esp + 0x14], ebx
00b195b8 89460c               mov dword ptr [esi + 0xc], eax
00b195bb 895614               mov dword ptr [esi + 0x14], edx
00b195be 894e04               mov dword ptr [esi + 4], ecx
00b195c1 eb0e                 jmp 0xb195d1
00b195c3 6a04                 push 4
00b195c5 8d4c2418             lea ecx, [esp + 0x18]
00b195c9 51                   push ecx
00b195ca 8bce                 mov ecx, esi
00b195cc e8cfa6e7ff           call 0x993ca0
00b195d1 8b542414             mov edx, dword ptr [esp + 0x14]
00b195d5 89572c               mov dword ptr [edi + 0x2c], edx
00b195d8 8b4604               mov eax, dword ptr [esi + 4]
00b195db 8d4804               lea ecx, [eax + 4]
00b195de 33c0                 xor eax, eax
00b195e0 7c44                 jl 0xb19626
00b195e2 7f04                 jg 0xb195e8
00b195e4 85c9                 test ecx, ecx
00b195e6 723e                 jb 0xb19626
00b195e8 85c0                 test eax, eax
00b195ea 7f3a                 jg 0xb19626
00b195ec 7c08                 jl 0xb195f6
00b195ee 81f9ffffff7f         cmp ecx, 0x7fffffff
00b195f4 7730                 ja 0xb19626
00b195f6 8b5614               mov edx, dword ptr [esi + 0x14]
00b195f9 83fa04               cmp edx, 4
00b195fc 7c1a                 jl 0xb19618
00b195fe 8b460c               mov eax, dword ptr [esi + 0xc]
00b19601 8b18                 mov ebx, dword ptr [eax]
00b19603 83c004               add eax, 4
00b19606 83c2fc               add edx, -4
00b19609 895c2414             mov dword ptr [esp + 0x14], ebx
00b1960d 89460c               mov dword ptr [esi + 0xc], eax
00b19610 895614               mov dword ptr [esi + 0x14], edx
00b19613 894e04               mov dword ptr [esi + 4], ecx
00b19616 eb0e                 jmp 0xb19626
00b19618 6a04                 push 4
00b1961a 8d442418             lea eax, [esp + 0x18]
00b1961e 50                   push eax
00b1961f 8bce                 mov ecx, esi
00b19621 e87aa6e7ff           call 0x993ca0
00b19626 8b4c2414             mov ecx, dword ptr [esp + 0x14]
00b1962a 894f30               mov dword ptr [edi + 0x30], ecx
00b1962d 8b4604               mov eax, dword ptr [esi + 4]
00b19630 8d4804               lea ecx, [eax + 4]
00b19633 33c0                 xor eax, eax
00b19635 7c44                 jl 0xb1967b
00b19637 7f04                 jg 0xb1963d
00b19639 85c9                 test ecx, ecx
00b1963b 723e                 jb 0xb1967b
00b1963d 85c0                 test eax, eax
00b1963f 7f3a                 jg 0xb1967b
00b19641 7c08                 jl 0xb1964b
00b19643 81f9ffffff7f         cmp ecx, 0x7fffffff
00b19649 7730                 ja 0xb1967b
00b1964b 8b5614               mov edx, dword ptr [esi + 0x14]
00b1964e 83fa04               cmp edx, 4
00b19651 7c1a                 jl 0xb1966d
00b19653 8b460c               mov eax, dword ptr [esi + 0xc]
00b19656 8b18                 mov ebx, dword ptr [eax]
00b19658 83c004               add eax, 4
00b1965b 83c2fc               add edx, -4
00b1965e 895c2414             mov dword ptr [esp + 0x14], ebx
00b19662 89460c               mov dword ptr [esi + 0xc], eax
00b19665 895614               mov dword ptr [esi + 0x14], edx
00b19668 894e04               mov dword ptr [esi + 4], ecx
00b1966b eb0e                 jmp 0xb1967b
00b1966d 6a04                 push 4
00b1966f 8d542418             lea edx, [esp + 0x18]
00b19673 52                   push edx
00b19674 8bce                 mov ecx, esi
00b19676 e825a6e7ff           call 0x993ca0
00b1967b 8b442414             mov eax, dword ptr [esp + 0x14]
00b1967f 894734               mov dword ptr [edi + 0x34], eax
00b19682 8b4604               mov eax, dword ptr [esi + 4]
00b19685 8d4804               lea ecx, [eax + 4]
00b19688 33c0                 xor eax, eax
00b1968a 7c44                 jl 0xb196d0
00b1968c 7f04                 jg 0xb19692
00b1968e 85c9                 test ecx, ecx
00b19690 723e                 jb 0xb196d0
00b19692 85c0                 test eax, eax
00b19694 7f3a                 jg 0xb196d0
00b19696 7c08                 jl 0xb196a0
00b19698 81f9ffffff7f         cmp ecx, 0x7fffffff
00b1969e 7730                 ja 0xb196d0
00b196a0 8b5614               mov edx, dword ptr [esi + 0x14]
00b196a3 83fa04               cmp edx, 4
00b196a6 7c1a                 jl 0xb196c2
00b196a8 8b460c               mov eax, dword ptr [esi + 0xc]
00b196ab 8b18                 mov ebx, dword ptr [eax]
00b196ad 83c004               add eax, 4
00b196b0 83c2fc               add edx, -4
00b196b3 895c2414             mov dword ptr [esp + 0x14], ebx
00b196b7 89460c               mov dword ptr [esi + 0xc], eax
00b196ba 895614               mov dword ptr [esi + 0x14], edx
00b196bd 894e04               mov dword ptr [esi + 4], ecx
00b196c0 eb0e                 jmp 0xb196d0
00b196c2 6a04                 push 4
00b196c4 8d4c2418             lea ecx, [esp + 0x18]
00b196c8 51                   push ecx
00b196c9 8bce                 mov ecx, esi
00b196cb e8d0a5e7ff           call 0x993ca0
00b196d0 8b542414             mov edx, dword ptr [esp + 0x14]
00b196d4 895738               mov dword ptr [edi + 0x38], edx
00b196d7 8b4604               mov eax, dword ptr [esi + 4]
00b196da 8d4804               lea ecx, [eax + 4]
00b196dd 33c0                 xor eax, eax
00b196df 7c44                 jl 0xb19725
00b196e1 7f04                 jg 0xb196e7
00b196e3 85c9                 test ecx, ecx
00b196e5 723e                 jb 0xb19725
00b196e7 85c0                 test eax, eax
00b196e9 7f3a                 jg 0xb19725
00b196eb 7c08                 jl 0xb196f5
00b196ed 81f9ffffff7f         cmp ecx, 0x7fffffff
00b196f3 7730                 ja 0xb19725
00b196f5 8b5614               mov edx, dword ptr [esi + 0x14]
00b196f8 83fa04               cmp edx, 4
00b196fb 7c1a                 jl 0xb19717
00b196fd 8b460c               mov eax, dword ptr [esi + 0xc]
00b19700 8b18                 mov ebx, dword ptr [eax]
00b19702 83c004               add eax, 4
00b19705 83c2fc               add edx, -4
00b19708 895c2414             mov dword ptr [esp + 0x14], ebx
00b1970c 89460c               mov dword ptr [esi + 0xc], eax
00b1970f 895614               mov dword ptr [esi + 0x14], edx
00b19712 894e04               mov dword ptr [esi + 4], ecx
00b19715 eb0e                 jmp 0xb19725
00b19717 6a04                 push 4
00b19719 8d442418             lea eax, [esp + 0x18]
00b1971d 50                   push eax
00b1971e 8bce                 mov ecx, esi
00b19720 e87ba5e7ff           call 0x993ca0
00b19725 8b4c2414             mov ecx, dword ptr [esp + 0x14]
00b19729 894f3c               mov dword ptr [edi + 0x3c], ecx
00b1972c 8b4604               mov eax, dword ptr [esi + 4]
00b1972f 8d4804               lea ecx, [eax + 4]
00b19732 33c0                 xor eax, eax
00b19734 7c44                 jl 0xb1977a
00b19736 7f04                 jg 0xb1973c
00b19738 85c9                 test ecx, ecx
00b1973a 723e                 jb 0xb1977a
00b1973c 85c0                 test eax, eax
00b1973e 7f3a                 jg 0xb1977a
00b19740 7c08                 jl 0xb1974a
00b19742 81f9ffffff7f         cmp ecx, 0x7fffffff
00b19748 7730                 ja 0xb1977a
00b1974a 8b5614               mov edx, dword ptr [esi + 0x14]
00b1974d 83fa04               cmp edx, 4
00b19750 7c1a                 jl 0xb1976c
00b19752 8b460c               mov eax, dword ptr [esi + 0xc]
00b19755 8b18                 mov ebx, dword ptr [eax]
00b19757 83c004               add eax, 4
00b1975a 83c2fc               add edx, -4
00b1975d 895c2414             mov dword ptr [esp + 0x14], ebx
00b19761 89460c               mov dword ptr [esi + 0xc], eax
00b19764 895614               mov dword ptr [esi + 0x14], edx
00b19767 894e04               mov dword ptr [esi + 4], ecx
00b1976a eb0e                 jmp 0xb1977a
00b1976c 6a04                 push 4
00b1976e 8d542418             lea edx, [esp + 0x18]
00b19772 52                   push edx
00b19773 8bce                 mov ecx, esi
00b19775 e826a5e7ff           call 0x993ca0
00b1977a 8b442414             mov eax, dword ptr [esp + 0x14]
00b1977e 8d4c2414             lea ecx, [esp + 0x14]
00b19782 51                   push ecx
00b19783 8bce                 mov ecx, esi
00b19785 894740               mov dword ptr [edi + 0x40], eax
00b19788 e853aee7ff           call 0x9945e0
00b1978d 8b10                 mov edx, dword ptr [eax]
00b1978f 895744               mov dword ptr [edi + 0x44], edx
00b19792 8b4604               mov eax, dword ptr [esi + 4]
00b19795 8d4801               lea ecx, [eax + 1]
00b19798 33c0                 xor eax, eax
00b1979a 7c40                 jl 0xb197dc
00b1979c 7f04                 jg 0xb197a2
00b1979e 85c9                 test ecx, ecx
00b197a0 723a                 jb 0xb197dc
00b197a2 85c0                 test eax, eax
00b197a4 7f36                 jg 0xb197dc
00b197a6 7c08                 jl 0xb197b0
00b197a8 81f9ffffff7f         cmp ecx, 0x7fffffff
00b197ae 772c                 ja 0xb197dc
00b197b0 8b6e14               mov ebp, dword ptr [esi + 0x14]
00b197b3 83fd01               cmp ebp, 1
00b197b6 7c16                 jl 0xb197ce
00b197b8 8b460c               mov eax, dword ptr [esi + 0xc]
00b197bb 8a10                 mov dl, byte ptr [eax]
00b197bd 40                   inc eax
00b197be 4d                   dec ebp
00b197bf 88542414             mov byte ptr [esp + 0x14], dl
00b197c3 89460c               mov dword ptr [esi + 0xc], eax
00b197c6 896e14               mov dword ptr [esi + 0x14], ebp
00b197c9 894e04               mov dword ptr [esi + 4], ecx
00b197cc eb0e                 jmp 0xb197dc
00b197ce 6a01                 push 1
00b197d0 8d442418             lea eax, [esp + 0x18]
00b197d4 50                   push eax
00b197d5 8bce                 mov ecx, esi
00b197d7 e8c4a4e7ff           call 0x993ca0
00b197dc 8a4c2414             mov cl, byte ptr [esp + 0x14]
00b197e0 8a4748               mov al, byte ptr [edi + 0x48]
00b197e3 84c9                 test cl, cl
00b197e5 0f95c1               setne cl
00b197e8 32c8                 xor cl, al
00b197ea 80e101               and cl, 1
00b197ed 8ad8                 mov bl, al
00b197ef 32d9                 xor bl, cl
00b197f1 885f48               mov byte ptr [edi + 0x48], bl
00b197f4 8b4604               mov eax, dword ptr [esi + 4]
00b197f7 8d4801               lea ecx, [eax + 1]
00b197fa 33c0                 xor eax, eax
00b197fc 7c40                 jl 0xb1983e
00b197fe 7f04                 jg 0xb19804
00b19800 85c9                 test ecx, ecx
00b19802 723a                 jb 0xb1983e
00b19804 85c0                 test eax, eax
00b19806 7f36                 jg 0xb1983e
00b19808 7c08                 jl 0xb19812
00b1980a 81f9ffffff7f         cmp ecx, 0x7fffffff
00b19810 772c                 ja 0xb1983e
00b19812 8b6e14               mov ebp, dword ptr [esi + 0x14]
00b19815 83fd01               cmp ebp, 1
00b19818 7c16                 jl 0xb19830
00b1981a 8b460c               mov eax, dword ptr [esi + 0xc]
00b1981d 8a10                 mov dl, byte ptr [eax]
00b1981f 40                   inc eax
00b19820 4d                   dec ebp
00b19821 88542414             mov byte ptr [esp + 0x14], dl
00b19825 89460c               mov dword ptr [esi + 0xc], eax
00b19828 896e14               mov dword ptr [esi + 0x14], ebp
00b1982b 894e04               mov dword ptr [esi + 4], ecx
00b1982e eb0e                 jmp 0xb1983e
00b19830 6a01                 push 1
00b19832 8d442418             lea eax, [esp + 0x18]
00b19836 50                   push eax
00b19837 8bce                 mov ecx, esi
00b19839 e862a4e7ff           call 0x993ca0
00b1983e 8a5c2414             mov bl, byte ptr [esp + 0x14]
00b19842 8a4748               mov al, byte ptr [edi + 0x48]
00b19845 84db                 test bl, bl
00b19847 0f95c1               setne cl
00b1984a d0e1                 shl cl, 1
00b1984c 32c8                 xor cl, al
00b1984e 80e102               and cl, 2
00b19851 8ad8                 mov bl, al
00b19853 32d9                 xor bl, cl
00b19855 885f48               mov byte ptr [edi + 0x48], bl
00b19858 8b4604               mov eax, dword ptr [esi + 4]
00b1985b 8d4801               lea ecx, [eax + 1]
00b1985e 33c0                 xor eax, eax
00b19860 7c40                 jl 0xb198a2
00b19862 7f04                 jg 0xb19868
00b19864 85c9                 test ecx, ecx
00b19866 723a                 jb 0xb198a2
00b19868 85c0                 test eax, eax
00b1986a 7f36                 jg 0xb198a2
00b1986c 7c08                 jl 0xb19876
00b1986e 81f9ffffff7f         cmp ecx, 0x7fffffff
00b19874 772c                 ja 0xb198a2
00b19876 8b6e14               mov ebp, dword ptr [esi + 0x14]
00b19879 83fd01               cmp ebp, 1
00b1987c 7c16                 jl 0xb19894
00b1987e 8b460c               mov eax, dword ptr [esi + 0xc]
00b19881 8a10                 mov dl, byte ptr [eax]
00b19883 40                   inc eax
00b19884 4d                   dec ebp
00b19885 88542414             mov byte ptr [esp + 0x14], dl
00b19889 89460c               mov dword ptr [esi + 0xc], eax
00b1988c 896e14               mov dword ptr [esi + 0x14], ebp
00b1988f 894e04               mov dword ptr [esi + 4], ecx
00b19892 eb0e                 jmp 0xb198a2
00b19894 6a01                 push 1
00b19896 8d442418             lea eax, [esp + 0x18]
00b1989a 50                   push eax
00b1989b 8bce                 mov ecx, esi
00b1989d e8fea3e7ff           call 0x993ca0
00b198a2 8a5c2414             mov bl, byte ptr [esp + 0x14]
00b198a6 8a4748               mov al, byte ptr [edi + 0x48]
00b198a9 84db                 test bl, bl
00b198ab 0f95c1               setne cl
00b198ae c0e102               shl cl, 2
00b198b1 32c8                 xor cl, al
00b198b3 80e104               and cl, 4
00b198b6 8ad8                 mov bl, al
00b198b8 32d9                 xor bl, cl
00b198ba 885f48               mov byte ptr [edi + 0x48], bl
00b198bd 8b4604               mov eax, dword ptr [esi + 4]
00b198c0 8d4801               lea ecx, [eax + 1]
00b198c3 33c0                 xor eax, eax
00b198c5 7c40                 jl 0xb19907
00b198c7 7f04                 jg 0xb198cd
00b198c9 85c9                 test ecx, ecx
00b198cb 723a                 jb 0xb19907
00b198cd 85c0                 test eax, eax
00b198cf 7f36                 jg 0xb19907
00b198d1 7c08                 jl 0xb198db
00b198d3 81f9ffffff7f         cmp ecx, 0x7fffffff
00b198d9 772c                 ja 0xb19907
00b198db 8b6e14               mov ebp, dword ptr [esi + 0x14]
00b198de 83fd01               cmp ebp, 1
00b198e1 7c16                 jl 0xb198f9
00b198e3 8b460c               mov eax, dword ptr [esi + 0xc]
00b198e6 8a10                 mov dl, byte ptr [eax]
00b198e8 40                   inc eax
00b198e9 4d                   dec ebp
00b198ea 88542414             mov byte ptr [esp + 0x14], dl
00b198ee 89460c               mov dword ptr [esi + 0xc], eax
00b198f1 896e14               mov dword ptr [esi + 0x14], ebp
00b198f4 894e04               mov dword ptr [esi + 4], ecx
00b198f7 eb0e                 jmp 0xb19907
00b198f9 6a01                 push 1
00b198fb 8d442418             lea eax, [esp + 0x18]
00b198ff 50                   push eax
00b19900 8bce                 mov ecx, esi
00b19902 e899a3e7ff           call 0x993ca0
00b19907 8a5c2414             mov bl, byte ptr [esp + 0x14]
00b1990b 8a4748               mov al, byte ptr [edi + 0x48]
00b1990e 84db                 test bl, bl
00b19910 0f95c1               setne cl
00b19913 c0e103               shl cl, 3
00b19916 32c8                 xor cl, al
00b19918 80e108               and cl, 8
00b1991b 8ad8                 mov bl, al
00b1991d 32d9                 xor bl, cl
00b1991f 885f48               mov byte ptr [edi + 0x48], bl
00b19922 8b4604               mov eax, dword ptr [esi + 4]
00b19925 8d4801               lea ecx, [eax + 1]
00b19928 33c0                 xor eax, eax
00b1992a 7c40                 jl 0xb1996c
00b1992c 7f04                 jg 0xb19932
00b1992e 85c9                 test ecx, ecx
00b19930 723a                 jb 0xb1996c
00b19932 85c0                 test eax, eax
00b19934 7f36                 jg 0xb1996c
00b19936 7c08                 jl 0xb19940
00b19938 81f9ffffff7f         cmp ecx, 0x7fffffff
00b1993e 772c                 ja 0xb1996c
00b19940 8b6e14               mov ebp, dword ptr [esi + 0x14]
00b19943 83fd01               cmp ebp, 1
00b19946 7c16                 jl 0xb1995e
00b19948 8b460c               mov eax, dword ptr [esi + 0xc]
00b1994b 8a10                 mov dl, byte ptr [eax]
00b1994d 40                   inc eax
00b1994e 4d                   dec ebp
00b1994f 88542414             mov byte ptr [esp + 0x14], dl
00b19953 89460c               mov dword ptr [esi + 0xc], eax
00b19956 896e14               mov dword ptr [esi + 0x14], ebp
00b19959 894e04               mov dword ptr [esi + 4], ecx
00b1995c eb0e                 jmp 0xb1996c
00b1995e 6a01                 push 1
00b19960 8d442418             lea eax, [esp + 0x18]
00b19964 50                   push eax
00b19965 8bce                 mov ecx, esi
00b19967 e834a3e7ff           call 0x993ca0
00b1996c 8a5c2414             mov bl, byte ptr [esp + 0x14]
00b19970 8a4748               mov al, byte ptr [edi + 0x48]
00b19973 84db                 test bl, bl
00b19975 0f95c1               setne cl
00b19978 c0e104               shl cl, 4
00b1997b 32c8                 xor cl, al
00b1997d 80e110               and cl, 0x10
00b19980 8ad8                 mov bl, al
00b19982 32d9                 xor bl, cl
00b19984 885f48               mov byte ptr [edi + 0x48], bl
00b19987 8b4604               mov eax, dword ptr [esi + 4]
00b1998a 8d4801               lea ecx, [eax + 1]
00b1998d 33c0                 xor eax, eax
00b1998f 7c40                 jl 0xb199d1
00b19991 7f04                 jg 0xb19997
00b19993 85c9                 test ecx, ecx
00b19995 723a                 jb 0xb199d1
00b19997 85c0                 test eax, eax
00b19999 7f36                 jg 0xb199d1
00b1999b 7c08                 jl 0xb199a5
00b1999d 81f9ffffff7f         cmp ecx, 0x7fffffff
00b199a3 772c                 ja 0xb199d1
00b199a5 8b6e14               mov ebp, dword ptr [esi + 0x14]
00b199a8 83fd01               cmp ebp, 1
00b199ab 7c16                 jl 0xb199c3
00b199ad 8b460c               mov eax, dword ptr [esi + 0xc]
00b199b0 8a10                 mov dl, byte ptr [eax]
00b199b2 40                   inc eax
00b199b3 4d                   dec ebp
00b199b4 88542414             mov byte ptr [esp + 0x14], dl
00b199b8 89460c               mov dword ptr [esi + 0xc], eax
00b199bb 896e14               mov dword ptr [esi + 0x14], ebp
00b199be 894e04               mov dword ptr [esi + 4], ecx
00b199c1 eb0e                 jmp 0xb199d1
00b199c3 6a01                 push 1
00b199c5 8d442418             lea eax, [esp + 0x18]
00b199c9 50                   push eax
00b199ca 8bce                 mov ecx, esi
00b199cc e8cfa2e7ff           call 0x993ca0
00b199d1 8a5c2414             mov bl, byte ptr [esp + 0x14]
00b199d5 8a4748               mov al, byte ptr [edi + 0x48]
00b199d8 84db                 test bl, bl
00b199da 0f95c1               setne cl
00b199dd c0e105               shl cl, 5
00b199e0 32c8                 xor cl, al
00b199e2 80e120               and cl, 0x20
00b199e5 8ad8                 mov bl, al
00b199e7 32d9                 xor bl, cl
00b199e9 885f48               mov byte ptr [edi + 0x48], bl
00b199ec 8b4604               mov eax, dword ptr [esi + 4]
00b199ef 8d4801               lea ecx, [eax + 1]
00b199f2 33c0                 xor eax, eax
00b199f4 7c40                 jl 0xb19a36
00b199f6 7f04                 jg 0xb199fc
00b199f8 85c9                 test ecx, ecx
00b199fa 723a                 jb 0xb19a36
00b199fc 85c0                 test eax, eax
00b199fe 7f36                 jg 0xb19a36
00b19a00 7c08                 jl 0xb19a0a
00b19a02 81f9ffffff7f         cmp ecx, 0x7fffffff
00b19a08 772c                 ja 0xb19a36
00b19a0a 8b6e14               mov ebp, dword ptr [esi + 0x14]
00b19a0d 83fd01               cmp ebp, 1
00b19a10 7c16                 jl 0xb19a28
00b19a12 8b460c               mov eax, dword ptr [esi + 0xc]
00b19a15 8a10                 mov dl, byte ptr [eax]
00b19a17 40                   inc eax
00b19a18 4d                   dec ebp
00b19a19 88542414             mov byte ptr [esp + 0x14], dl
00b19a1d 89460c               mov dword ptr [esi + 0xc], eax
00b19a20 896e14               mov dword ptr [esi + 0x14], ebp
00b19a23 894e04               mov dword ptr [esi + 4], ecx
00b19a26 eb0e                 jmp 0xb19a36
00b19a28 6a01                 push 1
00b19a2a 8d442418             lea eax, [esp + 0x18]
00b19a2e 50                   push eax
00b19a2f 8bce                 mov ecx, esi
00b19a31 e86aa2e7ff           call 0x993ca0
00b19a36 8a5c2414             mov bl, byte ptr [esp + 0x14]
00b19a3a 8a4748               mov al, byte ptr [edi + 0x48]
00b19a3d 84db                 test bl, bl
00b19a3f 0f95c1               setne cl
00b19a42 c0e106               shl cl, 6
00b19a45 32c8                 xor cl, al
00b19a47 80e140               and cl, 0x40
00b19a4a 8ad8                 mov bl, al
00b19a4c 32d9                 xor bl, cl
00b19a4e 885f48               mov byte ptr [edi + 0x48], bl
00b19a51 8b4604               mov eax, dword ptr [esi + 4]
00b19a54 8d4801               lea ecx, [eax + 1]
00b19a57 33c0                 xor eax, eax
00b19a59 7c40                 jl 0xb19a9b
00b19a5b 7f04                 jg 0xb19a61
00b19a5d 85c9                 test ecx, ecx
00b19a5f 723a                 jb 0xb19a9b
00b19a61 85c0                 test eax, eax
00b19a63 7f36                 jg 0xb19a9b
00b19a65 7c08                 jl 0xb19a6f
00b19a67 81f9ffffff7f         cmp ecx, 0x7fffffff
00b19a6d 772c                 ja 0xb19a9b
00b19a6f 8b6e14               mov ebp, dword ptr [esi + 0x14]
00b19a72 83fd01               cmp ebp, 1
00b19a75 7c16                 jl 0xb19a8d
00b19a77 8b460c               mov eax, dword ptr [esi + 0xc]
00b19a7a 8a10                 mov dl, byte ptr [eax]
00b19a7c 40                   inc eax
00b19a7d 4d                   dec ebp
00b19a7e 88542414             mov byte ptr [esp + 0x14], dl
00b19a82 89460c               mov dword ptr [esi + 0xc], eax
00b19a85 896e14               mov dword ptr [esi + 0x14], ebp
00b19a88 894e04               mov dword ptr [esi + 4], ecx
00b19a8b eb0e                 jmp 0xb19a9b
00b19a8d 6a01                 push 1
00b19a8f 8d442418             lea eax, [esp + 0x18]
00b19a93 50                   push eax
00b19a94 8bce                 mov ecx, esi
00b19a96 e805a2e7ff           call 0x993ca0
00b19a9b 8a4c2414             mov cl, byte ptr [esp + 0x14]
00b19a9f 8a5748               mov dl, byte ptr [edi + 0x48]
00b19aa2 84c9                 test cl, cl
00b19aa4 0f95c1               setne cl
00b19aa7 c0e107               shl cl, 7
00b19aaa 80e27f               and dl, 0x7f
00b19aad 0aca                 or cl, dl
00b19aaf 884f48               mov byte ptr [edi + 0x48], cl
00b19ab2 8b4604               mov eax, dword ptr [esi + 4]
00b19ab5 8d4801               lea ecx, [eax + 1]
00b19ab8 33c0                 xor eax, eax
00b19aba 7c40                 jl 0xb19afc
00b19abc 7f04                 jg 0xb19ac2
00b19abe 85c9                 test ecx, ecx
00b19ac0 723a                 jb 0xb19afc
00b19ac2 85c0                 test eax, eax
00b19ac4 7f36                 jg 0xb19afc
00b19ac6 7c08                 jl 0xb19ad0
00b19ac8 81f9ffffff7f         cmp ecx, 0x7fffffff
00b19ace 772c                 ja 0xb19afc
00b19ad0 8b6e14               mov ebp, dword ptr [esi + 0x14]
00b19ad3 83fd01               cmp ebp, 1
00b19ad6 7c16                 jl 0xb19aee
00b19ad8 8b460c               mov eax, dword ptr [esi + 0xc]
00b19adb 8a10                 mov dl, byte ptr [eax]
00b19add 40                   inc eax
00b19ade 4d                   dec ebp
00b19adf 88542414             mov byte ptr [esp + 0x14], dl
00b19ae3 89460c               mov dword ptr [esi + 0xc], eax
00b19ae6 896e14               mov dword ptr [esi + 0x14], ebp
00b19ae9 894e04               mov dword ptr [esi + 4], ecx
00b19aec eb0e                 jmp 0xb19afc
00b19aee 6a01                 push 1
00b19af0 8d442418             lea eax, [esp + 0x18]
00b19af4 50                   push eax
00b19af5 8bce                 mov ecx, esi
00b19af7 e8a4a1e7ff           call 0x993ca0
00b19afc 8a4c2414             mov cl, byte ptr [esp + 0x14]
00b19b00 8a4749               mov al, byte ptr [edi + 0x49]
00b19b03 84c9                 test cl, cl
00b19b05 0f95c1               setne cl
00b19b08 32c8                 xor cl, al
00b19b0a 80e101               and cl, 1
00b19b0d 8ad8                 mov bl, al
00b19b0f 32d9                 xor bl, cl
00b19b11 885f49               mov byte ptr [edi + 0x49], bl
00b19b14 8b4604               mov eax, dword ptr [esi + 4]
00b19b17 8d4801               lea ecx, [eax + 1]
00b19b1a 33c0                 xor eax, eax
00b19b1c 7c40                 jl 0xb19b5e
00b19b1e 7f04                 jg 0xb19b24
00b19b20 85c9                 test ecx, ecx
00b19b22 723a                 jb 0xb19b5e
00b19b24 85c0                 test eax, eax
00b19b26 7f36                 jg 0xb19b5e
00b19b28 7c08                 jl 0xb19b32
00b19b2a 81f9ffffff7f         cmp ecx, 0x7fffffff
00b19b30 772c                 ja 0xb19b5e
00b19b32 8b6e14               mov ebp, dword ptr [esi + 0x14]
00b19b35 83fd01               cmp ebp, 1
00b19b38 7c16                 jl 0xb19b50
00b19b3a 8b460c               mov eax, dword ptr [esi + 0xc]
00b19b3d 8a10                 mov dl, byte ptr [eax]
00b19b3f 40                   inc eax
00b19b40 4d                   dec ebp
00b19b41 88542414             mov byte ptr [esp + 0x14], dl
00b19b45 89460c               mov dword ptr [esi + 0xc], eax
00b19b48 896e14               mov dword ptr [esi + 0x14], ebp
00b19b4b 894e04               mov dword ptr [esi + 4], ecx
00b19b4e eb0e                 jmp 0xb19b5e
00b19b50 6a01                 push 1
00b19b52 8d442418             lea eax, [esp + 0x18]
00b19b56 50                   push eax
00b19b57 8bce                 mov ecx, esi
00b19b59 e842a1e7ff           call 0x993ca0
00b19b5e 8a5c2414             mov bl, byte ptr [esp + 0x14]
00b19b62 8a4749               mov al, byte ptr [edi + 0x49]
00b19b65 84db                 test bl, bl
00b19b67 0f95c1               setne cl
00b19b6a d0e1                 shl cl, 1
00b19b6c 32c8                 xor cl, al
00b19b6e 80e102               and cl, 2
00b19b71 8ad8                 mov bl, al
00b19b73 32d9                 xor bl, cl
00b19b75 885f49               mov byte ptr [edi + 0x49], bl
00b19b78 8b4604               mov eax, dword ptr [esi + 4]
00b19b7b 8d4801               lea ecx, [eax + 1]
00b19b7e 33c0                 xor eax, eax
00b19b80 7c40                 jl 0xb19bc2
00b19b82 7f04                 jg 0xb19b88
00b19b84 85c9                 test ecx, ecx
00b19b86 723a                 jb 0xb19bc2
00b19b88 85c0                 test eax, eax
00b19b8a 7f36                 jg 0xb19bc2
00b19b8c 7c08                 jl 0xb19b96
00b19b8e 81f9ffffff7f         cmp ecx, 0x7fffffff
00b19b94 772c                 ja 0xb19bc2
00b19b96 8b6e14               mov ebp, dword ptr [esi + 0x14]
00b19b99 83fd01               cmp ebp, 1
00b19b9c 7c16                 jl 0xb19bb4
00b19b9e 8b460c               mov eax, dword ptr [esi + 0xc]
00b19ba1 8a10                 mov dl, byte ptr [eax]
00b19ba3 40                   inc eax
00b19ba4 4d                   dec ebp
00b19ba5 88542414             mov byte ptr [esp + 0x14], dl
00b19ba9 89460c               mov dword ptr [esi + 0xc], eax
00b19bac 896e14               mov dword ptr [esi + 0x14], ebp
00b19baf 894e04               mov dword ptr [esi + 4], ecx
00b19bb2 eb0e                 jmp 0xb19bc2
00b19bb4 6a01                 push 1
00b19bb6 8d442418             lea eax, [esp + 0x18]
00b19bba 50                   push eax
00b19bbb 8bce                 mov ecx, esi
00b19bbd e8dea0e7ff           call 0x993ca0
00b19bc2 8a442414             mov al, byte ptr [esp + 0x14]
00b19bc6 8a5f49               mov bl, byte ptr [edi + 0x49]
00b19bc9 84c0                 test al, al
00b19bcb 0f95c1               setne cl
00b19bce c0e102               shl cl, 2
00b19bd1 32cb                 xor cl, bl
00b19bd3 80e104               and cl, 4
00b19bd6 8ac3                 mov al, bl
00b19bd8 32c1                 xor al, cl
00b19bda 8d542414             lea edx, [esp + 0x14]
00b19bde 52                   push edx
00b19bdf 8bce                 mov ecx, esi
00b19be1 884749               mov byte ptr [edi + 0x49], al
00b19be4 e8f7a9e7ff           call 0x9945e0
00b19be9 8b00                 mov eax, dword ptr [eax]
00b19beb 8d4c2414             lea ecx, [esp + 0x14]
00b19bef 51                   push ecx
00b19bf0 8bce                 mov ecx, esi
00b19bf2 89474c               mov dword ptr [edi + 0x4c], eax
00b19bf5 e8e6a9e7ff           call 0x9945e0
00b19bfa 8b10                 mov edx, dword ptr [eax]
00b19bfc 8d442414             lea eax, [esp + 0x14]
00b19c00 50                   push eax
00b19c01 8bce                 mov ecx, esi
00b19c03 895750               mov dword ptr [edi + 0x50], edx
00b19c06 e8d5a9e7ff           call 0x9945e0
00b19c0b 8b08                 mov ecx, dword ptr [eax]
00b19c0d 894f54               mov dword ptr [edi + 0x54], ecx
00b19c10 5f                   pop edi
00b19c11 5e                   pop esi
00b19c12 5d                   pop ebp
00b19c13 5b                   pop ebx
00b19c14 c20400               ret 4
