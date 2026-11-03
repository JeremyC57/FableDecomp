00b062a0 8b442408             mov eax, dword ptr [esp + 8]
00b062a4 83ec10               sub esp, 0x10
00b062a7 53                   push ebx
00b062a8 56                   push esi
00b062a9 57                   push edi
00b062aa 8b7c2420             mov edi, dword ptr [esp + 0x20]
00b062ae 50                   push eax
00b062af 57                   push edi
00b062b0 8bf1                 mov esi, ecx
00b062b2 e859c00100           call 0xb22310
00b062b7 d9442424             fld dword ptr [esp + 0x24]
00b062bb d81d9cf52201         fcomp dword ptr [0x122f59c]
00b062c1 dfe0                 fnstsw ax
00b062c3 f6c441               test ah, 0x41
00b062c6 7505                 jne 0xb062cd
00b062c8 8b4734               mov eax, dword ptr [edi + 0x34]
00b062cb eb03                 jmp 0xb062d0
00b062cd 8b4634               mov eax, dword ptr [esi + 0x34]
00b062d0 d9442424             fld dword ptr [esp + 0x24]
00b062d4 894634               mov dword ptr [esi + 0x34], eax
00b062d7 d81d9cf52201         fcomp dword ptr [0x122f59c]
00b062dd dfe0                 fnstsw ax
00b062df f6c441               test ah, 0x41
00b062e2 7505                 jne 0xb062e9
00b062e4 8b476c               mov eax, dword ptr [edi + 0x6c]
00b062e7 eb03                 jmp 0xb062ec
00b062e9 8b466c               mov eax, dword ptr [esi + 0x6c]
00b062ec d9442424             fld dword ptr [esp + 0x24]
00b062f0 89466c               mov dword ptr [esi + 0x6c], eax
00b062f3 d81d9cf52201         fcomp dword ptr [0x122f59c]
00b062f9 dfe0                 fnstsw ax
00b062fb f6c441               test ah, 0x41
00b062fe 7508                 jne 0xb06308
00b06300 8a8793000000         mov al, byte ptr [edi + 0x93]
00b06306 eb06                 jmp 0xb0630e
00b06308 8a8693000000         mov al, byte ptr [esi + 0x93]
00b0630e d905d8de2201         fld dword ptr [0x122ded8]
00b06314 8a9693000000         mov dl, byte ptr [esi + 0x93]
00b0631a d8642424             fsub dword ptr [esp + 0x24]
00b0631e c0e807               shr al, 7
00b06321 c0e007               shl al, 7
00b06324 80e27f               and dl, 0x7f
00b06327 d95c240c             fstp dword ptr [esp + 0xc]
00b0632b 0ad0                 or dl, al
00b0632d d9442424             fld dword ptr [esp + 0x24]
00b06331 889693000000         mov byte ptr [esi + 0x93], dl
00b06337 d84f44               fmul dword ptr [edi + 0x44]
00b0633a d944240c             fld dword ptr [esp + 0xc]
00b0633e d84e44               fmul dword ptr [esi + 0x44]
00b06341 dec1                 faddp st(1)
00b06343 d95e44               fstp dword ptr [esi + 0x44]
00b06346 d9442424             fld dword ptr [esp + 0x24]
00b0634a d81d9cf52201         fcomp dword ptr [0x122f59c]
00b06350 dfe0                 fnstsw ax
00b06352 f6c441               test ah, 0x41
00b06355 7508                 jne 0xb0635f
00b06357 8a8793000000         mov al, byte ptr [edi + 0x93]
00b0635d eb02                 jmp 0xb06361
00b0635f 8ac2                 mov al, dl
00b06361 d9442424             fld dword ptr [esp + 0x24]
00b06365 2401                 and al, 1
00b06367 8aca                 mov cl, dl
00b06369 32c8                 xor cl, al
00b0636b 80e101               and cl, 1
00b0636e 32ca                 xor cl, dl
00b06370 888e93000000         mov byte ptr [esi + 0x93], cl
00b06376 d84f48               fmul dword ptr [edi + 0x48]
00b06379 d944240c             fld dword ptr [esp + 0xc]
00b0637d d84e48               fmul dword ptr [esi + 0x48]
00b06380 dec1                 faddp st(1)
00b06382 d95e48               fstp dword ptr [esi + 0x48]
00b06385 d9442424             fld dword ptr [esp + 0x24]
00b06389 d84f4c               fmul dword ptr [edi + 0x4c]
00b0638c d944240c             fld dword ptr [esp + 0xc]
00b06390 d84e4c               fmul dword ptr [esi + 0x4c]
00b06393 dec1                 faddp st(1)
00b06395 d95e4c               fstp dword ptr [esi + 0x4c]
00b06398 d9442424             fld dword ptr [esp + 0x24]
00b0639c d81d9cf52201         fcomp dword ptr [0x122f59c]
00b063a2 dfe0                 fnstsw ax
00b063a4 f6c441               test ah, 0x41
00b063a7 7505                 jne 0xb063ae
00b063a9 8b4770               mov eax, dword ptr [edi + 0x70]
00b063ac eb03                 jmp 0xb063b1
00b063ae 8b4670               mov eax, dword ptr [esi + 0x70]
00b063b1 d9442424             fld dword ptr [esp + 0x24]
00b063b5 894670               mov dword ptr [esi + 0x70], eax
00b063b8 d81d9cf52201         fcomp dword ptr [0x122f59c]
00b063be dfe0                 fnstsw ax
00b063c0 f6c441               test ah, 0x41
00b063c3 7505                 jne 0xb063ca
00b063c5 8b4774               mov eax, dword ptr [edi + 0x74]
00b063c8 eb03                 jmp 0xb063cd
00b063ca 8b4674               mov eax, dword ptr [esi + 0x74]
00b063cd d9442424             fld dword ptr [esp + 0x24]
00b063d1 894674               mov dword ptr [esi + 0x74], eax
00b063d4 d81d9cf52201         fcomp dword ptr [0x122f59c]
00b063da dfe0                 fnstsw ax
00b063dc f6c441               test ah, 0x41
00b063df 7505                 jne 0xb063e6
00b063e1 8b4778               mov eax, dword ptr [edi + 0x78]
00b063e4 eb03                 jmp 0xb063e9
00b063e6 8b4678               mov eax, dword ptr [esi + 0x78]
00b063e9 d9442424             fld dword ptr [esp + 0x24]
00b063ed 894678               mov dword ptr [esi + 0x78], eax
00b063f0 d81d9cf52201         fcomp dword ptr [0x122f59c]
00b063f6 dfe0                 fnstsw ax
00b063f8 f6c441               test ah, 0x41
00b063fb 7505                 jne 0xb06402
00b063fd 8b477c               mov eax, dword ptr [edi + 0x7c]
00b06400 eb03                 jmp 0xb06405
00b06402 8b467c               mov eax, dword ptr [esi + 0x7c]
00b06405 d9442424             fld dword ptr [esp + 0x24]
00b06409 89467c               mov dword ptr [esi + 0x7c], eax
00b0640c d81d9cf52201         fcomp dword ptr [0x122f59c]
00b06412 dfe0                 fnstsw ax
00b06414 f6c441               test ah, 0x41
00b06417 7508                 jne 0xb06421
00b06419 8a8793000000         mov al, byte ptr [edi + 0x93]
00b0641f eb02                 jmp 0xb06423
00b06421 8ac1                 mov al, cl
00b06423 d9442424             fld dword ptr [esp + 0x24]
00b06427 c0e803               shr al, 3
00b0642a d81d9cf52201         fcomp dword ptr [0x122f59c]
00b06430 2401                 and al, 1
00b06432 c0e003               shl al, 3
00b06435 32c1                 xor al, cl
00b06437 2408                 and al, 8
00b06439 8ad0                 mov dl, al
00b0643b dfe0                 fnstsw ax
00b0643d 32d1                 xor dl, cl
00b0643f f6c441               test ah, 0x41
00b06442 889693000000         mov byte ptr [esi + 0x93], dl
00b06448 7508                 jne 0xb06452
00b0644a 8a8793000000         mov al, byte ptr [edi + 0x93]
00b06450 eb02                 jmp 0xb06454
00b06452 8ac2                 mov al, dl
00b06454 d9442424             fld dword ptr [esp + 0x24]
00b06458 c0e804               shr al, 4
00b0645b d81d9cf52201         fcomp dword ptr [0x122f59c]
00b06461 2401                 and al, 1
00b06463 c0e004               shl al, 4
00b06466 32c2                 xor al, dl
00b06468 2410                 and al, 0x10
00b0646a 8ac8                 mov cl, al
00b0646c dfe0                 fnstsw ax
00b0646e 32ca                 xor cl, dl
00b06470 f6c441               test ah, 0x41
00b06473 888e93000000         mov byte ptr [esi + 0x93], cl
00b06479 7508                 jne 0xb06483
00b0647b 8a8793000000         mov al, byte ptr [edi + 0x93]
00b06481 eb02                 jmp 0xb06485
00b06483 8ac1                 mov al, cl
00b06485 d9442424             fld dword ptr [esp + 0x24]
00b06489 c0e805               shr al, 5
00b0648c d81d9cf52201         fcomp dword ptr [0x122f59c]
00b06492 2401                 and al, 1
00b06494 c0e005               shl al, 5
00b06497 32c1                 xor al, cl
00b06499 2420                 and al, 0x20
00b0649b 8ad0                 mov dl, al
00b0649d dfe0                 fnstsw ax
00b0649f 32d1                 xor dl, cl
00b064a1 f6c441               test ah, 0x41
00b064a4 889693000000         mov byte ptr [esi + 0x93], dl
00b064aa 7505                 jne 0xb064b1
00b064ac 8b4750               mov eax, dword ptr [edi + 0x50]
00b064af eb03                 jmp 0xb064b4
00b064b1 8b4650               mov eax, dword ptr [esi + 0x50]
00b064b4 d9442424             fld dword ptr [esp + 0x24]
00b064b8 894650               mov dword ptr [esi + 0x50], eax
00b064bb d81d9cf52201         fcomp dword ptr [0x122f59c]
00b064c1 dfe0                 fnstsw ax
00b064c3 f6c441               test ah, 0x41
00b064c6 7508                 jne 0xb064d0
00b064c8 8a8793000000         mov al, byte ptr [edi + 0x93]
00b064ce eb02                 jmp 0xb064d2
00b064d0 8ac2                 mov al, dl
00b064d2 c0e802               shr al, 2
00b064d5 2401                 and al, 1
00b064d7 c0e002               shl al, 2
00b064da 32c2                 xor al, dl
00b064dc 2404                 and al, 4
00b064de 32c2                 xor al, dl
00b064e0 8b5654               mov edx, dword ptr [esi + 0x54]
00b064e3 89542420             mov dword ptr [esp + 0x20], edx
00b064e7 db442420             fild dword ptr [esp + 0x20]
00b064eb 888693000000         mov byte ptr [esi + 0x93], al
00b064f1 8b4f54               mov ecx, dword ptr [edi + 0x54]
00b064f4 d84c240c             fmul dword ptr [esp + 0xc]
00b064f8 894c2414             mov dword ptr [esp + 0x14], ecx
00b064fc db442414             fild dword ptr [esp + 0x14]
00b06500 d84c2424             fmul dword ptr [esp + 0x24]
00b06504 dec1                 faddp st(1)
00b06506 d95c2420             fstp dword ptr [esp + 0x20]
00b0650a d9442420             fld dword ptr [esp + 0x20]
00b0650e db5c2414             fistp dword ptr [esp + 0x14]
00b06512 8b442414             mov eax, dword ptr [esp + 0x14]
00b06516 d9442424             fld dword ptr [esp + 0x24]
00b0651a 8b565c               mov edx, dword ptr [esi + 0x5c]
00b0651d 894654               mov dword ptr [esi + 0x54], eax
00b06520 d84f58               fmul dword ptr [edi + 0x58]
00b06523 d944240c             fld dword ptr [esp + 0xc]
00b06527 89542420             mov dword ptr [esp + 0x20], edx
00b0652b d84e58               fmul dword ptr [esi + 0x58]
00b0652e dec1                 faddp st(1)
00b06530 d95e58               fstp dword ptr [esi + 0x58]
00b06533 8b4f5c               mov ecx, dword ptr [edi + 0x5c]
00b06536 db442420             fild dword ptr [esp + 0x20]
00b0653a 894c2414             mov dword ptr [esp + 0x14], ecx
00b0653e d84c240c             fmul dword ptr [esp + 0xc]
00b06542 db442414             fild dword ptr [esp + 0x14]
00b06546 d84c2424             fmul dword ptr [esp + 0x24]
00b0654a dec1                 faddp st(1)
00b0654c d95c2420             fstp dword ptr [esp + 0x20]
00b06550 d9442420             fld dword ptr [esp + 0x20]
00b06554 db5c2414             fistp dword ptr [esp + 0x14]
00b06558 8b5660               mov edx, dword ptr [esi + 0x60]
00b0655b 8b442414             mov eax, dword ptr [esp + 0x14]
00b0655f 89542420             mov dword ptr [esp + 0x20], edx
00b06563 db442420             fild dword ptr [esp + 0x20]
00b06567 89465c               mov dword ptr [esi + 0x5c], eax
00b0656a 8b4f60               mov ecx, dword ptr [edi + 0x60]
00b0656d d84c240c             fmul dword ptr [esp + 0xc]
00b06571 894c2414             mov dword ptr [esp + 0x14], ecx
00b06575 db442414             fild dword ptr [esp + 0x14]
00b06579 d84c2424             fmul dword ptr [esp + 0x24]
00b0657d dec1                 faddp st(1)
00b0657f d95c2420             fstp dword ptr [esp + 0x20]
00b06583 d9442420             fld dword ptr [esp + 0x20]
00b06587 db5c2414             fistp dword ptr [esp + 0x14]
00b0658b d9442424             fld dword ptr [esp + 0x24]
00b0658f 8b442414             mov eax, dword ptr [esp + 0x14]
00b06593 d81d9cf52201         fcomp dword ptr [0x122f59c]
00b06599 894660               mov dword ptr [esi + 0x60], eax
00b0659c dfe0                 fnstsw ax
00b0659e f6c441               test ah, 0x41
00b065a1 7508                 jne 0xb065ab
00b065a3 8a8793000000         mov al, byte ptr [edi + 0x93]
00b065a9 eb06                 jmp 0xb065b1
00b065ab 8a8693000000         mov al, byte ptr [esi + 0x93]
00b065b1 8a9e93000000         mov bl, byte ptr [esi + 0x93]
00b065b7 d9442424             fld dword ptr [esp + 0x24]
00b065bb d0e8                 shr al, 1
00b065bd 2401                 and al, 1
00b065bf d0e0                 shl al, 1
00b065c1 32c3                 xor al, bl
00b065c3 2402                 and al, 2
00b065c5 8acb                 mov cl, bl
00b065c7 32c8                 xor cl, al
00b065c9 888e93000000         mov byte ptr [esi + 0x93], cl
00b065cf d84f64               fmul dword ptr [edi + 0x64]
00b065d2 d944240c             fld dword ptr [esp + 0xc]
00b065d6 d84e64               fmul dword ptr [esi + 0x64]
00b065d9 dec1                 faddp st(1)
00b065db d95e64               fstp dword ptr [esi + 0x64]
00b065de d9442424             fld dword ptr [esp + 0x24]
00b065e2 d81d9cf52201         fcomp dword ptr [0x122f59c]
00b065e8 dfe0                 fnstsw ax
00b065ea f6c441               test ah, 0x41
00b065ed 7508                 jne 0xb065f7
00b065ef 8a8793000000         mov al, byte ptr [edi + 0x93]
00b065f5 eb02                 jmp 0xb065f9
00b065f7 8ac1                 mov al, cl
00b065f9 d9442424             fld dword ptr [esp + 0x24]
00b065fd c0e806               shr al, 6
00b06600 2401                 and al, 1
00b06602 c0e006               shl al, 6
00b06605 32c1                 xor al, cl
00b06607 2440                 and al, 0x40
00b06609 32c1                 xor al, cl
00b0660b 888693000000         mov byte ptr [esi + 0x93], al
00b06611 d84f68               fmul dword ptr [edi + 0x68]
00b06614 d944240c             fld dword ptr [esp + 0xc]
00b06618 d84e68               fmul dword ptr [esi + 0x68]
00b0661b dec1                 faddp st(1)
00b0661d d95e68               fstp dword ptr [esi + 0x68]
00b06620 d9442424             fld dword ptr [esp + 0x24]
00b06624 d80d40b22501         fmul dword ptr [0x125b240]
00b0662a d9542414             fst dword ptr [esp + 0x14]
00b0662e d95c2420             fstp dword ptr [esp + 0x20]
00b06632 d9442420             fld dword ptr [esp + 0x20]
00b06636 db5c2418             fistp dword ptr [esp + 0x18]
00b0663a 8a5f39               mov bl, byte ptr [edi + 0x39]
00b0663d 8b4c2418             mov ecx, dword ptr [esp + 0x18]
00b06641 8a5738               mov dl, byte ptr [edi + 0x38]
00b06644 51                   push ecx
00b06645 51                   push ecx
00b06646 8a4f3b               mov cl, byte ptr [edi + 0x3b]
00b06649 8bc4                 mov eax, esp
00b0664b 885c2428             mov byte ptr [esp + 0x28], bl
00b0664f 8a5f3a               mov bl, byte ptr [edi + 0x3a]
00b06652 885802               mov byte ptr [eax + 2], bl
00b06655 8a5c2428             mov bl, byte ptr [esp + 0x28]
00b06659 885801               mov byte ptr [eax + 1], bl
00b0665c 8810                 mov byte ptr [eax], dl
00b0665e 884803               mov byte ptr [eax + 3], cl
00b06661 8a5e39               mov bl, byte ptr [esi + 0x39]
00b06664 8a5638               mov dl, byte ptr [esi + 0x38]
00b06667 51                   push ecx
00b06668 8a4e3b               mov cl, byte ptr [esi + 0x3b]
00b0666b 8bc4                 mov eax, esp
00b0666d 885c242c             mov byte ptr [esp + 0x2c], bl
00b06671 8a5e3a               mov bl, byte ptr [esi + 0x3a]
00b06674 8810                 mov byte ptr [eax], dl
00b06676 885802               mov byte ptr [eax + 2], bl
00b06679 8a5c242c             mov bl, byte ptr [esp + 0x2c]
00b0667d 8d54241c             lea edx, [esp + 0x1c]
00b06681 52                   push edx
00b06682 885801               mov byte ptr [eax + 1], bl
00b06685 884803               mov byte ptr [eax + 3], cl
00b06688 e8c3dbf3ff           call 0xa44250
00b0668d 8b442420             mov eax, dword ptr [esp + 0x20]
00b06691 8b4c2424             mov ecx, dword ptr [esp + 0x24]
00b06695 83c410               add esp, 0x10
00b06698 894638               mov dword ptr [esi + 0x38], eax
00b0669b 894c2420             mov dword ptr [esp + 0x20], ecx
00b0669f d9442420             fld dword ptr [esp + 0x20]
00b066a3 db5c2418             fistp dword ptr [esp + 0x18]
00b066a7 8a5f3d               mov bl, byte ptr [edi + 0x3d]
00b066aa 8b542418             mov edx, dword ptr [esp + 0x18]
00b066ae 52                   push edx
00b066af 8a573c               mov dl, byte ptr [edi + 0x3c]
00b066b2 51                   push ecx
00b066b3 8a4f3f               mov cl, byte ptr [edi + 0x3f]
00b066b6 8bc4                 mov eax, esp
00b066b8 885c2428             mov byte ptr [esp + 0x28], bl
00b066bc 8a5f3e               mov bl, byte ptr [edi + 0x3e]
00b066bf 885802               mov byte ptr [eax + 2], bl
00b066c2 8a5c2428             mov bl, byte ptr [esp + 0x28]
00b066c6 885801               mov byte ptr [eax + 1], bl
00b066c9 8810                 mov byte ptr [eax], dl
00b066cb 884803               mov byte ptr [eax + 3], cl
00b066ce 8a5e3d               mov bl, byte ptr [esi + 0x3d]
00b066d1 8a563c               mov dl, byte ptr [esi + 0x3c]
00b066d4 51                   push ecx
00b066d5 8a4e3f               mov cl, byte ptr [esi + 0x3f]
00b066d8 8bc4                 mov eax, esp
00b066da 885c242c             mov byte ptr [esp + 0x2c], bl
00b066de 8a5e3e               mov bl, byte ptr [esi + 0x3e]
00b066e1 885802               mov byte ptr [eax + 2], bl
00b066e4 8a5c242c             mov bl, byte ptr [esp + 0x2c]
00b066e8 885801               mov byte ptr [eax + 1], bl
00b066eb 8810                 mov byte ptr [eax], dl
00b066ed 884803               mov byte ptr [eax + 3], cl
00b066f0 8d44241c             lea eax, [esp + 0x1c]
00b066f4 50                   push eax
00b066f5 e856dbf3ff           call 0xa44250
00b066fa 8b4c2420             mov ecx, dword ptr [esp + 0x20]
00b066fe 8b542424             mov edx, dword ptr [esp + 0x24]
00b06702 83c410               add esp, 0x10
00b06705 894e3c               mov dword ptr [esi + 0x3c], ecx
00b06708 89542420             mov dword ptr [esp + 0x20], edx
00b0670c d9442420             fld dword ptr [esp + 0x20]
00b06710 db5c2418             fistp dword ptr [esp + 0x18]
00b06714 8a5f41               mov bl, byte ptr [edi + 0x41]
00b06717 8b442418             mov eax, dword ptr [esp + 0x18]
00b0671b 8a5740               mov dl, byte ptr [edi + 0x40]
00b0671e 50                   push eax
00b0671f 51                   push ecx
00b06720 8a4f43               mov cl, byte ptr [edi + 0x43]
00b06723 8bc4                 mov eax, esp
00b06725 885c2428             mov byte ptr [esp + 0x28], bl
00b06729 8a5f42               mov bl, byte ptr [edi + 0x42]
00b0672c 885802               mov byte ptr [eax + 2], bl
00b0672f 8a5c2428             mov bl, byte ptr [esp + 0x28]
00b06733 885801               mov byte ptr [eax + 1], bl
00b06736 884803               mov byte ptr [eax + 3], cl
00b06739 8810                 mov byte ptr [eax], dl
00b0673b 8a5e41               mov bl, byte ptr [esi + 0x41]
00b0673e 8a5640               mov dl, byte ptr [esi + 0x40]
00b06741 51                   push ecx
00b06742 8a4e43               mov cl, byte ptr [esi + 0x43]
00b06745 8bc4                 mov eax, esp
00b06747 885c242c             mov byte ptr [esp + 0x2c], bl
00b0674b 8a5e42               mov bl, byte ptr [esi + 0x42]
00b0674e 884803               mov byte ptr [eax + 3], cl
00b06751 885802               mov byte ptr [eax + 2], bl
00b06754 8a5c242c             mov bl, byte ptr [esp + 0x2c]
00b06758 8d4c241c             lea ecx, [esp + 0x1c]
00b0675c 51                   push ecx
00b0675d 885801               mov byte ptr [eax + 1], bl
00b06760 8810                 mov byte ptr [eax], dl
00b06762 e8e9daf3ff           call 0xa44250
00b06767 d9442434             fld dword ptr [esp + 0x34]
00b0676b d81d9cf52201         fcomp dword ptr [0x122f59c]
00b06771 8b542420             mov edx, dword ptr [esp + 0x20]
00b06775 83c410               add esp, 0x10
00b06778 895640               mov dword ptr [esi + 0x40], edx
00b0677b dfe0                 fnstsw ax
00b0677d f6c441               test ah, 0x41
00b06780 7508                 jne 0xb0678a
00b06782 8a8790000000         mov al, byte ptr [edi + 0x90]
00b06788 eb06                 jmp 0xb06790
00b0678a 8a8690000000         mov al, byte ptr [esi + 0x90]
00b06790 d9442424             fld dword ptr [esp + 0x24]
00b06794 888690000000         mov byte ptr [esi + 0x90], al
00b0679a d81d9cf52201         fcomp dword ptr [0x122f59c]
00b067a0 dfe0                 fnstsw ax
00b067a2 f6c441               test ah, 0x41
00b067a5 7508                 jne 0xb067af
00b067a7 8a8791000000         mov al, byte ptr [edi + 0x91]
00b067ad eb06                 jmp 0xb067b5
00b067af 8a8691000000         mov al, byte ptr [esi + 0x91]
00b067b5 d9442424             fld dword ptr [esp + 0x24]
00b067b9 888691000000         mov byte ptr [esi + 0x91], al
00b067bf d81d9cf52201         fcomp dword ptr [0x122f59c]
00b067c5 dfe0                 fnstsw ax
00b067c7 f6c441               test ah, 0x41
00b067ca 7508                 jne 0xb067d4
00b067cc 8a8792000000         mov al, byte ptr [edi + 0x92]
00b067d2 eb06                 jmp 0xb067da
00b067d4 8a8692000000         mov al, byte ptr [esi + 0x92]
00b067da d9442424             fld dword ptr [esp + 0x24]
00b067de 888692000000         mov byte ptr [esi + 0x92], al
00b067e4 d81d9cf52201         fcomp dword ptr [0x122f59c]
00b067ea dfe0                 fnstsw ax
00b067ec f6c441               test ah, 0x41
00b067ef 7508                 jne 0xb067f9
00b067f1 8b8780000000         mov eax, dword ptr [edi + 0x80]
00b067f7 eb06                 jmp 0xb067ff
00b067f9 8b8680000000         mov eax, dword ptr [esi + 0x80]
00b067ff d9442424             fld dword ptr [esp + 0x24]
00b06803 898680000000         mov dword ptr [esi + 0x80], eax
00b06809 d88f84000000         fmul dword ptr [edi + 0x84]
00b0680f d944240c             fld dword ptr [esp + 0xc]
00b06813 d88e84000000         fmul dword ptr [esi + 0x84]
00b06819 dec1                 faddp st(1)
00b0681b d99e84000000         fstp dword ptr [esi + 0x84]
00b06821 db8788000000         fild dword ptr [edi + 0x88]
00b06827 d84c2424             fmul dword ptr [esp + 0x24]
00b0682b db8688000000         fild dword ptr [esi + 0x88]
00b06831 d84c240c             fmul dword ptr [esp + 0xc]
00b06835 dec1                 faddp st(1)
00b06837 d95c2420             fstp dword ptr [esp + 0x20]
00b0683b d9442420             fld dword ptr [esp + 0x20]
00b0683f db5c2418             fistp dword ptr [esp + 0x18]
00b06843 d9442424             fld dword ptr [esp + 0x24]
00b06847 8b442418             mov eax, dword ptr [esp + 0x18]
00b0684b d81d9cf52201         fcomp dword ptr [0x122f59c]
00b06851 898688000000         mov dword ptr [esi + 0x88], eax
00b06857 dfe0                 fnstsw ax
00b06859 f6c441               test ah, 0x41
00b0685c 7508                 jne 0xb06866
00b0685e 8a8794000000         mov al, byte ptr [edi + 0x94]
00b06864 eb06                 jmp 0xb0686c
00b06866 8a8694000000         mov al, byte ptr [esi + 0x94]
00b0686c 8a8e94000000         mov cl, byte ptr [esi + 0x94]
00b06872 d9442424             fld dword ptr [esp + 0x24]
00b06876 d81d9cf52201         fcomp dword ptr [0x122f59c]
00b0687c 2401                 and al, 1
00b0687e 32c8                 xor cl, al
00b06880 8a8694000000         mov al, byte ptr [esi + 0x94]
00b06886 80e101               and cl, 1
00b06889 32c1                 xor al, cl
00b0688b 888694000000         mov byte ptr [esi + 0x94], al
00b06891 dfe0                 fnstsw ax
00b06893 f6c441               test ah, 0x41
00b06896 7508                 jne 0xb068a0
00b06898 8a8795000000         mov al, byte ptr [edi + 0x95]
00b0689e eb06                 jmp 0xb068a6
00b068a0 8a8695000000         mov al, byte ptr [esi + 0x95]
00b068a6 d9442424             fld dword ptr [esp + 0x24]
00b068aa 888695000000         mov byte ptr [esi + 0x95], al
00b068b0 d81d9cf52201         fcomp dword ptr [0x122f59c]
00b068b6 dfe0                 fnstsw ax
00b068b8 f6c441               test ah, 0x41
00b068bb 7515                 jne 0xb068d2
00b068bd 8a8796000000         mov al, byte ptr [edi + 0x96]
00b068c3 888696000000         mov byte ptr [esi + 0x96], al
00b068c9 5f                   pop edi
00b068ca 5e                   pop esi
00b068cb 5b                   pop ebx
00b068cc 83c410               add esp, 0x10
00b068cf c20800               ret 8
00b068d2 8a8696000000         mov al, byte ptr [esi + 0x96]
00b068d8 5f                   pop edi
00b068d9 888696000000         mov byte ptr [esi + 0x96], al
00b068df 5e                   pop esi
00b068e0 5b                   pop ebx
00b068e1 83c410               add esp, 0x10
00b068e4 c20800               ret 8
