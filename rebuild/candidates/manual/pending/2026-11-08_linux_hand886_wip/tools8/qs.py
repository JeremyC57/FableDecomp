"""qs.py: c2.dll 13.10.3077 frame-slot sort (0x107ae0c4 / comparator 0x107ae9c2), K&R quicksort with middle pivot."""
def cmp(a, b, edi):
    fa, fb = a['flag'] & 1, b['flag'] & 1
    if fa != fb:
        if fa: return 1 if edi else -1
        return 1 if edi == 0 else -1
    da = int(a['w'] * 1000 / a['size']) if True else 0
    db = int(b['w'] * 1000 / b['size'])
    # c2: imul/cdq/idiv = truncation toward zero
    da = (abs(a['w'] * 1000) // a['size']) * (1 if a['w'] >= 0 else -1)
    db = (abs(b['w'] * 1000) // b['size']) * (1 if b['w'] >= 0 else -1)
    if da == db: return 0
    return 1 if da >= db else -1
def qsort(v, left, right, edi):
    while left < right:
        mid = (left + right)
        mid = int(mid / 2)
        v[left], v[mid] = v[mid], v[left]
        last = left
        for i in range(left + 1, right + 1):
            if cmp(v[left], v[i], edi) < 0:
                last += 1
                v[last], v[i] = v[i], v[last]
        v[left], v[last] = v[last], v[left]
        qsort(v, left, last - 1, edi)
        left = last + 1
