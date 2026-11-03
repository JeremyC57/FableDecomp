00b087c0 55                   push ebp
00b087c1 8d6c249c             lea ebp, [esp - 0x64]
00b087c5 81ecdc000000         sub esp, 0xdc
00b087cb 56                   push esi
00b087cc 8bf1                 mov esi, ecx
00b087ce 8b4634               mov eax, dword ptr [esi + 0x34]
00b087d1 85c0                 test eax, eax
00b087d3 750e                 jne 0xb087e3
00b087d5 8b8680000000         mov eax, dword ptr [esi + 0x80]
00b087db 85c0                 test eax, eax
00b087dd 0f846a0b0000         je 0xb0934d
00b087e3 8b4624               mov eax, dword ptr [esi + 0x24]
00b087e6 85c0                 test eax, eax
00b087e8 57                   push edi
00b087e9 8b7d74               mov edi, dword ptr [ebp + 0x74]
00b087ec 7417                 je 0xb08805
00b087ee 8b456c               mov eax, dword ptr [ebp + 0x6c]
00b087f1 8b8e84000000         mov ecx, dword ptr [esi + 0x84]
00b087f7 8b5578               mov edx, dword ptr [ebp + 0x78]
00b087fa 50                   push eax
00b087fb 51                   push ecx
00b087fc 52                   push edx
00b087fd 57                   push edi
00b087fe 8bce                 mov ecx, esi
00b08800 e84bfaffff           call 0xb08250
00b08805 e8e621feff           call 0xaea9f0
00b0880a 894560               mov dword ptr [ebp + 0x60], eax
00b0880d db4560               fild dword ptr [ebp + 0x60]
00b08810 f6475008             test byte ptr [edi + 0x50], 8
00b08814 d95d60               fstp dword ptr [ebp + 0x60]
00b08817 db465c               fild dword ptr [esi + 0x5c]
00b0881a d80d00632601         fmul dword ptr [0x1266300]
00b08820 d95d54               fstp dword ptr [ebp + 0x54]
00b08823 db4660               fild dword ptr [esi + 0x60]
00b08826 d80d00632601         fmul dword ptr [0x1266300]
00b0882c d95d18               fstp dword ptr [ebp + 0x18]
00b0882f db869c000000         fild dword ptr [esi + 0x9c]
00b08835 d8456c               fadd dword ptr [ebp + 0x6c]
00b08838 d95d14               fstp dword ptr [ebp + 0x14]
00b0883b 7416                 je 0xb08853
00b0883d 8b475c               mov eax, dword ptr [edi + 0x5c]
00b08840 894558               mov dword ptr [ebp + 0x58], eax
00b08843 d94558               fld dword ptr [ebp + 0x58]
00b08846 d81ddcde2201         fcomp dword ptr [0x122dedc]
00b0884c dfe0                 fnstsw ax
00b0884e f6c405               test ah, 5
00b08851 7a09                 jp 0xb0885c
00b08853 c7455800000000       mov dword ptr [ebp + 0x58], 0
00b0885a eb17                 jmp 0xb08873
00b0885c d94558               fld dword ptr [ebp + 0x58]
00b0885f d81dd8de2201         fcomp dword ptr [0x122ded8]
00b08865 dfe0                 fnstsw ax
00b08867 f6c441               test ah, 0x41
00b0886a 7507                 jne 0xb08873
00b0886c c745580000803f       mov dword ptr [ebp + 0x58], 0x3f800000
00b08873 53                   push ebx
00b08874 8a9e93000000         mov bl, byte ptr [esi + 0x93]
00b0887a 84db                 test bl, bl
00b0887c 783c                 js 0xb088ba
00b0887e d94558               fld dword ptr [ebp + 0x58]
00b08881 8b4d58               mov ecx, dword ptr [ebp + 0x58]
00b08884 d81ddcde2201         fcomp dword ptr [0x122dedc]
00b0888a 894d10               mov dword ptr [ebp + 0x10], ecx
00b0888d dfe0                 fnstsw ax
00b0888f f6c405               test ah, 5
00b08892 7a09                 jp 0xb0889d
00b08894 c7451000000000       mov dword ptr [ebp + 0x10], 0
00b0889b eb3e                 jmp 0xb088db
00b0889d d905d8de2201         fld dword ptr [0x122ded8]
00b088a3 d8253cba2901         fsub dword ptr [0x129ba3c]
00b088a9 d94558               fld dword ptr [ebp + 0x58]
00b088ac d8d9                 fcomp st(1)
00b088ae dfe0                 fnstsw ax
00b088b0 f6c441               test ah, 0x41
00b088b3 7524                 jne 0xb088d9
00b088b5 d95d10               fstp dword ptr [ebp + 0x10]
00b088b8 eb21                 jmp 0xb088db
00b088ba d94560               fld dword ptr [ebp + 0x60]
00b088bd d84e44               fmul dword ptr [esi + 0x44]
00b088c0 d83dd8de2201         fdivr dword ptr [0x122ded8]
00b088c6 d84d14               fmul dword ptr [ebp + 0x14]
00b088c9 dd05007f2301         fld qword ptr [0x1237f00]
00b088cf e854620f00           call 0xbfeb28
00b088d4 d95d10               fstp dword ptr [ebp + 0x10]
00b088d7 eb02                 jmp 0xb088db
00b088d9 ddd8                 fstp st(0)
00b088db 8a8e90000000         mov cl, byte ptr [esi + 0x90]
00b088e1 84c9                 test cl, cl
00b088e3 7405                 je 0xb088ea
00b088e5 8d4638               lea eax, [esi + 0x38]
00b088e8 eb31                 jmp 0xb0891b
00b088ea 8a8691000000         mov al, byte ptr [esi + 0x91]
00b088f0 84c0                 test al, al
00b088f2 7405                 je 0xb088f9
00b088f4 8d463c               lea eax, [esi + 0x3c]
00b088f7 eb22                 jmp 0xb0891b
00b088f9 8a8692000000         mov al, byte ptr [esi + 0x92]
00b088ff 84c0                 test al, al
00b08901 7405                 je 0xb08908
00b08903 8d4640               lea eax, [esi + 0x40]
00b08906 eb13                 jmp 0xb0891b
00b08908 c6455eff             mov byte ptr [ebp + 0x5e], 0xff
00b0890c c6455dff             mov byte ptr [ebp + 0x5d], 0xff
00b08910 c6455cff             mov byte ptr [ebp + 0x5c], 0xff
00b08914 c6455fff             mov byte ptr [ebp + 0x5f], 0xff
00b08918 8d455c               lea eax, [ebp + 0x5c]
00b0891b 0fb65002             movzx edx, byte ptr [eax + 2]
00b0891f 895560               mov dword ptr [ebp + 0x60], edx
00b08922 0fb65001             movzx edx, byte ptr [eax + 1]
00b08926 db4560               fild dword ptr [ebp + 0x60]
00b08929 0fb600               movzx eax, byte ptr [eax]
00b0892c d80d24172301         fmul dword ptr [0x1231724]
00b08932 895560               mov dword ptr [ebp + 0x60], edx
00b08935 0fb6563b             movzx edx, byte ptr [esi + 0x3b]
00b08939 d95d38               fstp dword ptr [ebp + 0x38]
00b0893c db4560               fild dword ptr [ebp + 0x60]
00b0893f 894560               mov dword ptr [ebp + 0x60], eax
00b08942 d80d24172301         fmul dword ptr [0x1231724]
00b08948 8a8692000000         mov al, byte ptr [esi + 0x92]
00b0894e 84c0                 test al, al
00b08950 d95d3c               fstp dword ptr [ebp + 0x3c]
00b08953 db4560               fild dword ptr [ebp + 0x60]
00b08956 895560               mov dword ptr [ebp + 0x60], edx
00b08959 d80d24172301         fmul dword ptr [0x1231724]
00b0895f d95d40               fstp dword ptr [ebp + 0x40]
00b08962 db4560               fild dword ptr [ebp + 0x60]
00b08965 d80d24172301         fmul dword ptr [0x1231724]
00b0896b 7405                 je 0xb08972
00b0896d 8d4640               lea eax, [esi + 0x40]
00b08970 eb2b                 jmp 0xb0899d
00b08972 8a8691000000         mov al, byte ptr [esi + 0x91]
00b08978 84c0                 test al, al
00b0897a 7405                 je 0xb08981
00b0897c 8d463c               lea eax, [esi + 0x3c]
00b0897f eb1c                 jmp 0xb0899d
00b08981 84c9                 test cl, cl
00b08983 7405                 je 0xb0898a
00b08985 8d4638               lea eax, [esi + 0x38]
00b08988 eb13                 jmp 0xb0899d
00b0898a c6455eff             mov byte ptr [ebp + 0x5e], 0xff
00b0898e c6455dff             mov byte ptr [ebp + 0x5d], 0xff
00b08992 c6455cff             mov byte ptr [ebp + 0x5c], 0xff
00b08996 c6455fff             mov byte ptr [ebp + 0x5f], 0xff
00b0899a 8d455c               lea eax, [ebp + 0x5c]
00b0899d 0fb64802             movzx ecx, byte ptr [eax + 2]
00b089a1 0fb65001             movzx edx, byte ptr [eax + 1]
00b089a5 0fb600               movzx eax, byte ptr [eax]
00b089a8 894d60               mov dword ptr [ebp + 0x60], ecx
00b089ab 0fb64e43             movzx ecx, byte ptr [esi + 0x43]
00b089af db4560               fild dword ptr [ebp + 0x60]
00b089b2 895560               mov dword ptr [ebp + 0x60], edx
00b089b5 d80d24172301         fmul dword ptr [0x1231724]
00b089bb d95d00               fstp dword ptr [ebp]
00b089be db4560               fild dword ptr [ebp + 0x60]
00b089c1 894560               mov dword ptr [ebp + 0x60], eax
00b089c4 8a8691000000         mov al, byte ptr [esi + 0x91]
00b089ca 84c0                 test al, al
00b089cc d80d24172301         fmul dword ptr [0x1231724]
00b089d2 d95d04               fstp dword ptr [ebp + 4]
00b089d5 db4560               fild dword ptr [ebp + 0x60]
00b089d8 894d60               mov dword ptr [ebp + 0x60], ecx
00b089db d80d24172301         fmul dword ptr [0x1231724]
00b089e1 d95d08               fstp dword ptr [ebp + 8]
00b089e4 db4560               fild dword ptr [ebp + 0x60]
00b089e7 d80d24172301         fmul dword ptr [0x1231724]
00b089ed 7408                 je 0xb089f7
00b089ef 8d463c               lea eax, [esi + 0x3c]
00b089f2 e9b9000000           jmp 0xb08ab0
00b089f7 d94500               fld dword ptr [ebp]
00b089fa d80d9cf52201         fmul dword ptr [0x122f59c]
00b08a00 d94504               fld dword ptr [ebp + 4]
00b08a03 d80d9cf52201         fmul dword ptr [0x122f59c]
00b08a09 d94508               fld dword ptr [ebp + 8]
00b08a0c d80d9cf52201         fmul dword ptr [0x122f59c]
00b08a12 d95dd8               fstp dword ptr [ebp - 0x28]
00b08a15 d9c2                 fld st(2)
00b08a17 d80d9cf52201         fmul dword ptr [0x122f59c]
00b08a1d d95ddc               fstp dword ptr [ebp - 0x24]
00b08a20 d94538               fld dword ptr [ebp + 0x38]
00b08a23 d80d9cf52201         fmul dword ptr [0x122f59c]
00b08a29 d9453c               fld dword ptr [ebp + 0x3c]
00b08a2c d80d9cf52201         fmul dword ptr [0x122f59c]
00b08a32 d95d2c               fstp dword ptr [ebp + 0x2c]
00b08a35 d94540               fld dword ptr [ebp + 0x40]
00b08a38 d80d9cf52201         fmul dword ptr [0x122f59c]
00b08a3e d95d30               fstp dword ptr [ebp + 0x30]
00b08a41 d9c4                 fld st(4)
00b08a43 d80d9cf52201         fmul dword ptr [0x122f59c]
00b08a49 d95d34               fstp dword ptr [ebp + 0x34]
00b08a4c d8c2                 fadd st(2)
00b08a4e d95df0               fstp dword ptr [ebp - 0x10]
00b08a51 d9452c               fld dword ptr [ebp + 0x2c]
00b08a54 d8c1                 fadd st(1)
00b08a56 ddda                 fstp st(2)
00b08a58 ddd8                 fstp st(0)
00b08a5a d94530               fld dword ptr [ebp + 0x30]
00b08a5d d845d8               fadd dword ptr [ebp - 0x28]
00b08a60 d95df8               fstp dword ptr [ebp - 8]
00b08a63 d94534               fld dword ptr [ebp + 0x34]
00b08a66 d845dc               fadd dword ptr [ebp - 0x24]
00b08a69 d95dfc               fstp dword ptr [ebp - 4]
00b08a6c d945f0               fld dword ptr [ebp - 0x10]
00b08a6f d80d14002301         fmul dword ptr [0x1230014]
00b08a75 e8f65f0f00           call 0xbfea70
00b08a7a d80d14002301         fmul dword ptr [0x1230014]
00b08a80 88455e               mov byte ptr [ebp + 0x5e], al
00b08a83 e8e85f0f00           call 0xbfea70
00b08a88 d945f8               fld dword ptr [ebp - 8]
00b08a8b d80d14002301         fmul dword ptr [0x1230014]
00b08a91 88455d               mov byte ptr [ebp + 0x5d], al
00b08a94 e8d75f0f00           call 0xbfea70
00b08a99 d945fc               fld dword ptr [ebp - 4]
00b08a9c d80d14002301         fmul dword ptr [0x1230014]
00b08aa2 88455c               mov byte ptr [ebp + 0x5c], al
00b08aa5 e8c65f0f00           call 0xbfea70
00b08aaa 88455f               mov byte ptr [ebp + 0x5f], al
00b08aad 8d455c               lea eax, [ebp + 0x5c]
00b08ab0 f6c301               test bl, 1
00b08ab3 0fb65002             movzx edx, byte ptr [eax + 2]
00b08ab7 0fb64801             movzx ecx, byte ptr [eax + 1]
00b08abb 895560               mov dword ptr [ebp + 0x60], edx
00b08abe 0fb610               movzx edx, byte ptr [eax]
00b08ac1 db4560               fild dword ptr [ebp + 0x60]
00b08ac4 0fb6463f             movzx eax, byte ptr [esi + 0x3f]
00b08ac8 d80d24172301         fmul dword ptr [0x1231724]
00b08ace 894d60               mov dword ptr [ebp + 0x60], ecx
00b08ad1 d95d28               fstp dword ptr [ebp + 0x28]
00b08ad4 668b4e50             mov cx, word ptr [esi + 0x50]
00b08ad8 db4560               fild dword ptr [ebp + 0x60]
00b08adb 895560               mov dword ptr [ebp + 0x60], edx
00b08ade 66894d5c             mov word ptr [ebp + 0x5c], cx
00b08ae2 d80d24172301         fmul dword ptr [0x1231724]
00b08ae8 d95d2c               fstp dword ptr [ebp + 0x2c]
00b08aeb db4560               fild dword ptr [ebp + 0x60]
00b08aee 894560               mov dword ptr [ebp + 0x60], eax
00b08af1 d80d24172301         fmul dword ptr [0x1231724]
00b08af7 d95d30               fstp dword ptr [ebp + 0x30]
00b08afa d94758               fld dword ptr [edi + 0x58]
00b08afd d9ca                 fxch st(2)
00b08aff d8ca                 fmul st(2)
00b08b01 d95d44               fstp dword ptr [ebp + 0x44]
00b08b04 d8c9                 fmul st(1)
00b08b06 d95d0c               fstp dword ptr [ebp + 0xc]
00b08b09 db4560               fild dword ptr [ebp + 0x60]
00b08b0c d80d24172301         fmul dword ptr [0x1231724]
00b08b12 d8c9                 fmul st(1)
00b08b14 d95d34               fstp dword ptr [ebp + 0x34]
00b08b17 ddd8                 fstp st(0)
00b08b19 7507                 jne 0xb08b22
00b08b1b 814d5c80000000       or dword ptr [ebp + 0x5c], 0x80
00b08b22 f6c320               test bl, 0x20
00b08b25 7408                 je 0xb08b2f
00b08b27 8b455c               mov eax, dword ptr [ebp + 0x5c]
00b08b2a 83c840               or eax, 0x40
00b08b2d eb14                 jmp 0xb08b43
00b08b2f f6c310               test bl, 0x10
00b08b32 7404                 je 0xb08b38
00b08b34 834d5c01             or dword ptr [ebp + 0x5c], 1
00b08b38 f6c304               test bl, 4
00b08b3b 7409                 je 0xb08b46
00b08b3d 8b455c               mov eax, dword ptr [ebp + 0x5c]
00b08b40 83c820               or eax, 0x20
00b08b43 89455c               mov dword ptr [ebp + 0x5c], eax
00b08b46 8a8695000000         mov al, byte ptr [esi + 0x95]
00b08b4c 84c0                 test al, al
00b08b4e 744d                 je 0xb08b9d
00b08b50 8b5e6c               mov ebx, dword ptr [esi + 0x6c]
00b08b53 85db                 test ebx, ebx
00b08b55 8d466c               lea eax, [esi + 0x6c]
00b08b58 7443                 je 0xb08b9d
00b08b5a 8b7d78               mov edi, dword ptr [ebp + 0x78]
00b08b5d 83c714               add edi, 0x14
00b08b60 50                   push eax
00b08b61 8bcf                 mov ecx, edi
00b08b63 e8e8a0feff           call 0xaf2c50
00b08b68 8b7f04               mov edi, dword ptr [edi + 4]
00b08b6b 3bc7                 cmp eax, edi
00b08b6d 7404                 je 0xb08b73
00b08b6f 3b18                 cmp ebx, dword ptr [eax]
00b08b71 7302                 jae 0xb08b75
00b08b73 8bc7                 mov eax, edi
00b08b75 8b5578               mov edx, dword ptr [ebp + 0x78]
00b08b78 3b4218               cmp eax, dword ptr [edx + 0x18]
00b08b7b 7420                 je 0xb08b9d
00b08b7d 8b7d78               mov edi, dword ptr [ebp + 0x78]
00b08b80 8b4808               mov ecx, dword ptr [eax + 8]
00b08b83 8b400c               mov eax, dword ptr [eax + 0xc]
00b08b86 8b11                 mov edx, dword ptr [ecx]
00b08b88 6a01                 push 1
00b08b8a 57                   push edi
00b08b8b 8b7d6c               mov edi, dword ptr [ebp + 0x6c]
00b08b8e 6a00                 push 0
00b08b90 57                   push edi
00b08b91 50                   push eax
00b08b92 8d451c               lea eax, [ebp + 0x1c]
00b08b95 50                   push eax
00b08b96 ff5204               call dword ptr [edx + 4]
00b08b99 84c0                 test al, al
00b08b9b 7513                 jne 0xb08bb0
00b08b9d 8b5578               mov edx, dword ptr [ebp + 0x78]
00b08ba0 6a00                 push 0
00b08ba2 6a00                 push 0
00b08ba4 8d4d1c               lea ecx, [ebp + 0x1c]
00b08ba7 51                   push ecx
00b08ba8 52                   push edx
00b08ba9 8bce                 mov ecx, esi
00b08bab e8f08d0100           call 0xb219a0
00b08bb0 8b7d74               mov edi, dword ptr [ebp + 0x74]
00b08bb3 8d45a0               lea eax, [ebp - 0x60]
00b08bb6 50                   push eax
00b08bb7 8bcf                 mov ecx, edi
00b08bb9 e89296feff           call 0xaf2250
00b08bbe d9451c               fld dword ptr [ebp + 0x1c]
00b08bc1 d94520               fld dword ptr [ebp + 0x20]
00b08bc4 d945b8               fld dword ptr [ebp - 0x48]
00b08bc7 d84d24               fmul dword ptr [ebp + 0x24]
00b08bca d945ac               fld dword ptr [ebp - 0x54]
00b08bcd d84d20               fmul dword ptr [ebp + 0x20]
00b08bd0 dec1                 faddp st(1)
00b08bd2 d9451c               fld dword ptr [ebp + 0x1c]
00b08bd5 d84da0               fmul dword ptr [ebp - 0x60]
00b08bd8 dec1                 faddp st(1)
00b08bda d845c4               fadd dword ptr [ebp - 0x3c]
00b08bdd d95d1c               fstp dword ptr [ebp + 0x1c]
00b08be0 d945bc               fld dword ptr [ebp - 0x44]
00b08be3 d84d24               fmul dword ptr [ebp + 0x24]
00b08be6 d945b0               fld dword ptr [ebp - 0x50]
00b08be9 d84d20               fmul dword ptr [ebp + 0x20]
00b08bec dec1                 faddp st(1)
00b08bee d945a4               fld dword ptr [ebp - 0x5c]
00b08bf1 d8cb                 fmul st(3)
00b08bf3 dec1                 faddp st(1)
00b08bf5 d845c8               fadd dword ptr [ebp - 0x38]
00b08bf8 d95d20               fstp dword ptr [ebp + 0x20]
00b08bfb d945c0               fld dword ptr [ebp - 0x40]
00b08bfe d84d24               fmul dword ptr [ebp + 0x24]
00b08c01 d945a8               fld dword ptr [ebp - 0x58]
00b08c04 d8cb                 fmul st(3)
00b08c06 dec1                 faddp st(1)
00b08c08 d945b4               fld dword ptr [ebp - 0x4c]
00b08c0b d8ca                 fmul st(2)
00b08c0d dec1                 faddp st(1)
00b08c0f d845cc               fadd dword ptr [ebp - 0x34]
00b08c12 d95d24               fstp dword ptr [ebp + 0x24]
00b08c15 ddd8                 fstp st(0)
00b08c17 ddd8                 fstp st(0)
00b08c19 e8821efeff           call 0xaeaaa0
00b08c1e d94558               fld dword ptr [ebp + 0x58]
00b08c21 8b4e58               mov ecx, dword ptr [esi + 0x58]
00b08c24 d81d9cf52201         fcomp dword ptr [0x122f59c]
00b08c2a 894560               mov dword ptr [ebp + 0x60], eax
00b08c2d d94558               fld dword ptr [ebp + 0x58]
00b08c30 894d14               mov dword ptr [ebp + 0x14], ecx
00b08c33 dfe0                 fnstsw ax
00b08c35 f6c405               test ah, 5
00b08c38 0f8a89000000         jp 0xb08cc7
00b08c3e dcc0                 fadd st(0), st(0)
00b08c40 d94528               fld dword ptr [ebp + 0x28]
00b08c43 d8c9                 fmul st(1)
00b08c45 d9452c               fld dword ptr [ebp + 0x2c]
00b08c48 d8ca                 fmul st(2)
00b08c4a d95df4               fstp dword ptr [ebp - 0xc]
00b08c4d d94530               fld dword ptr [ebp + 0x30]
00b08c50 d8ca                 fmul st(2)
00b08c52 d95df8               fstp dword ptr [ebp - 8]
00b08c55 d9c1                 fld st(1)
00b08c57 d84d34               fmul dword ptr [ebp + 0x34]
00b08c5a d95dfc               fstp dword ptr [ebp - 4]
00b08c5d d905d8de2201         fld dword ptr [0x122ded8]
00b08c63 d8e2                 fsub st(2)
00b08c65 d94538               fld dword ptr [ebp + 0x38]
00b08c68 d8c9                 fmul st(1)
00b08c6a d95d00               fstp dword ptr [ebp]
00b08c6d d9453c               fld dword ptr [ebp + 0x3c]
00b08c70 d8c9                 fmul st(1)
00b08c72 d95d04               fstp dword ptr [ebp + 4]
00b08c75 d94540               fld dword ptr [ebp + 0x40]
00b08c78 d8c9                 fmul st(1)
00b08c7a d95d08               fstp dword ptr [ebp + 8]
00b08c7d d84d44               fmul dword ptr [ebp + 0x44]
00b08c80 d95d0c               fstp dword ptr [ebp + 0xc]
00b08c83 d94500               fld dword ptr [ebp]
00b08c86 d8c1                 fadd st(1)
00b08c88 d95d38               fstp dword ptr [ebp + 0x38]
00b08c8b 8b5538               mov edx, dword ptr [ebp + 0x38]
00b08c8e 895528               mov dword ptr [ebp + 0x28], edx
00b08c91 ddd8                 fstp st(0)
00b08c93 ddd8                 fstp st(0)
00b08c95 d94504               fld dword ptr [ebp + 4]
00b08c98 d845f4               fadd dword ptr [ebp - 0xc]
00b08c9b d95d3c               fstp dword ptr [ebp + 0x3c]
00b08c9e 8b453c               mov eax, dword ptr [ebp + 0x3c]
00b08ca1 d94508               fld dword ptr [ebp + 8]
00b08ca4 89452c               mov dword ptr [ebp + 0x2c], eax
00b08ca7 d845f8               fadd dword ptr [ebp - 8]
00b08caa d95d40               fstp dword ptr [ebp + 0x40]
00b08cad 8b4d40               mov ecx, dword ptr [ebp + 0x40]
00b08cb0 d9450c               fld dword ptr [ebp + 0xc]
00b08cb3 894d30               mov dword ptr [ebp + 0x30], ecx
00b08cb6 d845fc               fadd dword ptr [ebp - 4]
00b08cb9 d95d44               fstp dword ptr [ebp + 0x44]
00b08cbc 8b5544               mov edx, dword ptr [ebp + 0x44]
00b08cbf 895534               mov dword ptr [ebp + 0x34], edx
00b08cc2 e98a000000           jmp 0xb08d51
00b08cc7 d8259cf52201         fsub dword ptr [0x122f59c]
00b08ccd dcc0                 fadd st(0), st(0)
00b08ccf d94500               fld dword ptr [ebp]
00b08cd2 d8c9                 fmul st(1)
00b08cd4 d94504               fld dword ptr [ebp + 4]
00b08cd7 d8ca                 fmul st(2)
00b08cd9 d95df4               fstp dword ptr [ebp - 0xc]
00b08cdc d94508               fld dword ptr [ebp + 8]
00b08cdf d8ca                 fmul st(2)
00b08ce1 d95df8               fstp dword ptr [ebp - 8]
00b08ce4 d9c1                 fld st(1)
00b08ce6 d84d0c               fmul dword ptr [ebp + 0xc]
00b08ce9 d95dfc               fstp dword ptr [ebp - 4]
00b08cec d905d8de2201         fld dword ptr [0x122ded8]
00b08cf2 d8e2                 fsub st(2)
00b08cf4 d94528               fld dword ptr [ebp + 0x28]
00b08cf7 d8c9                 fmul st(1)
00b08cf9 d95d00               fstp dword ptr [ebp]
00b08cfc d9452c               fld dword ptr [ebp + 0x2c]
00b08cff d8c9                 fmul st(1)
00b08d01 d95d04               fstp dword ptr [ebp + 4]
00b08d04 d94530               fld dword ptr [ebp + 0x30]
00b08d07 d8c9                 fmul st(1)
00b08d09 d95d08               fstp dword ptr [ebp + 8]
00b08d0c d84d34               fmul dword ptr [ebp + 0x34]
00b08d0f d95d0c               fstp dword ptr [ebp + 0xc]
00b08d12 d94500               fld dword ptr [ebp]
00b08d15 d8c1                 fadd st(1)
00b08d17 d95d38               fstp dword ptr [ebp + 0x38]
00b08d1a 8b4538               mov eax, dword ptr [ebp + 0x38]
00b08d1d 894528               mov dword ptr [ebp + 0x28], eax
00b08d20 ddd8                 fstp st(0)
00b08d22 ddd8                 fstp st(0)
00b08d24 d94504               fld dword ptr [ebp + 4]
00b08d27 d845f4               fadd dword ptr [ebp - 0xc]
00b08d2a d95d3c               fstp dword ptr [ebp + 0x3c]
00b08d2d 8b4d3c               mov ecx, dword ptr [ebp + 0x3c]
00b08d30 d94508               fld dword ptr [ebp + 8]
00b08d33 894d2c               mov dword ptr [ebp + 0x2c], ecx
00b08d36 d845f8               fadd dword ptr [ebp - 8]
00b08d39 d95d40               fstp dword ptr [ebp + 0x40]
00b08d3c 8b5540               mov edx, dword ptr [ebp + 0x40]
00b08d3f d9450c               fld dword ptr [ebp + 0xc]
00b08d42 895530               mov dword ptr [ebp + 0x30], edx
00b08d45 d845fc               fadd dword ptr [ebp - 4]
00b08d48 d95d44               fstp dword ptr [ebp + 0x44]
00b08d4b 8b4544               mov eax, dword ptr [ebp + 0x44]
00b08d4e 894534               mov dword ptr [ebp + 0x34], eax
00b08d51 d94558               fld dword ptr [ebp + 0x58]
00b08d54 8b4f54               mov ecx, dword ptr [edi + 0x54]
00b08d57 d85d54               fcomp dword ptr [ebp + 0x54]
00b08d5a 894d78               mov dword ptr [ebp + 0x78], ecx
00b08d5d c745ec0000803f       mov dword ptr [ebp - 0x14], 0x3f800000
00b08d64 dfe0                 fnstsw ax
00b08d66 f6c405               test ah, 5
00b08d69 7a17                 jp 0xb08d82
00b08d6b d94558               fld dword ptr [ebp + 0x58]
00b08d6e 51                   push ecx
00b08d6f d87554               fdiv dword ptr [ebp + 0x54]
00b08d72 8bce                 mov ecx, esi
00b08d74 d91c24               fstp dword ptr [esp]
00b08d77 e894d0ffff           call 0xb05e10
00b08d7c d84d78               fmul dword ptr [ebp + 0x78]
00b08d7f d95d78               fstp dword ptr [ebp + 0x78]
00b08d82 d94558               fld dword ptr [ebp + 0x58]
00b08d85 d85d18               fcomp dword ptr [ebp + 0x18]
00b08d88 dfe0                 fnstsw ax
00b08d8a f6c441               test ah, 0x41
00b08d8d 7525                 jne 0xb08db4
00b08d8f d905d8de2201         fld dword ptr [0x122ded8]
00b08d95 51                   push ecx
00b08d96 d86558               fsub dword ptr [ebp + 0x58]
00b08d99 8bce                 mov ecx, esi
00b08d9b d905d8de2201         fld dword ptr [0x122ded8]
00b08da1 d86518               fsub dword ptr [ebp + 0x18]
00b08da4 def9                 fdivp st(1)
00b08da6 d91c24               fstp dword ptr [esp]
00b08da9 e862d0ffff           call 0xb05e10
00b08dae d84d78               fmul dword ptr [ebp + 0x78]
00b08db1 d95d78               fstp dword ptr [ebp + 0x78]
00b08db4 d94578               fld dword ptr [ebp + 0x78]
00b08db7 d81dd8de2201         fcomp dword ptr [0x122ded8]
00b08dbd dfe0                 fnstsw ax
00b08dbf f6c405               test ah, 5
00b08dc2 7a35                 jp 0xb08df9
00b08dc4 8a8693000000         mov al, byte ptr [esi + 0x93]
00b08dca a802                 test al, 2
00b08dcc 7415                 je 0xb08de3
00b08dce d905d8de2201         fld dword ptr [0x122ded8]
00b08dd4 d86664               fsub dword ptr [esi + 0x64]
00b08dd7 d84d78               fmul dword ptr [ebp + 0x78]
00b08dda d84664               fadd dword ptr [esi + 0x64]
00b08ddd d84d34               fmul dword ptr [ebp + 0x34]
00b08de0 d95d34               fstp dword ptr [ebp + 0x34]
00b08de3 a840                 test al, 0x40
00b08de5 7412                 je 0xb08df9
00b08de7 d905d8de2201         fld dword ptr [0x122ded8]
00b08ded d86668               fsub dword ptr [esi + 0x68]
00b08df0 d84d78               fmul dword ptr [ebp + 0x78]
00b08df3 d84668               fadd dword ptr [esi + 0x68]
00b08df6 d95dec               fstp dword ptr [ebp - 0x14]
00b08df9 d94528               fld dword ptr [ebp + 0x28]
00b08dfc d80d14002301         fmul dword ptr [0x1230014]
00b08e02 d95d78               fstp dword ptr [ebp + 0x78]
00b08e05 d94578               fld dword ptr [ebp + 0x78]
00b08e08 db5d50               fistp dword ptr [ebp + 0x50]
00b08e0b d9452c               fld dword ptr [ebp + 0x2c]
00b08e0e 8b4550               mov eax, dword ptr [ebp + 0x50]
00b08e11 d80d14002301         fmul dword ptr [0x1230014]
00b08e17 894518               mov dword ptr [ebp + 0x18], eax
00b08e1a d95d78               fstp dword ptr [ebp + 0x78]
00b08e1d d94578               fld dword ptr [ebp + 0x78]
00b08e20 db5d50               fistp dword ptr [ebp + 0x50]
00b08e23 d94530               fld dword ptr [ebp + 0x30]
00b08e26 8b4d50               mov ecx, dword ptr [ebp + 0x50]
00b08e29 d80d14002301         fmul dword ptr [0x1230014]
00b08e2f 894de8               mov dword ptr [ebp - 0x18], ecx
00b08e32 d95d78               fstp dword ptr [ebp + 0x78]
00b08e35 d94578               fld dword ptr [ebp + 0x78]
00b08e38 db5d50               fistp dword ptr [ebp + 0x50]
00b08e3b d94534               fld dword ptr [ebp + 0x34]
00b08e3e 8b5550               mov edx, dword ptr [ebp + 0x50]
00b08e41 d80d14002301         fmul dword ptr [0x1230014]
00b08e47 895558               mov dword ptr [ebp + 0x58], edx
00b08e4a d95d78               fstp dword ptr [ebp + 0x78]
00b08e4d d94578               fld dword ptr [ebp + 0x78]
00b08e50 db5d50               fistp dword ptr [ebp + 0x50]
00b08e53 85c0                 test eax, eax
00b08e55 8b7d50               mov edi, dword ptr [ebp + 0x50]
00b08e58 897d78               mov dword ptr [ebp + 0x78], edi
00b08e5b bbff000000           mov ebx, 0xff
00b08e60 7d09                 jge 0xb08e6b
00b08e62 c7451800000000       mov dword ptr [ebp + 0x18], 0
00b08e69 eb07                 jmp 0xb08e72
00b08e6b 3bc3                 cmp eax, ebx
00b08e6d 7e03                 jle 0xb08e72
00b08e6f 895d18               mov dword ptr [ebp + 0x18], ebx
00b08e72 33c0                 xor eax, eax
00b08e74 3bc8                 cmp ecx, eax
00b08e76 7d05                 jge 0xb08e7d
00b08e78 8945e8               mov dword ptr [ebp - 0x18], eax
00b08e7b eb07                 jmp 0xb08e84
00b08e7d 3bcb                 cmp ecx, ebx
00b08e7f 7e03                 jle 0xb08e84
00b08e81 895de8               mov dword ptr [ebp - 0x18], ebx
00b08e84 3bd0                 cmp edx, eax
00b08e86 7d05                 jge 0xb08e8d
00b08e88 894558               mov dword ptr [ebp + 0x58], eax
00b08e8b eb07                 jmp 0xb08e94
00b08e8d 3bd3                 cmp edx, ebx
00b08e8f 7e03                 jle 0xb08e94
00b08e91 895d58               mov dword ptr [ebp + 0x58], ebx
00b08e94 3bf8                 cmp edi, eax
00b08e96 7d05                 jge 0xb08e9d
00b08e98 894578               mov dword ptr [ebp + 0x78], eax
00b08e9b eb07                 jmp 0xb08ea4
00b08e9d 3bfb                 cmp edi, ebx
00b08e9f 7e03                 jle 0xb08ea4
00b08ea1 895d78               mov dword ptr [ebp + 0x78], ebx
00b08ea4 894554               mov dword ptr [ebp + 0x54], eax
00b08ea7 8b4634               mov eax, dword ptr [esi + 0x34]
00b08eaa 85c0                 test eax, eax
00b08eac 0f848b020000         je 0xb0913d
00b08eb2 8b5670               mov edx, dword ptr [esi + 0x70]
00b08eb5 8b4e74               mov ecx, dword ptr [esi + 0x74]
00b08eb8 8b7d60               mov edi, dword ptr [ebp + 0x60]
00b08ebb 895550               mov dword ptr [ebp + 0x50], edx
00b08ebe 8b17                 mov edx, dword ptr [edi]
00b08ec0 894de0               mov dword ptr [ebp - 0x20], ecx
00b08ec3 50                   push eax
00b08ec4 8bcf                 mov ecx, edi
00b08ec6 ff5250               call dword ptr [edx + 0x50]
00b08ec9 8bc8                 mov ecx, eax
00b08ecb 894d4c               mov dword ptr [ebp + 0x4c], ecx
00b08ece db454c               fild dword ptr [ebp + 0x4c]
00b08ed1 c745e40000003f       mov dword ptr [ebp - 0x1c], 0x3f000000
00b08ed8 c7454c0000803f       mov dword ptr [ebp + 0x4c], 0x3f800000
00b08edf d84d10               fmul dword ptr [ebp + 0x10]
00b08ee2 d95d44               fstp dword ptr [ebp + 0x44]
00b08ee5 d94544               fld dword ptr [ebp + 0x44]
00b08ee8 d865e4               fsub dword ptr [ebp - 0x1c]
00b08eeb db5d54               fistp dword ptr [ebp + 0x54]
00b08eee db4554               fild dword ptr [ebp + 0x54]
00b08ef1 d8454c               fadd dword ptr [ebp + 0x4c]
00b08ef4 d95d48               fstp dword ptr [ebp + 0x48]
00b08ef7 8b4544               mov eax, dword ptr [ebp + 0x44]
00b08efa 3b4548               cmp eax, dword ptr [ebp + 0x48]
00b08efd 7503                 jne 0xb08f02
00b08eff ff4554               inc dword ptr [ebp + 0x54]
00b08f02 8b5d54               mov ebx, dword ptr [ebp + 0x54]
00b08f05 85db                 test ebx, ebx
00b08f07 895d54               mov dword ptr [ebp + 0x54], ebx
00b08f0a 7d04                 jge 0xb08f10
00b08f0c 33db                 xor ebx, ebx
00b08f0e eb09                 jmp 0xb08f19
00b08f10 8d41ff               lea eax, [ecx - 1]
00b08f13 3bd8                 cmp ebx, eax
00b08f15 7e05                 jle 0xb08f1c
00b08f17 8bd8                 mov ebx, eax
00b08f19 895d54               mov dword ptr [ebp + 0x54], ebx
00b08f1c 8b87e0010000         mov eax, dword ptr [edi + 0x1e0]
00b08f22 8b97e4010000         mov edx, dword ptr [edi + 0x1e4]
00b08f28 8b4e34               mov ecx, dword ptr [esi + 0x34]
00b08f2b 2bd0                 sub edx, eax
00b08f2d b8e9a28b2e           mov eax, 0x2e8ba2e9
00b08f32 f7ea                 imul edx
00b08f34 c1fa03               sar edx, 3
00b08f37 8bc2                 mov eax, edx
00b08f39 c1e81f               shr eax, 0x1f
00b08f3c 03c2                 add eax, edx
00b08f3e 3bc8                 cmp ecx, eax
00b08f40 730d                 jae 0xb08f4f
00b08f42 8b87e0010000         mov eax, dword ptr [edi + 0x1e0]
00b08f48 6bc92c               imul ecx, ecx, 0x2c
00b08f4b 03c8                 add ecx, eax
00b08f4d eb02                 jmp 0xb08f51
00b08f4f 33c9                 xor ecx, ecx
00b08f51 d94648               fld dword ptr [esi + 0x48]
00b08f54 8b5128               mov edx, dword ptr [ecx + 0x28]
00b08f57 d9464c               fld dword ptr [esi + 0x4c]
00b08f5a 8bc3                 mov eax, ebx
00b08f5c d8e1                 fsub st(1)
00b08f5e c1e004               shl eax, 4
00b08f61 03c2                 add eax, edx
00b08f63 8b400c               mov eax, dword ptr [eax + 0xc]
00b08f66 0fb74808             movzx ecx, word ptr [eax + 8]
00b08f6a 0fb75006             movzx edx, word ptr [eax + 6]
00b08f6e 894d48               mov dword ptr [ebp + 0x48], ecx
00b08f71 d84d10               fmul dword ptr [ebp + 0x10]
00b08f74 8b4d74               mov ecx, dword ptr [ebp + 0x74]
00b08f77 8d4540               lea eax, [ebp + 0x40]
00b08f7a 50                   push eax
00b08f7b d8c1                 fadd st(1)
00b08f7d d84dec               fmul dword ptr [ebp - 0x14]
00b08f80 d95d40               fstp dword ptr [ebp + 0x40]
00b08f83 ddd8                 fstp st(0)
00b08f85 db4548               fild dword ptr [ebp + 0x48]
00b08f88 895548               mov dword ptr [ebp + 0x48], edx
00b08f8b da7548               fidiv dword ptr [ebp + 0x48]
00b08f8e d84d40               fmul dword ptr [ebp + 0x40]
00b08f91 d95d44               fstp dword ptr [ebp + 0x44]
00b08f94 e867fffeff           call 0xaf8f00
00b08f99 f6869300000020       test byte ptr [esi + 0x93], 0x20
00b08fa0 0f84ee000000         je 0xb09094
00b08fa6 8b4650               mov eax, dword ptr [esi + 0x50]
00b08fa9 83f802               cmp eax, 2
00b08fac c745ec0000003f       mov dword ptr [ebp - 0x14], 0x3f000000
00b08fb3 7405                 je 0xb08fba
00b08fb5 83f804               cmp eax, 4
00b08fb8 7507                 jne 0xb08fc1
00b08fba c745ec0000803f       mov dword ptr [ebp - 0x14], 0x3f800000
00b08fc1 8b4654               mov eax, dword ptr [esi + 0x54]
00b08fc4 33ff                 xor edi, edi
00b08fc6 85c0                 test eax, eax
00b08fc8 897d4c               mov dword ptr [ebp + 0x4c], edi
00b08fcb 0f8eb8000000         jle 0xb09089
00b08fd1 eb0d                 jmp 0xb08fe0
00b08fd3 8b5d54               mov ebx, dword ptr [ebp + 0x54]
00b08fd6 eb08                 jmp 0xb08fe0
00b08fd8 8da42400000000       lea esp, [esp]
00b08fdf 90                   nop 
00b08fe0 db454c               fild dword ptr [ebp + 0x4c]
00b08fe3 8b8e30010000         mov ecx, dword ptr [esi + 0x130]
00b08fe9 8b862c010000         mov eax, dword ptr [esi + 0x12c]
00b08fef 2bc8                 sub ecx, eax
00b08ff1 d8059cf52201         fadd dword ptr [0x122f59c]
00b08ff7 c1f902               sar ecx, 2
00b08ffa 3bcf                 cmp ecx, edi
00b08ffc d84dec               fmul dword ptr [ebp - 0x14]
00b08fff da7654               fidiv dword ptr [esi + 0x54]
00b09002 d84514               fadd dword ptr [ebp + 0x14]
00b09005 d95d48               fstp dword ptr [ebp + 0x48]
00b09008 760a                 jbe 0xb09014
00b0900a 8bd0                 mov edx, eax
00b0900c 8b04ba               mov eax, dword ptr [edx + edi*4]
00b0900f 89454c               mov dword ptr [ebp + 0x4c], eax
00b09012 eb07                 jmp 0xb0901b
00b09014 c7454cffffffff       mov dword ptr [ebp + 0x4c], 0xffffffff
00b0901b 8b4d70               mov ecx, dword ptr [ebp + 0x70]
00b0901e 8b5134               mov edx, dword ptr [ecx + 0x34]
00b09021 8b02                 mov eax, dword ptr [edx]
00b09023 8b4de0               mov ecx, dword ptr [ebp - 0x20]
00b09026 8b5558               mov edx, dword ptr [ebp + 0x58]
00b09029 6a00                 push 0
00b0902b 51                   push ecx
00b0902c 8b4de8               mov ecx, dword ptr [ebp - 0x18]
00b0902f 6a00                 push 0
00b09031 6a00                 push 0
00b09033 52                   push edx
00b09034 8b5518               mov edx, dword ptr [ebp + 0x18]
00b09037 51                   push ecx
00b09038 8b4d78               mov ecx, dword ptr [ebp + 0x78]
00b0903b 52                   push edx
00b0903c 8b5550               mov edx, dword ptr [ebp + 0x50]
00b0903f 51                   push ecx
00b09040 8b4d44               mov ecx, dword ptr [ebp + 0x44]
00b09043 52                   push edx
00b09044 8b5540               mov edx, dword ptr [ebp + 0x40]
00b09047 51                   push ecx
00b09048 8b4d48               mov ecx, dword ptr [ebp + 0x48]
00b0904b 52                   push edx
00b0904c 8b555c               mov edx, dword ptr [ebp + 0x5c]
00b0904f 51                   push ecx
00b09050 8b4e34               mov ecx, dword ptr [esi + 0x34]
00b09053 52                   push edx
00b09054 53                   push ebx
00b09055 51                   push ecx
00b09056 8d551c               lea edx, [ebp + 0x1c]
00b09059 52                   push edx
00b0905a 8d4d88               lea ecx, [ebp - 0x78]
00b0905d 894548               mov dword ptr [ebp + 0x48], eax
00b09060 e869c8b4ff           call 0x6558ce
00b09065 8b4d7c               mov ecx, dword ptr [ebp + 0x7c]
00b09068 8b5570               mov edx, dword ptr [ebp + 0x70]
00b0906b 50                   push eax
00b0906c 8b454c               mov eax, dword ptr [ebp + 0x4c]
00b0906f 50                   push eax
00b09070 8b4548               mov eax, dword ptr [ebp + 0x48]
00b09073 51                   push ecx
00b09074 8b4a34               mov ecx, dword ptr [edx + 0x34]
00b09077 ff5070               call dword ptr [eax + 0x70]
00b0907a 8b4654               mov eax, dword ptr [esi + 0x54]
00b0907d 47                   inc edi
00b0907e 3bf8                 cmp edi, eax
00b09080 897d4c               mov dword ptr [ebp + 0x4c], edi
00b09083 0f8c4affffff         jl 0xb08fd3
00b09089 8b4654               mov eax, dword ptr [esi + 0x54]
00b0908c 894554               mov dword ptr [ebp + 0x54], eax
00b0908f e9a9000000           jmp 0xb0913d
00b09094 8b8e30010000         mov ecx, dword ptr [esi + 0x130]
00b0909a 8bbe2c010000         mov edi, dword ptr [esi + 0x12c]
00b090a0 2bcf                 sub ecx, edi
00b090a2 c1f902               sar ecx, 2
00b090a5 85c9                 test ecx, ecx
00b090a7 7606                 jbe 0xb090af
00b090a9 8bd7                 mov edx, edi
00b090ab 8b12                 mov edx, dword ptr [edx]
00b090ad eb03                 jmp 0xb090b2
00b090af 83caff               or edx, 0xffffffff
00b090b2 d94540               fld dword ptr [ebp + 0x40]
00b090b5 89554c               mov dword ptr [ebp + 0x4c], edx
00b090b8 d81ddcde2201         fcomp dword ptr [0x122dedc]
00b090be dfe0                 fnstsw ax
00b090c0 f6c441               test ah, 0x41
00b090c3 7561                 jne 0xb09126
00b090c5 8b4578               mov eax, dword ptr [ebp + 0x78]
00b090c8 84c0                 test al, al
00b090ca 745a                 je 0xb09126
00b090cc 8b55e0               mov edx, dword ptr [ebp - 0x20]
00b090cf 6a00                 push 0
00b090d1 52                   push edx
00b090d2 8b5558               mov edx, dword ptr [ebp + 0x58]
00b090d5 6a00                 push 0
00b090d7 8b4d70               mov ecx, dword ptr [ebp + 0x70]
00b090da 8b7934               mov edi, dword ptr [ecx + 0x34]
00b090dd 6a00                 push 0
00b090df 52                   push edx
00b090e0 8b55e8               mov edx, dword ptr [ebp - 0x18]
00b090e3 8b0f                 mov ecx, dword ptr [edi]
00b090e5 52                   push edx
00b090e6 8b5518               mov edx, dword ptr [ebp + 0x18]
00b090e9 52                   push edx
00b090ea 8b5544               mov edx, dword ptr [ebp + 0x44]
00b090ed 50                   push eax
00b090ee 8b4550               mov eax, dword ptr [ebp + 0x50]
00b090f1 50                   push eax
00b090f2 8b4540               mov eax, dword ptr [ebp + 0x40]
00b090f5 52                   push edx
00b090f6 8b5514               mov edx, dword ptr [ebp + 0x14]
00b090f9 50                   push eax
00b090fa 8b455c               mov eax, dword ptr [ebp + 0x5c]
00b090fd 52                   push edx
00b090fe 50                   push eax
00b090ff 8b4634               mov eax, dword ptr [esi + 0x34]
00b09102 53                   push ebx
00b09103 50                   push eax
00b09104 8d551c               lea edx, [ebp + 0x1c]
00b09107 894d78               mov dword ptr [ebp + 0x78], ecx
00b0910a 52                   push edx
00b0910b 8d4d88               lea ecx, [ebp - 0x78]
00b0910e e8bbc7b4ff           call 0x6558ce
00b09113 8b4d7c               mov ecx, dword ptr [ebp + 0x7c]
00b09116 50                   push eax
00b09117 8b454c               mov eax, dword ptr [ebp + 0x4c]
00b0911a 50                   push eax
00b0911b 8b4578               mov eax, dword ptr [ebp + 0x78]
00b0911e 51                   push ecx
00b0911f 8bcf                 mov ecx, edi
00b09121 ff5070               call dword ptr [eax + 0x70]
00b09124 eb10                 jmp 0xb09136
00b09126 8b4570               mov eax, dword ptr [ebp + 0x70]
00b09129 8b4834               mov ecx, dword ptr [eax + 0x34]
00b0912c 8b01                 mov eax, dword ptr [ecx]
00b0912e 52                   push edx
00b0912f 8b557c               mov edx, dword ptr [ebp + 0x7c]
00b09132 52                   push edx
00b09133 ff5078               call dword ptr [eax + 0x78]
00b09136 c7455401000000       mov dword ptr [ebp + 0x54], 1
00b0913d 8b8680000000         mov eax, dword ptr [esi + 0x80]
00b09143 85c0                 test eax, eax
00b09145 0f8400020000         je 0xb0934b
00b0914b 8b4e78               mov ecx, dword ptr [esi + 0x78]
00b0914e 8b567c               mov edx, dword ptr [esi + 0x7c]
00b09151 894d14               mov dword ptr [ebp + 0x14], ecx
00b09154 8b4d60               mov ecx, dword ptr [ebp + 0x60]
00b09157 895550               mov dword ptr [ebp + 0x50], edx
00b0915a 8b11                 mov edx, dword ptr [ecx]
00b0915c 50                   push eax
00b0915d ff5250               call dword ptr [edx + 0x50]
00b09160 8bc8                 mov ecx, eax
00b09162 894d78               mov dword ptr [ebp + 0x78], ecx
00b09165 db4578               fild dword ptr [ebp + 0x78]
00b09168 c745440000003f       mov dword ptr [ebp + 0x44], 0x3f000000
00b0916f c745480000803f       mov dword ptr [ebp + 0x48], 0x3f800000
00b09176 d84d10               fmul dword ptr [ebp + 0x10]
00b09179 d95de4               fstp dword ptr [ebp - 0x1c]
00b0917c d945e4               fld dword ptr [ebp - 0x1c]
00b0917f d86544               fsub dword ptr [ebp + 0x44]
00b09182 db5d78               fistp dword ptr [ebp + 0x78]
00b09185 db4578               fild dword ptr [ebp + 0x78]
00b09188 d84548               fadd dword ptr [ebp + 0x48]
00b0918b d95d60               fstp dword ptr [ebp + 0x60]
00b0918e 8b45e4               mov eax, dword ptr [ebp - 0x1c]
00b09191 3b4560               cmp eax, dword ptr [ebp + 0x60]
00b09194 7503                 jne 0xb09199
00b09196 ff4578               inc dword ptr [ebp + 0x78]
00b09199 8b4578               mov eax, dword ptr [ebp + 0x78]
00b0919c 85c0                 test eax, eax
00b0919e 7d04                 jge 0xb091a4
00b091a0 33c0                 xor eax, eax
00b091a2 eb07                 jmp 0xb091ab
00b091a4 49                   dec ecx
00b091a5 3bc1                 cmp eax, ecx
00b091a7 7e02                 jle 0xb091ab
00b091a9 8bc1                 mov eax, ecx
00b091ab 8b8e80000000         mov ecx, dword ptr [esi + 0x80]
00b091b1 898ec4000000         mov dword ptr [esi + 0xc4], ecx
00b091b7 8a8e95000000         mov cl, byte ptr [esi + 0x95]
00b091bd 84c9                 test cl, cl
00b091bf 8986c8000000         mov dword ptr [esi + 0xc8], eax
00b091c5 7429                 je 0xb091f0
00b091c7 d905d8de2201         fld dword ptr [0x122ded8]
00b091cd d8656c               fsub dword ptr [ebp + 0x6c]
00b091d0 d905d8de2201         fld dword ptr [0x122ded8]
00b091d6 d8253cba2901         fsub dword ptr [0x129ba3c]
00b091dc d95d78               fstp dword ptr [ebp + 0x78]
00b091df d85578               fcom dword ptr [ebp + 0x78]
00b091e2 dfe0                 fnstsw ax
00b091e4 f6c405               test ah, 5
00b091e7 7b0d                 jnp 0xb091f6
00b091e9 ddd8                 fstp st(0)
00b091eb d94578               fld dword ptr [ebp + 0x78]
00b091ee eb06                 jmp 0xb091f6
00b091f0 d905dcde2201         fld dword ptr [0x122dedc]
00b091f6 84c9                 test cl, cl
00b091f8 d99ecc000000         fstp dword ptr [esi + 0xcc]
00b091fe 752d                 jne 0xb0922d
00b09200 8a8696000000         mov al, byte ptr [esi + 0x96]
00b09206 84c0                 test al, al
00b09208 7423                 je 0xb0922d
00b0920a 8b4610               mov eax, dword ptr [esi + 0x10]
00b0920d 8b4e14               mov ecx, dword ptr [esi + 0x14]
00b09210 8b7d74               mov edi, dword ptr [ebp + 0x74]
00b09213 2bc8                 sub ecx, eax
00b09215 b8abaaaa2a           mov eax, 0x2aaaaaab
00b0921a f7e9                 imul ecx
00b0921c d1fa                 sar edx, 1
00b0921e 8bc2                 mov eax, edx
00b09220 c1e81f               shr eax, 0x1f
00b09223 03c2                 add eax, edx
00b09225 8986c0000000         mov dword ptr [esi + 0xc0], eax
00b0922b eb19                 jmp 0xb09246
00b0922d 8b8e88000000         mov ecx, dword ptr [esi + 0x88]
00b09233 8b7d74               mov edi, dword ptr [ebp + 0x74]
00b09236 8d86c0000000         lea eax, [esi + 0xc0]
00b0923c 8908                 mov dword ptr [eax], ecx
00b0923e 50                   push eax
00b0923f 8bcf                 mov ecx, edi
00b09241 e83ac9ffff           call 0xb05b80
00b09246 8b9684000000         mov edx, dword ptr [esi + 0x84]
00b0924c 8d86d0000000         lea eax, [esi + 0xd0]
00b09252 50                   push eax
00b09253 8bcf                 mov ecx, edi
00b09255 8910                 mov dword ptr [eax], edx
00b09257 e824fdfeff           call 0xaf8f80
00b0925c d94528               fld dword ptr [ebp + 0x28]
00b0925f d84d34               fmul dword ptr [ebp + 0x34]
00b09262 d9452c               fld dword ptr [ebp + 0x2c]
00b09265 d84d34               fmul dword ptr [ebp + 0x34]
00b09268 d95df4               fstp dword ptr [ebp - 0xc]
00b0926b d94530               fld dword ptr [ebp + 0x30]
00b0926e d84d34               fmul dword ptr [ebp + 0x34]
00b09271 d95df8               fstp dword ptr [ebp - 8]
00b09274 d94534               fld dword ptr [ebp + 0x34]
00b09277 d84d34               fmul dword ptr [ebp + 0x34]
00b0927a d95dfc               fstp dword ptr [ebp - 4]
00b0927d d80d14002301         fmul dword ptr [0x1230014]
00b09283 e8e8570f00           call 0xbfea70
00b09288 d945f4               fld dword ptr [ebp - 0xc]
00b0928b d80d14002301         fmul dword ptr [0x1230014]
00b09291 8ad8                 mov bl, al
00b09293 e8d8570f00           call 0xbfea70
00b09298 d945f8               fld dword ptr [ebp - 8]
00b0929b d80d14002301         fmul dword ptr [0x1230014]
00b092a1 884575               mov byte ptr [ebp + 0x75], al
00b092a4 e8c7570f00           call 0xbfea70
00b092a9 d945fc               fld dword ptr [ebp - 4]
00b092ac 8a4d75               mov cl, byte ptr [ebp + 0x75]
00b092af d80d14002301         fmul dword ptr [0x1230014]
00b092b5 885d7a               mov byte ptr [ebp + 0x7a], bl
00b092b8 884d79               mov byte ptr [ebp + 0x79], cl
00b092bb 884578               mov byte ptr [ebp + 0x78], al
00b092be e8ad570f00           call 0xbfea70
00b092c3 8b5514               mov edx, dword ptr [ebp + 0x14]
00b092c6 8b4d50               mov ecx, dword ptr [ebp + 0x50]
00b092c9 88457b               mov byte ptr [ebp + 0x7b], al
00b092cc 8b4578               mov eax, dword ptr [ebp + 0x78]
00b092cf 8996f4000000         mov dword ptr [esi + 0xf4], edx
00b092d5 8986d4000000         mov dword ptr [esi + 0xd4], eax
00b092db 8986d8000000         mov dword ptr [esi + 0xd8], eax
00b092e1 8986dc000000         mov dword ptr [esi + 0xdc], eax
00b092e7 668b455c             mov ax, word ptr [ebp + 0x5c]
00b092eb 8d96fc000000         lea edx, [esi + 0xfc]
00b092f1 898ef8000000         mov dword ptr [esi + 0xf8], ecx
00b092f7 52                   push edx
00b092f8 8bcf                 mov ecx, edi
00b092fa 668986f0000000       mov word ptr [esi + 0xf0], ax
00b09301 e83ac5ffff           call 0xb05840
00b09306 8b4d54               mov ecx, dword ptr [ebp + 0x54]
00b09309 c686bc00000000       mov byte ptr [esi + 0xbc], 0
00b09310 8b8630010000         mov eax, dword ptr [esi + 0x130]
00b09316 8bbe2c010000         mov edi, dword ptr [esi + 0x12c]
00b0931c 2bc7                 sub eax, edi
00b0931e c1f802               sar eax, 2
00b09321 3bc1                 cmp eax, ecx
00b09323 760a                 jbe 0xb0932f
00b09325 8b5554               mov edx, dword ptr [ebp + 0x54]
00b09328 8bcf                 mov ecx, edi
00b0932a 8b0491               mov eax, dword ptr [ecx + edx*4]
00b0932d eb03                 jmp 0xb09332
00b0932f 83c8ff               or eax, 0xffffffff
00b09332 8b4d70               mov ecx, dword ptr [ebp + 0x70]
00b09335 8b4934               mov ecx, dword ptr [ecx + 0x34]
00b09338 8b11                 mov edx, dword ptr [ecx]
00b0933a 6a00                 push 0
00b0933c 81c6ac000000         add esi, 0xac
00b09342 56                   push esi
00b09343 50                   push eax
00b09344 8b457c               mov eax, dword ptr [ebp + 0x7c]
00b09347 50                   push eax
00b09348 ff5270               call dword ptr [edx + 0x70]
00b0934b 5b                   pop ebx
00b0934c 5f                   pop edi
00b0934d 5e                   pop esi
00b0934e 83c564               add ebp, 0x64
00b09351 8be5                 mov esp, ebp
00b09353 5d                   pop ebp
00b09354 c21400               ret 0x14
