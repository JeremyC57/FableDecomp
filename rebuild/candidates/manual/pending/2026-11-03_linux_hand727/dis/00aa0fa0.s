00aa0fa0 55                   push ebp
00aa0fa1 8bec                 mov ebp, esp
00aa0fa3 83e4f0               and esp, 0xfffffff0
00aa0fa6 81ec14020000         sub esp, 0x214
00aa0fac 53                   push ebx
00aa0fad 56                   push esi
00aa0fae 57                   push edi
00aa0faf 8d44241c             lea eax, [esp + 0x1c]
00aa0fb3 8d7908               lea edi, [ecx + 8]
00aa0fb6 50                   push eax
00aa0fb7 894c2444             mov dword ptr [esp + 0x44], ecx
00aa0fbb 6a01                 push 1
00aa0fbd 8bcf                 mov ecx, edi
00aa0fbf c744242400000000     mov dword ptr [esp + 0x24], 0
00aa0fc7 e865399cff           call 0x464931
00aa0fcc 8b0f                 mov ecx, dword ptr [edi]
00aa0fce 8b4510               mov eax, dword ptr [ebp + 0x10]
00aa0fd1 8b550c               mov edx, dword ptr [ebp + 0xc]
00aa0fd4 8901                 mov dword ptr [ecx], eax
00aa0fd6 6bc03c               imul eax, eax, 0x3c
00aa0fd9 8b8a9c000000         mov ecx, dword ptr [edx + 0x9c]
00aa0fdf 8b740804             mov esi, dword ptr [eax + ecx + 4]
00aa0fe3 85f6                 test esi, esi
00aa0fe5 bb04000000           mov ebx, 4
00aa0fea 7c48                 jl 0xaa1034
00aa0fec 8d642400             lea esp, [esp]
00aa0ff0 8b4704               mov eax, dword ptr [edi + 4]
00aa0ff3 3b4708               cmp eax, dword ptr [edi + 8]
00aa0ff6 8974241c             mov dword ptr [esp + 0x1c], esi
00aa0ffa 740b                 je 0xaa1007
00aa0ffc 85c0                 test eax, eax
00aa0ffe 7402                 je 0xaa1002
00aa1000 8930                 mov dword ptr [eax], esi
00aa1002 015f04               add dword ptr [edi + 4], ebx
00aa1005 eb19                 jmp 0xaa1020
00aa1007 6a01                 push 1
00aa1009 6a01                 push 1
00aa100b 8d942483000000       lea edx, [esp + 0x83]
00aa1012 52                   push edx
00aa1013 8d4c2428             lea ecx, [esp + 0x28]
00aa1017 51                   push ecx
00aa1018 50                   push eax
00aa1019 8bcf                 mov ecx, edi
00aa101b e8ae3a9cff           call 0x464ace
00aa1020 8b550c               mov edx, dword ptr [ebp + 0xc]
00aa1023 6bf63c               imul esi, esi, 0x3c
00aa1026 8b829c000000         mov eax, dword ptr [edx + 0x9c]
00aa102c 8b740604             mov esi, dword ptr [esi + eax + 4]
00aa1030 85f6                 test esi, esi
00aa1032 7dbc                 jge 0xaa0ff0
00aa1034 8b07                 mov eax, dword ptr [edi]
00aa1036 8b5f04               mov ebx, dword ptr [edi + 4]
00aa1039 2bd8                 sub ebx, eax
00aa103b c1fb02               sar ebx, 2
00aa103e 33c0                 xor eax, eax
00aa1040 895c2430             mov dword ptr [esp + 0x30], ebx
00aa1044 8d0c9b               lea ecx, [ebx + ebx*4]
00aa1047 0f8cb6100000         jl 0xaa2103
00aa104d 7f08                 jg 0xaa1057
00aa104f 85c9                 test ecx, ecx
00aa1051 0f82ac100000         jb 0xaa2103
00aa1057 85c0                 test eax, eax
00aa1059 0f8fa4100000         jg 0xaa2103
00aa105f 7c0c                 jl 0xaa106d
00aa1061 81f9ffffff7f         cmp ecx, 0x7fffffff
00aa1067 0f8796100000         ja 0xaa2103
00aa106d 85c9                 test ecx, ecx
00aa106f 7427                 je 0xaa1098
00aa1071 8bc1                 mov eax, ecx
00aa1073 ba30000000           mov edx, 0x30
00aa1078 f7e2                 mul edx
00aa107a 85d2                 test edx, edx
00aa107c 771a                 ja 0xaa1098
00aa107e 83f8ff               cmp eax, -1
00aa1081 7715                 ja 0xaa1098
00aa1083 8d0449               lea eax, [ecx + ecx*2]
00aa1086 8b4c2440             mov ecx, dword ptr [esp + 0x40]
00aa108a c1e004               shl eax, 4
00aa108d 50                   push eax
00aa108e 83c114               add ecx, 0x14
00aa1091 e87ab8fbff           call 0xa5c910
00aa1096 eb02                 jmp 0xaa109a
00aa1098 33c0                 xor eax, eax
00aa109a 8b4d0c               mov ecx, dword ptr [ebp + 0xc]
00aa109d 8b7508               mov esi, dword ptr [ebp + 8]
00aa10a0 8b5604               mov edx, dword ptr [esi + 4]
00aa10a3 57                   push edi
00aa10a4 51                   push ecx
00aa10a5 8b4c2448             mov ecx, dword ptr [esp + 0x48]
00aa10a9 6a00                 push 0
00aa10ab 53                   push ebx
00aa10ac 50                   push eax
00aa10ad 89442448             mov dword ptr [esp + 0x48], eax
00aa10b1 8b4208               mov eax, dword ptr [edx + 8]
00aa10b4 50                   push eax
00aa10b5 e896fbffff           call 0xaa0c50
00aa10ba 33c0                 xor eax, eax
00aa10bc 8944247c             mov dword ptr [esp + 0x7c], eax
00aa10c0 8b4e04               mov ecx, dword ptr [esi + 4]
00aa10c3 85c9                 test ecx, ecx
00aa10c5 89442438             mov dword ptr [esp + 0x38], eax
00aa10c9 0f840e080000         je 0xaa18dd
00aa10cf 8b5110               mov edx, dword ptr [ecx + 0x10]
00aa10d2 8b790c               mov edi, dword ptr [ecx + 0xc]
00aa10d5 2bd7                 sub edx, edi
00aa10d7 b867666666           mov eax, 0x66666667
00aa10dc f7ea                 imul edx
00aa10de c1fa03               sar edx, 3
00aa10e1 8bc2                 mov eax, edx
00aa10e3 c1e81f               shr eax, 0x1f
00aa10e6 03c2                 add eax, edx
00aa10e8 3944247c             cmp dword ptr [esp + 0x7c], eax
00aa10ec 0f83eb070000         jae 0xaa18dd
00aa10f2 8b542438             mov edx, dword ptr [esp + 0x38]
00aa10f6 8b4c2440             mov ecx, dword ptr [esp + 0x40]
00aa10fa 8b4904               mov ecx, dword ptr [ecx + 4]
00aa10fd 8bf7                 mov esi, edi
00aa10ff 8b441608             mov eax, dword ptr [esi + edx + 8]
00aa1103 03f2                 add esi, edx
00aa1105 50                   push eax
00aa1106 e8b531f8ff           call 0xa242c0
00aa110b 8b550c               mov edx, dword ptr [ebp + 0xc]
00aa110e 52                   push edx
00aa110f 8d4c242c             lea ecx, [esp + 0x2c]
00aa1113 51                   push ecx
00aa1114 8bc8                 mov ecx, eax
00aa1116 e8455bf8ff           call 0xa26c60
00aa111b 8b4e0c               mov ecx, dword ptr [esi + 0xc]
00aa111e 8d542474             lea edx, [esp + 0x74]
00aa1122 52                   push edx
00aa1123 8d442428             lea eax, [esp + 0x28]
00aa1127 50                   push eax
00aa1128 51                   push ecx
00aa1129 8b4c2434             mov ecx, dword ptr [esp + 0x34]
00aa112d e81e15fbff           call 0xa52650
00aa1132 8b5610               mov edx, dword ptr [esi + 0x10]
00aa1135 89542418             mov dword ptr [esp + 0x18], edx
00aa1139 8b542428             mov edx, dword ptr [esp + 0x28]
00aa113d 8b4208               mov eax, dword ptr [edx + 8]
00aa1140 8bc8                 mov ecx, eax
00aa1142 c1e902               shr ecx, 2
00aa1145 f6c101               test cl, 1
00aa1148 0f847f030000         je 0xaa14cd
00aa114e d905d8de2201         fld dword ptr [0x122ded8]
00aa1154 33ff                 xor edi, edi
00aa1156 d9442418             fld dword ptr [esp + 0x18]
00aa115a dae9                 fucompp 
00aa115c dfe0                 fnstsw ax
00aa115e f6c444               test ah, 0x44
00aa1161 0f8aa0000000         jp 0xaa1207
00aa1167 85db                 test ebx, ebx
00aa1169 0f8627070000         jbe 0xaa1896
00aa116f 8b742434             mov esi, dword ptr [esp + 0x34]
00aa1173 8b5c2474             mov ebx, dword ptr [esp + 0x74]
00aa1177 83c618               add esi, 0x18
00aa117a 8d9b00000000         lea ebx, [ebx]
00aa1180 8b442440             mov eax, dword ptr [esp + 0x40]
00aa1184 83c008               add eax, 8
00aa1187 8b00                 mov eax, dword ptr [eax]
00aa1189 8b0cb8               mov ecx, dword ptr [eax + edi*4]
00aa118c 8b425c               mov eax, dword ptr [edx + 0x5c]
00aa118f 8d04c8               lea eax, [eax + ecx*8]
00aa1192 8b08                 mov ecx, dword ptr [eax]
00aa1194 8a410a               mov al, byte ptr [ecx + 0xa]
00aa1197 84c0                 test al, al
00aa1199 7859                 js 0xaa11f4
00aa119b 8b542424             mov edx, dword ptr [esp + 0x24]
00aa119f 53                   push ebx
00aa11a0 52                   push edx
00aa11a1 8d8424f8010000       lea eax, [esp + 0x1f8]
00aa11a8 50                   push eax
00aa11a9 e842b0faff           call 0xa4c1f0
00aa11ae 8d8c24f0010000       lea ecx, [esp + 0x1f0]
00aa11b5 51                   push ecx
00aa11b6 8d4ee8               lea ecx, [esi - 0x18]
00aa11b9 e8527afeff           call 0xa88c10
00aa11be d9842400020000       fld dword ptr [esp + 0x200]
00aa11c5 d846f8               fadd dword ptr [esi - 8]
00aa11c8 8b542428             mov edx, dword ptr [esp + 0x28]
00aa11cc d95ef8               fstp dword ptr [esi - 8]
00aa11cf d9842404020000       fld dword ptr [esp + 0x204]
00aa11d6 d846fc               fadd dword ptr [esi - 4]
00aa11d9 d95efc               fstp dword ptr [esi - 4]
00aa11dc d9842408020000       fld dword ptr [esp + 0x208]
00aa11e3 d806                 fadd dword ptr [esi]
00aa11e5 d91e                 fstp dword ptr [esi]
00aa11e7 d984240c020000       fld dword ptr [esp + 0x20c]
00aa11ee d84604               fadd dword ptr [esi + 4]
00aa11f1 d95e04               fstp dword ptr [esi + 4]
00aa11f4 8b442430             mov eax, dword ptr [esp + 0x30]
00aa11f8 47                   inc edi
00aa11f9 83c630               add esi, 0x30
00aa11fc 3bf8                 cmp edi, eax
00aa11fe 7280                 jb 0xaa1180
00aa1200 8bd8                 mov ebx, eax
00aa1202 e98f060000           jmp 0xaa1896
00aa1207 85db                 test ebx, ebx
00aa1209 0f8687060000         jbe 0xaa1896
00aa120f 8b742434             mov esi, dword ptr [esp + 0x34]
00aa1213 83c618               add esi, 0x18
00aa1216 8b442440             mov eax, dword ptr [esp + 0x40]
00aa121a 83c008               add eax, 8
00aa121d 8b00                 mov eax, dword ptr [eax]
00aa121f 8b0cb8               mov ecx, dword ptr [eax + edi*4]
00aa1222 8b425c               mov eax, dword ptr [edx + 0x5c]
00aa1225 8d04c8               lea eax, [eax + ecx*8]
00aa1228 8b08                 mov ecx, dword ptr [eax]
00aa122a 8a410a               mov al, byte ptr [ecx + 0xa]
00aa122d 84c0                 test al, al
00aa122f 0f8887020000         js 0xaa14bc
00aa1235 8b542474             mov edx, dword ptr [esp + 0x74]
00aa1239 8b442424             mov eax, dword ptr [esp + 0x24]
00aa123d 52                   push edx
00aa123e 50                   push eax
00aa123f 8d942458010000       lea edx, [esp + 0x158]
00aa1246 52                   push edx
00aa1247 e8a4affaff           call 0xa4c1f0
00aa124c d984245c010000       fld dword ptr [esp + 0x15c]
00aa1253 d81ddcde2201         fcomp dword ptr [0x122dedc]
00aa1259 8b84245c010000       mov eax, dword ptr [esp + 0x15c]
00aa1260 89442470             mov dword ptr [esp + 0x70], eax
00aa1264 c78424840000000000803f mov dword ptr [esp + 0x84], 0x3f800000
00aa126f dfe0                 fnstsw ax
00aa1271 f6c405               test ah, 5
00aa1274 7a18                 jp 0xaa128e
00aa1276 d984245c010000       fld dword ptr [esp + 0x15c]
00aa127d c7842484000000000080bf mov dword ptr [esp + 0x84], 0xbf800000
00aa1288 d9e0                 fchs 
00aa128a d95c2470             fstp dword ptr [esp + 0x70]
00aa128e d905d8de2201         fld dword ptr [0x122ded8]
00aa1294 d8642470             fsub dword ptr [esp + 0x70]
00aa1298 d81d5ca12501         fcomp dword ptr [0x125a15c]
00aa129e dfe0                 fnstsw ax
00aa12a0 f6c441               test ah, 0x41
00aa12a3 0f854f010000         jne 0xaa13f8
00aa12a9 d9442470             fld dword ptr [esp + 0x70]
00aa12ad d805d8de2201         fadd dword ptr [0x122ded8]
00aa12b3 d80d1ccd2901         fmul dword ptr [0x129cd1c]
00aa12b9 d9542410             fst dword ptr [esp + 0x10]
00aa12bd d8259cf52201         fsub dword ptr [0x122f59c]
00aa12c3 d95c2414             fstp dword ptr [esp + 0x14]
00aa12c7 d9442414             fld dword ptr [esp + 0x14]
00aa12cb db9c2480000000       fistp dword ptr [esp + 0x80]
00aa12d2 db842480000000       fild dword ptr [esp + 0x80]
00aa12d9 8b842480000000       mov eax, dword ptr [esp + 0x80]
00aa12e0 d86c2410             fsubr dword ptr [esp + 0x10]
00aa12e4 d905d8de2201         fld dword ptr [0x122ded8]
00aa12ea d8e1                 fsub st(1)
00aa12ec d80c8530b53c01       fmul dword ptr [eax*4 + 0x13cb530]
00aa12f3 d9c9                 fxch st(1)
00aa12f5 d80c8534b53c01       fmul dword ptr [eax*4 + 0x13cb534]
00aa12fc dec1                 faddp st(1)
00aa12fe d80d087f2301         fmul dword ptr [0x1237f08]
00aa1304 d95c2444             fstp dword ptr [esp + 0x44]
00aa1308 d9442470             fld dword ptr [esp + 0x70]
00aa130c d84c2470             fmul dword ptr [esp + 0x70]
00aa1310 d82dd8de2201         fsubr dword ptr [0x122ded8]
00aa1316 d9fa                 fsqrt 
00aa1318 d95c2420             fstp dword ptr [esp + 0x20]
00aa131c d905d8de2201         fld dword ptr [0x122ded8]
00aa1322 d8642418             fsub dword ptr [esp + 0x18]
00aa1326 d84c2444             fmul dword ptr [esp + 0x44]
00aa132a d80d10002301         fmul dword ptr [0x1230010]
00aa1330 d9542410             fst dword ptr [esp + 0x10]
00aa1334 d8259cf52201         fsub dword ptr [0x122f59c]
00aa133a d95c2414             fstp dword ptr [esp + 0x14]
00aa133e d9442414             fld dword ptr [esp + 0x14]
00aa1342 db9c2488000000       fistp dword ptr [esp + 0x88]
00aa1349 db842488000000       fild dword ptr [esp + 0x88]
00aa1350 8b842488000000       mov eax, dword ptr [esp + 0x88]
00aa1357 0500ffffff           add eax, 0xffffff00
00aa135c 25ff030000           and eax, 0x3ff
00aa1361 d86c2410             fsubr dword ptr [esp + 0x10]
00aa1365 d905d8de2201         fld dword ptr [0x122ded8]
00aa136b d8742420             fdiv dword ptr [esp + 0x20]
00aa136f d95c2410             fstp dword ptr [esp + 0x10]
00aa1373 d905d8de2201         fld dword ptr [0x122ded8]
00aa1379 d8e1                 fsub st(1)
00aa137b d80c8550d53c01       fmul dword ptr [eax*4 + 0x13cd550]
00aa1382 d9c9                 fxch st(1)
00aa1384 d80c8554d53c01       fmul dword ptr [eax*4 + 0x13cd554]
00aa138b dec1                 faddp st(1)
00aa138d d84c2410             fmul dword ptr [esp + 0x10]
00aa1391 d95c2420             fstp dword ptr [esp + 0x20]
00aa1395 d9442444             fld dword ptr [esp + 0x44]
00aa1399 d84c2418             fmul dword ptr [esp + 0x18]
00aa139d d80d10002301         fmul dword ptr [0x1230010]
00aa13a3 d9542444             fst dword ptr [esp + 0x44]
00aa13a7 d8259cf52201         fsub dword ptr [0x122f59c]
00aa13ad d95c2414             fstp dword ptr [esp + 0x14]
00aa13b1 d9442414             fld dword ptr [esp + 0x14]
00aa13b5 db9c248c000000       fistp dword ptr [esp + 0x8c]
00aa13bc db84248c000000       fild dword ptr [esp + 0x8c]
00aa13c3 8b84248c000000       mov eax, dword ptr [esp + 0x8c]
00aa13ca 0500ffffff           add eax, 0xffffff00
00aa13cf 25ff030000           and eax, 0x3ff
00aa13d4 d86c2444             fsubr dword ptr [esp + 0x44]
00aa13d8 d905d8de2201         fld dword ptr [0x122ded8]
00aa13de d8e1                 fsub st(1)
00aa13e0 d80c8550d53c01       fmul dword ptr [eax*4 + 0x13cd550]
00aa13e7 d9c9                 fxch st(1)
00aa13e9 d80c8554d53c01       fmul dword ptr [eax*4 + 0x13cd554]
00aa13f0 dec1                 faddp st(1)
00aa13f2 d84c2410             fmul dword ptr [esp + 0x10]
00aa13f6 eb12                 jmp 0xaa140a
00aa13f8 d905d8de2201         fld dword ptr [0x122ded8]
00aa13fe d8642418             fsub dword ptr [esp + 0x18]
00aa1402 d95c2420             fstp dword ptr [esp + 0x20]
00aa1406 d9442418             fld dword ptr [esp + 0x18]
00aa140a d88c2484000000       fmul dword ptr [esp + 0x84]
00aa1411 8d4c2450             lea ecx, [esp + 0x50]
00aa1415 d9842450010000       fld dword ptr [esp + 0x150]
00aa141c 51                   push ecx
00aa141d 8d4ee8               lea ecx, [esi - 0x18]
00aa1420 d8c9                 fmul st(1)
00aa1422 d95c2454             fstp dword ptr [esp + 0x54]
00aa1426 d9842458010000       fld dword ptr [esp + 0x158]
00aa142d d8c9                 fmul st(1)
00aa142f d95c2458             fstp dword ptr [esp + 0x58]
00aa1433 d984245c010000       fld dword ptr [esp + 0x15c]
00aa143a d8c9                 fmul st(1)
00aa143c d95c245c             fstp dword ptr [esp + 0x5c]
00aa1440 d9842460010000       fld dword ptr [esp + 0x160]
00aa1447 d8c9                 fmul st(1)
00aa1449 d8442424             fadd dword ptr [esp + 0x24]
00aa144d d95c2460             fstp dword ptr [esp + 0x60]
00aa1451 ddd8                 fstp st(0)
00aa1453 e8b877feff           call 0xa88c10
00aa1458 d9842460010000       fld dword ptr [esp + 0x160]
00aa145f 8b542428             mov edx, dword ptr [esp + 0x28]
00aa1463 d84c2418             fmul dword ptr [esp + 0x18]
00aa1467 d9842464010000       fld dword ptr [esp + 0x164]
00aa146e d84c2418             fmul dword ptr [esp + 0x18]
00aa1472 d9842468010000       fld dword ptr [esp + 0x168]
00aa1479 d84c2418             fmul dword ptr [esp + 0x18]
00aa147d d99c2488010000       fstp dword ptr [esp + 0x188]
00aa1484 d984246c010000       fld dword ptr [esp + 0x16c]
00aa148b d84c2418             fmul dword ptr [esp + 0x18]
00aa148f d99c248c010000       fstp dword ptr [esp + 0x18c]
00aa1496 d9c9                 fxch st(1)
00aa1498 d846f8               fadd dword ptr [esi - 8]
00aa149b d95ef8               fstp dword ptr [esi - 8]
00aa149e d846fc               fadd dword ptr [esi - 4]
00aa14a1 d95efc               fstp dword ptr [esi - 4]
00aa14a4 d9842488010000       fld dword ptr [esp + 0x188]
00aa14ab d806                 fadd dword ptr [esi]
00aa14ad d91e                 fstp dword ptr [esi]
00aa14af d984248c010000       fld dword ptr [esp + 0x18c]
00aa14b6 d84604               fadd dword ptr [esi + 4]
00aa14b9 d95e04               fstp dword ptr [esi + 4]
00aa14bc 47                   inc edi
00aa14bd 83c630               add esi, 0x30
00aa14c0 3bfb                 cmp edi, ebx
00aa14c2 0f824efdffff         jb 0xaa1216
00aa14c8 e9c9030000           jmp 0xaa1896
00aa14cd d1e8                 shr eax, 1
00aa14cf a801                 test al, 1
00aa14d1 0f84bf030000         je 0xaa1896
00aa14d7 d905d8de2201         fld dword ptr [0x122ded8]
00aa14dd d9442418             fld dword ptr [esp + 0x18]
00aa14e1 dae9                 fucompp 
00aa14e3 dfe0                 fnstsw ax
00aa14e5 f6c444               test ah, 0x44
00aa14e8 7a58                 jp 0xaa1542
00aa14ea 33f6                 xor esi, esi
00aa14ec 85db                 test ebx, ebx
00aa14ee 0f86a2030000         jbe 0xaa1896
00aa14f4 8b7c2434             mov edi, dword ptr [esp + 0x34]
00aa14f8 8b5c2474             mov ebx, dword ptr [esp + 0x74]
00aa14fc 8d642400             lea esp, [esp]
00aa1500 8b442440             mov eax, dword ptr [esp + 0x40]
00aa1504 83c008               add eax, 8
00aa1507 8b00                 mov eax, dword ptr [eax]
00aa1509 8b0cb0               mov ecx, dword ptr [eax + esi*4]
00aa150c 8b4268               mov eax, dword ptr [edx + 0x68]
00aa150f 85c0                 test eax, eax
00aa1511 7406                 je 0xaa1519
00aa1513 803c0800             cmp byte ptr [eax + ecx], 0
00aa1517 7416                 je 0xaa152f
00aa1519 8b442424             mov eax, dword ptr [esp + 0x24]
00aa151d 8b525c               mov edx, dword ptr [edx + 0x5c]
00aa1520 8b0cca               mov ecx, dword ptr [edx + ecx*8]
00aa1523 53                   push ebx
00aa1524 50                   push eax
00aa1525 57                   push edi
00aa1526 e8c5acfaff           call 0xa4c1f0
00aa152b 8b542428             mov edx, dword ptr [esp + 0x28]
00aa152f 8b442430             mov eax, dword ptr [esp + 0x30]
00aa1533 46                   inc esi
00aa1534 83c730               add edi, 0x30
00aa1537 3bf0                 cmp esi, eax
00aa1539 72c5                 jb 0xaa1500
00aa153b 8bd8                 mov ebx, eax
00aa153d e954030000           jmp 0xaa1896
00aa1542 33ff                 xor edi, edi
00aa1544 85db                 test ebx, ebx
00aa1546 0f864a030000         jbe 0xaa1896
00aa154c 8b742434             mov esi, dword ptr [esp + 0x34]
00aa1550 83c618               add esi, 0x18
00aa1553 8b442440             mov eax, dword ptr [esp + 0x40]
00aa1557 8b4a68               mov ecx, dword ptr [edx + 0x68]
00aa155a 83c008               add eax, 8
00aa155d 85c9                 test ecx, ecx
00aa155f 8b00                 mov eax, dword ptr [eax]
00aa1561 8b04b8               mov eax, dword ptr [eax + edi*4]
00aa1564 740a                 je 0xaa1570
00aa1566 803c0100             cmp byte ptr [ecx + eax], 0
00aa156a 0f841a030000         je 0xaa188a
00aa1570 8b4c2474             mov ecx, dword ptr [esp + 0x74]
00aa1574 8b525c               mov edx, dword ptr [edx + 0x5c]
00aa1577 51                   push ecx
00aa1578 8b4c2428             mov ecx, dword ptr [esp + 0x28]
00aa157c 51                   push ecx
00aa157d 8d8c2428010000       lea ecx, [esp + 0x128]
00aa1584 51                   push ecx
00aa1585 8b0cc2               mov ecx, dword ptr [edx + eax*8]
00aa1588 e863acfaff           call 0xa4c1f0
00aa158d d9842424010000       fld dword ptr [esp + 0x124]
00aa1594 d84eec               fmul dword ptr [esi - 0x14]
00aa1597 c744241c0000803f     mov dword ptr [esp + 0x1c], 0x3f800000
00aa159f d9842428010000       fld dword ptr [esp + 0x128]
00aa15a6 d84ef0               fmul dword ptr [esi - 0x10]
00aa15a9 dec1                 faddp st(1)
00aa15ab d984242c010000       fld dword ptr [esp + 0x12c]
00aa15b2 d84ef4               fmul dword ptr [esi - 0xc]
00aa15b5 dec1                 faddp st(1)
00aa15b7 d9842420010000       fld dword ptr [esp + 0x120]
00aa15be d84ee8               fmul dword ptr [esi - 0x18]
00aa15c1 dec1                 faddp st(1)
00aa15c3 d95c2410             fstp dword ptr [esp + 0x10]
00aa15c7 d9442410             fld dword ptr [esp + 0x10]
00aa15cb d81ddcde2201         fcomp dword ptr [0x122dedc]
00aa15d1 dfe0                 fnstsw ax
00aa15d3 f6c405               test ah, 5
00aa15d6 7a12                 jp 0xaa15ea
00aa15d8 d9442410             fld dword ptr [esp + 0x10]
00aa15dc c744241c000080bf     mov dword ptr [esp + 0x1c], 0xbf800000
00aa15e4 d9e0                 fchs 
00aa15e6 d95c2410             fstp dword ptr [esp + 0x10]
00aa15ea d905d8de2201         fld dword ptr [0x122ded8]
00aa15f0 d8642410             fsub dword ptr [esp + 0x10]
00aa15f4 d81d5ca12501         fcomp dword ptr [0x125a15c]
00aa15fa dfe0                 fnstsw ax
00aa15fc f6c441               test ah, 0x41
00aa15ff 0f8534010000         jne 0xaa1739
00aa1605 d9442410             fld dword ptr [esp + 0x10]
00aa1609 d805d8de2201         fadd dword ptr [0x122ded8]
00aa160f d80d1ccd2901         fmul dword ptr [0x129cd1c]
00aa1615 d9542420             fst dword ptr [esp + 0x20]
00aa1619 d8259cf52201         fsub dword ptr [0x122f59c]
00aa161f d95c2414             fstp dword ptr [esp + 0x14]
00aa1623 d9442414             fld dword ptr [esp + 0x14]
00aa1627 db5c2448             fistp dword ptr [esp + 0x48]
00aa162b db442448             fild dword ptr [esp + 0x48]
00aa162f 8b442448             mov eax, dword ptr [esp + 0x48]
00aa1633 d86c2420             fsubr dword ptr [esp + 0x20]
00aa1637 d905d8de2201         fld dword ptr [0x122ded8]
00aa163d d8e1                 fsub st(1)
00aa163f d80c8530b53c01       fmul dword ptr [eax*4 + 0x13cb530]
00aa1646 d9c9                 fxch st(1)
00aa1648 d80c8534b53c01       fmul dword ptr [eax*4 + 0x13cb534]
00aa164f dec1                 faddp st(1)
00aa1651 d80d087f2301         fmul dword ptr [0x1237f08]
00aa1657 d95c2444             fstp dword ptr [esp + 0x44]
00aa165b d9442410             fld dword ptr [esp + 0x10]
00aa165f d84c2410             fmul dword ptr [esp + 0x10]
00aa1663 d82dd8de2201         fsubr dword ptr [0x122ded8]
00aa1669 d9fa                 fsqrt 
00aa166b d95c2410             fstp dword ptr [esp + 0x10]
00aa166f d905d8de2201         fld dword ptr [0x122ded8]
00aa1675 d8642418             fsub dword ptr [esp + 0x18]
00aa1679 d84c2444             fmul dword ptr [esp + 0x44]
00aa167d d80d10002301         fmul dword ptr [0x1230010]
00aa1683 d9542420             fst dword ptr [esp + 0x20]
00aa1687 d8259cf52201         fsub dword ptr [0x122f59c]
00aa168d d95c2414             fstp dword ptr [esp + 0x14]
00aa1691 d9442414             fld dword ptr [esp + 0x14]
00aa1695 db5c244c             fistp dword ptr [esp + 0x4c]
00aa1699 db44244c             fild dword ptr [esp + 0x4c]
00aa169d 8b44244c             mov eax, dword ptr [esp + 0x4c]
00aa16a1 0500ffffff           add eax, 0xffffff00
00aa16a6 25ff030000           and eax, 0x3ff
00aa16ab d86c2420             fsubr dword ptr [esp + 0x20]
00aa16af d905d8de2201         fld dword ptr [0x122ded8]
00aa16b5 d8742410             fdiv dword ptr [esp + 0x10]
00aa16b9 d95c2414             fstp dword ptr [esp + 0x14]
00aa16bd d905d8de2201         fld dword ptr [0x122ded8]
00aa16c3 d8e1                 fsub st(1)
00aa16c5 d80c8550d53c01       fmul dword ptr [eax*4 + 0x13cd550]
00aa16cc d9c9                 fxch st(1)
00aa16ce d80c8554d53c01       fmul dword ptr [eax*4 + 0x13cd554]
00aa16d5 dec1                 faddp st(1)
00aa16d7 d84c2414             fmul dword ptr [esp + 0x14]
00aa16db d95c2410             fstp dword ptr [esp + 0x10]
00aa16df d9442444             fld dword ptr [esp + 0x44]
00aa16e3 d84c2418             fmul dword ptr [esp + 0x18]
00aa16e7 d80d10002301         fmul dword ptr [0x1230010]
00aa16ed d9542420             fst dword ptr [esp + 0x20]
00aa16f1 d8259cf52201         fsub dword ptr [0x122f59c]
00aa16f7 d95c2444             fstp dword ptr [esp + 0x44]
00aa16fb d9442444             fld dword ptr [esp + 0x44]
00aa16ff db5c243c             fistp dword ptr [esp + 0x3c]
00aa1703 db44243c             fild dword ptr [esp + 0x3c]
00aa1707 8b44243c             mov eax, dword ptr [esp + 0x3c]
00aa170b 0500ffffff           add eax, 0xffffff00
00aa1710 25ff030000           and eax, 0x3ff
00aa1715 d86c2420             fsubr dword ptr [esp + 0x20]
00aa1719 d905d8de2201         fld dword ptr [0x122ded8]
00aa171f d8e1                 fsub st(1)
00aa1721 d80c8550d53c01       fmul dword ptr [eax*4 + 0x13cd550]
00aa1728 d9c9                 fxch st(1)
00aa172a d80c8554d53c01       fmul dword ptr [eax*4 + 0x13cd554]
00aa1731 dec1                 faddp st(1)
00aa1733 d84c2414             fmul dword ptr [esp + 0x14]
00aa1737 eb12                 jmp 0xaa174b
00aa1739 d905d8de2201         fld dword ptr [0x122ded8]
00aa173f d8642418             fsub dword ptr [esp + 0x18]
00aa1743 d95c2410             fstp dword ptr [esp + 0x10]
00aa1747 d9442418             fld dword ptr [esp + 0x18]
00aa174b d84c241c             fmul dword ptr [esp + 0x1c]
00aa174f d9442410             fld dword ptr [esp + 0x10]
00aa1753 d84ee8               fmul dword ptr [esi - 0x18]
00aa1756 d9842420010000       fld dword ptr [esp + 0x120]
00aa175d d8ca                 fmul st(2)
00aa175f dec1                 faddp st(1)
00aa1761 d95c2460             fstp dword ptr [esp + 0x60]
00aa1765 8b442460             mov eax, dword ptr [esp + 0x60]
00aa1769 d9842424010000       fld dword ptr [esp + 0x124]
00aa1770 d8c9                 fmul st(1)
00aa1772 d9442410             fld dword ptr [esp + 0x10]
00aa1776 d84eec               fmul dword ptr [esi - 0x14]
00aa1779 dec1                 faddp st(1)
00aa177b d95c2464             fstp dword ptr [esp + 0x64]
00aa177f 8b4c2464             mov ecx, dword ptr [esp + 0x64]
00aa1783 d9842428010000       fld dword ptr [esp + 0x128]
00aa178a d8c9                 fmul st(1)
00aa178c d9442410             fld dword ptr [esp + 0x10]
00aa1790 d84ef0               fmul dword ptr [esi - 0x10]
00aa1793 dec1                 faddp st(1)
00aa1795 d95c2468             fstp dword ptr [esp + 0x68]
00aa1799 8b542468             mov edx, dword ptr [esp + 0x68]
00aa179d d984242c010000       fld dword ptr [esp + 0x12c]
00aa17a4 d8c9                 fmul st(1)
00aa17a6 d9442410             fld dword ptr [esp + 0x10]
00aa17aa d84ef4               fmul dword ptr [esi - 0xc]
00aa17ad 8946e8               mov dword ptr [esi - 0x18], eax
00aa17b0 894eec               mov dword ptr [esi - 0x14], ecx
00aa17b3 8956f0               mov dword ptr [esi - 0x10], edx
00aa17b6 dec1                 faddp st(1)
00aa17b8 d95c246c             fstp dword ptr [esp + 0x6c]
00aa17bc 8b44246c             mov eax, dword ptr [esp + 0x6c]
00aa17c0 8946f4               mov dword ptr [esi - 0xc], eax
00aa17c3 ddd8                 fstp st(0)
00aa17c5 d9842430010000       fld dword ptr [esp + 0x130]
00aa17cc d866f8               fsub dword ptr [esi - 8]
00aa17cf d9842434010000       fld dword ptr [esp + 0x134]
00aa17d6 d866fc               fsub dword ptr [esi - 4]
00aa17d9 d9842438010000       fld dword ptr [esp + 0x138]
00aa17e0 d826                 fsub dword ptr [esi]
00aa17e2 d99c2498010000       fstp dword ptr [esp + 0x198]
00aa17e9 d984243c010000       fld dword ptr [esp + 0x13c]
00aa17f0 d86604               fsub dword ptr [esi + 4]
00aa17f3 d99c249c010000       fstp dword ptr [esp + 0x19c]
00aa17fa d9c9                 fxch st(1)
00aa17fc d84c2418             fmul dword ptr [esp + 0x18]
00aa1800 d99c24a0010000       fstp dword ptr [esp + 0x1a0]
00aa1807 d84c2418             fmul dword ptr [esp + 0x18]
00aa180b d9842498010000       fld dword ptr [esp + 0x198]
00aa1812 d84c2418             fmul dword ptr [esp + 0x18]
00aa1816 d984249c010000       fld dword ptr [esp + 0x19c]
00aa181d d84c2418             fmul dword ptr [esp + 0x18]
00aa1821 d99c24ac010000       fstp dword ptr [esp + 0x1ac]
00aa1828 d98424a0010000       fld dword ptr [esp + 0x1a0]
00aa182f d846f8               fadd dword ptr [esi - 8]
00aa1832 d99c24d0000000       fstp dword ptr [esp + 0xd0]
00aa1839 8b8c24d0000000       mov ecx, dword ptr [esp + 0xd0]
00aa1840 d9c9                 fxch st(1)
00aa1842 d846fc               fadd dword ptr [esi - 4]
00aa1845 d99c24d4000000       fstp dword ptr [esp + 0xd4]
00aa184c 8b9424d4000000       mov edx, dword ptr [esp + 0xd4]
00aa1853 d806                 fadd dword ptr [esi]
00aa1855 d99c24d8000000       fstp dword ptr [esp + 0xd8]
00aa185c 8b8424d8000000       mov eax, dword ptr [esp + 0xd8]
00aa1863 d98424ac010000       fld dword ptr [esp + 0x1ac]
00aa186a d84604               fadd dword ptr [esi + 4]
00aa186d 894ef8               mov dword ptr [esi - 8], ecx
00aa1870 8956fc               mov dword ptr [esi - 4], edx
00aa1873 8b542428             mov edx, dword ptr [esp + 0x28]
00aa1877 d99c24dc000000       fstp dword ptr [esp + 0xdc]
00aa187e 8b8c24dc000000       mov ecx, dword ptr [esp + 0xdc]
00aa1885 8906                 mov dword ptr [esi], eax
00aa1887 894e04               mov dword ptr [esi + 4], ecx
00aa188a 47                   inc edi
00aa188b 83c630               add esi, 0x30
00aa188e 3bfb                 cmp edi, ebx
00aa1890 0f82bdfcffff         jb 0xaa1553
00aa1896 8b44242c             mov eax, dword ptr [esp + 0x2c]
00aa189a 33f6                 xor esi, esi
00aa189c 3bc6                 cmp eax, esi
00aa189e 741d                 je 0xaa18bd
00aa18a0 ff08                 dec dword ptr [eax]
00aa18a2 8b44242c             mov eax, dword ptr [esp + 0x2c]
00aa18a6 3930                 cmp dword ptr [eax], esi
00aa18a8 7513                 jne 0xaa18bd
00aa18aa 8b4808               mov ecx, dword ptr [eax + 8]
00aa18ad ff5004               call dword ptr [eax + 4]
00aa18b0 8b54242c             mov edx, dword ptr [esp + 0x2c]
00aa18b4 52                   push edx
00aa18b5 e802d11500           call 0xbfe9bc
00aa18ba 83c404               add esp, 4
00aa18bd 8b4c247c             mov ecx, dword ptr [esp + 0x7c]
00aa18c1 8b442438             mov eax, dword ptr [esp + 0x38]
00aa18c5 41                   inc ecx
00aa18c6 89742428             mov dword ptr [esp + 0x28], esi
00aa18ca 8974242c             mov dword ptr [esp + 0x2c], esi
00aa18ce 8b7508               mov esi, dword ptr [ebp + 8]
00aa18d1 894c247c             mov dword ptr [esp + 0x7c], ecx
00aa18d5 83c014               add eax, 0x14
00aa18d8 e9e3f7ffff           jmp 0xaa10c0
00aa18dd 8b4604               mov eax, dword ptr [esi + 4]
00aa18e0 85c0                 test eax, eax
00aa18e2 0f8479020000         je 0xaa1b61
00aa18e8 8b4818               mov ecx, dword ptr [eax + 0x18]
00aa18eb 85c9                 test ecx, ecx
00aa18ed 0f846e020000         je 0xaa1b61
00aa18f3 85db                 test ebx, ebx
00aa18f5 742d                 je 0xaa1924
00aa18f7 8bc3                 mov eax, ebx
00aa18f9 b90c000000           mov ecx, 0xc
00aa18fe f7e1                 mul ecx
00aa1900 85d2                 test edx, edx
00aa1902 7720                 ja 0xaa1924
00aa1904 83f8ff               cmp eax, -1
00aa1907 771b                 ja 0xaa1924
00aa1909 8b442430             mov eax, dword ptr [esp + 0x30]
00aa190d 8b4c2440             mov ecx, dword ptr [esp + 0x40]
00aa1911 8d1440               lea edx, [eax + eax*2]
00aa1914 c1e202               shl edx, 2
00aa1917 52                   push edx
00aa1918 83c114               add ecx, 0x14
00aa191b e8f0affbff           call 0xa5c910
00aa1920 8bd8                 mov ebx, eax
00aa1922 eb02                 jmp 0xaa1926
00aa1924 33db                 xor ebx, ebx
00aa1926 8b4d08               mov ecx, dword ptr [ebp + 8]
00aa1929 8b5104               mov edx, dword ptr [ecx + 4]
00aa192c 8b4a18               mov ecx, dword ptr [edx + 0x18]
00aa192f 8d442428             lea eax, [esp + 0x28]
00aa1933 50                   push eax
00aa1934 e82739faff           call 0xa45260
00aa1939 8b442430             mov eax, dword ptr [esp + 0x30]
00aa193d 33f6                 xor esi, esi
00aa193f 85c0                 test eax, eax
00aa1941 7634                 jbe 0xaa1977
00aa1943 8bfb                 mov edi, ebx
00aa1945 8b442440             mov eax, dword ptr [esp + 0x40]
00aa1949 8b550c               mov edx, dword ptr [ebp + 0xc]
00aa194c 83c008               add eax, 8
00aa194f 8b00                 mov eax, dword ptr [eax]
00aa1951 8b0cb0               mov ecx, dword ptr [eax + esi*4]
00aa1954 8b829c000000         mov eax, dword ptr [edx + 0x9c]
00aa195a 6bc93c               imul ecx, ecx, 0x3c
00aa195d 8b0c01               mov ecx, dword ptr [ecx + eax]
00aa1960 51                   push ecx
00aa1961 8b4c242c             mov ecx, dword ptr [esp + 0x2c]
00aa1965 57                   push edi
00aa1966 e86533faff           call 0xa44cd0
00aa196b 8b442430             mov eax, dword ptr [esp + 0x30]
00aa196f 46                   inc esi
00aa1970 83c70c               add edi, 0xc
00aa1973 3bf0                 cmp esi, eax
00aa1975 72ce                 jb 0xaa1945
00aa1977 8b542430             mov edx, dword ptr [esp + 0x30]
00aa197b 8d42ff               lea eax, [edx - 1]
00aa197e 83f804               cmp eax, 4
00aa1981 be01000000           mov esi, 1
00aa1986 0f8c38010000         jl 0xaa1ac4
00aa198c 8b442434             mov eax, dword ptr [esp + 0x34]
00aa1990 83c2fb               add edx, -5
00aa1993 c1ea02               shr edx, 2
00aa1996 83c018               add eax, 0x18
00aa1999 42                   inc edx
00aa199a 8d4b20               lea ecx, [ebx + 0x20]
00aa199d 8d349501000000       lea esi, [edx*4 + 1]
00aa19a4 8b78fc               mov edi, dword ptr [eax - 4]
00aa19a7 d940f8               fld dword ptr [eax - 8]
00aa19aa d849ec               fmul dword ptr [ecx - 0x14]
00aa19ad 897c2464             mov dword ptr [esp + 0x64], edi
00aa19b1 8b38                 mov edi, dword ptr [eax]
00aa19b3 d9442464             fld dword ptr [esp + 0x64]
00aa19b7 d849f0               fmul dword ptr [ecx - 0x10]
00aa19ba 897c2468             mov dword ptr [esp + 0x68], edi
00aa19be d9442468             fld dword ptr [esp + 0x68]
00aa19c2 83c130               add ecx, 0x30
00aa19c5 d849c4               fmul dword ptr [ecx - 0x3c]
00aa19c8 c7400400000000       mov dword ptr [eax + 4], 0
00aa19cf 05c0000000           add eax, 0xc0
00aa19d4 4a                   dec edx
00aa19d5 d95c2458             fstp dword ptr [esp + 0x58]
00aa19d9 8b7c2458             mov edi, dword ptr [esp + 0x58]
00aa19dd 89b840ffffff         mov dword ptr [eax - 0xc0], edi
00aa19e3 d9c9                 fxch st(1)
00aa19e5 d99838ffffff         fstp dword ptr [eax - 0xc8]
00aa19eb d9983cffffff         fstp dword ptr [eax - 0xc4]
00aa19f1 8bb86cffffff         mov edi, dword ptr [eax - 0x94]
00aa19f7 d98068ffffff         fld dword ptr [eax - 0x98]
00aa19fd 897c2464             mov dword ptr [esp + 0x64], edi
00aa1a01 d849c8               fmul dword ptr [ecx - 0x38]
00aa1a04 8bb870ffffff         mov edi, dword ptr [eax - 0x90]
00aa1a0a d9442464             fld dword ptr [esp + 0x64]
00aa1a0e 897c2468             mov dword ptr [esp + 0x68], edi
00aa1a12 d849cc               fmul dword ptr [ecx - 0x34]
00aa1a15 d9442468             fld dword ptr [esp + 0x68]
00aa1a19 d849d0               fmul dword ptr [ecx - 0x30]
00aa1a1c c78074ffffff00000000 mov dword ptr [eax - 0x8c], 0
00aa1a26 d95c2458             fstp dword ptr [esp + 0x58]
00aa1a2a 8b7c2458             mov edi, dword ptr [esp + 0x58]
00aa1a2e 89b870ffffff         mov dword ptr [eax - 0x90], edi
00aa1a34 d9c9                 fxch st(1)
00aa1a36 d99868ffffff         fstp dword ptr [eax - 0x98]
00aa1a3c d9986cffffff         fstp dword ptr [eax - 0x94]
00aa1a42 8b789c               mov edi, dword ptr [eax - 0x64]
00aa1a45 d94098               fld dword ptr [eax - 0x68]
00aa1a48 897c2464             mov dword ptr [esp + 0x64], edi
00aa1a4c d849d4               fmul dword ptr [ecx - 0x2c]
00aa1a4f 8b78a0               mov edi, dword ptr [eax - 0x60]
00aa1a52 d9442464             fld dword ptr [esp + 0x64]
00aa1a56 897c2468             mov dword ptr [esp + 0x68], edi
00aa1a5a d849d8               fmul dword ptr [ecx - 0x28]
00aa1a5d d9442468             fld dword ptr [esp + 0x68]
00aa1a61 d849dc               fmul dword ptr [ecx - 0x24]
00aa1a64 c740a400000000       mov dword ptr [eax - 0x5c], 0
00aa1a6b d95c2458             fstp dword ptr [esp + 0x58]
00aa1a6f 8b7c2458             mov edi, dword ptr [esp + 0x58]
00aa1a73 8978a0               mov dword ptr [eax - 0x60], edi
00aa1a76 d9c9                 fxch st(1)
00aa1a78 d95898               fstp dword ptr [eax - 0x68]
00aa1a7b d9589c               fstp dword ptr [eax - 0x64]
00aa1a7e 8b78cc               mov edi, dword ptr [eax - 0x34]
00aa1a81 d940c8               fld dword ptr [eax - 0x38]
00aa1a84 897c2464             mov dword ptr [esp + 0x64], edi
00aa1a88 d849e0               fmul dword ptr [ecx - 0x20]
00aa1a8b 8b78d0               mov edi, dword ptr [eax - 0x30]
00aa1a8e d9442464             fld dword ptr [esp + 0x64]
00aa1a92 897c2468             mov dword ptr [esp + 0x68], edi
00aa1a96 d849e4               fmul dword ptr [ecx - 0x1c]
00aa1a99 d9442468             fld dword ptr [esp + 0x68]
00aa1a9d d849e8               fmul dword ptr [ecx - 0x18]
00aa1aa0 c740d400000000       mov dword ptr [eax - 0x2c], 0
00aa1aa7 d95c2458             fstp dword ptr [esp + 0x58]
00aa1aab 8b7c2458             mov edi, dword ptr [esp + 0x58]
00aa1aaf d9c9                 fxch st(1)
00aa1ab1 8978d0               mov dword ptr [eax - 0x30], edi
00aa1ab4 d958c8               fstp dword ptr [eax - 0x38]
00aa1ab7 d958cc               fstp dword ptr [eax - 0x34]
00aa1aba 0f85e4feffff         jne 0xaa19a4
00aa1ac0 8b542430             mov edx, dword ptr [esp + 0x30]
00aa1ac4 3bf2                 cmp esi, edx
00aa1ac6 8b7c2434             mov edi, dword ptr [esp + 0x34]
00aa1aca 735a                 jae 0xaa1b26
00aa1acc 8d1476               lea edx, [esi + esi*2]
00aa1acf c1e204               shl edx, 4
00aa1ad2 8d443ae8             lea eax, [edx + edi - 0x18]
00aa1ad6 8b542430             mov edx, dword ptr [esp + 0x30]
00aa1ada 8d0c76               lea ecx, [esi + esi*2]
00aa1add 8d4c8b08             lea ecx, [ebx + ecx*4 + 8]
00aa1ae1 2bd6                 sub edx, esi
00aa1ae3 8b70fc               mov esi, dword ptr [eax - 4]
00aa1ae6 d940f8               fld dword ptr [eax - 8]
00aa1ae9 d849f8               fmul dword ptr [ecx - 8]
00aa1aec 89742464             mov dword ptr [esp + 0x64], esi
00aa1af0 8b30                 mov esi, dword ptr [eax]
00aa1af2 d9442464             fld dword ptr [esp + 0x64]
00aa1af6 d849fc               fmul dword ptr [ecx - 4]
00aa1af9 89742468             mov dword ptr [esp + 0x68], esi
00aa1afd d9442468             fld dword ptr [esp + 0x68]
00aa1b01 83c030               add eax, 0x30
00aa1b04 d809                 fmul dword ptr [ecx]
00aa1b06 c740d400000000       mov dword ptr [eax - 0x2c], 0
00aa1b0d 83c10c               add ecx, 0xc
00aa1b10 4a                   dec edx
00aa1b11 d95c2458             fstp dword ptr [esp + 0x58]
00aa1b15 8b742458             mov esi, dword ptr [esp + 0x58]
00aa1b19 d9c9                 fxch st(1)
00aa1b1b 8970d0               mov dword ptr [eax - 0x30], esi
00aa1b1e d958c8               fstp dword ptr [eax - 0x38]
00aa1b21 d958cc               fstp dword ptr [eax - 0x34]
00aa1b24 75bd                 jne 0xaa1ae3
00aa1b26 8b0b                 mov ecx, dword ptr [ebx]
00aa1b28 8b4518               mov eax, dword ptr [ebp + 0x18]
00aa1b2b 8908                 mov dword ptr [eax], ecx
00aa1b2d 8b5304               mov edx, dword ptr [ebx + 4]
00aa1b30 895004               mov dword ptr [eax + 4], edx
00aa1b33 8b4b08               mov ecx, dword ptr [ebx + 8]
00aa1b36 894808               mov dword ptr [eax + 8], ecx
00aa1b39 8b44242c             mov eax, dword ptr [esp + 0x2c]
00aa1b3d 85c0                 test eax, eax
00aa1b3f 7434                 je 0xaa1b75
00aa1b41 ff08                 dec dword ptr [eax]
00aa1b43 8b44242c             mov eax, dword ptr [esp + 0x2c]
00aa1b47 833800               cmp dword ptr [eax], 0
00aa1b4a 7529                 jne 0xaa1b75
00aa1b4c 8b4808               mov ecx, dword ptr [eax + 8]
00aa1b4f ff5004               call dword ptr [eax + 4]
00aa1b52 8b54242c             mov edx, dword ptr [esp + 0x2c]
00aa1b56 52                   push edx
00aa1b57 e860ce1500           call 0xbfe9bc
00aa1b5c 83c404               add esp, 4
00aa1b5f eb14                 jmp 0xaa1b75
00aa1b61 8b4518               mov eax, dword ptr [ebp + 0x18]
00aa1b64 8b7c2434             mov edi, dword ptr [esp + 0x34]
00aa1b68 b90000803f           mov ecx, 0x3f800000
00aa1b6d 8908                 mov dword ptr [eax], ecx
00aa1b6f 894804               mov dword ptr [eax + 4], ecx
00aa1b72 894808               mov dword ptr [eax + 8], ecx
00aa1b75 d94710               fld dword ptr [edi + 0x10]
00aa1b78 8b4718               mov eax, dword ptr [edi + 0x18]
00aa1b7b d94714               fld dword ptr [edi + 0x14]
00aa1b7e 89442458             mov dword ptr [esp + 0x58], eax
00aa1b82 d907                 fld dword ptr [edi]
00aa1b84 8db424e0000000       lea esi, [esp + 0xe0]
00aa1b8b d94704               fld dword ptr [edi + 4]
00aa1b8e 8d9c24b0010000       lea ebx, [esp + 0x1b0]
00aa1b95 d94708               fld dword ptr [edi + 8]
00aa1b98 d9470c               fld dword ptr [edi + 0xc]
00aa1b9b d9c0                 fld st(0)
00aa1b9d d8c9                 fmul st(1)
00aa1b9f d9c2                 fld st(2)
00aa1ba1 d8cb                 fmul st(3)
00aa1ba3 dec1                 faddp st(1)
00aa1ba5 d9c3                 fld st(3)
00aa1ba7 d8cc                 fmul st(4)
00aa1ba9 dec1                 faddp st(1)
00aa1bab d9c4                 fld st(4)
00aa1bad d8cd                 fmul st(5)
00aa1baf dec1                 faddp st(1)
00aa1bb1 d95c241c             fstp dword ptr [esp + 0x1c]
00aa1bb5 ddd8                 fstp st(0)
00aa1bb7 ddd8                 fstp st(0)
00aa1bb9 ddd8                 fstp st(0)
00aa1bbb ddd8                 fstp st(0)
00aa1bbd d905dcde2201         fld dword ptr [0x122dedc]
00aa1bc3 d944241c             fld dword ptr [esp + 0x1c]
00aa1bc7 dae9                 fucompp 
00aa1bc9 dfe0                 fnstsw ax
00aa1bcb f6c444               test ah, 0x44
00aa1bce 7b0c                 jnp 0xaa1bdc
00aa1bd0 d905dcdc2201         fld dword ptr [0x122dcdc]
00aa1bd6 d874241c             fdiv dword ptr [esp + 0x1c]
00aa1bda eb06                 jmp 0xaa1be2
00aa1bdc d905d8de2201         fld dword ptr [0x122ded8]
00aa1be2 d9c0                 fld st(0)
00aa1be4 8b442430             mov eax, dword ptr [esp + 0x30]
00aa1be8 83f801               cmp eax, 1
00aa1beb d80f                 fmul dword ptr [edi]
00aa1bed d9c1                 fld st(1)
00aa1bef 8b4c2458             mov ecx, dword ptr [esp + 0x58]
00aa1bf3 d84f04               fmul dword ptr [edi + 4]
00aa1bf6 d95c2438             fstp dword ptr [esp + 0x38]
00aa1bfa d9c9                 fxch st(1)
00aa1bfc d84f08               fmul dword ptr [edi + 8]
00aa1bff d95c2424             fstp dword ptr [esp + 0x24]
00aa1c03 d9c0                 fld st(0)
00aa1c05 d84f0c               fmul dword ptr [edi + 0xc]
00aa1c08 d95c244c             fstp dword ptr [esp + 0x4c]
00aa1c0c d9442438             fld dword ptr [esp + 0x38]
00aa1c10 d84f0c               fmul dword ptr [edi + 0xc]
00aa1c13 d95c2414             fstp dword ptr [esp + 0x14]
00aa1c17 d9442424             fld dword ptr [esp + 0x24]
00aa1c1b d84f0c               fmul dword ptr [edi + 0xc]
00aa1c1e d95c241c             fstp dword ptr [esp + 0x1c]
00aa1c22 d80f                 fmul dword ptr [edi]
00aa1c24 d9442438             fld dword ptr [esp + 0x38]
00aa1c28 d80f                 fmul dword ptr [edi]
00aa1c2a d9442424             fld dword ptr [esp + 0x24]
00aa1c2e d80f                 fmul dword ptr [edi]
00aa1c30 d9442438             fld dword ptr [esp + 0x38]
00aa1c34 d84f04               fmul dword ptr [edi + 4]
00aa1c37 d95c2448             fstp dword ptr [esp + 0x48]
00aa1c3b d9442424             fld dword ptr [esp + 0x24]
00aa1c3f d84f04               fmul dword ptr [edi + 4]
00aa1c42 d95c243c             fstp dword ptr [esp + 0x3c]
00aa1c46 d9442424             fld dword ptr [esp + 0x24]
00aa1c4a d84f08               fmul dword ptr [edi + 8]
00aa1c4d c78424ec00000000000000 mov dword ptr [esp + 0xec], 0
00aa1c58 d9442448             fld dword ptr [esp + 0x48]
00aa1c5c c78424fc00000000000000 mov dword ptr [esp + 0xfc], 0
00aa1c67 c784240c01000000000000 mov dword ptr [esp + 0x10c], 0
00aa1c72 d8c1                 fadd st(1)
00aa1c74 898c2418010000       mov dword ptr [esp + 0x118], ecx
00aa1c7b c784241c0100000000803f mov dword ptr [esp + 0x11c], 0x3f800000
00aa1c86 d82dd8de2201         fsubr dword ptr [0x122ded8]
00aa1c8c d99c24e0000000       fstp dword ptr [esp + 0xe0]
00aa1c93 d9c2                 fld st(2)
00aa1c95 d864241c             fsub dword ptr [esp + 0x1c]
00aa1c99 d99c24e4000000       fstp dword ptr [esp + 0xe4]
00aa1ca0 d9c1                 fld st(1)
00aa1ca2 d8442414             fadd dword ptr [esp + 0x14]
00aa1ca6 d99c24e8000000       fstp dword ptr [esp + 0xe8]
00aa1cad d9ca                 fxch st(2)
00aa1caf d844241c             fadd dword ptr [esp + 0x1c]
00aa1cb3 d99c24f0000000       fstp dword ptr [esp + 0xf0]
00aa1cba d9c9                 fxch st(1)
00aa1cbc d8c2                 fadd st(2)
00aa1cbe d82dd8de2201         fsubr dword ptr [0x122ded8]
00aa1cc4 d99c24f4000000       fstp dword ptr [esp + 0xf4]
00aa1ccb d944243c             fld dword ptr [esp + 0x3c]
00aa1ccf d864244c             fsub dword ptr [esp + 0x4c]
00aa1cd3 d99c24f8000000       fstp dword ptr [esp + 0xf8]
00aa1cda d8642414             fsub dword ptr [esp + 0x14]
00aa1cde d99c2400010000       fstp dword ptr [esp + 0x100]
00aa1ce5 d944243c             fld dword ptr [esp + 0x3c]
00aa1ce9 d844244c             fadd dword ptr [esp + 0x4c]
00aa1ced d99c2404010000       fstp dword ptr [esp + 0x104]
00aa1cf4 d9442448             fld dword ptr [esp + 0x48]
00aa1cf8 d8c1                 fadd st(1)
00aa1cfa d82dd8de2201         fsubr dword ptr [0x122ded8]
00aa1d00 d99c2408010000       fstp dword ptr [esp + 0x108]
00aa1d07 ddd8                 fstp st(0)
00aa1d09 d9c9                 fxch st(1)
00aa1d0b d99c2410010000       fstp dword ptr [esp + 0x110]
00aa1d12 d99c2414010000       fstp dword ptr [esp + 0x114]
00aa1d19 0f868f030000         jbe 0xaa20ae
00aa1d1f 8b4c2434             mov ecx, dword ptr [esp + 0x34]
00aa1d23 8bd0                 mov edx, eax
00aa1d25 83c13c               add ecx, 0x3c
00aa1d28 4a                   dec edx
00aa1d29 89542420             mov dword ptr [esp + 0x20], edx
00aa1d2d 8d4900               lea ecx, [ecx]
00aa1d30 d941f4               fld dword ptr [ecx - 0xc]
00aa1d33 8b4104               mov eax, dword ptr [ecx + 4]
00aa1d36 d941f8               fld dword ptr [ecx - 8]
00aa1d39 89442450             mov dword ptr [esp + 0x50], eax
00aa1d3d d941fc               fld dword ptr [ecx - 4]
00aa1d40 8b4108               mov eax, dword ptr [ecx + 8]
00aa1d43 d901                 fld dword ptr [ecx]
00aa1d45 89442454             mov dword ptr [esp + 0x54], eax
00aa1d49 d9c0                 fld st(0)
00aa1d4b 8b410c               mov eax, dword ptr [ecx + 0xc]
00aa1d4e d8c9                 fmul st(1)
00aa1d50 89442458             mov dword ptr [esp + 0x58], eax
00aa1d54 d9c2                 fld st(2)
00aa1d56 8bd6                 mov edx, esi
00aa1d58 d8cb                 fmul st(3)
00aa1d5a 8bf3                 mov esi, ebx
00aa1d5c 8bda                 mov ebx, edx
00aa1d5e dec1                 faddp st(1)
00aa1d60 d9c3                 fld st(3)
00aa1d62 d8cc                 fmul st(4)
00aa1d64 dec1                 faddp st(1)
00aa1d66 d9c4                 fld st(4)
00aa1d68 d8cd                 fmul st(5)
00aa1d6a dec1                 faddp st(1)
00aa1d6c dddc                 fstp st(4)
00aa1d6e ddd8                 fstp st(0)
00aa1d70 ddd8                 fstp st(0)
00aa1d72 ddd8                 fstp st(0)
00aa1d74 d905dcde2201         fld dword ptr [0x122dedc]
00aa1d7a d9c1                 fld st(1)
00aa1d7c dae9                 fucompp 
00aa1d7e dfe0                 fnstsw ax
00aa1d80 f6c444               test ah, 0x44
00aa1d83 7b08                 jnp 0xaa1d8d
00aa1d85 d83ddcdc2201         fdivr dword ptr [0x122dcdc]
00aa1d8b eb08                 jmp 0xaa1d95
00aa1d8d ddd8                 fstp st(0)
00aa1d8f d905d8de2201         fld dword ptr [0x122ded8]
00aa1d95 d9c0                 fld st(0)
00aa1d97 8b442450             mov eax, dword ptr [esp + 0x50]
00aa1d9b d849f4               fmul dword ptr [ecx - 0xc]
00aa1d9e d9c1                 fld st(1)
00aa1da0 d849f8               fmul dword ptr [ecx - 8]
00aa1da3 d95c2438             fstp dword ptr [esp + 0x38]
00aa1da7 d9c9                 fxch st(1)
00aa1da9 d849fc               fmul dword ptr [ecx - 4]
00aa1dac d95c2424             fstp dword ptr [esp + 0x24]
00aa1db0 d9c0                 fld st(0)
00aa1db2 d809                 fmul dword ptr [ecx]
00aa1db4 d95c244c             fstp dword ptr [esp + 0x4c]
00aa1db8 d9442438             fld dword ptr [esp + 0x38]
00aa1dbc d809                 fmul dword ptr [ecx]
00aa1dbe d95c2414             fstp dword ptr [esp + 0x14]
00aa1dc2 d9442424             fld dword ptr [esp + 0x24]
00aa1dc6 d809                 fmul dword ptr [ecx]
00aa1dc8 d95c241c             fstp dword ptr [esp + 0x1c]
00aa1dcc d849f4               fmul dword ptr [ecx - 0xc]
00aa1dcf d9442438             fld dword ptr [esp + 0x38]
00aa1dd3 d849f4               fmul dword ptr [ecx - 0xc]
00aa1dd6 d9442424             fld dword ptr [esp + 0x24]
00aa1dda d849f4               fmul dword ptr [ecx - 0xc]
00aa1ddd d9442438             fld dword ptr [esp + 0x38]
00aa1de1 d849f8               fmul dword ptr [ecx - 8]
00aa1de4 d95c2448             fstp dword ptr [esp + 0x48]
00aa1de8 d9442424             fld dword ptr [esp + 0x24]
00aa1dec d849f8               fmul dword ptr [ecx - 8]
00aa1def d95c243c             fstp dword ptr [esp + 0x3c]
00aa1df3 d9442424             fld dword ptr [esp + 0x24]
00aa1df7 d849fc               fmul dword ptr [ecx - 4]
00aa1dfa 898424c0000000       mov dword ptr [esp + 0xc0], eax
00aa1e01 d9442448             fld dword ptr [esp + 0x48]
00aa1e05 8b442454             mov eax, dword ptr [esp + 0x54]
00aa1e09 898424c4000000       mov dword ptr [esp + 0xc4], eax
00aa1e10 d8c1                 fadd st(1)
00aa1e12 8b442458             mov eax, dword ptr [esp + 0x58]
00aa1e16 898424c8000000       mov dword ptr [esp + 0xc8], eax
00aa1e1d a080283d01           mov al, byte ptr [0x13d2880]
00aa1e22 84c0                 test al, al
00aa1e24 d82dd8de2201         fsubr dword ptr [0x122ded8]
00aa1e2a c784249c00000000000000 mov dword ptr [esp + 0x9c], 0
00aa1e35 c78424ac00000000000000 mov dword ptr [esp + 0xac], 0
00aa1e40 d99c2490000000       fstp dword ptr [esp + 0x90]
00aa1e47 c78424bc00000000000000 mov dword ptr [esp + 0xbc], 0
00aa1e52 d9c2                 fld st(2)
00aa1e54 c78424cc0000000000803f mov dword ptr [esp + 0xcc], 0x3f800000
00aa1e5f d864241c             fsub dword ptr [esp + 0x1c]
00aa1e63 d99c2494000000       fstp dword ptr [esp + 0x94]
00aa1e6a d9c1                 fld st(1)
00aa1e6c d8442414             fadd dword ptr [esp + 0x14]
00aa1e70 d99c2498000000       fstp dword ptr [esp + 0x98]
00aa1e77 d9ca                 fxch st(2)
00aa1e79 d844241c             fadd dword ptr [esp + 0x1c]
00aa1e7d d99c24a0000000       fstp dword ptr [esp + 0xa0]
00aa1e84 d9c9                 fxch st(1)
00aa1e86 d8c2                 fadd st(2)
00aa1e88 d82dd8de2201         fsubr dword ptr [0x122ded8]
00aa1e8e d99c24a4000000       fstp dword ptr [esp + 0xa4]
00aa1e95 d944243c             fld dword ptr [esp + 0x3c]
00aa1e99 d864244c             fsub dword ptr [esp + 0x4c]
00aa1e9d d99c24a8000000       fstp dword ptr [esp + 0xa8]
00aa1ea4 d8642414             fsub dword ptr [esp + 0x14]
00aa1ea8 d99c24b0000000       fstp dword ptr [esp + 0xb0]
00aa1eaf d944243c             fld dword ptr [esp + 0x3c]
00aa1eb3 d844244c             fadd dword ptr [esp + 0x4c]
00aa1eb7 d99c24b4000000       fstp dword ptr [esp + 0xb4]
00aa1ebe d9442448             fld dword ptr [esp + 0x48]
00aa1ec2 d8c1                 fadd st(1)
00aa1ec4 d82dd8de2201         fsubr dword ptr [0x122ded8]
00aa1eca d99c24b8000000       fstp dword ptr [esp + 0xb8]
00aa1ed1 ddd8                 fstp st(0)
00aa1ed3 0f8409010000         je 0xaa1fe2
00aa1ed9 8954243c             mov dword ptr [esp + 0x3c], edx
00aa1edd 8d942490000000       lea edx, [esp + 0x90]
00aa1ee4 89542414             mov dword ptr [esp + 0x14], edx
00aa1ee8 8974241c             mov dword ptr [esp + 0x1c], esi
00aa1eec 8b44243c             mov eax, dword ptr [esp + 0x3c]
00aa1ef0 8b542414             mov edx, dword ptr [esp + 0x14]
00aa1ef4 8b7c241c             mov edi, dword ptr [esp + 0x1c]
00aa1ef8 0f2802               movaps xmm0, xmmword ptr [edx]
00aa1efb 0f284a10             movaps xmm1, xmmword ptr [edx + 0x10]
00aa1eff 0f285220             movaps xmm2, xmmword ptr [edx + 0x20]
00aa1f03 0f285a30             movaps xmm3, xmmword ptr [edx + 0x30]
00aa1f07 0f2820               movaps xmm4, xmmword ptr [eax]
00aa1f0a 0f28ec               movaps xmm5, xmm4
00aa1f0d 0f28f4               movaps xmm6, xmm4
00aa1f10 0f28fc               movaps xmm7, xmm4
00aa1f13 0fc6e400             shufps xmm4, xmm4, 0
00aa1f17 0fc6ed55             shufps xmm5, xmm5, 0x55
00aa1f1b 0fc6f6aa             shufps xmm6, xmm6, 0xaa
00aa1f1f 0fc6ffff             shufps xmm7, xmm7, 0xff
00aa1f23 0f59e0               mulps xmm4, xmm0
00aa1f26 0f59e9               mulps xmm5, xmm1
00aa1f29 0f59f2               mulps xmm6, xmm2
00aa1f2c 0f59fb               mulps xmm7, xmm3
00aa1f2f 0f58e5               addps xmm4, xmm5
00aa1f32 0f58f7               addps xmm6, xmm7
00aa1f35 0f58e6               addps xmm4, xmm6
00aa1f38 0f2927               movaps xmmword ptr [edi], xmm4
00aa1f3b 0f286010             movaps xmm4, xmmword ptr [eax + 0x10]
00aa1f3f 0f28ec               movaps xmm5, xmm4
00aa1f42 0f28f4               movaps xmm6, xmm4
00aa1f45 0f28fc               movaps xmm7, xmm4
00aa1f48 0fc6e400             shufps xmm4, xmm4, 0
00aa1f4c 0fc6ed55             shufps xmm5, xmm5, 0x55
00aa1f50 0fc6f6aa             shufps xmm6, xmm6, 0xaa
00aa1f54 0fc6ffff             shufps xmm7, xmm7, 0xff
00aa1f58 0f59e0               mulps xmm4, xmm0
00aa1f5b 0f59e9               mulps xmm5, xmm1
00aa1f5e 0f59f2               mulps xmm6, xmm2
00aa1f61 0f59fb               mulps xmm7, xmm3
00aa1f64 0f58e5               addps xmm4, xmm5
00aa1f67 0f58f7               addps xmm6, xmm7
00aa1f6a 0f58e6               addps xmm4, xmm6
00aa1f6d 0f296710             movaps xmmword ptr [edi + 0x10], xmm4
00aa1f71 0f286020             movaps xmm4, xmmword ptr [eax + 0x20]
00aa1f75 0f28ec               movaps xmm5, xmm4
00aa1f78 0f28f4               movaps xmm6, xmm4
00aa1f7b 0f28fc               movaps xmm7, xmm4
00aa1f7e 0fc6e400             shufps xmm4, xmm4, 0
00aa1f82 0fc6ed55             shufps xmm5, xmm5, 0x55
00aa1f86 0fc6f6aa             shufps xmm6, xmm6, 0xaa
00aa1f8a 0fc6ffff             shufps xmm7, xmm7, 0xff
00aa1f8e 0f59e0               mulps xmm4, xmm0
00aa1f91 0f59e9               mulps xmm5, xmm1
00aa1f94 0f59f2               mulps xmm6, xmm2
00aa1f97 0f59fb               mulps xmm7, xmm3
00aa1f9a 0f58e5               addps xmm4, xmm5
00aa1f9d 0f58f7               addps xmm6, xmm7
00aa1fa0 0f58e6               addps xmm4, xmm6
00aa1fa3 0f296720             movaps xmmword ptr [edi + 0x20], xmm4
00aa1fa7 0f286030             movaps xmm4, xmmword ptr [eax + 0x30]
00aa1fab 0f28ec               movaps xmm5, xmm4
00aa1fae 0f28f4               movaps xmm6, xmm4
00aa1fb1 0f28fc               movaps xmm7, xmm4
00aa1fb4 0fc6e400             shufps xmm4, xmm4, 0
00aa1fb8 0fc6ed55             shufps xmm5, xmm5, 0x55
00aa1fbc 0fc6f6aa             shufps xmm6, xmm6, 0xaa
00aa1fc0 0fc6ffff             shufps xmm7, xmm7, 0xff
00aa1fc4 0f59e0               mulps xmm4, xmm0
00aa1fc7 0f59e9               mulps xmm5, xmm1
00aa1fca 0f59f2               mulps xmm6, xmm2
00aa1fcd 0f59fb               mulps xmm7, xmm3
00aa1fd0 0f58e5               addps xmm4, xmm5
00aa1fd3 0f58f7               addps xmm6, xmm7
00aa1fd6 0f58e6               addps xmm4, xmm6
00aa1fd9 0f296730             movaps xmmword ptr [edi + 0x30], xmm4
00aa1fdd e9ba000000           jmp 0xaa209c
00aa1fe2 8d4208               lea eax, [edx + 8]
00aa1fe5 8d5608               lea edx, [esi + 8]
00aa1fe8 bf04000000           mov edi, 4
00aa1fed 8d4900               lea ecx, [ecx]
00aa1ff0 d9442450             fld dword ptr [esp + 0x50]
00aa1ff4 83c010               add eax, 0x10
00aa1ff7 d848f4               fmul dword ptr [eax - 0xc]
00aa1ffa 83c210               add edx, 0x10
00aa1ffd 4f                   dec edi
00aa1ffe d98424b0000000       fld dword ptr [esp + 0xb0]
00aa2005 d848f0               fmul dword ptr [eax - 0x10]
00aa2008 dec1                 faddp st(1)
00aa200a d98424a0000000       fld dword ptr [esp + 0xa0]
00aa2011 d848ec               fmul dword ptr [eax - 0x14]
00aa2014 dec1                 faddp st(1)
00aa2016 d9842490000000       fld dword ptr [esp + 0x90]
00aa201d d848e8               fmul dword ptr [eax - 0x18]
00aa2020 dec1                 faddp st(1)
00aa2022 d95ae8               fstp dword ptr [edx - 0x18]
00aa2025 d98424b4000000       fld dword ptr [esp + 0xb4]
00aa202c d848f0               fmul dword ptr [eax - 0x10]
00aa202f d98424a4000000       fld dword ptr [esp + 0xa4]
00aa2036 d848ec               fmul dword ptr [eax - 0x14]
00aa2039 dec1                 faddp st(1)
00aa203b d9842494000000       fld dword ptr [esp + 0x94]
00aa2042 d848e8               fmul dword ptr [eax - 0x18]
00aa2045 dec1                 faddp st(1)
00aa2047 d9442454             fld dword ptr [esp + 0x54]
00aa204b d848f4               fmul dword ptr [eax - 0xc]
00aa204e dec1                 faddp st(1)
00aa2050 d95aec               fstp dword ptr [edx - 0x14]
00aa2053 d98424b8000000       fld dword ptr [esp + 0xb8]
00aa205a d848f0               fmul dword ptr [eax - 0x10]
00aa205d d98424a8000000       fld dword ptr [esp + 0xa8]
00aa2064 d848ec               fmul dword ptr [eax - 0x14]
00aa2067 dec1                 faddp st(1)
00aa2069 d9842498000000       fld dword ptr [esp + 0x98]
00aa2070 d848e8               fmul dword ptr [eax - 0x18]
00aa2073 dec1                 faddp st(1)
00aa2075 d9442458             fld dword ptr [esp + 0x58]
00aa2079 d848f4               fmul dword ptr [eax - 0xc]
00aa207c dec1                 faddp st(1)
00aa207e d95af0               fstp dword ptr [edx - 0x10]
00aa2081 d940e8               fld dword ptr [eax - 0x18]
00aa2084 d840f0               fadd dword ptr [eax - 0x10]
00aa2087 d840ec               fadd dword ptr [eax - 0x14]
00aa208a d80ddcde2201         fmul dword ptr [0x122dedc]
00aa2090 d840f4               fadd dword ptr [eax - 0xc]
00aa2093 d95af4               fstp dword ptr [edx - 0xc]
00aa2096 0f8554ffffff         jne 0xaa1ff0
00aa209c 8b442420             mov eax, dword ptr [esp + 0x20]
00aa20a0 83c130               add ecx, 0x30
00aa20a3 48                   dec eax
00aa20a4 89442420             mov dword ptr [esp + 0x20], eax
00aa20a8 0f8582fcffff         jne 0xaa1d30
00aa20ae 8b4514               mov eax, dword ptr [ebp + 0x14]
00aa20b1 8b0e                 mov ecx, dword ptr [esi]
00aa20b3 8b5604               mov edx, dword ptr [esi + 4]
00aa20b6 8908                 mov dword ptr [eax], ecx
00aa20b8 8b4e08               mov ecx, dword ptr [esi + 8]
00aa20bb 894808               mov dword ptr [eax + 8], ecx
00aa20be 8b4e14               mov ecx, dword ptr [esi + 0x14]
00aa20c1 895004               mov dword ptr [eax + 4], edx
00aa20c4 8b5610               mov edx, dword ptr [esi + 0x10]
00aa20c7 894810               mov dword ptr [eax + 0x10], ecx
00aa20ca 8b4e20               mov ecx, dword ptr [esi + 0x20]
00aa20cd 89500c               mov dword ptr [eax + 0xc], edx
00aa20d0 8b5618               mov edx, dword ptr [esi + 0x18]
00aa20d3 894818               mov dword ptr [eax + 0x18], ecx
00aa20d6 8b4e28               mov ecx, dword ptr [esi + 0x28]
00aa20d9 895014               mov dword ptr [eax + 0x14], edx
00aa20dc 8b5624               mov edx, dword ptr [esi + 0x24]
00aa20df 894820               mov dword ptr [eax + 0x20], ecx
00aa20e2 8b4e34               mov ecx, dword ptr [esi + 0x34]
00aa20e5 89501c               mov dword ptr [eax + 0x1c], edx
00aa20e8 8b5630               mov edx, dword ptr [esi + 0x30]
00aa20eb 894828               mov dword ptr [eax + 0x28], ecx
00aa20ee 8b4c2440             mov ecx, dword ptr [esp + 0x40]
00aa20f2 895024               mov dword ptr [eax + 0x24], edx
00aa20f5 8b5638               mov edx, dword ptr [esi + 0x38]
00aa20f8 83c114               add ecx, 0x14
00aa20fb 89502c               mov dword ptr [eax + 0x2c], edx
00aa20fe e81da6fbff           call 0xa5c720
00aa2103 5f                   pop edi
00aa2104 5e                   pop esi
00aa2105 5b                   pop ebx
00aa2106 8be5                 mov esp, ebp
00aa2108 5d                   pop ebp
00aa2109 c21400               ret 0x14
