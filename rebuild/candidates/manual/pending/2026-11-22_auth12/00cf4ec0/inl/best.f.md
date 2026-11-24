@@ r1f0 b1f0
- and eax, <A>
+ and eax, 0xfffffffe
@@ r2bd b2bd
- mov dword ptr [esp + 0x40], <A>
+ mov dword ptr [esp + <A>], <A>
@@ r343 b343
- mov dword ptr [esp + 0xa8], <A>
+ mov dword ptr [esp + <A>], <A>
@@ r435 b435
- mov dword ptr [esp + 0xc0], <A>
+ mov dword ptr [esp + <A>], <A>
@@ r524 b524
- mov dword ptr [esp + 0xb4], <A>
+ mov dword ptr [esp + <A>], <A>
@@ r5f9 b5f9
- mov dword ptr [esp + 0x38], <A>
+ mov dword ptr [esp + <A>], <A>
@@ r7e7 b7e7
- push <A>
+ push 0x40800000
@@ rfa6 bfa6
- mov dword ptr [esp + 0x40], <A>
+ mov dword ptr [esp + <A>], <A>
@@ r1288 b1288
- mov dword ptr [esp + 0x38], <A>
+ mov dword ptr [esp + <A>], <A>
@@ r12a1 b12a1
- push <A>
+ push 0x40800000
@@ r1706 b1706
- mov dword ptr [esp + 0x38], <A>
+ mov dword ptr [esp + <A>], <A>
@@ r174f b174f
- mov dword ptr [esp + 0x38], <A>
+ mov dword ptr [esp + <A>], <A>
@@ r17c0 b17c0
- cli 
- push esi
- iretd 
- add byte ptr [eax + 0x57], bh
- iretd 
- add dh, dh
- push edi
- iretd 
- add byte ptr [eax + ebx*2 - 0x31], dh
- add dl, dh
- pop eax
- iretd 
- add byte ptr [eax + 0x59], dh
- iretd 
- add dh, ch
- pop ecx
- iretd 
- add byte ptr [edx + ebx*2 - 0x31], ch
- add dl, ch
- pop edx
- iretd 
- add byte ptr [edi + 0x5b], ch
- iretd 
- add ah, bl
- pop ebx
- iretd 
- add byte ptr [ecx + 0x5c], cl
- iretd 
- add byte ptr [esi + <A>], dh
- pop ebp
- iretd 
- add byte ptr [ecx + <A>], dh
- iretd 
- add bh, ah
+ add byte ptr [eax], al
+ add byte ptr [eax], al
+ add byte ptr [eax], al
+ add byte ptr [eax], al
+ add byte ptr [eax], al
+ add byte ptr [eax], al
+ add byte ptr [eax], al
+ add byte ptr [eax], al
+ add byte ptr [eax], al
+ add byte ptr [eax], al
+ add byte ptr [eax], al
+ add byte ptr [eax], al
+ add byte ptr [eax], al
+ add byte ptr [eax], al
+ add byte ptr [eax], al
+ add byte ptr [eax], al
+ add byte ptr [eax], al
+ add byte ptr [eax], al
+ add byte ptr [eax], al
+ add byte ptr [eax], al
+ add byte ptr [eax], al
+ add byte ptr [eax], al
+ add byte ptr [eax], al
+ add byte ptr [eax], al
+ add byte ptr [eax], al
+ add byte ptr [eax], al
+ add byte ptr [eax], al
+ add byte ptr [eax], al
+ add byte ptr [eax], al
+ add byte ptr [eax], al
+ add byte ptr [eax], al
+ add byte ptr [eax], al
+ add byte ptr [eax], al
+ add byte ptr [eax], al
+ add byte ptr [eax], al
+ add byte ptr [eax], al
+ add byte ptr [eax], al
+ add byte ptr [eax], al
+ add byte ptr [eax], al
+ add byte ptr [eax], al
+ add byte ptr [eax], al
+ add byte ptr [eax], al
DIFFINS 54 len 6164 v 6164
