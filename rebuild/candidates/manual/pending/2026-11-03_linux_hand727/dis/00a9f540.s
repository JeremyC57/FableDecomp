00a9f540 81ec4c010000         sub esp, 0x14c
00a9f546 53                   push ebx
00a9f547 55                   push ebp
00a9f548 8bac2460010000       mov ebp, dword ptr [esp + 0x160]
00a9f54f 8b9d98000000         mov ebx, dword ptr [ebp + 0x98]
00a9f555 56                   push esi
00a9f556 57                   push edi
00a9f557 8bf9                 mov edi, ecx
00a9f559 33f6                 xor esi, esi
00a9f55b 33c0                 xor eax, eax
00a9f55d 3bc6                 cmp eax, esi
00a9f55f 897c2438             mov dword ptr [esp + 0x38], edi
00a9f563 895c2430             mov dword ptr [esp + 0x30], ebx
00a9f567 8d0c9b               lea ecx, [ebx + ebx*4]
00a9f56a 0f8cb3080000         jl 0xa9fe23
00a9f570 7f08                 jg 0xa9f57a
00a9f572 3bce                 cmp ecx, esi
00a9f574 0f82a9080000         jb 0xa9fe23
00a9f57a 3bc6                 cmp eax, esi
00a9f57c 0f8fa1080000         jg 0xa9fe23
00a9f582 7c0c                 jl 0xa9f590
00a9f584 81f9ffffff7f         cmp ecx, 0x7fffffff
00a9f58a 0f8793080000         ja 0xa9fe23
00a9f590 3bce                 cmp ecx, esi
00a9f592 7427                 je 0xa9f5bb
00a9f594 8bc1                 mov eax, ecx
00a9f596 ba30000000           mov edx, 0x30
00a9f59b f7e2                 mul edx
00a9f59d 3bd6                 cmp edx, esi
00a9f59f 771a                 ja 0xa9f5bb
00a9f5a1 83f8ff               cmp eax, -1
00a9f5a4 7715                 ja 0xa9f5bb
00a9f5a6 8d0449               lea eax, [ecx + ecx*2]
00a9f5a9 c1e004               shl eax, 4
00a9f5ac 50                   push eax
00a9f5ad 8d4f14               lea ecx, [edi + 0x14]
00a9f5b0 e85bd3fbff           call 0xa5c910
00a9f5b5 8944242c             mov dword ptr [esp + 0x2c], eax
00a9f5b9 eb04                 jmp 0xa9f5bf
00a9f5bb 8974242c             mov dword ptr [esp + 0x2c], esi
00a9f5bf 8b942460010000       mov edx, dword ptr [esp + 0x160]
00a9f5c6 8b4c242c             mov ecx, dword ptr [esp + 0x2c]
00a9f5ca 8b4204               mov eax, dword ptr [edx + 4]
00a9f5cd 55                   push ebp
00a9f5ce 56                   push esi
00a9f5cf 53                   push ebx
00a9f5d0 51                   push ecx
00a9f5d1 8b4808               mov ecx, dword ptr [eax + 8]
00a9f5d4 51                   push ecx
00a9f5d5 8bcf                 mov ecx, edi
00a9f5d7 e8f4fbffff           call 0xa9f1d0
00a9f5dc 89742444             mov dword ptr [esp + 0x44], esi
00a9f5e0 89742448             mov dword ptr [esp + 0x48], esi
00a9f5e4 8b942460010000       mov edx, dword ptr [esp + 0x160]
00a9f5eb 8b4a04               mov ecx, dword ptr [edx + 4]
00a9f5ee 3bce                 cmp ecx, esi
00a9f5f0 0f84d8070000         je 0xa9fdce
00a9f5f6 8b5110               mov edx, dword ptr [ecx + 0x10]
00a9f5f9 8b710c               mov esi, dword ptr [ecx + 0xc]
00a9f5fc 2bd6                 sub edx, esi
00a9f5fe b867666666           mov eax, 0x66666667
00a9f603 f7ea                 imul edx
00a9f605 c1fa03               sar edx, 3
00a9f608 8bc2                 mov eax, edx
00a9f60a c1e81f               shr eax, 0x1f
00a9f60d 03c2                 add eax, edx
00a9f60f 39442444             cmp dword ptr [esp + 0x44], eax
00a9f613 0f83b5070000         jae 0xa9fdce
00a9f619 8b442448             mov eax, dword ptr [esp + 0x48]
00a9f61d 8b4f04               mov ecx, dword ptr [edi + 4]
00a9f620 03f0                 add esi, eax
00a9f622 8b4608               mov eax, dword ptr [esi + 8]
00a9f625 50                   push eax
00a9f626 e8954cf8ff           call 0xa242c0
00a9f62b 55                   push ebp
00a9f62c 8d4c2424             lea ecx, [esp + 0x24]
00a9f630 51                   push ecx
00a9f631 8bc8                 mov ecx, eax
00a9f633 e82876f8ff           call 0xa26c60
00a9f638 8b4e0c               mov ecx, dword ptr [esi + 0xc]
00a9f63b 8d542440             lea edx, [esp + 0x40]
00a9f63f 52                   push edx
00a9f640 8d442440             lea eax, [esp + 0x40]
00a9f644 50                   push eax
00a9f645 51                   push ecx
00a9f646 8b4c242c             mov ecx, dword ptr [esp + 0x2c]
00a9f64a e80130fbff           call 0xa52650
00a9f64f 8b5610               mov edx, dword ptr [esi + 0x10]
00a9f652 89542414             mov dword ptr [esp + 0x14], edx
00a9f656 8b542420             mov edx, dword ptr [esp + 0x20]
00a9f65a 8b4208               mov eax, dword ptr [edx + 8]
00a9f65d 8bc8                 mov ecx, eax
00a9f65f c1e902               shr ecx, 2
00a9f662 f6c101               test cl, 1
00a9f665 0f8448030000         je 0xa9f9b3
00a9f66b d905d8de2201         fld dword ptr [0x122ded8]
00a9f671 33ff                 xor edi, edi
00a9f673 d9442414             fld dword ptr [esp + 0x14]
00a9f677 dae9                 fucompp 
00a9f679 dfe0                 fnstsw ax
00a9f67b f6c444               test ah, 0x44
00a9f67e 0f8a92000000         jp 0xa9f716
00a9f684 85db                 test ebx, ebx
00a9f686 0f86ed060000         jbe 0xa9fd79
00a9f68c 8b74242c             mov esi, dword ptr [esp + 0x2c]
00a9f690 8b6c2440             mov ebp, dword ptr [esp + 0x40]
00a9f694 8b5c243c             mov ebx, dword ptr [esp + 0x3c]
00a9f698 83c618               add esi, 0x18
00a9f69b eb03                 jmp 0xa9f6a0
00a9f69d 8d4900               lea ecx, [ecx]
00a9f6a0 8b425c               mov eax, dword ptr [edx + 0x5c]
00a9f6a3 8b0cf8               mov ecx, dword ptr [eax + edi*8]
00a9f6a6 8d04f8               lea eax, [eax + edi*8]
00a9f6a9 8a410a               mov al, byte ptr [ecx + 0xa]
00a9f6ac 84c0                 test al, al
00a9f6ae 7855                 js 0xa9f705
00a9f6b0 55                   push ebp
00a9f6b1 53                   push ebx
00a9f6b2 8d942434010000       lea edx, [esp + 0x134]
00a9f6b9 52                   push edx
00a9f6ba e831cbfaff           call 0xa4c1f0
00a9f6bf 8d84242c010000       lea eax, [esp + 0x12c]
00a9f6c6 50                   push eax
00a9f6c7 8d4ee8               lea ecx, [esi - 0x18]
00a9f6ca e84195feff           call 0xa88c10
00a9f6cf d984243c010000       fld dword ptr [esp + 0x13c]
00a9f6d6 d846f8               fadd dword ptr [esi - 8]
00a9f6d9 8b542420             mov edx, dword ptr [esp + 0x20]
00a9f6dd d95ef8               fstp dword ptr [esi - 8]
00a9f6e0 d9842440010000       fld dword ptr [esp + 0x140]
00a9f6e7 d846fc               fadd dword ptr [esi - 4]
00a9f6ea d95efc               fstp dword ptr [esi - 4]
00a9f6ed d9842444010000       fld dword ptr [esp + 0x144]
00a9f6f4 d806                 fadd dword ptr [esi]
00a9f6f6 d91e                 fstp dword ptr [esi]
00a9f6f8 d9842448010000       fld dword ptr [esp + 0x148]
00a9f6ff d84604               fadd dword ptr [esi + 4]
00a9f702 d95e04               fstp dword ptr [esi + 4]
00a9f705 8b442430             mov eax, dword ptr [esp + 0x30]
00a9f709 47                   inc edi
00a9f70a 83c630               add esi, 0x30
00a9f70d 3bf8                 cmp edi, eax
00a9f70f 728f                 jb 0xa9f6a0
00a9f711 e95f060000           jmp 0xa9fd75
00a9f716 85db                 test ebx, ebx
00a9f718 0f865b060000         jbe 0xa9fd79
00a9f71e 8b74242c             mov esi, dword ptr [esp + 0x2c]
00a9f722 8b6c2440             mov ebp, dword ptr [esp + 0x40]
00a9f726 83c618               add esi, 0x18
00a9f729 8da42400000000       lea esp, [esp]
00a9f730 8b4a5c               mov ecx, dword ptr [edx + 0x5c]
00a9f733 8d04f9               lea eax, [ecx + edi*8]
00a9f736 8b08                 mov ecx, dword ptr [eax]
00a9f738 8a410a               mov al, byte ptr [ecx + 0xa]
00a9f73b 84c0                 test al, al
00a9f73d 0f885f020000         js 0xa9f9a2
00a9f743 8b54243c             mov edx, dword ptr [esp + 0x3c]
00a9f747 55                   push ebp
00a9f748 52                   push edx
00a9f749 8d8424d4000000       lea eax, [esp + 0xd4]
00a9f750 50                   push eax
00a9f751 e89acafaff           call 0xa4c1f0
00a9f756 d98424d8000000       fld dword ptr [esp + 0xd8]
00a9f75d d81ddcde2201         fcomp dword ptr [0x122dedc]
00a9f763 8b8c24d8000000       mov ecx, dword ptr [esp + 0xd8]
00a9f76a 894c2434             mov dword ptr [esp + 0x34], ecx
00a9f76e c74424680000803f     mov dword ptr [esp + 0x68], 0x3f800000
00a9f776 dfe0                 fnstsw ax
00a9f778 f6c405               test ah, 5
00a9f77b 7a15                 jp 0xa9f792
00a9f77d d98424d8000000       fld dword ptr [esp + 0xd8]
00a9f784 c7442468000080bf     mov dword ptr [esp + 0x68], 0xbf800000
00a9f78c d9e0                 fchs 
00a9f78e d95c2434             fstp dword ptr [esp + 0x34]
00a9f792 d905d8de2201         fld dword ptr [0x122ded8]
00a9f798 d8642434             fsub dword ptr [esp + 0x34]
00a9f79c d81d5ca12501         fcomp dword ptr [0x125a15c]
00a9f7a2 dfe0                 fnstsw ax
00a9f7a4 f6c441               test ah, 0x41
00a9f7a7 0f8534010000         jne 0xa9f8e1
00a9f7ad d9442434             fld dword ptr [esp + 0x34]
00a9f7b1 d805d8de2201         fadd dword ptr [0x122ded8]
00a9f7b7 d80d1ccd2901         fmul dword ptr [0x129cd1c]
00a9f7bd d9542410             fst dword ptr [esp + 0x10]
00a9f7c1 d8259cf52201         fsub dword ptr [0x122f59c]
00a9f7c7 d95c241c             fstp dword ptr [esp + 0x1c]
00a9f7cb d944241c             fld dword ptr [esp + 0x1c]
00a9f7cf db5c2464             fistp dword ptr [esp + 0x64]
00a9f7d3 db442464             fild dword ptr [esp + 0x64]
00a9f7d7 8b442464             mov eax, dword ptr [esp + 0x64]
00a9f7db d86c2410             fsubr dword ptr [esp + 0x10]
00a9f7df d905d8de2201         fld dword ptr [0x122ded8]
00a9f7e5 d8e1                 fsub st(1)
00a9f7e7 d80c8530b53c01       fmul dword ptr [eax*4 + 0x13cb530]
00a9f7ee d9c9                 fxch st(1)
00a9f7f0 d80c8534b53c01       fmul dword ptr [eax*4 + 0x13cb534]
00a9f7f7 dec1                 faddp st(1)
00a9f7f9 d80d087f2301         fmul dword ptr [0x1237f08]
00a9f7ff d95c2428             fstp dword ptr [esp + 0x28]
00a9f803 d9442434             fld dword ptr [esp + 0x34]
00a9f807 d84c2434             fmul dword ptr [esp + 0x34]
00a9f80b d82dd8de2201         fsubr dword ptr [0x122ded8]
00a9f811 d9fa                 fsqrt 
00a9f813 d95c2418             fstp dword ptr [esp + 0x18]
00a9f817 d905d8de2201         fld dword ptr [0x122ded8]
00a9f81d d8642414             fsub dword ptr [esp + 0x14]
00a9f821 d84c2428             fmul dword ptr [esp + 0x28]
00a9f825 d80d10002301         fmul dword ptr [0x1230010]
00a9f82b d9542410             fst dword ptr [esp + 0x10]
00a9f82f d8259cf52201         fsub dword ptr [0x122f59c]
00a9f835 d95c241c             fstp dword ptr [esp + 0x1c]
00a9f839 d944241c             fld dword ptr [esp + 0x1c]
00a9f83d db5c2454             fistp dword ptr [esp + 0x54]
00a9f841 db442454             fild dword ptr [esp + 0x54]
00a9f845 8b442454             mov eax, dword ptr [esp + 0x54]
00a9f849 0500ffffff           add eax, 0xffffff00
00a9f84e 25ff030000           and eax, 0x3ff
00a9f853 d86c2410             fsubr dword ptr [esp + 0x10]
00a9f857 d905d8de2201         fld dword ptr [0x122ded8]
00a9f85d d8742418             fdiv dword ptr [esp + 0x18]
00a9f861 d95c2410             fstp dword ptr [esp + 0x10]
00a9f865 d905d8de2201         fld dword ptr [0x122ded8]
00a9f86b d8e1                 fsub st(1)
00a9f86d d80c8550d53c01       fmul dword ptr [eax*4 + 0x13cd550]
00a9f874 d9c9                 fxch st(1)
00a9f876 d80c8554d53c01       fmul dword ptr [eax*4 + 0x13cd554]
00a9f87d dec1                 faddp st(1)
00a9f87f d84c2410             fmul dword ptr [esp + 0x10]
00a9f883 d95c2418             fstp dword ptr [esp + 0x18]
00a9f887 d9442428             fld dword ptr [esp + 0x28]
00a9f88b d84c2414             fmul dword ptr [esp + 0x14]
00a9f88f d80d10002301         fmul dword ptr [0x1230010]
00a9f895 d9542428             fst dword ptr [esp + 0x28]
00a9f899 d8259cf52201         fsub dword ptr [0x122f59c]
00a9f89f d95c241c             fstp dword ptr [esp + 0x1c]
00a9f8a3 d944241c             fld dword ptr [esp + 0x1c]
00a9f8a7 db5c2458             fistp dword ptr [esp + 0x58]
00a9f8ab db442458             fild dword ptr [esp + 0x58]
00a9f8af 8b442458             mov eax, dword ptr [esp + 0x58]
00a9f8b3 0500ffffff           add eax, 0xffffff00
00a9f8b8 25ff030000           and eax, 0x3ff
00a9f8bd d86c2428             fsubr dword ptr [esp + 0x28]
00a9f8c1 d905d8de2201         fld dword ptr [0x122ded8]
00a9f8c7 d8e1                 fsub st(1)
00a9f8c9 d80c8550d53c01       fmul dword ptr [eax*4 + 0x13cd550]
00a9f8d0 d9c9                 fxch st(1)
00a9f8d2 d80c8554d53c01       fmul dword ptr [eax*4 + 0x13cd554]
00a9f8d9 dec1                 faddp st(1)
00a9f8db d84c2410             fmul dword ptr [esp + 0x10]
00a9f8df eb12                 jmp 0xa9f8f3
00a9f8e1 d905d8de2201         fld dword ptr [0x122ded8]
00a9f8e7 d8642414             fsub dword ptr [esp + 0x14]
00a9f8eb d95c2418             fstp dword ptr [esp + 0x18]
00a9f8ef d9442414             fld dword ptr [esp + 0x14]
00a9f8f3 d84c2468             fmul dword ptr [esp + 0x68]
00a9f8f7 8d54246c             lea edx, [esp + 0x6c]
00a9f8fb d98424cc000000       fld dword ptr [esp + 0xcc]
00a9f902 52                   push edx
00a9f903 8d4ee8               lea ecx, [esi - 0x18]
00a9f906 d8c9                 fmul st(1)
00a9f908 d95c2470             fstp dword ptr [esp + 0x70]
00a9f90c d98424d4000000       fld dword ptr [esp + 0xd4]
00a9f913 d8c9                 fmul st(1)
00a9f915 d95c2474             fstp dword ptr [esp + 0x74]
00a9f919 d98424d8000000       fld dword ptr [esp + 0xd8]
00a9f920 d8c9                 fmul st(1)
00a9f922 d95c2478             fstp dword ptr [esp + 0x78]
00a9f926 d98424dc000000       fld dword ptr [esp + 0xdc]
00a9f92d d8c9                 fmul st(1)
00a9f92f d844241c             fadd dword ptr [esp + 0x1c]
00a9f933 d95c247c             fstp dword ptr [esp + 0x7c]
00a9f937 ddd8                 fstp st(0)
00a9f939 e8d292feff           call 0xa88c10
00a9f93e d98424dc000000       fld dword ptr [esp + 0xdc]
00a9f945 8b542420             mov edx, dword ptr [esp + 0x20]
00a9f949 d84c2414             fmul dword ptr [esp + 0x14]
00a9f94d d98424e0000000       fld dword ptr [esp + 0xe0]
00a9f954 d84c2414             fmul dword ptr [esp + 0x14]
00a9f958 d98424e4000000       fld dword ptr [esp + 0xe4]
00a9f95f d84c2414             fmul dword ptr [esp + 0x14]
00a9f963 d99c2424010000       fstp dword ptr [esp + 0x124]
00a9f96a d98424e8000000       fld dword ptr [esp + 0xe8]
00a9f971 d84c2414             fmul dword ptr [esp + 0x14]
00a9f975 d99c2428010000       fstp dword ptr [esp + 0x128]
00a9f97c d9c9                 fxch st(1)
00a9f97e d846f8               fadd dword ptr [esi - 8]
00a9f981 d95ef8               fstp dword ptr [esi - 8]
00a9f984 d846fc               fadd dword ptr [esi - 4]
00a9f987 d95efc               fstp dword ptr [esi - 4]
00a9f98a d9842424010000       fld dword ptr [esp + 0x124]
00a9f991 d806                 fadd dword ptr [esi]
00a9f993 d91e                 fstp dword ptr [esi]
00a9f995 d9842428010000       fld dword ptr [esp + 0x128]
00a9f99c d84604               fadd dword ptr [esi + 4]
00a9f99f d95e04               fstp dword ptr [esi + 4]
00a9f9a2 47                   inc edi
00a9f9a3 83c630               add esi, 0x30
00a9f9a6 3bfb                 cmp edi, ebx
00a9f9a8 0f8282fdffff         jb 0xa9f730
00a9f9ae e9c6030000           jmp 0xa9fd79
00a9f9b3 d1e8                 shr eax, 1
00a9f9b5 a801                 test al, 1
00a9f9b7 0f84bc030000         je 0xa9fd79
00a9f9bd d905d8de2201         fld dword ptr [0x122ded8]
00a9f9c3 d9442414             fld dword ptr [esp + 0x14]
00a9f9c7 dae9                 fucompp 
00a9f9c9 dfe0                 fnstsw ax
00a9f9cb f6c444               test ah, 0x44
00a9f9ce 7a46                 jp 0xa9fa16
00a9f9d0 33f6                 xor esi, esi
00a9f9d2 85db                 test ebx, ebx
00a9f9d4 0f869f030000         jbe 0xa9fd79
00a9f9da 8b7c242c             mov edi, dword ptr [esp + 0x2c]
00a9f9de 8b6c2440             mov ebp, dword ptr [esp + 0x40]
00a9f9e2 8b5c243c             mov ebx, dword ptr [esp + 0x3c]
00a9f9e6 8b4268               mov eax, dword ptr [edx + 0x68]
00a9f9e9 85c0                 test eax, eax
00a9f9eb 7406                 je 0xa9f9f3
00a9f9ed 803c3000             cmp byte ptr [eax + esi], 0
00a9f9f1 7412                 je 0xa9fa05
00a9f9f3 8b425c               mov eax, dword ptr [edx + 0x5c]
00a9f9f6 8b0cf0               mov ecx, dword ptr [eax + esi*8]
00a9f9f9 55                   push ebp
00a9f9fa 53                   push ebx
00a9f9fb 57                   push edi
00a9f9fc e8efc7faff           call 0xa4c1f0
00a9fa01 8b542420             mov edx, dword ptr [esp + 0x20]
00a9fa05 8b442430             mov eax, dword ptr [esp + 0x30]
00a9fa09 46                   inc esi
00a9fa0a 83c730               add edi, 0x30
00a9fa0d 3bf0                 cmp esi, eax
00a9fa0f 72d5                 jb 0xa9f9e6
00a9fa11 e95f030000           jmp 0xa9fd75
00a9fa16 33ff                 xor edi, edi
00a9fa18 85db                 test ebx, ebx
00a9fa1a 0f8659030000         jbe 0xa9fd79
00a9fa20 8b74242c             mov esi, dword ptr [esp + 0x2c]
00a9fa24 8b6c2440             mov ebp, dword ptr [esp + 0x40]
00a9fa28 8b5c243c             mov ebx, dword ptr [esp + 0x3c]
00a9fa2c 83c618               add esi, 0x18
00a9fa2f 90                   nop 
00a9fa30 8b4268               mov eax, dword ptr [edx + 0x68]
00a9fa33 85c0                 test eax, eax
00a9fa35 740a                 je 0xa9fa41
00a9fa37 803c3800             cmp byte ptr [eax + edi], 0
00a9fa3b 0f8424030000         je 0xa9fd65
00a9fa41 8b525c               mov edx, dword ptr [edx + 0x5c]
00a9fa44 55                   push ebp
00a9fa45 53                   push ebx
00a9fa46 8d8c24a4000000       lea ecx, [esp + 0xa4]
00a9fa4d 51                   push ecx
00a9fa4e 8b0cfa               mov ecx, dword ptr [edx + edi*8]
00a9fa51 e89ac7faff           call 0xa4c1f0
00a9fa56 d98424a0000000       fld dword ptr [esp + 0xa0]
00a9fa5d d84eec               fmul dword ptr [esi - 0x14]
00a9fa60 c74424600000803f     mov dword ptr [esp + 0x60], 0x3f800000
00a9fa68 d98424a4000000       fld dword ptr [esp + 0xa4]
00a9fa6f d84ef0               fmul dword ptr [esi - 0x10]
00a9fa72 dec1                 faddp st(1)
00a9fa74 d98424a8000000       fld dword ptr [esp + 0xa8]
00a9fa7b d84ef4               fmul dword ptr [esi - 0xc]
00a9fa7e dec1                 faddp st(1)
00a9fa80 d984249c000000       fld dword ptr [esp + 0x9c]
00a9fa87 d84ee8               fmul dword ptr [esi - 0x18]
00a9fa8a dec1                 faddp st(1)
00a9fa8c d95c2410             fstp dword ptr [esp + 0x10]
00a9fa90 d9442410             fld dword ptr [esp + 0x10]
00a9fa94 d81ddcde2201         fcomp dword ptr [0x122dedc]
00a9fa9a dfe0                 fnstsw ax
00a9fa9c f6c405               test ah, 5
00a9fa9f 7a12                 jp 0xa9fab3
00a9faa1 d9442410             fld dword ptr [esp + 0x10]
00a9faa5 c7442460000080bf     mov dword ptr [esp + 0x60], 0xbf800000
00a9faad d9e0                 fchs 
00a9faaf d95c2410             fstp dword ptr [esp + 0x10]
00a9fab3 d905d8de2201         fld dword ptr [0x122ded8]
00a9fab9 d8642410             fsub dword ptr [esp + 0x10]
00a9fabd d81d5ca12501         fcomp dword ptr [0x125a15c]
00a9fac3 dfe0                 fnstsw ax
00a9fac5 f6c441               test ah, 0x41
00a9fac8 0f8534010000         jne 0xa9fc02
00a9face d9442410             fld dword ptr [esp + 0x10]
00a9fad2 d805d8de2201         fadd dword ptr [0x122ded8]
00a9fad8 d80d1ccd2901         fmul dword ptr [0x129cd1c]
00a9fade d9542418             fst dword ptr [esp + 0x18]
00a9fae2 d8259cf52201         fsub dword ptr [0x122f59c]
00a9fae8 d95c241c             fstp dword ptr [esp + 0x1c]
00a9faec d944241c             fld dword ptr [esp + 0x1c]
00a9faf0 db5c244c             fistp dword ptr [esp + 0x4c]
00a9faf4 db44244c             fild dword ptr [esp + 0x4c]
00a9faf8 8b44244c             mov eax, dword ptr [esp + 0x4c]
00a9fafc d86c2418             fsubr dword ptr [esp + 0x18]
00a9fb00 d905d8de2201         fld dword ptr [0x122ded8]
00a9fb06 d8e1                 fsub st(1)
00a9fb08 d80c8530b53c01       fmul dword ptr [eax*4 + 0x13cb530]
00a9fb0f d9c9                 fxch st(1)
00a9fb11 d80c8534b53c01       fmul dword ptr [eax*4 + 0x13cb534]
00a9fb18 dec1                 faddp st(1)
00a9fb1a d80d087f2301         fmul dword ptr [0x1237f08]
00a9fb20 d95c2428             fstp dword ptr [esp + 0x28]
00a9fb24 d9442410             fld dword ptr [esp + 0x10]
00a9fb28 d84c2410             fmul dword ptr [esp + 0x10]
00a9fb2c d82dd8de2201         fsubr dword ptr [0x122ded8]
00a9fb32 d9fa                 fsqrt 
00a9fb34 d95c2410             fstp dword ptr [esp + 0x10]
00a9fb38 d905d8de2201         fld dword ptr [0x122ded8]
00a9fb3e d8642414             fsub dword ptr [esp + 0x14]
00a9fb42 d84c2428             fmul dword ptr [esp + 0x28]
00a9fb46 d80d10002301         fmul dword ptr [0x1230010]
00a9fb4c d9542418             fst dword ptr [esp + 0x18]
00a9fb50 d8259cf52201         fsub dword ptr [0x122f59c]
00a9fb56 d95c241c             fstp dword ptr [esp + 0x1c]
00a9fb5a d944241c             fld dword ptr [esp + 0x1c]
00a9fb5e db5c245c             fistp dword ptr [esp + 0x5c]
00a9fb62 db44245c             fild dword ptr [esp + 0x5c]
00a9fb66 8b44245c             mov eax, dword ptr [esp + 0x5c]
00a9fb6a 0500ffffff           add eax, 0xffffff00
00a9fb6f 25ff030000           and eax, 0x3ff
00a9fb74 d86c2418             fsubr dword ptr [esp + 0x18]
00a9fb78 d905d8de2201         fld dword ptr [0x122ded8]
00a9fb7e d8742410             fdiv dword ptr [esp + 0x10]
00a9fb82 d95c241c             fstp dword ptr [esp + 0x1c]
00a9fb86 d905d8de2201         fld dword ptr [0x122ded8]
00a9fb8c d8e1                 fsub st(1)
00a9fb8e d80c8550d53c01       fmul dword ptr [eax*4 + 0x13cd550]
00a9fb95 d9c9                 fxch st(1)
00a9fb97 d80c8554d53c01       fmul dword ptr [eax*4 + 0x13cd554]
00a9fb9e dec1                 faddp st(1)
00a9fba0 d84c241c             fmul dword ptr [esp + 0x1c]
00a9fba4 d95c2410             fstp dword ptr [esp + 0x10]
00a9fba8 d9442428             fld dword ptr [esp + 0x28]
00a9fbac d84c2414             fmul dword ptr [esp + 0x14]
00a9fbb0 d80d10002301         fmul dword ptr [0x1230010]
00a9fbb6 d9542418             fst dword ptr [esp + 0x18]
00a9fbba d8259cf52201         fsub dword ptr [0x122f59c]
00a9fbc0 d95c2428             fstp dword ptr [esp + 0x28]
00a9fbc4 d9442428             fld dword ptr [esp + 0x28]
00a9fbc8 db5c2450             fistp dword ptr [esp + 0x50]
00a9fbcc db442450             fild dword ptr [esp + 0x50]
00a9fbd0 8b442450             mov eax, dword ptr [esp + 0x50]
00a9fbd4 0500ffffff           add eax, 0xffffff00
00a9fbd9 25ff030000           and eax, 0x3ff
00a9fbde d86c2418             fsubr dword ptr [esp + 0x18]
00a9fbe2 d905d8de2201         fld dword ptr [0x122ded8]
00a9fbe8 d8e1                 fsub st(1)
00a9fbea d80c8550d53c01       fmul dword ptr [eax*4 + 0x13cd550]
00a9fbf1 d9c9                 fxch st(1)
00a9fbf3 d80c8554d53c01       fmul dword ptr [eax*4 + 0x13cd554]
00a9fbfa dec1                 faddp st(1)
00a9fbfc d84c241c             fmul dword ptr [esp + 0x1c]
00a9fc00 eb12                 jmp 0xa9fc14
00a9fc02 d905d8de2201         fld dword ptr [0x122ded8]
00a9fc08 d8642414             fsub dword ptr [esp + 0x14]
00a9fc0c d95c2410             fstp dword ptr [esp + 0x10]
00a9fc10 d9442414             fld dword ptr [esp + 0x14]
00a9fc14 d84c2460             fmul dword ptr [esp + 0x60]
00a9fc18 d984249c000000       fld dword ptr [esp + 0x9c]
00a9fc1f d8c9                 fmul st(1)
00a9fc21 d9442410             fld dword ptr [esp + 0x10]
00a9fc25 d84ee8               fmul dword ptr [esi - 0x18]
00a9fc28 dec1                 faddp st(1)
00a9fc2a d99c248c000000       fstp dword ptr [esp + 0x8c]
00a9fc31 8b84248c000000       mov eax, dword ptr [esp + 0x8c]
00a9fc38 d98424a0000000       fld dword ptr [esp + 0xa0]
00a9fc3f d8c9                 fmul st(1)
00a9fc41 d9442410             fld dword ptr [esp + 0x10]
00a9fc45 d84eec               fmul dword ptr [esi - 0x14]
00a9fc48 dec1                 faddp st(1)
00a9fc4a d99c2490000000       fstp dword ptr [esp + 0x90]
00a9fc51 8b8c2490000000       mov ecx, dword ptr [esp + 0x90]
00a9fc58 d98424a4000000       fld dword ptr [esp + 0xa4]
00a9fc5f d8c9                 fmul st(1)
00a9fc61 d9442410             fld dword ptr [esp + 0x10]
00a9fc65 d84ef0               fmul dword ptr [esi - 0x10]
00a9fc68 dec1                 faddp st(1)
00a9fc6a d99c2494000000       fstp dword ptr [esp + 0x94]
00a9fc71 8b942494000000       mov edx, dword ptr [esp + 0x94]
00a9fc78 d98424a8000000       fld dword ptr [esp + 0xa8]
00a9fc7f d8c9                 fmul st(1)
00a9fc81 d9442410             fld dword ptr [esp + 0x10]
00a9fc85 d84ef4               fmul dword ptr [esi - 0xc]
00a9fc88 8946e8               mov dword ptr [esi - 0x18], eax
00a9fc8b 894eec               mov dword ptr [esi - 0x14], ecx
00a9fc8e 8956f0               mov dword ptr [esi - 0x10], edx
00a9fc91 dec1                 faddp st(1)
00a9fc93 d99c2498000000       fstp dword ptr [esp + 0x98]
00a9fc9a 8b842498000000       mov eax, dword ptr [esp + 0x98]
00a9fca1 8946f4               mov dword ptr [esi - 0xc], eax
00a9fca4 ddd8                 fstp st(0)
00a9fca6 d98424ac000000       fld dword ptr [esp + 0xac]
00a9fcad d866f8               fsub dword ptr [esi - 8]
00a9fcb0 d98424b0000000       fld dword ptr [esp + 0xb0]
00a9fcb7 d866fc               fsub dword ptr [esi - 4]
00a9fcba d98424b4000000       fld dword ptr [esp + 0xb4]
00a9fcc1 d826                 fsub dword ptr [esi]
00a9fcc3 d99c2404010000       fstp dword ptr [esp + 0x104]
00a9fcca d98424b8000000       fld dword ptr [esp + 0xb8]
00a9fcd1 d86604               fsub dword ptr [esi + 4]
00a9fcd4 d99c2408010000       fstp dword ptr [esp + 0x108]
00a9fcdb d9c9                 fxch st(1)
00a9fcdd d84c2414             fmul dword ptr [esp + 0x14]
00a9fce1 d99c240c010000       fstp dword ptr [esp + 0x10c]
00a9fce8 d84c2414             fmul dword ptr [esp + 0x14]
00a9fcec d9842404010000       fld dword ptr [esp + 0x104]
00a9fcf3 d84c2414             fmul dword ptr [esp + 0x14]
00a9fcf7 d9842408010000       fld dword ptr [esp + 0x108]
00a9fcfe d84c2414             fmul dword ptr [esp + 0x14]
00a9fd02 d99c2418010000       fstp dword ptr [esp + 0x118]
00a9fd09 d984240c010000       fld dword ptr [esp + 0x10c]
00a9fd10 d846f8               fadd dword ptr [esi - 8]
00a9fd13 d95c247c             fstp dword ptr [esp + 0x7c]
00a9fd17 8b4c247c             mov ecx, dword ptr [esp + 0x7c]
00a9fd1b d9c9                 fxch st(1)
00a9fd1d d846fc               fadd dword ptr [esi - 4]
00a9fd20 d99c2480000000       fstp dword ptr [esp + 0x80]
00a9fd27 8b942480000000       mov edx, dword ptr [esp + 0x80]
00a9fd2e d806                 fadd dword ptr [esi]
00a9fd30 d99c2484000000       fstp dword ptr [esp + 0x84]
00a9fd37 8b842484000000       mov eax, dword ptr [esp + 0x84]
00a9fd3e d9842418010000       fld dword ptr [esp + 0x118]
00a9fd45 d84604               fadd dword ptr [esi + 4]
00a9fd48 894ef8               mov dword ptr [esi - 8], ecx
00a9fd4b 8956fc               mov dword ptr [esi - 4], edx
00a9fd4e 8b542420             mov edx, dword ptr [esp + 0x20]
00a9fd52 d99c2488000000       fstp dword ptr [esp + 0x88]
00a9fd59 8b8c2488000000       mov ecx, dword ptr [esp + 0x88]
00a9fd60 8906                 mov dword ptr [esi], eax
00a9fd62 894e04               mov dword ptr [esi + 4], ecx
00a9fd65 8b442430             mov eax, dword ptr [esp + 0x30]
00a9fd69 47                   inc edi
00a9fd6a 83c630               add esi, 0x30
00a9fd6d 3bf8                 cmp edi, eax
00a9fd6f 0f82bbfcffff         jb 0xa9fa30
00a9fd75 8b5c2430             mov ebx, dword ptr [esp + 0x30]
00a9fd79 8b442424             mov eax, dword ptr [esp + 0x24]
00a9fd7d 33f6                 xor esi, esi
00a9fd7f 3bc6                 cmp eax, esi
00a9fd81 741d                 je 0xa9fda0
00a9fd83 ff08                 dec dword ptr [eax]
00a9fd85 8b442424             mov eax, dword ptr [esp + 0x24]
00a9fd89 3930                 cmp dword ptr [eax], esi
00a9fd8b 7513                 jne 0xa9fda0
00a9fd8d 8b4808               mov ecx, dword ptr [eax + 8]
00a9fd90 ff5004               call dword ptr [eax + 4]
00a9fd93 8b542424             mov edx, dword ptr [esp + 0x24]
00a9fd97 52                   push edx
00a9fd98 e81fec1500           call 0xbfe9bc
00a9fd9d 83c404               add esp, 4
00a9fda0 8b4c2444             mov ecx, dword ptr [esp + 0x44]
00a9fda4 8b442448             mov eax, dword ptr [esp + 0x48]
00a9fda8 8b7c2438             mov edi, dword ptr [esp + 0x38]
00a9fdac 8bac2468010000       mov ebp, dword ptr [esp + 0x168]
00a9fdb3 41                   inc ecx
00a9fdb4 83c014               add eax, 0x14
00a9fdb7 89742420             mov dword ptr [esp + 0x20], esi
00a9fdbb 89742424             mov dword ptr [esp + 0x24], esi
00a9fdbf 894c2444             mov dword ptr [esp + 0x44], ecx
00a9fdc3 89442448             mov dword ptr [esp + 0x48], eax
00a9fdc7 33f6                 xor esi, esi
00a9fdc9 e916f8ffff           jmp 0xa9f5e4
00a9fdce 8bb42464010000       mov esi, dword ptr [esp + 0x164]
00a9fdd5 85f6                 test esi, esi
00a9fdd7 740e                 je 0xa9fde7
00a9fdd9 8b44242c             mov eax, dword ptr [esp + 0x2c]
00a9fddd 55                   push ebp
00a9fdde 56                   push esi
00a9fddf 50                   push eax
00a9fde0 8bcf                 mov ecx, edi
00a9fde2 e8b9e1ffff           call 0xa9dfa0
00a9fde7 8bbc2474010000       mov edi, dword ptr [esp + 0x174]
00a9fdee 8b4c242c             mov ecx, dword ptr [esp + 0x2c]
00a9fdf2 57                   push edi
00a9fdf3 55                   push ebp
00a9fdf4 8bac2474010000       mov ebp, dword ptr [esp + 0x174]
00a9fdfb 55                   push ebp
00a9fdfc 51                   push ecx
00a9fdfd 8b4c2448             mov ecx, dword ptr [esp + 0x48]
00a9fe01 e8dae3ffff           call 0xa9e1e0
00a9fe06 85f6                 test esi, esi
00a9fe08 740d                 je 0xa9fe17
00a9fe0a 8b4c2438             mov ecx, dword ptr [esp + 0x38]
00a9fe0e 57                   push edi
00a9fe0f 53                   push ebx
00a9fe10 55                   push ebp
00a9fe11 56                   push esi
00a9fe12 e839d9ffff           call 0xa9d750
00a9fe17 8b4c2438             mov ecx, dword ptr [esp + 0x38]
00a9fe1b 83c114               add ecx, 0x14
00a9fe1e e8fdc8fbff           call 0xa5c720
00a9fe23 5f                   pop edi
00a9fe24 5e                   pop esi
00a9fe25 5d                   pop ebp
00a9fe26 5b                   pop ebx
00a9fe27 81c44c010000         add esp, 0x14c
00a9fe2d c21800               ret 0x18
