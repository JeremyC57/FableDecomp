00b77370 CEngineWaterRenderer::RenderSeaReflectiveForeground DIFFER(2798v2734) flags= []
@@ retail 1-2 built 1-2
   00b77370  a108b53c01             mov eax, dword ptr [0x13cb508]
 - 00b77375  83ec44                 sub esp, 0x44
 + 00b77375  83ec48                 sub esp, 0x48
   00b77378  53                     push ebx
   00b77379  8d90243a0000           lea edx, [eax + 0x3a24]
@@ retail 120-121 built 120-121
   00b774d0  8b591c                 mov ebx, dword ptr [ecx + 0x1c]
   00b774d3  85db                   test ebx, ebx
 - 00b774d5  895c2410               mov dword ptr [esp + 0x10], ebx
 + 00b774d5  895c2414               mov dword ptr [esp + 0x14], ebx
   00b774d9  7402                   je 0xb774dd
   00b774db  ff03                   inc dword ptr [ebx]
@@ retail 252-254 built 252-255
   00b77658  e82368e8ff             call 0x9fde80
   00b7765d  8b7608                 mov esi, dword ptr [esi + 8]
 - 00b77660  8b5e1c                 mov ebx, dword ptr [esi + 0x1c]
 - 00b77663  85db                   test ebx, ebx
 + 00b77660  8b761c                 mov esi, dword ptr [esi + 0x1c]
 + 00b77663  85f6                   test esi, esi
 + 00b77665  89742410               mov dword ptr [esp + 0x10], esi
   00b77665  7402                   je 0xb77669
   00b77667  ff03                   inc dword ptr [ebx]
@@ retail 255-256 built 256-257
   00b77663  85db                   test ebx, ebx
   00b77665  7402                   je 0xb77669
 - 00b77667  ff03                   inc dword ptr [ebx]
 + 00b7766b  ff06                   inc dword ptr [esi]
   00b77669  8b8de8010000           mov ecx, dword ptr [ebp + 0x1e8]
   00b7766f  85c9                   test ecx, ecx
@@ retail 263-264 built 264-269
   00b7767e  8b09                   mov ecx, dword ptr [ecx]
   00b77680  e9a8000000             jmp 0xb7772d
 - 00b77685  8b591c                 mov ebx, dword ptr [ecx + 0x1c]
 + 00b77689  8b491c                 mov ecx, dword ptr [ecx + 0x1c]
 + 00b7768c  85c9                   test ecx, ecx
 + 00b7768e  894c2410               mov dword ptr [esp + 0x10], ecx
 + 00b77692  74d9                   je 0xb7766d
 + 00b77694  ff01                   inc dword ptr [ecx]
   00b77688  ebd9                   jmp 0xb77663
   00b7768a  6a00                   push 0
@@ retail 265-265 built 270-291
   00b77685  8b591c                 mov ebx, dword ptr [ecx + 0x1c]
   00b77688  ebd9                   jmp 0xb77663
 + 00b77698  6a44                   push 0x44
 + 00b7769a  e800000000             call 0xb7769f
 + 00b7769f  83c404                 add esp, 4
 + 00b776a2  85c0                   test eax, eax
 + 00b776a4  740c                   je 0xb776b2
 + 00b776a6  56                     push esi
 + 00b776a7  8bc8                   mov ecx, eax
 + 00b776a9  e800000000             call 0xb776ae
 + 00b776ae  8bf8                   mov edi, eax
 + 00b776b0  eb02                   jmp 0xb776b4
 + 00b776b2  33ff                   xor edi, edi
 + 00b776b4  8b4e08                 mov ecx, dword ptr [esi + 8]
 + 00b776b7  85c9                   test ecx, ecx
 + 00b776b9  7411                   je 0xb776cc
 + 00b776bb  ff4904                 dec dword ptr [ecx + 4]
 + 00b776be  7505                   jne 0xb776c5
 + 00b776c0  8b01                   mov eax, dword ptr [ecx]
 + 00b776c2  ff5004                 call dword ptr [eax + 4]
 + 00b776c5  c7460800000000         mov dword ptr [esi + 8], 0
 + 00b776cc  897e08                 mov dword ptr [esi + 8], edi
 + 00b776cf  8bcf                   mov ecx, edi
   00b7768a  6a00                   push 0
   00b7768c  6a00                   push 0
@@ retail 268-270 built 294-296
   00b7768c  6a00                   push 0
   00b7768e  e8ed67e8ff             call 0x9fde80
 - 00b77693  33ed                   xor ebp, ebp
 - 00b77695  3bdd                   cmp ebx, ebp
 + 00b776da  33f6                   xor esi, esi
 + 00b776dc  3bde                   cmp ebx, esi
   00b77697  0f844e070000           je 0xb77deb
   00b7769d  ff0b                   dec dword ptr [ebx]
@@ retail 278-279 built 304-305
   00b776ad  e8ee54e8ff             call 0x9fcba0
   00b776b2  8b430c                 mov eax, dword ptr [ebx + 0xc]
 - 00b776b5  3bc5                   cmp eax, ebp
 + 00b776fc  3bc6                   cmp eax, esi
   00b776b7  741b                   je 0xb776d4
   00b776b9  ff08                   dec dword ptr [eax]
@@ retail 282-283 built 308-309
   00b776b9  ff08                   dec dword ptr [eax]
   00b776bb  8b430c                 mov eax, dword ptr [ebx + 0xc]
 - 00b776be  3928                   cmp dword ptr [eax], ebp
 + 00b77705  3930                   cmp dword ptr [eax], esi
   00b776c0  7512                   jne 0xb776d4
   00b776c2  8b4808                 mov ecx, dword ptr [eax + 8]
@@ retail 286-288 built 312-314
   00b776c2  8b4808                 mov ecx, dword ptr [eax + 8]
   00b776c5  ff5004                 call dword ptr [eax + 4]
 - 00b776c8  8b530c                 mov edx, dword ptr [ebx + 0xc]
 - 00b776cb  52                     push edx
 + 00b7770f  8b4b0c                 mov ecx, dword ptr [ebx + 0xc]
 + 00b77712  51                     push ecx
   00b776cc  e8eb720800             call 0xbfe9bc
   00b776d1  83c404                 add esp, 4
@@ retail 291-293 built 317-319
   00b776d1  83c404                 add esp, 4
   00b776d4  53                     push ebx
 - 00b776d5  896b08                 mov dword ptr [ebx + 8], ebp
 - 00b776d8  896b0c                 mov dword ptr [ebx + 0xc], ebp
 + 00b7771c  897308                 mov dword ptr [ebx + 8], esi
 + 00b7771f  89730c                 mov dword ptr [ebx + 0xc], esi
   00b776db  e8dc720800             call 0xbfe9bc
   00b776e0  83c404                 add esp, 4
@@ retail 295-321 built 321-321
   00b776db  e8dc720800             call 0xbfe9bc
   00b776e0  83c404                 add esp, 4
 - 00b776e3  e903070000             jmp 0xb77deb
 - 00b776e8  6a44                   push 0x44
 - 00b776ea  e82b730800             call 0xbfea1a
 - 00b776ef  33ed                   xor ebp, ebp
 - 00b776f1  83c404                 add esp, 4
 - 00b776f4  3bc5                   cmp eax, ebp
 - 00b776f6  740c                   je 0xb77704
 - 00b776f8  56                     push esi
 - 00b776f9  8bc8                   mov ecx, eax
 - 00b776fb  e8806ce8ff             call 0x9fe380
 - 00b77700  8bf8                   mov edi, eax
 - 00b77702  eb02                   jmp 0xb77706
 - 00b77704  33ff                   xor edi, edi
 - 00b77706  8b4e08                 mov ecx, dword ptr [esi + 8]
 - 00b77709  3bcd                   cmp ecx, ebp
 - 00b7770b  740d                   je 0xb7771a
 - 00b7770d  ff4904                 dec dword ptr [ecx + 4]
 - 00b77710  7505                   jne 0xb77717
 - 00b77712  8b01                   mov eax, dword ptr [ecx]
 - 00b77714  ff5004                 call dword ptr [eax + 4]
 - 00b77717  896e08                 mov dword ptr [esi + 8], ebp
 - 00b7771a  55                     push ebp
 - 00b7771b  55                     push ebp
 - 00b7771c  8bcf                   mov ecx, edi
 - 00b7771e  897e08                 mov dword ptr [esi + 8], edi
 - 00b77721  e85a67e8ff             call 0x9fde80
   00b77726  e96affffff             jmp 0xb77695
   00b7772b  33c9                   xor ecx, ecx
@@ retail 362-364 built 362-363
   00b777b2  8996b83c0000           mov dword ptr [esi + 0x3cb8], edx
   00b777b8  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00b777bd  8b4c2410               mov ecx, dword ptr [esp + 0x10]
 - 00b777c1  8b4908                 mov ecx, dword ptr [ecx + 8]
 + 00b777c1  8b4b08                 mov ecx, dword ptr [ebx + 8]
   00b777c4  85c9                   test ecx, ecx
   00b777c6  8bf0                   mov esi, eax
@@ retail 392-392 built 391-392
   00b77822  8bc8                   mov ecx, eax
   00b77824  7455                   je 0xb7787b
 + 00b77826  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00b77826  8b7208                 mov esi, dword ptr [edx + 8]
   00b77829  8b8014280000           mov eax, dword ptr [eax + 0x2814]
@@ retail 393-394 built 393-393
   00b77824  7455                   je 0xb7787b
   00b77826  8b7208                 mov esi, dword ptr [edx + 8]
 - 00b77829  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00b7782f  85c6                   test esi, eax
   00b77831  7522                   jne 0xb77855
@@ retail 419-419 built 418-419
   00b7788f  8bc8                   mov ecx, eax
   00b77891  7455                   je 0xb778e8
 + 00b77893  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00b77893  8b7208                 mov esi, dword ptr [edx + 8]
   00b77896  8b8014280000           mov eax, dword ptr [eax + 0x2814]
@@ retail 420-421 built 420-420
   00b77891  7455                   je 0xb778e8
   00b77893  8b7208                 mov esi, dword ptr [edx + 8]
 - 00b77896  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00b7789c  85c6                   test esi, eax
   00b7789e  7522                   jne 0xb778c2
@@ retail 440-442 built 439-442
   00b778dd  ff810c280000           inc dword ptr [ecx + 0x280c]
   00b778e3  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00b778e8  8b35cc6d3901           mov esi, dword ptr [0x1396dcc]
 - 00b778ee  39b0102f0000           cmp dword ptr [eax + 0x2f10], esi
 + 00b778e8  8b3504000000           mov esi, dword ptr [4]
 + 00b778ee  8b98102f0000           mov ebx, dword ptr [eax + 0x2f10]
 + 00b778f4  3bde                   cmp ebx, esi
   00b778f4  8d900c2f0000           lea edx, [eax + 0x2f0c]
   00b778fa  8bc8                   mov ecx, eax
@@ retail 446-446 built 446-447
   00b778fc  8bfe                   mov edi, esi
   00b778fe  7455                   je 0xb77955
 + 00b77902  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00b77900  8b7208                 mov esi, dword ptr [edx + 8]
   00b77903  8b8014280000           mov eax, dword ptr [eax + 0x2814]
@@ retail 447-448 built 448-448
   00b778fe  7455                   je 0xb77955
   00b77900  8b7208                 mov esi, dword ptr [edx + 8]
 - 00b77903  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00b77909  85c6                   test esi, eax
   00b7790b  7522                   jne 0xb7792f
@@ retail 467-469 built 467-470
   00b7794a  ff810c280000           inc dword ptr [ecx + 0x280c]
   00b77950  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00b77955  8b35d06d3901           mov esi, dword ptr [0x1396dd0]
 - 00b7795b  39b0b02f0000           cmp dword ptr [eax + 0x2fb0], esi
 + 00b77957  8b3508000000           mov esi, dword ptr [8]
 + 00b7795d  8b98b02f0000           mov ebx, dword ptr [eax + 0x2fb0]
 + 00b77963  3bde                   cmp ebx, esi
   00b77961  8d90ac2f0000           lea edx, [eax + 0x2fac]
   00b77967  8bc8                   mov ecx, eax
@@ retail 473-473 built 474-475
   00b77969  8bfe                   mov edi, esi
   00b7796b  7455                   je 0xb779c2
 + 00b77971  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00b7796d  8b7208                 mov esi, dword ptr [edx + 8]
   00b77970  8b8014280000           mov eax, dword ptr [eax + 0x2814]
@@ retail 474-475 built 476-476
   00b7796b  7455                   je 0xb779c2
   00b7796d  8b7208                 mov esi, dword ptr [edx + 8]
 - 00b77970  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00b77976  85c6                   test esi, eax
   00b77978  7522                   jne 0xb7799c
@@ retail 500-500 built 501-502
   00b779d6  8bc8                   mov ecx, eax
   00b779d8  7455                   je 0xb77a2f
 + 00b779de  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00b779da  8b7208                 mov esi, dword ptr [edx + 8]
   00b779dd  8b8014280000           mov eax, dword ptr [eax + 0x2814]
@@ retail 501-503 built 503-504
   00b779d8  7455                   je 0xb77a2f
   00b779da  8b7208                 mov esi, dword ptr [edx + 8]
 - 00b779dd  8b8014280000           mov eax, dword ptr [eax + 0x2814]
 - 00b779e3  85c6                   test esi, eax
 + 00b779e7  85f0                   test eax, esi
   00b779e5  7522                   jne 0xb77a09
   00b779e7  0bc6                   or eax, esi
@@ retail 504-506 built 505-507
   00b779e3  85c6                   test esi, eax
   00b779e5  7522                   jne 0xb77a09
 - 00b779e7  0bc6                   or eax, esi
 - 00b779e9  894208                 mov dword ptr [edx + 8], eax
 + 00b779eb  0bf0                   or esi, eax
 + 00b779ed  897208                 mov dword ptr [edx + 8], esi
   00b779ec  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
   00b779f2  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
@@ retail 527-527 built 528-529
   00b77a43  8bd0                   mov edx, eax
   00b77a45  7450                   je 0xb77a97
 + 00b77a4b  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00b77a47  8b7108                 mov esi, dword ptr [ecx + 8]
   00b77a4a  8b8014280000           mov eax, dword ptr [eax + 0x2814]
@@ retail 528-530 built 530-531
   00b77a45  7450                   je 0xb77a97
   00b77a47  8b7108                 mov esi, dword ptr [ecx + 8]
 - 00b77a4a  8b8014280000           mov eax, dword ptr [eax + 0x2814]
 - 00b77a50  85c6                   test esi, eax
 + 00b77a54  85f0                   test eax, esi
   00b77a52  7522                   jne 0xb77a76
   00b77a54  0bc6                   or eax, esi
@@ retail 531-533 built 532-534
   00b77a50  85c6                   test esi, eax
   00b77a52  7522                   jne 0xb77a76
 - 00b77a54  0bc6                   or eax, esi
 - 00b77a56  894108                 mov dword ptr [ecx + 8], eax
 + 00b77a58  0bf0                   or esi, eax
 + 00b77a5a  897108                 mov dword ptr [ecx + 8], esi
   00b77a59  8b8208280000           mov eax, dword ptr [edx + 0x2808]
   00b77a5f  894cc208               mov dword ptr [edx + eax*8 + 8], ecx
@@ retail 547-549 built 548-552
   00b77a8a  898c8208200000         mov dword ptr [edx + eax*4 + 0x2008], ecx
   00b77a91  ff820c280000           inc dword ptr [edx + 0x280c]
 - 00b77a97  33f6                   xor esi, esi
 - 00b77a99  3bde                   cmp ebx, esi
 + 00b77a9b  8b742410               mov esi, dword ptr [esp + 0x10]
 + 00b77a9f  33ff                   xor edi, edi
 + 00b77aa1  3bf7                   cmp esi, edi
 + 00b77aa3  b310                   mov bl, 0x10
   00b77a9b  7442                   je 0xb77adf
   00b77a9d  ff0b                   dec dword ptr [ebx]
@@ retail 550-551 built 553-554
   00b77a99  3bde                   cmp ebx, esi
   00b77a9b  7442                   je 0xb77adf
 - 00b77a9d  ff0b                   dec dword ptr [ebx]
 + 00b77aa7  ff0e                   dec dword ptr [esi]
   00b77a9f  753e                   jne 0xb77adf
   00b77aa1  f6431a10               test byte ptr [ebx + 0x1a], 0x10
@@ retail 552-553 built 555-556
   00b77a9d  ff0b                   dec dword ptr [ebx]
   00b77a9f  753e                   jne 0xb77adf
 - 00b77aa1  f6431a10               test byte ptr [ebx + 0x1a], 0x10
 + 00b77aab  845e1a                 test byte ptr [esi + 0x1a], bl
   00b77aa5  7407                   je 0xb77aae
   00b77aa7  8bcb                   mov ecx, ebx
@@ retail 554-555 built 557-558
   00b77aa1  f6431a10               test byte ptr [ebx + 0x1a], 0x10
   00b77aa5  7407                   je 0xb77aae
 - 00b77aa7  8bcb                   mov ecx, ebx
 + 00b77ab0  8bce                   mov ecx, esi
   00b77aa9  e8f250e8ff             call 0x9fcba0
   00b77aae  8b430c                 mov eax, dword ptr [ebx + 0xc]
@@ retail 556-558 built 559-561
   00b77aa7  8bcb                   mov ecx, ebx
   00b77aa9  e8f250e8ff             call 0x9fcba0
 - 00b77aae  8b430c                 mov eax, dword ptr [ebx + 0xc]
 - 00b77ab1  3bc6                   cmp eax, esi
 + 00b77ab7  8b460c                 mov eax, dword ptr [esi + 0xc]
 + 00b77aba  3bc7                   cmp eax, edi
   00b77ab3  741b                   je 0xb77ad0
   00b77ab5  ff08                   dec dword ptr [eax]
@@ retail 560-562 built 563-565
   00b77ab3  741b                   je 0xb77ad0
   00b77ab5  ff08                   dec dword ptr [eax]
 - 00b77ab7  8b430c                 mov eax, dword ptr [ebx + 0xc]
 - 00b77aba  3930                   cmp dword ptr [eax], esi
 + 00b77ac0  8b460c                 mov eax, dword ptr [esi + 0xc]
 + 00b77ac3  3938                   cmp dword ptr [eax], edi
   00b77abc  7512                   jne 0xb77ad0
   00b77abe  8b4808                 mov ecx, dword ptr [eax + 8]
@@ retail 565-566 built 568-569
   00b77abe  8b4808                 mov ecx, dword ptr [eax + 8]
   00b77ac1  ff5004                 call dword ptr [eax + 4]
 - 00b77ac4  8b4b0c                 mov ecx, dword ptr [ebx + 0xc]
 + 00b77acd  8b4e0c                 mov ecx, dword ptr [esi + 0xc]
   00b77ac7  51                     push ecx
   00b77ac8  e8ef6e0800             call 0xbfe9bc
@@ retail 569-572 built 572-575
   00b77ac8  e8ef6e0800             call 0xbfe9bc
   00b77acd  83c404                 add esp, 4
 - 00b77ad0  53                     push ebx
 - 00b77ad1  897308                 mov dword ptr [ebx + 8], esi
 - 00b77ad4  89730c                 mov dword ptr [ebx + 0xc], esi
 + 00b77ad9  56                     push esi
 + 00b77ada  897e08                 mov dword ptr [esi + 8], edi
 + 00b77add  897e0c                 mov dword ptr [esi + 0xc], edi
   00b77ad7  e8e06e0800             call 0xbfe9bc
   00b77adc  83c404                 add esp, 4
@@ retail 574-576 built 577-579
   00b77ad7  e8e06e0800             call 0xbfe9bc
   00b77adc  83c404                 add esp, 4
 - 00b77adf  8b4c2410               mov ecx, dword ptr [esp + 0x10]
 - 00b77ae3  ff09                   dec dword ptr [ecx]
 + 00b77ae8  8b742414               mov esi, dword ptr [esp + 0x14]
 + 00b77aec  ff0e                   dec dword ptr [esi]
   00b77ae5  7507                   jne 0xb77aee
   00b77ae7  6a01                   push 1
@@ retail 577-578 built 580-583
   00b77ae3  ff09                   dec dword ptr [ecx]
   00b77ae5  7507                   jne 0xb77aee
 - 00b77ae7  6a01                   push 1
 + 00b77af0  845e1a                 test byte ptr [esi + 0x1a], bl
 + 00b77af3  7407                   je 0xb77afc
 + 00b77af5  8bce                   mov ecx, esi
   00b77ae9  e802438aff             call 0x41bdf0
   00b77aee  d985dc020000           fld dword ptr [ebp + 0x2dc]
@@ retail 579-579 built 584-602
   00b77ae7  6a01                   push 1
   00b77ae9  e802438aff             call 0x41bdf0
 + 00b77afc  8b460c                 mov eax, dword ptr [esi + 0xc]
 + 00b77aff  3bc7                   cmp eax, edi
 + 00b77b01  741b                   je 0xb77b1e
 + 00b77b03  ff08                   dec dword ptr [eax]
 + 00b77b05  8b460c                 mov eax, dword ptr [esi + 0xc]
 + 00b77b08  3938                   cmp dword ptr [eax], edi
 + 00b77b0a  7512                   jne 0xb77b1e
 + 00b77b0c  8b4808                 mov ecx, dword ptr [eax + 8]
 + 00b77b0f  ff5004                 call dword ptr [eax + 4]
 + 00b77b12  8b560c                 mov edx, dword ptr [esi + 0xc]
 + 00b77b15  52                     push edx
 + 00b77b16  e800000000             call 0xb77b1b
 + 00b77b1b  83c404                 add esp, 4
 + 00b77b1e  56                     push esi
 + 00b77b1f  897e08                 mov dword ptr [esi + 8], edi
 + 00b77b22  897e0c                 mov dword ptr [esi + 0xc], edi
 + 00b77b25  e800000000             call 0xb77b2a
 + 00b77b2a  83c404                 add esp, 4
   00b77aee  d985dc020000           fld dword ptr [ebp + 0x2dc]
   00b77af4  56                     push esi
@@ retail 580-581 built 603-604
   00b77ae9  e802438aff             call 0x41bdf0
   00b77aee  d985dc020000           fld dword ptr [ebp + 0x2dc]
 - 00b77af4  56                     push esi
 + 00b77b33  57                     push edi
   00b77af5  d88dc0050000           fmul dword ptr [ebp + 0x5c0]
   00b77afb  680000803f             push 0x3f800000
@@ retail 583-584 built 606-607
   00b77af5  d88dc0050000           fmul dword ptr [ebp + 0x5c0]
   00b77afb  680000803f             push 0x3f800000
 - 00b77b00  56                     push esi
 + 00b77b3f  57                     push edi
   00b77b01  51                     push ecx
   00b77b02  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
@@ retail 590-591 built 613-614
   00b77b0d  e8ee1fe1ff             call 0x989b00
   00b77b12  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
 - 00b77b18  56                     push esi
 + 00b77b57  57                     push edi
   00b77b19  8d85cc000000           lea eax, [ebp + 0xcc]
   00b77b1f  50                     push eax
@@ retail 600-601 built 623-624
   00b77b37  8b0d186e4301           mov ecx, dword ptr [0x1436e18]
   00b77b3d  8d450c                 lea eax, [ebp + 0xc]
 - 00b77b40  3bc6                   cmp eax, esi
 + 00b77b7f  3bc7                   cmp eax, edi
   00b77b42  8981f43c0000           mov dword ptr [ecx + 0x3cf4], eax
   00b77b48  8b89f03c0000           mov ecx, dword ptr [ecx + 0x3cf0]
@@ retail 608-609 built 631-632
   00b77b55  50                     push eax
   00b77b56  eb01                   jmp 0xb77b59
 - 00b77b58  56                     push esi
 + 00b77b97  57                     push edi
   00b77b59  51                     push ecx
   00b77b5a  ff92a0010000           call dword ptr [edx + 0x1a0]
@@ retail 613-614 built 636-637
   00b77b60  8a858e020000           mov al, byte ptr [ebp + 0x28e]
   00b77b66  84c0                   test al, al
 - 00b77b68  56                     push esi
 + 00b77ba7  57                     push edi
   00b77b69  7417                   je 0xb77b82
   00b77b6b  8b8508030000           mov eax, dword ptr [ebp + 0x308]
@@ retail 622-625 built 645-648
   00b77b7f  52                     push edx
   00b77b80  eb03                   jmp 0xb77b85
 - 00b77b82  56                     push esi
 - 00b77b83  56                     push esi
 - 00b77b84  56                     push esi
 + 00b77bc1  57                     push edi
 + 00b77bc2  57                     push edi
 + 00b77bc3  57                     push edi
   00b77b85  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
   00b77b8b  56                     push esi
@@ retail 626-627 built 649-650
   00b77b84  56                     push esi
   00b77b85  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
 - 00b77b8b  56                     push esi
 + 00b77bca  57                     push edi
   00b77b8c  e86f1fe1ff             call 0x989b00
   00b77b91  8b85f4020000           mov eax, dword ptr [ebp + 0x2f4]
@@ retail 642-643 built 665-666
   00b77bc0  8b95f8020000           mov edx, dword ptr [ebp + 0x2f8]
   00b77bc6  d9e0                   fchs 
 - 00b77bc8  56                     push esi
 + 00b77c07  57                     push edi
   00b77bc9  51                     push ecx
   00b77bca  8b8dfc020000           mov ecx, dword ptr [ebp + 0x2fc]
@@ retail 656-659 built 679-682
   00b77bf3  51                     push ecx
   00b77bf4  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
 - 00b77bfa  d95c2418               fstp dword ptr [esp + 0x18]
 - 00b77bfe  d9442418               fld dword ptr [esp + 0x18]
 - 00b77c02  8b442418               mov eax, dword ptr [esp + 0x18]
 + 00b77c39  d95c241c               fstp dword ptr [esp + 0x1c]
 + 00b77c3d  d944241c               fld dword ptr [esp + 0x1c]
 + 00b77c41  8b44241c               mov eax, dword ptr [esp + 0x1c]
   00b77c06  d80d98872301           fmul dword ptr [0x1238798]
   00b77c0c  d91c24                 fstp dword ptr [esp]
@@ retail 661-662 built 684-685
   00b77c06  d80d98872301           fmul dword ptr [0x1238798]
   00b77c0c  d91c24                 fstp dword ptr [esp]
 - 00b77c0f  56                     push esi
 + 00b77c4e  57                     push edi
   00b77c10  50                     push eax
   00b77c11  6a03                   push 3
@@ retail 665-666 built 688-689
   00b77c11  6a03                   push 3
   00b77c13  e8e81ee1ff             call 0x989b00
 - 00b77c18  8d4c2424               lea ecx, [esp + 0x24]
 + 00b77c57  8d4c2428               lea ecx, [esp + 0x28]
   00b77c1c  51                     push ecx
   00b77c1d  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
@@ retail 674-675 built 697-698
   00b77c36  8d95d4040000           lea edx, [ebp + 0x4d4]
   00b77c3c  52                     push edx
 - 00b77c3d  56                     push esi
 + 00b77c7c  57                     push edi
   00b77c3e  e8ad2be1ff             call 0x98a7f0
   00b77c43  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
@@ retail 703-704 built 726-727
   00b77ca8  d905dcde2201           fld dword ptr [0x122dedc]
   00b77cae  a1146e4301             mov eax, dword ptr [0x1436e14]
 - 00b77cb3  d9542414               fst dword ptr [esp + 0x14]
 + 00b77cf2  d9542418               fst dword ptr [esp + 0x18]
   00b77cb7  8b88cc030000           mov ecx, dword ptr [eax + 0x3cc]
   00b77cbd  d9542418               fst dword ptr [esp + 0x18]
@@ retail 705-707 built 728-728
   00b77cb3  d9542414               fst dword ptr [esp + 0x14]
   00b77cb7  8b88cc030000           mov ecx, dword ptr [eax + 0x3cc]
 - 00b77cbd  d9542418               fst dword ptr [esp + 0x18]
 - 00b77cc1  8b542414               mov edx, dword ptr [esp + 0x14]
   00b77cc5  d954241c               fst dword ptr [esp + 0x1c]
   00b77cc9  83c150                 add ecx, 0x50
@@ retail 708-708 built 729-731
   00b77cc1  8b542414               mov edx, dword ptr [esp + 0x14]
   00b77cc5  d954241c               fst dword ptr [esp + 0x1c]
 + 00b77d00  8b542418               mov edx, dword ptr [esp + 0x18]
 + 00b77d04  d9542420               fst dword ptr [esp + 0x20]
   00b77cc9  83c150                 add ecx, 0x50
   00b77ccc  d95c2420               fstp dword ptr [esp + 0x20]
@@ retail 709-710 built 732-733
   00b77cc5  d954241c               fst dword ptr [esp + 0x1c]
   00b77cc9  83c150                 add ecx, 0x50
 - 00b77ccc  d95c2420               fstp dword ptr [esp + 0x20]
 + 00b77d0b  d95c2424               fstp dword ptr [esp + 0x24]
   00b77cd0  8911                   mov dword ptr [ecx], edx
   00b77cd2  8b542418               mov edx, dword ptr [esp + 0x18]
@@ retail 711-712 built 734-735
   00b77ccc  d95c2420               fstp dword ptr [esp + 0x20]
   00b77cd0  8911                   mov dword ptr [ecx], edx
 - 00b77cd2  8b542418               mov edx, dword ptr [esp + 0x18]
 + 00b77d11  8b54241c               mov edx, dword ptr [esp + 0x1c]
   00b77cd6  895104                 mov dword ptr [ecx + 4], edx
   00b77cd9  8b54241c               mov edx, dword ptr [esp + 0x1c]
@@ retail 713-714 built 736-737
   00b77cd2  8b542418               mov edx, dword ptr [esp + 0x18]
   00b77cd6  895104                 mov dword ptr [ecx + 4], edx
 - 00b77cd9  8b54241c               mov edx, dword ptr [esp + 0x1c]
 + 00b77d18  8b542420               mov edx, dword ptr [esp + 0x20]
   00b77cdd  895108                 mov dword ptr [ecx + 8], edx
   00b77ce0  8b542420               mov edx, dword ptr [esp + 0x20]
@@ retail 715-716 built 738-739
   00b77cd9  8b54241c               mov edx, dword ptr [esp + 0x1c]
   00b77cdd  895108                 mov dword ptr [ecx + 8], edx
 - 00b77ce0  8b542420               mov edx, dword ptr [esp + 0x20]
 + 00b77d1f  8b542424               mov edx, dword ptr [esp + 0x24]
   00b77ce4  89510c                 mov dword ptr [ecx + 0xc], edx
   00b77ce7  8388d803000002         or dword ptr [eax + 0x3d8], 2
@@ retail 788-789 built 811-812
   00b77dc5  3bb548020000           cmp esi, dword ptr [ebp + 0x248]
   00b77dcb  741e                   je 0xb77deb
 - 00b77dcd  8d4900                 lea ecx, [ecx]
 + 00b77e0c  8d642400               lea esp, [esp]
   00b77dd0  8b85b8000000           mov eax, dword ptr [ebp + 0xb8]
   00b77dd6  8b0e                   mov ecx, dword ptr [esi]
@@ retail 803-804 built 826-827
   00b77df8  5d                     pop ebp
   00b77df9  5b                     pop ebx
 - 00b77dfa  83c444                 add esp, 0x44
 + 00b77e3a  83c448                 add esp, 0x48
   00b77dfd  c20400                 ret 4
   00b77e00  d815d8de2201           fcom dword ptr [0x122ded8]
DIFFLINES 158 retail_instr 812 built_instr 835
