def mod_inv(m,p):
    return pow(m,-1,p)
def field_add(a,p,xP,yP,xQ,yQ):#定义O=-1
    if xP==-1 and yP==-1:
        return xQ,yQ
    elif xQ==-1 and yQ==-1:
        return xP,yP
    elif xP==xQ and yP+yQ==0:
        return -1,-1
    elif xP==xQ and yP==yQ:
        return field_PadP(a,p,xP,yP)
    else:
        lbda=((yQ-yP)*mod_inv(xQ-xP,p))%p
        #print(lbda)
        #print()
        x3=(lbda**2-xP-xQ)%p
        y3=(lbda*(xP-x3)-yP)%p
        return x3,y3
def field_sub(a,p,xP,yP,xQ,yQ):#定义O=-1
    if xP==-1 and yP==-1:
        return xQ,-yQ
    elif xQ==-1 and yQ==-1:
        return xP,yP
    elif xP==xQ and yP==yQ:
        return -1,-1
    elif xP==xQ and yP+yQ==0:
        return field_PadP(a,p,xP,yP)
    else:
        yQ=-yQ
        lbda=((yQ-yP)*mod_inv(xQ-xP,p))%p
        #print(lbda)
        #print()
        x3=(lbda**2-xP-xQ)%p
        y3=(lbda*(xP-x3)-yP)%p
        return x3,y3
def field_PadP(a,p,x,y):
    if x==-1 or y==-1:
        return -1,-1
    elif y==0:
        return -1,-1
    else:
        lbda=((3*(x**2)+a)*mod_inv(2*y,p))%p
        x3=(lbda**2-2*x)%p
        y3=(lbda*(x-x3)-y)%p
        return x3,y3
'''
def field_mul(a,p,k,x,y):#没有完整分解，但我没想到这么慢
    #考虑倍乘法：）
    if k<=1:
        return x,y
    xR,yR=x,y
    n=k.bit_length()-1
    for i in range(n):
        xR,yR=field_PadP(a,p,xR,yR)
    l=k-(k>>n<<n)
    for i in range(l):
        xR,yR=field_add(a,p,xR,yR,x,y)
    return xR,yR
'''
def field_mul_optimized(a,p,k,x,y):#完整分解的优化版本
    if k<1:
        return -1,-1
    if k==1:
        return x,y
    elif k==2:
        return field_PadP(a,p,x,y)
    elif k==3:
        return field_add(a,p,x,y,*field_PadP(a,p,x,y))
    elif k&1==0:# k/2 * P + k/2 * P
        return field_PadP(a,p,*field_mul_optimized(a,p,k>>1,x,y))
    elif k&1==1:# P + k/2 * P + k/2 * P
        return field_add(a,p,x,y,*field_PadP(a,p,*field_mul_optimized(a,p,k>>1,x,y)))
