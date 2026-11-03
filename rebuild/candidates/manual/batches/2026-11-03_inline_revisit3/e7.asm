
work/00e7bd60/plain/00e7bd60.obj:     file format pe-i386


Disassembly of section .text:

00000000 <??0CBaseClass@@QAE@ABV0@@Z>:
   0:	mov    eax,ecx
   2:	mov    DWORD PTR [eax],0x0
			4: dir32	??_7CBaseClass@@6B@
   8:	ret    0x4

Disassembly of section .text:

00000000 <??_GCBaseClass@@UAEPAXI@Z>:
   0:	push   esi
   1:	mov    esi,ecx
   3:	call   8 <??_GCBaseClass@@UAEPAXI@Z+0x8>
			4: DISP32	??1CBaseClass@@UAE@XZ
   8:	test   BYTE PTR [esp+0x8],0x1
   d:	je     18 <??_GCBaseClass@@UAEPAXI@Z+0x18>
   f:	push   esi
  10:	call   15 <??_GCBaseClass@@UAEPAXI@Z+0x15>
			11: DISP32	??3@YAXPAX@Z
  15:	add    esp,0x4
  18:	mov    eax,esi
  1a:	pop    esi
  1b:	ret    0x4

Disassembly of section .text:

00000000 <?SpawnFunction@CV_FishermanScript@NScript@@QAEXPAVCSpawnedFuncBase@@ABVCCharString@@@Z>:
   0:	jmp    5 <@feat.00+0x4>
			1: DISP32	?AddSpawnedFunction@CScriptBase@@QAEXPAVCSpawnedFuncBase@@ABVCCharString@@@Z

Disassembly of section .text:

00000000 <??0?$CCountedPointer@VCScriptThing@@@@QAE@ABV0@@Z>:
   0:	mov    eax,ecx
   2:	mov    ecx,DWORD PTR [esp+0x4]
   6:	mov    edx,DWORD PTR [ecx]
   8:	mov    DWORD PTR [eax],edx
   a:	mov    edx,DWORD PTR [ecx+0x4]
   d:	test   edx,edx
   f:	mov    DWORD PTR [eax+0x4],edx
  12:	je     16 <??0?$CCountedPointer@VCScriptThing@@@@QAE@ABV0@@Z+0x16>
  14:	inc    DWORD PTR [edx]
  16:	ret    0x4

Disassembly of section .text:

00000000 <??1?$CCountedPointer@VCScriptThing@@@@QAE@XZ>:
   0:	push   esi
   1:	mov    esi,ecx
   3:	mov    eax,DWORD PTR [esi+0x4]
   6:	test   eax,eax
   8:	je     26 <??1?$CCountedPointer@VCScriptThing@@@@QAE@XZ+0x26>
   a:	dec    DWORD PTR [eax]
   c:	mov    eax,DWORD PTR [esi+0x4]
   f:	cmp    DWORD PTR [eax],0x0
  12:	jne    26 <??1?$CCountedPointer@VCScriptThing@@@@QAE@XZ+0x26>
  14:	mov    ecx,DWORD PTR [eax+0x8]
  17:	call   DWORD PTR [eax+0x4]
  1a:	mov    eax,DWORD PTR [esi+0x4]
  1d:	push   eax
  1e:	call   23 <??1?$CCountedPointer@VCScriptThing@@@@QAE@XZ+0x23>
			1f: DISP32	??3@YAXPAX@Z
  23:	add    esp,0x4
  26:	mov    DWORD PTR [esi],0x0
  2c:	mov    DWORD PTR [esi+0x4],0x0
  33:	pop    esi
  34:	ret

Disassembly of section .text:

00000000 <??1?$CCountedPointer@$$CBVCPredicateBase@NScript@@@@QAE@XZ>:
   0:	push   esi
   1:	mov    esi,ecx
   3:	mov    eax,DWORD PTR [esi+0x4]
   6:	test   eax,eax
   8:	je     26 <??1?$CCountedPointer@$$CBVCPredicateBase@NScript@@@@QAE@XZ+0x26>
   a:	dec    DWORD PTR [eax]
   c:	mov    eax,DWORD PTR [esi+0x4]
   f:	cmp    DWORD PTR [eax],0x0
  12:	jne    26 <??1?$CCountedPointer@$$CBVCPredicateBase@NScript@@@@QAE@XZ+0x26>
  14:	mov    ecx,DWORD PTR [eax+0x8]
  17:	call   DWORD PTR [eax+0x4]
  1a:	mov    eax,DWORD PTR [esi+0x4]
  1d:	push   eax
  1e:	call   23 <??1?$CCountedPointer@$$CBVCPredicateBase@NScript@@@@QAE@XZ+0x23>
			1f: DISP32	??3@YAXPAX@Z
  23:	add    esp,0x4
  26:	mov    DWORD PTR [esi],0x0
  2c:	mov    DWORD PTR [esi+0x4],0x0
  33:	pop    esi
  34:	ret

Disassembly of section .text:

00000000 <??0CScriptThing@@QAE@ABV0@@Z>:
   0:	mov    eax,ecx
   2:	mov    ecx,DWORD PTR [esp+0x4]
   6:	mov    DWORD PTR [eax],0x0
			8: dir32	??_7CScriptThing@@6B@
   c:	mov    edx,DWORD PTR [ecx+0x4]
   f:	mov    DWORD PTR [eax+0x4],edx
  12:	mov    ecx,DWORD PTR [ecx+0x8]
  15:	test   ecx,ecx
  17:	mov    DWORD PTR [eax+0x8],ecx
  1a:	je     1e <??0CScriptThing@@QAE@ABV0@@Z+0x1e>
  1c:	inc    DWORD PTR [ecx]
  1e:	ret    0x4

Disassembly of section .text:

00000000 <??1CScriptThing@@UAE@XZ>:
   0:	push   esi
   1:	mov    esi,ecx
   3:	mov    DWORD PTR [esi],0x0
			5: dir32	??_7CScriptThing@@6B@
   9:	mov    eax,DWORD PTR [esi+0x8]
   c:	test   eax,eax
   e:	je     2c <??1CScriptThing@@UAE@XZ+0x2c>
  10:	dec    DWORD PTR [eax]
  12:	mov    eax,DWORD PTR [esi+0x8]
  15:	cmp    DWORD PTR [eax],0x0
  18:	jne    2c <??1CScriptThing@@UAE@XZ+0x2c>
  1a:	mov    ecx,DWORD PTR [eax+0x8]
  1d:	call   DWORD PTR [eax+0x4]
  20:	mov    eax,DWORD PTR [esi+0x8]
  23:	push   eax
  24:	call   29 <??1CScriptThing@@UAE@XZ+0x29>
			25: DISP32	??3@YAXPAX@Z
  29:	add    esp,0x4
  2c:	mov    DWORD PTR [esi+0x4],0x0
  33:	mov    DWORD PTR [esi+0x8],0x0
  3a:	mov    ecx,esi
  3c:	pop    esi
  3d:	jmp    42 <@feat.00+0x41>
			3e: DISP32	??1CBaseClass@@UAE@XZ

Disassembly of section .text:

00000000 <??1CSpawnedFuncBase@@UAE@XZ>:
   0:	push   esi
   1:	mov    esi,ecx
   3:	mov    eax,DWORD PTR [esi+0x30]
   6:	test   eax,eax
   8:	je     26 <??1CSpawnedFuncBase@@UAE@XZ+0x26>
   a:	dec    DWORD PTR [eax]
   c:	mov    eax,DWORD PTR [esi+0x30]
   f:	cmp    DWORD PTR [eax],0x0
  12:	jne    26 <??1CSpawnedFuncBase@@UAE@XZ+0x26>
  14:	mov    ecx,DWORD PTR [eax+0x8]
  17:	call   DWORD PTR [eax+0x4]
  1a:	mov    eax,DWORD PTR [esi+0x30]
  1d:	push   eax
  1e:	call   23 <??1CSpawnedFuncBase@@UAE@XZ+0x23>
			1f: DISP32	??3@YAXPAX@Z
  23:	add    esp,0x4
  26:	lea    ecx,[esi+0x24]
  29:	mov    DWORD PTR [esi+0x2c],0x0
  30:	mov    DWORD PTR [esi+0x30],0x0
  37:	call   3c <??1CSpawnedFuncBase@@UAE@XZ+0x3c>
			38: DISP32	??1CCharString@@QAE@XZ
  3c:	lea    ecx,[esi+0x20]
  3f:	call   44 <??1CSpawnedFuncBase@@UAE@XZ+0x44>
			40: DISP32	??1CCharString@@QAE@XZ
  44:	mov    ecx,esi
  46:	pop    esi
  47:	jmp    4c <@feat.00+0x4b>
			48: DISP32	??1CSuspendableProcessMicrothread@@MAE@XZ

Disassembly of section .text:

00000000 <??_GCScriptThing@@UAEPAXI@Z>:
   0:	push   esi
   1:	mov    esi,ecx
   3:	mov    DWORD PTR [esi],0x0
			5: dir32	??_7CScriptThing@@6B@
   9:	mov    eax,DWORD PTR [esi+0x8]
   c:	test   eax,eax
   e:	je     2c <??_GCScriptThing@@UAEPAXI@Z+0x2c>
  10:	dec    DWORD PTR [eax]
  12:	mov    eax,DWORD PTR [esi+0x8]
  15:	cmp    DWORD PTR [eax],0x0
  18:	jne    2c <??_GCScriptThing@@UAEPAXI@Z+0x2c>
  1a:	mov    ecx,DWORD PTR [eax+0x8]
  1d:	call   DWORD PTR [eax+0x4]
  20:	mov    eax,DWORD PTR [esi+0x8]
  23:	push   eax
  24:	call   29 <??_GCScriptThing@@UAEPAXI@Z+0x29>
			25: DISP32	??3@YAXPAX@Z
  29:	add    esp,0x4
  2c:	mov    ecx,esi
  2e:	mov    DWORD PTR [esi+0x4],0x0
  35:	mov    DWORD PTR [esi+0x8],0x0
  3c:	call   41 <??_GCScriptThing@@UAEPAXI@Z+0x41>
			3d: DISP32	??1CBaseClass@@UAE@XZ
  41:	test   BYTE PTR [esp+0x8],0x1
  46:	je     51 <??_GCScriptThing@@UAEPAXI@Z+0x51>
  48:	push   esi
  49:	call   4e <??_GCScriptThing@@UAEPAXI@Z+0x4e>
			4a: DISP32	??3@YAXPAX@Z
  4e:	add    esp,0x4
  51:	mov    eax,esi
  53:	pop    esi
  54:	ret    0x4

Disassembly of section .text:

00000000 <??0?$CSpawnedFunc1@VCV_FishermanScript@NScript@@VCScriptThing@@@@QAE@ABVCCharString@@KPAVCV_FishermanScript@NScript@@P823@AEXVCScriptThing@@@Z2@Z>:
   0:	mov    eax,DWORD PTR [esp+0x8]
   4:	push   esi
   5:	mov    esi,ecx
   7:	mov    ecx,DWORD PTR [esp+0x8]
   b:	push   eax
   c:	push   ecx
   d:	mov    ecx,esi
   f:	call   14 <??0?$CSpawnedFunc1@VCV_FishermanScript@NScript@@VCScriptThing@@@@QAE@ABVCCharString@@KPAVCV_FishermanScript@NScript@@P823@AEXVCScriptThing@@@Z2@Z+0x14>
			10: DISP32	??0CSpawnedFuncBase@@QAE@ABVCCharString@@K@Z
  14:	mov    eax,DWORD PTR [esp+0x10]
  18:	mov    edx,DWORD PTR [esp+0x14]
  1c:	mov    DWORD PTR [esi+0x38],eax
  1f:	mov    eax,DWORD PTR [esp+0x20]
  23:	test   eax,eax
  25:	mov    DWORD PTR [esi+0x34],edx
  28:	mov    edx,DWORD PTR [esp+0x1c]
  2c:	mov    DWORD PTR [esi],0x0
			2e: dir32	??_7?$CSpawnedFunc1@VCV_FishermanScript@NScript@@VCScriptThing@@@@6B@
  32:	mov    ecx,0x0
			33: dir32	??_7CScriptThing@@6B@
  37:	mov    DWORD PTR [esi+0x3c],ecx
  3a:	mov    DWORD PTR [esi+0x40],edx
  3d:	mov    DWORD PTR [esi+0x44],eax
  40:	je     48 <??0?$CSpawnedFunc1@VCV_FishermanScript@NScript@@VCScriptThing@@@@QAE@ABVCCharString@@KPAVCV_FishermanScript@NScript@@P823@AEXVCScriptThing@@@Z2@Z+0x48>
  42:	inc    DWORD PTR [eax]
  44:	mov    eax,DWORD PTR [esp+0x20]
  48:	test   eax,eax
  4a:	mov    DWORD PTR [esp+0x18],ecx
  4e:	je     6e <??0?$CSpawnedFunc1@VCV_FishermanScript@NScript@@VCScriptThing@@@@QAE@ABVCCharString@@KPAVCV_FishermanScript@NScript@@P823@AEXVCScriptThing@@@Z2@Z+0x6e>
  50:	dec    DWORD PTR [eax]
  52:	mov    eax,DWORD PTR [esp+0x20]
  56:	cmp    DWORD PTR [eax],0x0
  59:	jne    6e <??0?$CSpawnedFunc1@VCV_FishermanScript@NScript@@VCScriptThing@@@@QAE@ABVCCharString@@KPAVCV_FishermanScript@NScript@@P823@AEXVCScriptThing@@@Z2@Z+0x6e>
  5b:	mov    ecx,DWORD PTR [eax+0x8]
  5e:	call   DWORD PTR [eax+0x4]
  61:	mov    eax,DWORD PTR [esp+0x20]
  65:	push   eax
  66:	call   6b <??0?$CSpawnedFunc1@VCV_FishermanScript@NScript@@VCScriptThing@@@@QAE@ABVCCharString@@KPAVCV_FishermanScript@NScript@@P823@AEXVCScriptThing@@@Z2@Z+0x6b>
			67: DISP32	??3@YAXPAX@Z
  6b:	add    esp,0x4
  6e:	lea    ecx,[esp+0x18]
  72:	mov    DWORD PTR [esp+0x1c],0x0
  7a:	mov    DWORD PTR [esp+0x20],0x0
  82:	call   87 <??0?$CSpawnedFunc1@VCV_FishermanScript@NScript@@VCScriptThing@@@@QAE@ABVCCharString@@KPAVCV_FishermanScript@NScript@@P823@AEXVCScriptThing@@@Z2@Z+0x87>
			83: DISP32	??1CBaseClass@@UAE@XZ
  87:	mov    eax,esi
  89:	pop    esi
  8a:	ret    0x1c

Disassembly of section .text:

00000000 <?SuspendableProcess@?$CSpawnedFunc1@VCV_FishermanScript@NScript@@VCScriptThing@@@@UAEXXZ>:
   0:	sub    esp,0xc
   3:	mov    eax,esp
   5:	mov    DWORD PTR [eax],0x0
			7: dir32	??_7CBaseClass@@6B@
   b:	mov    DWORD PTR [eax],0x0
			d: dir32	??_7CScriptThing@@6B@
  11:	mov    edx,ecx
  13:	mov    ecx,DWORD PTR [edx+0x40]
  16:	mov    DWORD PTR [eax+0x4],ecx
  19:	mov    ecx,DWORD PTR [edx+0x44]
  1c:	test   ecx,ecx
  1e:	mov    DWORD PTR [eax+0x8],ecx
  21:	je     25 <?SuspendableProcess@?$CSpawnedFunc1@VCV_FishermanScript@NScript@@VCScriptThing@@@@UAEXXZ+0x25>
  23:	inc    DWORD PTR [ecx]
  25:	mov    ecx,DWORD PTR [edx+0x38]
  28:	call   DWORD PTR [edx+0x34]
  2b:	ret

Disassembly of section .text:

00000000 <??_G?$CSpawnedFunc1@VCV_FishermanScript@NScript@@VCScriptThing@@@@UAEPAXI@Z>:
   0:	push   esi
   1:	mov    esi,ecx
   3:	lea    ecx,[esi+0x3c]
   6:	call   b <??_G?$CSpawnedFunc1@VCV_FishermanScript@NScript@@VCScriptThing@@@@UAEPAXI@Z+0xb>
			7: DISP32	??1CScriptThing@@UAE@XZ
   b:	mov    ecx,esi
   d:	call   12 <??_G?$CSpawnedFunc1@VCV_FishermanScript@NScript@@VCScriptThing@@@@UAEPAXI@Z+0x12>
			e: DISP32	??1CSpawnedFuncBase@@UAE@XZ
  12:	test   BYTE PTR [esp+0x8],0x1
  17:	je     22 <??_G?$CSpawnedFunc1@VCV_FishermanScript@NScript@@VCScriptThing@@@@UAEPAXI@Z+0x22>
  19:	push   esi
  1a:	call   1f <??_G?$CSpawnedFunc1@VCV_FishermanScript@NScript@@VCScriptThing@@@@UAEPAXI@Z+0x1f>
			1b: DISP32	??3@YAXPAX@Z
  1f:	add    esp,0x4
  22:	mov    eax,esi
  24:	pop    esi
  25:	ret    0x4

Disassembly of section .text:

00000000 <?Init@CFisherman@CV_FishermanScript@NScript@@EAEXXZ>:
   0:	sub    esp,0x20
   3:	push   ebx
   4:	push   ebp
   5:	push   esi
   6:	push   edi
   7:	mov    esi,ecx
   9:	mov    eax,DWORD PTR [esi+0x14]
   c:	xor    ebx,ebx
   e:	push   0x1
  10:	mov    BYTE PTR [eax+0x50],bl
  13:	mov    ecx,DWORD PTR [esi+0x4]
  16:	mov    edx,DWORD PTR [ecx]
  18:	push   ebx
  19:	lea    edi,[esi+0x8]
  1c:	push   edi
  1d:	mov    DWORD PTR [esp+0x20],ebx
  21:	call   DWORD PTR [edx+0x814]
  27:	mov    eax,DWORD PTR [esi+0x14]
  2a:	cmp    BYTE PTR [eax+0x49],bl
  2d:	jne    1b0 <?Init@CFisherman@CV_FishermanScript@NScript@@EAEXXZ+0x1b0>
  33:	push   0xffffffff
  35:	push   0x0
			36: dir32	??_C@_0N@BNENPPLG@MK_FI_FISHER?$AA@
  3a:	lea    ecx,[esp+0x18]
  3e:	call   43 <?Init@CFisherman@CV_FishermanScript@NScript@@EAEXXZ+0x43>
			3f: DISP32	??0CCharString@@QAE@PBDJ@Z
  43:	mov    ecx,DWORD PTR [esi+0x4]
  46:	mov    eax,DWORD PTR [ecx]
  48:	mov    edx,ecx
  4a:	mov    ebp,DWORD PTR [edx]
  4c:	push   ebx
  4d:	lea    edx,[esp+0x14]
  51:	push   edx
  52:	lea    edx,[esp+0x2c]
  56:	push   edx
  57:	call   DWORD PTR [eax+0x120]
  5d:	mov    ecx,DWORD PTR [esi+0x4]
  60:	push   eax
  61:	push   edi
  62:	call   DWORD PTR [ebp+0x760]
  68:	mov    eax,DWORD PTR [esp+0x2c]
  6c:	cmp    eax,ebx
  6e:	mov    DWORD PTR [esp+0x24],0x0
			72: dir32	??_7CScriptThing@@6B@
  76:	je     95 <?Init@CFisherman@CV_FishermanScript@NScript@@EAEXXZ+0x95>
  78:	dec    DWORD PTR [eax]
  7a:	mov    eax,DWORD PTR [esp+0x2c]
  7e:	cmp    DWORD PTR [eax],ebx
  80:	jne    95 <?Init@CFisherman@CV_FishermanScript@NScript@@EAEXXZ+0x95>
  82:	mov    ecx,DWORD PTR [eax+0x8]
  85:	call   DWORD PTR [eax+0x4]
  88:	mov    eax,DWORD PTR [esp+0x2c]
  8c:	push   eax
  8d:	call   92 <?Init@CFisherman@CV_FishermanScript@NScript@@EAEXXZ+0x92>
			8e: DISP32	??3@YAXPAX@Z
  92:	add    esp,0x4
  95:	lea    ecx,[esp+0x24]
  99:	mov    DWORD PTR [esp+0x28],ebx
  9d:	mov    DWORD PTR [esp+0x2c],ebx
  a1:	call   a6 <?Init@CFisherman@CV_FishermanScript@NScript@@EAEXXZ+0xa6>
			a2: DISP32	??1CBaseClass@@UAE@XZ
  a6:	lea    ecx,[esp+0x10]
  aa:	call   af <?Init@CFisherman@CV_FishermanScript@NScript@@EAEXXZ+0xaf>
			ab: DISP32	??1CCharString@@QAE@XZ
  af:	push   0xffffffff
  b1:	push   0x0
			b2: dir32	??_C@_0N@BNENPPLG@MK_FI_FISHER?$AA@
  b6:	lea    ecx,[esp+0x18]
  ba:	call   bf <?Init@CFisherman@CV_FishermanScript@NScript@@EAEXXZ+0xbf>
			bb: DISP32	??0CCharString@@QAE@PBDJ@Z
  bf:	mov    ecx,DWORD PTR [esi+0x4]
  c2:	mov    edx,DWORD PTR [ecx]
  c4:	lea    eax,[esp+0x10]
  c8:	push   eax
  c9:	lea    eax,[esp+0x28]
  cd:	push   eax
  ce:	call   DWORD PTR [edx+0x120]
  d4:	mov    ecx,DWORD PTR [esi+0x4]
  d7:	mov    ebp,DWORD PTR [ecx]
  d9:	mov    edx,DWORD PTR [eax]
  db:	push   0x1
  dd:	mov    ecx,eax
  df:	call   DWORD PTR [edx+0x28]
  e2:	push   ecx
  e3:	mov    ecx,DWORD PTR [esi+0x4]
  e6:	fstp   DWORD PTR [esp]
  e9:	push   edi
  ea:	call   DWORD PTR [ebp+0x768]
  f0:	mov    eax,DWORD PTR [esp+0x2c]
  f4:	cmp    eax,ebx
  f6:	mov    ebp,0x0
			f7: dir32	??_7CScriptThing@@6B@
  fb:	mov    DWORD PTR [esp+0x24],ebp
  ff:	je     11e <?Init@CFisherman@CV_FishermanScript@NScript@@EAEXXZ+0x11e>
 101:	dec    DWORD PTR [eax]
 103:	mov    eax,DWORD PTR [esp+0x2c]
 107:	cmp    DWORD PTR [eax],ebx
 109:	jne    11e <?Init@CFisherman@CV_FishermanScript@NScript@@EAEXXZ+0x11e>
 10b:	mov    ecx,DWORD PTR [eax+0x8]
 10e:	call   DWORD PTR [eax+0x4]
 111:	mov    eax,DWORD PTR [esp+0x2c]
 115:	push   eax
 116:	call   11b <?Init@CFisherman@CV_FishermanScript@NScript@@EAEXXZ+0x11b>
			117: DISP32	??3@YAXPAX@Z
 11b:	add    esp,0x4
 11e:	lea    ecx,[esp+0x24]
 122:	mov    DWORD PTR [esp+0x28],ebx
 126:	mov    DWORD PTR [esp+0x2c],ebx
 12a:	call   12f <?Init@CFisherman@CV_FishermanScript@NScript@@EAEXXZ+0x12f>
			12b: DISP32	??1CBaseClass@@UAE@XZ
 12f:	lea    ecx,[esp+0x10]
 133:	call   138 <?Init@CFisherman@CV_FishermanScript@NScript@@EAEXXZ+0x138>
			134: DISP32	??1CCharString@@QAE@XZ
 138:	push   0xffffffff
 13a:	push   0x0
			13b: dir32	??_C@_0BB@COLLABPK@FishermanHutDoor?$AA@
 13f:	lea    ecx,[esp+0x18]
 143:	call   148 <?Init@CFisherman@CV_FishermanScript@NScript@@EAEXXZ+0x148>
			144: DISP32	??0CCharString@@QAE@PBDJ@Z
 148:	mov    ecx,DWORD PTR [esi+0x4]
 14b:	mov    eax,DWORD PTR [ecx]
 14d:	mov    edx,ecx
 14f:	mov    edi,DWORD PTR [edx]
 151:	lea    edx,[esp+0x10]
 155:	push   edx
 156:	lea    edx,[esp+0x28]
 15a:	push   edx
 15b:	call   DWORD PTR [eax+0x120]
 161:	mov    ecx,DWORD PTR [esi+0x4]
 164:	push   eax
 165:	call   DWORD PTR [edi+0x6a8]
 16b:	mov    eax,DWORD PTR [esp+0x2c]
 16f:	cmp    eax,ebx
 171:	mov    DWORD PTR [esp+0x24],ebp
 175:	je     194 <?Init@CFisherman@CV_FishermanScript@NScript@@EAEXXZ+0x194>
 177:	dec    DWORD PTR [eax]
 179:	mov    eax,DWORD PTR [esp+0x2c]
 17d:	cmp    DWORD PTR [eax],ebx
 17f:	jne    194 <?Init@CFisherman@CV_FishermanScript@NScript@@EAEXXZ+0x194>
 181:	mov    ecx,DWORD PTR [eax+0x8]
 184:	call   DWORD PTR [eax+0x4]
 187:	mov    eax,DWORD PTR [esp+0x2c]
 18b:	push   eax
 18c:	call   191 <?Init@CFisherman@CV_FishermanScript@NScript@@EAEXXZ+0x191>
			18d: DISP32	??3@YAXPAX@Z
 191:	add    esp,0x4
 194:	lea    ecx,[esp+0x24]
 198:	mov    DWORD PTR [esp+0x28],ebx
 19c:	mov    DWORD PTR [esp+0x2c],ebx
 1a0:	call   1a5 <?Init@CFisherman@CV_FishermanScript@NScript@@EAEXXZ+0x1a5>
			1a1: DISP32	??1CBaseClass@@UAE@XZ
 1a5:	lea    ecx,[esp+0x10]
 1a9:	call   1ae <?Init@CFisherman@CV_FishermanScript@NScript@@EAEXXZ+0x1ae>
			1aa: DISP32	??1CCharString@@QAE@XZ
 1ae:	jmp    1b5 <?Init@CFisherman@CV_FishermanScript@NScript@@EAEXXZ+0x1b5>
 1b0:	mov    ebp,0x0
			1b1: dir32	??_7CScriptThing@@6B@
 1b5:	push   0x48
 1b7:	call   1bc <?Init@CFisherman@CV_FishermanScript@NScript@@EAEXXZ+0x1bc>
			1b8: DISP32	??2@YAPAXI@Z
 1bc:	mov    edi,eax
 1be:	add    esp,0x4
 1c1:	cmp    edi,ebx
 1c3:	je     28d <?Init@CFisherman@CV_FishermanScript@NScript@@EAEXXZ+0x28d>
 1c9:	mov    eax,DWORD PTR [esi+0x10]
 1cc:	cmp    eax,ebx
 1ce:	mov    ecx,DWORD PTR [esi+0xc]
 1d1:	mov    DWORD PTR [esp+0x14],0x7
 1d9:	mov    DWORD PTR [esp+0x24],ebp
 1dd:	mov    DWORD PTR [esp+0x28],ecx
 1e1:	mov    DWORD PTR [esp+0x2c],eax
 1e5:	je     1e9 <?Init@CFisherman@CV_FishermanScript@NScript@@EAEXXZ+0x1e9>
 1e7:	inc    DWORD PTR [eax]
 1e9:	mov    ebp,DWORD PTR [esi+0x14]
 1ec:	push   0xffffffff
 1ee:	push   0x0
			1ef: dir32	??_C@_0BD@CBJOLHGK@NPCAttitudeProcess?$AA@
 1f3:	lea    ecx,[esp+0x28]
 1f7:	call   1fc <?Init@CFisherman@CV_FishermanScript@NScript@@EAEXXZ+0x1fc>
			1f8: DISP32	??0CCharString@@QAE@PBDJ@Z
 1fc:	push   eax
 1fd:	push   0xffffffff
 1ff:	push   0x0
			200: dir32	??_C@_0N@ONHAGBFO@ParentClass?4?$AA@
 204:	lea    ecx,[esp+0x28]
 208:	call   20d <?Init@CFisherman@CV_FishermanScript@NScript@@EAEXXZ+0x20d>
			209: DISP32	??0CCharString@@QAE@PBDJ@Z
 20d:	mov    edx,eax
 20f:	lea    ecx,[esp+0x1c]
 213:	call   218 <?Init@CFisherman@CV_FishermanScript@NScript@@EAEXXZ+0x218>
			214: DISP32	??H@YI?AVCCharString@@ABV0@0@Z
 218:	push   ebx
 219:	push   eax
 21a:	mov    ecx,edi
 21c:	call   221 <?Init@CFisherman@CV_FishermanScript@NScript@@EAEXXZ+0x221>
			21d: DISP32	??0CSpawnedFuncBase@@QAE@ABVCCharString@@K@Z
 221:	mov    DWORD PTR [edi],0x0
			223: dir32	??_7?$CSpawnedFunc1@VCV_FishermanScript@NScript@@VCScriptThing@@@@6B@
 227:	mov    DWORD PTR [edi+0x34],0x0
			22a: dir32	?NPCAttitudeProcess@CV_FishermanScript@NScript@@QAEXVCScriptThing@@@Z
 22e:	mov    DWORD PTR [edi+0x38],ebp
 231:	mov    ecx,0x0
			232: dir32	??_7CScriptThing@@6B@
 236:	mov    DWORD PTR [edi+0x3c],ecx
 239:	mov    edx,DWORD PTR [esp+0x28]
 23d:	mov    DWORD PTR [edi+0x40],edx
 240:	mov    eax,DWORD PTR [esp+0x2c]
 244:	mov    DWORD PTR [edi+0x44],eax
 247:	mov    eax,DWORD PTR [esp+0x2c]
 24b:	cmp    eax,ebx
 24d:	je     255 <?Init@CFisherman@CV_FishermanScript@NScript@@EAEXXZ+0x255>
 24f:	inc    DWORD PTR [eax]
 251:	mov    eax,DWORD PTR [esp+0x2c]
 255:	cmp    eax,ebx
 257:	mov    DWORD PTR [esp+0x24],ecx
 25b:	je     27a <?Init@CFisherman@CV_FishermanScript@NScript@@EAEXXZ+0x27a>
 25d:	dec    DWORD PTR [eax]
 25f:	mov    eax,DWORD PTR [esp+0x2c]
 263:	cmp    DWORD PTR [eax],ebx
 265:	jne    27a <?Init@CFisherman@CV_FishermanScript@NScript@@EAEXXZ+0x27a>
 267:	mov    ecx,DWORD PTR [eax+0x8]
 26a:	call   DWORD PTR [eax+0x4]
 26d:	mov    ecx,DWORD PTR [esp+0x2c]
 271:	push   ecx
 272:	call   277 <?Init@CFisherman@CV_FishermanScript@NScript@@EAEXXZ+0x277>
			273: DISP32	??3@YAXPAX@Z
 277:	add    esp,0x4
 27a:	lea    ecx,[esp+0x24]
 27e:	mov    DWORD PTR [esp+0x28],ebx
 282:	mov    DWORD PTR [esp+0x2c],ebx
 286:	call   28b <?Init@CFisherman@CV_FishermanScript@NScript@@EAEXXZ+0x28b>
			287: DISP32	??1CBaseClass@@UAE@XZ
 28b:	jmp    28f <?Init@CFisherman@CV_FishermanScript@NScript@@EAEXXZ+0x28f>
 28d:	xor    edi,edi
 28f:	push   0xffffffff
 291:	push   0x0
			292: dir32	??_C@_00CNPNBAHC@?$AA@
 296:	lea    ecx,[esp+0x18]
 29a:	call   29f <?Init@CFisherman@CV_FishermanScript@NScript@@EAEXXZ+0x29f>
			29b: DISP32	??0CCharString@@QAE@PBDJ@Z
 29f:	mov    ecx,DWORD PTR [esi+0x14]
 2a2:	push   eax
 2a3:	push   edi
 2a4:	call   2a9 <?Init@CFisherman@CV_FishermanScript@NScript@@EAEXXZ+0x2a9>
			2a5: DISP32	?AddSpawnedFunction@CScriptBase@@QAEXPAVCSpawnedFuncBase@@ABVCCharString@@@Z
 2a9:	lea    ecx,[esp+0x10]
 2ad:	call   2b2 <?Init@CFisherman@CV_FishermanScript@NScript@@EAEXXZ+0x2b2>
			2ae: DISP32	??1CCharString@@QAE@XZ
 2b2:	mov    ebx,DWORD PTR [esp+0x14]
 2b6:	test   bl,0x4
 2b9:	je     2c7 <?Init@CFisherman@CV_FishermanScript@NScript@@EAEXXZ+0x2c7>
 2bb:	lea    ecx,[esp+0x18]
 2bf:	and    ebx,0xfffffffb
 2c2:	call   2c7 <?Init@CFisherman@CV_FishermanScript@NScript@@EAEXXZ+0x2c7>
			2c3: DISP32	??1CCharString@@QAE@XZ
 2c7:	test   bl,0x2
 2ca:	je     2d8 <?Init@CFisherman@CV_FishermanScript@NScript@@EAEXXZ+0x2d8>
 2cc:	lea    ecx,[esp+0x1c]
 2d0:	and    ebx,0xfffffffd
 2d3:	call   2d8 <?Init@CFisherman@CV_FishermanScript@NScript@@EAEXXZ+0x2d8>
			2d4: DISP32	??1CCharString@@QAE@XZ
 2d8:	test   bl,0x1
 2db:	je     2e6 <?Init@CFisherman@CV_FishermanScript@NScript@@EAEXXZ+0x2e6>
 2dd:	lea    ecx,[esp+0x20]
 2e1:	call   2e6 <?Init@CFisherman@CV_FishermanScript@NScript@@EAEXXZ+0x2e6>
			2e2: DISP32	??1CCharString@@QAE@XZ
 2e6:	pop    edi
 2e7:	pop    esi
 2e8:	pop    ebp
 2e9:	pop    ebx
 2ea:	add    esp,0x20
 2ed:	ret
