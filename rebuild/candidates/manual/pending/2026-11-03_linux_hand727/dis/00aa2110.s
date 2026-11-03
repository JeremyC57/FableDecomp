00aa2110 55                   push ebp
00aa2111 8bec                 mov ebp, esp
00aa2113 83e4f0               and esp, 0xfffffff0
00aa2116 81ec14020000         sub esp, 0x214
00aa211c 53                   push ebx
00aa211d 56                   push esi
00aa211e 57                   push edi
00aa211f 8d442420             lea eax, [esp + 0x20]
00aa2123 8d7908               lea edi, [ecx + 8]
00aa2126 50                   push eax
00aa2127 894c2440             mov dword ptr [esp + 0x40], ecx
00aa212b 6a01                 push 1
00aa212d 8bcf                 mov ecx, edi
00aa212f c744242800000000     mov dword ptr [esp + 0x28], 0
00aa2137 e8f5279cff           call 0x464931
00aa213c 8b0f                 mov ecx, dword ptr [edi]
00aa213e 8b4518               mov eax, dword ptr [ebp + 0x18]
00aa2141 8b5514               mov edx, dword ptr [ebp + 0x14]
00aa2144 8901                 mov dword ptr [ecx], eax
00aa2146 6bc03c               imul eax, eax, 0x3c
00aa2149 8b8a9c000000         mov ecx, dword ptr [edx + 0x9c]
00aa214f 8b740804             mov esi, dword ptr [eax + ecx + 4]
00aa2153 85f6                 test esi, esi
00aa2155 bb04000000           mov ebx, 4
00aa215a 7c48                 jl 0xaa21a4
00aa215c 8d642400             lea esp, [esp]
00aa2160 8b4704               mov eax, dword ptr [edi + 4]
00aa2163 3b4708               cmp eax, dword ptr [edi + 8]
00aa2166 89742420             mov dword ptr [esp + 0x20], esi
00aa216a 740b                 je 0xaa2177
00aa216c 85c0                 test eax, eax
00aa216e 7402                 je 0xaa2172
00aa2170 8930                 mov dword ptr [eax], esi
00aa2172 015f04               add dword ptr [edi + 4], ebx
00aa2175 eb19                 jmp 0xaa2190
00aa2177 6a01                 push 1
00aa2179 6a01                 push 1
00aa217b 8d942487000000       lea edx, [esp + 0x87]
00aa2182 52                   push edx
00aa2183 8d4c242c             lea ecx, [esp + 0x2c]
00aa2187 51                   push ecx
00aa2188 50                   push eax
00aa2189 8bcf                 mov ecx, edi
00aa218b e83e299cff           call 0x464ace
00aa2190 8b5514               mov edx, dword ptr [ebp + 0x14]
00aa2193 6bf63c               imul esi, esi, 0x3c
00aa2196 8b829c000000         mov eax, dword ptr [edx + 0x9c]
00aa219c 8b740604             mov esi, dword ptr [esi + eax + 4]
00aa21a0 85f6                 test esi, esi
00aa21a2 7dbc                 jge 0xaa2160
00aa21a4 8b07                 mov eax, dword ptr [edi]
00aa21a6 8b7704               mov esi, dword ptr [edi + 4]
00aa21a9 2bf0                 sub esi, eax
00aa21ab c1fe02               sar esi, 2
00aa21ae 33c0                 xor eax, eax
00aa21b0 8974241c             mov dword ptr [esp + 0x1c], esi
00aa21b4 8d0cb6               lea ecx, [esi + esi*4]
00aa21b7 0f8c16110000         jl 0xaa32d3
00aa21bd 7f08                 jg 0xaa21c7
00aa21bf 85c9                 test ecx, ecx
00aa21c1 0f820c110000         jb 0xaa32d3
00aa21c7 85c0                 test eax, eax
00aa21c9 0f8f04110000         jg 0xaa32d3
00aa21cf 7c0c                 jl 0xaa21dd
00aa21d1 81f9ffffff7f         cmp ecx, 0x7fffffff
00aa21d7 0f87f6100000         ja 0xaa32d3
00aa21dd 85c9                 test ecx, ecx
00aa21df 7427                 je 0xaa2208
00aa21e1 8bc1                 mov eax, ecx
00aa21e3 ba30000000           mov edx, 0x30
00aa21e8 f7e2                 mul edx
00aa21ea 85d2                 test edx, edx
00aa21ec 771a                 ja 0xaa2208
00aa21ee 83f8ff               cmp eax, -1
00aa21f1 7715                 ja 0xaa2208
00aa21f3 8b5c243c             mov ebx, dword ptr [esp + 0x3c]
00aa21f7 8d0449               lea eax, [ecx + ecx*2]
00aa21fa c1e004               shl eax, 4
00aa21fd 50                   push eax
00aa21fe 8d4b14               lea ecx, [ebx + 0x14]
00aa2201 e80aa7fbff           call 0xa5c910
00aa2206 eb06                 jmp 0xaa220e
00aa2208 8b5c243c             mov ebx, dword ptr [esp + 0x3c]
00aa220c 33c0                 xor eax, eax
00aa220e 8b4d14               mov ecx, dword ptr [ebp + 0x14]
00aa2211 8b5510               mov edx, dword ptr [ebp + 0x10]
00aa2214 57                   push edi
00aa2215 51                   push ecx
00aa2216 6a00                 push 0
00aa2218 56                   push esi
00aa2219 8b7508               mov esi, dword ptr [ebp + 8]
00aa221c 50                   push eax
00aa221d 89442448             mov dword ptr [esp + 0x48], eax
00aa2221 8b450c               mov eax, dword ptr [ebp + 0xc]
00aa2224 8b4804               mov ecx, dword ptr [eax + 4]
00aa2227 8b4604               mov eax, dword ptr [esi + 4]
00aa222a 52                   push edx
00aa222b 8b5108               mov edx, dword ptr [ecx + 8]
00aa222e 8b4808               mov ecx, dword ptr [eax + 8]
00aa2231 52                   push edx
00aa2232 51                   push ecx
00aa2233 8bcb                 mov ecx, ebx
00aa2235 e886ebffff           call 0xaa0dc0
00aa223a 8b4604               mov eax, dword ptr [esi + 4]
00aa223d 85c0                 test eax, eax
00aa223f 7417                 je 0xaa2258
00aa2241 8b4810               mov ecx, dword ptr [eax + 0x10]
00aa2244 2b480c               sub ecx, dword ptr [eax + 0xc]
00aa2247 b867666666           mov eax, 0x66666667
00aa224c f7e9                 imul ecx
00aa224e c1fa03               sar edx, 3
00aa2251 8bc2                 mov eax, edx
00aa2253 c1e81f               shr eax, 0x1f
00aa2256 03c2                 add eax, edx
00aa2258 85c0                 test eax, eax
00aa225a 0f8640080000         jbe 0xaa2aa0
00aa2260 c744243800000000     mov dword ptr [esp + 0x38], 0
00aa2268 89442420             mov dword ptr [esp + 0x20], eax
00aa226c 8d642400             lea esp, [esp]
00aa2270 8b5508               mov edx, dword ptr [ebp + 8]
00aa2273 8b4204               mov eax, dword ptr [edx + 4]
00aa2276 8b700c               mov esi, dword ptr [eax + 0xc]
00aa2279 8b4d0c               mov ecx, dword ptr [ebp + 0xc]
00aa227c 8b5104               mov edx, dword ptr [ecx + 4]
00aa227f 8b442438             mov eax, dword ptr [esp + 0x38]
00aa2283 8b7a0c               mov edi, dword ptr [edx + 0xc]
00aa2286 8b4b04               mov ecx, dword ptr [ebx + 4]
00aa2289 03f0                 add esi, eax
00aa228b 03f8                 add edi, eax
00aa228d 8b4608               mov eax, dword ptr [esi + 8]
00aa2290 50                   push eax
00aa2291 e82a20f8ff           call 0xa242c0
00aa2296 8b4d14               mov ecx, dword ptr [ebp + 0x14]
00aa2299 51                   push ecx
00aa229a 8d54242c             lea edx, [esp + 0x2c]
00aa229e 52                   push edx
00aa229f 8bc8                 mov ecx, eax
00aa22a1 e8ba49f8ff           call 0xa26c60
00aa22a6 d9460c               fld dword ptr [esi + 0xc]
00aa22a9 d9470c               fld dword ptr [edi + 0xc]
00aa22ac 8d442478             lea eax, [esp + 0x78]
00aa22b0 d8e1                 fsub st(1)
00aa22b2 50                   push eax
00aa22b3 8d4c2428             lea ecx, [esp + 0x28]
00aa22b7 51                   push ecx
00aa22b8 d84d10               fmul dword ptr [ebp + 0x10]
00aa22bb 51                   push ecx
00aa22bc 8b4c2434             mov ecx, dword ptr [esp + 0x34]
00aa22c0 d8c1                 fadd st(1)
00aa22c2 d91c24               fstp dword ptr [esp]
00aa22c5 ddd8                 fstp st(0)
00aa22c7 e88403fbff           call 0xa52650
00aa22cc d94610               fld dword ptr [esi + 0x10]
00aa22cf 8b542428             mov edx, dword ptr [esp + 0x28]
00aa22d3 d94710               fld dword ptr [edi + 0x10]
00aa22d6 8b4208               mov eax, dword ptr [edx + 8]
00aa22d9 d8e1                 fsub st(1)
00aa22db 8bc8                 mov ecx, eax
00aa22dd c1e902               shr ecx, 2
00aa22e0 f6c101               test cl, 1
00aa22e3 d84d10               fmul dword ptr [ebp + 0x10]
00aa22e6 d8c1                 fadd st(1)
00aa22e8 d95c2418             fstp dword ptr [esp + 0x18]
00aa22ec ddd8                 fstp st(0)
00aa22ee 0f8491030000         je 0xaa2685
00aa22f4 d9442418             fld dword ptr [esp + 0x18]
00aa22f8 33ff                 xor edi, edi
00aa22fa d825d8de2201         fsub dword ptr [0x122ded8]
00aa2300 d9e1                 fabs 
00aa2302 d81d3cba2901         fcomp dword ptr [0x129ba3c]
00aa2308 dfe0                 fnstsw ax
00aa230a f6c405               test ah, 5
00aa230d 8b44241c             mov eax, dword ptr [esp + 0x1c]
00aa2311 0f8aa2000000         jp 0xaa23b9
00aa2317 85c0                 test eax, eax
00aa2319 0f8637070000         jbe 0xaa2a56
00aa231f 8b742434             mov esi, dword ptr [esp + 0x34]
00aa2323 8b5c2478             mov ebx, dword ptr [esp + 0x78]
00aa2327 83c618               add esi, 0x18
00aa232a 8d9b00000000         lea ebx, [ebx]
00aa2330 8b44243c             mov eax, dword ptr [esp + 0x3c]
00aa2334 83c008               add eax, 8
00aa2337 8b00                 mov eax, dword ptr [eax]
00aa2339 8b0cb8               mov ecx, dword ptr [eax + edi*4]
00aa233c 8b425c               mov eax, dword ptr [edx + 0x5c]
00aa233f 8d04c8               lea eax, [eax + ecx*8]
00aa2342 8b08                 mov ecx, dword ptr [eax]
00aa2344 8a410a               mov al, byte ptr [ecx + 0xa]
00aa2347 84c0                 test al, al
00aa2349 7859                 js 0xaa23a4
00aa234b 8b542424             mov edx, dword ptr [esp + 0x24]
00aa234f 53                   push ebx
00aa2350 52                   push edx
00aa2351 8d8424f8010000       lea eax, [esp + 0x1f8]
00aa2358 50                   push eax
00aa2359 e8929efaff           call 0xa4c1f0
00aa235e 8d8c24f0010000       lea ecx, [esp + 0x1f0]
00aa2365 51                   push ecx
00aa2366 8d4ee8               lea ecx, [esi - 0x18]
00aa2369 e8a268feff           call 0xa88c10
00aa236e d9842400020000       fld dword ptr [esp + 0x200]
00aa2375 d846f8               fadd dword ptr [esi - 8]
00aa2378 8b542428             mov edx, dword ptr [esp + 0x28]
00aa237c d95ef8               fstp dword ptr [esi - 8]
00aa237f d9842404020000       fld dword ptr [esp + 0x204]
00aa2386 d846fc               fadd dword ptr [esi - 4]
00aa2389 d95efc               fstp dword ptr [esi - 4]
00aa238c d9842408020000       fld dword ptr [esp + 0x208]
00aa2393 d806                 fadd dword ptr [esi]
00aa2395 d91e                 fstp dword ptr [esi]
00aa2397 d984240c020000       fld dword ptr [esp + 0x20c]
00aa239e d84604               fadd dword ptr [esi + 4]
00aa23a1 d95e04               fstp dword ptr [esi + 4]
00aa23a4 8b44241c             mov eax, dword ptr [esp + 0x1c]
00aa23a8 47                   inc edi
00aa23a9 83c630               add esi, 0x30
00aa23ac 3bf8                 cmp edi, eax
00aa23ae 7280                 jb 0xaa2330
00aa23b0 8b5c243c             mov ebx, dword ptr [esp + 0x3c]
00aa23b4 e99d060000           jmp 0xaa2a56
00aa23b9 85c0                 test eax, eax
00aa23bb 0f8695060000         jbe 0xaa2a56
00aa23c1 8b742434             mov esi, dword ptr [esp + 0x34]
00aa23c5 83c618               add esi, 0x18
00aa23c8 eb06                 jmp 0xaa23d0
00aa23ca 8d9b00000000         lea ebx, [ebx]
00aa23d0 8b4308               mov eax, dword ptr [ebx + 8]
00aa23d3 8b0cb8               mov ecx, dword ptr [eax + edi*4]
00aa23d6 8b425c               mov eax, dword ptr [edx + 0x5c]
00aa23d9 8d04c8               lea eax, [eax + ecx*8]
00aa23dc 8b08                 mov ecx, dword ptr [eax]
00aa23de 8a410a               mov al, byte ptr [ecx + 0xa]
00aa23e1 84c0                 test al, al
00aa23e3 0f8887020000         js 0xaa2670
00aa23e9 8b542478             mov edx, dword ptr [esp + 0x78]
00aa23ed 8b442424             mov eax, dword ptr [esp + 0x24]
00aa23f1 52                   push edx
00aa23f2 50                   push eax
00aa23f3 8d942458010000       lea edx, [esp + 0x158]
00aa23fa 52                   push edx
00aa23fb e8f09dfaff           call 0xa4c1f0
00aa2400 d984245c010000       fld dword ptr [esp + 0x15c]
00aa2407 d81ddcde2201         fcomp dword ptr [0x122dedc]
00aa240d 8b84245c010000       mov eax, dword ptr [esp + 0x15c]
00aa2414 89442474             mov dword ptr [esp + 0x74], eax
00aa2418 c784248c0000000000803f mov dword ptr [esp + 0x8c], 0x3f800000
00aa2423 dfe0                 fnstsw ax
00aa2425 f6c405               test ah, 5
00aa2428 7a18                 jp 0xaa2442
00aa242a d984245c010000       fld dword ptr [esp + 0x15c]
00aa2431 c784248c000000000080bf mov dword ptr [esp + 0x8c], 0xbf800000
00aa243c d9e0                 fchs 
00aa243e d95c2474             fstp dword ptr [esp + 0x74]
00aa2442 d905d8de2201         fld dword ptr [0x122ded8]
00aa2448 d8642474             fsub dword ptr [esp + 0x74]
00aa244c d81d5ca12501         fcomp dword ptr [0x125a15c]
00aa2452 dfe0                 fnstsw ax
00aa2454 f6c441               test ah, 0x41
00aa2457 0f854f010000         jne 0xaa25ac
00aa245d d9442474             fld dword ptr [esp + 0x74]
00aa2461 d805d8de2201         fadd dword ptr [0x122ded8]
00aa2467 d80d1ccd2901         fmul dword ptr [0x129cd1c]
00aa246d d9542410             fst dword ptr [esp + 0x10]
00aa2471 d8259cf52201         fsub dword ptr [0x122f59c]
00aa2477 d95c2414             fstp dword ptr [esp + 0x14]
00aa247b d9442414             fld dword ptr [esp + 0x14]
00aa247f db9c2488000000       fistp dword ptr [esp + 0x88]
00aa2486 db842488000000       fild dword ptr [esp + 0x88]
00aa248d 8b842488000000       mov eax, dword ptr [esp + 0x88]
00aa2494 d86c2410             fsubr dword ptr [esp + 0x10]
00aa2498 d905d8de2201         fld dword ptr [0x122ded8]
00aa249e d8e1                 fsub st(1)
00aa24a0 d80c8530b53c01       fmul dword ptr [eax*4 + 0x13cb530]
00aa24a7 d9c9                 fxch st(1)
00aa24a9 d80c8534b53c01       fmul dword ptr [eax*4 + 0x13cb534]
00aa24b0 dec1                 faddp st(1)
00aa24b2 d80d087f2301         fmul dword ptr [0x1237f08]
00aa24b8 d95c2444             fstp dword ptr [esp + 0x44]
00aa24bc d9442474             fld dword ptr [esp + 0x74]
00aa24c0 d84c2474             fmul dword ptr [esp + 0x74]
00aa24c4 d82dd8de2201         fsubr dword ptr [0x122ded8]
00aa24ca d9fa                 fsqrt 
00aa24cc d95c2430             fstp dword ptr [esp + 0x30]
00aa24d0 d905d8de2201         fld dword ptr [0x122ded8]
00aa24d6 d8642418             fsub dword ptr [esp + 0x18]
00aa24da d84c2444             fmul dword ptr [esp + 0x44]
00aa24de d80d10002301         fmul dword ptr [0x1230010]
00aa24e4 d9542410             fst dword ptr [esp + 0x10]
00aa24e8 d8259cf52201         fsub dword ptr [0x122f59c]
00aa24ee d95c2414             fstp dword ptr [esp + 0x14]
00aa24f2 d9442414             fld dword ptr [esp + 0x14]
00aa24f6 db9c2480000000       fistp dword ptr [esp + 0x80]
00aa24fd db842480000000       fild dword ptr [esp + 0x80]
00aa2504 8b842480000000       mov eax, dword ptr [esp + 0x80]
00aa250b 0500ffffff           add eax, 0xffffff00
00aa2510 25ff030000           and eax, 0x3ff
00aa2515 d86c2410             fsubr dword ptr [esp + 0x10]
00aa2519 d905d8de2201         fld dword ptr [0x122ded8]
00aa251f d8742430             fdiv dword ptr [esp + 0x30]
00aa2523 d95c2410             fstp dword ptr [esp + 0x10]
00aa2527 d905d8de2201         fld dword ptr [0x122ded8]
00aa252d d8e1                 fsub st(1)
00aa252f d80c8550d53c01       fmul dword ptr [eax*4 + 0x13cd550]
00aa2536 d9c9                 fxch st(1)
00aa2538 d80c8554d53c01       fmul dword ptr [eax*4 + 0x13cd554]
00aa253f dec1                 faddp st(1)
00aa2541 d84c2410             fmul dword ptr [esp + 0x10]
00aa2545 d95c2430             fstp dword ptr [esp + 0x30]
00aa2549 d9442444             fld dword ptr [esp + 0x44]
00aa254d d84c2418             fmul dword ptr [esp + 0x18]
00aa2551 d80d10002301         fmul dword ptr [0x1230010]
00aa2557 d9542444             fst dword ptr [esp + 0x44]
00aa255b d8259cf52201         fsub dword ptr [0x122f59c]
00aa2561 d95c2414             fstp dword ptr [esp + 0x14]
00aa2565 d9442414             fld dword ptr [esp + 0x14]
00aa2569 db9c2484000000       fistp dword ptr [esp + 0x84]
00aa2570 db842484000000       fild dword ptr [esp + 0x84]
00aa2577 8b842484000000       mov eax, dword ptr [esp + 0x84]
00aa257e 0500ffffff           add eax, 0xffffff00
00aa2583 25ff030000           and eax, 0x3ff
00aa2588 d86c2444             fsubr dword ptr [esp + 0x44]
00aa258c d905d8de2201         fld dword ptr [0x122ded8]
00aa2592 d8e1                 fsub st(1)
00aa2594 d80c8550d53c01       fmul dword ptr [eax*4 + 0x13cd550]
00aa259b d9c9                 fxch st(1)
00aa259d d80c8554d53c01       fmul dword ptr [eax*4 + 0x13cd554]
00aa25a4 dec1                 faddp st(1)
00aa25a6 d84c2410             fmul dword ptr [esp + 0x10]
00aa25aa eb12                 jmp 0xaa25be
00aa25ac d905d8de2201         fld dword ptr [0x122ded8]
00aa25b2 d8642418             fsub dword ptr [esp + 0x18]
00aa25b6 d95c2430             fstp dword ptr [esp + 0x30]
00aa25ba d9442418             fld dword ptr [esp + 0x18]
00aa25be d88c248c000000       fmul dword ptr [esp + 0x8c]
00aa25c5 8d4c2450             lea ecx, [esp + 0x50]
00aa25c9 d9842450010000       fld dword ptr [esp + 0x150]
00aa25d0 51                   push ecx
00aa25d1 8d4ee8               lea ecx, [esi - 0x18]
00aa25d4 d8c9                 fmul st(1)
00aa25d6 d95c2454             fstp dword ptr [esp + 0x54]
00aa25da d9842458010000       fld dword ptr [esp + 0x158]
00aa25e1 d8c9                 fmul st(1)
00aa25e3 d95c2458             fstp dword ptr [esp + 0x58]
00aa25e7 d984245c010000       fld dword ptr [esp + 0x15c]
00aa25ee d8c9                 fmul st(1)
00aa25f0 d95c245c             fstp dword ptr [esp + 0x5c]
00aa25f4 d9842460010000       fld dword ptr [esp + 0x160]
00aa25fb d8c9                 fmul st(1)
00aa25fd d8442434             fadd dword ptr [esp + 0x34]
00aa2601 d95c2460             fstp dword ptr [esp + 0x60]
00aa2605 ddd8                 fstp st(0)
00aa2607 e80466feff           call 0xa88c10
00aa260c d9842460010000       fld dword ptr [esp + 0x160]
00aa2613 8b542428             mov edx, dword ptr [esp + 0x28]
00aa2617 d84c2418             fmul dword ptr [esp + 0x18]
00aa261b d9842464010000       fld dword ptr [esp + 0x164]
00aa2622 d84c2418             fmul dword ptr [esp + 0x18]
00aa2626 d9842468010000       fld dword ptr [esp + 0x168]
00aa262d d84c2418             fmul dword ptr [esp + 0x18]
00aa2631 d99c2488010000       fstp dword ptr [esp + 0x188]
00aa2638 d984246c010000       fld dword ptr [esp + 0x16c]
00aa263f d84c2418             fmul dword ptr [esp + 0x18]
00aa2643 d99c248c010000       fstp dword ptr [esp + 0x18c]
00aa264a d9c9                 fxch st(1)
00aa264c d846f8               fadd dword ptr [esi - 8]
00aa264f d95ef8               fstp dword ptr [esi - 8]
00aa2652 d846fc               fadd dword ptr [esi - 4]
00aa2655 d95efc               fstp dword ptr [esi - 4]
00aa2658 d9842488010000       fld dword ptr [esp + 0x188]
00aa265f d806                 fadd dword ptr [esi]
00aa2661 d91e                 fstp dword ptr [esi]
00aa2663 d984248c010000       fld dword ptr [esp + 0x18c]
00aa266a d84604               fadd dword ptr [esi + 4]
00aa266d d95e04               fstp dword ptr [esi + 4]
00aa2670 8b44241c             mov eax, dword ptr [esp + 0x1c]
00aa2674 47                   inc edi
00aa2675 83c630               add esi, 0x30
00aa2678 3bf8                 cmp edi, eax
00aa267a 0f8250fdffff         jb 0xaa23d0
00aa2680 e9d1030000           jmp 0xaa2a56
00aa2685 d1e8                 shr eax, 1
00aa2687 a801                 test al, 1
00aa2689 0f84c7030000         je 0xaa2a56
00aa268f d9442418             fld dword ptr [esp + 0x18]
00aa2693 d825d8de2201         fsub dword ptr [0x122ded8]
00aa2699 d9e1                 fabs 
00aa269b d81d3cba2901         fcomp dword ptr [0x129ba3c]
00aa26a1 dfe0                 fnstsw ax
00aa26a3 f6c405               test ah, 5
00aa26a6 8b44241c             mov eax, dword ptr [esp + 0x1c]
00aa26aa 7a58                 jp 0xaa2704
00aa26ac 33f6                 xor esi, esi
00aa26ae 85c0                 test eax, eax
00aa26b0 0f86a0030000         jbe 0xaa2a56
00aa26b6 8b7c2434             mov edi, dword ptr [esp + 0x34]
00aa26ba 8b5c2478             mov ebx, dword ptr [esp + 0x78]
00aa26be 8bff                 mov edi, edi
00aa26c0 8b44243c             mov eax, dword ptr [esp + 0x3c]
00aa26c4 83c008               add eax, 8
00aa26c7 8b00                 mov eax, dword ptr [eax]
00aa26c9 8b0cb0               mov ecx, dword ptr [eax + esi*4]
00aa26cc 8b4268               mov eax, dword ptr [edx + 0x68]
00aa26cf 85c0                 test eax, eax
00aa26d1 7406                 je 0xaa26d9
00aa26d3 803c0800             cmp byte ptr [eax + ecx], 0
00aa26d7 7416                 je 0xaa26ef
00aa26d9 8b442424             mov eax, dword ptr [esp + 0x24]
00aa26dd 8b525c               mov edx, dword ptr [edx + 0x5c]
00aa26e0 8b0cca               mov ecx, dword ptr [edx + ecx*8]
00aa26e3 53                   push ebx
00aa26e4 50                   push eax
00aa26e5 57                   push edi
00aa26e6 e8059bfaff           call 0xa4c1f0
00aa26eb 8b542428             mov edx, dword ptr [esp + 0x28]
00aa26ef 8b44241c             mov eax, dword ptr [esp + 0x1c]
00aa26f3 46                   inc esi
00aa26f4 83c730               add edi, 0x30
00aa26f7 3bf0                 cmp esi, eax
00aa26f9 72c5                 jb 0xaa26c0
00aa26fb 8b5c243c             mov ebx, dword ptr [esp + 0x3c]
00aa26ff e952030000           jmp 0xaa2a56
00aa2704 33ff                 xor edi, edi
00aa2706 85c0                 test eax, eax
00aa2708 0f8648030000         jbe 0xaa2a56
00aa270e 8b742434             mov esi, dword ptr [esp + 0x34]
00aa2712 83c618               add esi, 0x18
00aa2715 8b4a68               mov ecx, dword ptr [edx + 0x68]
00aa2718 85c9                 test ecx, ecx
00aa271a 8b4308               mov eax, dword ptr [ebx + 8]
00aa271d 8b04b8               mov eax, dword ptr [eax + edi*4]
00aa2720 740a                 je 0xaa272c
00aa2722 803c0100             cmp byte ptr [ecx + eax], 0
00aa2726 0f841a030000         je 0xaa2a46
00aa272c 8b4c2478             mov ecx, dword ptr [esp + 0x78]
00aa2730 8b525c               mov edx, dword ptr [edx + 0x5c]
00aa2733 51                   push ecx
00aa2734 8b4c2428             mov ecx, dword ptr [esp + 0x28]
00aa2738 51                   push ecx
00aa2739 8d8c2428010000       lea ecx, [esp + 0x128]
00aa2740 51                   push ecx
00aa2741 8b0cc2               mov ecx, dword ptr [edx + eax*8]
00aa2744 e8a79afaff           call 0xa4c1f0
00aa2749 d9842424010000       fld dword ptr [esp + 0x124]
00aa2750 d84eec               fmul dword ptr [esi - 0x14]
00aa2753 c74424400000803f     mov dword ptr [esp + 0x40], 0x3f800000
00aa275b d9842428010000       fld dword ptr [esp + 0x128]
00aa2762 d84ef0               fmul dword ptr [esi - 0x10]
00aa2765 dec1                 faddp st(1)
00aa2767 d984242c010000       fld dword ptr [esp + 0x12c]
00aa276e d84ef4               fmul dword ptr [esi - 0xc]
00aa2771 dec1                 faddp st(1)
00aa2773 d9842420010000       fld dword ptr [esp + 0x120]
00aa277a d84ee8               fmul dword ptr [esi - 0x18]
00aa277d dec1                 faddp st(1)
00aa277f d95c2410             fstp dword ptr [esp + 0x10]
00aa2783 d9442410             fld dword ptr [esp + 0x10]
00aa2787 d81ddcde2201         fcomp dword ptr [0x122dedc]
00aa278d dfe0                 fnstsw ax
00aa278f f6c405               test ah, 5
00aa2792 7a12                 jp 0xaa27a6
00aa2794 d9442410             fld dword ptr [esp + 0x10]
00aa2798 c7442440000080bf     mov dword ptr [esp + 0x40], 0xbf800000
00aa27a0 d9e0                 fchs 
00aa27a2 d95c2410             fstp dword ptr [esp + 0x10]
00aa27a6 d905d8de2201         fld dword ptr [0x122ded8]
00aa27ac d8642410             fsub dword ptr [esp + 0x10]
00aa27b0 d81d5ca12501         fcomp dword ptr [0x125a15c]
00aa27b6 dfe0                 fnstsw ax
00aa27b8 f6c441               test ah, 0x41
00aa27bb 0f8534010000         jne 0xaa28f5
00aa27c1 d9442410             fld dword ptr [esp + 0x10]
00aa27c5 d805d8de2201         fadd dword ptr [0x122ded8]
00aa27cb d80d1ccd2901         fmul dword ptr [0x129cd1c]
00aa27d1 d9542430             fst dword ptr [esp + 0x30]
00aa27d5 d8259cf52201         fsub dword ptr [0x122f59c]
00aa27db d95c2414             fstp dword ptr [esp + 0x14]
00aa27df d9442414             fld dword ptr [esp + 0x14]
00aa27e3 db5c2470             fistp dword ptr [esp + 0x70]
00aa27e7 db442470             fild dword ptr [esp + 0x70]
00aa27eb 8b442470             mov eax, dword ptr [esp + 0x70]
00aa27ef d86c2430             fsubr dword ptr [esp + 0x30]
00aa27f3 d905d8de2201         fld dword ptr [0x122ded8]
00aa27f9 d8e1                 fsub st(1)
00aa27fb d80c8530b53c01       fmul dword ptr [eax*4 + 0x13cb530]
00aa2802 d9c9                 fxch st(1)
00aa2804 d80c8534b53c01       fmul dword ptr [eax*4 + 0x13cb534]
00aa280b dec1                 faddp st(1)
00aa280d d80d087f2301         fmul dword ptr [0x1237f08]
00aa2813 d95c2444             fstp dword ptr [esp + 0x44]
00aa2817 d9442410             fld dword ptr [esp + 0x10]
00aa281b d84c2410             fmul dword ptr [esp + 0x10]
00aa281f d82dd8de2201         fsubr dword ptr [0x122ded8]
00aa2825 d9fa                 fsqrt 
00aa2827 d95c2410             fstp dword ptr [esp + 0x10]
00aa282b d905d8de2201         fld dword ptr [0x122ded8]
00aa2831 d8642418             fsub dword ptr [esp + 0x18]
00aa2835 d84c2444             fmul dword ptr [esp + 0x44]
00aa2839 d80d10002301         fmul dword ptr [0x1230010]
00aa283f d9542430             fst dword ptr [esp + 0x30]
00aa2843 d8259cf52201         fsub dword ptr [0x122f59c]
00aa2849 d95c2414             fstp dword ptr [esp + 0x14]
00aa284d d9442414             fld dword ptr [esp + 0x14]
00aa2851 db5c2448             fistp dword ptr [esp + 0x48]
00aa2855 db442448             fild dword ptr [esp + 0x48]
00aa2859 8b442448             mov eax, dword ptr [esp + 0x48]
00aa285d 0500ffffff           add eax, 0xffffff00
00aa2862 25ff030000           and eax, 0x3ff
00aa2867 d86c2430             fsubr dword ptr [esp + 0x30]
00aa286b d905d8de2201         fld dword ptr [0x122ded8]
00aa2871 d8742410             fdiv dword ptr [esp + 0x10]
00aa2875 d95c2414             fstp dword ptr [esp + 0x14]
00aa2879 d905d8de2201         fld dword ptr [0x122ded8]
00aa287f d8e1                 fsub st(1)
00aa2881 d80c8550d53c01       fmul dword ptr [eax*4 + 0x13cd550]
00aa2888 d9c9                 fxch st(1)
00aa288a d80c8554d53c01       fmul dword ptr [eax*4 + 0x13cd554]
00aa2891 dec1                 faddp st(1)
00aa2893 d84c2414             fmul dword ptr [esp + 0x14]
00aa2897 d95c2410             fstp dword ptr [esp + 0x10]
00aa289b d9442444             fld dword ptr [esp + 0x44]
00aa289f d84c2418             fmul dword ptr [esp + 0x18]
00aa28a3 d80d10002301         fmul dword ptr [0x1230010]
00aa28a9 d9542430             fst dword ptr [esp + 0x30]
00aa28ad d8259cf52201         fsub dword ptr [0x122f59c]
00aa28b3 d95c2444             fstp dword ptr [esp + 0x44]
00aa28b7 d9442444             fld dword ptr [esp + 0x44]
00aa28bb db5c244c             fistp dword ptr [esp + 0x4c]
00aa28bf db44244c             fild dword ptr [esp + 0x4c]
00aa28c3 8b44244c             mov eax, dword ptr [esp + 0x4c]
00aa28c7 0500ffffff           add eax, 0xffffff00
00aa28cc 25ff030000           and eax, 0x3ff
00aa28d1 d86c2430             fsubr dword ptr [esp + 0x30]
00aa28d5 d905d8de2201         fld dword ptr [0x122ded8]
00aa28db d8e1                 fsub st(1)
00aa28dd d80c8550d53c01       fmul dword ptr [eax*4 + 0x13cd550]
00aa28e4 d9c9                 fxch st(1)
00aa28e6 d80c8554d53c01       fmul dword ptr [eax*4 + 0x13cd554]
00aa28ed dec1                 faddp st(1)
00aa28ef d84c2414             fmul dword ptr [esp + 0x14]
00aa28f3 eb12                 jmp 0xaa2907
00aa28f5 d905d8de2201         fld dword ptr [0x122ded8]
00aa28fb d8642418             fsub dword ptr [esp + 0x18]
00aa28ff d95c2410             fstp dword ptr [esp + 0x10]
00aa2903 d9442418             fld dword ptr [esp + 0x18]
00aa2907 d84c2440             fmul dword ptr [esp + 0x40]
00aa290b d9442410             fld dword ptr [esp + 0x10]
00aa290f d84ee8               fmul dword ptr [esi - 0x18]
00aa2912 d9842420010000       fld dword ptr [esp + 0x120]
00aa2919 d8ca                 fmul st(2)
00aa291b dec1                 faddp st(1)
00aa291d d95c2460             fstp dword ptr [esp + 0x60]
00aa2921 8b442460             mov eax, dword ptr [esp + 0x60]
00aa2925 d9842424010000       fld dword ptr [esp + 0x124]
00aa292c d8c9                 fmul st(1)
00aa292e d9442410             fld dword ptr [esp + 0x10]
00aa2932 d84eec               fmul dword ptr [esi - 0x14]
00aa2935 dec1                 faddp st(1)
00aa2937 d95c2464             fstp dword ptr [esp + 0x64]
00aa293b 8b4c2464             mov ecx, dword ptr [esp + 0x64]
00aa293f d9842428010000       fld dword ptr [esp + 0x128]
00aa2946 d8c9                 fmul st(1)
00aa2948 d9442410             fld dword ptr [esp + 0x10]
00aa294c d84ef0               fmul dword ptr [esi - 0x10]
00aa294f dec1                 faddp st(1)
00aa2951 d95c2468             fstp dword ptr [esp + 0x68]
00aa2955 8b542468             mov edx, dword ptr [esp + 0x68]
00aa2959 d984242c010000       fld dword ptr [esp + 0x12c]
00aa2960 d8c9                 fmul st(1)
00aa2962 d9442410             fld dword ptr [esp + 0x10]
00aa2966 d84ef4               fmul dword ptr [esi - 0xc]
00aa2969 8946e8               mov dword ptr [esi - 0x18], eax
00aa296c 894eec               mov dword ptr [esi - 0x14], ecx
00aa296f 8956f0               mov dword ptr [esi - 0x10], edx
00aa2972 dec1                 faddp st(1)
00aa2974 d95c246c             fstp dword ptr [esp + 0x6c]
00aa2978 8b44246c             mov eax, dword ptr [esp + 0x6c]
00aa297c 8946f4               mov dword ptr [esi - 0xc], eax
00aa297f ddd8                 fstp st(0)
00aa2981 d9842430010000       fld dword ptr [esp + 0x130]
00aa2988 d866f8               fsub dword ptr [esi - 8]
00aa298b d9842434010000       fld dword ptr [esp + 0x134]
00aa2992 d866fc               fsub dword ptr [esi - 4]
00aa2995 d9842438010000       fld dword ptr [esp + 0x138]
00aa299c d826                 fsub dword ptr [esi]
00aa299e d99c2498010000       fstp dword ptr [esp + 0x198]
00aa29a5 d984243c010000       fld dword ptr [esp + 0x13c]
00aa29ac d86604               fsub dword ptr [esi + 4]
00aa29af d99c249c010000       fstp dword ptr [esp + 0x19c]
00aa29b6 d9c9                 fxch st(1)
00aa29b8 d84c2418             fmul dword ptr [esp + 0x18]
00aa29bc d99c24a0010000       fstp dword ptr [esp + 0x1a0]
00aa29c3 d84c2418             fmul dword ptr [esp + 0x18]
00aa29c7 d9842498010000       fld dword ptr [esp + 0x198]
00aa29ce d84c2418             fmul dword ptr [esp + 0x18]
00aa29d2 d984249c010000       fld dword ptr [esp + 0x19c]
00aa29d9 d84c2418             fmul dword ptr [esp + 0x18]
00aa29dd d99c24ac010000       fstp dword ptr [esp + 0x1ac]
00aa29e4 d98424a0010000       fld dword ptr [esp + 0x1a0]
00aa29eb d846f8               fadd dword ptr [esi - 8]
00aa29ee d99c24d0000000       fstp dword ptr [esp + 0xd0]
00aa29f5 8b8c24d0000000       mov ecx, dword ptr [esp + 0xd0]
00aa29fc d9c9                 fxch st(1)
00aa29fe d846fc               fadd dword ptr [esi - 4]
00aa2a01 d99c24d4000000       fstp dword ptr [esp + 0xd4]
00aa2a08 8b9424d4000000       mov edx, dword ptr [esp + 0xd4]
00aa2a0f d806                 fadd dword ptr [esi]
00aa2a11 d99c24d8000000       fstp dword ptr [esp + 0xd8]
00aa2a18 8b8424d8000000       mov eax, dword ptr [esp + 0xd8]
00aa2a1f d98424ac010000       fld dword ptr [esp + 0x1ac]
00aa2a26 d84604               fadd dword ptr [esi + 4]
00aa2a29 894ef8               mov dword ptr [esi - 8], ecx
00aa2a2c 8956fc               mov dword ptr [esi - 4], edx
00aa2a2f 8b542428             mov edx, dword ptr [esp + 0x28]
00aa2a33 d99c24dc000000       fstp dword ptr [esp + 0xdc]
00aa2a3a 8b8c24dc000000       mov ecx, dword ptr [esp + 0xdc]
00aa2a41 8906                 mov dword ptr [esi], eax
00aa2a43 894e04               mov dword ptr [esi + 4], ecx
00aa2a46 8b44241c             mov eax, dword ptr [esp + 0x1c]
00aa2a4a 47                   inc edi
00aa2a4b 83c630               add esi, 0x30
00aa2a4e 3bf8                 cmp edi, eax
00aa2a50 0f82bffcffff         jb 0xaa2715
00aa2a56 8b44242c             mov eax, dword ptr [esp + 0x2c]
00aa2a5a 85c0                 test eax, eax
00aa2a5c 741e                 je 0xaa2a7c
00aa2a5e ff08                 dec dword ptr [eax]
00aa2a60 8b44242c             mov eax, dword ptr [esp + 0x2c]
00aa2a64 833800               cmp dword ptr [eax], 0
00aa2a67 7513                 jne 0xaa2a7c
00aa2a69 8b4808               mov ecx, dword ptr [eax + 8]
00aa2a6c ff5004               call dword ptr [eax + 4]
00aa2a6f 8b54242c             mov edx, dword ptr [esp + 0x2c]
00aa2a73 52                   push edx
00aa2a74 e843bf1500           call 0xbfe9bc
00aa2a79 83c404               add esp, 4
00aa2a7c 8b4c2438             mov ecx, dword ptr [esp + 0x38]
00aa2a80 33c0                 xor eax, eax
00aa2a82 89442428             mov dword ptr [esp + 0x28], eax
00aa2a86 8944242c             mov dword ptr [esp + 0x2c], eax
00aa2a8a 8b442420             mov eax, dword ptr [esp + 0x20]
00aa2a8e 83c114               add ecx, 0x14
00aa2a91 48                   dec eax
00aa2a92 894c2438             mov dword ptr [esp + 0x38], ecx
00aa2a96 89442420             mov dword ptr [esp + 0x20], eax
00aa2a9a 0f85d0f7ffff         jne 0xaa2270
00aa2aa0 8b4508               mov eax, dword ptr [ebp + 8]
00aa2aa3 8b4004               mov eax, dword ptr [eax + 4]
00aa2aa6 85c0                 test eax, eax
00aa2aa8 0f8480020000         je 0xaa2d2e
00aa2aae 8b4818               mov ecx, dword ptr [eax + 0x18]
00aa2ab1 85c9                 test ecx, ecx
00aa2ab3 0f8475020000         je 0xaa2d2e
00aa2ab9 8b44241c             mov eax, dword ptr [esp + 0x1c]
00aa2abd 85c0                 test eax, eax
00aa2abf 742b                 je 0xaa2aec
00aa2ac1 b90c000000           mov ecx, 0xc
00aa2ac6 f7e1                 mul ecx
00aa2ac8 85d2                 test edx, edx
00aa2aca 7720                 ja 0xaa2aec
00aa2acc 83f8ff               cmp eax, -1
00aa2acf 771b                 ja 0xaa2aec
00aa2ad1 8b44241c             mov eax, dword ptr [esp + 0x1c]
00aa2ad5 8b4c243c             mov ecx, dword ptr [esp + 0x3c]
00aa2ad9 8d1440               lea edx, [eax + eax*2]
00aa2adc c1e202               shl edx, 2
00aa2adf 52                   push edx
00aa2ae0 83c114               add ecx, 0x14
00aa2ae3 e8289efbff           call 0xa5c910
00aa2ae8 8bd8                 mov ebx, eax
00aa2aea eb02                 jmp 0xaa2aee
00aa2aec 33db                 xor ebx, ebx
00aa2aee 8b4d08               mov ecx, dword ptr [ebp + 8]
00aa2af1 8b5104               mov edx, dword ptr [ecx + 4]
00aa2af4 8b4a18               mov ecx, dword ptr [edx + 0x18]
00aa2af7 8d442428             lea eax, [esp + 0x28]
00aa2afb 50                   push eax
00aa2afc e85f27faff           call 0xa45260
00aa2b01 8b44241c             mov eax, dword ptr [esp + 0x1c]
00aa2b05 33f6                 xor esi, esi
00aa2b07 85c0                 test eax, eax
00aa2b09 7637                 jbe 0xaa2b42
00aa2b0b 8bfb                 mov edi, ebx
00aa2b0d 8d4900               lea ecx, [ecx]
00aa2b10 8b44243c             mov eax, dword ptr [esp + 0x3c]
00aa2b14 8b5514               mov edx, dword ptr [ebp + 0x14]
00aa2b17 83c008               add eax, 8
00aa2b1a 8b00                 mov eax, dword ptr [eax]
00aa2b1c 8b0cb0               mov ecx, dword ptr [eax + esi*4]
00aa2b1f 8b829c000000         mov eax, dword ptr [edx + 0x9c]
00aa2b25 6bc93c               imul ecx, ecx, 0x3c
00aa2b28 8b0c01               mov ecx, dword ptr [ecx + eax]
00aa2b2b 51                   push ecx
00aa2b2c 8b4c242c             mov ecx, dword ptr [esp + 0x2c]
00aa2b30 57                   push edi
00aa2b31 e89a21faff           call 0xa44cd0
00aa2b36 8b44241c             mov eax, dword ptr [esp + 0x1c]
00aa2b3a 46                   inc esi
00aa2b3b 83c70c               add edi, 0xc
00aa2b3e 3bf0                 cmp esi, eax
00aa2b40 72ce                 jb 0xaa2b10
00aa2b42 8b54241c             mov edx, dword ptr [esp + 0x1c]
00aa2b46 8d42ff               lea eax, [edx - 1]
00aa2b49 83f804               cmp eax, 4
00aa2b4c be01000000           mov esi, 1
00aa2b51 0f8c39010000         jl 0xaa2c90
00aa2b57 8b442434             mov eax, dword ptr [esp + 0x34]
00aa2b5b 83c2fb               add edx, -5
00aa2b5e c1ea02               shr edx, 2
00aa2b61 83c018               add eax, 0x18
00aa2b64 42                   inc edx
00aa2b65 8d4b20               lea ecx, [ebx + 0x20]
00aa2b68 8d349501000000       lea esi, [edx*4 + 1]
00aa2b6f 90                   nop 
00aa2b70 8b78fc               mov edi, dword ptr [eax - 4]
00aa2b73 d940f8               fld dword ptr [eax - 8]
00aa2b76 d849ec               fmul dword ptr [ecx - 0x14]
00aa2b79 897c2464             mov dword ptr [esp + 0x64], edi
00aa2b7d 8b38                 mov edi, dword ptr [eax]
00aa2b7f d9442464             fld dword ptr [esp + 0x64]
00aa2b83 d849f0               fmul dword ptr [ecx - 0x10]
00aa2b86 897c2468             mov dword ptr [esp + 0x68], edi
00aa2b8a d9442468             fld dword ptr [esp + 0x68]
00aa2b8e 83c130               add ecx, 0x30
00aa2b91 d849c4               fmul dword ptr [ecx - 0x3c]
00aa2b94 c7400400000000       mov dword ptr [eax + 4], 0
00aa2b9b 05c0000000           add eax, 0xc0
00aa2ba0 4a                   dec edx
00aa2ba1 d95c2458             fstp dword ptr [esp + 0x58]
00aa2ba5 8b7c2458             mov edi, dword ptr [esp + 0x58]
00aa2ba9 89b840ffffff         mov dword ptr [eax - 0xc0], edi
00aa2baf d9c9                 fxch st(1)
00aa2bb1 d99838ffffff         fstp dword ptr [eax - 0xc8]
00aa2bb7 d9983cffffff         fstp dword ptr [eax - 0xc4]
00aa2bbd 8bb86cffffff         mov edi, dword ptr [eax - 0x94]
00aa2bc3 d98068ffffff         fld dword ptr [eax - 0x98]
00aa2bc9 897c2464             mov dword ptr [esp + 0x64], edi
00aa2bcd d849c8               fmul dword ptr [ecx - 0x38]
00aa2bd0 8bb870ffffff         mov edi, dword ptr [eax - 0x90]
00aa2bd6 d9442464             fld dword ptr [esp + 0x64]
00aa2bda 897c2468             mov dword ptr [esp + 0x68], edi
00aa2bde d849cc               fmul dword ptr [ecx - 0x34]
00aa2be1 d9442468             fld dword ptr [esp + 0x68]
00aa2be5 d849d0               fmul dword ptr [ecx - 0x30]
00aa2be8 c78074ffffff00000000 mov dword ptr [eax - 0x8c], 0
00aa2bf2 d95c2458             fstp dword ptr [esp + 0x58]
00aa2bf6 8b7c2458             mov edi, dword ptr [esp + 0x58]
00aa2bfa 89b870ffffff         mov dword ptr [eax - 0x90], edi
00aa2c00 d9c9                 fxch st(1)
00aa2c02 d99868ffffff         fstp dword ptr [eax - 0x98]
00aa2c08 d9986cffffff         fstp dword ptr [eax - 0x94]
00aa2c0e 8b789c               mov edi, dword ptr [eax - 0x64]
00aa2c11 d94098               fld dword ptr [eax - 0x68]
00aa2c14 897c2464             mov dword ptr [esp + 0x64], edi
00aa2c18 d849d4               fmul dword ptr [ecx - 0x2c]
00aa2c1b 8b78a0               mov edi, dword ptr [eax - 0x60]
00aa2c1e d9442464             fld dword ptr [esp + 0x64]
00aa2c22 897c2468             mov dword ptr [esp + 0x68], edi
00aa2c26 d849d8               fmul dword ptr [ecx - 0x28]
00aa2c29 d9442468             fld dword ptr [esp + 0x68]
00aa2c2d d849dc               fmul dword ptr [ecx - 0x24]
00aa2c30 c740a400000000       mov dword ptr [eax - 0x5c], 0
00aa2c37 d95c2458             fstp dword ptr [esp + 0x58]
00aa2c3b 8b7c2458             mov edi, dword ptr [esp + 0x58]
00aa2c3f 8978a0               mov dword ptr [eax - 0x60], edi
00aa2c42 d9c9                 fxch st(1)
00aa2c44 d95898               fstp dword ptr [eax - 0x68]
00aa2c47 d9589c               fstp dword ptr [eax - 0x64]
00aa2c4a 8b78cc               mov edi, dword ptr [eax - 0x34]
00aa2c4d d940c8               fld dword ptr [eax - 0x38]
00aa2c50 897c2464             mov dword ptr [esp + 0x64], edi
00aa2c54 d849e0               fmul dword ptr [ecx - 0x20]
00aa2c57 8b78d0               mov edi, dword ptr [eax - 0x30]
00aa2c5a d9442464             fld dword ptr [esp + 0x64]
00aa2c5e 897c2468             mov dword ptr [esp + 0x68], edi
00aa2c62 d849e4               fmul dword ptr [ecx - 0x1c]
00aa2c65 d9442468             fld dword ptr [esp + 0x68]
00aa2c69 d849e8               fmul dword ptr [ecx - 0x18]
00aa2c6c c740d400000000       mov dword ptr [eax - 0x2c], 0
00aa2c73 d95c2458             fstp dword ptr [esp + 0x58]
00aa2c77 8b7c2458             mov edi, dword ptr [esp + 0x58]
00aa2c7b d9c9                 fxch st(1)
00aa2c7d 8978d0               mov dword ptr [eax - 0x30], edi
00aa2c80 d958c8               fstp dword ptr [eax - 0x38]
00aa2c83 d958cc               fstp dword ptr [eax - 0x34]
00aa2c86 0f85e4feffff         jne 0xaa2b70
00aa2c8c 8b54241c             mov edx, dword ptr [esp + 0x1c]
00aa2c90 3bf2                 cmp esi, edx
00aa2c92 8b7c2434             mov edi, dword ptr [esp + 0x34]
00aa2c96 735b                 jae 0xaa2cf3
00aa2c98 8d1476               lea edx, [esi + esi*2]
00aa2c9b c1e204               shl edx, 4
00aa2c9e 8d443ae8             lea eax, [edx + edi - 0x18]
00aa2ca2 8b54241c             mov edx, dword ptr [esp + 0x1c]
00aa2ca6 8d0c76               lea ecx, [esi + esi*2]
00aa2ca9 8d4c8b08             lea ecx, [ebx + ecx*4 + 8]
00aa2cad 2bd6                 sub edx, esi
00aa2caf 90                   nop 
00aa2cb0 8b70fc               mov esi, dword ptr [eax - 4]
00aa2cb3 d940f8               fld dword ptr [eax - 8]
00aa2cb6 d849f8               fmul dword ptr [ecx - 8]
00aa2cb9 89742464             mov dword ptr [esp + 0x64], esi
00aa2cbd 8b30                 mov esi, dword ptr [eax]
00aa2cbf d9442464             fld dword ptr [esp + 0x64]
00aa2cc3 d849fc               fmul dword ptr [ecx - 4]
00aa2cc6 89742468             mov dword ptr [esp + 0x68], esi
00aa2cca d9442468             fld dword ptr [esp + 0x68]
00aa2cce 83c030               add eax, 0x30
00aa2cd1 d809                 fmul dword ptr [ecx]
00aa2cd3 c740d400000000       mov dword ptr [eax - 0x2c], 0
00aa2cda 83c10c               add ecx, 0xc
00aa2cdd 4a                   dec edx
00aa2cde d95c2458             fstp dword ptr [esp + 0x58]
00aa2ce2 8b742458             mov esi, dword ptr [esp + 0x58]
00aa2ce6 d9c9                 fxch st(1)
00aa2ce8 8970d0               mov dword ptr [eax - 0x30], esi
00aa2ceb d958c8               fstp dword ptr [eax - 0x38]
00aa2cee d958cc               fstp dword ptr [eax - 0x34]
00aa2cf1 75bd                 jne 0xaa2cb0
00aa2cf3 8b0b                 mov ecx, dword ptr [ebx]
00aa2cf5 8b4520               mov eax, dword ptr [ebp + 0x20]
00aa2cf8 8908                 mov dword ptr [eax], ecx
00aa2cfa 8b5304               mov edx, dword ptr [ebx + 4]
00aa2cfd 895004               mov dword ptr [eax + 4], edx
00aa2d00 8b4b08               mov ecx, dword ptr [ebx + 8]
00aa2d03 894808               mov dword ptr [eax + 8], ecx
00aa2d06 8b44242c             mov eax, dword ptr [esp + 0x2c]
00aa2d0a 85c0                 test eax, eax
00aa2d0c 7434                 je 0xaa2d42
00aa2d0e ff08                 dec dword ptr [eax]
00aa2d10 8b44242c             mov eax, dword ptr [esp + 0x2c]
00aa2d14 833800               cmp dword ptr [eax], 0
00aa2d17 7529                 jne 0xaa2d42
00aa2d19 8b4808               mov ecx, dword ptr [eax + 8]
00aa2d1c ff5004               call dword ptr [eax + 4]
00aa2d1f 8b54242c             mov edx, dword ptr [esp + 0x2c]
00aa2d23 52                   push edx
00aa2d24 e893bc1500           call 0xbfe9bc
00aa2d29 83c404               add esp, 4
00aa2d2c eb14                 jmp 0xaa2d42
00aa2d2e 8b4520               mov eax, dword ptr [ebp + 0x20]
00aa2d31 8b7c2434             mov edi, dword ptr [esp + 0x34]
00aa2d35 b90000803f           mov ecx, 0x3f800000
00aa2d3a 8908                 mov dword ptr [eax], ecx
00aa2d3c 894804               mov dword ptr [eax + 4], ecx
00aa2d3f 894808               mov dword ptr [eax + 8], ecx
00aa2d42 d94710               fld dword ptr [edi + 0x10]
00aa2d45 8b4718               mov eax, dword ptr [edi + 0x18]
00aa2d48 d94714               fld dword ptr [edi + 0x14]
00aa2d4b 89442458             mov dword ptr [esp + 0x58], eax
00aa2d4f d907                 fld dword ptr [edi]
00aa2d51 8db424e0000000       lea esi, [esp + 0xe0]
00aa2d58 d94704               fld dword ptr [edi + 4]
00aa2d5b 8d9c24b0010000       lea ebx, [esp + 0x1b0]
00aa2d62 d94708               fld dword ptr [edi + 8]
00aa2d65 d9470c               fld dword ptr [edi + 0xc]
00aa2d68 d9c0                 fld st(0)
00aa2d6a d8c9                 fmul st(1)
00aa2d6c d9c2                 fld st(2)
00aa2d6e d8cb                 fmul st(3)
00aa2d70 dec1                 faddp st(1)
00aa2d72 d9c3                 fld st(3)
00aa2d74 d8cc                 fmul st(4)
00aa2d76 dec1                 faddp st(1)
00aa2d78 d9c4                 fld st(4)
00aa2d7a d8cd                 fmul st(5)
00aa2d7c dec1                 faddp st(1)
00aa2d7e d95c2420             fstp dword ptr [esp + 0x20]
00aa2d82 ddd8                 fstp st(0)
00aa2d84 ddd8                 fstp st(0)
00aa2d86 ddd8                 fstp st(0)
00aa2d88 ddd8                 fstp st(0)
00aa2d8a d905dcde2201         fld dword ptr [0x122dedc]
00aa2d90 d9442420             fld dword ptr [esp + 0x20]
00aa2d94 dae9                 fucompp 
00aa2d96 dfe0                 fnstsw ax
00aa2d98 f6c444               test ah, 0x44
00aa2d9b 7b0c                 jnp 0xaa2da9
00aa2d9d d905dcdc2201         fld dword ptr [0x122dcdc]
00aa2da3 d8742420             fdiv dword ptr [esp + 0x20]
00aa2da7 eb06                 jmp 0xaa2daf
00aa2da9 d905d8de2201         fld dword ptr [0x122ded8]
00aa2daf d9c0                 fld st(0)
00aa2db1 8b44241c             mov eax, dword ptr [esp + 0x1c]
00aa2db5 83f801               cmp eax, 1
00aa2db8 d80f                 fmul dword ptr [edi]
00aa2dba d9c1                 fld st(1)
00aa2dbc 8b4c2458             mov ecx, dword ptr [esp + 0x58]
00aa2dc0 d84f04               fmul dword ptr [edi + 4]
00aa2dc3 d95c2438             fstp dword ptr [esp + 0x38]
00aa2dc7 d9c9                 fxch st(1)
00aa2dc9 d84f08               fmul dword ptr [edi + 8]
00aa2dcc d95c2424             fstp dword ptr [esp + 0x24]
00aa2dd0 d9c0                 fld st(0)
00aa2dd2 d84f0c               fmul dword ptr [edi + 0xc]
00aa2dd5 d95c244c             fstp dword ptr [esp + 0x4c]
00aa2dd9 d9442438             fld dword ptr [esp + 0x38]
00aa2ddd d84f0c               fmul dword ptr [edi + 0xc]
00aa2de0 d95c2440             fstp dword ptr [esp + 0x40]
00aa2de4 d9442424             fld dword ptr [esp + 0x24]
00aa2de8 d84f0c               fmul dword ptr [edi + 0xc]
00aa2deb d95c2420             fstp dword ptr [esp + 0x20]
00aa2def d80f                 fmul dword ptr [edi]
00aa2df1 d9442438             fld dword ptr [esp + 0x38]
00aa2df5 d80f                 fmul dword ptr [edi]
00aa2df7 d9442424             fld dword ptr [esp + 0x24]
00aa2dfb d80f                 fmul dword ptr [edi]
00aa2dfd d9442438             fld dword ptr [esp + 0x38]
00aa2e01 d84f04               fmul dword ptr [edi + 4]
00aa2e04 d95c2448             fstp dword ptr [esp + 0x48]
00aa2e08 d9442424             fld dword ptr [esp + 0x24]
00aa2e0c d84f04               fmul dword ptr [edi + 4]
00aa2e0f d95c2414             fstp dword ptr [esp + 0x14]
00aa2e13 d9442424             fld dword ptr [esp + 0x24]
00aa2e17 d84f08               fmul dword ptr [edi + 8]
00aa2e1a c78424ec00000000000000 mov dword ptr [esp + 0xec], 0
00aa2e25 d9442448             fld dword ptr [esp + 0x48]
00aa2e29 c78424fc00000000000000 mov dword ptr [esp + 0xfc], 0
00aa2e34 c784240c01000000000000 mov dword ptr [esp + 0x10c], 0
00aa2e3f d8c1                 fadd st(1)
00aa2e41 898c2418010000       mov dword ptr [esp + 0x118], ecx
00aa2e48 c784241c0100000000803f mov dword ptr [esp + 0x11c], 0x3f800000
00aa2e53 d82dd8de2201         fsubr dword ptr [0x122ded8]
00aa2e59 d99c24e0000000       fstp dword ptr [esp + 0xe0]
00aa2e60 d9c2                 fld st(2)
00aa2e62 d8642420             fsub dword ptr [esp + 0x20]
00aa2e66 d99c24e4000000       fstp dword ptr [esp + 0xe4]
00aa2e6d d9c1                 fld st(1)
00aa2e6f d8442440             fadd dword ptr [esp + 0x40]
00aa2e73 d99c24e8000000       fstp dword ptr [esp + 0xe8]
00aa2e7a d9ca                 fxch st(2)
00aa2e7c d8442420             fadd dword ptr [esp + 0x20]
00aa2e80 d99c24f0000000       fstp dword ptr [esp + 0xf0]
00aa2e87 d9c9                 fxch st(1)
00aa2e89 d8c2                 fadd st(2)
00aa2e8b d82dd8de2201         fsubr dword ptr [0x122ded8]
00aa2e91 d99c24f4000000       fstp dword ptr [esp + 0xf4]
00aa2e98 d9442414             fld dword ptr [esp + 0x14]
00aa2e9c d864244c             fsub dword ptr [esp + 0x4c]
00aa2ea0 d99c24f8000000       fstp dword ptr [esp + 0xf8]
00aa2ea7 d8642440             fsub dword ptr [esp + 0x40]
00aa2eab d99c2400010000       fstp dword ptr [esp + 0x100]
00aa2eb2 d9442414             fld dword ptr [esp + 0x14]
00aa2eb6 d844244c             fadd dword ptr [esp + 0x4c]
00aa2eba d99c2404010000       fstp dword ptr [esp + 0x104]
00aa2ec1 d9442448             fld dword ptr [esp + 0x48]
00aa2ec5 d8c1                 fadd st(1)
00aa2ec7 d82dd8de2201         fsubr dword ptr [0x122ded8]
00aa2ecd d99c2408010000       fstp dword ptr [esp + 0x108]
00aa2ed4 ddd8                 fstp st(0)
00aa2ed6 d9c9                 fxch st(1)
00aa2ed8 d99c2410010000       fstp dword ptr [esp + 0x110]
00aa2edf d99c2414010000       fstp dword ptr [esp + 0x114]
00aa2ee6 0f8692030000         jbe 0xaa327e
00aa2eec 8b4c2434             mov ecx, dword ptr [esp + 0x34]
00aa2ef0 8bd0                 mov edx, eax
00aa2ef2 83c13c               add ecx, 0x3c
00aa2ef5 4a                   dec edx
00aa2ef6 89542470             mov dword ptr [esp + 0x70], edx
00aa2efa 8d9b00000000         lea ebx, [ebx]
00aa2f00 d941f4               fld dword ptr [ecx - 0xc]
00aa2f03 8b4104               mov eax, dword ptr [ecx + 4]
00aa2f06 d941f8               fld dword ptr [ecx - 8]
00aa2f09 89442450             mov dword ptr [esp + 0x50], eax
00aa2f0d d941fc               fld dword ptr [ecx - 4]
00aa2f10 8b4108               mov eax, dword ptr [ecx + 8]
00aa2f13 d901                 fld dword ptr [ecx]
00aa2f15 89442454             mov dword ptr [esp + 0x54], eax
00aa2f19 d9c0                 fld st(0)
00aa2f1b 8b410c               mov eax, dword ptr [ecx + 0xc]
00aa2f1e d8c9                 fmul st(1)
00aa2f20 89442458             mov dword ptr [esp + 0x58], eax
00aa2f24 d9c2                 fld st(2)
00aa2f26 8bd6                 mov edx, esi
00aa2f28 d8cb                 fmul st(3)
00aa2f2a 8bf3                 mov esi, ebx
00aa2f2c 8bda                 mov ebx, edx
00aa2f2e dec1                 faddp st(1)
00aa2f30 d9c3                 fld st(3)
00aa2f32 d8cc                 fmul st(4)
00aa2f34 dec1                 faddp st(1)
00aa2f36 d9c4                 fld st(4)
00aa2f38 d8cd                 fmul st(5)
00aa2f3a dec1                 faddp st(1)
00aa2f3c dddc                 fstp st(4)
00aa2f3e ddd8                 fstp st(0)
00aa2f40 ddd8                 fstp st(0)
00aa2f42 ddd8                 fstp st(0)
00aa2f44 d905dcde2201         fld dword ptr [0x122dedc]
00aa2f4a d9c1                 fld st(1)
00aa2f4c dae9                 fucompp 
00aa2f4e dfe0                 fnstsw ax
00aa2f50 f6c444               test ah, 0x44
00aa2f53 7b08                 jnp 0xaa2f5d
00aa2f55 d83ddcdc2201         fdivr dword ptr [0x122dcdc]
00aa2f5b eb08                 jmp 0xaa2f65
00aa2f5d ddd8                 fstp st(0)
00aa2f5f d905d8de2201         fld dword ptr [0x122ded8]
00aa2f65 d9c0                 fld st(0)
00aa2f67 8b442450             mov eax, dword ptr [esp + 0x50]
00aa2f6b d849f4               fmul dword ptr [ecx - 0xc]
00aa2f6e d9c1                 fld st(1)
00aa2f70 d849f8               fmul dword ptr [ecx - 8]
00aa2f73 d95c2438             fstp dword ptr [esp + 0x38]
00aa2f77 d9c9                 fxch st(1)
00aa2f79 d849fc               fmul dword ptr [ecx - 4]
00aa2f7c d95c2424             fstp dword ptr [esp + 0x24]
00aa2f80 d9c0                 fld st(0)
00aa2f82 d809                 fmul dword ptr [ecx]
00aa2f84 d95c244c             fstp dword ptr [esp + 0x4c]
00aa2f88 d9442438             fld dword ptr [esp + 0x38]
00aa2f8c d809                 fmul dword ptr [ecx]
00aa2f8e d95c2440             fstp dword ptr [esp + 0x40]
00aa2f92 d9442424             fld dword ptr [esp + 0x24]
00aa2f96 d809                 fmul dword ptr [ecx]
00aa2f98 d95c2420             fstp dword ptr [esp + 0x20]
00aa2f9c d849f4               fmul dword ptr [ecx - 0xc]
00aa2f9f d9442438             fld dword ptr [esp + 0x38]
00aa2fa3 d849f4               fmul dword ptr [ecx - 0xc]
00aa2fa6 d9442424             fld dword ptr [esp + 0x24]
00aa2faa d849f4               fmul dword ptr [ecx - 0xc]
00aa2fad d9442438             fld dword ptr [esp + 0x38]
00aa2fb1 d849f8               fmul dword ptr [ecx - 8]
00aa2fb4 d95c2448             fstp dword ptr [esp + 0x48]
00aa2fb8 d9442424             fld dword ptr [esp + 0x24]
00aa2fbc d849f8               fmul dword ptr [ecx - 8]
00aa2fbf d95c2414             fstp dword ptr [esp + 0x14]
00aa2fc3 d9442424             fld dword ptr [esp + 0x24]
00aa2fc7 d849fc               fmul dword ptr [ecx - 4]
00aa2fca 898424c0000000       mov dword ptr [esp + 0xc0], eax
00aa2fd1 d9442448             fld dword ptr [esp + 0x48]
00aa2fd5 8b442454             mov eax, dword ptr [esp + 0x54]
00aa2fd9 898424c4000000       mov dword ptr [esp + 0xc4], eax
00aa2fe0 d8c1                 fadd st(1)
00aa2fe2 8b442458             mov eax, dword ptr [esp + 0x58]
00aa2fe6 898424c8000000       mov dword ptr [esp + 0xc8], eax
00aa2fed a080283d01           mov al, byte ptr [0x13d2880]
00aa2ff2 84c0                 test al, al
00aa2ff4 d82dd8de2201         fsubr dword ptr [0x122ded8]
00aa2ffa c784249c00000000000000 mov dword ptr [esp + 0x9c], 0
00aa3005 c78424ac00000000000000 mov dword ptr [esp + 0xac], 0
00aa3010 d99c2490000000       fstp dword ptr [esp + 0x90]
00aa3017 c78424bc00000000000000 mov dword ptr [esp + 0xbc], 0
00aa3022 d9c2                 fld st(2)
00aa3024 c78424cc0000000000803f mov dword ptr [esp + 0xcc], 0x3f800000
00aa302f d8642420             fsub dword ptr [esp + 0x20]
00aa3033 d99c2494000000       fstp dword ptr [esp + 0x94]
00aa303a d9c1                 fld st(1)
00aa303c d8442440             fadd dword ptr [esp + 0x40]
00aa3040 d99c2498000000       fstp dword ptr [esp + 0x98]
00aa3047 d9ca                 fxch st(2)
00aa3049 d8442420             fadd dword ptr [esp + 0x20]
00aa304d d99c24a0000000       fstp dword ptr [esp + 0xa0]
00aa3054 d9c9                 fxch st(1)
00aa3056 d8c2                 fadd st(2)
00aa3058 d82dd8de2201         fsubr dword ptr [0x122ded8]
00aa305e d99c24a4000000       fstp dword ptr [esp + 0xa4]
00aa3065 d9442414             fld dword ptr [esp + 0x14]
00aa3069 d864244c             fsub dword ptr [esp + 0x4c]
00aa306d d99c24a8000000       fstp dword ptr [esp + 0xa8]
00aa3074 d8642440             fsub dword ptr [esp + 0x40]
00aa3078 d99c24b0000000       fstp dword ptr [esp + 0xb0]
00aa307f d9442414             fld dword ptr [esp + 0x14]
00aa3083 d844244c             fadd dword ptr [esp + 0x4c]
00aa3087 d99c24b4000000       fstp dword ptr [esp + 0xb4]
00aa308e d9442448             fld dword ptr [esp + 0x48]
00aa3092 d8c1                 fadd st(1)
00aa3094 d82dd8de2201         fsubr dword ptr [0x122ded8]
00aa309a d99c24b8000000       fstp dword ptr [esp + 0xb8]
00aa30a1 ddd8                 fstp st(0)
00aa30a3 0f8409010000         je 0xaa31b2
00aa30a9 89542414             mov dword ptr [esp + 0x14], edx
00aa30ad 8d942490000000       lea edx, [esp + 0x90]
00aa30b4 89542440             mov dword ptr [esp + 0x40], edx
00aa30b8 89742420             mov dword ptr [esp + 0x20], esi
00aa30bc 8b442414             mov eax, dword ptr [esp + 0x14]
00aa30c0 8b542440             mov edx, dword ptr [esp + 0x40]
00aa30c4 8b7c2420             mov edi, dword ptr [esp + 0x20]
00aa30c8 0f2802               movaps xmm0, xmmword ptr [edx]
00aa30cb 0f284a10             movaps xmm1, xmmword ptr [edx + 0x10]
00aa30cf 0f285220             movaps xmm2, xmmword ptr [edx + 0x20]
00aa30d3 0f285a30             movaps xmm3, xmmword ptr [edx + 0x30]
00aa30d7 0f2820               movaps xmm4, xmmword ptr [eax]
00aa30da 0f28ec               movaps xmm5, xmm4
00aa30dd 0f28f4               movaps xmm6, xmm4
00aa30e0 0f28fc               movaps xmm7, xmm4
00aa30e3 0fc6e400             shufps xmm4, xmm4, 0
00aa30e7 0fc6ed55             shufps xmm5, xmm5, 0x55
00aa30eb 0fc6f6aa             shufps xmm6, xmm6, 0xaa
00aa30ef 0fc6ffff             shufps xmm7, xmm7, 0xff
00aa30f3 0f59e0               mulps xmm4, xmm0
00aa30f6 0f59e9               mulps xmm5, xmm1
00aa30f9 0f59f2               mulps xmm6, xmm2
00aa30fc 0f59fb               mulps xmm7, xmm3
00aa30ff 0f58e5               addps xmm4, xmm5
00aa3102 0f58f7               addps xmm6, xmm7
00aa3105 0f58e6               addps xmm4, xmm6
00aa3108 0f2927               movaps xmmword ptr [edi], xmm4
00aa310b 0f286010             movaps xmm4, xmmword ptr [eax + 0x10]
00aa310f 0f28ec               movaps xmm5, xmm4
00aa3112 0f28f4               movaps xmm6, xmm4
00aa3115 0f28fc               movaps xmm7, xmm4
00aa3118 0fc6e400             shufps xmm4, xmm4, 0
00aa311c 0fc6ed55             shufps xmm5, xmm5, 0x55
00aa3120 0fc6f6aa             shufps xmm6, xmm6, 0xaa
00aa3124 0fc6ffff             shufps xmm7, xmm7, 0xff
00aa3128 0f59e0               mulps xmm4, xmm0
00aa312b 0f59e9               mulps xmm5, xmm1
00aa312e 0f59f2               mulps xmm6, xmm2
00aa3131 0f59fb               mulps xmm7, xmm3
00aa3134 0f58e5               addps xmm4, xmm5
00aa3137 0f58f7               addps xmm6, xmm7
00aa313a 0f58e6               addps xmm4, xmm6
00aa313d 0f296710             movaps xmmword ptr [edi + 0x10], xmm4
00aa3141 0f286020             movaps xmm4, xmmword ptr [eax + 0x20]
00aa3145 0f28ec               movaps xmm5, xmm4
00aa3148 0f28f4               movaps xmm6, xmm4
00aa314b 0f28fc               movaps xmm7, xmm4
00aa314e 0fc6e400             shufps xmm4, xmm4, 0
00aa3152 0fc6ed55             shufps xmm5, xmm5, 0x55
00aa3156 0fc6f6aa             shufps xmm6, xmm6, 0xaa
00aa315a 0fc6ffff             shufps xmm7, xmm7, 0xff
00aa315e 0f59e0               mulps xmm4, xmm0
00aa3161 0f59e9               mulps xmm5, xmm1
00aa3164 0f59f2               mulps xmm6, xmm2
00aa3167 0f59fb               mulps xmm7, xmm3
00aa316a 0f58e5               addps xmm4, xmm5
00aa316d 0f58f7               addps xmm6, xmm7
00aa3170 0f58e6               addps xmm4, xmm6
00aa3173 0f296720             movaps xmmword ptr [edi + 0x20], xmm4
00aa3177 0f286030             movaps xmm4, xmmword ptr [eax + 0x30]
00aa317b 0f28ec               movaps xmm5, xmm4
00aa317e 0f28f4               movaps xmm6, xmm4
00aa3181 0f28fc               movaps xmm7, xmm4
00aa3184 0fc6e400             shufps xmm4, xmm4, 0
00aa3188 0fc6ed55             shufps xmm5, xmm5, 0x55
00aa318c 0fc6f6aa             shufps xmm6, xmm6, 0xaa
00aa3190 0fc6ffff             shufps xmm7, xmm7, 0xff
00aa3194 0f59e0               mulps xmm4, xmm0
00aa3197 0f59e9               mulps xmm5, xmm1
00aa319a 0f59f2               mulps xmm6, xmm2
00aa319d 0f59fb               mulps xmm7, xmm3
00aa31a0 0f58e5               addps xmm4, xmm5
00aa31a3 0f58f7               addps xmm6, xmm7
00aa31a6 0f58e6               addps xmm4, xmm6
00aa31a9 0f296730             movaps xmmword ptr [edi + 0x30], xmm4
00aa31ad e9ba000000           jmp 0xaa326c
00aa31b2 8d4208               lea eax, [edx + 8]
00aa31b5 8d5608               lea edx, [esi + 8]
00aa31b8 bf04000000           mov edi, 4
00aa31bd 8d4900               lea ecx, [ecx]
00aa31c0 d98424b0000000       fld dword ptr [esp + 0xb0]
00aa31c7 83c010               add eax, 0x10
00aa31ca d848f0               fmul dword ptr [eax - 0x10]
00aa31cd 83c210               add edx, 0x10
00aa31d0 4f                   dec edi
00aa31d1 d98424a0000000       fld dword ptr [esp + 0xa0]
00aa31d8 d848ec               fmul dword ptr [eax - 0x14]
00aa31db dec1                 faddp st(1)
00aa31dd d9842490000000       fld dword ptr [esp + 0x90]
00aa31e4 d848e8               fmul dword ptr [eax - 0x18]
00aa31e7 dec1                 faddp st(1)
00aa31e9 d9442450             fld dword ptr [esp + 0x50]
00aa31ed d848f4               fmul dword ptr [eax - 0xc]
00aa31f0 dec1                 faddp st(1)
00aa31f2 d95ae8               fstp dword ptr [edx - 0x18]
00aa31f5 d98424b4000000       fld dword ptr [esp + 0xb4]
00aa31fc d848f0               fmul dword ptr [eax - 0x10]
00aa31ff d98424a4000000       fld dword ptr [esp + 0xa4]
00aa3206 d848ec               fmul dword ptr [eax - 0x14]
00aa3209 dec1                 faddp st(1)
00aa320b d9842494000000       fld dword ptr [esp + 0x94]
00aa3212 d848e8               fmul dword ptr [eax - 0x18]
00aa3215 dec1                 faddp st(1)
00aa3217 d9442454             fld dword ptr [esp + 0x54]
00aa321b d848f4               fmul dword ptr [eax - 0xc]
00aa321e dec1                 faddp st(1)
00aa3220 d95aec               fstp dword ptr [edx - 0x14]
00aa3223 d98424b8000000       fld dword ptr [esp + 0xb8]
00aa322a d848f0               fmul dword ptr [eax - 0x10]
00aa322d d98424a8000000       fld dword ptr [esp + 0xa8]
00aa3234 d848ec               fmul dword ptr [eax - 0x14]
00aa3237 dec1                 faddp st(1)
00aa3239 d9842498000000       fld dword ptr [esp + 0x98]
00aa3240 d848e8               fmul dword ptr [eax - 0x18]
00aa3243 dec1                 faddp st(1)
00aa3245 d9442458             fld dword ptr [esp + 0x58]
00aa3249 d848f4               fmul dword ptr [eax - 0xc]
00aa324c dec1                 faddp st(1)
00aa324e d95af0               fstp dword ptr [edx - 0x10]
00aa3251 d940e8               fld dword ptr [eax - 0x18]
00aa3254 d840f0               fadd dword ptr [eax - 0x10]
00aa3257 d840ec               fadd dword ptr [eax - 0x14]
00aa325a d80ddcde2201         fmul dword ptr [0x122dedc]
00aa3260 d840f4               fadd dword ptr [eax - 0xc]
00aa3263 d95af4               fstp dword ptr [edx - 0xc]
00aa3266 0f8554ffffff         jne 0xaa31c0
00aa326c 8b442470             mov eax, dword ptr [esp + 0x70]
00aa3270 83c130               add ecx, 0x30
00aa3273 48                   dec eax
00aa3274 89442470             mov dword ptr [esp + 0x70], eax
00aa3278 0f8582fcffff         jne 0xaa2f00
00aa327e 8b451c               mov eax, dword ptr [ebp + 0x1c]
00aa3281 8b0e                 mov ecx, dword ptr [esi]
00aa3283 8b5604               mov edx, dword ptr [esi + 4]
00aa3286 8908                 mov dword ptr [eax], ecx
00aa3288 8b4e08               mov ecx, dword ptr [esi + 8]
00aa328b 894808               mov dword ptr [eax + 8], ecx
00aa328e 8b4e14               mov ecx, dword ptr [esi + 0x14]
00aa3291 895004               mov dword ptr [eax + 4], edx
00aa3294 8b5610               mov edx, dword ptr [esi + 0x10]
00aa3297 894810               mov dword ptr [eax + 0x10], ecx
00aa329a 8b4e20               mov ecx, dword ptr [esi + 0x20]
00aa329d 89500c               mov dword ptr [eax + 0xc], edx
00aa32a0 8b5618               mov edx, dword ptr [esi + 0x18]
00aa32a3 894818               mov dword ptr [eax + 0x18], ecx
00aa32a6 8b4e28               mov ecx, dword ptr [esi + 0x28]
00aa32a9 895014               mov dword ptr [eax + 0x14], edx
00aa32ac 8b5624               mov edx, dword ptr [esi + 0x24]
00aa32af 894820               mov dword ptr [eax + 0x20], ecx
00aa32b2 8b4e34               mov ecx, dword ptr [esi + 0x34]
00aa32b5 89501c               mov dword ptr [eax + 0x1c], edx
00aa32b8 8b5630               mov edx, dword ptr [esi + 0x30]
00aa32bb 894828               mov dword ptr [eax + 0x28], ecx
00aa32be 8b4c243c             mov ecx, dword ptr [esp + 0x3c]
00aa32c2 895024               mov dword ptr [eax + 0x24], edx
00aa32c5 8b5638               mov edx, dword ptr [esi + 0x38]
00aa32c8 83c114               add ecx, 0x14
00aa32cb 89502c               mov dword ptr [eax + 0x2c], edx
00aa32ce e84d94fbff           call 0xa5c720
00aa32d3 5f                   pop edi
00aa32d4 5e                   pop esi
00aa32d5 5b                   pop ebx
00aa32d6 8be5                 mov esp, ebp
00aa32d8 5d                   pop ebp
00aa32d9 c21c00               ret 0x1c
