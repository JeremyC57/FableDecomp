00b52dd0 55                   push ebp
00b52dd1 8bec                 mov ebp, esp
00b52dd3 83ec34               sub esp, 0x34
00b52dd6 d981d4010000         fld dword ptr [ecx + 0x1d4]
00b52ddc 53                   push ebx
00b52ddd d80d9cf52201         fmul dword ptr [0x122f59c]
00b52de3 56                   push esi
00b52de4 57                   push edi
00b52de5 c745dc0000003f       mov dword ptr [ebp - 0x24], 0x3f000000
00b52dec d95dd8               fstp dword ptr [ebp - 0x28]
00b52def c745cc0000803f       mov dword ptr [ebp - 0x34], 0x3f800000
00b52df6 d981d8010000         fld dword ptr [ecx + 0x1d8]
00b52dfc d80d9cf52201         fmul dword ptr [0x122f59c]
00b52e02 d95dd4               fstp dword ptr [ebp - 0x2c]
00b52e05 d981dc010000         fld dword ptr [ecx + 0x1dc]
00b52e0b 8b4d08               mov ecx, dword ptr [ebp + 8]
00b52e0e d80d9cf52201         fmul dword ptr [0x122f59c]
00b52e14 d84108               fadd dword ptr [ecx + 8]
00b52e17 d95dd0               fstp dword ptr [ebp - 0x30]
00b52e1a d901                 fld dword ptr [ecx]
00b52e1c d865d8               fsub dword ptr [ebp - 0x28]
00b52e1f d95de0               fstp dword ptr [ebp - 0x20]
00b52e22 d945e0               fld dword ptr [ebp - 0x20]
00b52e25 d865dc               fsub dword ptr [ebp - 0x24]
00b52e28 db5de4               fistp dword ptr [ebp - 0x1c]
00b52e2b db45e4               fild dword ptr [ebp - 0x1c]
00b52e2e d845cc               fadd dword ptr [ebp - 0x34]
00b52e31 d95de8               fstp dword ptr [ebp - 0x18]
00b52e34 8b45e0               mov eax, dword ptr [ebp - 0x20]
00b52e37 3b45e8               cmp eax, dword ptr [ebp - 0x18]
00b52e3a 7503                 jne 0xb52e3f
00b52e3c ff45e4               inc dword ptr [ebp - 0x1c]
00b52e3f d94104               fld dword ptr [ecx + 4]
00b52e42 c745dc0000003f       mov dword ptr [ebp - 0x24], 0x3f000000
00b52e49 d865d4               fsub dword ptr [ebp - 0x2c]
00b52e4c c745cc0000803f       mov dword ptr [ebp - 0x34], 0x3f800000
00b52e53 d95de0               fstp dword ptr [ebp - 0x20]
00b52e56 d945e0               fld dword ptr [ebp - 0x20]
00b52e59 d865dc               fsub dword ptr [ebp - 0x24]
00b52e5c db5de8               fistp dword ptr [ebp - 0x18]
00b52e5f db45e8               fild dword ptr [ebp - 0x18]
00b52e62 d845cc               fadd dword ptr [ebp - 0x34]
00b52e65 d95dec               fstp dword ptr [ebp - 0x14]
00b52e68 8b45e0               mov eax, dword ptr [ebp - 0x20]
00b52e6b 3b45ec               cmp eax, dword ptr [ebp - 0x14]
00b52e6e 7503                 jne 0xb52e73
00b52e70 ff45e8               inc dword ptr [ebp - 0x18]
00b52e73 a18c6e4301           mov eax, dword ptr [0x1436e8c]
00b52e78 8b4818               mov ecx, dword ptr [eax + 0x18]
00b52e7b 8b5de4               mov ebx, dword ptr [ebp - 0x1c]
00b52e7e 8b7de8               mov edi, dword ptr [ebp - 0x18]
00b52e81 2bd9                 sub ebx, ecx
00b52e83 894de0               mov dword ptr [ebp - 0x20], ecx
00b52e86 8b481c               mov ecx, dword ptr [eax + 0x1c]
00b52e89 2bf9                 sub edi, ecx
00b52e8b 85db                 test ebx, ebx
00b52e8d 895dcc               mov dword ptr [ebp - 0x34], ebx
00b52e90 894ddc               mov dword ptr [ebp - 0x24], ecx
00b52e93 7c4d                 jl 0xb52ee2
00b52e95 85ff                 test edi, edi
00b52e97 7c49                 jl 0xb52ee2
00b52e99 8b5010               mov edx, dword ptr [eax + 0x10]
00b52e9c 8bca                 mov ecx, edx
00b52e9e c1e105               shl ecx, 5
00b52ea1 3bd9                 cmp ebx, ecx
00b52ea3 8955f8               mov dword ptr [ebp - 8], edx
00b52ea6 894df4               mov dword ptr [ebp - 0xc], ecx
00b52ea9 7d37                 jge 0xb52ee2
00b52eab 8b4814               mov ecx, dword ptr [eax + 0x14]
00b52eae c1e105               shl ecx, 5
00b52eb1 3bf9                 cmp edi, ecx
00b52eb3 894df0               mov dword ptr [ebp - 0x10], ecx
00b52eb6 7d2a                 jge 0xb52ee2
00b52eb8 8b480c               mov ecx, dword ptr [eax + 0xc]
00b52ebb 8bf7                 mov esi, edi
00b52ebd c1fe05               sar esi, 5
00b52ec0 0faff2               imul esi, edx
00b52ec3 8bd3                 mov edx, ebx
00b52ec5 c1fa05               sar edx, 5
00b52ec8 03f2                 add esi, edx
00b52eca 894dfc               mov dword ptr [ebp - 4], ecx
00b52ecd 0fbf0c71             movsx ecx, word ptr [ecx + esi*2]
00b52ed1 85c9                 test ecx, ecx
00b52ed3 7e0d                 jle 0xb52ee2
00b52ed5 8b5020               mov edx, dword ptr [eax + 0x20]
00b52ed8 8b0c8a               mov ecx, dword ptr [edx + ecx*4]
00b52edb 85c9                 test ecx, ecx
00b52edd 8955ec               mov dword ptr [ebp - 0x14], edx
00b52ee0 7563                 jne 0xb52f45
00b52ee2 8d73ff               lea esi, [ebx - 1]
00b52ee5 85f6                 test esi, esi
00b52ee7 0f8c9f000000         jl 0xb52f8c
00b52eed 85ff                 test edi, edi
00b52eef 0f8c97000000         jl 0xb52f8c
00b52ef5 8b5010               mov edx, dword ptr [eax + 0x10]
00b52ef8 8bca                 mov ecx, edx
00b52efa c1e105               shl ecx, 5
00b52efd 3bf1                 cmp esi, ecx
00b52eff 8955f8               mov dword ptr [ebp - 8], edx
00b52f02 894df4               mov dword ptr [ebp - 0xc], ecx
00b52f05 0f8d81000000         jge 0xb52f8c
00b52f0b 8b4814               mov ecx, dword ptr [eax + 0x14]
00b52f0e c1e105               shl ecx, 5
00b52f11 3bf9                 cmp edi, ecx
00b52f13 894df0               mov dword ptr [ebp - 0x10], ecx
00b52f16 7d74                 jge 0xb52f8c
00b52f18 8b480c               mov ecx, dword ptr [eax + 0xc]
00b52f1b 894dfc               mov dword ptr [ebp - 4], ecx
00b52f1e 8bcf                 mov ecx, edi
00b52f20 c1f905               sar ecx, 5
00b52f23 0fafca               imul ecx, edx
00b52f26 8bd6                 mov edx, esi
00b52f28 c1fa05               sar edx, 5
00b52f2b 03ca                 add ecx, edx
00b52f2d 8b55fc               mov edx, dword ptr [ebp - 4]
00b52f30 0fbf0c4a             movsx ecx, word ptr [edx + ecx*2]
00b52f34 85c9                 test ecx, ecx
00b52f36 7e54                 jle 0xb52f8c
00b52f38 8b5020               mov edx, dword ptr [eax + 0x20]
00b52f3b 8b0c8a               mov ecx, dword ptr [edx + ecx*4]
00b52f3e 85c9                 test ecx, ecx
00b52f40 8955ec               mov dword ptr [ebp - 0x14], edx
00b52f43 7447                 je 0xb52f8c
00b52f45 0fb74162             movzx eax, word ptr [ecx + 0x62]
00b52f49 8b55e8               mov edx, dword ptr [ebp - 0x18]
00b52f4c 0fb77160             movzx esi, word ptr [ecx + 0x60]
00b52f50 2bd0                 sub edx, eax
00b52f52 8b45e4               mov eax, dword ptr [ebp - 0x1c]
00b52f55 2bc6                 sub eax, esi
00b52f57 0fb7715c             movzx esi, word ptr [ecx + 0x5c]
00b52f5b 3bc6                 cmp eax, esi
00b52f5d 0f8d6b010000         jge 0xb530ce
00b52f63 0fb7715e             movzx esi, word ptr [ecx + 0x5e]
00b52f67 3bd6                 cmp edx, esi
00b52f69 0f8d5f010000         jge 0xb530ce
00b52f6f 85c0                 test eax, eax
00b52f71 0f8c57010000         jl 0xb530ce
00b52f77 85d2                 test edx, edx
00b52f79 0f8c4f010000         jl 0xb530ce
00b52f7f 8b4108               mov eax, dword ptr [ecx + 8]
00b52f82 85c0                 test eax, eax
00b52f84 0f95c0               setne al
00b52f87 e944010000           jmp 0xb530d0
00b52f8c 85db                 test ebx, ebx
00b52f8e 8d4fff               lea ecx, [edi - 1]
00b52f91 0f8c9f000000         jl 0xb53036
00b52f97 85c9                 test ecx, ecx
00b52f99 0f8c97000000         jl 0xb53036
00b52f9f 8b5010               mov edx, dword ptr [eax + 0x10]
00b52fa2 8955f8               mov dword ptr [ebp - 8], edx
00b52fa5 c1e205               shl edx, 5
00b52fa8 3bda                 cmp ebx, edx
00b52faa 8955f4               mov dword ptr [ebp - 0xc], edx
00b52fad 0f8d83000000         jge 0xb53036
00b52fb3 8b5014               mov edx, dword ptr [eax + 0x14]
00b52fb6 c1e205               shl edx, 5
00b52fb9 3bca                 cmp ecx, edx
00b52fbb 8955f0               mov dword ptr [ebp - 0x10], edx
00b52fbe 7d76                 jge 0xb53036
00b52fc0 8b500c               mov edx, dword ptr [eax + 0xc]
00b52fc3 8b5dcc               mov ebx, dword ptr [ebp - 0x34]
00b52fc6 8955fc               mov dword ptr [ebp - 4], edx
00b52fc9 8bd1                 mov edx, ecx
00b52fcb c1fa05               sar edx, 5
00b52fce 0faf55f8             imul edx, dword ptr [ebp - 8]
00b52fd2 c1fb05               sar ebx, 5
00b52fd5 03d3                 add edx, ebx
00b52fd7 8b5dfc               mov ebx, dword ptr [ebp - 4]
00b52fda 0fbf1453             movsx edx, word ptr [ebx + edx*2]
00b52fde 85d2                 test edx, edx
00b52fe0 7e51                 jle 0xb53033
00b52fe2 8b5820               mov ebx, dword ptr [eax + 0x20]
00b52fe5 8b1493               mov edx, dword ptr [ebx + edx*4]
00b52fe8 85d2                 test edx, edx
00b52fea 895dec               mov dword ptr [ebp - 0x14], ebx
00b52fed 7444                 je 0xb53033
00b52fef 0fb74262             movzx eax, word ptr [edx + 0x62]
00b52ff3 8b4de8               mov ecx, dword ptr [ebp - 0x18]
00b52ff6 0fb77260             movzx esi, word ptr [edx + 0x60]
00b52ffa 2bc8                 sub ecx, eax
00b52ffc 8b45e4               mov eax, dword ptr [ebp - 0x1c]
00b52fff 2bc6                 sub eax, esi
00b53001 0fb7725c             movzx esi, word ptr [edx + 0x5c]
00b53005 3bc6                 cmp eax, esi
00b53007 7d20                 jge 0xb53029
00b53009 0fb7725e             movzx esi, word ptr [edx + 0x5e]
00b5300d 3bce                 cmp ecx, esi
00b5300f 7d18                 jge 0xb53029
00b53011 85c0                 test eax, eax
00b53013 7c14                 jl 0xb53029
00b53015 85c9                 test ecx, ecx
00b53017 7c10                 jl 0xb53029
00b53019 8b4208               mov eax, dword ptr [edx + 8]
00b5301c 8b5dcc               mov ebx, dword ptr [ebp - 0x34]
00b5301f 85c0                 test eax, eax
00b53021 0f95c0               setne al
00b53024 e9a7000000           jmp 0xb530d0
00b53029 8b5dcc               mov ebx, dword ptr [ebp - 0x34]
00b5302c 32c0                 xor al, al
00b5302e e99d000000           jmp 0xb530d0
00b53033 8b5dcc               mov ebx, dword ptr [ebp - 0x34]
00b53036 85f6                 test esi, esi
00b53038 0f8c100f0000         jl 0xb53f4e
00b5303e 85c9                 test ecx, ecx
00b53040 0f8c080f0000         jl 0xb53f4e
00b53046 8b5010               mov edx, dword ptr [eax + 0x10]
00b53049 8955f8               mov dword ptr [ebp - 8], edx
00b5304c c1e205               shl edx, 5
00b5304f 3bf2                 cmp esi, edx
00b53051 8955f4               mov dword ptr [ebp - 0xc], edx
00b53054 0f8df40e0000         jge 0xb53f4e
00b5305a 8b5014               mov edx, dword ptr [eax + 0x14]
00b5305d c1e205               shl edx, 5
00b53060 3bca                 cmp ecx, edx
00b53062 8955f0               mov dword ptr [ebp - 0x10], edx
00b53065 0f8de30e0000         jge 0xb53f4e
00b5306b 8b500c               mov edx, dword ptr [eax + 0xc]
00b5306e c1f905               sar ecx, 5
00b53071 0faf4df8             imul ecx, dword ptr [ebp - 8]
00b53075 c1fe05               sar esi, 5
00b53078 03ce                 add ecx, esi
00b5307a 0fbf0c4a             movsx ecx, word ptr [edx + ecx*2]
00b5307e 85c9                 test ecx, ecx
00b53080 8955fc               mov dword ptr [ebp - 4], edx
00b53083 0f8ec50e0000         jle 0xb53f4e
00b53089 8b4020               mov eax, dword ptr [eax + 0x20]
00b5308c 8945ec               mov dword ptr [ebp - 0x14], eax
00b5308f 8b0488               mov eax, dword ptr [eax + ecx*4]
00b53092 85c0                 test eax, eax
00b53094 0f84b40e0000         je 0xb53f4e
00b5309a 0fb74862             movzx ecx, word ptr [eax + 0x62]
00b5309e 8b55e8               mov edx, dword ptr [ebp - 0x18]
00b530a1 0fb77060             movzx esi, word ptr [eax + 0x60]
00b530a5 2bd1                 sub edx, ecx
00b530a7 8b4de4               mov ecx, dword ptr [ebp - 0x1c]
00b530aa 2bce                 sub ecx, esi
00b530ac 0fb7705c             movzx esi, word ptr [eax + 0x5c]
00b530b0 3bce                 cmp ecx, esi
00b530b2 7d1a                 jge 0xb530ce
00b530b4 0fb7705e             movzx esi, word ptr [eax + 0x5e]
00b530b8 3bd6                 cmp edx, esi
00b530ba 7d12                 jge 0xb530ce
00b530bc 85c9                 test ecx, ecx
00b530be 7c0e                 jl 0xb530ce
00b530c0 85d2                 test edx, edx
00b530c2 7c0a                 jl 0xb530ce
00b530c4 8b4808               mov ecx, dword ptr [eax + 8]
00b530c7 85c9                 test ecx, ecx
00b530c9 0f95c0               setne al
00b530cc eb02                 jmp 0xb530d0
00b530ce 32c0                 xor al, al
00b530d0 84c0                 test al, al
00b530d2 0f84760e0000         je 0xb53f4e
00b530d8 85db                 test ebx, ebx
00b530da 0f8c62010000         jl 0xb53242
00b530e0 85ff                 test edi, edi
00b530e2 0f8c5a010000         jl 0xb53242
00b530e8 3b5df4               cmp ebx, dword ptr [ebp - 0xc]
00b530eb 0f8d51010000         jge 0xb53242
00b530f1 3b7df0               cmp edi, dword ptr [ebp - 0x10]
00b530f4 0f8d48010000         jge 0xb53242
00b530fa 8b55fc               mov edx, dword ptr [ebp - 4]
00b530fd c1ff05               sar edi, 5
00b53100 0faf7df8             imul edi, dword ptr [ebp - 8]
00b53104 c1fb05               sar ebx, 5
00b53107 03fb                 add edi, ebx
00b53109 0fbf047a             movsx eax, word ptr [edx + edi*2]
00b5310d 85c0                 test eax, eax
00b5310f 0f8e2d010000         jle 0xb53242
00b53115 8b4dec               mov ecx, dword ptr [ebp - 0x14]
00b53118 8b0481               mov eax, dword ptr [ecx + eax*4]
00b5311b 85c0                 test eax, eax
00b5311d 8945cc               mov dword ptr [ebp - 0x34], eax
00b53120 0f841c010000         je 0xb53242
00b53126 0fb75062             movzx edx, word ptr [eax + 0x62]
00b5312a 8b4de8               mov ecx, dword ptr [ebp - 0x18]
00b5312d 0fb77060             movzx esi, word ptr [eax + 0x60]
00b53131 8b7de4               mov edi, dword ptr [ebp - 0x1c]
00b53134 2bca                 sub ecx, edx
00b53136 894de8               mov dword ptr [ebp - 0x18], ecx
00b53139 0fb7485c             movzx ecx, word ptr [eax + 0x5c]
00b5313d 2bfe                 sub edi, esi
00b5313f 3bf9                 cmp edi, ecx
00b53141 7d1e                 jge 0xb53161
00b53143 0fb7485e             movzx ecx, word ptr [eax + 0x5e]
00b53147 8b5de8               mov ebx, dword ptr [ebp - 0x18]
00b5314a 3bd9                 cmp ebx, ecx
00b5314c 7d13                 jge 0xb53161
00b5314e 85ff                 test edi, edi
00b53150 7c0f                 jl 0xb53161
00b53152 85db                 test ebx, ebx
00b53154 7c0b                 jl 0xb53161
00b53156 8b4808               mov ecx, dword ptr [eax + 8]
00b53159 85c9                 test ecx, ecx
00b5315b 0f85b6000000         jne 0xb53217
00b53161 8b5de0               mov ebx, dword ptr [ebp - 0x20]
00b53164 8bce                 mov ecx, esi
00b53166 2bcb                 sub ecx, ebx
00b53168 8b5ddc               mov ebx, dword ptr [ebp - 0x24]
00b5316b 8bc2                 mov eax, edx
00b5316d 2bc3                 sub eax, ebx
00b5316f 8b5de8               mov ebx, dword ptr [ebp - 0x18]
00b53172 03cf                 add ecx, edi
00b53174 03c3                 add eax, ebx
00b53176 85c9                 test ecx, ecx
00b53178 7c6d                 jl 0xb531e7
00b5317a 85c0                 test eax, eax
00b5317c 7c69                 jl 0xb531e7
00b5317e 3b4df4               cmp ecx, dword ptr [ebp - 0xc]
00b53181 7d61                 jge 0xb531e4
00b53183 3b45f0               cmp eax, dword ptr [ebp - 0x10]
00b53186 7d5c                 jge 0xb531e4
00b53188 c1f805               sar eax, 5
00b5318b 0faf45f8             imul eax, dword ptr [ebp - 8]
00b5318f c1f905               sar ecx, 5
00b53192 03c1                 add eax, ecx
00b53194 8b4dfc               mov ecx, dword ptr [ebp - 4]
00b53197 0fbf0441             movsx eax, word ptr [ecx + eax*2]
00b5319b 85c0                 test eax, eax
00b5319d 7e45                 jle 0xb531e4
00b5319f 8b4dec               mov ecx, dword ptr [ebp - 0x14]
00b531a2 8b0c81               mov ecx, dword ptr [ecx + eax*4]
00b531a5 85c9                 test ecx, ecx
00b531a7 743b                 je 0xb531e4
00b531a9 0fb74162             movzx eax, word ptr [ecx + 0x62]
00b531ad 8b5de8               mov ebx, dword ptr [ebp - 0x18]
00b531b0 2bd0                 sub edx, eax
00b531b2 0fb74160             movzx eax, word ptr [ecx + 0x60]
00b531b6 2bf0                 sub esi, eax
00b531b8 0fb7415c             movzx eax, word ptr [ecx + 0x5c]
00b531bc 03f7                 add esi, edi
00b531be 03d3                 add edx, ebx
00b531c0 3bf0                 cmp esi, eax
00b531c2 7d23                 jge 0xb531e7
00b531c4 0fb7415e             movzx eax, word ptr [ecx + 0x5e]
00b531c8 3bd0                 cmp edx, eax
00b531ca 7d1b                 jge 0xb531e7
00b531cc 85f6                 test esi, esi
00b531ce 7c17                 jl 0xb531e7
00b531d0 85d2                 test edx, edx
00b531d2 7c13                 jl 0xb531e7
00b531d4 8b4108               mov eax, dword ptr [ecx + 8]
00b531d7 85c0                 test eax, eax
00b531d9 740c                 je 0xb531e7
00b531db 52                   push edx
00b531dc 56                   push esi
00b531dd e8de07ffff           call 0xb439c0
00b531e2 eb64                 jmp 0xb53248
00b531e4 8b5de8               mov ebx, dword ptr [ebp - 0x18]
00b531e7 85ff                 test edi, edi
00b531e9 8b45cc               mov eax, dword ptr [ebp - 0x34]
00b531ec 7d04                 jge 0xb531f2
00b531ee 33ff                 xor edi, edi
00b531f0 eb0b                 jmp 0xb531fd
00b531f2 0fb7485c             movzx ecx, word ptr [eax + 0x5c]
00b531f6 49                   dec ecx
00b531f7 3bf9                 cmp edi, ecx
00b531f9 7e02                 jle 0xb531fd
00b531fb 8bf9                 mov edi, ecx
00b531fd 85db                 test ebx, ebx
00b531ff 7d04                 jge 0xb53205
00b53201 33db                 xor ebx, ebx
00b53203 eb0b                 jmp 0xb53210
00b53205 0fb7485e             movzx ecx, word ptr [eax + 0x5e]
00b53209 49                   dec ecx
00b5320a 3bd9                 cmp ebx, ecx
00b5320c 7e02                 jle 0xb53210
00b5320e 8bd9                 mov ebx, ecx
00b53210 8b4808               mov ecx, dword ptr [eax + 8]
00b53213 85c9                 test ecx, ecx
00b53215 742b                 je 0xb53242
00b53217 8b4008               mov eax, dword ptr [eax + 8]
00b5321a 8b10                 mov edx, dword ptr [eax]
00b5321c 680000803f           push 0x3f800000
00b53221 53                   push ebx
00b53222 57                   push edi
00b53223 8bc8                 mov ecx, eax
00b53225 ff5214               call dword ptr [edx + 0x14]
00b53228 d80db0712501         fmul dword ptr [0x12571b0]
00b5322e d95dcc               fstp dword ptr [ebp - 0x34]
00b53231 d945cc               fld dword ptr [ebp - 0x34]
00b53234 db5ddc               fistp dword ptr [ebp - 0x24]
00b53237 db45dc               fild dword ptr [ebp - 0x24]
00b5323a d80d341f2a01         fmul dword ptr [0x12a1f34]
00b53240 eb06                 jmp 0xb53248
00b53242 d905dcde2201         fld dword ptr [0x122dedc]
00b53248 d85dd0               fcomp dword ptr [ebp - 0x30]
00b5324b dfe0                 fnstsw ax
00b5324d f6c405               test ah, 5
00b53250 0f8bf80c0000         jnp 0xb53f4e
00b53256 8b4d08               mov ecx, dword ptr [ebp + 8]
00b53259 d901                 fld dword ptr [ecx]
00b5325b c745dc0000003f       mov dword ptr [ebp - 0x24], 0x3f000000
00b53262 d865d8               fsub dword ptr [ebp - 0x28]
00b53265 c745cc0000803f       mov dword ptr [ebp - 0x34], 0x3f800000
00b5326c d95de0               fstp dword ptr [ebp - 0x20]
00b5326f d945e0               fld dword ptr [ebp - 0x20]
00b53272 d865dc               fsub dword ptr [ebp - 0x24]
00b53275 db5de8               fistp dword ptr [ebp - 0x18]
00b53278 db45e8               fild dword ptr [ebp - 0x18]
00b5327b d845cc               fadd dword ptr [ebp - 0x34]
00b5327e d95de4               fstp dword ptr [ebp - 0x1c]
00b53281 8b45e0               mov eax, dword ptr [ebp - 0x20]
00b53284 3b45e4               cmp eax, dword ptr [ebp - 0x1c]
00b53287 7503                 jne 0xb5328c
00b53289 ff45e8               inc dword ptr [ebp - 0x18]
00b5328c d945d4               fld dword ptr [ebp - 0x2c]
00b5328f c745dc0000003f       mov dword ptr [ebp - 0x24], 0x3f000000
00b53296 d84104               fadd dword ptr [ecx + 4]
00b53299 c745cc0000803f       mov dword ptr [ebp - 0x34], 0x3f800000
00b532a0 d95de0               fstp dword ptr [ebp - 0x20]
00b532a3 d945e0               fld dword ptr [ebp - 0x20]
00b532a6 d845dc               fadd dword ptr [ebp - 0x24]
00b532a9 db5de4               fistp dword ptr [ebp - 0x1c]
00b532ac db45e4               fild dword ptr [ebp - 0x1c]
00b532af d865cc               fsub dword ptr [ebp - 0x34]
00b532b2 d95dec               fstp dword ptr [ebp - 0x14]
00b532b5 8b45e0               mov eax, dword ptr [ebp - 0x20]
00b532b8 3b45ec               cmp eax, dword ptr [ebp - 0x14]
00b532bb 7503                 jne 0xb532c0
00b532bd ff4de4               dec dword ptr [ebp - 0x1c]
00b532c0 a18c6e4301           mov eax, dword ptr [0x1436e8c]
00b532c5 8b4818               mov ecx, dword ptr [eax + 0x18]
00b532c8 8b5de8               mov ebx, dword ptr [ebp - 0x18]
00b532cb 8b7de4               mov edi, dword ptr [ebp - 0x1c]
00b532ce 2bd9                 sub ebx, ecx
00b532d0 894de0               mov dword ptr [ebp - 0x20], ecx
00b532d3 8b481c               mov ecx, dword ptr [eax + 0x1c]
00b532d6 2bf9                 sub edi, ecx
00b532d8 85db                 test ebx, ebx
00b532da 895dcc               mov dword ptr [ebp - 0x34], ebx
00b532dd 894ddc               mov dword ptr [ebp - 0x24], ecx
00b532e0 7c4d                 jl 0xb5332f
00b532e2 85ff                 test edi, edi
00b532e4 7c49                 jl 0xb5332f
00b532e6 8b5010               mov edx, dword ptr [eax + 0x10]
00b532e9 8bca                 mov ecx, edx
00b532eb c1e105               shl ecx, 5
00b532ee 3bd9                 cmp ebx, ecx
00b532f0 8955f8               mov dword ptr [ebp - 8], edx
00b532f3 894df4               mov dword ptr [ebp - 0xc], ecx
00b532f6 7d37                 jge 0xb5332f
00b532f8 8b4814               mov ecx, dword ptr [eax + 0x14]
00b532fb c1e105               shl ecx, 5
00b532fe 3bf9                 cmp edi, ecx
00b53300 894df0               mov dword ptr [ebp - 0x10], ecx
00b53303 7d2a                 jge 0xb5332f
00b53305 8b480c               mov ecx, dword ptr [eax + 0xc]
00b53308 8bf7                 mov esi, edi
00b5330a c1fe05               sar esi, 5
00b5330d 0faff2               imul esi, edx
00b53310 8bd3                 mov edx, ebx
00b53312 c1fa05               sar edx, 5
00b53315 03f2                 add esi, edx
00b53317 894dfc               mov dword ptr [ebp - 4], ecx
00b5331a 0fbf0c71             movsx ecx, word ptr [ecx + esi*2]
00b5331e 85c9                 test ecx, ecx
00b53320 7e0d                 jle 0xb5332f
00b53322 8b5020               mov edx, dword ptr [eax + 0x20]
00b53325 8b0c8a               mov ecx, dword ptr [edx + ecx*4]
00b53328 85c9                 test ecx, ecx
00b5332a 8955ec               mov dword ptr [ebp - 0x14], edx
00b5332d 7563                 jne 0xb53392
00b5332f 8d73ff               lea esi, [ebx - 1]
00b53332 85f6                 test esi, esi
00b53334 0f8c9f000000         jl 0xb533d9
00b5333a 85ff                 test edi, edi
00b5333c 0f8c97000000         jl 0xb533d9
00b53342 8b5010               mov edx, dword ptr [eax + 0x10]
00b53345 8bca                 mov ecx, edx
00b53347 c1e105               shl ecx, 5
00b5334a 3bf1                 cmp esi, ecx
00b5334c 8955f8               mov dword ptr [ebp - 8], edx
00b5334f 894df4               mov dword ptr [ebp - 0xc], ecx
00b53352 0f8d81000000         jge 0xb533d9
00b53358 8b4814               mov ecx, dword ptr [eax + 0x14]
00b5335b c1e105               shl ecx, 5
00b5335e 3bf9                 cmp edi, ecx
00b53360 894df0               mov dword ptr [ebp - 0x10], ecx
00b53363 7d74                 jge 0xb533d9
00b53365 8b480c               mov ecx, dword ptr [eax + 0xc]
00b53368 894dfc               mov dword ptr [ebp - 4], ecx
00b5336b 8bcf                 mov ecx, edi
00b5336d c1f905               sar ecx, 5
00b53370 0fafca               imul ecx, edx
00b53373 8bd6                 mov edx, esi
00b53375 c1fa05               sar edx, 5
00b53378 03ca                 add ecx, edx
00b5337a 8b55fc               mov edx, dword ptr [ebp - 4]
00b5337d 0fbf0c4a             movsx ecx, word ptr [edx + ecx*2]
00b53381 85c9                 test ecx, ecx
00b53383 7e54                 jle 0xb533d9
00b53385 8b5020               mov edx, dword ptr [eax + 0x20]
00b53388 8b0c8a               mov ecx, dword ptr [edx + ecx*4]
00b5338b 85c9                 test ecx, ecx
00b5338d 8955ec               mov dword ptr [ebp - 0x14], edx
00b53390 7447                 je 0xb533d9
00b53392 0fb74162             movzx eax, word ptr [ecx + 0x62]
00b53396 8b55e4               mov edx, dword ptr [ebp - 0x1c]
00b53399 0fb77160             movzx esi, word ptr [ecx + 0x60]
00b5339d 2bd0                 sub edx, eax
00b5339f 8b45e8               mov eax, dword ptr [ebp - 0x18]
00b533a2 2bc6                 sub eax, esi
00b533a4 0fb7715c             movzx esi, word ptr [ecx + 0x5c]
00b533a8 3bc6                 cmp eax, esi
00b533aa 0f8d6b010000         jge 0xb5351b
00b533b0 0fb7715e             movzx esi, word ptr [ecx + 0x5e]
00b533b4 3bd6                 cmp edx, esi
00b533b6 0f8d5f010000         jge 0xb5351b
00b533bc 85c0                 test eax, eax
00b533be 0f8c57010000         jl 0xb5351b
00b533c4 85d2                 test edx, edx
00b533c6 0f8c4f010000         jl 0xb5351b
00b533cc 8b4108               mov eax, dword ptr [ecx + 8]
00b533cf 85c0                 test eax, eax
00b533d1 0f95c0               setne al
00b533d4 e944010000           jmp 0xb5351d
00b533d9 85db                 test ebx, ebx
00b533db 8d4fff               lea ecx, [edi - 1]
00b533de 0f8c9f000000         jl 0xb53483
00b533e4 85c9                 test ecx, ecx
00b533e6 0f8c97000000         jl 0xb53483
00b533ec 8b5010               mov edx, dword ptr [eax + 0x10]
00b533ef 8955f8               mov dword ptr [ebp - 8], edx
00b533f2 c1e205               shl edx, 5
00b533f5 3bda                 cmp ebx, edx
00b533f7 8955f4               mov dword ptr [ebp - 0xc], edx
00b533fa 0f8d83000000         jge 0xb53483
00b53400 8b5014               mov edx, dword ptr [eax + 0x14]
00b53403 c1e205               shl edx, 5
00b53406 3bca                 cmp ecx, edx
00b53408 8955f0               mov dword ptr [ebp - 0x10], edx
00b5340b 7d76                 jge 0xb53483
00b5340d 8b500c               mov edx, dword ptr [eax + 0xc]
00b53410 8b5dcc               mov ebx, dword ptr [ebp - 0x34]
00b53413 8955fc               mov dword ptr [ebp - 4], edx
00b53416 8bd1                 mov edx, ecx
00b53418 c1fa05               sar edx, 5
00b5341b 0faf55f8             imul edx, dword ptr [ebp - 8]
00b5341f c1fb05               sar ebx, 5
00b53422 03d3                 add edx, ebx
00b53424 8b5dfc               mov ebx, dword ptr [ebp - 4]
00b53427 0fbf1453             movsx edx, word ptr [ebx + edx*2]
00b5342b 85d2                 test edx, edx
00b5342d 7e51                 jle 0xb53480
00b5342f 8b5820               mov ebx, dword ptr [eax + 0x20]
00b53432 8b1493               mov edx, dword ptr [ebx + edx*4]
00b53435 85d2                 test edx, edx
00b53437 895dec               mov dword ptr [ebp - 0x14], ebx
00b5343a 7444                 je 0xb53480
00b5343c 0fb74262             movzx eax, word ptr [edx + 0x62]
00b53440 8b4de4               mov ecx, dword ptr [ebp - 0x1c]
00b53443 0fb77260             movzx esi, word ptr [edx + 0x60]
00b53447 2bc8                 sub ecx, eax
00b53449 8b45e8               mov eax, dword ptr [ebp - 0x18]
00b5344c 2bc6                 sub eax, esi
00b5344e 0fb7725c             movzx esi, word ptr [edx + 0x5c]
00b53452 3bc6                 cmp eax, esi
00b53454 7d20                 jge 0xb53476
00b53456 0fb7725e             movzx esi, word ptr [edx + 0x5e]
00b5345a 3bce                 cmp ecx, esi
00b5345c 7d18                 jge 0xb53476
00b5345e 85c0                 test eax, eax
00b53460 7c14                 jl 0xb53476
00b53462 85c9                 test ecx, ecx
00b53464 7c10                 jl 0xb53476
00b53466 8b4208               mov eax, dword ptr [edx + 8]
00b53469 8b5dcc               mov ebx, dword ptr [ebp - 0x34]
00b5346c 85c0                 test eax, eax
00b5346e 0f95c0               setne al
00b53471 e9a7000000           jmp 0xb5351d
00b53476 8b5dcc               mov ebx, dword ptr [ebp - 0x34]
00b53479 32c0                 xor al, al
00b5347b e99d000000           jmp 0xb5351d
00b53480 8b5dcc               mov ebx, dword ptr [ebp - 0x34]
00b53483 85f6                 test esi, esi
00b53485 0f8cc30a0000         jl 0xb53f4e
00b5348b 85c9                 test ecx, ecx
00b5348d 0f8cbb0a0000         jl 0xb53f4e
00b53493 8b5010               mov edx, dword ptr [eax + 0x10]
00b53496 8955f8               mov dword ptr [ebp - 8], edx
00b53499 c1e205               shl edx, 5
00b5349c 3bf2                 cmp esi, edx
00b5349e 8955f4               mov dword ptr [ebp - 0xc], edx
00b534a1 0f8da70a0000         jge 0xb53f4e
00b534a7 8b5014               mov edx, dword ptr [eax + 0x14]
00b534aa c1e205               shl edx, 5
00b534ad 3bca                 cmp ecx, edx
00b534af 8955f0               mov dword ptr [ebp - 0x10], edx
00b534b2 0f8d960a0000         jge 0xb53f4e
00b534b8 8b500c               mov edx, dword ptr [eax + 0xc]
00b534bb c1f905               sar ecx, 5
00b534be 0faf4df8             imul ecx, dword ptr [ebp - 8]
00b534c2 c1fe05               sar esi, 5
00b534c5 03ce                 add ecx, esi
00b534c7 0fbf0c4a             movsx ecx, word ptr [edx + ecx*2]
00b534cb 85c9                 test ecx, ecx
00b534cd 8955fc               mov dword ptr [ebp - 4], edx
00b534d0 0f8e780a0000         jle 0xb53f4e
00b534d6 8b4020               mov eax, dword ptr [eax + 0x20]
00b534d9 8945ec               mov dword ptr [ebp - 0x14], eax
00b534dc 8b0488               mov eax, dword ptr [eax + ecx*4]
00b534df 85c0                 test eax, eax
00b534e1 0f84670a0000         je 0xb53f4e
00b534e7 0fb74862             movzx ecx, word ptr [eax + 0x62]
00b534eb 8b55e4               mov edx, dword ptr [ebp - 0x1c]
00b534ee 0fb77060             movzx esi, word ptr [eax + 0x60]
00b534f2 2bd1                 sub edx, ecx
00b534f4 8b4de8               mov ecx, dword ptr [ebp - 0x18]
00b534f7 2bce                 sub ecx, esi
00b534f9 0fb7705c             movzx esi, word ptr [eax + 0x5c]
00b534fd 3bce                 cmp ecx, esi
00b534ff 7d1a                 jge 0xb5351b
00b53501 0fb7705e             movzx esi, word ptr [eax + 0x5e]
00b53505 3bd6                 cmp edx, esi
00b53507 7d12                 jge 0xb5351b
00b53509 85c9                 test ecx, ecx
00b5350b 7c0e                 jl 0xb5351b
00b5350d 85d2                 test edx, edx
00b5350f 7c0a                 jl 0xb5351b
00b53511 8b4808               mov ecx, dword ptr [eax + 8]
00b53514 85c9                 test ecx, ecx
00b53516 0f95c0               setne al
00b53519 eb02                 jmp 0xb5351d
00b5351b 32c0                 xor al, al
00b5351d 84c0                 test al, al
00b5351f 0f84290a0000         je 0xb53f4e
00b53525 85db                 test ebx, ebx
00b53527 0f8c62010000         jl 0xb5368f
00b5352d 85ff                 test edi, edi
00b5352f 0f8c5a010000         jl 0xb5368f
00b53535 3b5df4               cmp ebx, dword ptr [ebp - 0xc]
00b53538 0f8d51010000         jge 0xb5368f
00b5353e 3b7df0               cmp edi, dword ptr [ebp - 0x10]
00b53541 0f8d48010000         jge 0xb5368f
00b53547 8b55fc               mov edx, dword ptr [ebp - 4]
00b5354a c1ff05               sar edi, 5
00b5354d 0faf7df8             imul edi, dword ptr [ebp - 8]
00b53551 c1fb05               sar ebx, 5
00b53554 03fb                 add edi, ebx
00b53556 0fbf047a             movsx eax, word ptr [edx + edi*2]
00b5355a 85c0                 test eax, eax
00b5355c 0f8e2d010000         jle 0xb5368f
00b53562 8b4dec               mov ecx, dword ptr [ebp - 0x14]
00b53565 8b0481               mov eax, dword ptr [ecx + eax*4]
00b53568 85c0                 test eax, eax
00b5356a 8945cc               mov dword ptr [ebp - 0x34], eax
00b5356d 0f841c010000         je 0xb5368f
00b53573 0fb75062             movzx edx, word ptr [eax + 0x62]
00b53577 8b4de4               mov ecx, dword ptr [ebp - 0x1c]
00b5357a 0fb77060             movzx esi, word ptr [eax + 0x60]
00b5357e 8b7de8               mov edi, dword ptr [ebp - 0x18]
00b53581 2bca                 sub ecx, edx
00b53583 894de4               mov dword ptr [ebp - 0x1c], ecx
00b53586 0fb7485c             movzx ecx, word ptr [eax + 0x5c]
00b5358a 2bfe                 sub edi, esi
00b5358c 3bf9                 cmp edi, ecx
00b5358e 7d1e                 jge 0xb535ae
00b53590 0fb7485e             movzx ecx, word ptr [eax + 0x5e]
00b53594 8b5de4               mov ebx, dword ptr [ebp - 0x1c]
00b53597 3bd9                 cmp ebx, ecx
00b53599 7d13                 jge 0xb535ae
00b5359b 85ff                 test edi, edi
00b5359d 7c0f                 jl 0xb535ae
00b5359f 85db                 test ebx, ebx
00b535a1 7c0b                 jl 0xb535ae
00b535a3 8b4808               mov ecx, dword ptr [eax + 8]
00b535a6 85c9                 test ecx, ecx
00b535a8 0f85b6000000         jne 0xb53664
00b535ae 8b5de0               mov ebx, dword ptr [ebp - 0x20]
00b535b1 8bce                 mov ecx, esi
00b535b3 2bcb                 sub ecx, ebx
00b535b5 8b5ddc               mov ebx, dword ptr [ebp - 0x24]
00b535b8 8bc2                 mov eax, edx
00b535ba 2bc3                 sub eax, ebx
00b535bc 8b5de4               mov ebx, dword ptr [ebp - 0x1c]
00b535bf 03cf                 add ecx, edi
00b535c1 03c3                 add eax, ebx
00b535c3 85c9                 test ecx, ecx
00b535c5 7c6d                 jl 0xb53634
00b535c7 85c0                 test eax, eax
00b535c9 7c69                 jl 0xb53634
00b535cb 3b4df4               cmp ecx, dword ptr [ebp - 0xc]
00b535ce 7d61                 jge 0xb53631
00b535d0 3b45f0               cmp eax, dword ptr [ebp - 0x10]
00b535d3 7d5c                 jge 0xb53631
00b535d5 c1f805               sar eax, 5
00b535d8 0faf45f8             imul eax, dword ptr [ebp - 8]
00b535dc c1f905               sar ecx, 5
00b535df 03c1                 add eax, ecx
00b535e1 8b4dfc               mov ecx, dword ptr [ebp - 4]
00b535e4 0fbf0441             movsx eax, word ptr [ecx + eax*2]
00b535e8 85c0                 test eax, eax
00b535ea 7e45                 jle 0xb53631
00b535ec 8b4dec               mov ecx, dword ptr [ebp - 0x14]
00b535ef 8b0c81               mov ecx, dword ptr [ecx + eax*4]
00b535f2 85c9                 test ecx, ecx
00b535f4 743b                 je 0xb53631
00b535f6 0fb74162             movzx eax, word ptr [ecx + 0x62]
00b535fa 8b5de4               mov ebx, dword ptr [ebp - 0x1c]
00b535fd 2bd0                 sub edx, eax
00b535ff 0fb74160             movzx eax, word ptr [ecx + 0x60]
00b53603 2bf0                 sub esi, eax
00b53605 0fb7415c             movzx eax, word ptr [ecx + 0x5c]
00b53609 03f7                 add esi, edi
00b5360b 03d3                 add edx, ebx
00b5360d 3bf0                 cmp esi, eax
00b5360f 7d23                 jge 0xb53634
00b53611 0fb7415e             movzx eax, word ptr [ecx + 0x5e]
00b53615 3bd0                 cmp edx, eax
00b53617 7d1b                 jge 0xb53634
00b53619 85f6                 test esi, esi
00b5361b 7c17                 jl 0xb53634
00b5361d 85d2                 test edx, edx
00b5361f 7c13                 jl 0xb53634
00b53621 8b4108               mov eax, dword ptr [ecx + 8]
00b53624 85c0                 test eax, eax
00b53626 740c                 je 0xb53634
00b53628 52                   push edx
00b53629 56                   push esi
00b5362a e89103ffff           call 0xb439c0
00b5362f eb64                 jmp 0xb53695
00b53631 8b5de4               mov ebx, dword ptr [ebp - 0x1c]
00b53634 85ff                 test edi, edi
00b53636 8b45cc               mov eax, dword ptr [ebp - 0x34]
00b53639 7d04                 jge 0xb5363f
00b5363b 33ff                 xor edi, edi
00b5363d eb0b                 jmp 0xb5364a
00b5363f 0fb7485c             movzx ecx, word ptr [eax + 0x5c]
00b53643 49                   dec ecx
00b53644 3bf9                 cmp edi, ecx
00b53646 7e02                 jle 0xb5364a
00b53648 8bf9                 mov edi, ecx
00b5364a 85db                 test ebx, ebx
00b5364c 7d04                 jge 0xb53652
00b5364e 33db                 xor ebx, ebx
00b53650 eb0b                 jmp 0xb5365d
00b53652 0fb7485e             movzx ecx, word ptr [eax + 0x5e]
00b53656 49                   dec ecx
00b53657 3bd9                 cmp ebx, ecx
00b53659 7e02                 jle 0xb5365d
00b5365b 8bd9                 mov ebx, ecx
00b5365d 8b4808               mov ecx, dword ptr [eax + 8]
00b53660 85c9                 test ecx, ecx
00b53662 742b                 je 0xb5368f
00b53664 8b4008               mov eax, dword ptr [eax + 8]
00b53667 8b10                 mov edx, dword ptr [eax]
00b53669 680000803f           push 0x3f800000
00b5366e 53                   push ebx
00b5366f 57                   push edi
00b53670 8bc8                 mov ecx, eax
00b53672 ff5214               call dword ptr [edx + 0x14]
00b53675 d80db0712501         fmul dword ptr [0x12571b0]
00b5367b d95dcc               fstp dword ptr [ebp - 0x34]
00b5367e d945cc               fld dword ptr [ebp - 0x34]
00b53681 db5ddc               fistp dword ptr [ebp - 0x24]
00b53684 db45dc               fild dword ptr [ebp - 0x24]
00b53687 d80d341f2a01         fmul dword ptr [0x12a1f34]
00b5368d eb06                 jmp 0xb53695
00b5368f d905dcde2201         fld dword ptr [0x122dedc]
00b53695 d85dd0               fcomp dword ptr [ebp - 0x30]
00b53698 dfe0                 fnstsw ax
00b5369a f6c405               test ah, 5
00b5369d 0f8bab080000         jnp 0xb53f4e
00b536a3 d945d8               fld dword ptr [ebp - 0x28]
00b536a6 8b4d08               mov ecx, dword ptr [ebp + 8]
00b536a9 d801                 fadd dword ptr [ecx]
00b536ab c745dc0000003f       mov dword ptr [ebp - 0x24], 0x3f000000
00b536b2 c745cc0000803f       mov dword ptr [ebp - 0x34], 0x3f800000
00b536b9 d95de0               fstp dword ptr [ebp - 0x20]
00b536bc d945e0               fld dword ptr [ebp - 0x20]
00b536bf d845dc               fadd dword ptr [ebp - 0x24]
00b536c2 db5de8               fistp dword ptr [ebp - 0x18]
00b536c5 db45e8               fild dword ptr [ebp - 0x18]
00b536c8 d865cc               fsub dword ptr [ebp - 0x34]
00b536cb d95de4               fstp dword ptr [ebp - 0x1c]
00b536ce 8b45e0               mov eax, dword ptr [ebp - 0x20]
00b536d1 3b45e4               cmp eax, dword ptr [ebp - 0x1c]
00b536d4 7503                 jne 0xb536d9
00b536d6 ff4de8               dec dword ptr [ebp - 0x18]
00b536d9 d94104               fld dword ptr [ecx + 4]
00b536dc c745dc0000003f       mov dword ptr [ebp - 0x24], 0x3f000000
00b536e3 d865d4               fsub dword ptr [ebp - 0x2c]
00b536e6 c745cc0000803f       mov dword ptr [ebp - 0x34], 0x3f800000
00b536ed d95de0               fstp dword ptr [ebp - 0x20]
00b536f0 d945e0               fld dword ptr [ebp - 0x20]
00b536f3 d865dc               fsub dword ptr [ebp - 0x24]
00b536f6 db5de4               fistp dword ptr [ebp - 0x1c]
00b536f9 db45e4               fild dword ptr [ebp - 0x1c]
00b536fc d845cc               fadd dword ptr [ebp - 0x34]
00b536ff d95dec               fstp dword ptr [ebp - 0x14]
00b53702 8b45e0               mov eax, dword ptr [ebp - 0x20]
00b53705 3b45ec               cmp eax, dword ptr [ebp - 0x14]
00b53708 7503                 jne 0xb5370d
00b5370a ff45e4               inc dword ptr [ebp - 0x1c]
00b5370d a18c6e4301           mov eax, dword ptr [0x1436e8c]
00b53712 8b4818               mov ecx, dword ptr [eax + 0x18]
00b53715 8b7de8               mov edi, dword ptr [ebp - 0x18]
00b53718 8b5de4               mov ebx, dword ptr [ebp - 0x1c]
00b5371b 2bf9                 sub edi, ecx
00b5371d 894de0               mov dword ptr [ebp - 0x20], ecx
00b53720 8b481c               mov ecx, dword ptr [eax + 0x1c]
00b53723 2bd9                 sub ebx, ecx
00b53725 85ff                 test edi, edi
00b53727 897dcc               mov dword ptr [ebp - 0x34], edi
00b5372a 894ddc               mov dword ptr [ebp - 0x24], ecx
00b5372d 7c4d                 jl 0xb5377c
00b5372f 85db                 test ebx, ebx
00b53731 7c49                 jl 0xb5377c
00b53733 8b5010               mov edx, dword ptr [eax + 0x10]
00b53736 8bca                 mov ecx, edx
00b53738 c1e105               shl ecx, 5
00b5373b 3bf9                 cmp edi, ecx
00b5373d 8955f8               mov dword ptr [ebp - 8], edx
00b53740 894df4               mov dword ptr [ebp - 0xc], ecx
00b53743 7d37                 jge 0xb5377c
00b53745 8b4814               mov ecx, dword ptr [eax + 0x14]
00b53748 c1e105               shl ecx, 5
00b5374b 3bd9                 cmp ebx, ecx
00b5374d 894df0               mov dword ptr [ebp - 0x10], ecx
00b53750 7d2a                 jge 0xb5377c
00b53752 8b480c               mov ecx, dword ptr [eax + 0xc]
00b53755 8bf3                 mov esi, ebx
00b53757 c1fe05               sar esi, 5
00b5375a 0faff2               imul esi, edx
00b5375d 8bd7                 mov edx, edi
00b5375f c1fa05               sar edx, 5
00b53762 03f2                 add esi, edx
00b53764 894dfc               mov dword ptr [ebp - 4], ecx
00b53767 0fbf0c71             movsx ecx, word ptr [ecx + esi*2]
00b5376b 85c9                 test ecx, ecx
00b5376d 7e0d                 jle 0xb5377c
00b5376f 8b5020               mov edx, dword ptr [eax + 0x20]
00b53772 8b0c8a               mov ecx, dword ptr [edx + ecx*4]
00b53775 85c9                 test ecx, ecx
00b53777 8955ec               mov dword ptr [ebp - 0x14], edx
00b5377a 7563                 jne 0xb537df
00b5377c 8d77ff               lea esi, [edi - 1]
00b5377f 85f6                 test esi, esi
00b53781 0f8c9f000000         jl 0xb53826
00b53787 85db                 test ebx, ebx
00b53789 0f8c97000000         jl 0xb53826
00b5378f 8b5010               mov edx, dword ptr [eax + 0x10]
00b53792 8bca                 mov ecx, edx
00b53794 c1e105               shl ecx, 5
00b53797 3bf1                 cmp esi, ecx
00b53799 8955f8               mov dword ptr [ebp - 8], edx
00b5379c 894df4               mov dword ptr [ebp - 0xc], ecx
00b5379f 0f8d81000000         jge 0xb53826
00b537a5 8b4814               mov ecx, dword ptr [eax + 0x14]
00b537a8 c1e105               shl ecx, 5
00b537ab 3bd9                 cmp ebx, ecx
00b537ad 894df0               mov dword ptr [ebp - 0x10], ecx
00b537b0 7d74                 jge 0xb53826
00b537b2 8b480c               mov ecx, dword ptr [eax + 0xc]
00b537b5 894dfc               mov dword ptr [ebp - 4], ecx
00b537b8 8bcb                 mov ecx, ebx
00b537ba c1f905               sar ecx, 5
00b537bd 0fafca               imul ecx, edx
00b537c0 8bd6                 mov edx, esi
00b537c2 c1fa05               sar edx, 5
00b537c5 03ca                 add ecx, edx
00b537c7 8b55fc               mov edx, dword ptr [ebp - 4]
00b537ca 0fbf0c4a             movsx ecx, word ptr [edx + ecx*2]
00b537ce 85c9                 test ecx, ecx
00b537d0 7e54                 jle 0xb53826
00b537d2 8b5020               mov edx, dword ptr [eax + 0x20]
00b537d5 8b0c8a               mov ecx, dword ptr [edx + ecx*4]
00b537d8 85c9                 test ecx, ecx
00b537da 8955ec               mov dword ptr [ebp - 0x14], edx
00b537dd 7447                 je 0xb53826
00b537df 0fb74162             movzx eax, word ptr [ecx + 0x62]
00b537e3 8b55e4               mov edx, dword ptr [ebp - 0x1c]
00b537e6 0fb77160             movzx esi, word ptr [ecx + 0x60]
00b537ea 2bd0                 sub edx, eax
00b537ec 8b45e8               mov eax, dword ptr [ebp - 0x18]
00b537ef 2bc6                 sub eax, esi
00b537f1 0fb7715c             movzx esi, word ptr [ecx + 0x5c]
00b537f5 3bc6                 cmp eax, esi
00b537f7 0f8d6b010000         jge 0xb53968
00b537fd 0fb7715e             movzx esi, word ptr [ecx + 0x5e]
00b53801 3bd6                 cmp edx, esi
00b53803 0f8d5f010000         jge 0xb53968
00b53809 85c0                 test eax, eax
00b5380b 0f8c57010000         jl 0xb53968
00b53811 85d2                 test edx, edx
00b53813 0f8c4f010000         jl 0xb53968
00b53819 8b4108               mov eax, dword ptr [ecx + 8]
00b5381c 85c0                 test eax, eax
00b5381e 0f95c0               setne al
00b53821 e944010000           jmp 0xb5396a
00b53826 85ff                 test edi, edi
00b53828 8d4bff               lea ecx, [ebx - 1]
00b5382b 0f8c9f000000         jl 0xb538d0
00b53831 85c9                 test ecx, ecx
00b53833 0f8c97000000         jl 0xb538d0
00b53839 8b5010               mov edx, dword ptr [eax + 0x10]
00b5383c 8955f8               mov dword ptr [ebp - 8], edx
00b5383f c1e205               shl edx, 5
00b53842 3bfa                 cmp edi, edx
00b53844 8955f4               mov dword ptr [ebp - 0xc], edx
00b53847 0f8d83000000         jge 0xb538d0
00b5384d 8b5014               mov edx, dword ptr [eax + 0x14]
00b53850 c1e205               shl edx, 5
00b53853 3bca                 cmp ecx, edx
00b53855 8955f0               mov dword ptr [ebp - 0x10], edx
00b53858 7d76                 jge 0xb538d0
00b5385a 8b500c               mov edx, dword ptr [eax + 0xc]
00b5385d 8b7dcc               mov edi, dword ptr [ebp - 0x34]
00b53860 8955fc               mov dword ptr [ebp - 4], edx
00b53863 8bd1                 mov edx, ecx
00b53865 c1fa05               sar edx, 5
00b53868 0faf55f8             imul edx, dword ptr [ebp - 8]
00b5386c c1ff05               sar edi, 5
00b5386f 03d7                 add edx, edi
00b53871 8b7dfc               mov edi, dword ptr [ebp - 4]
00b53874 0fbf1457             movsx edx, word ptr [edi + edx*2]
00b53878 85d2                 test edx, edx
00b5387a 7e51                 jle 0xb538cd
00b5387c 8b7820               mov edi, dword ptr [eax + 0x20]
00b5387f 8b1497               mov edx, dword ptr [edi + edx*4]
00b53882 85d2                 test edx, edx
00b53884 897dec               mov dword ptr [ebp - 0x14], edi
00b53887 7444                 je 0xb538cd
00b53889 0fb74262             movzx eax, word ptr [edx + 0x62]
00b5388d 8b4de4               mov ecx, dword ptr [ebp - 0x1c]
00b53890 0fb77260             movzx esi, word ptr [edx + 0x60]
00b53894 2bc8                 sub ecx, eax
00b53896 8b45e8               mov eax, dword ptr [ebp - 0x18]
00b53899 2bc6                 sub eax, esi
00b5389b 0fb7725c             movzx esi, word ptr [edx + 0x5c]
00b5389f 3bc6                 cmp eax, esi
00b538a1 7d20                 jge 0xb538c3
00b538a3 0fb7725e             movzx esi, word ptr [edx + 0x5e]
00b538a7 3bce                 cmp ecx, esi
00b538a9 7d18                 jge 0xb538c3
00b538ab 85c0                 test eax, eax
00b538ad 7c14                 jl 0xb538c3
00b538af 85c9                 test ecx, ecx
00b538b1 7c10                 jl 0xb538c3
00b538b3 8b4208               mov eax, dword ptr [edx + 8]
00b538b6 8b7dcc               mov edi, dword ptr [ebp - 0x34]
00b538b9 85c0                 test eax, eax
00b538bb 0f95c0               setne al
00b538be e9a7000000           jmp 0xb5396a
00b538c3 8b7dcc               mov edi, dword ptr [ebp - 0x34]
00b538c6 32c0                 xor al, al
00b538c8 e99d000000           jmp 0xb5396a
00b538cd 8b7dcc               mov edi, dword ptr [ebp - 0x34]
00b538d0 85f6                 test esi, esi
00b538d2 0f8c76060000         jl 0xb53f4e
00b538d8 85c9                 test ecx, ecx
00b538da 0f8c6e060000         jl 0xb53f4e
00b538e0 8b5010               mov edx, dword ptr [eax + 0x10]
00b538e3 8955f8               mov dword ptr [ebp - 8], edx
00b538e6 c1e205               shl edx, 5
00b538e9 3bf2                 cmp esi, edx
00b538eb 8955f4               mov dword ptr [ebp - 0xc], edx
00b538ee 0f8d5a060000         jge 0xb53f4e
00b538f4 8b5014               mov edx, dword ptr [eax + 0x14]
00b538f7 c1e205               shl edx, 5
00b538fa 3bca                 cmp ecx, edx
00b538fc 8955f0               mov dword ptr [ebp - 0x10], edx
00b538ff 0f8d49060000         jge 0xb53f4e
00b53905 8b500c               mov edx, dword ptr [eax + 0xc]
00b53908 c1f905               sar ecx, 5
00b5390b 0faf4df8             imul ecx, dword ptr [ebp - 8]
00b5390f c1fe05               sar esi, 5
00b53912 03ce                 add ecx, esi
00b53914 0fbf0c4a             movsx ecx, word ptr [edx + ecx*2]
00b53918 85c9                 test ecx, ecx
00b5391a 8955fc               mov dword ptr [ebp - 4], edx
00b5391d 0f8e2b060000         jle 0xb53f4e
00b53923 8b4020               mov eax, dword ptr [eax + 0x20]
00b53926 8945ec               mov dword ptr [ebp - 0x14], eax
00b53929 8b0488               mov eax, dword ptr [eax + ecx*4]
00b5392c 85c0                 test eax, eax
00b5392e 0f841a060000         je 0xb53f4e
00b53934 0fb74862             movzx ecx, word ptr [eax + 0x62]
00b53938 8b55e4               mov edx, dword ptr [ebp - 0x1c]
00b5393b 0fb77060             movzx esi, word ptr [eax + 0x60]
00b5393f 2bd1                 sub edx, ecx
00b53941 8b4de8               mov ecx, dword ptr [ebp - 0x18]
00b53944 2bce                 sub ecx, esi
00b53946 0fb7705c             movzx esi, word ptr [eax + 0x5c]
00b5394a 3bce                 cmp ecx, esi
00b5394c 7d1a                 jge 0xb53968
00b5394e 0fb7705e             movzx esi, word ptr [eax + 0x5e]
00b53952 3bd6                 cmp edx, esi
00b53954 7d12                 jge 0xb53968
00b53956 85c9                 test ecx, ecx
00b53958 7c0e                 jl 0xb53968
00b5395a 85d2                 test edx, edx
00b5395c 7c0a                 jl 0xb53968
00b5395e 8b4808               mov ecx, dword ptr [eax + 8]
00b53961 85c9                 test ecx, ecx
00b53963 0f95c0               setne al
00b53966 eb02                 jmp 0xb5396a
00b53968 32c0                 xor al, al
00b5396a 84c0                 test al, al
00b5396c 0f84dc050000         je 0xb53f4e
00b53972 85ff                 test edi, edi
00b53974 0f8c62010000         jl 0xb53adc
00b5397a 85db                 test ebx, ebx
00b5397c 0f8c5a010000         jl 0xb53adc
00b53982 3b7df4               cmp edi, dword ptr [ebp - 0xc]
00b53985 0f8d51010000         jge 0xb53adc
00b5398b 3b5df0               cmp ebx, dword ptr [ebp - 0x10]
00b5398e 0f8d48010000         jge 0xb53adc
00b53994 8b55fc               mov edx, dword ptr [ebp - 4]
00b53997 c1fb05               sar ebx, 5
00b5399a 0faf5df8             imul ebx, dword ptr [ebp - 8]
00b5399e c1ff05               sar edi, 5
00b539a1 03df                 add ebx, edi
00b539a3 0fbf045a             movsx eax, word ptr [edx + ebx*2]
00b539a7 85c0                 test eax, eax
00b539a9 0f8e2d010000         jle 0xb53adc
00b539af 8b4dec               mov ecx, dword ptr [ebp - 0x14]
00b539b2 8b0481               mov eax, dword ptr [ecx + eax*4]
00b539b5 85c0                 test eax, eax
00b539b7 8945cc               mov dword ptr [ebp - 0x34], eax
00b539ba 0f841c010000         je 0xb53adc
00b539c0 0fb75062             movzx edx, word ptr [eax + 0x62]
00b539c4 8b4de4               mov ecx, dword ptr [ebp - 0x1c]
00b539c7 0fb77060             movzx esi, word ptr [eax + 0x60]
00b539cb 8b7de8               mov edi, dword ptr [ebp - 0x18]
00b539ce 2bca                 sub ecx, edx
00b539d0 894de4               mov dword ptr [ebp - 0x1c], ecx
00b539d3 0fb7485c             movzx ecx, word ptr [eax + 0x5c]
00b539d7 2bfe                 sub edi, esi
00b539d9 3bf9                 cmp edi, ecx
00b539db 7d1e                 jge 0xb539fb
00b539dd 0fb7485e             movzx ecx, word ptr [eax + 0x5e]
00b539e1 8b5de4               mov ebx, dword ptr [ebp - 0x1c]
00b539e4 3bd9                 cmp ebx, ecx
00b539e6 7d13                 jge 0xb539fb
00b539e8 85ff                 test edi, edi
00b539ea 7c0f                 jl 0xb539fb
00b539ec 85db                 test ebx, ebx
00b539ee 7c0b                 jl 0xb539fb
00b539f0 8b4808               mov ecx, dword ptr [eax + 8]
00b539f3 85c9                 test ecx, ecx
00b539f5 0f85b6000000         jne 0xb53ab1
00b539fb 8b5de0               mov ebx, dword ptr [ebp - 0x20]
00b539fe 8bce                 mov ecx, esi
00b53a00 2bcb                 sub ecx, ebx
00b53a02 8b5ddc               mov ebx, dword ptr [ebp - 0x24]
00b53a05 8bc2                 mov eax, edx
00b53a07 2bc3                 sub eax, ebx
00b53a09 8b5de4               mov ebx, dword ptr [ebp - 0x1c]
00b53a0c 03cf                 add ecx, edi
00b53a0e 03c3                 add eax, ebx
00b53a10 85c9                 test ecx, ecx
00b53a12 7c6d                 jl 0xb53a81
00b53a14 85c0                 test eax, eax
00b53a16 7c69                 jl 0xb53a81
00b53a18 3b4df4               cmp ecx, dword ptr [ebp - 0xc]
00b53a1b 7d61                 jge 0xb53a7e
00b53a1d 3b45f0               cmp eax, dword ptr [ebp - 0x10]
00b53a20 7d5c                 jge 0xb53a7e
00b53a22 c1f805               sar eax, 5
00b53a25 0faf45f8             imul eax, dword ptr [ebp - 8]
00b53a29 c1f905               sar ecx, 5
00b53a2c 03c1                 add eax, ecx
00b53a2e 8b4dfc               mov ecx, dword ptr [ebp - 4]
00b53a31 0fbf0441             movsx eax, word ptr [ecx + eax*2]
00b53a35 85c0                 test eax, eax
00b53a37 7e45                 jle 0xb53a7e
00b53a39 8b4dec               mov ecx, dword ptr [ebp - 0x14]
00b53a3c 8b0c81               mov ecx, dword ptr [ecx + eax*4]
00b53a3f 85c9                 test ecx, ecx
00b53a41 743b                 je 0xb53a7e
00b53a43 0fb74162             movzx eax, word ptr [ecx + 0x62]
00b53a47 8b5de4               mov ebx, dword ptr [ebp - 0x1c]
00b53a4a 2bd0                 sub edx, eax
00b53a4c 0fb74160             movzx eax, word ptr [ecx + 0x60]
00b53a50 2bf0                 sub esi, eax
00b53a52 0fb7415c             movzx eax, word ptr [ecx + 0x5c]
00b53a56 03f7                 add esi, edi
00b53a58 03d3                 add edx, ebx
00b53a5a 3bf0                 cmp esi, eax
00b53a5c 7d23                 jge 0xb53a81
00b53a5e 0fb7415e             movzx eax, word ptr [ecx + 0x5e]
00b53a62 3bd0                 cmp edx, eax
00b53a64 7d1b                 jge 0xb53a81
00b53a66 85f6                 test esi, esi
00b53a68 7c17                 jl 0xb53a81
00b53a6a 85d2                 test edx, edx
00b53a6c 7c13                 jl 0xb53a81
00b53a6e 8b4108               mov eax, dword ptr [ecx + 8]
00b53a71 85c0                 test eax, eax
00b53a73 740c                 je 0xb53a81
00b53a75 52                   push edx
00b53a76 56                   push esi
00b53a77 e844fffeff           call 0xb439c0
00b53a7c eb64                 jmp 0xb53ae2
00b53a7e 8b5de4               mov ebx, dword ptr [ebp - 0x1c]
00b53a81 85ff                 test edi, edi
00b53a83 8b45cc               mov eax, dword ptr [ebp - 0x34]
00b53a86 7d04                 jge 0xb53a8c
00b53a88 33ff                 xor edi, edi
00b53a8a eb0b                 jmp 0xb53a97
00b53a8c 0fb7485c             movzx ecx, word ptr [eax + 0x5c]
00b53a90 49                   dec ecx
00b53a91 3bf9                 cmp edi, ecx
00b53a93 7e02                 jle 0xb53a97
00b53a95 8bf9                 mov edi, ecx
00b53a97 85db                 test ebx, ebx
00b53a99 7d04                 jge 0xb53a9f
00b53a9b 33db                 xor ebx, ebx
00b53a9d eb0b                 jmp 0xb53aaa
00b53a9f 0fb7485e             movzx ecx, word ptr [eax + 0x5e]
00b53aa3 49                   dec ecx
00b53aa4 3bd9                 cmp ebx, ecx
00b53aa6 7e02                 jle 0xb53aaa
00b53aa8 8bd9                 mov ebx, ecx
00b53aaa 8b4808               mov ecx, dword ptr [eax + 8]
00b53aad 85c9                 test ecx, ecx
00b53aaf 742b                 je 0xb53adc
00b53ab1 8b4008               mov eax, dword ptr [eax + 8]
00b53ab4 8b10                 mov edx, dword ptr [eax]
00b53ab6 680000803f           push 0x3f800000
00b53abb 53                   push ebx
00b53abc 57                   push edi
00b53abd 8bc8                 mov ecx, eax
00b53abf ff5214               call dword ptr [edx + 0x14]
00b53ac2 d80db0712501         fmul dword ptr [0x12571b0]
00b53ac8 d95dcc               fstp dword ptr [ebp - 0x34]
00b53acb d945cc               fld dword ptr [ebp - 0x34]
00b53ace db5ddc               fistp dword ptr [ebp - 0x24]
00b53ad1 db45dc               fild dword ptr [ebp - 0x24]
00b53ad4 d80d341f2a01         fmul dword ptr [0x12a1f34]
00b53ada eb06                 jmp 0xb53ae2
00b53adc d905dcde2201         fld dword ptr [0x122dedc]
00b53ae2 d85dd0               fcomp dword ptr [ebp - 0x30]
00b53ae5 dfe0                 fnstsw ax
00b53ae7 f6c405               test ah, 5
00b53aea 0f8b5e040000         jnp 0xb53f4e
00b53af0 d945d8               fld dword ptr [ebp - 0x28]
00b53af3 8b4d08               mov ecx, dword ptr [ebp + 8]
00b53af6 d801                 fadd dword ptr [ecx]
00b53af8 c745cc0000003f       mov dword ptr [ebp - 0x34], 0x3f000000
00b53aff c745080000803f       mov dword ptr [ebp + 8], 0x3f800000
00b53b06 d95dd8               fstp dword ptr [ebp - 0x28]
00b53b09 d945d8               fld dword ptr [ebp - 0x28]
00b53b0c d845cc               fadd dword ptr [ebp - 0x34]
00b53b0f db5de4               fistp dword ptr [ebp - 0x1c]
00b53b12 db45e4               fild dword ptr [ebp - 0x1c]
00b53b15 d86508               fsub dword ptr [ebp + 8]
00b53b18 d95ddc               fstp dword ptr [ebp - 0x24]
00b53b1b 8b45d8               mov eax, dword ptr [ebp - 0x28]
00b53b1e 3b45dc               cmp eax, dword ptr [ebp - 0x24]
00b53b21 7503                 jne 0xb53b26
00b53b23 ff4de4               dec dword ptr [ebp - 0x1c]
00b53b26 d945d4               fld dword ptr [ebp - 0x2c]
00b53b29 c745d40000003f       mov dword ptr [ebp - 0x2c], 0x3f000000
00b53b30 d84104               fadd dword ptr [ecx + 4]
00b53b33 c745cc0000803f       mov dword ptr [ebp - 0x34], 0x3f800000
00b53b3a d95dd8               fstp dword ptr [ebp - 0x28]
00b53b3d d945d8               fld dword ptr [ebp - 0x28]
00b53b40 d845d4               fadd dword ptr [ebp - 0x2c]
00b53b43 db5d08               fistp dword ptr [ebp + 8]
00b53b46 db4508               fild dword ptr [ebp + 8]
00b53b49 d865cc               fsub dword ptr [ebp - 0x34]
00b53b4c d95ddc               fstp dword ptr [ebp - 0x24]
00b53b4f 8b45d8               mov eax, dword ptr [ebp - 0x28]
00b53b52 3b45dc               cmp eax, dword ptr [ebp - 0x24]
00b53b55 7503                 jne 0xb53b5a
00b53b57 ff4d08               dec dword ptr [ebp + 8]
00b53b5a a18c6e4301           mov eax, dword ptr [0x1436e8c]
00b53b5f 8b4818               mov ecx, dword ptr [eax + 0x18]
00b53b62 8b5de4               mov ebx, dword ptr [ebp - 0x1c]
00b53b65 8b7d08               mov edi, dword ptr [ebp + 8]
00b53b68 2bd9                 sub ebx, ecx
00b53b6a 894de0               mov dword ptr [ebp - 0x20], ecx
00b53b6d 8b481c               mov ecx, dword ptr [eax + 0x1c]
00b53b70 2bf9                 sub edi, ecx
00b53b72 85db                 test ebx, ebx
00b53b74 895dcc               mov dword ptr [ebp - 0x34], ebx
00b53b77 894ddc               mov dword ptr [ebp - 0x24], ecx
00b53b7a 7c4d                 jl 0xb53bc9
00b53b7c 85ff                 test edi, edi
00b53b7e 7c49                 jl 0xb53bc9
00b53b80 8b5010               mov edx, dword ptr [eax + 0x10]
00b53b83 8bca                 mov ecx, edx
00b53b85 c1e105               shl ecx, 5
00b53b88 3bd9                 cmp ebx, ecx
00b53b8a 8955f8               mov dword ptr [ebp - 8], edx
00b53b8d 894df4               mov dword ptr [ebp - 0xc], ecx
00b53b90 7d37                 jge 0xb53bc9
00b53b92 8b4814               mov ecx, dword ptr [eax + 0x14]
00b53b95 c1e105               shl ecx, 5
00b53b98 3bf9                 cmp edi, ecx
00b53b9a 894df0               mov dword ptr [ebp - 0x10], ecx
00b53b9d 7d2a                 jge 0xb53bc9
00b53b9f 8b480c               mov ecx, dword ptr [eax + 0xc]
00b53ba2 8bf7                 mov esi, edi
00b53ba4 c1fe05               sar esi, 5
00b53ba7 0faff2               imul esi, edx
00b53baa 8bd3                 mov edx, ebx
00b53bac c1fa05               sar edx, 5
00b53baf 03f2                 add esi, edx
00b53bb1 894dfc               mov dword ptr [ebp - 4], ecx
00b53bb4 0fbf0c71             movsx ecx, word ptr [ecx + esi*2]
00b53bb8 85c9                 test ecx, ecx
00b53bba 7e0d                 jle 0xb53bc9
00b53bbc 8b5020               mov edx, dword ptr [eax + 0x20]
00b53bbf 8b0c8a               mov ecx, dword ptr [edx + ecx*4]
00b53bc2 85c9                 test ecx, ecx
00b53bc4 8955ec               mov dword ptr [ebp - 0x14], edx
00b53bc7 7563                 jne 0xb53c2c
00b53bc9 8d73ff               lea esi, [ebx - 1]
00b53bcc 85f6                 test esi, esi
00b53bce 0f8c9f000000         jl 0xb53c73
00b53bd4 85ff                 test edi, edi
00b53bd6 0f8c97000000         jl 0xb53c73
00b53bdc 8b5010               mov edx, dword ptr [eax + 0x10]
00b53bdf 8bca                 mov ecx, edx
00b53be1 c1e105               shl ecx, 5
00b53be4 3bf1                 cmp esi, ecx
00b53be6 8955f8               mov dword ptr [ebp - 8], edx
00b53be9 894df4               mov dword ptr [ebp - 0xc], ecx
00b53bec 0f8d81000000         jge 0xb53c73
00b53bf2 8b4814               mov ecx, dword ptr [eax + 0x14]
00b53bf5 c1e105               shl ecx, 5
00b53bf8 3bf9                 cmp edi, ecx
00b53bfa 894df0               mov dword ptr [ebp - 0x10], ecx
00b53bfd 7d74                 jge 0xb53c73
00b53bff 8b480c               mov ecx, dword ptr [eax + 0xc]
00b53c02 894dfc               mov dword ptr [ebp - 4], ecx
00b53c05 8bcf                 mov ecx, edi
00b53c07 c1f905               sar ecx, 5
00b53c0a 0fafca               imul ecx, edx
00b53c0d 8bd6                 mov edx, esi
00b53c0f c1fa05               sar edx, 5
00b53c12 03ca                 add ecx, edx
00b53c14 8b55fc               mov edx, dword ptr [ebp - 4]
00b53c17 0fbf0c4a             movsx ecx, word ptr [edx + ecx*2]
00b53c1b 85c9                 test ecx, ecx
00b53c1d 7e54                 jle 0xb53c73
00b53c1f 8b5020               mov edx, dword ptr [eax + 0x20]
00b53c22 8b0c8a               mov ecx, dword ptr [edx + ecx*4]
00b53c25 85c9                 test ecx, ecx
00b53c27 8955ec               mov dword ptr [ebp - 0x14], edx
00b53c2a 7447                 je 0xb53c73
00b53c2c 0fb74162             movzx eax, word ptr [ecx + 0x62]
00b53c30 8b5508               mov edx, dword ptr [ebp + 8]
00b53c33 0fb77160             movzx esi, word ptr [ecx + 0x60]
00b53c37 2bd0                 sub edx, eax
00b53c39 8b45e4               mov eax, dword ptr [ebp - 0x1c]
00b53c3c 2bc6                 sub eax, esi
00b53c3e 0fb7715c             movzx esi, word ptr [ecx + 0x5c]
00b53c42 3bc6                 cmp eax, esi
00b53c44 0f8d6b010000         jge 0xb53db5
00b53c4a 0fb7715e             movzx esi, word ptr [ecx + 0x5e]
00b53c4e 3bd6                 cmp edx, esi
00b53c50 0f8d5f010000         jge 0xb53db5
00b53c56 85c0                 test eax, eax
00b53c58 0f8c57010000         jl 0xb53db5
00b53c5e 85d2                 test edx, edx
00b53c60 0f8c4f010000         jl 0xb53db5
00b53c66 8b4108               mov eax, dword ptr [ecx + 8]
00b53c69 85c0                 test eax, eax
00b53c6b 0f95c0               setne al
00b53c6e e944010000           jmp 0xb53db7
00b53c73 85db                 test ebx, ebx
00b53c75 8d4fff               lea ecx, [edi - 1]
00b53c78 0f8c9f000000         jl 0xb53d1d
00b53c7e 85c9                 test ecx, ecx
00b53c80 0f8c97000000         jl 0xb53d1d
00b53c86 8b5010               mov edx, dword ptr [eax + 0x10]
00b53c89 8955f8               mov dword ptr [ebp - 8], edx
00b53c8c c1e205               shl edx, 5
00b53c8f 3bda                 cmp ebx, edx
00b53c91 8955f4               mov dword ptr [ebp - 0xc], edx
00b53c94 0f8d83000000         jge 0xb53d1d
00b53c9a 8b5014               mov edx, dword ptr [eax + 0x14]
00b53c9d c1e205               shl edx, 5
00b53ca0 3bca                 cmp ecx, edx
00b53ca2 8955f0               mov dword ptr [ebp - 0x10], edx
00b53ca5 7d76                 jge 0xb53d1d
00b53ca7 8b500c               mov edx, dword ptr [eax + 0xc]
00b53caa 8b5dcc               mov ebx, dword ptr [ebp - 0x34]
00b53cad 8955fc               mov dword ptr [ebp - 4], edx
00b53cb0 8bd1                 mov edx, ecx
00b53cb2 c1fa05               sar edx, 5
00b53cb5 0faf55f8             imul edx, dword ptr [ebp - 8]
00b53cb9 c1fb05               sar ebx, 5
00b53cbc 03d3                 add edx, ebx
00b53cbe 8b5dfc               mov ebx, dword ptr [ebp - 4]
00b53cc1 0fbf1453             movsx edx, word ptr [ebx + edx*2]
00b53cc5 85d2                 test edx, edx
00b53cc7 7e51                 jle 0xb53d1a
00b53cc9 8b5820               mov ebx, dword ptr [eax + 0x20]
00b53ccc 8b1493               mov edx, dword ptr [ebx + edx*4]
00b53ccf 85d2                 test edx, edx
00b53cd1 895dec               mov dword ptr [ebp - 0x14], ebx
00b53cd4 7444                 je 0xb53d1a
00b53cd6 0fb74262             movzx eax, word ptr [edx + 0x62]
00b53cda 8b4d08               mov ecx, dword ptr [ebp + 8]
00b53cdd 0fb77260             movzx esi, word ptr [edx + 0x60]
00b53ce1 2bc8                 sub ecx, eax
00b53ce3 8b45e4               mov eax, dword ptr [ebp - 0x1c]
00b53ce6 2bc6                 sub eax, esi
00b53ce8 0fb7725c             movzx esi, word ptr [edx + 0x5c]
00b53cec 3bc6                 cmp eax, esi
00b53cee 7d20                 jge 0xb53d10
00b53cf0 0fb7725e             movzx esi, word ptr [edx + 0x5e]
00b53cf4 3bce                 cmp ecx, esi
00b53cf6 7d18                 jge 0xb53d10
00b53cf8 85c0                 test eax, eax
00b53cfa 7c14                 jl 0xb53d10
00b53cfc 85c9                 test ecx, ecx
00b53cfe 7c10                 jl 0xb53d10
00b53d00 8b4208               mov eax, dword ptr [edx + 8]
00b53d03 8b5dcc               mov ebx, dword ptr [ebp - 0x34]
00b53d06 85c0                 test eax, eax
00b53d08 0f95c0               setne al
00b53d0b e9a7000000           jmp 0xb53db7
00b53d10 8b5dcc               mov ebx, dword ptr [ebp - 0x34]
00b53d13 32c0                 xor al, al
00b53d15 e99d000000           jmp 0xb53db7
00b53d1a 8b5dcc               mov ebx, dword ptr [ebp - 0x34]
00b53d1d 85f6                 test esi, esi
00b53d1f 0f8c29020000         jl 0xb53f4e
00b53d25 85c9                 test ecx, ecx
00b53d27 0f8c21020000         jl 0xb53f4e
00b53d2d 8b5010               mov edx, dword ptr [eax + 0x10]
00b53d30 8955f8               mov dword ptr [ebp - 8], edx
00b53d33 c1e205               shl edx, 5
00b53d36 3bf2                 cmp esi, edx
00b53d38 8955f4               mov dword ptr [ebp - 0xc], edx
00b53d3b 0f8d0d020000         jge 0xb53f4e
00b53d41 8b5014               mov edx, dword ptr [eax + 0x14]
00b53d44 c1e205               shl edx, 5
00b53d47 3bca                 cmp ecx, edx
00b53d49 8955f0               mov dword ptr [ebp - 0x10], edx
00b53d4c 0f8dfc010000         jge 0xb53f4e
00b53d52 8b500c               mov edx, dword ptr [eax + 0xc]
00b53d55 c1f905               sar ecx, 5
00b53d58 0faf4df8             imul ecx, dword ptr [ebp - 8]
00b53d5c c1fe05               sar esi, 5
00b53d5f 03ce                 add ecx, esi
00b53d61 0fbf0c4a             movsx ecx, word ptr [edx + ecx*2]
00b53d65 85c9                 test ecx, ecx
00b53d67 8955fc               mov dword ptr [ebp - 4], edx
00b53d6a 0f8ede010000         jle 0xb53f4e
00b53d70 8b4020               mov eax, dword ptr [eax + 0x20]
00b53d73 8945ec               mov dword ptr [ebp - 0x14], eax
00b53d76 8b0488               mov eax, dword ptr [eax + ecx*4]
00b53d79 85c0                 test eax, eax
00b53d7b 0f84cd010000         je 0xb53f4e
00b53d81 0fb74862             movzx ecx, word ptr [eax + 0x62]
00b53d85 8b5508               mov edx, dword ptr [ebp + 8]
00b53d88 0fb77060             movzx esi, word ptr [eax + 0x60]
00b53d8c 2bd1                 sub edx, ecx
00b53d8e 8b4de4               mov ecx, dword ptr [ebp - 0x1c]
00b53d91 2bce                 sub ecx, esi
00b53d93 0fb7705c             movzx esi, word ptr [eax + 0x5c]
00b53d97 3bce                 cmp ecx, esi
00b53d99 7d1a                 jge 0xb53db5
00b53d9b 0fb7705e             movzx esi, word ptr [eax + 0x5e]
00b53d9f 3bd6                 cmp edx, esi
00b53da1 7d12                 jge 0xb53db5
00b53da3 85c9                 test ecx, ecx
00b53da5 7c0e                 jl 0xb53db5
00b53da7 85d2                 test edx, edx
00b53da9 7c0a                 jl 0xb53db5
00b53dab 8b4808               mov ecx, dword ptr [eax + 8]
00b53dae 85c9                 test ecx, ecx
00b53db0 0f95c0               setne al
00b53db3 eb02                 jmp 0xb53db7
00b53db5 32c0                 xor al, al
00b53db7 84c0                 test al, al
00b53db9 0f848f010000         je 0xb53f4e
00b53dbf 85db                 test ebx, ebx
00b53dc1 0f8c69010000         jl 0xb53f30
00b53dc7 85ff                 test edi, edi
00b53dc9 0f8c61010000         jl 0xb53f30
00b53dcf 3b5df4               cmp ebx, dword ptr [ebp - 0xc]
00b53dd2 0f8d58010000         jge 0xb53f30
00b53dd8 3b7df0               cmp edi, dword ptr [ebp - 0x10]
00b53ddb 0f8d4f010000         jge 0xb53f30
00b53de1 8b55fc               mov edx, dword ptr [ebp - 4]
00b53de4 c1ff05               sar edi, 5
00b53de7 0faf7df8             imul edi, dword ptr [ebp - 8]
00b53deb c1fb05               sar ebx, 5
00b53dee 03fb                 add edi, ebx
00b53df0 0fbf047a             movsx eax, word ptr [edx + edi*2]
00b53df4 85c0                 test eax, eax
00b53df6 0f8e34010000         jle 0xb53f30
00b53dfc 8b4dec               mov ecx, dword ptr [ebp - 0x14]
00b53dff 8b0481               mov eax, dword ptr [ecx + eax*4]
00b53e02 85c0                 test eax, eax
00b53e04 8945d4               mov dword ptr [ebp - 0x2c], eax
00b53e07 0f8423010000         je 0xb53f30
00b53e0d 0fb75860             movzx ebx, word ptr [eax + 0x60]
00b53e11 0fb77862             movzx edi, word ptr [eax + 0x62]
00b53e15 8b55e4               mov edx, dword ptr [ebp - 0x1c]
00b53e18 0fb7485c             movzx ecx, word ptr [eax + 0x5c]
00b53e1c 8b7508               mov esi, dword ptr [ebp + 8]
00b53e1f 2bd3                 sub edx, ebx
00b53e21 2bf7                 sub esi, edi
00b53e23 3bd1                 cmp edx, ecx
00b53e25 7d1b                 jge 0xb53e42
00b53e27 0fb7485e             movzx ecx, word ptr [eax + 0x5e]
00b53e2b 3bf1                 cmp esi, ecx
00b53e2d 7d13                 jge 0xb53e42
00b53e2f 85d2                 test edx, edx
00b53e31 7c0f                 jl 0xb53e42
00b53e33 85f6                 test esi, esi
00b53e35 7c0b                 jl 0xb53e42
00b53e37 8b4808               mov ecx, dword ptr [eax + 8]
00b53e3a 85c9                 test ecx, ecx
00b53e3c 0f85c3000000         jne 0xb53f05
00b53e42 8b45e0               mov eax, dword ptr [ebp - 0x20]
00b53e45 8bca                 mov ecx, edx
00b53e47 2bc8                 sub ecx, eax
00b53e49 03cb                 add ecx, ebx
00b53e4b 8b5ddc               mov ebx, dword ptr [ebp - 0x24]
00b53e4e 8bc6                 mov eax, esi
00b53e50 2bc3                 sub eax, ebx
00b53e52 03c7                 add eax, edi
00b53e54 85c9                 test ecx, ecx
00b53e56 7c7d                 jl 0xb53ed5
00b53e58 85c0                 test eax, eax
00b53e5a 7c79                 jl 0xb53ed5
00b53e5c 3b4df4               cmp ecx, dword ptr [ebp - 0xc]
00b53e5f 7d74                 jge 0xb53ed5
00b53e61 3b45f0               cmp eax, dword ptr [ebp - 0x10]
00b53e64 7d6f                 jge 0xb53ed5
00b53e66 c1f805               sar eax, 5
00b53e69 0faf45f8             imul eax, dword ptr [ebp - 8]
00b53e6d c1f905               sar ecx, 5
00b53e70 03c1                 add eax, ecx
00b53e72 8b4dfc               mov ecx, dword ptr [ebp - 4]
00b53e75 0fbf0441             movsx eax, word ptr [ecx + eax*2]
00b53e79 85c0                 test eax, eax
00b53e7b 7e58                 jle 0xb53ed5
00b53e7d 8b4dec               mov ecx, dword ptr [ebp - 0x14]
00b53e80 8b0c81               mov ecx, dword ptr [ecx + eax*4]
00b53e83 85c9                 test ecx, ecx
00b53e85 744e                 je 0xb53ed5
00b53e87 0fb74162             movzx eax, word ptr [ecx + 0x62]
00b53e8b 8bde                 mov ebx, esi
00b53e8d 2bd8                 sub ebx, eax
00b53e8f 0fb74160             movzx eax, word ptr [ecx + 0x60]
00b53e93 03df                 add ebx, edi
00b53e95 895d08               mov dword ptr [ebp + 8], ebx
00b53e98 8bda                 mov ebx, edx
00b53e9a 2bd8                 sub ebx, eax
00b53e9c 8b45d4               mov eax, dword ptr [ebp - 0x2c]
00b53e9f 0fb77860             movzx edi, word ptr [eax + 0x60]
00b53ea3 03fb                 add edi, ebx
00b53ea5 0fb7595c             movzx ebx, word ptr [ecx + 0x5c]
00b53ea9 3bfb                 cmp edi, ebx
00b53eab 897dcc               mov dword ptr [ebp - 0x34], edi
00b53eae 7d28                 jge 0xb53ed8
00b53eb0 0fb7595e             movzx ebx, word ptr [ecx + 0x5e]
00b53eb4 8b7d08               mov edi, dword ptr [ebp + 8]
00b53eb7 3bfb                 cmp edi, ebx
00b53eb9 7d1d                 jge 0xb53ed8
00b53ebb 8b5dcc               mov ebx, dword ptr [ebp - 0x34]
00b53ebe 85db                 test ebx, ebx
00b53ec0 7c16                 jl 0xb53ed8
00b53ec2 85ff                 test edi, edi
00b53ec4 7c12                 jl 0xb53ed8
00b53ec6 83790800             cmp dword ptr [ecx + 8], 0
00b53eca 740c                 je 0xb53ed8
00b53ecc 57                   push edi
00b53ecd 53                   push ebx
00b53ece e8edfafeff           call 0xb439c0
00b53ed3 eb61                 jmp 0xb53f36
00b53ed5 8b45d4               mov eax, dword ptr [ebp - 0x2c]
00b53ed8 85d2                 test edx, edx
00b53eda 7d04                 jge 0xb53ee0
00b53edc 33d2                 xor edx, edx
00b53ede eb0b                 jmp 0xb53eeb
00b53ee0 0fb7485c             movzx ecx, word ptr [eax + 0x5c]
00b53ee4 49                   dec ecx
00b53ee5 3bd1                 cmp edx, ecx
00b53ee7 7e02                 jle 0xb53eeb
00b53ee9 8bd1                 mov edx, ecx
00b53eeb 85f6                 test esi, esi
00b53eed 7d04                 jge 0xb53ef3
00b53eef 33f6                 xor esi, esi
00b53ef1 eb0b                 jmp 0xb53efe
00b53ef3 0fb7485e             movzx ecx, word ptr [eax + 0x5e]
00b53ef7 49                   dec ecx
00b53ef8 3bf1                 cmp esi, ecx
00b53efa 7e02                 jle 0xb53efe
00b53efc 8bf1                 mov esi, ecx
00b53efe 8b4808               mov ecx, dword ptr [eax + 8]
00b53f01 85c9                 test ecx, ecx
00b53f03 742b                 je 0xb53f30
00b53f05 8b4008               mov eax, dword ptr [eax + 8]
00b53f08 8b38                 mov edi, dword ptr [eax]
00b53f0a 680000803f           push 0x3f800000
00b53f0f 56                   push esi
00b53f10 52                   push edx
00b53f11 8bc8                 mov ecx, eax
00b53f13 ff5714               call dword ptr [edi + 0x14]
00b53f16 d80db0712501         fmul dword ptr [0x12571b0]
00b53f1c d95d08               fstp dword ptr [ebp + 8]
00b53f1f d94508               fld dword ptr [ebp + 8]
00b53f22 db5dcc               fistp dword ptr [ebp - 0x34]
00b53f25 db45cc               fild dword ptr [ebp - 0x34]
00b53f28 d80d341f2a01         fmul dword ptr [0x12a1f34]
00b53f2e eb06                 jmp 0xb53f36
00b53f30 d905dcde2201         fld dword ptr [0x122dedc]
00b53f36 d85dd0               fcomp dword ptr [ebp - 0x30]
00b53f39 dfe0                 fnstsw ax
00b53f3b f6c405               test ah, 5
00b53f3e 7b0e                 jnp 0xb53f4e
00b53f40 5f                   pop edi
00b53f41 5e                   pop esi
00b53f42 b801000000           mov eax, 1
00b53f47 5b                   pop ebx
00b53f48 8be5                 mov esp, ebp
00b53f4a 5d                   pop ebp
00b53f4b c20400               ret 4
00b53f4e 5f                   pop edi
00b53f4f 5e                   pop esi
00b53f50 33c0                 xor eax, eax
00b53f52 5b                   pop ebx
00b53f53 8be5                 mov esp, ebp
00b53f55 5d                   pop ebp
00b53f56 c20400               ret 4
