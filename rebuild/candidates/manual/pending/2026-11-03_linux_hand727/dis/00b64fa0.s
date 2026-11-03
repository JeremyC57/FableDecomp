00b64fa0 83ec38               sub esp, 0x38
00b64fa3 a108b53c01           mov eax, dword ptr [0x13cb508]
00b64fa8 53                   push ebx
00b64fa9 894c2408             mov dword ptr [esp + 8], ecx
00b64fad 8d8808280000         lea ecx, [eax + 0x2808]
00b64fb3 55                   push ebp
00b64fb4 56                   push esi
00b64fb5 8b31                 mov esi, dword ptr [ecx]
00b64fb7 8d90243a0000         lea edx, [eax + 0x3a24]
00b64fbd 8954f008             mov dword ptr [eax + esi*8 + 8], edx
00b64fc1 ff01                 inc dword ptr [ecx]
00b64fc3 d1a014280000         shl dword ptr [eax + 0x2814], 1
00b64fc9 a18c6e4301           mov eax, dword ptr [0x1436e8c]
00b64fce 8b8898010000         mov ecx, dword ptr [eax + 0x198]
00b64fd4 8b11                 mov edx, dword ptr [ecx]
00b64fd6 57                   push edi
00b64fd7 ff5204               call dword ptr [edx + 4]
00b64fda 8b10                 mov edx, dword ptr [eax]
00b64fdc 8bc8                 mov ecx, eax
00b64fde ff5204               call dword ptr [edx + 4]
00b64fe1 89442418             mov dword ptr [esp + 0x18], eax
00b64fe5 8b4054               mov eax, dword ptr [eax + 0x54]
00b64fe8 8b88d8010000         mov ecx, dword ptr [eax + 0x1d8]
00b64fee 8b11                 mov edx, dword ptr [ecx]
00b64ff0 8b80c0010000         mov eax, dword ptr [eax + 0x1c0]
00b64ff6 8b4904               mov ecx, dword ptr [ecx + 4]
00b64ff9 89542428             mov dword ptr [esp + 0x28], edx
00b64ffd 8b10                 mov edx, dword ptr [eax]
00b64fff 8b4004               mov eax, dword ptr [eax + 4]
00b65002 33db                 xor ebx, ebx
00b65004 894c242c             mov dword ptr [esp + 0x2c], ecx
00b65008 89542430             mov dword ptr [esp + 0x30], edx
00b6500c 89442434             mov dword ptr [esp + 0x34], eax
00b65010 895c2410             mov dword ptr [esp + 0x10], ebx
00b65014 33ed                 xor ebp, ebp
00b65016 eb08                 jmp 0xb65020
00b65018 8da42400000000       lea esp, [esp]
00b6501f 90                   nop 
00b65020 8b742c28             mov esi, dword ptr [esp + ebp + 0x28]
00b65024 3bf3                 cmp esi, ebx
00b65026 8b0d986e4301         mov ecx, dword ptr [0x1436e98]
00b6502c 8b8954090000         mov ecx, dword ptr [ecx + 0x954]
00b65032 0f8491090000         je 0xb659c9
00b65038 8b91e4010000         mov edx, dword ptr [ecx + 0x1e4]
00b6503e 8bb9e0010000         mov edi, dword ptr [ecx + 0x1e0]
00b65044 2bd7                 sub edx, edi
00b65046 b8e9a28b2e           mov eax, 0x2e8ba2e9
00b6504b f7ea                 imul edx
00b6504d c1fa03               sar edx, 3
00b65050 8bc2                 mov eax, edx
00b65052 c1e81f               shr eax, 0x1f
00b65055 03c2                 add eax, edx
00b65057 3bf0                 cmp esi, eax
00b65059 7332                 jae 0xb6508d
00b6505b 8bd6                 mov edx, esi
00b6505d 6bd22c               imul edx, edx, 0x2c
00b65060 8bc7                 mov eax, edi
00b65062 8d3c02               lea edi, [edx + eax]
00b65065 3bfb                 cmp edi, ebx
00b65067 7424                 je 0xb6508d
00b65069 8b91e4010000         mov edx, dword ptr [ecx + 0x1e4]
00b6506f 2bd0                 sub edx, eax
00b65071 b8e9a28b2e           mov eax, 0x2e8ba2e9
00b65076 f7ea                 imul edx
00b65078 c1fa03               sar edx, 3
00b6507b 8bca                 mov ecx, edx
00b6507d c1e91f               shr ecx, 0x1f
00b65080 03ca                 add ecx, edx
00b65082 3bf1                 cmp esi, ecx
00b65084 1bc0                 sbb eax, eax
00b65086 23c7                 and eax, edi
00b65088 8b7028               mov esi, dword ptr [eax + 0x28]
00b6508b eb02                 jmp 0xb6508f
00b6508d 33f6                 xor esi, esi
00b6508f 8b4e08               mov ecx, dword ptr [esi + 8]
00b65092 3bcb                 cmp ecx, ebx
00b65094 0f844d090000         je 0xb659e7
00b6509a 39591c               cmp dword ptr [ecx + 0x1c], ebx
00b6509d 0f841f090000         je 0xb659c2
00b650a3 8b560c               mov edx, dword ptr [esi + 0xc]
00b650a6 8b4220               mov eax, dword ptr [edx + 0x20]
00b650a9 8b9094010000         mov edx, dword ptr [eax + 0x194]
00b650af 89513c               mov dword ptr [ecx + 0x3c], edx
00b650b2 8b4608               mov eax, dword ptr [esi + 8]
00b650b5 8b7824               mov edi, dword ptr [eax + 0x24]
00b650b8 83c020               add eax, 0x20
00b650bb 3bfb                 cmp edi, ebx
00b650bd 7431                 je 0xb650f0
00b650bf 8b4708               mov eax, dword ptr [edi + 8]
00b650c2 3bc3                 cmp eax, ebx
00b650c4 742a                 je 0xb650f0
00b650c6 8b4f18               mov ecx, dword ptr [edi + 0x18]
00b650c9 3b482c               cmp ecx, dword ptr [eax + 0x2c]
00b650cc 7422                 je 0xb650f0
00b650ce 8bcf                 mov ecx, edi
00b650d0 e8db6ee9ff           call 0x9fbfb0
00b650d5 8b4708               mov eax, dword ptr [edi + 8]
00b650d8 8d5004               lea edx, [eax + 4]
00b650db 8b4010               mov eax, dword ptr [eax + 0x10]
00b650de 52                   push edx
00b650df 50                   push eax
00b650e0 8bcf                 mov ecx, edi
00b650e2 e8a96ee9ff           call 0x9fbf90
00b650e7 8b4f08               mov ecx, dword ptr [edi + 8]
00b650ea 8b512c               mov edx, dword ptr [ecx + 0x2c]
00b650ed 895718               mov dword ptr [edi + 0x18], edx
00b650f0 8b7e08               mov edi, dword ptr [esi + 8]
00b650f3 8b4708               mov eax, dword ptr [edi + 8]
00b650f6 3bc3                 cmp eax, ebx
00b650f8 742a                 je 0xb65124
00b650fa 8b4f18               mov ecx, dword ptr [edi + 0x18]
00b650fd 3b482c               cmp ecx, dword ptr [eax + 0x2c]
00b65100 7422                 je 0xb65124
00b65102 8bcf                 mov ecx, edi
00b65104 e8a76ee9ff           call 0x9fbfb0
00b65109 8b4708               mov eax, dword ptr [edi + 8]
00b6510c 8d5004               lea edx, [eax + 4]
00b6510f 8b4010               mov eax, dword ptr [eax + 0x10]
00b65112 52                   push edx
00b65113 50                   push eax
00b65114 8bcf                 mov ecx, edi
00b65116 e8756ee9ff           call 0x9fbf90
00b6511b 8b4f08               mov ecx, dword ptr [edi + 8]
00b6511e 8b512c               mov edx, dword ptr [ecx + 0x2c]
00b65121 895718               mov dword ptr [edi + 0x18], edx
00b65124 8b4608               mov eax, dword ptr [esi + 8]
00b65127 8a483a               mov cl, byte ptr [eax + 0x3a]
00b6512a 84c9                 test cl, cl
00b6512c 7604                 jbe 0xb65132
00b6512e c6403a00             mov byte ptr [eax + 0x3a], 0
00b65132 8b4e08               mov ecx, dword ptr [esi + 8]
00b65135 8a4138               mov al, byte ptr [ecx + 0x38]
00b65138 84c0                 test al, al
00b6513a 0f867a080000         jbe 0xb659ba
00b65140 53                   push ebx
00b65141 53                   push ebx
00b65142 e8398de9ff           call 0x9fde80
00b65147 8b7608               mov esi, dword ptr [esi + 8]
00b6514a 8b761c               mov esi, dword ptr [esi + 0x1c]
00b6514d 3bf3                 cmp esi, ebx
00b6514f 7402                 je 0xb65153
00b65151 ff06                 inc dword ptr [esi]
00b65153 8b4e08               mov ecx, dword ptr [esi + 8]
00b65156 894c2c38             mov dword ptr [esp + ebp + 0x38], ecx
00b6515a ff0e                 dec dword ptr [esi]
00b6515c 753e                 jne 0xb6519c
00b6515e f6461a10             test byte ptr [esi + 0x1a], 0x10
00b65162 7407                 je 0xb6516b
00b65164 8bce                 mov ecx, esi
00b65166 e8357ae9ff           call 0x9fcba0
00b6516b 8b460c               mov eax, dword ptr [esi + 0xc]
00b6516e 3bc3                 cmp eax, ebx
00b65170 741b                 je 0xb6518d
00b65172 ff08                 dec dword ptr [eax]
00b65174 8b460c               mov eax, dword ptr [esi + 0xc]
00b65177 3918                 cmp dword ptr [eax], ebx
00b65179 7512                 jne 0xb6518d
00b6517b 8b4808               mov ecx, dword ptr [eax + 8]
00b6517e ff5004               call dword ptr [eax + 4]
00b65181 8b460c               mov eax, dword ptr [esi + 0xc]
00b65184 50                   push eax
00b65185 e832980900           call 0xbfe9bc
00b6518a 83c404               add esp, 4
00b6518d 56                   push esi
00b6518e 895e08               mov dword ptr [esi + 8], ebx
00b65191 895e0c               mov dword ptr [esi + 0xc], ebx
00b65194 e823980900           call 0xbfe9bc
00b65199 83c404               add esp, 4
00b6519c 83c504               add ebp, 4
00b6519f 83fd10               cmp ebp, 0x10
00b651a2 0f8c78feffff         jl 0xb65020
00b651a8 837c241004           cmp dword ptr [esp + 0x10], 4
00b651ad 0f84f2070000         je 0xb659a5
00b651b3 8b6c2414             mov ebp, dword ptr [esp + 0x14]
00b651b7 8d8d20010000         lea ecx, [ebp + 0x120]
00b651bd 51                   push ecx
00b651be 8b0d146e4301         mov ecx, dword ptr [0x1436e14]
00b651c4 e8772fe2ff           call 0x988140
00b651c9 d905d8de2201         fld dword ptr [0x122ded8]
00b651cf 8b542418             mov edx, dword ptr [esp + 0x18]
00b651d3 8b4254               mov eax, dword ptr [edx + 0x54]
00b651d6 8b88e4010000         mov ecx, dword ptr [eax + 0x1e4]
00b651dc d821                 fsub dword ptr [ecx]
00b651de 8b90cc010000         mov edx, dword ptr [eax + 0x1cc]
00b651e4 d905d8de2201         fld dword ptr [0x122ded8]
00b651ea 83c070               add eax, 0x70
00b651ed d822                 fsub dword ptr [edx]
00b651ef 8b08                 mov ecx, dword ptr [eax]
00b651f1 8b5004               mov edx, dword ptr [eax + 4]
00b651f4 d9c1                 fld st(1)
00b651f6 8954242c             mov dword ptr [esp + 0x2c], edx
00b651fa d95c2418             fstp dword ptr [esp + 0x18]
00b651fe 894c2428             mov dword ptr [esp + 0x28], ecx
00b65202 d9c1                 fld st(1)
00b65204 8b4808               mov ecx, dword ptr [eax + 8]
00b65207 d95c241c             fstp dword ptr [esp + 0x1c]
00b6520b 8b500c               mov edx, dword ptr [eax + 0xc]
00b6520e d9c1                 fld st(1)
00b65210 a1146e4301           mov eax, dword ptr [0x1436e14]
00b65215 d95c2420             fstp dword ptr [esp + 0x20]
00b65219 89542434             mov dword ptr [esp + 0x34], edx
00b6521d d9c9                 fxch st(1)
00b6521f 8b542418             mov edx, dword ptr [esp + 0x18]
00b65223 d95c2424             fstp dword ptr [esp + 0x24]
00b65227 894c2430             mov dword ptr [esp + 0x30], ecx
00b6522b 8b88cc030000         mov ecx, dword ptr [eax + 0x3cc]
00b65231 d9542418             fst dword ptr [esp + 0x18]
00b65235 8b742418             mov esi, dword ptr [esp + 0x18]
00b65239 83c150               add ecx, 0x50
00b6523c 8911                 mov dword ptr [ecx], edx
00b6523e 8b54241c             mov edx, dword ptr [esp + 0x1c]
00b65242 d954241c             fst dword ptr [esp + 0x1c]
00b65246 895104               mov dword ptr [ecx + 4], edx
00b65249 8b542420             mov edx, dword ptr [esp + 0x20]
00b6524d d9542420             fst dword ptr [esp + 0x20]
00b65251 895108               mov dword ptr [ecx + 8], edx
00b65254 8b542424             mov edx, dword ptr [esp + 0x24]
00b65258 d95c2424             fstp dword ptr [esp + 0x24]
00b6525c 89510c               mov dword ptr [ecx + 0xc], edx
00b6525f 8b90d8030000         mov edx, dword ptr [eax + 0x3d8]
00b65265 b902000000           mov ecx, 2
00b6526a 0bd1                 or edx, ecx
00b6526c 8990d8030000         mov dword ptr [eax + 0x3d8], edx
00b65272 a1146e4301           mov eax, dword ptr [0x1436e14]
00b65277 8b90cc030000         mov edx, dword ptr [eax + 0x3cc]
00b6527d 83c260               add edx, 0x60
00b65280 8932                 mov dword ptr [edx], esi
00b65282 8b74241c             mov esi, dword ptr [esp + 0x1c]
00b65286 897204               mov dword ptr [edx + 4], esi
00b65289 8b742420             mov esi, dword ptr [esp + 0x20]
00b6528d 897208               mov dword ptr [edx + 8], esi
00b65290 8b742424             mov esi, dword ptr [esp + 0x24]
00b65294 89720c               mov dword ptr [edx + 0xc], esi
00b65297 8bb8d8030000         mov edi, dword ptr [eax + 0x3d8]
00b6529d 8b54242c             mov edx, dword ptr [esp + 0x2c]
00b652a1 0bf9                 or edi, ecx
00b652a3 89b8d8030000         mov dword ptr [eax + 0x3d8], edi
00b652a9 8b442428             mov eax, dword ptr [esp + 0x28]
00b652ad 89442418             mov dword ptr [esp + 0x18], eax
00b652b1 8b442430             mov eax, dword ptr [esp + 0x30]
00b652b5 8b742418             mov esi, dword ptr [esp + 0x18]
00b652b9 8954241c             mov dword ptr [esp + 0x1c], edx
00b652bd 89442420             mov dword ptr [esp + 0x20], eax
00b652c1 a1146e4301           mov eax, dword ptr [0x1436e14]
00b652c6 8b90cc030000         mov edx, dword ptr [eax + 0x3cc]
00b652cc 83c270               add edx, 0x70
00b652cf 8932                 mov dword ptr [edx], esi
00b652d1 8b74241c             mov esi, dword ptr [esp + 0x1c]
00b652d5 897204               mov dword ptr [edx + 4], esi
00b652d8 8b742420             mov esi, dword ptr [esp + 0x20]
00b652dc 897208               mov dword ptr [edx + 8], esi
00b652df c74424240000803f     mov dword ptr [esp + 0x24], 0x3f800000
00b652e7 8b742424             mov esi, dword ptr [esp + 0x24]
00b652eb 89720c               mov dword ptr [edx + 0xc], esi
00b652ee 0988d8030000         or dword ptr [eax + 0x3d8], ecx
00b652f4 a1186e4301           mov eax, dword ptr [0x1436e18]
00b652f9 8b0dc86d3901         mov ecx, dword ptr [0x1396dc8]
00b652ff 3988e82e0000         cmp dword ptr [eax + 0x2ee8], ecx
00b65305 8d90e42e0000         lea edx, [eax + 0x2ee4]
00b6530b 8bf0                 mov esi, eax
00b6530d 8bf9                 mov edi, ecx
00b6530f bb01000000           mov ebx, 1
00b65314 745a                 je 0xb65370
00b65316 8b8014280000         mov eax, dword ptr [eax + 0x2814]
00b6531c 8b4a08               mov ecx, dword ptr [edx + 8]
00b6531f 85c8                 test eax, ecx
00b65321 7522                 jne 0xb65345
00b65323 0bc8                 or ecx, eax
00b65325 894a08               mov dword ptr [edx + 8], ecx
00b65328 8b8608280000         mov eax, dword ptr [esi + 0x2808]
00b6532e 8954c608             mov dword ptr [esi + eax*8 + 8], edx
00b65332 8b4204               mov eax, dword ptr [edx + 4]
00b65335 8b8e08280000         mov ecx, dword ptr [esi + 0x2808]
00b6533b 8944ce0c             mov dword ptr [esi + ecx*8 + 0xc], eax
00b6533f ff8608280000         inc dword ptr [esi + 0x2808]
00b65345 8a4210               mov al, byte ptr [edx + 0x10]
00b65348 84c0                 test al, al
00b6534a 897a04               mov dword ptr [edx + 4], edi
00b6534d 7516                 jne 0xb65365
00b6534f 885a10               mov byte ptr [edx + 0x10], bl
00b65352 8b8e0c280000         mov ecx, dword ptr [esi + 0x280c]
00b65358 89948e08200000       mov dword ptr [esi + ecx*4 + 0x2008], edx
00b6535f ff860c280000         inc dword ptr [esi + 0x280c]
00b65365 a1186e4301           mov eax, dword ptr [0x1436e18]
00b6536a 8b0dc86d3901         mov ecx, dword ptr [0x1396dc8]
00b65370 3988882f0000         cmp dword ptr [eax + 0x2f88], ecx
00b65376 8db0842f0000         lea esi, [eax + 0x2f84]
00b6537c 8bd0                 mov edx, eax
00b6537e 8bf9                 mov edi, ecx
00b65380 745a                 je 0xb653dc
00b65382 8b8014280000         mov eax, dword ptr [eax + 0x2814]
00b65388 8b4e08               mov ecx, dword ptr [esi + 8]
00b6538b 85c8                 test eax, ecx
00b6538d 7522                 jne 0xb653b1
00b6538f 0bc8                 or ecx, eax
00b65391 894e08               mov dword ptr [esi + 8], ecx
00b65394 8b8208280000         mov eax, dword ptr [edx + 0x2808]
00b6539a 8974c208             mov dword ptr [edx + eax*8 + 8], esi
00b6539e 8b4604               mov eax, dword ptr [esi + 4]
00b653a1 8b8a08280000         mov ecx, dword ptr [edx + 0x2808]
00b653a7 8944ca0c             mov dword ptr [edx + ecx*8 + 0xc], eax
00b653ab ff8208280000         inc dword ptr [edx + 0x2808]
00b653b1 8a4610               mov al, byte ptr [esi + 0x10]
00b653b4 84c0                 test al, al
00b653b6 897e04               mov dword ptr [esi + 4], edi
00b653b9 7516                 jne 0xb653d1
00b653bb 885e10               mov byte ptr [esi + 0x10], bl
00b653be 8b8a0c280000         mov ecx, dword ptr [edx + 0x280c]
00b653c4 89b48a08200000       mov dword ptr [edx + ecx*4 + 0x2008], esi
00b653cb ff820c280000         inc dword ptr [edx + 0x280c]
00b653d1 a1186e4301           mov eax, dword ptr [0x1436e18]
00b653d6 8b0dc86d3901         mov ecx, dword ptr [0x1396dc8]
00b653dc 3988fc2e0000         cmp dword ptr [eax + 0x2efc], ecx
00b653e2 8db0f82e0000         lea esi, [eax + 0x2ef8]
00b653e8 8bd0                 mov edx, eax
00b653ea 8bf9                 mov edi, ecx
00b653ec 745a                 je 0xb65448
00b653ee 8b8014280000         mov eax, dword ptr [eax + 0x2814]
00b653f4 8b4e08               mov ecx, dword ptr [esi + 8]
00b653f7 85c1                 test ecx, eax
00b653f9 7522                 jne 0xb6541d
00b653fb 0bc1                 or eax, ecx
00b653fd 894608               mov dword ptr [esi + 8], eax
00b65400 8b8208280000         mov eax, dword ptr [edx + 0x2808]
00b65406 8974c208             mov dword ptr [edx + eax*8 + 8], esi
00b6540a 8b4604               mov eax, dword ptr [esi + 4]
00b6540d 8b8a08280000         mov ecx, dword ptr [edx + 0x2808]
00b65413 8944ca0c             mov dword ptr [edx + ecx*8 + 0xc], eax
00b65417 ff8208280000         inc dword ptr [edx + 0x2808]
00b6541d 8a4610               mov al, byte ptr [esi + 0x10]
00b65420 84c0                 test al, al
00b65422 897e04               mov dword ptr [esi + 4], edi
00b65425 7516                 jne 0xb6543d
00b65427 885e10               mov byte ptr [esi + 0x10], bl
00b6542a 8b8a0c280000         mov ecx, dword ptr [edx + 0x280c]
00b65430 89b48a08200000       mov dword ptr [edx + ecx*4 + 0x2008], esi
00b65437 ff820c280000         inc dword ptr [edx + 0x280c]
00b6543d a1186e4301           mov eax, dword ptr [0x1436e18]
00b65442 8b0dc86d3901         mov ecx, dword ptr [0x1396dc8]
00b65448 39889c2f0000         cmp dword ptr [eax + 0x2f9c], ecx
00b6544e 8db0982f0000         lea esi, [eax + 0x2f98]
00b65454 8bd0                 mov edx, eax
00b65456 8bf9                 mov edi, ecx
00b65458 745a                 je 0xb654b4
00b6545a 8b8014280000         mov eax, dword ptr [eax + 0x2814]
00b65460 8b4e08               mov ecx, dword ptr [esi + 8]
00b65463 85c1                 test ecx, eax
00b65465 7522                 jne 0xb65489
00b65467 0bc1                 or eax, ecx
00b65469 894608               mov dword ptr [esi + 8], eax
00b6546c 8b8208280000         mov eax, dword ptr [edx + 0x2808]
00b65472 8974c208             mov dword ptr [edx + eax*8 + 8], esi
00b65476 8b4604               mov eax, dword ptr [esi + 4]
00b65479 8b8a08280000         mov ecx, dword ptr [edx + 0x2808]
00b6547f 8944ca0c             mov dword ptr [edx + ecx*8 + 0xc], eax
00b65483 ff8208280000         inc dword ptr [edx + 0x2808]
00b65489 8a4610               mov al, byte ptr [esi + 0x10]
00b6548c 84c0                 test al, al
00b6548e 897e04               mov dword ptr [esi + 4], edi
00b65491 7516                 jne 0xb654a9
00b65493 885e10               mov byte ptr [esi + 0x10], bl
00b65496 8b8a0c280000         mov ecx, dword ptr [edx + 0x280c]
00b6549c 89b48a08200000       mov dword ptr [edx + ecx*4 + 0x2008], esi
00b654a3 ff820c280000         inc dword ptr [edx + 0x280c]
00b654a9 a1186e4301           mov eax, dword ptr [0x1436e18]
00b654ae 8b0dc86d3901         mov ecx, dword ptr [0x1396dc8]
00b654b4 3988102f0000         cmp dword ptr [eax + 0x2f10], ecx
00b654ba 8db00c2f0000         lea esi, [eax + 0x2f0c]
00b654c0 8bd0                 mov edx, eax
00b654c2 8bf9                 mov edi, ecx
00b654c4 745a                 je 0xb65520
00b654c6 8b8014280000         mov eax, dword ptr [eax + 0x2814]
00b654cc 8b4e08               mov ecx, dword ptr [esi + 8]
00b654cf 85c1                 test ecx, eax
00b654d1 7522                 jne 0xb654f5
00b654d3 0bc1                 or eax, ecx
00b654d5 894608               mov dword ptr [esi + 8], eax
00b654d8 8b8208280000         mov eax, dword ptr [edx + 0x2808]
00b654de 8974c208             mov dword ptr [edx + eax*8 + 8], esi
00b654e2 8b4604               mov eax, dword ptr [esi + 4]
00b654e5 8b8a08280000         mov ecx, dword ptr [edx + 0x2808]
00b654eb 8944ca0c             mov dword ptr [edx + ecx*8 + 0xc], eax
00b654ef ff8208280000         inc dword ptr [edx + 0x2808]
00b654f5 8a4610               mov al, byte ptr [esi + 0x10]
00b654f8 84c0                 test al, al
00b654fa 897e04               mov dword ptr [esi + 4], edi
00b654fd 7516                 jne 0xb65515
00b654ff 885e10               mov byte ptr [esi + 0x10], bl
00b65502 8b8a0c280000         mov ecx, dword ptr [edx + 0x280c]
00b65508 89b48a08200000       mov dword ptr [edx + ecx*4 + 0x2008], esi
00b6550f ff820c280000         inc dword ptr [edx + 0x280c]
00b65515 a1186e4301           mov eax, dword ptr [0x1436e18]
00b6551a 8b0dc86d3901         mov ecx, dword ptr [0x1396dc8]
00b65520 3988b02f0000         cmp dword ptr [eax + 0x2fb0], ecx
00b65526 8db0ac2f0000         lea esi, [eax + 0x2fac]
00b6552c 8bd0                 mov edx, eax
00b6552e 8bf9                 mov edi, ecx
00b65530 745a                 je 0xb6558c
00b65532 8b8014280000         mov eax, dword ptr [eax + 0x2814]
00b65538 8b4e08               mov ecx, dword ptr [esi + 8]
00b6553b 85c8                 test eax, ecx
00b6553d 7522                 jne 0xb65561
00b6553f 0bc8                 or ecx, eax
00b65541 894e08               mov dword ptr [esi + 8], ecx
00b65544 8b8208280000         mov eax, dword ptr [edx + 0x2808]
00b6554a 8974c208             mov dword ptr [edx + eax*8 + 8], esi
00b6554e 8b4604               mov eax, dword ptr [esi + 4]
00b65551 8b8a08280000         mov ecx, dword ptr [edx + 0x2808]
00b65557 8944ca0c             mov dword ptr [edx + ecx*8 + 0xc], eax
00b6555b ff8208280000         inc dword ptr [edx + 0x2808]
00b65561 8a4610               mov al, byte ptr [esi + 0x10]
00b65564 84c0                 test al, al
00b65566 897e04               mov dword ptr [esi + 4], edi
00b65569 7516                 jne 0xb65581
00b6556b 885e10               mov byte ptr [esi + 0x10], bl
00b6556e 8b8a0c280000         mov ecx, dword ptr [edx + 0x280c]
00b65574 89b48a08200000       mov dword ptr [edx + ecx*4 + 0x2008], esi
00b6557b ff820c280000         inc dword ptr [edx + 0x280c]
00b65581 a1186e4301           mov eax, dword ptr [0x1436e18]
00b65586 8b0dc86d3901         mov ecx, dword ptr [0x1396dc8]
00b6558c 3988242f0000         cmp dword ptr [eax + 0x2f24], ecx
00b65592 8db0202f0000         lea esi, [eax + 0x2f20]
00b65598 8bd0                 mov edx, eax
00b6559a 8bf9                 mov edi, ecx
00b6559c 745a                 je 0xb655f8
00b6559e 8b8014280000         mov eax, dword ptr [eax + 0x2814]
00b655a4 8b4e08               mov ecx, dword ptr [esi + 8]
00b655a7 85c8                 test eax, ecx
00b655a9 7522                 jne 0xb655cd
00b655ab 0bc8                 or ecx, eax
00b655ad 894e08               mov dword ptr [esi + 8], ecx
00b655b0 8b8208280000         mov eax, dword ptr [edx + 0x2808]
00b655b6 8974c208             mov dword ptr [edx + eax*8 + 8], esi
00b655ba 8b4604               mov eax, dword ptr [esi + 4]
00b655bd 8b8a08280000         mov ecx, dword ptr [edx + 0x2808]
00b655c3 8944ca0c             mov dword ptr [edx + ecx*8 + 0xc], eax
00b655c7 ff8208280000         inc dword ptr [edx + 0x2808]
00b655cd 8a4610               mov al, byte ptr [esi + 0x10]
00b655d0 84c0                 test al, al
00b655d2 897e04               mov dword ptr [esi + 4], edi
00b655d5 7516                 jne 0xb655ed
00b655d7 885e10               mov byte ptr [esi + 0x10], bl
00b655da 8b8a0c280000         mov ecx, dword ptr [edx + 0x280c]
00b655e0 89b48a08200000       mov dword ptr [edx + ecx*4 + 0x2008], esi
00b655e7 ff820c280000         inc dword ptr [edx + 0x280c]
00b655ed a1186e4301           mov eax, dword ptr [0x1436e18]
00b655f2 8b0dc86d3901         mov ecx, dword ptr [0x1396dc8]
00b655f8 3988c42f0000         cmp dword ptr [eax + 0x2fc4], ecx
00b655fe 8db0c02f0000         lea esi, [eax + 0x2fc0]
00b65604 8bd0                 mov edx, eax
00b65606 8bf9                 mov edi, ecx
00b65608 7454                 je 0xb6565e
00b6560a 8b8014280000         mov eax, dword ptr [eax + 0x2814]
00b65610 8b4e08               mov ecx, dword ptr [esi + 8]
00b65613 85c8                 test eax, ecx
00b65615 7522                 jne 0xb65639
00b65617 0bc8                 or ecx, eax
00b65619 894e08               mov dword ptr [esi + 8], ecx
00b6561c 8b8208280000         mov eax, dword ptr [edx + 0x2808]
00b65622 8974c208             mov dword ptr [edx + eax*8 + 8], esi
00b65626 8b4604               mov eax, dword ptr [esi + 4]
00b65629 8b8a08280000         mov ecx, dword ptr [edx + 0x2808]
00b6562f 8944ca0c             mov dword ptr [edx + ecx*8 + 0xc], eax
00b65633 ff8208280000         inc dword ptr [edx + 0x2808]
00b65639 8a4610               mov al, byte ptr [esi + 0x10]
00b6563c 84c0                 test al, al
00b6563e 897e04               mov dword ptr [esi + 4], edi
00b65641 7516                 jne 0xb65659
00b65643 885e10               mov byte ptr [esi + 0x10], bl
00b65646 8b8a0c280000         mov ecx, dword ptr [edx + 0x280c]
00b6564c 89b48a08200000       mov dword ptr [edx + ecx*4 + 0x2008], esi
00b65653 ff820c280000         inc dword ptr [edx + 0x280c]
00b65659 a1186e4301           mov eax, dword ptr [0x1436e18]
00b6565e 8b356c6f3901         mov esi, dword ptr [0x1396f6c]
00b65664 39b020290000         cmp dword ptr [eax + 0x2920], esi
00b6566a 8d901c290000         lea edx, [eax + 0x291c]
00b65670 8bc8                 mov ecx, eax
00b65672 8bfe                 mov edi, esi
00b65674 7454                 je 0xb656ca
00b65676 8b8014280000         mov eax, dword ptr [eax + 0x2814]
00b6567c 8b7208               mov esi, dword ptr [edx + 8]
00b6567f 85f0                 test eax, esi
00b65681 7522                 jne 0xb656a5
00b65683 0bf0                 or esi, eax
00b65685 897208               mov dword ptr [edx + 8], esi
00b65688 8b8108280000         mov eax, dword ptr [ecx + 0x2808]
00b6568e 8954c108             mov dword ptr [ecx + eax*8 + 8], edx
00b65692 8b8108280000         mov eax, dword ptr [ecx + 0x2808]
00b65698 8b7204               mov esi, dword ptr [edx + 4]
00b6569b 8974c10c             mov dword ptr [ecx + eax*8 + 0xc], esi
00b6569f ff8108280000         inc dword ptr [ecx + 0x2808]
00b656a5 8a4210               mov al, byte ptr [edx + 0x10]
00b656a8 84c0                 test al, al
00b656aa 897a04               mov dword ptr [edx + 4], edi
00b656ad 7516                 jne 0xb656c5
00b656af 885a10               mov byte ptr [edx + 0x10], bl
00b656b2 8b810c280000         mov eax, dword ptr [ecx + 0x280c]
00b656b8 89948108200000       mov dword ptr [ecx + eax*4 + 0x2008], edx
00b656bf ff810c280000         inc dword ptr [ecx + 0x280c]
00b656c5 a1186e4301           mov eax, dword ptr [0x1436e18]
00b656ca 8b357c6f3901         mov esi, dword ptr [0x1396f7c]
00b656d0 39b034290000         cmp dword ptr [eax + 0x2934], esi
00b656d6 8d9030290000         lea edx, [eax + 0x2930]
00b656dc 8bc8                 mov ecx, eax
00b656de 8bfe                 mov edi, esi
00b656e0 7454                 je 0xb65736
00b656e2 8b8014280000         mov eax, dword ptr [eax + 0x2814]
00b656e8 8b7208               mov esi, dword ptr [edx + 8]
00b656eb 85f0                 test eax, esi
00b656ed 7522                 jne 0xb65711
00b656ef 0bf0                 or esi, eax
00b656f1 897208               mov dword ptr [edx + 8], esi
00b656f4 8b8108280000         mov eax, dword ptr [ecx + 0x2808]
00b656fa 8954c108             mov dword ptr [ecx + eax*8 + 8], edx
00b656fe 8b8108280000         mov eax, dword ptr [ecx + 0x2808]
00b65704 8b7204               mov esi, dword ptr [edx + 4]
00b65707 8974c10c             mov dword ptr [ecx + eax*8 + 0xc], esi
00b6570b ff8108280000         inc dword ptr [ecx + 0x2808]
00b65711 8a4210               mov al, byte ptr [edx + 0x10]
00b65714 84c0                 test al, al
00b65716 897a04               mov dword ptr [edx + 4], edi
00b65719 7516                 jne 0xb65731
00b6571b 885a10               mov byte ptr [edx + 0x10], bl
00b6571e 8b810c280000         mov eax, dword ptr [ecx + 0x280c]
00b65724 89948108200000       mov dword ptr [ecx + eax*4 + 0x2008], edx
00b6572b ff810c280000         inc dword ptr [ecx + 0x280c]
00b65731 a1186e4301           mov eax, dword ptr [0x1436e18]
00b65736 8bb0bc280000         mov esi, dword ptr [eax + 0x28bc]
00b6573c 3bf3                 cmp esi, ebx
00b6573e 8d90b8280000         lea edx, [eax + 0x28b8]
00b65744 8bc8                 mov ecx, eax
00b65746 7454                 je 0xb6579c
00b65748 8b8014280000         mov eax, dword ptr [eax + 0x2814]
00b6574e 8b7208               mov esi, dword ptr [edx + 8]
00b65751 85f0                 test eax, esi
00b65753 7522                 jne 0xb65777
00b65755 0bf0                 or esi, eax
00b65757 897208               mov dword ptr [edx + 8], esi
00b6575a 8b8108280000         mov eax, dword ptr [ecx + 0x2808]
00b65760 8954c108             mov dword ptr [ecx + eax*8 + 8], edx
00b65764 8b8108280000         mov eax, dword ptr [ecx + 0x2808]
00b6576a 8b7204               mov esi, dword ptr [edx + 4]
00b6576d 8974c10c             mov dword ptr [ecx + eax*8 + 0xc], esi
00b65771 ff8108280000         inc dword ptr [ecx + 0x2808]
00b65777 8a4210               mov al, byte ptr [edx + 0x10]
00b6577a 84c0                 test al, al
00b6577c 895a04               mov dword ptr [edx + 4], ebx
00b6577f 7516                 jne 0xb65797
00b65781 885a10               mov byte ptr [edx + 0x10], bl
00b65784 8b810c280000         mov eax, dword ptr [ecx + 0x280c]
00b6578a 89948108200000       mov dword ptr [ecx + eax*4 + 0x2008], edx
00b65791 ff810c280000         inc dword ptr [ecx + 0x280c]
00b65797 a1186e4301           mov eax, dword ptr [0x1436e18]
00b6579c 8bb0d0280000         mov esi, dword ptr [eax + 0x28d0]
00b657a2 85f6                 test esi, esi
00b657a4 8d90cc280000         lea edx, [eax + 0x28cc]
00b657aa 8bc8                 mov ecx, eax
00b657ac 7453                 je 0xb65801
00b657ae 8b8014280000         mov eax, dword ptr [eax + 0x2814]
00b657b4 8b7208               mov esi, dword ptr [edx + 8]
00b657b7 85f0                 test eax, esi
00b657b9 7522                 jne 0xb657dd
00b657bb 0bf0                 or esi, eax
00b657bd 897208               mov dword ptr [edx + 8], esi
00b657c0 8b8108280000         mov eax, dword ptr [ecx + 0x2808]
00b657c6 8954c108             mov dword ptr [ecx + eax*8 + 8], edx
00b657ca 8b8108280000         mov eax, dword ptr [ecx + 0x2808]
00b657d0 8b7204               mov esi, dword ptr [edx + 4]
00b657d3 8974c10c             mov dword ptr [ecx + eax*8 + 0xc], esi
00b657d7 ff8108280000         inc dword ptr [ecx + 0x2808]
00b657dd 8a4210               mov al, byte ptr [edx + 0x10]
00b657e0 84c0                 test al, al
00b657e2 c7420400000000       mov dword ptr [edx + 4], 0
00b657e9 7516                 jne 0xb65801
00b657eb 885a10               mov byte ptr [edx + 0x10], bl
00b657ee 8b810c280000         mov eax, dword ptr [ecx + 0x280c]
00b657f4 89948108200000       mov dword ptr [ecx + eax*4 + 0x2008], edx
00b657fb ff810c280000         inc dword ptr [ecx + 0x280c]
00b65801 6a00                 push 0
00b65803 8d8de8000000         lea ecx, [ebp + 0xe8]
00b65809 51                   push ecx
00b6580a 8b0d146e4301         mov ecx, dword ptr [0x1436e14]
00b65810 e80b28e2ff           call 0x988020
00b65815 d9856c010000         fld dword ptr [ebp + 0x16c]
00b6581b d8a574010000         fsub dword ptr [ebp + 0x174]
00b65821 d98570010000         fld dword ptr [ebp + 0x170]
00b65827 d8a578010000         fsub dword ptr [ebp + 0x178]
00b6582d d9c9                 fxch st(1)
00b6582f d84c244c             fmul dword ptr [esp + 0x4c]
00b65833 d95c2418             fstp dword ptr [esp + 0x18]
00b65837 d84c244c             fmul dword ptr [esp + 0x4c]
00b6583b d9442418             fld dword ptr [esp + 0x18]
00b6583f d88574010000         fadd dword ptr [ebp + 0x174]
00b65845 d95c2418             fstp dword ptr [esp + 0x18]
00b65849 8b542418             mov edx, dword ptr [esp + 0x18]
00b6584d d88578010000         fadd dword ptr [ebp + 0x178]
00b65853 89955c010000         mov dword ptr [ebp + 0x15c], edx
00b65859 d95c241c             fstp dword ptr [esp + 0x1c]
00b6585d 8b44241c             mov eax, dword ptr [esp + 0x1c]
00b65861 898560010000         mov dword ptr [ebp + 0x160], eax
00b65867 d9857c010000         fld dword ptr [ebp + 0x17c]
00b6586d d8a584010000         fsub dword ptr [ebp + 0x184]
00b65873 d98580010000         fld dword ptr [ebp + 0x180]
00b65879 d8a588010000         fsub dword ptr [ebp + 0x188]
00b6587f d9c9                 fxch st(1)
00b65881 d84c244c             fmul dword ptr [esp + 0x4c]
00b65885 d95c2418             fstp dword ptr [esp + 0x18]
00b65889 d84c244c             fmul dword ptr [esp + 0x4c]
00b6588d d9442418             fld dword ptr [esp + 0x18]
00b65891 d88584010000         fadd dword ptr [ebp + 0x184]
00b65897 d95c2418             fstp dword ptr [esp + 0x18]
00b6589b 8b4c2418             mov ecx, dword ptr [esp + 0x18]
00b6589f d88588010000         fadd dword ptr [ebp + 0x188]
00b658a5 898d64010000         mov dword ptr [ebp + 0x164], ecx
00b658ab d95c241c             fstp dword ptr [esp + 0x1c]
00b658af 8b54241c             mov edx, dword ptr [esp + 0x1c]
00b658b3 8bc2                 mov eax, edx
00b658b5 50                   push eax
00b658b6 8b855c010000         mov eax, dword ptr [ebp + 0x15c]
00b658bc 51                   push ecx
00b658bd 899568010000         mov dword ptr [ebp + 0x168], edx
00b658c3 8b9560010000         mov edx, dword ptr [ebp + 0x160]
00b658c9 8b0d146e4301         mov ecx, dword ptr [0x1436e14]
00b658cf 52                   push edx
00b658d0 50                   push eax
00b658d1 6a00                 push 0
00b658d3 e82842e2ff           call 0x989b00
00b658d8 8b0d146e4301         mov ecx, dword ptr [0x1436e14]
00b658de 8a81d8030000         mov al, byte ptr [ecx + 0x3d8]
00b658e4 a804                 test al, 4
00b658e6 8db1d8030000         lea esi, [ecx + 0x3d8]
00b658ec 8bf9                 mov edi, ecx
00b658ee 7435                 je 0xb65925
00b658f0 8b4104               mov eax, dword ptr [ecx + 4]
00b658f3 8b9040010000         mov edx, dword ptr [eax + 0x140]
00b658f9 8ba844010000         mov ebp, dword ptr [eax + 0x144]
00b658ff 0540010000           add eax, 0x140
00b65904 2bea                 sub ebp, edx
00b65906 c1fd04               sar ebp, 4
00b65909 85ed                 test ebp, ebp
00b6590b 7702                 ja 0xb6590f
00b6590d 8bd0                 mov edx, eax
00b6590f 8b6804               mov ebp, dword ptr [eax + 4]
00b65912 2b28                 sub ebp, dword ptr [eax]
00b65914 c1fd04               sar ebp, 4
00b65917 55                   push ebp
00b65918 52                   push edx
00b65919 8b5104               mov edx, dword ptr [ecx + 4]
00b6591c 8b4208               mov eax, dword ptr [edx + 8]
00b6591f 50                   push eax
00b65920 e80b60e2ff           call 0x98b930
00b65925 841e                 test byte ptr [esi], bl
00b65927 7407                 je 0xb65930
00b65929 8bcf                 mov ecx, edi
00b6592b e8f030e2ff           call 0x988a20
00b65930 f60602               test byte ptr [esi], 2
00b65933 7407                 je 0xb6593c
00b65935 8bcf                 mov ecx, edi
00b65937 e8b442e2ff           call 0x989bf0
00b6593c f60604               test byte ptr [esi], 4
00b6593f 7407                 je 0xb65948
00b65941 8bcf                 mov ecx, edi
00b65943 e8183ee2ff           call 0x989760
00b65948 f70600000200         test dword ptr [esi], 0x20000
00b6594e 7407                 je 0xb65957
00b65950 8bcf                 mov ecx, edi
00b65952 e8693ee2ff           call 0x9897c0
00b65957 8b06                 mov eax, dword ptr [esi]
00b65959 f6c404               test ah, 4
00b6595c 7407                 je 0xb65965
00b6595e 8bcf                 mov ecx, edi
00b65960 e8fb4de2ff           call 0x98a760
00b65965 8a87e8010000         mov al, byte ptr [edi + 0x1e8]
00b6596b 84c0                 test al, al
00b6596d 7507                 jne 0xb65976
00b6596f 8bcf                 mov ecx, edi
00b65971 e81a29e2ff           call 0x988290
00b65976 803e00               cmp byte ptr [esi], 0
00b65979 7907                 jns 0xb65982
00b6597b 8bcf                 mov ecx, edi
00b6597d e8ce30e2ff           call 0x988a50
00b65982 6840020000           push 0x240
00b65987 6844010000           push 0x144
00b6598c 684d010000           push 0x14d
00b65991 6a49                 push 0x49
00b65993 6a04                 push 4
00b65995 8d4c244c             lea ecx, [esp + 0x4c]
00b65999 51                   push ecx
00b6599a 8b0d186e4301         mov ecx, dword ptr [0x1436e18]
00b659a0 e8fb52eaff           call 0xa0aca0
00b659a5 8b0d08b53c01         mov ecx, dword ptr [0x13cb508]
00b659ab e890fee9ff           call 0xa05840
00b659b0 5f                   pop edi
00b659b1 5e                   pop esi
00b659b2 5d                   pop ebp
00b659b3 5b                   pop ebx
00b659b4 83c438               add esp, 0x38
00b659b7 c20400               ret 4
00b659ba 8b711c               mov esi, dword ptr [ecx + 0x1c]
00b659bd e98bf7ffff           jmp 0xb6514d
00b659c2 53                   push ebx
00b659c3 53                   push ebx
00b659c4 e8b784e9ff           call 0x9fde80
00b659c9 8b15986e4301         mov edx, dword ptr [0x1436e98]
00b659cf 8b442410             mov eax, dword ptr [esp + 0x10]
00b659d3 81c2d42a0000         add edx, 0x2ad4
00b659d9 40                   inc eax
00b659da 89542c38             mov dword ptr [esp + ebp + 0x38], edx
00b659de 89442410             mov dword ptr [esp + 0x10], eax
00b659e2 e9b5f7ffff           jmp 0xb6519c
00b659e7 6a44                 push 0x44
00b659e9 e82c900900           call 0xbfea1a
00b659ee 83c404               add esp, 4
00b659f1 3bc3                 cmp eax, ebx
00b659f3 740c                 je 0xb65a01
00b659f5 56                   push esi
00b659f6 8bc8                 mov ecx, eax
00b659f8 e88389e9ff           call 0x9fe380
00b659fd 8bf8                 mov edi, eax
00b659ff eb02                 jmp 0xb65a03
00b65a01 33ff                 xor edi, edi
00b65a03 8b4e08               mov ecx, dword ptr [esi + 8]
00b65a06 3bcb                 cmp ecx, ebx
00b65a08 740d                 je 0xb65a17
00b65a0a ff4904               dec dword ptr [ecx + 4]
00b65a0d 7505                 jne 0xb65a14
00b65a0f 8b01                 mov eax, dword ptr [ecx]
00b65a11 ff5004               call dword ptr [eax + 4]
00b65a14 895e08               mov dword ptr [esi + 8], ebx
00b65a17 53                   push ebx
00b65a18 897e08               mov dword ptr [esi + 8], edi
00b65a1b 53                   push ebx
00b65a1c 8bcf                 mov ecx, edi
00b65a1e eba4                 jmp 0xb659c4
