00a9e1e0 55                   push ebp
00a9e1e1 8bec                 mov ebp, esp
00a9e1e3 83e4f0               and esp, 0xfffffff0
00a9e1e6 81ec84000000         sub esp, 0x84
00a9e1ec 8a4d14               mov cl, byte ptr [ebp + 0x14]
00a9e1ef 84c9                 test cl, cl
00a9e1f1 8b4510               mov eax, dword ptr [ebp + 0x10]
00a9e1f4 8b8098000000         mov eax, dword ptr [eax + 0x98]
00a9e1fa 53                   push ebx
00a9e1fb 56                   push esi
00a9e1fc 57                   push edi
00a9e1fd 0f84f1040000         je 0xa9e6f4
00a9e203 33ff                 xor edi, edi
00a9e205 3bc7                 cmp eax, edi
00a9e207 0f8672090000         jbe 0xa9eb7f
00a9e20d 8b4d08               mov ecx, dword ptr [ebp + 8]
00a9e210 8b750c               mov esi, dword ptr [ebp + 0xc]
00a9e213 33db                 xor ebx, ebx
00a9e215 83c10c               add ecx, 0xc
00a9e218 83c60c               add esi, 0xc
00a9e21b 89442434             mov dword ptr [esp + 0x34], eax
00a9e21f 90                   nop 
00a9e220 8b5510               mov edx, dword ptr [ebp + 0x10]
00a9e223 8b829c000000         mov eax, dword ptr [edx + 0x9c]
00a9e229 8b540304             mov edx, dword ptr [ebx + eax + 4]
00a9e22d 3bd7                 cmp edx, edi
00a9e22f 0f8d56010000         jge 0xa9e38b
00a9e235 d94104               fld dword ptr [ecx + 4]
00a9e238 8b510c               mov edx, dword ptr [ecx + 0xc]
00a9e23b d94108               fld dword ptr [ecx + 8]
00a9e23e 8954244c             mov dword ptr [esp + 0x4c], edx
00a9e242 d941f4               fld dword ptr [ecx - 0xc]
00a9e245 d941f8               fld dword ptr [ecx - 8]
00a9e248 d941fc               fld dword ptr [ecx - 4]
00a9e24b d901                 fld dword ptr [ecx]
00a9e24d d9c0                 fld st(0)
00a9e24f d8c9                 fmul st(1)
00a9e251 d9c2                 fld st(2)
00a9e253 d8cb                 fmul st(3)
00a9e255 dec1                 faddp st(1)
00a9e257 d9c3                 fld st(3)
00a9e259 d8cc                 fmul st(4)
00a9e25b dec1                 faddp st(1)
00a9e25d d9c4                 fld st(4)
00a9e25f d8cd                 fmul st(5)
00a9e261 dec1                 faddp st(1)
00a9e263 d95c241c             fstp dword ptr [esp + 0x1c]
00a9e267 ddd8                 fstp st(0)
00a9e269 ddd8                 fstp st(0)
00a9e26b ddd8                 fstp st(0)
00a9e26d ddd8                 fstp st(0)
00a9e26f d905dcde2201         fld dword ptr [0x122dedc]
00a9e275 d944241c             fld dword ptr [esp + 0x1c]
00a9e279 dae9                 fucompp 
00a9e27b dfe0                 fnstsw ax
00a9e27d f6c444               test ah, 0x44
00a9e280 7b0c                 jnp 0xa9e28e
00a9e282 d905dcdc2201         fld dword ptr [0x122dcdc]
00a9e288 d874241c             fdiv dword ptr [esp + 0x1c]
00a9e28c eb06                 jmp 0xa9e294
00a9e28e d905d8de2201         fld dword ptr [0x122ded8]
00a9e294 d9c0                 fld st(0)
00a9e296 8b44244c             mov eax, dword ptr [esp + 0x4c]
00a9e29a d849f4               fmul dword ptr [ecx - 0xc]
00a9e29d d9c1                 fld st(1)
00a9e29f d849f8               fmul dword ptr [ecx - 8]
00a9e2a2 d95c2418             fstp dword ptr [esp + 0x18]
00a9e2a6 d9c9                 fxch st(1)
00a9e2a8 d849fc               fmul dword ptr [ecx - 4]
00a9e2ab d95c2414             fstp dword ptr [esp + 0x14]
00a9e2af d9c0                 fld st(0)
00a9e2b1 d809                 fmul dword ptr [ecx]
00a9e2b3 d95c2428             fstp dword ptr [esp + 0x28]
00a9e2b7 d9442418             fld dword ptr [esp + 0x18]
00a9e2bb d809                 fmul dword ptr [ecx]
00a9e2bd d95c2424             fstp dword ptr [esp + 0x24]
00a9e2c1 d9442414             fld dword ptr [esp + 0x14]
00a9e2c5 d809                 fmul dword ptr [ecx]
00a9e2c7 d95c241c             fstp dword ptr [esp + 0x1c]
00a9e2cb d849f4               fmul dword ptr [ecx - 0xc]
00a9e2ce d95c2430             fstp dword ptr [esp + 0x30]
00a9e2d2 d9442418             fld dword ptr [esp + 0x18]
00a9e2d6 d849f4               fmul dword ptr [ecx - 0xc]
00a9e2d9 d9442414             fld dword ptr [esp + 0x14]
00a9e2dd d849f4               fmul dword ptr [ecx - 0xc]
00a9e2e0 d95c2420             fstp dword ptr [esp + 0x20]
00a9e2e4 d9442418             fld dword ptr [esp + 0x18]
00a9e2e8 d849f8               fmul dword ptr [ecx - 8]
00a9e2eb d95c242c             fstp dword ptr [esp + 0x2c]
00a9e2ef d9442414             fld dword ptr [esp + 0x14]
00a9e2f3 d849f8               fmul dword ptr [ecx - 8]
00a9e2f6 d95c2418             fstp dword ptr [esp + 0x18]
00a9e2fa d9442414             fld dword ptr [esp + 0x14]
00a9e2fe d849fc               fmul dword ptr [ecx - 4]
00a9e301 d944242c             fld dword ptr [esp + 0x2c]
00a9e305 d8c1                 fadd st(1)
00a9e307 d82dd8de2201         fsubr dword ptr [0x122ded8]
00a9e30d d95ef4               fstp dword ptr [esi - 0xc]
00a9e310 d9c1                 fld st(1)
00a9e312 d844241c             fadd dword ptr [esp + 0x1c]
00a9e316 d95ef8               fstp dword ptr [esi - 8]
00a9e319 d9442420             fld dword ptr [esp + 0x20]
00a9e31d d8642424             fsub dword ptr [esp + 0x24]
00a9e321 d95efc               fstp dword ptr [esi - 4]
00a9e324 d9cb                 fxch st(3)
00a9e326 d91e                 fstp dword ptr [esi]
00a9e328 d864241c             fsub dword ptr [esp + 0x1c]
00a9e32c d95e04               fstp dword ptr [esi + 4]
00a9e32f d9c9                 fxch st(1)
00a9e331 d8442430             fadd dword ptr [esp + 0x30]
00a9e335 d82dd8de2201         fsubr dword ptr [0x122ded8]
00a9e33b d95e08               fstp dword ptr [esi + 8]
00a9e33e d9442418             fld dword ptr [esp + 0x18]
00a9e342 d8442428             fadd dword ptr [esp + 0x28]
00a9e346 d95e0c               fstp dword ptr [esi + 0xc]
00a9e349 d95e10               fstp dword ptr [esi + 0x10]
00a9e34c d9442420             fld dword ptr [esp + 0x20]
00a9e350 d8442424             fadd dword ptr [esp + 0x24]
00a9e354 d95e14               fstp dword ptr [esi + 0x14]
00a9e357 d9442418             fld dword ptr [esp + 0x18]
00a9e35b d8642428             fsub dword ptr [esp + 0x28]
00a9e35f d95e18               fstp dword ptr [esi + 0x18]
00a9e362 d944242c             fld dword ptr [esp + 0x2c]
00a9e366 d8442430             fadd dword ptr [esp + 0x30]
00a9e36a d82dd8de2201         fsubr dword ptr [0x122ded8]
00a9e370 d95e1c               fstp dword ptr [esi + 0x1c]
00a9e373 894620               mov dword ptr [esi + 0x20], eax
00a9e376 897e24               mov dword ptr [esi + 0x24], edi
00a9e379 897e28               mov dword ptr [esi + 0x28], edi
00a9e37c 897e2c               mov dword ptr [esi + 0x2c], edi
00a9e37f c746300000803f       mov dword ptr [esi + 0x30], 0x3f800000
00a9e386 e948030000           jmp 0xa9e6d3
00a9e38b d941f4               fld dword ptr [ecx - 0xc]
00a9e38e 8b4104               mov eax, dword ptr [ecx + 4]
00a9e391 d941f8               fld dword ptr [ecx - 8]
00a9e394 89442438             mov dword ptr [esp + 0x38], eax
00a9e398 d941fc               fld dword ptr [ecx - 4]
00a9e39b 8b4108               mov eax, dword ptr [ecx + 8]
00a9e39e d901                 fld dword ptr [ecx]
00a9e3a0 8944243c             mov dword ptr [esp + 0x3c], eax
00a9e3a4 d9c0                 fld st(0)
00a9e3a6 8b410c               mov eax, dword ptr [ecx + 0xc]
00a9e3a9 d8c9                 fmul st(1)
00a9e3ab 89442440             mov dword ptr [esp + 0x40], eax
00a9e3af d9c2                 fld st(2)
00a9e3b1 d8cb                 fmul st(3)
00a9e3b3 dec1                 faddp st(1)
00a9e3b5 d9c3                 fld st(3)
00a9e3b7 d8cc                 fmul st(4)
00a9e3b9 dec1                 faddp st(1)
00a9e3bb d9c4                 fld st(4)
00a9e3bd d8cd                 fmul st(5)
00a9e3bf dec1                 faddp st(1)
00a9e3c1 dddc                 fstp st(4)
00a9e3c3 ddd8                 fstp st(0)
00a9e3c5 ddd8                 fstp st(0)
00a9e3c7 ddd8                 fstp st(0)
00a9e3c9 d905dcde2201         fld dword ptr [0x122dedc]
00a9e3cf d9c1                 fld st(1)
00a9e3d1 dae9                 fucompp 
00a9e3d3 dfe0                 fnstsw ax
00a9e3d5 f6c444               test ah, 0x44
00a9e3d8 7b08                 jnp 0xa9e3e2
00a9e3da d83ddcdc2201         fdivr dword ptr [0x122dcdc]
00a9e3e0 eb08                 jmp 0xa9e3ea
00a9e3e2 ddd8                 fstp st(0)
00a9e3e4 d905d8de2201         fld dword ptr [0x122ded8]
00a9e3ea d9c0                 fld st(0)
00a9e3ec 8b442438             mov eax, dword ptr [esp + 0x38]
00a9e3f0 d849f4               fmul dword ptr [ecx - 0xc]
00a9e3f3 8b7d0c               mov edi, dword ptr [ebp + 0xc]
00a9e3f6 d9c1                 fld st(1)
00a9e3f8 8944245c             mov dword ptr [esp + 0x5c], eax
00a9e3fc d849f8               fmul dword ptr [ecx - 8]
00a9e3ff 8b44243c             mov eax, dword ptr [esp + 0x3c]
00a9e403 c1e206               shl edx, 6
00a9e406 8944246c             mov dword ptr [esp + 0x6c], eax
00a9e40a d95c2418             fstp dword ptr [esp + 0x18]
00a9e40e 8b442440             mov eax, dword ptr [esp + 0x40]
00a9e412 d9c9                 fxch st(1)
00a9e414 03d7                 add edx, edi
00a9e416 d849fc               fmul dword ptr [ecx - 4]
00a9e419 8944247c             mov dword ptr [esp + 0x7c], eax
00a9e41d 8bc2                 mov eax, edx
00a9e41f 8a1580283d01         mov dl, byte ptr [0x13d2880]
00a9e425 84d2                 test dl, dl
00a9e427 d95c2414             fstp dword ptr [esp + 0x14]
00a9e42b c784248000000000000000 mov dword ptr [esp + 0x80], 0
00a9e436 d9c0                 fld st(0)
00a9e438 c784248400000000000000 mov dword ptr [esp + 0x84], 0
00a9e443 d809                 fmul dword ptr [ecx]
00a9e445 c784248800000000000000 mov dword ptr [esp + 0x88], 0
00a9e450 c784248c0000000000803f mov dword ptr [esp + 0x8c], 0x3f800000
00a9e45b d95c2424             fstp dword ptr [esp + 0x24]
00a9e45f d9442418             fld dword ptr [esp + 0x18]
00a9e463 d809                 fmul dword ptr [ecx]
00a9e465 d95c242c             fstp dword ptr [esp + 0x2c]
00a9e469 d9442414             fld dword ptr [esp + 0x14]
00a9e46d d809                 fmul dword ptr [ecx]
00a9e46f d95c2430             fstp dword ptr [esp + 0x30]
00a9e473 d849f4               fmul dword ptr [ecx - 0xc]
00a9e476 d9442418             fld dword ptr [esp + 0x18]
00a9e47a d849f4               fmul dword ptr [ecx - 0xc]
00a9e47d d9442414             fld dword ptr [esp + 0x14]
00a9e481 d849f4               fmul dword ptr [ecx - 0xc]
00a9e484 d9442418             fld dword ptr [esp + 0x18]
00a9e488 d849f8               fmul dword ptr [ecx - 8]
00a9e48b d95c2420             fstp dword ptr [esp + 0x20]
00a9e48f d9442414             fld dword ptr [esp + 0x14]
00a9e493 d849f8               fmul dword ptr [ecx - 8]
00a9e496 d95c2428             fstp dword ptr [esp + 0x28]
00a9e49a d9442414             fld dword ptr [esp + 0x14]
00a9e49e d849fc               fmul dword ptr [ecx - 4]
00a9e4a1 d9442420             fld dword ptr [esp + 0x20]
00a9e4a5 d8c1                 fadd st(1)
00a9e4a7 d82dd8de2201         fsubr dword ptr [0x122ded8]
00a9e4ad d95c2450             fstp dword ptr [esp + 0x50]
00a9e4b1 d9c2                 fld st(2)
00a9e4b3 d8442430             fadd dword ptr [esp + 0x30]
00a9e4b7 d95c2454             fstp dword ptr [esp + 0x54]
00a9e4bb d9c1                 fld st(1)
00a9e4bd d864242c             fsub dword ptr [esp + 0x2c]
00a9e4c1 d95c2458             fstp dword ptr [esp + 0x58]
00a9e4c5 d9ca                 fxch st(2)
00a9e4c7 d8642430             fsub dword ptr [esp + 0x30]
00a9e4cb d95c2460             fstp dword ptr [esp + 0x60]
00a9e4cf d9c9                 fxch st(1)
00a9e4d1 d8c2                 fadd st(2)
00a9e4d3 d82dd8de2201         fsubr dword ptr [0x122ded8]
00a9e4d9 d95c2464             fstp dword ptr [esp + 0x64]
00a9e4dd d9442428             fld dword ptr [esp + 0x28]
00a9e4e1 d8442424             fadd dword ptr [esp + 0x24]
00a9e4e5 d95c2468             fstp dword ptr [esp + 0x68]
00a9e4e9 d844242c             fadd dword ptr [esp + 0x2c]
00a9e4ed d95c2470             fstp dword ptr [esp + 0x70]
00a9e4f1 d9442428             fld dword ptr [esp + 0x28]
00a9e4f5 d8642424             fsub dword ptr [esp + 0x24]
00a9e4f9 d95c2474             fstp dword ptr [esp + 0x74]
00a9e4fd d9442420             fld dword ptr [esp + 0x20]
00a9e501 d8c1                 fadd st(1)
00a9e503 d82dd8de2201         fsubr dword ptr [0x122ded8]
00a9e509 d95c2478             fstp dword ptr [esp + 0x78]
00a9e50d ddd8                 fstp st(0)
00a9e50f 0f8409010000         je 0xa9e61e
00a9e515 89442428             mov dword ptr [esp + 0x28], eax
00a9e519 8d542450             lea edx, [esp + 0x50]
00a9e51d 8d46f4               lea eax, [esi - 0xc]
00a9e520 8954242c             mov dword ptr [esp + 0x2c], edx
00a9e524 89442430             mov dword ptr [esp + 0x30], eax
00a9e528 8b442428             mov eax, dword ptr [esp + 0x28]
00a9e52c 8b54242c             mov edx, dword ptr [esp + 0x2c]
00a9e530 8b7c2430             mov edi, dword ptr [esp + 0x30]
00a9e534 0f2802               movaps xmm0, xmmword ptr [edx]
00a9e537 0f284a10             movaps xmm1, xmmword ptr [edx + 0x10]
00a9e53b 0f285220             movaps xmm2, xmmword ptr [edx + 0x20]
00a9e53f 0f285a30             movaps xmm3, xmmword ptr [edx + 0x30]
00a9e543 0f2820               movaps xmm4, xmmword ptr [eax]
00a9e546 0f28ec               movaps xmm5, xmm4
00a9e549 0f28f4               movaps xmm6, xmm4
00a9e54c 0f28fc               movaps xmm7, xmm4
00a9e54f 0fc6e400             shufps xmm4, xmm4, 0
00a9e553 0fc6ed55             shufps xmm5, xmm5, 0x55
00a9e557 0fc6f6aa             shufps xmm6, xmm6, 0xaa
00a9e55b 0fc6ffff             shufps xmm7, xmm7, 0xff
00a9e55f 0f59e0               mulps xmm4, xmm0
00a9e562 0f59e9               mulps xmm5, xmm1
00a9e565 0f59f2               mulps xmm6, xmm2
00a9e568 0f59fb               mulps xmm7, xmm3
00a9e56b 0f58e5               addps xmm4, xmm5
00a9e56e 0f58f7               addps xmm6, xmm7
00a9e571 0f58e6               addps xmm4, xmm6
00a9e574 0f2927               movaps xmmword ptr [edi], xmm4
00a9e577 0f286010             movaps xmm4, xmmword ptr [eax + 0x10]
00a9e57b 0f28ec               movaps xmm5, xmm4
00a9e57e 0f28f4               movaps xmm6, xmm4
00a9e581 0f28fc               movaps xmm7, xmm4
00a9e584 0fc6e400             shufps xmm4, xmm4, 0
00a9e588 0fc6ed55             shufps xmm5, xmm5, 0x55
00a9e58c 0fc6f6aa             shufps xmm6, xmm6, 0xaa
00a9e590 0fc6ffff             shufps xmm7, xmm7, 0xff
00a9e594 0f59e0               mulps xmm4, xmm0
00a9e597 0f59e9               mulps xmm5, xmm1
00a9e59a 0f59f2               mulps xmm6, xmm2
00a9e59d 0f59fb               mulps xmm7, xmm3
00a9e5a0 0f58e5               addps xmm4, xmm5
00a9e5a3 0f58f7               addps xmm6, xmm7
00a9e5a6 0f58e6               addps xmm4, xmm6
00a9e5a9 0f296710             movaps xmmword ptr [edi + 0x10], xmm4
00a9e5ad 0f286020             movaps xmm4, xmmword ptr [eax + 0x20]
00a9e5b1 0f28ec               movaps xmm5, xmm4
00a9e5b4 0f28f4               movaps xmm6, xmm4
00a9e5b7 0f28fc               movaps xmm7, xmm4
00a9e5ba 0fc6e400             shufps xmm4, xmm4, 0
00a9e5be 0fc6ed55             shufps xmm5, xmm5, 0x55
00a9e5c2 0fc6f6aa             shufps xmm6, xmm6, 0xaa
00a9e5c6 0fc6ffff             shufps xmm7, xmm7, 0xff
00a9e5ca 0f59e0               mulps xmm4, xmm0
00a9e5cd 0f59e9               mulps xmm5, xmm1
00a9e5d0 0f59f2               mulps xmm6, xmm2
00a9e5d3 0f59fb               mulps xmm7, xmm3
00a9e5d6 0f58e5               addps xmm4, xmm5
00a9e5d9 0f58f7               addps xmm6, xmm7
00a9e5dc 0f58e6               addps xmm4, xmm6
00a9e5df 0f296720             movaps xmmword ptr [edi + 0x20], xmm4
00a9e5e3 0f286030             movaps xmm4, xmmword ptr [eax + 0x30]
00a9e5e7 0f28ec               movaps xmm5, xmm4
00a9e5ea 0f28f4               movaps xmm6, xmm4
00a9e5ed 0f28fc               movaps xmm7, xmm4
00a9e5f0 0fc6e400             shufps xmm4, xmm4, 0
00a9e5f4 0fc6ed55             shufps xmm5, xmm5, 0x55
00a9e5f8 0fc6f6aa             shufps xmm6, xmm6, 0xaa
00a9e5fc 0fc6ffff             shufps xmm7, xmm7, 0xff
00a9e600 0f59e0               mulps xmm4, xmm0
00a9e603 0f59e9               mulps xmm5, xmm1
00a9e606 0f59f2               mulps xmm6, xmm2
00a9e609 0f59fb               mulps xmm7, xmm3
00a9e60c 0f58e5               addps xmm4, xmm5
00a9e60f 0f58f7               addps xmm6, xmm7
00a9e612 0f58e6               addps xmm4, xmm6
00a9e615 0f296730             movaps xmmword ptr [edi + 0x30], xmm4
00a9e619 e9b3000000           jmp 0xa9e6d1
00a9e61e 83c008               add eax, 8
00a9e621 8d56fc               lea edx, [esi - 4]
00a9e624 bf04000000           mov edi, 4
00a9e629 8da42400000000       lea esp, [esp]
00a9e630 d9442470             fld dword ptr [esp + 0x70]
00a9e634 83c010               add eax, 0x10
00a9e637 d848f0               fmul dword ptr [eax - 0x10]
00a9e63a 83c210               add edx, 0x10
00a9e63d 4f                   dec edi
00a9e63e d9442460             fld dword ptr [esp + 0x60]
00a9e642 d848ec               fmul dword ptr [eax - 0x14]
00a9e645 dec1                 faddp st(1)
00a9e647 d9442450             fld dword ptr [esp + 0x50]
00a9e64b d848e8               fmul dword ptr [eax - 0x18]
00a9e64e dec1                 faddp st(1)
00a9e650 d940f4               fld dword ptr [eax - 0xc]
00a9e653 d80ddcde2201         fmul dword ptr [0x122dedc]
00a9e659 dec1                 faddp st(1)
00a9e65b d95ae8               fstp dword ptr [edx - 0x18]
00a9e65e d9442474             fld dword ptr [esp + 0x74]
00a9e662 d848f0               fmul dword ptr [eax - 0x10]
00a9e665 d9442464             fld dword ptr [esp + 0x64]
00a9e669 d848ec               fmul dword ptr [eax - 0x14]
00a9e66c dec1                 faddp st(1)
00a9e66e d9442454             fld dword ptr [esp + 0x54]
00a9e672 d848e8               fmul dword ptr [eax - 0x18]
00a9e675 dec1                 faddp st(1)
00a9e677 d940f4               fld dword ptr [eax - 0xc]
00a9e67a d80ddcde2201         fmul dword ptr [0x122dedc]
00a9e680 dec1                 faddp st(1)
00a9e682 d95aec               fstp dword ptr [edx - 0x14]
00a9e685 d9442478             fld dword ptr [esp + 0x78]
00a9e689 d848f0               fmul dword ptr [eax - 0x10]
00a9e68c d9442468             fld dword ptr [esp + 0x68]
00a9e690 d848ec               fmul dword ptr [eax - 0x14]
00a9e693 dec1                 faddp st(1)
00a9e695 d9442458             fld dword ptr [esp + 0x58]
00a9e699 d848e8               fmul dword ptr [eax - 0x18]
00a9e69c dec1                 faddp st(1)
00a9e69e d940f4               fld dword ptr [eax - 0xc]
00a9e6a1 d80ddcde2201         fmul dword ptr [0x122dedc]
00a9e6a7 dec1                 faddp st(1)
00a9e6a9 d95af0               fstp dword ptr [edx - 0x10]
00a9e6ac d9442440             fld dword ptr [esp + 0x40]
00a9e6b0 d848f0               fmul dword ptr [eax - 0x10]
00a9e6b3 d944243c             fld dword ptr [esp + 0x3c]
00a9e6b7 d848ec               fmul dword ptr [eax - 0x14]
00a9e6ba dec1                 faddp st(1)
00a9e6bc d9442438             fld dword ptr [esp + 0x38]
00a9e6c0 d848e8               fmul dword ptr [eax - 0x18]
00a9e6c3 dec1                 faddp st(1)
00a9e6c5 d840f4               fadd dword ptr [eax - 0xc]
00a9e6c8 d95af4               fstp dword ptr [edx - 0xc]
00a9e6cb 0f855fffffff         jne 0xa9e630
00a9e6d1 33ff                 xor edi, edi
00a9e6d3 8b442434             mov eax, dword ptr [esp + 0x34]
00a9e6d7 83c33c               add ebx, 0x3c
00a9e6da 83c130               add ecx, 0x30
00a9e6dd 83c640               add esi, 0x40
00a9e6e0 48                   dec eax
00a9e6e1 89442434             mov dword ptr [esp + 0x34], eax
00a9e6e5 0f8535fbffff         jne 0xa9e220
00a9e6eb 5f                   pop edi
00a9e6ec 5e                   pop esi
00a9e6ed 5b                   pop ebx
00a9e6ee 8be5                 mov esp, ebp
00a9e6f0 5d                   pop ebp
00a9e6f1 c21000               ret 0x10
00a9e6f4 33d2                 xor edx, edx
00a9e6f6 3bc2                 cmp eax, edx
00a9e6f8 0f8681040000         jbe 0xa9eb7f
00a9e6fe 8b7508               mov esi, dword ptr [ebp + 8]
00a9e701 8b5d0c               mov ebx, dword ptr [ebp + 0xc]
00a9e704 83c60c               add esi, 0xc
00a9e707 8954241c             mov dword ptr [esp + 0x1c], edx
00a9e70b 83c30c               add ebx, 0xc
00a9e70e 89442420             mov dword ptr [esp + 0x20], eax
00a9e712 eb02                 jmp 0xa9e716
00a9e714 33d2                 xor edx, edx
00a9e716 d94604               fld dword ptr [esi + 4]
00a9e719 8b4d10               mov ecx, dword ptr [ebp + 0x10]
00a9e71c d94608               fld dword ptr [esi + 8]
00a9e71f 8b819c000000         mov eax, dword ptr [ecx + 0x9c]
00a9e725 d946f4               fld dword ptr [esi - 0xc]
00a9e728 8b4c241c             mov ecx, dword ptr [esp + 0x1c]
00a9e72c d946f8               fld dword ptr [esi - 8]
00a9e72f 8b4c0104             mov ecx, dword ptr [ecx + eax + 4]
00a9e733 3bca                 cmp ecx, edx
00a9e735 d946fc               fld dword ptr [esi - 4]
00a9e738 d906                 fld dword ptr [esi]
00a9e73a d9c0                 fld st(0)
00a9e73c d8c9                 fmul st(1)
00a9e73e d9c2                 fld st(2)
00a9e740 d8cb                 fmul st(3)
00a9e742 dec1                 faddp st(1)
00a9e744 d9c3                 fld st(3)
00a9e746 d8cc                 fmul st(4)
00a9e748 dec1                 faddp st(1)
00a9e74a d9c4                 fld st(4)
00a9e74c d8cd                 fmul st(5)
00a9e74e dec1                 faddp st(1)
00a9e750 d95c2434             fstp dword ptr [esp + 0x34]
00a9e754 ddd8                 fstp st(0)
00a9e756 ddd8                 fstp st(0)
00a9e758 ddd8                 fstp st(0)
00a9e75a ddd8                 fstp st(0)
00a9e75c d905dcde2201         fld dword ptr [0x122dedc]
00a9e762 d9442434             fld dword ptr [esp + 0x34]
00a9e766 dae9                 fucompp 
00a9e768 0f8d09010000         jge 0xa9e877
00a9e76e 8b460c               mov eax, dword ptr [esi + 0xc]
00a9e771 8944244c             mov dword ptr [esp + 0x4c], eax
00a9e775 dfe0                 fnstsw ax
00a9e777 f6c444               test ah, 0x44
00a9e77a 7b0c                 jnp 0xa9e788
00a9e77c d905dcdc2201         fld dword ptr [0x122dcdc]
00a9e782 d8742434             fdiv dword ptr [esp + 0x34]
00a9e786 eb06                 jmp 0xa9e78e
00a9e788 d905d8de2201         fld dword ptr [0x122ded8]
00a9e78e d9c0                 fld st(0)
00a9e790 8b4c244c             mov ecx, dword ptr [esp + 0x4c]
00a9e794 d84ef4               fmul dword ptr [esi - 0xc]
00a9e797 d9c1                 fld st(1)
00a9e799 d84ef8               fmul dword ptr [esi - 8]
00a9e79c d95c2418             fstp dword ptr [esp + 0x18]
00a9e7a0 d9c9                 fxch st(1)
00a9e7a2 d84efc               fmul dword ptr [esi - 4]
00a9e7a5 d95c2414             fstp dword ptr [esp + 0x14]
00a9e7a9 d9c0                 fld st(0)
00a9e7ab d80e                 fmul dword ptr [esi]
00a9e7ad d95c2428             fstp dword ptr [esp + 0x28]
00a9e7b1 d9442418             fld dword ptr [esp + 0x18]
00a9e7b5 d80e                 fmul dword ptr [esi]
00a9e7b7 d95c2430             fstp dword ptr [esp + 0x30]
00a9e7bb d9442414             fld dword ptr [esp + 0x14]
00a9e7bf d80e                 fmul dword ptr [esi]
00a9e7c1 d95c2434             fstp dword ptr [esp + 0x34]
00a9e7c5 d84ef4               fmul dword ptr [esi - 0xc]
00a9e7c8 d9442418             fld dword ptr [esp + 0x18]
00a9e7cc d84ef4               fmul dword ptr [esi - 0xc]
00a9e7cf d9442414             fld dword ptr [esp + 0x14]
00a9e7d3 d84ef4               fmul dword ptr [esi - 0xc]
00a9e7d6 d9442418             fld dword ptr [esp + 0x18]
00a9e7da d84ef8               fmul dword ptr [esi - 8]
00a9e7dd d95c2424             fstp dword ptr [esp + 0x24]
00a9e7e1 d9442414             fld dword ptr [esp + 0x14]
00a9e7e5 d84ef8               fmul dword ptr [esi - 8]
00a9e7e8 d95c242c             fstp dword ptr [esp + 0x2c]
00a9e7ec d9442414             fld dword ptr [esp + 0x14]
00a9e7f0 d84efc               fmul dword ptr [esi - 4]
00a9e7f3 d9442424             fld dword ptr [esp + 0x24]
00a9e7f7 d8c1                 fadd st(1)
00a9e7f9 d82dd8de2201         fsubr dword ptr [0x122ded8]
00a9e7ff d95bf4               fstp dword ptr [ebx - 0xc]
00a9e802 d9c2                 fld st(2)
00a9e804 d8642434             fsub dword ptr [esp + 0x34]
00a9e808 d95bf8               fstp dword ptr [ebx - 8]
00a9e80b d9c1                 fld st(1)
00a9e80d d8442430             fadd dword ptr [esp + 0x30]
00a9e811 d95bfc               fstp dword ptr [ebx - 4]
00a9e814 8913                 mov dword ptr [ebx], edx
00a9e816 d9ca                 fxch st(2)
00a9e818 d8442434             fadd dword ptr [esp + 0x34]
00a9e81c d95b04               fstp dword ptr [ebx + 4]
00a9e81f d9c9                 fxch st(1)
00a9e821 d8c2                 fadd st(2)
00a9e823 d82dd8de2201         fsubr dword ptr [0x122ded8]
00a9e829 d95b08               fstp dword ptr [ebx + 8]
00a9e82c d944242c             fld dword ptr [esp + 0x2c]
00a9e830 d8642428             fsub dword ptr [esp + 0x28]
00a9e834 d95b0c               fstp dword ptr [ebx + 0xc]
00a9e837 895310               mov dword ptr [ebx + 0x10], edx
00a9e83a d8642430             fsub dword ptr [esp + 0x30]
00a9e83e d95b14               fstp dword ptr [ebx + 0x14]
00a9e841 d944242c             fld dword ptr [esp + 0x2c]
00a9e845 d8442428             fadd dword ptr [esp + 0x28]
00a9e849 d95b18               fstp dword ptr [ebx + 0x18]
00a9e84c d9442424             fld dword ptr [esp + 0x24]
00a9e850 d8c1                 fadd st(1)
00a9e852 d82dd8de2201         fsubr dword ptr [0x122ded8]
00a9e858 d95b1c               fstp dword ptr [ebx + 0x1c]
00a9e85b 895320               mov dword ptr [ebx + 0x20], edx
00a9e85e ddd8                 fstp st(0)
00a9e860 d9c9                 fxch st(1)
00a9e862 d95b24               fstp dword ptr [ebx + 0x24]
00a9e865 d95b28               fstp dword ptr [ebx + 0x28]
00a9e868 894b2c               mov dword ptr [ebx + 0x2c], ecx
00a9e86b c743300000803f       mov dword ptr [ebx + 0x30], 0x3f800000
00a9e872 e9e8020000           jmp 0xa9eb5f
00a9e877 8b560c               mov edx, dword ptr [esi + 0xc]
00a9e87a dfe0                 fnstsw ax
00a9e87c 89542440             mov dword ptr [esp + 0x40], edx
00a9e880 f6c444               test ah, 0x44
00a9e883 7b0c                 jnp 0xa9e891
00a9e885 d905dcdc2201         fld dword ptr [0x122dcdc]
00a9e88b d8742434             fdiv dword ptr [esp + 0x34]
00a9e88f eb06                 jmp 0xa9e897
00a9e891 d905d8de2201         fld dword ptr [0x122ded8]
00a9e897 d9c0                 fld st(0)
00a9e899 8b442440             mov eax, dword ptr [esp + 0x40]
00a9e89d d84ef4               fmul dword ptr [esi - 0xc]
00a9e8a0 8b550c               mov edx, dword ptr [ebp + 0xc]
00a9e8a3 d9c1                 fld st(1)
00a9e8a5 c1e106               shl ecx, 6
00a9e8a8 d84ef8               fmul dword ptr [esi - 8]
00a9e8ab 89842488000000       mov dword ptr [esp + 0x88], eax
00a9e8b2 a080283d01           mov al, byte ptr [0x13d2880]
00a9e8b7 03ca                 add ecx, edx
00a9e8b9 84c0                 test al, al
00a9e8bb d95c2418             fstp dword ptr [esp + 0x18]
00a9e8bf d9c9                 fxch st(1)
00a9e8c1 c744245c00000000     mov dword ptr [esp + 0x5c], 0
00a9e8c9 d84efc               fmul dword ptr [esi - 4]
00a9e8cc c744246c00000000     mov dword ptr [esp + 0x6c], 0
00a9e8d4 c744247c00000000     mov dword ptr [esp + 0x7c], 0
00a9e8dc c784248c0000000000803f mov dword ptr [esp + 0x8c], 0x3f800000
00a9e8e7 d95c2414             fstp dword ptr [esp + 0x14]
00a9e8eb d9c0                 fld st(0)
00a9e8ed d80e                 fmul dword ptr [esi]
00a9e8ef d95c2428             fstp dword ptr [esp + 0x28]
00a9e8f3 d9442418             fld dword ptr [esp + 0x18]
00a9e8f7 d80e                 fmul dword ptr [esi]
00a9e8f9 d95c2430             fstp dword ptr [esp + 0x30]
00a9e8fd d9442414             fld dword ptr [esp + 0x14]
00a9e901 d80e                 fmul dword ptr [esi]
00a9e903 d95c2434             fstp dword ptr [esp + 0x34]
00a9e907 d84ef4               fmul dword ptr [esi - 0xc]
00a9e90a d9442418             fld dword ptr [esp + 0x18]
00a9e90e d84ef4               fmul dword ptr [esi - 0xc]
00a9e911 d9442414             fld dword ptr [esp + 0x14]
00a9e915 d84ef4               fmul dword ptr [esi - 0xc]
00a9e918 d9442418             fld dword ptr [esp + 0x18]
00a9e91c d84ef8               fmul dword ptr [esi - 8]
00a9e91f d95c2424             fstp dword ptr [esp + 0x24]
00a9e923 d9442414             fld dword ptr [esp + 0x14]
00a9e927 d84ef8               fmul dword ptr [esi - 8]
00a9e92a d95c242c             fstp dword ptr [esp + 0x2c]
00a9e92e d9442414             fld dword ptr [esp + 0x14]
00a9e932 d84efc               fmul dword ptr [esi - 4]
00a9e935 d9442424             fld dword ptr [esp + 0x24]
00a9e939 d8c1                 fadd st(1)
00a9e93b d82dd8de2201         fsubr dword ptr [0x122ded8]
00a9e941 d95c2450             fstp dword ptr [esp + 0x50]
00a9e945 d9c2                 fld st(2)
00a9e947 d8642434             fsub dword ptr [esp + 0x34]
00a9e94b d95c2454             fstp dword ptr [esp + 0x54]
00a9e94f d9c1                 fld st(1)
00a9e951 d8442430             fadd dword ptr [esp + 0x30]
00a9e955 d95c2458             fstp dword ptr [esp + 0x58]
00a9e959 d9ca                 fxch st(2)
00a9e95b d8442434             fadd dword ptr [esp + 0x34]
00a9e95f d95c2460             fstp dword ptr [esp + 0x60]
00a9e963 d9c9                 fxch st(1)
00a9e965 d8c2                 fadd st(2)
00a9e967 d82dd8de2201         fsubr dword ptr [0x122ded8]
00a9e96d d95c2464             fstp dword ptr [esp + 0x64]
00a9e971 d944242c             fld dword ptr [esp + 0x2c]
00a9e975 d8642428             fsub dword ptr [esp + 0x28]
00a9e979 d95c2468             fstp dword ptr [esp + 0x68]
00a9e97d d8642430             fsub dword ptr [esp + 0x30]
00a9e981 d95c2470             fstp dword ptr [esp + 0x70]
00a9e985 d944242c             fld dword ptr [esp + 0x2c]
00a9e989 d8442428             fadd dword ptr [esp + 0x28]
00a9e98d d95c2474             fstp dword ptr [esp + 0x74]
00a9e991 d9442424             fld dword ptr [esp + 0x24]
00a9e995 d8c1                 fadd st(1)
00a9e997 d82dd8de2201         fsubr dword ptr [0x122ded8]
00a9e99d d95c2478             fstp dword ptr [esp + 0x78]
00a9e9a1 ddd8                 fstp st(0)
00a9e9a3 d9c9                 fxch st(1)
00a9e9a5 d99c2480000000       fstp dword ptr [esp + 0x80]
00a9e9ac d99c2484000000       fstp dword ptr [esp + 0x84]
00a9e9b3 0f8409010000         je 0xa9eac2
00a9e9b9 8d542450             lea edx, [esp + 0x50]
00a9e9bd 8d43f4               lea eax, [ebx - 0xc]
00a9e9c0 8954242c             mov dword ptr [esp + 0x2c], edx
00a9e9c4 894c2430             mov dword ptr [esp + 0x30], ecx
00a9e9c8 89442434             mov dword ptr [esp + 0x34], eax
00a9e9cc 8b44242c             mov eax, dword ptr [esp + 0x2c]
00a9e9d0 8b542430             mov edx, dword ptr [esp + 0x30]
00a9e9d4 8b7c2434             mov edi, dword ptr [esp + 0x34]
00a9e9d8 0f2802               movaps xmm0, xmmword ptr [edx]
00a9e9db 0f284a10             movaps xmm1, xmmword ptr [edx + 0x10]
00a9e9df 0f285220             movaps xmm2, xmmword ptr [edx + 0x20]
00a9e9e3 0f285a30             movaps xmm3, xmmword ptr [edx + 0x30]
00a9e9e7 0f2820               movaps xmm4, xmmword ptr [eax]
00a9e9ea 0f28ec               movaps xmm5, xmm4
00a9e9ed 0f28f4               movaps xmm6, xmm4
00a9e9f0 0f28fc               movaps xmm7, xmm4
00a9e9f3 0fc6e400             shufps xmm4, xmm4, 0
00a9e9f7 0fc6ed55             shufps xmm5, xmm5, 0x55
00a9e9fb 0fc6f6aa             shufps xmm6, xmm6, 0xaa
00a9e9ff 0fc6ffff             shufps xmm7, xmm7, 0xff
00a9ea03 0f59e0               mulps xmm4, xmm0
00a9ea06 0f59e9               mulps xmm5, xmm1
00a9ea09 0f59f2               mulps xmm6, xmm2
00a9ea0c 0f59fb               mulps xmm7, xmm3
00a9ea0f 0f58e5               addps xmm4, xmm5
00a9ea12 0f58f7               addps xmm6, xmm7
00a9ea15 0f58e6               addps xmm4, xmm6
00a9ea18 0f2927               movaps xmmword ptr [edi], xmm4
00a9ea1b 0f286010             movaps xmm4, xmmword ptr [eax + 0x10]
00a9ea1f 0f28ec               movaps xmm5, xmm4
00a9ea22 0f28f4               movaps xmm6, xmm4
00a9ea25 0f28fc               movaps xmm7, xmm4
00a9ea28 0fc6e400             shufps xmm4, xmm4, 0
00a9ea2c 0fc6ed55             shufps xmm5, xmm5, 0x55
00a9ea30 0fc6f6aa             shufps xmm6, xmm6, 0xaa
00a9ea34 0fc6ffff             shufps xmm7, xmm7, 0xff
00a9ea38 0f59e0               mulps xmm4, xmm0
00a9ea3b 0f59e9               mulps xmm5, xmm1
00a9ea3e 0f59f2               mulps xmm6, xmm2
00a9ea41 0f59fb               mulps xmm7, xmm3
00a9ea44 0f58e5               addps xmm4, xmm5
00a9ea47 0f58f7               addps xmm6, xmm7
00a9ea4a 0f58e6               addps xmm4, xmm6
00a9ea4d 0f296710             movaps xmmword ptr [edi + 0x10], xmm4
00a9ea51 0f286020             movaps xmm4, xmmword ptr [eax + 0x20]
00a9ea55 0f28ec               movaps xmm5, xmm4
00a9ea58 0f28f4               movaps xmm6, xmm4
00a9ea5b 0f28fc               movaps xmm7, xmm4
00a9ea5e 0fc6e400             shufps xmm4, xmm4, 0
00a9ea62 0fc6ed55             shufps xmm5, xmm5, 0x55
00a9ea66 0fc6f6aa             shufps xmm6, xmm6, 0xaa
00a9ea6a 0fc6ffff             shufps xmm7, xmm7, 0xff
00a9ea6e 0f59e0               mulps xmm4, xmm0
00a9ea71 0f59e9               mulps xmm5, xmm1
00a9ea74 0f59f2               mulps xmm6, xmm2
00a9ea77 0f59fb               mulps xmm7, xmm3
00a9ea7a 0f58e5               addps xmm4, xmm5
00a9ea7d 0f58f7               addps xmm6, xmm7
00a9ea80 0f58e6               addps xmm4, xmm6
00a9ea83 0f296720             movaps xmmword ptr [edi + 0x20], xmm4
00a9ea87 0f286030             movaps xmm4, xmmword ptr [eax + 0x30]
00a9ea8b 0f28ec               movaps xmm5, xmm4
00a9ea8e 0f28f4               movaps xmm6, xmm4
00a9ea91 0f28fc               movaps xmm7, xmm4
00a9ea94 0fc6e400             shufps xmm4, xmm4, 0
00a9ea98 0fc6ed55             shufps xmm5, xmm5, 0x55
00a9ea9c 0fc6f6aa             shufps xmm6, xmm6, 0xaa
00a9eaa0 0fc6ffff             shufps xmm7, xmm7, 0xff
00a9eaa4 0f59e0               mulps xmm4, xmm0
00a9eaa7 0f59e9               mulps xmm5, xmm1
00a9eaaa 0f59f2               mulps xmm6, xmm2
00a9eaad 0f59fb               mulps xmm7, xmm3
00a9eab0 0f58e5               addps xmm4, xmm5
00a9eab3 0f58f7               addps xmm6, xmm7
00a9eab6 0f58e6               addps xmm4, xmm6
00a9eab9 0f296730             movaps xmmword ptr [edi + 0x30], xmm4
00a9eabd e99d000000           jmp 0xa9eb5f
00a9eac2 8d442458             lea eax, [esp + 0x58]
00a9eac6 8d53fc               lea edx, [ebx - 4]
00a9eac9 bf04000000           mov edi, 4
00a9eace 8bff                 mov edi, edi
00a9ead0 d900                 fld dword ptr [eax]
00a9ead2 83c010               add eax, 0x10
00a9ead5 d84920               fmul dword ptr [ecx + 0x20]
00a9ead8 83c210               add edx, 0x10
00a9eadb 4f                   dec edi
00a9eadc d94130               fld dword ptr [ecx + 0x30]
00a9eadf d848f4               fmul dword ptr [eax - 0xc]
00a9eae2 dec1                 faddp st(1)
00a9eae4 d94110               fld dword ptr [ecx + 0x10]
00a9eae7 d848ec               fmul dword ptr [eax - 0x14]
00a9eaea dec1                 faddp st(1)
00a9eaec d940e8               fld dword ptr [eax - 0x18]
00a9eaef d809                 fmul dword ptr [ecx]
00a9eaf1 dec1                 faddp st(1)
00a9eaf3 d95ae8               fstp dword ptr [edx - 0x18]
00a9eaf6 d940f4               fld dword ptr [eax - 0xc]
00a9eaf9 d84934               fmul dword ptr [ecx + 0x34]
00a9eafc d940ec               fld dword ptr [eax - 0x14]
00a9eaff d84914               fmul dword ptr [ecx + 0x14]
00a9eb02 dec1                 faddp st(1)
00a9eb04 d940f0               fld dword ptr [eax - 0x10]
00a9eb07 d84924               fmul dword ptr [ecx + 0x24]
00a9eb0a dec1                 faddp st(1)
00a9eb0c d940e8               fld dword ptr [eax - 0x18]
00a9eb0f d84904               fmul dword ptr [ecx + 4]
00a9eb12 dec1                 faddp st(1)
00a9eb14 d95aec               fstp dword ptr [edx - 0x14]
00a9eb17 d940f4               fld dword ptr [eax - 0xc]
00a9eb1a d84938               fmul dword ptr [ecx + 0x38]
00a9eb1d d940ec               fld dword ptr [eax - 0x14]
00a9eb20 d84918               fmul dword ptr [ecx + 0x18]
00a9eb23 dec1                 faddp st(1)
00a9eb25 d940f0               fld dword ptr [eax - 0x10]
00a9eb28 d84928               fmul dword ptr [ecx + 0x28]
00a9eb2b dec1                 faddp st(1)
00a9eb2d d94108               fld dword ptr [ecx + 8]
00a9eb30 d848e8               fmul dword ptr [eax - 0x18]
00a9eb33 dec1                 faddp st(1)
00a9eb35 d95af0               fstp dword ptr [edx - 0x10]
00a9eb38 d940f4               fld dword ptr [eax - 0xc]
00a9eb3b d8493c               fmul dword ptr [ecx + 0x3c]
00a9eb3e d940ec               fld dword ptr [eax - 0x14]
00a9eb41 d8491c               fmul dword ptr [ecx + 0x1c]
00a9eb44 dec1                 faddp st(1)
00a9eb46 d9412c               fld dword ptr [ecx + 0x2c]
00a9eb49 d848f0               fmul dword ptr [eax - 0x10]
00a9eb4c dec1                 faddp st(1)
00a9eb4e d940e8               fld dword ptr [eax - 0x18]
00a9eb51 d8490c               fmul dword ptr [ecx + 0xc]
00a9eb54 dec1                 faddp st(1)
00a9eb56 d95af4               fstp dword ptr [edx - 0xc]
00a9eb59 0f8571ffffff         jne 0xa9ead0
00a9eb5f 8b7c241c             mov edi, dword ptr [esp + 0x1c]
00a9eb63 8b442420             mov eax, dword ptr [esp + 0x20]
00a9eb67 83c73c               add edi, 0x3c
00a9eb6a 83c630               add esi, 0x30
00a9eb6d 83c340               add ebx, 0x40
00a9eb70 48                   dec eax
00a9eb71 897c241c             mov dword ptr [esp + 0x1c], edi
00a9eb75 89442420             mov dword ptr [esp + 0x20], eax
00a9eb79 0f8595fbffff         jne 0xa9e714
00a9eb7f 5f                   pop edi
00a9eb80 5e                   pop esi
00a9eb81 5b                   pop ebx
00a9eb82 8be5                 mov esp, ebp
00a9eb84 5d                   pop ebp
00a9eb85 c21000               ret 0x10
