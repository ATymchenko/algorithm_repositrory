
def merge_sort(v):
    if(len(v) > 1):
        m = len(v) // 2
        left =  merge_sort(v[:m])
        right = merge_sort(v[m:])
        v = merge(left, right)

    return v