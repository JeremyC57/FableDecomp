006aa2e0 CThingPlayerCreature::DrawPreparePrimitivesForRendering DIFFER(808v1080) flags= []
@@ retail 0-2 built 0-2
 - 006aa2e0  83ec44                 sub esp, 0x44
 - 006aa2e3  c744240400000000       mov dword ptr [esp + 4], 0
 + 006aa2e0  83ec40                 sub esp, 0x40
 + 006aa2e3  c7042400000000         mov dword ptr [esp], 0
   006aa2eb  a140f93701             mov eax, dword ptr [0x137f940]
   006aa2f0  85c0                   test eax, eax
@@ retail 9-10 built 9-10
   006aa2f7  32c0                   xor al, al
   006aa2f9  5e                     pop esi
 - 006aa2fa  83c444                 add esp, 0x44
 + 006aa2f9  83c440                 add esp, 0x40
   006aa2fd  c20400                 ret 4
   006aa300  55                     push ebp
@@ retail 11-14 built 11-14
   006aa2fa  83c444                 add esp, 0x44
   006aa2fd  c20400                 ret 4
 - 006aa300  55                     push ebp
 - 006aa301  8b6c2450               mov ebp, dword ptr [esp + 0x50]
 - 006aa305  55                     push ebp
 + 006aa2ff  57                     push edi
 + 006aa300  8b7c244c               mov edi, dword ptr [esp + 0x4c]
 + 006aa304  57                     push edi
   006aa306  8bce                   mov ecx, esi
   006aa308  e83399fbff             call 0x663c40
@@ retail 17-17 built 17-23
   006aa308  e83399fbff             call 0x663c40
   006aa30d  84c0                   test al, al
 + 006aa30e  7508                   jne 0x6aa318
 + 006aa310  5f                     pop edi
 + 006aa311  5e                     pop esi
 + 006aa312  83c440                 add esp, 0x40
 + 006aa315  c20400                 ret 4
 + 006aa318  f7462400000100         test dword ptr [esi + 0x24], 0x10000
   006aa30f  0f84f9030000           je 0x6aa70e
   006aa315  f7462400000100         test dword ptr [esi + 0x24], 0x10000
@@ retail 18-20 built 24-28
   006aa30d  84c0                   test al, al
   006aa30f  0f84f9030000           je 0x6aa70e
 - 006aa315  f7462400000100         test dword ptr [esi + 0x24], 0x10000
 - 006aa31c  53                     push ebx
 + 006aa321  6a30                   push 0x30
 + 006aa323  8bce                   mov ecx, esi
 + 006aa325  e800000000             call 0x6aa32a
 + 006aa32a  8b10                   mov edx, dword ptr [eax]
   006aa31d  57                     push edi
   006aa31e  742e                   je 0x6aa34e
@@ retail 21-37 built 29-30
   006aa31c  53                     push ebx
   006aa31d  57                     push edi
 - 006aa31e  742e                   je 0x6aa34e
 - 006aa320  8d442418               lea eax, [esp + 0x18]
 - 006aa324  8d7e44                 lea edi, [esi + 0x44]
 - 006aa327  50                     push eax
 - 006aa328  8bcf                   mov ecx, edi
 - 006aa32a  c744241c30000000       mov dword ptr [esp + 0x1c], 0x30
 - 006aa332  e8e94cd6ff             call 0x40f020
 - 006aa337  8b7f04                 mov edi, dword ptr [edi + 4]
 - 006aa33a  3bc7                   cmp eax, edi
 - 006aa33c  7405                   je 0x6aa343
 - 006aa33e  833830                 cmp dword ptr [eax], 0x30
 - 006aa341  7e02                   jle 0x6aa345
 - 006aa343  8bc7                   mov eax, edi
 - 006aa345  8b4804                 mov ecx, dword ptr [eax + 4]
 - 006aa348  8b11                   mov edx, dword ptr [ecx]
 - 006aa34a  55                     push ebp
 + 006aa32d  8bc8                   mov ecx, eax
   006aa34b  ff522c                 call dword ptr [edx + 0x2c]
   006aa34e  8a4634                 mov al, byte ptr [esi + 0x34]
@@ retail 41-46 built 34-36
   006aa351  84c0                   test al, al
   006aa353  7931                   jns 0x6aa386
 - 006aa355  8d442418               lea eax, [esp + 0x18]
 - 006aa359  8d7e44                 lea edi, [esi + 0x44]
 - 006aa35c  50                     push eax
 - 006aa35d  8bcf                   mov ecx, edi
 - 006aa35f  c744241ca7000000       mov dword ptr [esp + 0x1c], 0xa7
 + 006aa339  68a7000000             push 0xa7
 + 006aa33e  8bce                   mov ecx, esi
   006aa367  e8b44cd6ff             call 0x40f020
   006aa36c  8b7f04                 mov edi, dword ptr [edi + 4]
@@ retail 47-56 built 37-40
   006aa35f  c744241ca7000000       mov dword ptr [esp + 0x1c], 0xa7
   006aa367  e8b44cd6ff             call 0x40f020
 - 006aa36c  8b7f04                 mov edi, dword ptr [edi + 4]
 - 006aa36f  3bc7                   cmp eax, edi
 - 006aa371  7408                   je 0x6aa37b
 - 006aa373  8138a7000000           cmp dword ptr [eax], 0xa7
 - 006aa379  7e02                   jle 0x6aa37d
 - 006aa37b  8bc7                   mov eax, edi
 - 006aa37d  8b4804                 mov ecx, dword ptr [eax + 4]
 - 006aa380  8b11                   mov edx, dword ptr [ecx]
 - 006aa382  55                     push ebp
 + 006aa345  8b10                   mov edx, dword ptr [eax]
 + 006aa347  57                     push edi
 + 006aa348  8bc8                   mov ecx, eax
   006aa383  ff522c                 call dword ptr [edx + 0x2c]
   006aa386  8b462c                 mov eax, dword ptr [esi + 0x2c]
@@ retail 58-58 built 42-43
   006aa383  ff522c                 call dword ptr [edx + 0x2c]
   006aa386  8b462c                 mov eax, dword ptr [esi + 0x2c]
 + 006aa350  53                     push ebx
   006aa389  bb00010000             mov ebx, 0x100
   006aa38e  85c3                   test ebx, eax
@@ retail 61-66 built 46-48
   006aa38e  85c3                   test ebx, eax
   006aa390  742e                   je 0x6aa3c0
 - 006aa392  8d442418               lea eax, [esp + 0x18]
 - 006aa396  8d7e44                 lea edi, [esi + 0x44]
 - 006aa399  50                     push eax
 - 006aa39a  8bcf                   mov ecx, edi
 - 006aa39c  c744241c68000000       mov dword ptr [esp + 0x1c], 0x68
 + 006aa35a  6a68                   push 0x68
 + 006aa35c  8bce                   mov ecx, esi
   006aa3a4  e8774cd6ff             call 0x40f020
   006aa3a9  8b7f04                 mov edi, dword ptr [edi + 4]
@@ retail 67-76 built 49-52
   006aa39c  c744241c68000000       mov dword ptr [esp + 0x1c], 0x68
   006aa3a4  e8774cd6ff             call 0x40f020
 - 006aa3a9  8b7f04                 mov edi, dword ptr [edi + 4]
 - 006aa3ac  3bc7                   cmp eax, edi
 - 006aa3ae  7405                   je 0x6aa3b5
 - 006aa3b0  833868                 cmp dword ptr [eax], 0x68
 - 006aa3b3  7e02                   jle 0x6aa3b7
 - 006aa3b5  8bc7                   mov eax, edi
 - 006aa3b7  8b4804                 mov ecx, dword ptr [eax + 4]
 - 006aa3ba  8b11                   mov edx, dword ptr [ecx]
 - 006aa3bc  55                     push ebp
 + 006aa363  8b10                   mov edx, dword ptr [eax]
 + 006aa365  57                     push edi
 + 006aa366  8bc8                   mov ecx, eax
   006aa3bd  ff522c                 call dword ptr [edx + 0x2c]
   006aa3c0  8b4638                 mov eax, dword ptr [esi + 0x38]
@@ retail 80-85 built 56-58
   006aa3c3  f6c408                 test ah, 8
   006aa3c6  7431                   je 0x6aa3f9
 - 006aa3c8  8d442418               lea eax, [esp + 0x18]
 - 006aa3cc  8d7e44                 lea edi, [esi + 0x44]
 - 006aa3cf  50                     push eax
 - 006aa3d0  8bcf                   mov ecx, edi
 - 006aa3d2  c744241ccb000000       mov dword ptr [esp + 0x1c], 0xcb
 + 006aa373  68cb000000             push 0xcb
 + 006aa378  8bce                   mov ecx, esi
   006aa3da  e8414cd6ff             call 0x40f020
   006aa3df  8b7f04                 mov edi, dword ptr [edi + 4]
@@ retail 86-95 built 59-62
   006aa3d2  c744241ccb000000       mov dword ptr [esp + 0x1c], 0xcb
   006aa3da  e8414cd6ff             call 0x40f020
 - 006aa3df  8b7f04                 mov edi, dword ptr [edi + 4]
 - 006aa3e2  3bc7                   cmp eax, edi
 - 006aa3e4  7408                   je 0x6aa3ee
 - 006aa3e6  8138cb000000           cmp dword ptr [eax], 0xcb
 - 006aa3ec  7e02                   jle 0x6aa3f0
 - 006aa3ee  8bc7                   mov eax, edi
 - 006aa3f0  8b4804                 mov ecx, dword ptr [eax + 4]
 - 006aa3f3  8b11                   mov edx, dword ptr [ecx]
 - 006aa3f5  55                     push ebp
 + 006aa37f  8b10                   mov edx, dword ptr [eax]
 + 006aa381  57                     push edi
 + 006aa382  8bc8                   mov ecx, eax
   006aa3f6  ff522c                 call dword ptr [edx + 0x2c]
   006aa3f9  855e3c                 test dword ptr [esi + 0x3c], ebx
@@ retail 98-103 built 65-67
   006aa3f9  855e3c                 test dword ptr [esi + 0x3c], ebx
   006aa3fc  7431                   je 0x6aa42f
 - 006aa3fe  8d442418               lea eax, [esp + 0x18]
 - 006aa402  8d7e44                 lea edi, [esi + 0x44]
 - 006aa405  50                     push eax
 - 006aa406  8bcf                   mov ecx, edi
 - 006aa408  c744241ce8000000       mov dword ptr [esp + 0x1c], 0xe8
 + 006aa38c  68e8000000             push 0xe8
 + 006aa391  8bce                   mov ecx, esi
   006aa410  e80b4cd6ff             call 0x40f020
   006aa415  8b7f04                 mov edi, dword ptr [edi + 4]
@@ retail 104-113 built 68-71
   006aa408  c744241ce8000000       mov dword ptr [esp + 0x1c], 0xe8
   006aa410  e80b4cd6ff             call 0x40f020
 - 006aa415  8b7f04                 mov edi, dword ptr [edi + 4]
 - 006aa418  3bc7                   cmp eax, edi
 - 006aa41a  7408                   je 0x6aa424
 - 006aa41c  8138e8000000           cmp dword ptr [eax], 0xe8
 - 006aa422  7e02                   jle 0x6aa426
 - 006aa424  8bc7                   mov eax, edi
 - 006aa426  8b4804                 mov ecx, dword ptr [eax + 4]
 - 006aa429  8b11                   mov edx, dword ptr [ecx]
 - 006aa42b  55                     push ebp
 + 006aa398  8b10                   mov edx, dword ptr [eax]
 + 006aa39a  57                     push edi
 + 006aa39b  8bc8                   mov ecx, eax
   006aa42c  ff522c                 call dword ptr [edx + 0x2c]
   006aa42f  f6464002               test byte ptr [esi + 0x40], 2
@@ retail 116-121 built 74-76
   006aa42f  f6464002               test byte ptr [esi + 0x40], 2
   006aa433  7431                   je 0x6aa466
 - 006aa435  8d442418               lea eax, [esp + 0x18]
 - 006aa439  8d7e44                 lea edi, [esi + 0x44]
 - 006aa43c  50                     push eax
 - 006aa43d  8bcf                   mov ecx, edi
 - 006aa43f  c744241c01010000       mov dword ptr [esp + 0x1c], 0x101
 + 006aa3a6  6801010000             push 0x101
 + 006aa3ab  8bce                   mov ecx, esi
   006aa447  e8d44bd6ff             call 0x40f020
   006aa44c  8b7f04                 mov edi, dword ptr [edi + 4]
@@ retail 122-131 built 77-80
   006aa43f  c744241c01010000       mov dword ptr [esp + 0x1c], 0x101
   006aa447  e8d44bd6ff             call 0x40f020
 - 006aa44c  8b7f04                 mov edi, dword ptr [edi + 4]
 - 006aa44f  3bc7                   cmp eax, edi
 - 006aa451  7408                   je 0x6aa45b
 - 006aa453  813801010000           cmp dword ptr [eax], 0x101
 - 006aa459  7e02                   jle 0x6aa45d
 - 006aa45b  8bc7                   mov eax, edi
 - 006aa45d  8b4804                 mov ecx, dword ptr [eax + 4]
 - 006aa460  8b11                   mov edx, dword ptr [ecx]
 - 006aa462  55                     push ebp
 + 006aa3b2  8b10                   mov edx, dword ptr [eax]
 + 006aa3b4  57                     push edi
 + 006aa3b5  8bc8                   mov ecx, eax
   006aa463  ff522c                 call dword ptr [edx + 0x2c]
   006aa466  a040863b01             mov al, byte ptr [0x13b8640]
@@ retail 135-136 built 84-86
   006aa46b  84c0                   test al, al
   006aa46d  0f84e3010000           je 0x6aa656
 - 006aa473  8b4504                 mov eax, dword ptr [ebp + 4]
 + 006aa3c7  8b4704                 mov eax, dword ptr [edi + 4]
 + 006aa3ca  55                     push ebp
   006aa476  50                     push eax
   006aa477  8d4c2420               lea ecx, [esp + 0x20]
@@ retail 137-138 built 87-88
   006aa473  8b4504                 mov eax, dword ptr [ebp + 4]
   006aa476  50                     push eax
 - 006aa477  8d4c2420               lea ecx, [esp + 0x20]
 + 006aa3cc  8d4c241c               lea ecx, [esp + 0x1c]
   006aa47b  51                     push ecx
   006aa47c  8b4e64                 mov ecx, dword ptr [esi + 0x64]
@@ retail 141-142 built 91-92
   006aa47c  8b4e64                 mov ecx, dword ptr [esi + 0x64]
   006aa47f  e8dc6be1ff             call 0x4c1060
 - 006aa484  d9442424               fld dword ptr [esp + 0x24]
 + 006aa3d9  d9442420               fld dword ptr [esp + 0x20]
   006aa488  d80518532501           fadd dword ptr [0x1255318]
   006aa48e  8bce                   mov ecx, esi
@@ retail 144-145 built 94-95
   006aa488  d80518532501           fadd dword ptr [0x1255318]
   006aa48e  8bce                   mov ecx, esi
 - 006aa490  d95c2424               fstp dword ptr [esp + 0x24]
 + 006aa3e5  d95c2420               fstp dword ptr [esp + 0x20]
   006aa494  e837cfe1ff             call 0x4c73d0
   006aa499  50                     push eax
@@ retail 150-151 built 100-101
   006aa49c  e87f78fbff             call 0x661d20
   006aa4a1  8bd0                   mov edx, eax
 - 006aa4a3  8d4c2418               lea ecx, [esp + 0x18]
 + 006aa3f8  8d4c2414               lea ecx, [esp + 0x14]
   006aa4a7  e854bad5ff             call 0x405f00
   006aa4ac  8bce                   mov ecx, esi
@@ retail 152-152 built 102-103
   006aa4a3  8d4c2418               lea ecx, [esp + 0x18]
   006aa4a7  e854bad5ff             call 0x405f00
 + 006aa401  b0ff                   mov al, 0xff
   006aa4ac  8bce                   mov ecx, esi
   006aa4ae  c6442412ff             mov byte ptr [esp + 0x12], 0xff
@@ retail 153-157 built 104-108
   006aa4a7  e854bad5ff             call 0x405f00
   006aa4ac  8bce                   mov ecx, esi
 - 006aa4ae  c6442412ff             mov byte ptr [esp + 0x12], 0xff
 - 006aa4b3  c6442411ff             mov byte ptr [esp + 0x11], 0xff
 - 006aa4b8  c644241000             mov byte ptr [esp + 0x10], 0
 - 006aa4bd  c6442413ff             mov byte ptr [esp + 0x13], 0xff
 + 006aa405  88442456               mov byte ptr [esp + 0x56], al
 + 006aa409  88442455               mov byte ptr [esp + 0x55], al
 + 006aa40d  c644245400             mov byte ptr [esp + 0x54], 0
 + 006aa412  88442457               mov byte ptr [esp + 0x57], al
   006aa4c2  e8a9d5e1ff             call 0x4c7a70
   006aa4c7  8b8044610100           mov eax, dword ptr [eax + 0x16144]
@@ retail 161-163 built 112-114
   006aa4cd  8b561c                 mov edx, dword ptr [esi + 0x1c]
   006aa4d0  8b4e18                 mov ecx, dword ptr [esi + 0x18]
 - 006aa4d3  8b7d34                 mov edi, dword ptr [ebp + 0x34]
 - 006aa4d6  8b3f                   mov edi, dword ptr [edi]
 + 006aa427  8b5f34                 mov ebx, dword ptr [edi + 0x34]
 + 006aa42a  8b1b                   mov ebx, dword ptr [ebx]
   006aa4d8  6a00                   push 0
   006aa4da  6a01                   push 1
@@ retail 165-167 built 116-118
   006aa4d8  6a00                   push 0
   006aa4da  6a01                   push 1
 - 006aa4dc  8d5c2418               lea ebx, [esp + 0x18]
 - 006aa4e0  53                     push ebx
 + 006aa430  8d6c245c               lea ebp, [esp + 0x5c]
 + 006aa434  55                     push ebp
   006aa4e1  50                     push eax
   006aa4e2  8d442428               lea eax, [esp + 0x28]
@@ retail 168-169 built 119-120
   006aa4e0  53                     push ebx
   006aa4e1  50                     push eax
 - 006aa4e2  8d442428               lea eax, [esp + 0x28]
 + 006aa436  8d442424               lea eax, [esp + 0x24]
   006aa4e6  50                     push eax
   006aa4e7  680000803f             push 0x3f800000
@@ retail 171-172 built 122-123
   006aa4e6  50                     push eax
   006aa4e7  680000803f             push 0x3f800000
 - 006aa4ec  8d442434               lea eax, [esp + 0x34]
 + 006aa440  8d442430               lea eax, [esp + 0x30]
   006aa4f0  50                     push eax
   006aa4f1  52                     push edx
@@ retail 175-176 built 126-127
   006aa4f1  52                     push edx
   006aa4f2  51                     push ecx
 - 006aa4f3  8d4c244c               lea ecx, [esp + 0x4c]
 + 006aa447  8d4c2448               lea ecx, [esp + 0x48]
   006aa4f7  e8248ce1ff             call 0x4c3120
   006aa4fc  8b4d34                 mov ecx, dword ptr [ebp + 0x34]
@@ retail 177-178 built 128-129
   006aa4f3  8d4c244c               lea ecx, [esp + 0x4c]
   006aa4f7  e8248ce1ff             call 0x4c3120
 - 006aa4fc  8b4d34                 mov ecx, dword ptr [ebp + 0x34]
 + 006aa450  8b4f34                 mov ecx, dword ptr [edi + 0x34]
   006aa4ff  50                     push eax
   006aa500  ff5750                 call dword ptr [edi + 0x50]
@@ retail 179-181 built 130-132
   006aa4fc  8b4d34                 mov ecx, dword ptr [ebp + 0x34]
   006aa4ff  50                     push eax
 - 006aa500  ff5750                 call dword ptr [edi + 0x50]
 - 006aa503  8d4c2448               lea ecx, [esp + 0x48]
 + 006aa454  ff5350                 call dword ptr [ebx + 0x50]
 + 006aa457  8d4c2444               lea ecx, [esp + 0x44]
   006aa507  e804102f00             call 0x99b510
   006aa50c  f7462c00002000         test dword ptr [esi + 0x2c], 0x200000
@@ retail 184-186 built 135-137
   006aa50c  f7462c00002000         test dword ptr [esi + 0x2c], 0x200000
   006aa513  745c                   je 0x6aa571
 - 006aa515  8d4c2410               lea ecx, [esp + 0x10]
 - 006aa519  8d7e44                 lea edi, [esi + 0x44]
 + 006aa469  8d4c2454               lea ecx, [esp + 0x54]
 + 006aa46d  8d5e44                 lea ebx, [esi + 0x44]
   006aa51c  51                     push ecx
   006aa51d  8bcf                   mov ecx, edi
@@ retail 187-189 built 138-140
   006aa519  8d7e44                 lea edi, [esi + 0x44]
   006aa51c  51                     push ecx
 - 006aa51d  8bcf                   mov ecx, edi
 - 006aa51f  c744241475000000       mov dword ptr [esp + 0x14], 0x75
 + 006aa471  8bcb                   mov ecx, ebx
 + 006aa473  c744245875000000       mov dword ptr [esp + 0x58], 0x75
   006aa527  e8f44ad6ff             call 0x40f020
   006aa52c  8b7f04                 mov edi, dword ptr [edi + 4]
@@ retail 190-192 built 141-143
   006aa51f  c744241475000000       mov dword ptr [esp + 0x14], 0x75
   006aa527  e8f44ad6ff             call 0x40f020
 - 006aa52c  8b7f04                 mov edi, dword ptr [edi + 4]
 - 006aa52f  3bc7                   cmp eax, edi
 + 006aa480  8b5b04                 mov ebx, dword ptr [ebx + 4]
 + 006aa483  3bc3                   cmp eax, ebx
   006aa531  7405                   je 0x6aa538
   006aa533  833875                 cmp dword ptr [eax], 0x75
@@ retail 195-196 built 146-147
   006aa533  833875                 cmp dword ptr [eax], 0x75
   006aa536  7e02                   jle 0x6aa53a
 - 006aa538  8bc7                   mov eax, edi
 + 006aa48c  8bc3                   mov eax, ebx
   006aa53a  8b6804                 mov ebp, dword ptr [eax + 4]
   006aa53d  8d7d0c                 lea edi, [ebp + 0xc]
@@ retail 197-199 built 148-150
   006aa538  8bc7                   mov eax, edi
   006aa53a  8b6804                 mov ebp, dword ptr [eax + 4]
 - 006aa53d  8d7d0c                 lea edi, [ebp + 0xc]
 - 006aa540  8bcf                   mov ecx, edi
 + 006aa491  8d5d0c                 lea ebx, [ebp + 0xc]
 + 006aa494  8bcb                   mov ecx, ebx
   006aa542  e809763500             call 0xa01b50
   006aa547  85c0                   test eax, eax
@@ retail 202-202 built 153-155
   006aa547  85c0                   test eax, eax
   006aa549  742a                   je 0x6aa575
 + 006aa49f  8bcb                   mov ecx, ebx
 + 006aa4a1  e800000000             call 0x6aa4a6
   006aa54b  f6809100000001         test byte ptr [eax + 0x91], 1
   006aa552  7521                   jne 0x6aa575
@@ retail 204-206 built 157-159
   006aa54b  f6809100000001         test byte ptr [eax + 0x91], 1
   006aa552  7521                   jne 0x6aa575
 - 006aa554  8bcf                   mov ecx, edi
 - 006aa556  c744241401000000       mov dword ptr [esp + 0x14], 1
 + 006aa4af  8bcb                   mov ecx, ebx
 + 006aa4b1  c744241001000000       mov dword ptr [esp + 0x10], 1
   006aa55e  e8ed753500             call 0xa01b50
   006aa563  8b4870                 mov ecx, dword ptr [eax + 0x70]
@@ retail 213-215 built 166-168
   006aa56d  b301                   mov bl, 1
   006aa56f  eb0a                   jmp 0x6aa57b
 - 006aa571  8b6c2458               mov ebp, dword ptr [esp + 0x58]
 - 006aa575  8b4c2458               mov ecx, dword ptr [esp + 0x58]
 + 006aa4cc  8b6c2454               mov ebp, dword ptr [esp + 0x54]
 + 006aa4d0  8b4c2454               mov ecx, dword ptr [esp + 0x54]
   006aa579  32db                   xor bl, bl
   006aa57b  f644241401             test byte ptr [esp + 0x14], 1
@@ retail 216-217 built 169-170
   006aa575  8b4c2458               mov ecx, dword ptr [esp + 0x58]
   006aa579  32db                   xor bl, bl
 - 006aa57b  f644241401             test byte ptr [esp + 0x14], 1
 + 006aa4d6  f644241001             test byte ptr [esp + 0x10], 1
   006aa580  740e                   je 0x6aa590
   006aa582  85c9                   test ecx, ecx
@@ retail 226-227 built 179-180
   006aa590  84db                   test bl, bl
   006aa592  0f84b1000000           je 0x6aa649
 - 006aa598  d9442424               fld dword ptr [esp + 0x24]
 + 006aa4f3  d9442420               fld dword ptr [esp + 0x20]
   006aa59c  8d4d0c                 lea ecx, [ebp + 0xc]
   006aa59f  d805a0052301           fadd dword ptr [0x12305a0]
@@ retail 229-230 built 182-183
   006aa59c  8d4d0c                 lea ecx, [ebp + 0xc]
   006aa59f  d805a0052301           fadd dword ptr [0x12305a0]
 - 006aa5a5  d95c2424               fstp dword ptr [esp + 0x24]
 + 006aa500  d95c2420               fstp dword ptr [esp + 0x20]
   006aa5a9  e8a2753500             call 0xa01b50
   006aa5ae  8b7870                 mov edi, dword ptr [eax + 0x70]
@@ retail 231-233 built 184-186
   006aa5a5  d95c2424               fstp dword ptr [esp + 0x24]
   006aa5a9  e8a2753500             call 0xa01b50
 - 006aa5ae  8b7870                 mov edi, dword ptr [eax + 0x70]
 - 006aa5b1  85ff                   test edi, edi
 + 006aa509  8b5870                 mov ebx, dword ptr [eax + 0x70]
 + 006aa50c  85db                   test ebx, ebx
   006aa5b3  7403                   je 0x6aa5b8
   006aa5b5  ff4704                 inc dword ptr [edi + 4]
@@ retail 234-237 built 187-190
   006aa5b1  85ff                   test edi, edi
   006aa5b3  7403                   je 0x6aa5b8
 - 006aa5b5  ff4704                 inc dword ptr [edi + 4]
 - 006aa5b8  8b17                   mov edx, dword ptr [edi]
 - 006aa5ba  8d442414               lea eax, [esp + 0x14]
 + 006aa510  ff4304                 inc dword ptr [ebx + 4]
 + 006aa513  8b13                   mov edx, dword ptr [ebx]
 + 006aa515  8d442410               lea eax, [esp + 0x10]
   006aa5be  50                     push eax
   006aa5bf  8d442414               lea eax, [esp + 0x14]
@@ retail 238-239 built 191-192
   006aa5ba  8d442414               lea eax, [esp + 0x14]
   006aa5be  50                     push eax
 - 006aa5bf  8d442414               lea eax, [esp + 0x14]
 + 006aa51a  8d442458               lea eax, [esp + 0x58]
   006aa5c3  50                     push eax
   006aa5c4  8bcf                   mov ecx, edi
@@ retail 240-241 built 193-194
   006aa5bf  8d442414               lea eax, [esp + 0x14]
   006aa5c3  50                     push eax
 - 006aa5c4  8bcf                   mov ecx, edi
 + 006aa51f  8bcb                   mov ecx, ebx
   006aa5c6  ff5228                 call dword ptr [edx + 0x28]
   006aa5c9  8bc8                   mov ecx, eax
@@ retail 244-245 built 197-198
   006aa5c9  8bc8                   mov ecx, eax
   006aa5cb  e8c0b7d6ff             call 0x415d90
 - 006aa5d0  ff4f04                 dec dword ptr [edi + 4]
 + 006aa52b  ff4b04                 dec dword ptr [ebx + 4]
   006aa5d3  7507                   jne 0x6aa5dc
   006aa5d5  8b17                   mov edx, dword ptr [edi]
@@ retail 246-248 built 199-201
   006aa5d0  ff4f04                 dec dword ptr [edi + 4]
   006aa5d3  7507                   jne 0x6aa5dc
 - 006aa5d5  8b17                   mov edx, dword ptr [edi]
 - 006aa5d7  8bcf                   mov ecx, edi
 + 006aa530  8b13                   mov edx, dword ptr [ebx]
 + 006aa532  8bcb                   mov ecx, ebx
   006aa5d9  ff5204                 call dword ptr [edx + 4]
   006aa5dc  8bce                   mov ecx, esi
@@ retail 249-249 built 202-203
   006aa5d7  8bcf                   mov ecx, edi
   006aa5d9  ff5204                 call dword ptr [edx + 4]
 + 006aa537  b0ff                   mov al, 0xff
   006aa5dc  8bce                   mov ecx, esi
   006aa5de  c6442412ff             mov byte ptr [esp + 0x12], 0xff
@@ retail 250-254 built 204-208
   006aa5d9  ff5204                 call dword ptr [edx + 4]
   006aa5dc  8bce                   mov ecx, esi
 - 006aa5de  c6442412ff             mov byte ptr [esp + 0x12], 0xff
 - 006aa5e3  c6442411ff             mov byte ptr [esp + 0x11], 0xff
 - 006aa5e8  c644241000             mov byte ptr [esp + 0x10], 0
 - 006aa5ed  c6442413ff             mov byte ptr [esp + 0x13], 0xff
 + 006aa53b  88442456               mov byte ptr [esp + 0x56], al
 + 006aa53f  88442455               mov byte ptr [esp + 0x55], al
 + 006aa543  c644245400             mov byte ptr [esp + 0x54], 0
 + 006aa548  88442457               mov byte ptr [esp + 0x57], al
   006aa5f2  e879d4e1ff             call 0x4c7a70
   006aa5f7  8b8044610100           mov eax, dword ptr [eax + 0x16144]
@@ retail 258-259 built 212-212
   006aa5fd  8b561c                 mov edx, dword ptr [esi + 0x1c]
   006aa600  8b4e18                 mov ecx, dword ptr [esi + 0x18]
 - 006aa603  8b7c2458               mov edi, dword ptr [esp + 0x58]
   006aa607  8b5f34                 mov ebx, dword ptr [edi + 0x34]
   006aa60a  8b1b                   mov ebx, dword ptr [ebx]
@@ retail 263-264 built 216-217
   006aa60c  6a00                   push 0
   006aa60e  6a01                   push 1
 - 006aa610  8d6c2418               lea ebp, [esp + 0x18]
 + 006aa566  8d6c245c               lea ebp, [esp + 0x5c]
   006aa614  55                     push ebp
   006aa615  50                     push eax
@@ retail 266-267 built 219-220
   006aa614  55                     push ebp
   006aa615  50                     push eax
 - 006aa616  8d442424               lea eax, [esp + 0x24]
 + 006aa56c  8d442420               lea eax, [esp + 0x20]
   006aa61a  50                     push eax
   006aa61b  680ad7a33d             push 0x3da3d70a
@@ retail 269-270 built 222-223
   006aa61a  50                     push eax
   006aa61b  680ad7a33d             push 0x3da3d70a
 - 006aa620  8d442434               lea eax, [esp + 0x34]
 + 006aa576  8d442430               lea eax, [esp + 0x30]
   006aa624  50                     push eax
   006aa625  52                     push edx
@@ retail 273-274 built 226-227
   006aa625  52                     push edx
   006aa626  51                     push ecx
 - 006aa627  8d4c244c               lea ecx, [esp + 0x4c]
 + 006aa57d  8d4c2448               lea ecx, [esp + 0x48]
   006aa62b  e8f08ae1ff             call 0x4c3120
   006aa630  8b4f34                 mov ecx, dword ptr [edi + 0x34]
@@ retail 278-279 built 231-234
   006aa633  50                     push eax
   006aa634  ff5350                 call dword ptr [ebx + 0x50]
 - 006aa637  8d4c2448               lea ecx, [esp + 0x48]
 + 006aa58d  8d4c2444               lea ecx, [esp + 0x44]
 + 006aa591  e800000000             call 0x6aa596
 + 006aa596  8d4c2410               lea ecx, [esp + 0x10]
   006aa63b  e8d00e2f00             call 0x99b510
   006aa640  8d4c2414               lea ecx, [esp + 0x14]
@@ retail 282-285 built 237-238
   006aa640  8d4c2414               lea ecx, [esp + 0x14]
   006aa644  e8c70e2f00             call 0x99b510
 - 006aa649  8d4c2418               lea ecx, [esp + 0x18]
 - 006aa64d  e8be0e2f00             call 0x99b510
 - 006aa652  8b6c2458               mov ebp, dword ptr [esp + 0x58]
 + 006aa5a8  5d                     pop ebp
   006aa656  a0808a3b01             mov al, byte ptr [0x13b8a80]
   006aa65b  84c0                   test al, al
@@ retail 287-287 built 240-241
   006aa656  a0808a3b01             mov al, byte ptr [0x13b8a80]
   006aa65b  84c0                   test al, al
 + 006aa5b0  5b                     pop ebx
   006aa65d  7437                   je 0x6aa696
   006aa65f  f7462400004000         test dword ptr [esi + 0x24], 0x400000
@@ retail 290-295 built 244-246
   006aa65f  f7462400004000         test dword ptr [esi + 0x24], 0x400000
   006aa666  742e                   je 0x6aa696
 - 006aa668  8d4c2458               lea ecx, [esp + 0x58]
 - 006aa66c  8d7e44                 lea edi, [esi + 0x44]
 - 006aa66f  51                     push ecx
 - 006aa670  8bcf                   mov ecx, edi
 - 006aa672  c744245c36000000       mov dword ptr [esp + 0x5c], 0x36
 + 006aa5bc  6a36                   push 0x36
 + 006aa5be  8bce                   mov ecx, esi
   006aa67a  e8a149d6ff             call 0x40f020
   006aa67f  8b7f04                 mov edi, dword ptr [edi + 4]
@@ retail 296-305 built 247-250
   006aa672  c744245c36000000       mov dword ptr [esp + 0x5c], 0x36
   006aa67a  e8a149d6ff             call 0x40f020
 - 006aa67f  8b7f04                 mov edi, dword ptr [edi + 4]
 - 006aa682  3bc7                   cmp eax, edi
 - 006aa684  7405                   je 0x6aa68b
 - 006aa686  833836                 cmp dword ptr [eax], 0x36
 - 006aa689  7e02                   jle 0x6aa68d
 - 006aa68b  8bc7                   mov eax, edi
 - 006aa68d  8b4804                 mov ecx, dword ptr [eax + 4]
 - 006aa690  8b11                   mov edx, dword ptr [ecx]
 - 006aa692  55                     push ebp
 + 006aa5c5  8b10                   mov edx, dword ptr [eax]
 + 006aa5c7  57                     push edi
 + 006aa5c8  8bc8                   mov ecx, eax
   006aa693  ff522c                 call dword ptr [edx + 0x2c]
   006aa696  f6462010               test byte ptr [esi + 0x20], 0x10
@@ retail 307-308 built 252-252
   006aa693  ff522c                 call dword ptr [edx + 0x2c]
   006aa696  f6462010               test byte ptr [esi + 0x20], 0x10
 - 006aa69a  bb04000000             mov ebx, 4
   006aa69f  7429                   je 0x6aa6ca
   006aa6a1  8d442458               lea eax, [esp + 0x58]
@@ retail 309-314 built 253-255
   006aa69a  bb04000000             mov ebx, 4
   006aa69f  7429                   je 0x6aa6ca
 - 006aa6a1  8d442458               lea eax, [esp + 0x58]
 - 006aa6a5  8d7e44                 lea edi, [esi + 0x44]
 - 006aa6a8  50                     push eax
 - 006aa6a9  8bcf                   mov ecx, edi
 - 006aa6ab  895c245c               mov dword ptr [esp + 0x5c], ebx
 + 006aa5d3  6a04                   push 4
 + 006aa5d5  8bce                   mov ecx, esi
   006aa6af  e86c49d6ff             call 0x40f020
   006aa6b4  8b7f04                 mov edi, dword ptr [edi + 4]
@@ retail 315-317 built 256-261
   006aa6ab  895c245c               mov dword ptr [esp + 0x5c], ebx
   006aa6af  e86c49d6ff             call 0x40f020
 - 006aa6b4  8b7f04                 mov edi, dword ptr [edi + 4]
 - 006aa6b7  3bc7                   cmp eax, edi
 + 006aa5dc  8b10                   mov edx, dword ptr [eax]
 + 006aa5de  57                     push edi
 + 006aa5df  8bc8                   mov ecx, eax
 + 006aa5e1  ff522c                 call dword ptr [edx + 0x2c]
 + 006aa5e4  f6464004               test byte ptr [esi + 0x40], 4
   006aa6b9  7404                   je 0x6aa6bf
   006aa6bb  3918                   cmp dword ptr [eax], ebx
@@ retail 318-324 built 262-268
   006aa6b7  3bc7                   cmp eax, edi
   006aa6b9  7404                   je 0x6aa6bf
 - 006aa6bb  3918                   cmp dword ptr [eax], ebx
 - 006aa6bd  7e02                   jle 0x6aa6c1
 - 006aa6bf  8bc7                   mov eax, edi
 - 006aa6c1  8b4804                 mov ecx, dword ptr [eax + 4]
 - 006aa6c4  8b11                   mov edx, dword ptr [ecx]
 - 006aa6c6  55                     push ebp
 + 006aa5ea  6802010000             push 0x102
 + 006aa5ef  8bce                   mov ecx, esi
 + 006aa5f1  e800000000             call 0x6aa5f6
 + 006aa5f6  8b10                   mov edx, dword ptr [eax]
 + 006aa5f8  57                     push edi
 + 006aa5f9  8bc8                   mov ecx, eax
   006aa6c7  ff522c                 call dword ptr [edx + 0x2c]
   006aa6ca  8a4640                 mov al, byte ptr [esi + 0x40]
@@ retail 325-326 built 269-269
   006aa6c6  55                     push ebp
   006aa6c7  ff522c                 call dword ptr [edx + 0x2c]
 - 006aa6ca  8a4640                 mov al, byte ptr [esi + 0x40]
   006aa6cd  5f                     pop edi
   006aa6ce  84c3                   test bl, al
@@ retail 327-347 built 270-270
   006aa6ca  8a4640                 mov al, byte ptr [esi + 0x40]
   006aa6cd  5f                     pop edi
 - 006aa6ce  84c3                   test bl, al
 - 006aa6d0  5b                     pop ebx
 - 006aa6d1  7431                   je 0x6aa704
 - 006aa6d3  8d442450               lea eax, [esp + 0x50]
 - 006aa6d7  83c644                 add esi, 0x44
 - 006aa6da  50                     push eax
 - 006aa6db  8bce                   mov ecx, esi
 - 006aa6dd  c744245402010000       mov dword ptr [esp + 0x54], 0x102
 - 006aa6e5  e83649d6ff             call 0x40f020
 - 006aa6ea  8b7604                 mov esi, dword ptr [esi + 4]
 - 006aa6ed  3bc6                   cmp eax, esi
 - 006aa6ef  7408                   je 0x6aa6f9
 - 006aa6f1  813802010000           cmp dword ptr [eax], 0x102
 - 006aa6f7  7e02                   jle 0x6aa6fb
 - 006aa6f9  8bc6                   mov eax, esi
 - 006aa6fb  8b4804                 mov ecx, dword ptr [eax + 4]
 - 006aa6fe  8b11                   mov edx, dword ptr [ecx]
 - 006aa700  55                     push ebp
 - 006aa701  ff522c                 call dword ptr [edx + 0x2c]
 - 006aa704  5d                     pop ebp
   006aa705  b001                   mov al, 1
   006aa707  5e                     pop esi
@@ retail 349-350 built 272-273
   006aa705  b001                   mov al, 1
   006aa707  5e                     pop esi
 - 006aa708  83c444                 add esp, 0x44
 + 006aa602  83c440                 add esp, 0x40
   006aa70b  c20400                 ret 4
   006aa70e  5d                     pop ebp
@@ retail 351-356 built 274-274
   006aa708  83c444                 add esp, 0x44
   006aa70b  c20400                 ret 4
 - 006aa70e  5d                     pop ebp
 - 006aa70f  32c0                   xor al, al
 - 006aa711  5e                     pop esi
 - 006aa712  83c444                 add esp, 0x44
 - 006aa715  c20400                 ret 4
DIFFLINES 227 retail_instr 356 built_instr 274
