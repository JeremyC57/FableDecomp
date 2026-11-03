00aa0090 81ec4c010000         sub esp, 0x14c
00aa0096 53                   push ebx
00aa0097 55                   push ebp
00aa0098 56                   push esi
00aa0099 57                   push edi
00aa009a 8bbc2470010000       mov edi, dword ptr [esp + 0x170]
00aa00a1 8b9f98000000         mov ebx, dword ptr [edi + 0x98]
00aa00a7 8be9                 mov ebp, ecx
00aa00a9 33f6                 xor esi, esi
00aa00ab 33c0                 xor eax, eax
00aa00ad 3bc6                 cmp eax, esi
00aa00af 896c2438             mov dword ptr [esp + 0x38], ebp
00aa00b3 895c2430             mov dword ptr [esp + 0x30], ebx
00aa00b7 8d0c9b               lea ecx, [ebx + ebx*4]
00aa00ba 0f8c19090000         jl 0xaa09d9
00aa00c0 7f08                 jg 0xaa00ca
00aa00c2 3bce                 cmp ecx, esi
00aa00c4 0f820f090000         jb 0xaa09d9
00aa00ca 3bc6                 cmp eax, esi
00aa00cc 0f8f07090000         jg 0xaa09d9
00aa00d2 7c0c                 jl 0xaa00e0
00aa00d4 81f9ffffff7f         cmp ecx, 0x7fffffff
00aa00da 0f87f9080000         ja 0xaa09d9
00aa00e0 3bce                 cmp ecx, esi
00aa00e2 7427                 je 0xaa010b
00aa00e4 8bc1                 mov eax, ecx
00aa00e6 ba30000000           mov edx, 0x30
00aa00eb f7e2                 mul edx
00aa00ed 3bd6                 cmp edx, esi
00aa00ef 771a                 ja 0xaa010b
00aa00f1 83f8ff               cmp eax, -1
00aa00f4 7715                 ja 0xaa010b
00aa00f6 8d0449               lea eax, [ecx + ecx*2]
00aa00f9 c1e004               shl eax, 4
00aa00fc 50                   push eax
00aa00fd 8d4d14               lea ecx, [ebp + 0x14]
00aa0100 e80bc8fbff           call 0xa5c910
00aa0105 8944242c             mov dword ptr [esp + 0x2c], eax
00aa0109 eb04                 jmp 0xaa010f
00aa010b 8974242c             mov dword ptr [esp + 0x2c], esi
00aa010f 8b4c242c             mov ecx, dword ptr [esp + 0x2c]
00aa0113 8b842464010000       mov eax, dword ptr [esp + 0x164]
00aa011a 8b942468010000       mov edx, dword ptr [esp + 0x168]
00aa0121 57                   push edi
00aa0122 56                   push esi
00aa0123 8bb42468010000       mov esi, dword ptr [esp + 0x168]
00aa012a 53                   push ebx
00aa012b 51                   push ecx
00aa012c 8b4804               mov ecx, dword ptr [eax + 4]
00aa012f 8b4604               mov eax, dword ptr [esi + 4]
00aa0132 52                   push edx
00aa0133 8b5108               mov edx, dword ptr [ecx + 8]
00aa0136 8b4808               mov ecx, dword ptr [eax + 8]
00aa0139 52                   push edx
00aa013a 51                   push ecx
00aa013b 8bcd                 mov ecx, ebp
00aa013d e8aef1ffff           call 0xa9f2f0
00aa0142 8b4604               mov eax, dword ptr [esi + 4]
00aa0145 85c0                 test eax, eax
00aa0147 7417                 je 0xaa0160
00aa0149 8b4810               mov ecx, dword ptr [eax + 0x10]
00aa014c 2b480c               sub ecx, dword ptr [eax + 0xc]
00aa014f b867666666           mov eax, 0x66666667
00aa0154 f7e9                 imul ecx
00aa0156 c1fa03               sar edx, 3
00aa0159 8bc2                 mov eax, edx
00aa015b c1e81f               shr eax, 0x1f
00aa015e 03c2                 add eax, edx
00aa0160 85c0                 test eax, eax
00aa0162 0f8615080000         jbe 0xaa097d
00aa0168 33c9                 xor ecx, ecx
00aa016a 894c2444             mov dword ptr [esp + 0x44], ecx
00aa016e 89442460             mov dword ptr [esp + 0x60], eax
00aa0172 eb0c                 jmp 0xaa0180
00aa0174 8b4c2444             mov ecx, dword ptr [esp + 0x44]
00aa0178 8bb42460010000       mov esi, dword ptr [esp + 0x160]
00aa017f 90                   nop 
00aa0180 8b5604               mov edx, dword ptr [esi + 4]
00aa0183 8b720c               mov esi, dword ptr [edx + 0xc]
00aa0186 8b842464010000       mov eax, dword ptr [esp + 0x164]
00aa018d 8b5004               mov edx, dword ptr [eax + 4]
00aa0190 8b7a0c               mov edi, dword ptr [edx + 0xc]
00aa0193 8b440e08             mov eax, dword ptr [esi + ecx + 8]
00aa0197 03f1                 add esi, ecx
00aa0199 03f9                 add edi, ecx
00aa019b 8b4d04               mov ecx, dword ptr [ebp + 4]
00aa019e 50                   push eax
00aa019f e81c41f8ff           call 0xa242c0
00aa01a4 8b8c2470010000       mov ecx, dword ptr [esp + 0x170]
00aa01ab 51                   push ecx
00aa01ac 8d542424             lea edx, [esp + 0x24]
00aa01b0 52                   push edx
00aa01b1 8bc8                 mov ecx, eax
00aa01b3 e8a86af8ff           call 0xa26c60
00aa01b8 d9460c               fld dword ptr [esi + 0xc]
00aa01bb d9470c               fld dword ptr [edi + 0xc]
00aa01be 8d44243c             lea eax, [esp + 0x3c]
00aa01c2 d8e1                 fsub st(1)
00aa01c4 50                   push eax
00aa01c5 8d4c2444             lea ecx, [esp + 0x44]
00aa01c9 51                   push ecx
00aa01ca d88c2470010000       fmul dword ptr [esp + 0x170]
00aa01d1 51                   push ecx
00aa01d2 8b4c242c             mov ecx, dword ptr [esp + 0x2c]
00aa01d6 d8c1                 fadd st(1)
00aa01d8 d91c24               fstp dword ptr [esp]
00aa01db ddd8                 fstp st(0)
00aa01dd e86e24fbff           call 0xa52650
00aa01e2 d94610               fld dword ptr [esi + 0x10]
00aa01e5 8b542420             mov edx, dword ptr [esp + 0x20]
00aa01e9 d94710               fld dword ptr [edi + 0x10]
00aa01ec 8b4208               mov eax, dword ptr [edx + 8]
00aa01ef d8e1                 fsub st(1)
00aa01f1 8bc8                 mov ecx, eax
00aa01f3 c1e902               shr ecx, 2
00aa01f6 f6c101               test cl, 1
00aa01f9 d88c2468010000       fmul dword ptr [esp + 0x168]
00aa0200 d8c1                 fadd st(1)
00aa0202 d95c2414             fstp dword ptr [esp + 0x14]
00aa0206 ddd8                 fstp st(0)
00aa0208 0f8445030000         je 0xaa0553
00aa020e d9442414             fld dword ptr [esp + 0x14]
00aa0212 33ff                 xor edi, edi
00aa0214 d825d8de2201         fsub dword ptr [0x122ded8]
00aa021a d9e1                 fabs 
00aa021c d81d3cba2901         fcomp dword ptr [0x129ba3c]
00aa0222 dfe0                 fnstsw ax
00aa0224 f6c405               test ah, 5
00aa0227 0f8a8d000000         jp 0xaa02ba
00aa022d 85db                 test ebx, ebx
00aa022f 0f86f4060000         jbe 0xaa0929
00aa0235 8b74242c             mov esi, dword ptr [esp + 0x2c]
00aa0239 8b6c243c             mov ebp, dword ptr [esp + 0x3c]
00aa023d 8b5c2440             mov ebx, dword ptr [esp + 0x40]
00aa0241 83c618               add esi, 0x18
00aa0244 8b425c               mov eax, dword ptr [edx + 0x5c]
00aa0247 8b0cf8               mov ecx, dword ptr [eax + edi*8]
00aa024a 8d04f8               lea eax, [eax + edi*8]
00aa024d 8a410a               mov al, byte ptr [ecx + 0xa]
00aa0250 84c0                 test al, al
00aa0252 7855                 js 0xaa02a9
00aa0254 55                   push ebp
00aa0255 53                   push ebx
00aa0256 8d942434010000       lea edx, [esp + 0x134]
00aa025d 52                   push edx
00aa025e e88dbffaff           call 0xa4c1f0
00aa0263 8d84242c010000       lea eax, [esp + 0x12c]
00aa026a 50                   push eax
00aa026b 8d4ee8               lea ecx, [esi - 0x18]
00aa026e e89d89feff           call 0xa88c10
00aa0273 d984243c010000       fld dword ptr [esp + 0x13c]
00aa027a d846f8               fadd dword ptr [esi - 8]
00aa027d 8b542420             mov edx, dword ptr [esp + 0x20]
00aa0281 d95ef8               fstp dword ptr [esi - 8]
00aa0284 d9842440010000       fld dword ptr [esp + 0x140]
00aa028b d846fc               fadd dword ptr [esi - 4]
00aa028e d95efc               fstp dword ptr [esi - 4]
00aa0291 d9842444010000       fld dword ptr [esp + 0x144]
00aa0298 d806                 fadd dword ptr [esi]
00aa029a d91e                 fstp dword ptr [esi]
00aa029c d9842448010000       fld dword ptr [esp + 0x148]
00aa02a3 d84604               fadd dword ptr [esi + 4]
00aa02a6 d95e04               fstp dword ptr [esi + 4]
00aa02a9 8b442430             mov eax, dword ptr [esp + 0x30]
00aa02ad 47                   inc edi
00aa02ae 83c630               add esi, 0x30
00aa02b1 3bf8                 cmp edi, eax
00aa02b3 728f                 jb 0xaa0244
00aa02b5 e96b060000           jmp 0xaa0925
00aa02ba 85db                 test ebx, ebx
00aa02bc 0f8667060000         jbe 0xaa0929
00aa02c2 8b74242c             mov esi, dword ptr [esp + 0x2c]
00aa02c6 8b6c243c             mov ebp, dword ptr [esp + 0x3c]
00aa02ca 83c618               add esi, 0x18
00aa02cd 8d4900               lea ecx, [ecx]
00aa02d0 8b4a5c               mov ecx, dword ptr [edx + 0x5c]
00aa02d3 8d04f9               lea eax, [ecx + edi*8]
00aa02d6 8b08                 mov ecx, dword ptr [eax]
00aa02d8 8a410a               mov al, byte ptr [ecx + 0xa]
00aa02db 84c0                 test al, al
00aa02dd 0f885f020000         js 0xaa0542
00aa02e3 8b542440             mov edx, dword ptr [esp + 0x40]
00aa02e7 55                   push ebp
00aa02e8 52                   push edx
00aa02e9 8d8424d4000000       lea eax, [esp + 0xd4]
00aa02f0 50                   push eax
00aa02f1 e8fabefaff           call 0xa4c1f0
00aa02f6 d98424d8000000       fld dword ptr [esp + 0xd8]
00aa02fd d81ddcde2201         fcomp dword ptr [0x122dedc]
00aa0303 8b8c24d8000000       mov ecx, dword ptr [esp + 0xd8]
00aa030a 894c2434             mov dword ptr [esp + 0x34], ecx
00aa030e c74424680000803f     mov dword ptr [esp + 0x68], 0x3f800000
00aa0316 dfe0                 fnstsw ax
00aa0318 f6c405               test ah, 5
00aa031b 7a15                 jp 0xaa0332
00aa031d d98424d8000000       fld dword ptr [esp + 0xd8]
00aa0324 c7442468000080bf     mov dword ptr [esp + 0x68], 0xbf800000
00aa032c d9e0                 fchs 
00aa032e d95c2434             fstp dword ptr [esp + 0x34]
00aa0332 d905d8de2201         fld dword ptr [0x122ded8]
00aa0338 d8642434             fsub dword ptr [esp + 0x34]
00aa033c d81d5ca12501         fcomp dword ptr [0x125a15c]
00aa0342 dfe0                 fnstsw ax
00aa0344 f6c441               test ah, 0x41
00aa0347 0f8534010000         jne 0xaa0481
00aa034d d9442434             fld dword ptr [esp + 0x34]
00aa0351 d805d8de2201         fadd dword ptr [0x122ded8]
00aa0357 d80d1ccd2901         fmul dword ptr [0x129cd1c]
00aa035d d9542410             fst dword ptr [esp + 0x10]
00aa0361 d8259cf52201         fsub dword ptr [0x122f59c]
00aa0367 d95c241c             fstp dword ptr [esp + 0x1c]
00aa036b d944241c             fld dword ptr [esp + 0x1c]
00aa036f db5c2454             fistp dword ptr [esp + 0x54]
00aa0373 db442454             fild dword ptr [esp + 0x54]
00aa0377 8b442454             mov eax, dword ptr [esp + 0x54]
00aa037b d86c2410             fsubr dword ptr [esp + 0x10]
00aa037f d905d8de2201         fld dword ptr [0x122ded8]
00aa0385 d8e1                 fsub st(1)
00aa0387 d80c8530b53c01       fmul dword ptr [eax*4 + 0x13cb530]
00aa038e d9c9                 fxch st(1)
00aa0390 d80c8534b53c01       fmul dword ptr [eax*4 + 0x13cb534]
00aa0397 dec1                 faddp st(1)
00aa0399 d80d087f2301         fmul dword ptr [0x1237f08]
00aa039f d95c2428             fstp dword ptr [esp + 0x28]
00aa03a3 d9442434             fld dword ptr [esp + 0x34]
00aa03a7 d84c2434             fmul dword ptr [esp + 0x34]
00aa03ab d82dd8de2201         fsubr dword ptr [0x122ded8]
00aa03b1 d9fa                 fsqrt 
00aa03b3 d95c2418             fstp dword ptr [esp + 0x18]
00aa03b7 d905d8de2201         fld dword ptr [0x122ded8]
00aa03bd d8642414             fsub dword ptr [esp + 0x14]
00aa03c1 d84c2428             fmul dword ptr [esp + 0x28]
00aa03c5 d80d10002301         fmul dword ptr [0x1230010]
00aa03cb d9542410             fst dword ptr [esp + 0x10]
00aa03cf d8259cf52201         fsub dword ptr [0x122f59c]
00aa03d5 d95c241c             fstp dword ptr [esp + 0x1c]
00aa03d9 d944241c             fld dword ptr [esp + 0x1c]
00aa03dd db5c245c             fistp dword ptr [esp + 0x5c]
00aa03e1 db44245c             fild dword ptr [esp + 0x5c]
00aa03e5 8b44245c             mov eax, dword ptr [esp + 0x5c]
00aa03e9 0500ffffff           add eax, 0xffffff00
00aa03ee 25ff030000           and eax, 0x3ff
00aa03f3 d86c2410             fsubr dword ptr [esp + 0x10]
00aa03f7 d905d8de2201         fld dword ptr [0x122ded8]
00aa03fd d8742418             fdiv dword ptr [esp + 0x18]
00aa0401 d95c2410             fstp dword ptr [esp + 0x10]
00aa0405 d905d8de2201         fld dword ptr [0x122ded8]
00aa040b d8e1                 fsub st(1)
00aa040d d80c8550d53c01       fmul dword ptr [eax*4 + 0x13cd550]
00aa0414 d9c9                 fxch st(1)
00aa0416 d80c8554d53c01       fmul dword ptr [eax*4 + 0x13cd554]
00aa041d dec1                 faddp st(1)
00aa041f d84c2410             fmul dword ptr [esp + 0x10]
00aa0423 d95c2418             fstp dword ptr [esp + 0x18]
00aa0427 d9442428             fld dword ptr [esp + 0x28]
00aa042b d84c2414             fmul dword ptr [esp + 0x14]
00aa042f d80d10002301         fmul dword ptr [0x1230010]
00aa0435 d9542428             fst dword ptr [esp + 0x28]
00aa0439 d8259cf52201         fsub dword ptr [0x122f59c]
00aa043f d95c241c             fstp dword ptr [esp + 0x1c]
00aa0443 d944241c             fld dword ptr [esp + 0x1c]
00aa0447 db5c2450             fistp dword ptr [esp + 0x50]
00aa044b db442450             fild dword ptr [esp + 0x50]
00aa044f 8b442450             mov eax, dword ptr [esp + 0x50]
00aa0453 0500ffffff           add eax, 0xffffff00
00aa0458 25ff030000           and eax, 0x3ff
00aa045d d86c2428             fsubr dword ptr [esp + 0x28]
00aa0461 d905d8de2201         fld dword ptr [0x122ded8]
00aa0467 d8e1                 fsub st(1)
00aa0469 d80c8550d53c01       fmul dword ptr [eax*4 + 0x13cd550]
00aa0470 d9c9                 fxch st(1)
00aa0472 d80c8554d53c01       fmul dword ptr [eax*4 + 0x13cd554]
00aa0479 dec1                 faddp st(1)
00aa047b d84c2410             fmul dword ptr [esp + 0x10]
00aa047f eb12                 jmp 0xaa0493
00aa0481 d905d8de2201         fld dword ptr [0x122ded8]
00aa0487 d8642414             fsub dword ptr [esp + 0x14]
00aa048b d95c2418             fstp dword ptr [esp + 0x18]
00aa048f d9442414             fld dword ptr [esp + 0x14]
00aa0493 d84c2468             fmul dword ptr [esp + 0x68]
00aa0497 8d54246c             lea edx, [esp + 0x6c]
00aa049b d98424cc000000       fld dword ptr [esp + 0xcc]
00aa04a2 52                   push edx
00aa04a3 8d4ee8               lea ecx, [esi - 0x18]
00aa04a6 d8c9                 fmul st(1)
00aa04a8 d95c2470             fstp dword ptr [esp + 0x70]
00aa04ac d98424d4000000       fld dword ptr [esp + 0xd4]
00aa04b3 d8c9                 fmul st(1)
00aa04b5 d95c2474             fstp dword ptr [esp + 0x74]
00aa04b9 d98424d8000000       fld dword ptr [esp + 0xd8]
00aa04c0 d8c9                 fmul st(1)
00aa04c2 d95c2478             fstp dword ptr [esp + 0x78]
00aa04c6 d98424dc000000       fld dword ptr [esp + 0xdc]
00aa04cd d8c9                 fmul st(1)
00aa04cf d844241c             fadd dword ptr [esp + 0x1c]
00aa04d3 d95c247c             fstp dword ptr [esp + 0x7c]
00aa04d7 ddd8                 fstp st(0)
00aa04d9 e83287feff           call 0xa88c10
00aa04de d98424dc000000       fld dword ptr [esp + 0xdc]
00aa04e5 8b542420             mov edx, dword ptr [esp + 0x20]
00aa04e9 d84c2414             fmul dword ptr [esp + 0x14]
00aa04ed d98424e0000000       fld dword ptr [esp + 0xe0]
00aa04f4 d84c2414             fmul dword ptr [esp + 0x14]
00aa04f8 d98424e4000000       fld dword ptr [esp + 0xe4]
00aa04ff d84c2414             fmul dword ptr [esp + 0x14]
00aa0503 d99c2424010000       fstp dword ptr [esp + 0x124]
00aa050a d98424e8000000       fld dword ptr [esp + 0xe8]
00aa0511 d84c2414             fmul dword ptr [esp + 0x14]
00aa0515 d99c2428010000       fstp dword ptr [esp + 0x128]
00aa051c d9c9                 fxch st(1)
00aa051e d846f8               fadd dword ptr [esi - 8]
00aa0521 d95ef8               fstp dword ptr [esi - 8]
00aa0524 d846fc               fadd dword ptr [esi - 4]
00aa0527 d95efc               fstp dword ptr [esi - 4]
00aa052a d9842424010000       fld dword ptr [esp + 0x124]
00aa0531 d806                 fadd dword ptr [esi]
00aa0533 d91e                 fstp dword ptr [esi]
00aa0535 d9842428010000       fld dword ptr [esp + 0x128]
00aa053c d84604               fadd dword ptr [esi + 4]
00aa053f d95e04               fstp dword ptr [esi + 4]
00aa0542 47                   inc edi
00aa0543 83c630               add esi, 0x30
00aa0546 3bfb                 cmp edi, ebx
00aa0548 0f8282fdffff         jb 0xaa02d0
00aa054e e9d6030000           jmp 0xaa0929
00aa0553 d1e8                 shr eax, 1
00aa0555 a801                 test al, 1
00aa0557 0f84cc030000         je 0xaa0929
00aa055d d9442414             fld dword ptr [esp + 0x14]
00aa0561 d825d8de2201         fsub dword ptr [0x122ded8]
00aa0567 d9e1                 fabs 
00aa0569 d81d3cba2901         fcomp dword ptr [0x129ba3c]
00aa056f dfe0                 fnstsw ax
00aa0571 f6c405               test ah, 5
00aa0574 7a4a                 jp 0xaa05c0
00aa0576 33f6                 xor esi, esi
00aa0578 85db                 test ebx, ebx
00aa057a 0f86a9030000         jbe 0xaa0929
00aa0580 8b7c242c             mov edi, dword ptr [esp + 0x2c]
00aa0584 8b6c243c             mov ebp, dword ptr [esp + 0x3c]
00aa0588 8b5c2440             mov ebx, dword ptr [esp + 0x40]
00aa058c 8d642400             lea esp, [esp]
00aa0590 8b4268               mov eax, dword ptr [edx + 0x68]
00aa0593 85c0                 test eax, eax
00aa0595 7406                 je 0xaa059d
00aa0597 803c3000             cmp byte ptr [eax + esi], 0
00aa059b 7412                 je 0xaa05af
00aa059d 8b425c               mov eax, dword ptr [edx + 0x5c]
00aa05a0 8b0cf0               mov ecx, dword ptr [eax + esi*8]
00aa05a3 55                   push ebp
00aa05a4 53                   push ebx
00aa05a5 57                   push edi
00aa05a6 e845bcfaff           call 0xa4c1f0
00aa05ab 8b542420             mov edx, dword ptr [esp + 0x20]
00aa05af 8b442430             mov eax, dword ptr [esp + 0x30]
00aa05b3 46                   inc esi
00aa05b4 83c730               add edi, 0x30
00aa05b7 3bf0                 cmp esi, eax
00aa05b9 72d5                 jb 0xaa0590
00aa05bb e965030000           jmp 0xaa0925
00aa05c0 33ff                 xor edi, edi
00aa05c2 85db                 test ebx, ebx
00aa05c4 0f865f030000         jbe 0xaa0929
00aa05ca 8b74242c             mov esi, dword ptr [esp + 0x2c]
00aa05ce 8b6c243c             mov ebp, dword ptr [esp + 0x3c]
00aa05d2 8b5c2440             mov ebx, dword ptr [esp + 0x40]
00aa05d6 83c618               add esi, 0x18
00aa05d9 8da42400000000       lea esp, [esp]
00aa05e0 8b4268               mov eax, dword ptr [edx + 0x68]
00aa05e3 85c0                 test eax, eax
00aa05e5 740a                 je 0xaa05f1
00aa05e7 803c3800             cmp byte ptr [eax + edi], 0
00aa05eb 0f8424030000         je 0xaa0915
00aa05f1 8b525c               mov edx, dword ptr [edx + 0x5c]
00aa05f4 55                   push ebp
00aa05f5 53                   push ebx
00aa05f6 8d8c24a4000000       lea ecx, [esp + 0xa4]
00aa05fd 51                   push ecx
00aa05fe 8b0cfa               mov ecx, dword ptr [edx + edi*8]
00aa0601 e8eabbfaff           call 0xa4c1f0
00aa0606 d984249c000000       fld dword ptr [esp + 0x9c]
00aa060d d84ee8               fmul dword ptr [esi - 0x18]
00aa0610 c74424580000803f     mov dword ptr [esp + 0x58], 0x3f800000
00aa0618 d98424a0000000       fld dword ptr [esp + 0xa0]
00aa061f d84eec               fmul dword ptr [esi - 0x14]
00aa0622 dec1                 faddp st(1)
00aa0624 d98424a4000000       fld dword ptr [esp + 0xa4]
00aa062b d84ef0               fmul dword ptr [esi - 0x10]
00aa062e dec1                 faddp st(1)
00aa0630 d98424a8000000       fld dword ptr [esp + 0xa8]
00aa0637 d84ef4               fmul dword ptr [esi - 0xc]
00aa063a dec1                 faddp st(1)
00aa063c d95c2410             fstp dword ptr [esp + 0x10]
00aa0640 d9442410             fld dword ptr [esp + 0x10]
00aa0644 d81ddcde2201         fcomp dword ptr [0x122dedc]
00aa064a dfe0                 fnstsw ax
00aa064c f6c405               test ah, 5
00aa064f 7a12                 jp 0xaa0663
00aa0651 d9442410             fld dword ptr [esp + 0x10]
00aa0655 c7442458000080bf     mov dword ptr [esp + 0x58], 0xbf800000
00aa065d d9e0                 fchs 
00aa065f d95c2410             fstp dword ptr [esp + 0x10]
00aa0663 d905d8de2201         fld dword ptr [0x122ded8]
00aa0669 d8642410             fsub dword ptr [esp + 0x10]
00aa066d d81d5ca12501         fcomp dword ptr [0x125a15c]
00aa0673 dfe0                 fnstsw ax
00aa0675 f6c441               test ah, 0x41
00aa0678 0f8534010000         jne 0xaa07b2
00aa067e d9442410             fld dword ptr [esp + 0x10]
00aa0682 d805d8de2201         fadd dword ptr [0x122ded8]
00aa0688 d80d1ccd2901         fmul dword ptr [0x129cd1c]
00aa068e d9542418             fst dword ptr [esp + 0x18]
00aa0692 d8259cf52201         fsub dword ptr [0x122f59c]
00aa0698 d95c241c             fstp dword ptr [esp + 0x1c]
00aa069c d944241c             fld dword ptr [esp + 0x1c]
00aa06a0 db5c2448             fistp dword ptr [esp + 0x48]
00aa06a4 db442448             fild dword ptr [esp + 0x48]
00aa06a8 8b442448             mov eax, dword ptr [esp + 0x48]
00aa06ac d86c2418             fsubr dword ptr [esp + 0x18]
00aa06b0 d905d8de2201         fld dword ptr [0x122ded8]
00aa06b6 d8e1                 fsub st(1)
00aa06b8 d80c8530b53c01       fmul dword ptr [eax*4 + 0x13cb530]
00aa06bf d9c9                 fxch st(1)
00aa06c1 d80c8534b53c01       fmul dword ptr [eax*4 + 0x13cb534]
00aa06c8 dec1                 faddp st(1)
00aa06ca d80d087f2301         fmul dword ptr [0x1237f08]
00aa06d0 d95c2428             fstp dword ptr [esp + 0x28]
00aa06d4 d9442410             fld dword ptr [esp + 0x10]
00aa06d8 d84c2410             fmul dword ptr [esp + 0x10]
00aa06dc d82dd8de2201         fsubr dword ptr [0x122ded8]
00aa06e2 d9fa                 fsqrt 
00aa06e4 d95c2410             fstp dword ptr [esp + 0x10]
00aa06e8 d905d8de2201         fld dword ptr [0x122ded8]
00aa06ee d8642414             fsub dword ptr [esp + 0x14]
00aa06f2 d84c2428             fmul dword ptr [esp + 0x28]
00aa06f6 d80d10002301         fmul dword ptr [0x1230010]
00aa06fc d9542418             fst dword ptr [esp + 0x18]
00aa0700 d8259cf52201         fsub dword ptr [0x122f59c]
00aa0706 d95c241c             fstp dword ptr [esp + 0x1c]
00aa070a d944241c             fld dword ptr [esp + 0x1c]
00aa070e db5c2464             fistp dword ptr [esp + 0x64]
00aa0712 db442464             fild dword ptr [esp + 0x64]
00aa0716 8b442464             mov eax, dword ptr [esp + 0x64]
00aa071a 0500ffffff           add eax, 0xffffff00
00aa071f 25ff030000           and eax, 0x3ff
00aa0724 d86c2418             fsubr dword ptr [esp + 0x18]
00aa0728 d905d8de2201         fld dword ptr [0x122ded8]
00aa072e d8742410             fdiv dword ptr [esp + 0x10]
00aa0732 d95c241c             fstp dword ptr [esp + 0x1c]
00aa0736 d905d8de2201         fld dword ptr [0x122ded8]
00aa073c d8e1                 fsub st(1)
00aa073e d80c8550d53c01       fmul dword ptr [eax*4 + 0x13cd550]
00aa0745 d9c9                 fxch st(1)
00aa0747 d80c8554d53c01       fmul dword ptr [eax*4 + 0x13cd554]
00aa074e dec1                 faddp st(1)
00aa0750 d84c241c             fmul dword ptr [esp + 0x1c]
00aa0754 d95c2410             fstp dword ptr [esp + 0x10]
00aa0758 d9442428             fld dword ptr [esp + 0x28]
00aa075c d84c2414             fmul dword ptr [esp + 0x14]
00aa0760 d80d10002301         fmul dword ptr [0x1230010]
00aa0766 d9542418             fst dword ptr [esp + 0x18]
00aa076a d8259cf52201         fsub dword ptr [0x122f59c]
00aa0770 d95c2428             fstp dword ptr [esp + 0x28]
00aa0774 d9442428             fld dword ptr [esp + 0x28]
00aa0778 db5c244c             fistp dword ptr [esp + 0x4c]
00aa077c db44244c             fild dword ptr [esp + 0x4c]
00aa0780 8b44244c             mov eax, dword ptr [esp + 0x4c]
00aa0784 0500ffffff           add eax, 0xffffff00
00aa0789 25ff030000           and eax, 0x3ff
00aa078e d86c2418             fsubr dword ptr [esp + 0x18]
00aa0792 d905d8de2201         fld dword ptr [0x122ded8]
00aa0798 d8e1                 fsub st(1)
00aa079a d80c8550d53c01       fmul dword ptr [eax*4 + 0x13cd550]
00aa07a1 d9c9                 fxch st(1)
00aa07a3 d80c8554d53c01       fmul dword ptr [eax*4 + 0x13cd554]
00aa07aa dec1                 faddp st(1)
00aa07ac d84c241c             fmul dword ptr [esp + 0x1c]
00aa07b0 eb12                 jmp 0xaa07c4
00aa07b2 d905d8de2201         fld dword ptr [0x122ded8]
00aa07b8 d8642414             fsub dword ptr [esp + 0x14]
00aa07bc d95c2410             fstp dword ptr [esp + 0x10]
00aa07c0 d9442414             fld dword ptr [esp + 0x14]
00aa07c4 d84c2458             fmul dword ptr [esp + 0x58]
00aa07c8 d984249c000000       fld dword ptr [esp + 0x9c]
00aa07cf d8c9                 fmul st(1)
00aa07d1 d9442410             fld dword ptr [esp + 0x10]
00aa07d5 d84ee8               fmul dword ptr [esi - 0x18]
00aa07d8 dec1                 faddp st(1)
00aa07da d99c248c000000       fstp dword ptr [esp + 0x8c]
00aa07e1 8b84248c000000       mov eax, dword ptr [esp + 0x8c]
00aa07e8 d9442410             fld dword ptr [esp + 0x10]
00aa07ec d84eec               fmul dword ptr [esi - 0x14]
00aa07ef d98424a0000000       fld dword ptr [esp + 0xa0]
00aa07f6 d8ca                 fmul st(2)
00aa07f8 dec1                 faddp st(1)
00aa07fa d99c2490000000       fstp dword ptr [esp + 0x90]
00aa0801 8b8c2490000000       mov ecx, dword ptr [esp + 0x90]
00aa0808 d9442410             fld dword ptr [esp + 0x10]
00aa080c d84ef0               fmul dword ptr [esi - 0x10]
00aa080f d98424a4000000       fld dword ptr [esp + 0xa4]
00aa0816 d8ca                 fmul st(2)
00aa0818 dec1                 faddp st(1)
00aa081a d99c2494000000       fstp dword ptr [esp + 0x94]
00aa0821 8b942494000000       mov edx, dword ptr [esp + 0x94]
00aa0828 d9442410             fld dword ptr [esp + 0x10]
00aa082c d84ef4               fmul dword ptr [esi - 0xc]
00aa082f 8946e8               mov dword ptr [esi - 0x18], eax
00aa0832 d98424a8000000       fld dword ptr [esp + 0xa8]
00aa0839 894eec               mov dword ptr [esi - 0x14], ecx
00aa083c d8ca                 fmul st(2)
00aa083e 8956f0               mov dword ptr [esi - 0x10], edx
00aa0841 dec1                 faddp st(1)
00aa0843 d99c2498000000       fstp dword ptr [esp + 0x98]
00aa084a 8b842498000000       mov eax, dword ptr [esp + 0x98]
00aa0851 8946f4               mov dword ptr [esi - 0xc], eax
00aa0854 ddd8                 fstp st(0)
00aa0856 d98424ac000000       fld dword ptr [esp + 0xac]
00aa085d d866f8               fsub dword ptr [esi - 8]
00aa0860 d98424b0000000       fld dword ptr [esp + 0xb0]
00aa0867 d866fc               fsub dword ptr [esi - 4]
00aa086a d98424b4000000       fld dword ptr [esp + 0xb4]
00aa0871 d826                 fsub dword ptr [esi]
00aa0873 d99c2404010000       fstp dword ptr [esp + 0x104]
00aa087a d98424b8000000       fld dword ptr [esp + 0xb8]
00aa0881 d86604               fsub dword ptr [esi + 4]
00aa0884 d99c2408010000       fstp dword ptr [esp + 0x108]
00aa088b d9c9                 fxch st(1)
00aa088d d84c2414             fmul dword ptr [esp + 0x14]
00aa0891 d99c240c010000       fstp dword ptr [esp + 0x10c]
00aa0898 d84c2414             fmul dword ptr [esp + 0x14]
00aa089c d9842404010000       fld dword ptr [esp + 0x104]
00aa08a3 d84c2414             fmul dword ptr [esp + 0x14]
00aa08a7 d9842408010000       fld dword ptr [esp + 0x108]
00aa08ae d84c2414             fmul dword ptr [esp + 0x14]
00aa08b2 d99c2418010000       fstp dword ptr [esp + 0x118]
00aa08b9 d984240c010000       fld dword ptr [esp + 0x10c]
00aa08c0 d846f8               fadd dword ptr [esi - 8]
00aa08c3 d95c247c             fstp dword ptr [esp + 0x7c]
00aa08c7 8b4c247c             mov ecx, dword ptr [esp + 0x7c]
00aa08cb d9c9                 fxch st(1)
00aa08cd d846fc               fadd dword ptr [esi - 4]
00aa08d0 d99c2480000000       fstp dword ptr [esp + 0x80]
00aa08d7 8b942480000000       mov edx, dword ptr [esp + 0x80]
00aa08de d806                 fadd dword ptr [esi]
00aa08e0 d99c2484000000       fstp dword ptr [esp + 0x84]
00aa08e7 8b842484000000       mov eax, dword ptr [esp + 0x84]
00aa08ee d9842418010000       fld dword ptr [esp + 0x118]
00aa08f5 d84604               fadd dword ptr [esi + 4]
00aa08f8 894ef8               mov dword ptr [esi - 8], ecx
00aa08fb 8956fc               mov dword ptr [esi - 4], edx
00aa08fe 8b542420             mov edx, dword ptr [esp + 0x20]
00aa0902 d99c2488000000       fstp dword ptr [esp + 0x88]
00aa0909 8b8c2488000000       mov ecx, dword ptr [esp + 0x88]
00aa0910 8906                 mov dword ptr [esi], eax
00aa0912 894e04               mov dword ptr [esi + 4], ecx
00aa0915 8b442430             mov eax, dword ptr [esp + 0x30]
00aa0919 47                   inc edi
00aa091a 83c630               add esi, 0x30
00aa091d 3bf8                 cmp edi, eax
00aa091f 0f82bbfcffff         jb 0xaa05e0
00aa0925 8b5c2430             mov ebx, dword ptr [esp + 0x30]
00aa0929 8b442424             mov eax, dword ptr [esp + 0x24]
00aa092d 33f6                 xor esi, esi
00aa092f 3bc6                 cmp eax, esi
00aa0931 741d                 je 0xaa0950
00aa0933 ff08                 dec dword ptr [eax]
00aa0935 8b442424             mov eax, dword ptr [esp + 0x24]
00aa0939 3930                 cmp dword ptr [eax], esi
00aa093b 7513                 jne 0xaa0950
00aa093d 8b4808               mov ecx, dword ptr [eax + 8]
00aa0940 ff5004               call dword ptr [eax + 4]
00aa0943 8b542424             mov edx, dword ptr [esp + 0x24]
00aa0947 52                   push edx
00aa0948 e86fe01500           call 0xbfe9bc
00aa094d 83c404               add esp, 4
00aa0950 8b4c2444             mov ecx, dword ptr [esp + 0x44]
00aa0954 8b442460             mov eax, dword ptr [esp + 0x60]
00aa0958 8b6c2438             mov ebp, dword ptr [esp + 0x38]
00aa095c 83c114               add ecx, 0x14
00aa095f 48                   dec eax
00aa0960 89742420             mov dword ptr [esp + 0x20], esi
00aa0964 89742424             mov dword ptr [esp + 0x24], esi
00aa0968 894c2444             mov dword ptr [esp + 0x44], ecx
00aa096c 89442460             mov dword ptr [esp + 0x60], eax
00aa0970 0f85fef7ffff         jne 0xaa0174
00aa0976 8bbc2470010000       mov edi, dword ptr [esp + 0x170]
00aa097d 8bb4246c010000       mov esi, dword ptr [esp + 0x16c]
00aa0984 85f6                 test esi, esi
00aa0986 740e                 je 0xaa0996
00aa0988 8b44242c             mov eax, dword ptr [esp + 0x2c]
00aa098c 57                   push edi
00aa098d 56                   push esi
00aa098e 50                   push eax
00aa098f 8bcd                 mov ecx, ebp
00aa0991 e80ad6ffff           call 0xa9dfa0
00aa0996 8bbc247c010000       mov edi, dword ptr [esp + 0x17c]
00aa099d 8b8c2470010000       mov ecx, dword ptr [esp + 0x170]
00aa09a4 8bac2474010000       mov ebp, dword ptr [esp + 0x174]
00aa09ab 8b54242c             mov edx, dword ptr [esp + 0x2c]
00aa09af 57                   push edi
00aa09b0 51                   push ecx
00aa09b1 8b4c2440             mov ecx, dword ptr [esp + 0x40]
00aa09b5 55                   push ebp
00aa09b6 52                   push edx
00aa09b7 e824d8ffff           call 0xa9e1e0
00aa09bc 85f6                 test esi, esi
00aa09be 740d                 je 0xaa09cd
00aa09c0 8b4c2438             mov ecx, dword ptr [esp + 0x38]
00aa09c4 57                   push edi
00aa09c5 53                   push ebx
00aa09c6 55                   push ebp
00aa09c7 56                   push esi
00aa09c8 e883cdffff           call 0xa9d750
00aa09cd 8b4c2438             mov ecx, dword ptr [esp + 0x38]
00aa09d1 83c114               add ecx, 0x14
00aa09d4 e847bdfbff           call 0xa5c720
00aa09d9 5f                   pop edi
00aa09da 5e                   pop esi
00aa09db 5d                   pop ebp
00aa09dc 5b                   pop ebx
00aa09dd 81c44c010000         add esp, 0x14c
00aa09e3 c22000               ret 0x20
